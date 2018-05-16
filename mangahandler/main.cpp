#include <iostream>
#include <fstream>
#include <vector>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <fcntl.h>         // open
#include <unistd.h>        // close
#include <sys/stat.h>      // fstat
#include <sys/types.h>     // fstat
#include <uuid/uuid.h>
#include <chrono>
#include "includes/json.hpp"
#include "restclient-cpp/restclient.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <unicode/unistr.h>
#include <unicode/ustream.h>
#include <experimental/filesystem>
#include "includes/scanlation.hpp"

using std::cout; using std::endl; using std::string;
using icu::UnicodeString; namespace fs = std::experimental::filesystem;

string  stringreplace(string istr, string target, string sub){
    size_t index = 0;
    cout << "in string replace" << endl;
    while (true){
        index = istr.find(target, index);
        cout << "index in stringreplace: " << index << endl;
        if (index == std::string::npos) break;
        cout << "found string: " << target << " replacing with: " << sub <<  endl;
        istr.replace(index, target.length(), sub);
        cout << "new string: " << istr << endl;
        index += sub.length();
    }
    return istr;
}


int createDirectory(UnicodeString workdir, UnicodeString subdir){

    UnicodeString command("mkdir -p " +  workdir + "/" + subdir);
    string comstring;
    command.toUTF8String(comstring);
    const int de = system(comstring.c_str());
    if (de != 0){
        cout << "dir error: "<< de << "- "  << comstring << endl;
        return -1;
    }

    return 0;
}

int unziprar(UnicodeString rawdir){
    // find file and unzip
    cout << "rawdir in unziprar: " << rawdir << endl;
    string path(""), zip(".zip"), rar(".rar");
    rawdir.toUTF8String(path);
    string zipcommand("unzip '{file}'  '*[.jpg,.jpeg,.JPEG,.png,.PNG]' -d {outdir}; find {outdir} -mindepth 2 -type f -print -exec mv {} {outdir} \\;");
    string rarcommand("unrar x '{file}' '*.jpg' '*.jpeg' '*.JPEG' '*.png' '*.PNG' {outdir}; find {outdir} -mindepth 2 -type f -print -exec mv {} {outdir} \\;");
    size_t index;
    for (const auto& p : fs::recursive_directory_iterator(path)){
        cout << p.path() << endl;
        if (p.path().extension() == rar){
            
            rarcommand = stringreplace(rarcommand, "{file}", p.path());
            rarcommand = stringreplace(rarcommand, "{outdir}", p.path().parent_path());
            cout << "rarcommand is: " << rarcommand << endl;
            const int de = system(rarcommand.c_str());
            cout << "rar command: " << rarcommand.c_str() << endl;
            std::ofstream ofs;
            ofs.open(p.path().parent_path().string() + "/uploadtransfer.log", std::ofstream::out | std::ofstream::app);
            ofs << rarcommand << endl;
            ofs.close();
            if (de !=0){
                cout << "unrar error: " << de << "- " << p.path() << endl;
                return -1;
            }
        }
        else if(p.path().extension() == zip){
            zipcommand = stringreplace(zipcommand, "{file}", p.path());
            zipcommand = stringreplace(zipcommand, "{outdir}", p.path().parent_path());
            cout << "zip command is: " << zipcommand << endl;
            std::ofstream ofs;
            ofs.open(p.path().parent_path().string() + "/uploadtransfer.log", std::ofstream::out | std::ofstream::app);
            ofs << zipcommand << endl;
            ofs.close();
            const int de = system(zipcommand.c_str());
            if (de !=0){
                cout << "unzip error: " << de << "- " << p.path() << endl;
                return -1;
            }
        }
    }
    return 0;
}

bool isImage(string extension){
    if (extension == ".jpg" ||
        extension == ".JPG" ||
        extension == ".png" ||
        extension == ".PNG" ||
        extension == ".jpeg" ||
        extension == ".JPEG")
        {
        return true;
        }
    return false;
}

int main (int argc, char** argv){
    std::vector<string> languages = {"ar",
                                "zh-CN",
                                "zh-TW",
                                "en",
                                "fr",
                                "de",
                                "it",
                                "ja",
                                "ko",
                                "pt",
                                "ru",
                                "es"
                                };
    string worklang, callback;
    uuid_t workid;
    char idstr[37] = {0};
    UnicodeString filename;
    bool     move = false,
            scanlate = false,
            translate = false,
            unzip = false,
            tess = false;
    UnicodeString usrcpath, workdir, token;
    json j = json::object();

    for (int i=0;i<argc;i++){
        cout << argv[i] << endl;
        if(UnicodeString(argv[i]) == "-i"){
            cout << "Source Path: " << argv[++i] << endl;
            usrcpath = UnicodeString(argv[i]);
        }
        else if(UnicodeString(argv[i]) == "-d"){
            cout << "Working directory: " << argv[++i] << endl;
            workdir = UnicodeString(argv[i]);
        }
        else if(UnicodeString(argv[i]) == "-m"){
            cout << "Moving to directory: " << argv[++i] << endl;
            workdir = UnicodeString(argv[i]);
            move = true;
        }
        else if(UnicodeString(argv[i]) == "-f"){
            filename = UnicodeString(argv[++i]);
            cout << "Filename: " << filename << endl;
        }
        else if(UnicodeString(argv[i]) == "-s"){
            cout << "Will scanlate." << endl;
            scanlate = true;
        }
        else if(UnicodeString(argv[i]) == "-t"){
            cout << "Will translate." << endl;
            translate = true;
        }
        else if(UnicodeString(argv[i]) == "-u"){
            cout << "Will Unzip." << endl;
            unzip = true;
        }
        else if(UnicodeString(argv[i]) == "--token"){
            cout << "Token is: " << argv[++i] << endl;
            cout << "Will use Google." << endl;
            token = UnicodeString(argv[i]);
        }
        else if(UnicodeString(argv[i]) == "--tess"){
            cout << "Will use tesseract. " << endl;
            tess = true;
        }
        else if(UnicodeString(argv[i]) == "-l"){
            worklang = string(argv[++i]);
            cout << "Selected language: " <<  worklang << endl;
        }
        else if(UnicodeString(argv[i]) == "--id"){
            uuid_parse(argv[++i], workid);
            char idstr[37];
            uuid_unparse(workid, idstr);
            cout << "workid is: " << idstr << endl;
        }
        else if(UnicodeString(argv[i]) == "--callback"){
            callback = string(argv[++i]);
            cout << "callback is: " << callback << endl;
        }
    }
    if (move){
        //create directory if doesn't exist
        int start = usrcpath.lastIndexOf("/") + 1;
        string fname;
        filename.toUTF8String(fname);
        string filename_str;
        filename.toUTF8String(filename_str);
        uuid_t id;
        uuid_generate(id);
        uuid_unparse(id, idstr);
        UnicodeString mangadir = workdir + UnicodeString(idstr);
        string comstring;
        mangadir.toUTF8String(comstring);
        cout << endl << comstring << endl;
        cout << UnicodeString(idstr) << endl;
        int result = 0;
        result = createDirectory(mangadir, "raw");
        if (result != 0){
            cout << "result is: " << result << endl;
        }
        result = createDirectory(mangadir, "experiments");
        if (result != 0){
            cout << "result is: " << result << endl;
        }
        result = createDirectory(mangadir, "cleaned");
        if (result != 0){
            cout << "result is: " << result << endl;
        }
        for (auto lang : languages){
            int result = createDirectory(mangadir, icu::UnicodeString::fromUTF8(lang));
            if (result != 0) return result;
        }
        std::ofstream ofs;
        string mdir;
        mangadir.toUTF8String(mdir);
        //if (unzip){
        //    cout << "attempting unzip" << endl;
        //    cout << UnicodeString(idstr) << endl;
        //    unziprar(workdir + UnicodeString(idstr) + "/raw");
        //}
        fname.resize(fname.size()-4);
        auto now = std::chrono::system_clock::now();
        auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
        auto epoch = now_ms.time_since_epoch();
        auto value = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);
        long duration = value.count();
        j = {
            {"id", string(idstr)},
            {"filename", filename_str},
            {"title", fname},
            {"date", duration},
            {"languages", {"en", "raw", "cleaned"} }
        };

        ofs.open(mdir+"/info.json", std::ofstream::out | std::ofstream::trunc);
                //ofs << "name: " << filename << endl;
        //ofs << "id: " << UnicodeString(idstr) << endl;
        ofs << j;
        ofs.close();
        //move files to raw directory
        string srcpath;
        (usrcpath+filename).toUTF8String(srcpath);
        cout << "FILE SRC PATH: " << srcpath << endl;
        string destpath;
        (mangadir+"/raw/"+filename).toUTF8String(destpath);
        cout << "FILE DEST PATH: " << destpath << endl;
        system(("cp '" + srcpath + "' '" + destpath+ "'").c_str());
    }
    if (unzip){
        unziprar(workdir + UnicodeString(idstr) + "/raw");
        UnicodeString mangadir = workdir + UnicodeString(idstr);
        string mdir;
        mangadir.toUTF8String(mdir);
        json imagelist = json::array(); 
        for(auto p = fs::recursive_directory_iterator(mdir+"/raw");
                p != fs::recursive_directory_iterator(); p++){
            if (isImage(p->path().extension())){
                imagelist.push_back(p->path().filename());
            }
        }
        if (j.empty()){
            std::ifstream ifs(mdir+"/info.json");
            auto infojson = string(std::istreambuf_iterator<char>(ifs),
                    std::istreambuf_iterator<char>());
            ifs.close();
            if (infojson.length() == 0){
                cout << "info json is empty and no j object exists." << endl;
            }
            j = json::parse(infojson); 
        }
        j["images"] = imagelist;
        std::ofstream ofs(mdir+"/info.json", std::ofstream::out | std::ofstream::trunc);
        ofs << std::setw(4) << j << endl;
        ofs.close();
    }
    if (scanlate){
        RunScanlation(token, tess, translate, worklang, workdir+UnicodeString(idstr), callback, languages);
        // check if token is set || tess return if neither
        // check if files in raw
        // check if workdir has zip
        // unzip if only zip in workdir, into workdir/raw
        // start scanlation process
        // if translate, save json blobs.
    }

    return 0;
}

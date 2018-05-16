#include <iostream>
#include <fstream>
#include <unordered_map>
#include <map>
#include <vector>
#include <string>
#include <queue>
#include <chrono>
#include <thread>
#include <mutex>
#include <uuid/uuid.h>
#include <uWS/uWS.h>
#include <time.h>
#include "includes/Parser.h"
#include "includes/json.hpp"
#include "restclient-cpp/restclient.h"
#include <unicode/unistr.h>
#include <unicode/ustream.h>
#include <experimental/filesystem>
//#include "includes/server_buffer_generated.h"
//#include "includes/client_buffer_generated.h"



using std::cout; using std::endl; using std::string; using std::map; using std::unordered_map; using std::vector;
using icu::UnicodeString; namespace fs = std::experimental::filesystem;
namespace rc = RestClient; using json = nlohmann::json;
json mangamap_1 = json::object();
json mangamap_2 = json::object();
json* mangamap_ptr = &mangamap_1;
json listings_1 = json::object();
json listings_2 = json::object();
json* listings_ptr = &listings_1;

std::mutex mtx;
json config;
/*const diogenes::general::SB* createServerBuffer(){
	// create a `FlatBufferBuilder`, which will be used to create our
	// dir's Flatbuffers.
	flatbuffers::FlatBufferBuilder builder(1024);
    auto dname = builder.CreateString("M");
    auto parent = builder.CreateString("_none_");
    std::vector<std::string> kids = {"MA", "ME", "MO"};
    auto children = builder.CreateVectorOfStrings(kids);
    auto leaf = false;
    
    auto dir = diogenes::general::CreateSB(builder, dname, parent, children, leaf);
    builder.Finish(dir);
    
    return GetDirectory(builder.GetBufferPointer());
}*/
bool shutitdown = false;

void updateMangaInfo(){
    std::chrono::time_point last = std::chrono::system_clock::now();
    cout << "---UpdateMangaInfo---" << endl;
    bool initializing = true;
    while(true){
        if (shutitdown) return;
        if (std::chrono::system_clock::now() - last > std::chrono::seconds(60) || initializing){
            last = std::chrono::system_clock::now();
            initializing = false;
            string mangadir = config["mangadir"];
            json* mangamap_assigner = mangamap_ptr==&mangamap_1 ? &mangamap_2 : &mangamap_1;
            json* listings_assigner = listings_ptr==&listings_1 ? &listings_2 : &listings_1;
            listings_assigner->erase("listings");
            (*listings_assigner)["listings"] = json::array();
            string mangainfo = "";
            for(auto p = fs::recursive_directory_iterator(mangadir);
                    p != fs::recursive_directory_iterator(); p++){
                if (shutitdown) return;
                //cout << p->path() << endl;
                //if (p.depth() > 2) {p.pop();}
                if (p->path().filename() == "info.json" && p.depth() == 1){
                    //cout << p.depth() << endl;
                    cout << p->path() << endl;
                    std::ifstream ifs(p->path());
                    string manga_info = string(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
                    //cout << "before parse" << endl;
                    json j = json::parse(manga_info);
                    //cout << "after parse" << endl;
                    (*mangamap_assigner)[j["id"].get<string>()] = j;
                    //cout << "after id grab" << endl;
                    (*listings_assigner)["listings"].push_back(
                            {
                            {"id", j["id"].get<string>()},
                            {"title", j["title"].get<string>()},
                            {"date", j["date"].get<int64_t>()},
                            {"languages", j["languages"]}
                            }
                            );
                    //cout << "after listings pushback" << endl;
                    //cout << listings_assigner->dump() << endl << endl;
                    //cout << mangamap_assigner->dump() << endl;
                    ifs.close();
                    //cout << "after close" << endl;
                }
            }

            mtx.lock();
            //cout << "after lock" << endl;
            mangamap_ptr = mangamap_ptr == &mangamap_1 ? &mangamap_2 : &mangamap_1;
            listings_ptr = listings_ptr == &listings_1 ? &listings_2 : &listings_1;
            //cout << "before unlock" << endl;
            mtx.unlock();
        }
            std::this_thread::sleep_for(std::chrono::seconds(3));
    }
}

UnicodeString HTMLStringToUnicode(string ip){
    //cout << "ip: " << ip << endl;
    //cout << string::npos << endl;
    //cout << ip.size() << endl;
    UnicodeString output("");
    std::size_t i=0, last=0;
    int32_t uval = 0;
    while(i != string::npos){
        i = ip.find("&#",last);
        //cout << "outer substr: " << ip.substr(last,i-last) << endl;
        if (i == string::npos){
            //cout << "current output when i == npos is: " << endl;
            output += UnicodeString(ip.substr(last).data());
            break;
        }
        else
            output += UnicodeString(ip.substr(last,i-last).data());
        last = i+2;
        //cout << "i=" << i << endl;
        //cout << "last=" << last << endl;

        i = ip.find(";", last);
        if (i != string::npos){
            //cout << "inner substr: " << ip.substr(last, i-last) << endl;
            uval = std::stoul(ip.substr(last,i-last));
            if (uval < 1111998 && uval >= 0){
                //cout << "uval is: " << uval << ", " << UnicodeString(uval) << ". adding Unicode value to output." << endl;
                output += UnicodeString(uval);
            }
            last = i+1;
        }
        if (i == string::npos || last ==ip.size()) break; 
    }
    return output;
}

int handleUpload(uWS::HttpResponse* res, std::string contentType, char* data, size_t length, size_t remainingBytes){
	try{
        MPFD::Parser* parser = new MPFD::Parser();
        parser->SetTempDirForFileUpload("/tmp");
        parser->SetMaxCollectedDataLength(20*1024);
        parser->SetContentType(contentType);
        parser->AcceptSomeData(data, length);
        cout << "data: " << endl << endl<< data << endl;
		map<string,MPFD::Field*> fields = parser->GetFieldsMap();
		cout << "Have " << fields.size() << "fields\n\r";
		map<string,MPFD::Field*>::iterator it;
        string manga_dir = "";
        string temp_dir = "";
        string filename_str = "";
        UnicodeString filename;
		for(it=fields.begin();it!=fields.end();it++){
			if (fields[it->first]->GetType()==MPFD::Field::TextType) {
	            cout<<"Got text field: ["<<it->first<<"], value: ["<< fields[it->first]->GetTextTypeContent() <<"]\n";
                try{
                    if (it->first == "file1.path" || it->first == "myFile.path") temp_dir = fields[it->first]->GetTextTypeContent();
                    if (it->first == "file1.name" || it->first == "myFile.name") filename = HTMLStringToUnicode(fields[it->first]->GetTextTypeContent());
                }
                catch(std::exception e){
                    cout << "could not handle file" << endl;
                    return 1;
                }
                cout << HTMLStringToUnicode(fields[it->first]->GetTextTypeContent()) << endl;
	        } //else {
	          //  cout<<"Got file field: ["<<it->first<<"] Filename:["<<fields[it->first]->GetFileName()<<"] \n";
              //  filename_str = fields[it->first]->GetFileName();
                //filename.toUTF8String(filename_str);
	        //}
		}
        
        filename.toUTF8String(filename_str);
		// This is where a separate executable is called to handle the download
        std::ofstream ofs;
        cout << "upload log file: " <<  "/roboscans/tmp/uploadtransfer.log" << endl;
        ofs.open("/roboscans/tmp/uploadtransfer.log", std::ofstream::out | std::ofstream::app);
        string upload_info =  "mv '" + temp_dir + "' '" + temp_dir.substr(0,17) + filename_str + "';mangahandler -i '" + temp_dir.substr(0,17) + "' -f '" + filename_str + "' -m '" + config["mangadir"].get<string>() + "' --token 'googleapitokenlolosioio2348459' -s -u &";
        ofs << upload_info << endl;
        ofs.close(); 
        system(("mv '" + temp_dir + "' '" + temp_dir.substr(0,17) + filename_str + "';mangahandler -i '" + temp_dir.substr(0,17) + "' -f '" + filename_str + "' -m '" + config["mangadir"].get<string>() + "' --token 'googleapitokkenffisdfosdifiodif' -s -u &").c_str());
        return 0;

	} catch (MPFD::Exception e){
		cout << e.GetError() << endl;
		// Parsing input error
		res->end(nullptr, 0);
		return 1;
	}
	return 0;
}

int configure(){
    std::ifstream config_file("/usr/local/bin/manga_server_config.json");
    string config_str = string(std::istreambuf_iterator<char>(config_file), std::istreambuf_iterator<char>());
    config_file.close();
    if (config_str.length() == 0) {
        cout << "could not open config file" << endl;
    }
    config = json::parse(config_str);
    if (config.empty()){
        cout << "config is empty...why"  << endl;
    }
    if (config.empty()) return 1;
    cout << config << endl;
    return 0;
}

int main()
{	
    if (configure() != 0){
        cout << "error with configuration file" << endl;
        return 1;
    }
    cout << "before launchmangachecker" << endl;
    //launchMangaChecker();
    std::thread updater(updateMangaInfo); 
    cout << "after launchmangachecker" << endl;
    std::this_thread::sleep_for(std::chrono::seconds(10));

	uWS::Hub h;
    std::string header100 = "HTTP/1.1 100 Hold on a sec\r\nContent-Type: text/json\r\n\r\n";
    std::string header200 = "HTTP/1.1 200 OK\r\nContent-Type: text/json\r\n\r\n";
    std::string header300 = "HTTP/1.1 300 Go somewhere else.\r\nContent-Type: text/json\r\n\r\n";
    std::string header400 = "HTTP/1.1 400 Wtf are you doing?\r\nContent-Type: text/json\r\n\r\n";
    std::string header500 = "HTTP/1.1 500 Fuck, something broke.\r\nContent-Type: text/json\r\n\r\n";
    vector<string> headers = {header100, header200, header300, header400, header500};
    auto controlData = [&h, &headers](uWS::HttpResponse *res, char *data, size_t length, size_t remainingBytes) {

	        std::string *buffer = (std::string *) res->httpSocket->getUserData();
	        buffer->append(data, length);

	        std::cout << "HTTP POST, chunk: " << length << ", total: " << buffer->length() << ", remainingBytes: " << remainingBytes << std::endl;

	        if (!remainingBytes) {
	            // control the contents

	            std::cout << "Buffer: " << *buffer << std::endl;

	            delete (std::string *) res->httpSocket->getUserData();
	        }
	    };

	h.onHttpData([&controlData, &headers](uWS::HttpResponse *res, char *data, size_t length, size_t remainingBytes) {
        std::cout << "Got some data" << std::endl;
    });
    
	h.onHttpRequest([&controlData, &headers](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t length, size_t remainingBytes) {
		std::cout << req.getUrl().toString() << std::endl;
		std::cout << req.getMethod() << std::endl;
        string url = req.getUrl().toString();
        auto method = req.getMethod();
        cout << "wtfwtfwtf" << endl;
		std::string thing = "hello world";
        if (req.getUrl().valueLength == 1) {
            res->end(thing.data(), thing.length());
        } 
        else {
            int result = 1;
            if (url  == "/upload" && 
            	method == uWS::HttpMethod::METHOD_POST) { 
                std::cout << "arrived at upload" << std::endl;
                //res->httpSocket->setUserData(new std::string);
                //controlData(res, data, length, remainingBytes);
                result = handleUpload(res, req.getHeader("content-type").toString(), data, length, remainingBytes);
                res->end(headers[1].data(), headers[1].length());
            }
            else if (url  == "/listings" &&
                    method == uWS::HttpMethod::METHOD_GET){
                cout << "got listing request" << endl;
                string listings_str = listings_ptr->dump();
                cout << "dump: " << listings_str << endl;
                res->end(listings_str.data(), listings_str.length());
            }
            else if (url.find("/manga/") == 0 && url.length() > 42 &&
                    url.length() < 52 && method == uWS::HttpMethod::METHOD_POST){
                cout << "got manga request" << endl;
                string uuid = url.substr(7,36);
                if (url.length() > 43){
                    string lang = url.substr(43);
                    cout << lang << endl;
                }

                string mangamap_str = "{}";
                try{
                    mangamap_str = (*mangamap_ptr)[uuid].dump();
                }
                catch(const std::exception& e){
                    cout << "could not find the manga" << endl;
                    res->end(headers[3].data(), headers[3].length());
                }
                res->end(mangamap_str.data(), mangamap_str.length());
            }
            else{
                std::cout << "got something" << std::endl;
                cout << url << endl;
                cout << url.length() << endl;
                std::string r;
                //res->end(r.data(), r.length());
                res->end(headers[3].data(), headers[4].length());
            }
        }
        return;
    });
	  
    h.onMessage([](uWS::WebSocket<uWS::SERVER> *ws, char *message, size_t length, uWS::OpCode opCode) {
        ws->send(message, length, opCode);
    });

    if (h.listen(8081)) {
        h.run();
    }
    shutitdown = true;
    updater.join();
}

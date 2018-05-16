// Example program
#include <iostream>
#include <sys/types.h>
#include <dirent.h>
#include <uuid/uuid.h>
#include <vector>
#include <string>
#include <experimental/filesystem>
#include "includes/json.hpp"
#include "restclient-cpp/restclient.h"
#include "includes/cppcodec/base64_default_rfc4648.hpp"
#include <unicode/unistr.h>
#include <unicode/ustream.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/photo/photo.hpp>
#include <opencv2/core/types.hpp>


using std::cout; using std::endl; using std::string; using std::vector; using icu::UnicodeString;
using cv::Rect; using cv::Mat; using cv:Point;
struct Brect{
    vector<Point> contours;
    Rect rect;
    vector<Rect> text_bounds;
    Mat* image;
    float scaling;
    string text_info;
    string raw;
    unordered_map<string, string> sentences;

    bool is_inverted = false;
    // need to create +operator for merging Brects
};

// These functions are called in rough order
string OCR(bool tess, UnicodeString token, cv::Mat& image, string worklang, string imgfmt);
vector<Brect> getBrects(string& text_info);
    void brectGetContours(Brect& b);
void removePronunciationText(string& text_info);
    bool detectOnlyKatakana(string description);
    bool detectOnlyHiragana(string description);
bool canMergeBrects(Brect& a, Brect& b);
    vector<Point> getSharedBubble(Brect& a, Brect& b);
vector<Rect> getTextbounds(Brect& b);
float findTextSize(string s, vector<Rect> bounds);
void brectsInsertText(Mat& image, Mat& finished, vector<Brect> brects);

int main()
{

    return 0;
}

string OCR(bool tess, UnicodeString token, cv::Mat& image, string worklang, string imgfmt){
    string output("");
    if (tess){
        cout << "running tesseract" << endl;
    }
    if (token.length() > 0){
        cout << "using google" << endl;
        string *enc_img = EncodeImage(image, imgfmt);
        json j = json::object();
        j["requests"] = {{"image", {{"content", /*"dummy"*/ *enc_img }}},
                          {"features", {{{"type", "TEXT_DETECTION"}}}},
                          {"imageContext", {{"languageHints", {"ja"}}}}
                        };
        delete enc_img;
        //cout << j.dump() << endl;
        string url;
        ("https://vision.googleapis.com/v1/images:annotate?key=" + token).toUTF8String(url);
        const string googlevision = url;
        const string *jdump = new string(j.dump());
        rc::Response r = rc::post(url, "text/json", *jdump);
        delete jdump;
        cout << "response code: " << r.code << endl;
        if (r.code != 200) return "";
        //cout << "response headers: " << r.headers << endl;
        //cout << "response body: " << r.body << endl;
        output += r.body;
    }
    return output;
}

void removePronunciationText(string& text_info){
    cout << "---removePronunciationText---" << endl;
    json j = json::parse(text_info);
    int count = 0;
    for(auto& i : j["responses"]){
        if (j["responses"].size() == 1 && i.empty()){
            cout << "responses size is 1 and i response is empty" << endl;
            return;
        }
        if (i.empty() || i.find("textAnnotations") == i.end()){
            cout << "i is empty or textAnnotations is at the end of i." << endl;
            continue;
        }
        for(int k=0;k<i["textAnnotations"].size();k++){
            count++;
            if (count==1) continue;
            if (i["textAnnotations"][k].find("boundingPoly") == i["textAnnotations"][k].end()) continue;
            if (i["textAnnotations"][k]["boundingPoly"].find("vertices") == i["textAnnotations"][k]["boundingPoly"].end()) continue;
            auto v = i["textAnnotations"][k]["boundingPoly"]["vertices"];
            if (v[0].find("x") == v[0].end()) v[0]["x"] = 0;
            if (v[0].find("y") == v[0].end()) v[0]["y"] = 0;
            if (v[2].find("x") == v[2].end()) v[2]["x"] = 0;
            if (v[2].find("y") == v[2].end()) v[2]["y"] = 0;
            auto current_rect = cv::Rect(cv::Point(v[0]["x"],v[0]["y"]),cv::Point(v[2]["x"],v[2]["y"]));
            if (i["textAnnotations"][k].find("description") == i["textAnnotations"][k].end()) continue;
            if (detectOnlyHiragana(i["textAnnotations"][k]["description"])){
                // full text description is hiragana, may be pronunciation text, check ahead
                cout << "description #" << count << ": full description is hiragana- " << i["textAnnotations"][k]["description"] << endl;
                // if first non-hiragana only description is to the left of our detected one and the width is larger, we're in a pronunciation col.
                for(int c=0;c<i["textAnnotations"].size();++c){
                    if (!detectOnlyHiragana(i["textAnnotations"][c]["description"])){
                        cout << "found non-hiragana after finding only hiragana. at "<< c << " , checking position and size." << endl;
                        auto vv = i["textAnnotations"][c]["boundingPoly"]["vertices"];
                        if (vv[0].find("x") == vv[0].end()) vv[0]["x"] = 0;
                        if (vv[0].find("y") == vv[0].end()) vv[0]["y"] = 0;
                        if (vv[2].find("x") == vv[2].end()) vv[2]["x"] = 0;
                        if (vv[2].find("y") == vv[2].end()) vv[2]["y"] = 0;
                        auto check_rect = cv::Rect(cv::Point(vv[0]["x"],vv[0]["y"]),cv::Point(vv[2]["x"],vv[2]["y"]));
                        cout << "check_rect:    " << check_rect << endl;
                        cout << "current_rect:  " << current_rect << endl;
                        if (check_rect.width > current_rect.width *1.4 &&
                            check_rect.tl().y < current_rect.br().y &&
                            check_rect.br().y > current_rect.tl().y && 
                            check_rect.br().x > current_rect.tl().x - current_rect.width &&
                            check_rect.br().x < current_rect.br().x){
                            cout << "width is larger, and positioned to the left nearby, removing: " << k << endl;
                            // non-overlapping, larger width, positioned to the left close by-- FOUND HINT, remove current
                            i["textAnnotations"].erase(i["textAnnotations"].begin()+k);
                            --k;
                            break;
                        }
                    }
                }
            }
            else if (detectOnlyKatakana(i["textAnnotations"][k]["description"])){
                // full text description is hiragana, may be pronunciation text, check ahead
                cout << count << ": full description is katakana- " << i["textAnnotations"][k]["description"] << endl;
                // if first non-katakana only description is to the left of our detected one and the width is larger, we're in a pronunciation col.
                for(int c=0;c<i["textAnnotations"].size();++c){
                    if (!detectOnlyKatakana(i["textAnnotations"][c]["description"])){
                        cout << "found non-katakana after finding only katakana. at "<< c << " , checking position and size." << endl;
                        auto vv = i["textAnnotations"][c]["boundingPoly"]["vertices"];
                        if (vv[0].find("x") == vv[0].end()) vv[0]["x"] = 0;
                        if (vv[0].find("y") == vv[0].end()) vv[0]["y"] = 0;
                        if (vv[2].find("x") == vv[2].end()) vv[2]["x"] = 0;
                        if (vv[2].find("y") == vv[2].end()) vv[2]["y"] = 0;
                        auto check_rect = cv::Rect(cv::Point(vv[0]["x"],vv[0]["y"]),cv::Point(vv[2]["x"],vv[2]["y"]));
                        cout << "check_rect:    " << check_rect << endl;
                        cout << "current_rect:  " << current_rect << endl;
                        if (check_rect.width > current_rect.width *1.4 &&
                            check_rect.tl().y < current_rect.br().y &&
                            check_rect.br().y > current_rect.tl().y && 
                            check_rect.br().x > current_rect.tl().x - current_rect.width &&
                            check_rect.br().x < current_rect.br().x){
                            cout << "width is larger, and positioned to the left nearby, removing: " << k << endl;
                            // non-overlapping, larger width, positioned to the left close by-- FOUND HINT, remove current
                            i["textAnnotations"].erase(i["textAnnotations"].begin()+k);
                            --k;
                            break;
                        }
                    }
                }
            }
            
       }
    }
    text_info = j.dump();
}

bool detectOnlyKatakana(string description){
    UnicodeString katakana = UnicodeString("゠ァアィイゥウェエォオカガキギクグケゲコゴサザシジスズセゼソゾタダチヂッツヅテデトドナニヌネノハバパヒビピフブプヘベペホボポマミムメモャヤュユョヨラリルレロヮワヰヱヲンヴヵヶヷヸヹヺ・ーヽヾヿ");
    UnicodeString input_string = UnicodeString::fromUTF8(description);
    for(int i = 0;i<input_string.length();i++){
        bool found_katakana = false;
        for (int p = 0;p<katakana.length();p++){
            if (katakana[p] == input_string[i]){
                found_katakana = true;
            }
        }
        if (!found_katakana){
            return false;
        }
    }
    return true;
}

bool detectOnlyHiragana(string description){
    UnicodeString hiragana = UnicodeString("ぁあぃいぅうぇえぉおかがきぎくぐけげこごさざしじすずせぜそぞただちぢっつづてでとどなにぬねのはばぱひびぴふぶぷへべぺほぼぽまみむめもゃやゅゆょよらりるれろゎわゐゑをんゔゕゖ゗゘゙-゚゛゜ゝゞゟ");
    UnicodeString input_string = UnicodeString::fromUTF8(description);
    for(int i = 0;i<input_string.length();i++){
        bool found_hiragana = false;
        for (int p = 0;p<hiragana.length();p++){
            if (hiragana[p] == input_string[i]){
                found_hiragana = true;
            }
        }
        if (!found_hiragana){
            return false;
        }
    }
    return true;
}

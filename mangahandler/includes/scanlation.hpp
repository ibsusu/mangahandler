#include <iostream>
#include <uuid/uuid.h>
#include <unicode/unistr.h>
#include <unicode/ustream.h>
#include <experimental/filesystem>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <fstream>
#include <algorithm>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/photo/photo.hpp>
#include <opencv2/core/types.hpp>
#include "json.hpp"
#include "restclient-cpp/connection.h"
#include "restclient-cpp/restclient.h"
#include "cppcodec/base64_default_rfc4648.hpp"


using std::cout; using std::endl; using std::string; using std::unordered_map; using std::vector;
using icu::UnicodeString; namespace fs = std::experimental::filesystem;
namespace rc = RestClient; using json = nlohmann::json;

enum Shade {white, black, grey};

struct Word {
    string raw;
    string detectedBreak;
    unordered_map<string, string> translations;
    cv::Rect box;
};
struct Bubble {
    // isn't necessarily a bubble, just the location of a block of text and its containing shade
    cv::Rect box;
    Shade shade;
};

struct Brect{
    vector<cv::Point> contours;
    cv::Rect rect;
    cv::Mat* image;
    float scaling;
    string text_info;
    string raw;
    unordered_map<string, string> sentences;
    bool is_bubble = false;
};

struct Sentence {
    string raw;
    vector<Word> words;
    vector<Bubble> bubbles;
};

void RunScanlation(UnicodeString token, bool tess, bool translate, string worklang,
                UnicodeString mangadir, string callback_location, vector<string> languages);

cv::Mat* Clean(cv::Mat& image, json& pz){
    cout << "---Clean---" << endl;
    cv::Mat mask = cv::Mat::zeros(image.rows, image.cols, CV_8U);
    cv::Mat* cleaned = new cv::Mat();
    bool first1 = true;
    for (auto i : pz){
        if (first1){
            first1 = false;
            continue;
        }
        cv::Point uleft(i[0]["x"], i[0]["y"]), lright(i[2]["x"],i[2]["y"]);
        int adj = (lright.x-uleft.x) * .12;
        uleft.x >= adj ? uleft.x-=adj:uleft.x-=0;
        uleft.y >= adj ? uleft.y-=adj:uleft.y-=0;
        lright.x <= mask.cols-adj ? lright.x-=adj:lright.x-=0;
        lright.y <= mask.rows-adj ? lright.y-=adj:lright.y-=0;
        cv::Rect rect =  cv::Rect(uleft, lright );
        if (rect.width * 2 > mask.cols && rect.height * 2 > mask.rows){
            continue;
        }
        //cout << rect << endl;
        mask(rect) = 255;
    }
    //cv::namedWindow("mask", cv::WINDOW_NORMAL);
    ////cv::imshow("mask", mask);
    //cv::waitKey(0);
    //cv::destroyWindow("mask");
    cv::inpaint(image, mask, *cleaned, 5, cv::INPAINT_TELEA);
    return cleaned;
}

string* EncodeImage(cv::Mat& image, string imgfmt){
    vector<uchar> buf;
    cv::imencode(imgfmt, image, buf);
    //uchar *enc_msg = new uchar[buf.size()];
    //string *msg = new string("");
    //for (int i=0;i<buf.size();i++) enc_msg[i] = buf[i];
    //for (int i=0;i<buf.size();i++) msg.append(buf);
    string *msg = new string(&buf[0], &buf[0]+buf.size());
    string *encoded = new string(base64::encode(*msg));
    delete msg;
    return encoded;
}

bool SortRectsByOrigin(cv::Rect a, cv::Rect b){
    if (a.br().x > b.br().x) return true;
    if (a.br().x == b.br().x){
        if (a.br().y < b.br().y) return true;
    }
    return false;
}

bool SortWordsByOrigin(Word a, Word b){
    return SortRectsByOrigin(a.box, b.box);
}

void MergeStubbornBubbles(const cv::Mat& image, vector<Bubble> &bubbles){
    int mergeable = true;
    int counter = 0;
    while(mergeable){
        mergeable = false;
        counter++;
        if (counter > 1000) return;
        for(int i=0;i < bubbles.size();i++){  // TODO: remove shitty nested for loop
            cout << "i: " << i << endl;
            for (int j=i+1;j<bubbles.size();j++){
                //cout << "j: " << j << endl;
                //if (j == 34) cout << "bubb size: " << bubbles.size();
                if ((bubbles[i].box & bubbles[j].box).area() > 0){
                    mergeable = true;
                    //cout << "got intersection stubborn without alteration, unifying" << endl;
                    bubbles[i].box = bubbles[i].box | bubbles[j].box;
                    bubbles.erase(bubbles.begin()+j);
                    j--;
                    continue;
                }
                cv::Rect limits = bubbles[i].box;
                limits.x -= image.cols*.03;//std::max(rects[i-1].width*5, 0);
                limits.width += image.cols*.03;//std::min(rects[i-1].width*5, image.cols - limits.x);
                if ((bubbles[j].box & limits).area() > 0){
                    mergeable = true;
                    //cout << "got x intersection stuborn, unifying left " << endl;
                    bubbles[i].box = bubbles[j].box | limits;
                    bubbles.erase(bubbles.begin()+j);
                    j--;
                    continue;
                }
                limits = bubbles[j].box;
                limits.y -= 10;//30; //0.05*image.rows;//std::max(rects[i-1].width*5, 0);
                limits.height += 10;
                if ((bubbles[i].box & limits).area() > 0){
                    mergeable = true;
                    //cout << "got top intersection stubborn, unifying up" << endl;
                    bubbles[i].box = bubbles[i].box | limits;
                    bubbles.erase(bubbles.begin()+j);
                    j--;
                    continue;
                }
                limits = bubbles[j].box;
                limits.height += 10;//60;//std::min(rects[i-1].width*5, image.rows - limits.y);
                if ((bubbles[i].box & limits).area() > 0){
                    mergeable = true;
                    //cout << "got bottom intersection stubborn, unifying down" << endl;
                    bubbles[i].box = bubbles[i].box | limits;
                    bubbles.erase(bubbles.begin()+j);
                    j--;
                    continue;
                }
            }
        }
    }
}

/*void checkImage(cv::Mat image){
    cv::namedWindow("Image Check", cv::WINDOW_NORMAL);
    cv::imshow("Image Check", image);
    cv::waitKey(0);
    cv::destroyWindow("Image Check");
}*/

vector<Brect> getContourBubbles(cv::Mat image){
    cv::Mat image_gray;
    cv::Mat image_bin;
    cv::cvtColor(image, image_gray, CV_BGR2GRAY);
    cv::blur(image_gray, image_gray, cv::Size(2,2));
    cv::threshold(image_gray, image_bin, 180, 255, cv::THRESH_BINARY);
    vector<vector<cv::Point>> contours;
    vector<cv::Vec4i> hierarchy;
    cv::Mat3b out;
    cv::cvtColor(image_gray, out, cv::COLOR_GRAY2BGR);
    cv::findContours(image_bin, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_NONE);
    cout << "finding contours" << endl;
    cout << "contours size: " << contours.size() << endl;
    vector<vector<cv::Point>> round_contours;
    round_contours.resize(contours.size());
    vector<Brect> brects;
    for(size_t i=0;i<contours.size();i++){
        double area = cv::contourArea(contours[i], true);
        if (area >=0){
            //cv::drawContours(out, contours, i, cv::Scalar(0,255, 0), -1);
        }
        else{
            double arc = 0.01*cv::arcLength(contours[i], true);
            cv::approxPolyDP(cv::Mat(contours[i]), round_contours[i], arc, true);
            if (-area > 3000 ){
                int minx=image_gray.cols,miny=image_gray.rows;
                int maxx=0, maxy=0;
                for(size_t j = 0;j<round_contours[i].size();j++){
                    if (round_contours[i][j].x < minx) minx = round_contours[i][j].x;
                    if (round_contours[i][j].y < miny) miny = round_contours[i][j].y;
                    if (round_contours[i][j].x > maxx) maxx = round_contours[i][j].x;
                    if (round_contours[i][j].y < maxy) maxy = round_contours[i][j].y;
                }
                if (maxx - minx > image_gray.cols * .4) continue;
                if (maxy - miny > image_gray.rows * .4) continue;
                if (hierarchy[i][2] == -1) continue;
                //cout << hierarchy[i][2] << endl;
                if (cv::contourArea(contours[hierarchy[i][2]], true) < 0 ) continue;
                cv::Rect rect = cv::boundingRect(round_contours[i]);
                Brect brect;
                brect.contours = round_contours[i];
                brect.rect = rect;
                brects.push_back(brect);
                //cout << "should have bubble" << endl;
                //cout << rect << endl;
                cv::drawContours(out, contours, i, cv::Scalar(0, 0, 255), -1);
            }
        }
    }
    //cout << "creating bubble window" << endl;
    //cv::namedWindow("Bubbles", cv::WINDOW_NORMAL);
    //cout << "opening bubble window" << endl;
    //cv::imshow("Bubbles", image_bin);
    //cv::waitKey();
    //cv::destroyWindow("Bubbles");
    return brects;
}

void brectsEnhance(cv::Mat image, vector<Brect> brects){
    cout << "---brectsEnhance---" << endl;
    for (int i=0;i<brects.size();i++){
        cv::Mat ROI(image, brects[i].rect);
	    cv::resize(ROI, ROI, cv::Size(), 2, 2 );
  	  	cv::threshold(ROI, ROI, 230, 255, cv::THRESH_BINARY);
    	cv::dilate(ROI, ROI, cv::Mat(), cv::Point(-1, -1), 1, 1, 1);
   		//cv::namedWindow("E", cv::WINDOW_AUTOSIZE);
    	//cv::imshow("E", ROI);
    	//cv::waitKey();
    	//cv::destroyAllWindows();
        *brects[i].image = ROI.clone();
	}
    cout << "made it to end" << endl;
}

vector<Bubble> GetBubbles(const cv::Mat& image, string text_info){
    cout << "---GetBubbles---" << endl;
    vector<Bubble> bubbles;
    if (text_info.length() == 0) return bubbles;
    json j = json::parse(text_info);
    int ct = 0;
    cv::Rect standard;
    standard.x = standard.y = standard.width = standard.height = 0;
    vector<cv::Rect> rects;
    for(auto i : j["responses"]){
        if (j["responses"].size() == 1 && i.empty()) return bubbles;
        if (i.empty() || i.find("textAnnotations") == i.end()) continue;
        for(auto anno : i["textAnnotations"]){
            ct++;
            if (ct==1) continue;
            if (anno.find("boundingPoly") == anno.end()) continue;
            if (anno["boundingPoly"].find("vertices") == anno["boundingPoly"].end()) continue;
            auto v = anno["boundingPoly"]["vertices"];
            rects.push_back(cv::Rect(cv::Point(v[0]["x"],v[0]["y"]),cv::Point(v[2]["x"],v[2]["y"])));
        }
    }
    // sorted in right to left, top to bottom order
    std::sort(rects.begin(), rects.end(), SortRectsByOrigin);
    Bubble bubble;
    bubble.box = rects[0];
    for(int i=1;i < rects.size();i++){
        if ((rects[i] & bubble.box).area() > 0){
            //cout << "got intersection without alteration, unifying" << endl;
            bubble.box = bubble.box | rects[i];
            continue;
        }
        cv::Rect limits = bubble.box;
        limits.x -= rects[i].width;//rects[i].width;//std::max(rects[i-1].width*5, 0);
        limits.width += rects[i].width;//std::min(rects[i-1].width*5, image.cols - limits.x);
        if ((rects[i] & limits).area() > 0){
            //cout << "got x intersection, unifying left" << endl;
            bubble.box = limits;
            bubble.box = bubble.box | rects[i];
            continue;
        }
        limits = bubble.box;
        limits.y -= 0.05*image.rows;//std::max(rects[i-1].width*5, 0);
        limits.height += 0.05*image.rows;
        if ((rects[i] & limits).area() > 0){
            //cout << "got top intersection, unifying up" << endl;
            bubble.box = limits;
            bubble.box = bubble.box | rects[i];
            continue;
        }
        limits = bubble.box;
        limits.height += 10;//0.05*image.rows;//60;//std::min(rects[i-1].width*5, image.rows - limits.y);
        if ((rects[i] & limits).area() > 0){
            //cout << "got bottom intersection, unifying down" << endl;
            bubble.box = limits;
            bubble.box = bubble.box | rects[i];
            continue;
        }
        bubbles.push_back(bubble);
        bubble.box = rects[i++];
    }
    bubbles.push_back(bubble);
    MergeStubbornBubbles(image, bubbles);
    for (int b=0;b<bubbles.size();b++){
        bubbles[b].box.x -= bubbles[b].box.width/2;
        bubbles[b].box.width += bubbles[b].box.width/2;
    }
    return bubbles;
}

vector<Sentence> GetSentences(string text_info, vector<Bubble> bubbles){
    cout << "---GetSentences---" << endl;
    vector<Sentence> sentences;
    if (text_info.length() == 0) return sentences;
    json j = json::parse(text_info);
    vector<Word> words;
    int ct = 0;
    for(auto i : j["responses"]){
        if (j["responses"].size() == 1 && i.empty()) return sentences;
        if (i.find("textAnnotations") == i.end()) continue;
        for(auto anno : i["textAnnotations"]){
            ct++;
            if (ct==1) continue;
            if (anno.find("boundingPoly") == anno.end()) continue;
            if (anno["boundingPoly"].find("vertices") == anno["boundingPoly"].end()) continue;
            auto v = anno["boundingPoly"]["vertices"];
            Word word;
            word.box = cv::Rect(cv::Point(v[0]["x"],v[0]["y"]),cv::Point(v[2]["x"],v[2]["y"]));
            word.raw = anno["description"];
            if (anno.find("en") != anno.end()){
                word.translations["en"] = anno["en"];
            }
            words.push_back(word);
        }
    }
    vector<Word> wds = words;
    vector<Bubble> wb = bubbles;
    Sentence sentence;
    for(int b=0;b<wb.size();b++){
        bool found = false;
        for(int w=0;w<wds.size();w++){
            if((wb[b].box & wds[w].box).area() > 0){
                if (!found){
                    sentence.bubbles.push_back(wb[b]);
                }
                sentence.words.push_back(wds[w]);
                wds.erase(wds.begin()+w);
                w--;
                found = true;
            }
        }
        for (auto k : sentence.words) sentence.raw += k.raw;
        if (sentence.bubbles.empty())
            cout << "sentence is empty: " << sentence.raw << endl;
        else
            sentences.push_back(sentence);
        sentence.raw = "";
        sentence.words.clear();
        sentence.bubbles.clear();
    }
    return sentences;
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

bool inspectGoogleTextLocation(cv::Mat& image, string& text_info){
    cout << "---inspectGoogleTextLocation---" << endl;
    //cv::namedWindow("text_inspector", cv::WINDOW_NORMAL);
    cv::Mat inspector_image = image.clone();
    json j = json::parse(text_info);
    bool has_text = false;
    int count = 0;
    cout << "after parse" << endl;
    cout << j << endl;
    if (j.find("responses") == j.end()){
        cout << "no responses, at all" << endl;
        return false;
    }
    for(auto& i : j["responses"]){
        cout << "in loop" << endl;
        if (j["responses"].size() == 1 && i.empty()){
            cout << "responses size is 1 and i response is empty" << endl;
            return false;
        }
        if (i.empty() || i.find("textAnnotations") == i.end()){
            cout << "i is empty or textAnnotations is at the end of i." << endl;
            continue;
        }
        for(auto& anno : i["textAnnotations"]){
            has_text = true;
            count++;
            if (count==1) continue;
            if (anno.find("boundingPoly") == anno.end()) continue;
            cout << "after boundingpoly" << endl;
            if (anno["boundingPoly"].find("vertices") == anno["boundingPoly"].end()) continue;
            cout << "after findvertices" << endl;
            auto& v = anno["boundingPoly"]["vertices"];
            cout << "after v assign" << endl;
            cout << v << endl;
            if (v[0].find("x") == v[0].end()) v[0]["x"] = 0;
            if (v[0].find("y") == v[0].end()) v[0]["y"] = 0;
            if (v[2].find("x") == v[2].end()) v[2]["x"] = 0;
            if (v[2].find("y") == v[2].end()) v[2]["y"] = 0;
            auto current_rect = cv::Rect(cv::Point(v[0]["x"],v[0]["y"]),cv::Point(v[2]["x"],v[2]["y"]));
            cout << "after current_rect assign" << endl;
            cv::rectangle(inspector_image, current_rect, cv::Scalar(255,0,0), 1, 0);
            cout << "rectangle" << endl;
            cv::putText(inspector_image, std::to_string(count), current_rect.br(),
                    cv::FONT_HERSHEY_PLAIN, 1,  cv::Scalar(0,0,255), 2);
            cout << "after put_text" << endl;
       }
    }
    //cv::imshow("text_inspector", inspector_image);
    //cv::waitKey();
    //cv::destroyWindow("text_inspector");
    return has_text;
}

void brectsOCR(bool tess, UnicodeString token, vector<Brect>& brects, string worklang, string imgfmt){
    cout << "---brectsOCR---" << endl;
    for(int i=0;i<brects.size();i++){
        string text_info = OCR(tess, token, *brects[i].image, worklang, imgfmt);
        brects[i].text_info = text_info;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
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
void removeOverlapping(vector<Brect>& brects){
    cout << "---removeOverlapping---" << endl;
    for(int i=0;i<brects.size();i++){
        for(int j=i+1;j<brects.size();j++){
            cv::Rect intersection = brects[i].rect & brects[j].rect;
            if (intersection.area() > 0){
                Brect* smaller = brects[i].rect.area() < brects[j].rect.area() ? &brects[i] : &brects[j];
                if ((smaller->rect & intersection).area() > smaller->rect.area() * .5){
                    // too much overlap, remove bigger one, it's not safe.
                    cout << "removing brect, it overlaps another." << endl;
                    if (brects[i].rect.area() > brects[j].rect.area()){
                        brects.erase(brects.begin()+i);
                        --i;--j;
                    }
                    else if(brects[i].rect.area() <= brects[j].rect.area()){
                        brects.erase(brects.begin()+j);
                        --j;
                    }
                }
            }
        }
    }
}

string brectsCreateSentence(string &text_info){
    cout << "---brectsCreateSentence---" << endl;
    json j = json::parse(text_info);
    int count = 0;
    string sentence = "";
    for(auto& i : j["responses"]){
        if (j["responses"].size() == 1 && i.empty()){
            cout << "responses size is 1 and i response is empty" << endl;
            return sentence;
        }
        if (i.empty() || i.find("textAnnotations") == i.end()){
            cout << "i is empty or textAnnotations is at the end of i." << endl;
            continue;
        }
        for(int k=0;k<i["textAnnotations"].size();k++){
            count++;
            if (count==1) continue;
            if (i["textAnnotations"][k].find("description") == i["textAnnotations"][k].end()) continue;
            cout << "adding --" << i["textAnnotations"][k]["description"] << endl;
            sentence += i["textAnnotations"][k]["description"];
        }
    }
    return sentence;
}

void brectsCleanText(vector<Brect>& brects){
    cout << "---brectsCleanText---" << endl;
    cout << brects.size() << endl;
    removeOverlapping(brects);
    for(int i=0;i<brects.size();++i){
        if (!inspectGoogleTextLocation(*brects[i].image, brects[i].text_info)){
            cout << "Deleting brect,"<< i <<" it has no text" << endl;
            delete brects[i].image;
            cout << "brects size: " << brects.size() << endl;
            brects.erase(brects.begin()+i);
            --i;
            cout << "brects size: " << brects.size() << endl;
            cout << "i is now: " << i << endl;
            continue;
        }
        removePronunciationText(brects[i].text_info);
        cout << "AFTER REMOVING PRONUNCIATIONS" << endl;
        cout << brects[i].text_info << endl;
        //inspectGoogleTextLocation(*brects[i].image, brects[i].text_info);
        brects[i].raw = brectsCreateSentence(brects[i].text_info);
    }
}

void brectsTranslate(vector<Brect>& brects, const UnicodeString token, string rawlang, vector<string> languages){
    cout << "---brectsTranslate---" << endl;
    json rtxts = json::array();
    json rbody = json::object();
    string url = "https://translation.googleapis.com/language/translate/v2?key=";
    string token_str;
    token.toUTF8String(token_str);
    rc::init();
    rc::Connection* conn = new rc::Connection(url+token_str);
    rc::HeaderFields headers;
    headers["Content-Type"] = "application/json; charset=utf-8";
    conn->SetHeaders(headers);
    // create queries
    for(int i=0;i<brects.size();i++){
        rtxts.push_back(brects[i].raw);
        if (rtxts.size() > 127) break;
    }
    rbody["q"] = rtxts;
    rbody["source"] = rawlang;
    rbody["format"] = "text";
    json ret;
    for (auto lang: languages){
        cout  << lang << endl;
    }
    for (auto lang : languages){
        if (lang != "en") continue;  // TODO: remove when translations are working
        rbody["target"] = "en";
        const string* rdump = new string(rbody.dump());
        rc::Response r = conn->post("", *rdump);
        delete rdump;
        //cout << "translated code: " << r.code << endl;
        //cout << "translated body: " << r.body << endl;
        if (r.code != 200){
            //cout << "translation return code: " << r.code << endl;
            cout << r.body << endl;
            continue;
        }
        ret = json::parse(r.body);
    }

    cout << "gonna input translations" << endl;
    //save the translations back to the text_info_object
    auto translations = ret["data"]["translations"];
    for(int i=0;i<brects.size();++i){
        brects[i].sentences["en"] = translations.at(i)["translatedText"];
        cout << "assigning " << translations.at(i)["translatedText"]
             << " to " << brects[i].raw << endl;
    }
    rc::disable();
    cout << "Finished Translating." << endl;
}
void brectsCleanImage(cv::Mat& image, cv::Mat& cleaned, vector<Brect>& brects){
    cout << "---brectsCleanImage---" << endl;
    cleaned = image.clone();
    cout << "after clone" << endl;
    vector<vector<cv::Point>> contours;
    for(int i=0;i<brects.size();++i){
        contours.push_back(brects[i].contours);
        cout << "drawing contour: " << brects[i].raw << endl;
        cv::drawContours(cleaned, contours, i, cv::Scalar(255, 255, 255), -1);
    }
}
vector<string> splitString(string input, char delimiter){
    vector<string> tokens;
    std::istringstream tokenStream(input);
    string token;
    while (std::getline(tokenStream, token, delimiter)){
        tokens.push_back(token);
    }
    return tokens;
}

void brectsInsertText(cv::Mat& image, cv::Mat& finished, vector<Brect> brects){
    cout << "---brectsInsertText---" << endl;
    finished = image.clone();
    vector<string> words;
    int baseline = 0;
    for(int i=0;i<brects.size();i++){
        // load words
        words = splitString(brects[i].sentences["en"], ' ');
        string line = "";
        cv::Rect bounds = brects[i].rect;
        bounds.y += bounds.height / 2.5;
        bounds.x += 2;
        cout << "after bounds init" << endl;
        for (auto w : words){
            cv::Size text_size = getTextSize(line+w, cv::FONT_HERSHEY_COMPLEX, 0.75,2,&baseline);
            cout << "after getTextSize" << endl;
            if (text_size.width < brects[i].rect.width){
                line += w;
                line += " ";
                cout << "after line appending" << endl;
            }
            else{
                // write to image, line ending
                cout << "outputting text: " << line << endl;
                cv::putText(finished, line, bounds.tl(), cv::FONT_HERSHEY_COMPLEX,
                            .75, cv::Scalar(0,204,255), 2);
                // new line
                line = w;
                line += " ";
                bounds.y += text_size.height+3;
            }
            cout << "going to next word" << endl;
        }
        cv::putText(finished, line, bounds.tl(), cv::FONT_HERSHEY_COMPLEX,
                    .75, cv::Scalar(0,204,255), 2);
    }
    
}

void GetTranslations(string& text_info, const UnicodeString token, string rawlang, vector<string> languages){
    cout << "---GetTranslations---" << endl;
    cout << text_info << endl;
    if (text_info.length() == 0) return;
    json j = json::parse(text_info);
    json rtxts = json::array();
    json rbody = json::object();
    string url = "https://translation.googleapis.com/language/translate/v2?key=";
    string token_str;
    token.toUTF8String(token_str);
    rc::init();
    rc::Connection* conn = new rc::Connection(url+token_str);
    rc::HeaderFields headers;
    headers["Content-Type"] = "application/json; charset=utf-8";
    conn->SetHeaders(headers);
    // create queries
    int ct=0;
    for(auto i : j["responses"]){
        if (i.find("textAnnotations") == i.end()) continue;
        for(auto anno : i["textAnnotations"]){
            ct++;
            if (ct==1) continue;
            if (anno.find("boundingPoly") == anno.end()) continue;
            if (anno["boundingPoly"].find("vertices") == anno["boundingPoly"].end()) continue;
            rtxts.push_back(anno["description"]);
			if (rtxts.size() > 127) break;
            //cout << "adding to rtxts: " << anno["description"] << endl;
        }
    }

    rbody["q"] = rtxts;
    rbody["source"] = rawlang;
    rbody["format"] = "text";
    json ret;
    for (auto lang: languages){
        cout  << lang << endl;
    }
    for (auto lang : languages){
        if (lang != "en") continue;  // TODO: remove when translations are working
        rbody["target"] = "en";
        const string* rdump = new string(rbody.dump());
        rc::Response r = conn->post("", *rdump);
        delete rdump;
        //cout << "translated code: " << r.code << endl;
        //cout << "translated body: " << r.body << endl;
		if (r.code != 200){
            //cout << "translation return code: " << r.code << endl;
            cout << r.body << endl;
            continue;
        }
		ret = json::parse(r.body);
    }
    cout << "gonna input translations" << endl;
	//save the translations back to the text_info_object

	for(auto &i : j["responses"]){
        if (i.find("textAnnotations") == i.end()) continue;
        auto translations = ret["data"]["translations"];
        auto & annos = i["textAnnotations"];
        for (int k=0;k<translations.size();k++){
            annos.at(k+1)["en"] = translations.at(k)["translatedText"];
            //cout << "assigning " << translations.at(k)["translatedText"]
            //     << " to " << annos.at(k+1)["description"] << endl;
        }
    }
    rc::disable();
    cout << "Finished Translating." << endl;
    text_info = j.dump();
}

cv::Mat* TransformImageText(string& text_info, cv::Mat& image){
    cout << "---TransformImageText---" << endl;
    if (text_info.length() == 0){
        cv::Mat* output = new cv::Mat();
        *output = image.clone();
        return output;
    }
    //cout << "text_info length: " << text_info.length() << endl;
	json j = json::parse(text_info);
    // get vertices for each block's paragraph and draw a rectangle
    if (j.find("responses") == j.end()) return nullptr;
    json pz = json::array();
    for(auto i : j["responses"]){
       if (i.find("fullTextAnnotation") == i.end()) continue;
       if (i["fullTextAnnotation"].find("pages") == i["fullTextAnnotation"].end()) continue;
       for(auto page : i["fullTextAnnotation"]["pages"]){
            if (page.find("blocks") == page.end()) continue;
            for(auto block : page["blocks"]){
                if (block.find("paragraphs") == block.end()) continue;
                for(auto para : block["paragraphs"]){
                    if (para.find("boundingBox") == para.end()) continue;
                    if (para["boundingBox"].find("vertices") == para["boundingBox"].end()) continue;
                    for (auto box : para["boundingBox"]){
                        //pz.push_back(box);
                    }
                }
            }
       }
    }
    for(auto i : j["responses"]){
        if (i.find("textAnnotations") == i.end()) continue;
        for(auto anno : i["textAnnotations"]){
            if (anno.find("boundingPoly") == anno.end()) continue;
            if (anno["boundingPoly"].find("vertices") == anno["boundingPoly"].end()) continue;
            pz.push_back(anno["boundingPoly"]["vertices"]);
            //cout << anno["description"] << endl;
        }
    }
    return Clean(image, pz);
}

cv::Mat* Typeset(cv::Mat* cleaned, vector<Sentence>& sentences, const string language){
    // place text in boxes based on location of large text.

    int baseline = 0;
    cout << "---Typesetting---" << endl;
    cv::Mat* outputimage = new cv::Mat();
    *outputimage = cleaned->clone();
    if (sentences.size() == 0) return outputimage;
    for(auto sen : sentences){
        string sentence;
        cout << "before bubble" << endl;
        cv::Rect bounds = sen.bubbles[0].box; // TODO fill all bubbles not just first
        cout << "after bounds init" << endl;
        for (auto w : sen.words){
            cv::Size text_size = getTextSize(sentence+w.translations[language], cv::FONT_HERSHEY_COMPLEX, 0.75,2,&baseline);
            cout << "after getTextSize" << endl;
            if (text_size.width < 200){
                sentence += w.translations[language];
                sentence += " ";
                cout << "after sentence appending" << endl;
            }
            else{
                // write to image, line ending
                cout << "outputting text: " << sentence << endl;
                cv::putText(*outputimage, sentence, bounds.tl(), cv::FONT_HERSHEY_COMPLEX,
                            .75, cv::Scalar(0,0,255), 2);
                // new line
                sentence = w.translations[language];
                sentence += " ";
                bounds.y += text_size.height+3;
            }
            cout << "going to next word" << endl;
        }
        cout << "going to next SENTENCE" << endl;
        cout << "sentence: " << sentence << endl;
        cout << "bounds: " << bounds.tl() << endl;
        if (outputimage != nullptr) cout << "output image isn't pointing to nullptr" << endl;
        // write to image, out of words
        cv::putText(*outputimage, sentence, bounds.tl(), cv::FONT_HERSHEY_COMPLEX,
                    .75, cv::Scalar(0,0,255), 2);
        cout << "wtf is it" << endl;
    }
    cout << "returning typeset image" << endl;
    return outputimage;
}

void CheckBubbles(cv::Mat* image, vector<Bubble> bubbles){
    int count=0;
    for (auto i : bubbles){
        cv::rectangle(*image, i.box, cv::Scalar(255,0,0), 1, 0);
        cv::putText(*image, std::to_string(count),  i.box.br(),
                    cv::FONT_HERSHEY_PLAIN, 1,  cv::Scalar(0,0,255), 2);
        count++;
    }
}

int ScanlatePage(UnicodeString token, bool tess, bool translate, string worklang,
                UnicodeString mangadir, string callback_location, vector<string> languages,
                const fs::directory_entry& p){
    UnicodeString rawdir = mangadir + UnicodeString("/raw");
    // use opencv to open image
    try {
        cv::Mat image;
        cv::Mat cleaned;
        cv::Mat finished;
        image = cv::imread(p.path().string());
        cout << "stem: " << p.path().stem() << endl;
        vector<Brect> brects = getContourBubbles(image);
        string stemi = p.path().stem();
        string rawdir_str;
        rawdir.toUTF8String(rawdir_str);
        string stemj = stemi + ".json";
        cout << "json file: " << rawdir_str + "/" + stemj << endl;
        if (!image.data){
            cout << "image had no data: " << p.path().string() << endl;
            return 1;
        }
        string text_info("");
        if (tess || token.length() != 0 && !translate) {
            cout << "OCRing" << endl;
            for (int i=0;i<brects.size();i++){
                brects[i].image = new cv::Mat(); // must be set here in order to bring back the images.
            }
			brectsEnhance(image, brects);
            cout << "done enhancing" << endl;
            cout << "about to enter brectsOCR" << endl;
            brectsOCR(tess, token, brects, worklang, p.path().extension());
            brectsCleanText(brects);
            brectsTranslate(brects, token, worklang, languages);
        }
        else {
			return 1;
            //-----------------load json data
            //cout << "loading previously generated text_info" << endl;
            //std::ifstream ifs(rawdir_str+ "/" + stemj);
            //text_info = string(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
            //ifs.close();
        }
        if (translate && token.length() != 0){
            cout << "Getting translation" << endl;
            brectsTranslate(brects, token, worklang, languages);
        }
        
        brectsCleanImage(image, cleaned, brects);
        brectsInsertText(cleaned, finished, brects);


        cout << "showing cleaned" << endl;
        if (!cleaned.empty()){
            /*cv::namedWindow(p.path().string()+"--cleaned", cv::WINDOW_NORMAL);
            cv::imshow(p.path().string()+ "--cleaned", cleaned);
            cv::waitKey();
            cv::destroyWindow(p.path().string()+"--cleaned");*/
        }

        cout << "showing finished" << endl;
        if (!finished.empty()){
            /*cv::namedWindow(p.path().string()+"--finished", cv::WINDOW_NORMAL);
            cv::imshow(p.path().string()+ "--finished", finished);
            cv::waitKey();
            cv::destroyWindow(p.path().string()+"--finished");*/
        }

        std::ofstream ofs;
        //cout << "output json file: " << rawdir_str + "/" + stemj << endl;
        //ofs.open(rawdir_str + "/" + stemj, std::ofstream::out | std::ofstream::trunc);
        //ofs << text_info << endl;
        //ofs.close(); 
        string lang = "en";
        string mangadir_str = "";
        mangadir.toUTF8String(mangadir_str);
        cout << "output image file: " << mangadir_str + "/" + lang + "/" + p.path().filename().string() << endl;
        cv::imwrite(mangadir_str + "/" + "cleaned" + "/" + p.path().filename().string(), cleaned);
        cv::imwrite(mangadir_str + "/" + lang + "/" + p.path().filename().string(), finished);
    }
    catch(const std::exception& e){
        std::ofstream errorlogfile;
        errorlogfile.open(p.path().string() + "-error.log", std::ofstream::out | std::ofstream::app);
        errorlogfile << e.what() << endl;
        errorlogfile.close();
        cout << "Error Scanlating page: " << p.path().string() << endl;
        std::cerr << e.what() << endl;
        return 1;
    }
    return 0;
}


void RunScanlation(UnicodeString token, bool tess, bool translate, string worklang,
                UnicodeString mangadir, string callback_location, vector<string> languages){
    if (mangadir.length() == 0) return;

    // get all images
    cout << "Scanlating: " << mangadir << endl;
    string path("");
    UnicodeString rawdir = mangadir + UnicodeString("/raw");
    rawdir.toUTF8String(path);
    int poolsize = 30;
    vector<std::thread> workerthreads;
    for (const auto& p : fs::recursive_directory_iterator(path)){
        if (p.path().extension() == ".jpg" ||  p.path().extension() == ".png" ||
            p.path().extension() == ".JPG" || p.path().extension() == ".PNG" || p.path().extension() == ".jpeg"){
            // use opencv to open image
            std::thread scanlator(ScanlatePage, token, tess, translate, worklang, mangadir, callback_location, languages, p);
            workerthreads.push_back(std::move(scanlator));
            while(workerthreads.size() >= poolsize){
                for (int i=0;i<workerthreads.size();i++){
                    if (workerthreads[i].joinable()){
                        workerthreads[i].join();
                        workerthreads.erase(workerthreads.begin() + i);
                    }
                }
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            //scanlator.join();
        }
    }
    for (auto& i : workerthreads){
        i.join();
    }
    cout << "Done Scanlating, exiting" << endl;
    // use opencv to open images in mangadirectory
}

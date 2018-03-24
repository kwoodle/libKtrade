#include "ktcurl.h"
#include "CodeMap.h"
#include <iostream>
#include <cstring>
#include <chrono>
#include <algorithm>
using std::cout;
using std::get;

using namespace std::chrono;
using namespace drk;

string EzCurl::froot{"/var/tmp/ktrade/"};
string EzCurl::suff{".html"};
const string Multi::froot{"/var/tmp/ktrade/"};
const string Multi::suff{".html"};

// EzCurl
// Default constructor sets some default options including
// follow redirects
// write response body to file
EzCurl::EzCurl(const string name)
        :flnm(froot+name)
{
    flnm += suff;
    init();
}

EzCurl::EzCurl(const string name, long to)
        :flnm(froot+name), timeout{to}
{
    flnm += suff;
    init();
}

//EzCurl::EzCurl(EzCurl& ez)
//        :curl{ez.curl} { }

EzCurl::~EzCurl()
{
//    if(flptr)
//        fclose(flptr);
    curl_easy_cleanup(curl);
}

//
// GetResponseFile
// Creates file to hold response to curl_easy_perform
// sets url
// calls curl_easy_perform
// returns file nam
// closes file
string EzCurl::GetResponseFile(const URL& url)
{
    //
    // response file
    FILE* flptr = fopen(flnm.c_str(), "w");
    if (!flptr) throw std::runtime_error{"Failed to open curl response file\n"};
    //
    // response file name
    codes.push(curl_easy_setopt(curl, CURLOPT_WRITEDATA, flptr));
    codes.push(curl_easy_setopt(curl, CURLOPT_URL, url.c_str()));
    //
    // set timeout
    codes.push(curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout));
    //
    // get response
    codes.push(curl_easy_perform(curl));
    if (codes.top()!=CURLE_OK) {
        check_codes();
        throw std::runtime_error{"EzCurl::GetResponseFile failed.\n"};
    }
    CURLcode res = curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &response_time);
    if (CURLE_OK==res) {
        cout << "Total CURL response_time = " << response_time << ".\n";
    }
    if (flptr)
        fclose(flptr);
    return flnm;
}

// private: get_code
// returns the top CURLcode from the codes stack
// or CURLE_OK if empty
CURLcode EzCurl::get_code()
{
    CURLcode ret{CURL_LAST};
    if (!codes.empty()) {
        ret = codes.top();
        codes.pop();
    }
    return ret;
}

// private: print_error
// prints error associated with CURLcode not equal to CURLE_OK
// to cout
void EzCurl::print_error(CURLcode res)
{
    size_t len = strlen(errbuf);
    fprintf(stderr, "\nlibcurl: (%d) ", res);
    if (len)
        fprintf(stderr, "%s%s", errbuf,
                ((errbuf[len-1]!='\n') ? "\n" : ""));
    else
        fprintf(stderr, "%s\n", curl_easy_strerror(res));
}

void EzCurl::check_codes()
{
    CURLcode tmp{CURLE_OK};
    while (!codes.empty()) {
        if (codes.top()==CURLE_OK) {
            codes.pop();
            continue;
        }
        else {
            tmp = codes.top();
            print_error(tmp);
            codes.pop();
        }
    }
    if (tmp!=CURLE_OK)
        throw std::runtime_error{"CURL failed to initialize"};
}

FILE* EzCurl::setup(URL url)
{
    //
    // response file
    FILE* flptr = fopen(flnm.c_str(), "wb");
    if (!flptr) throw std::runtime_error{"Failed to open curl response file\n"};
    //
    // response file name
    codes.push(curl_easy_setopt(curl, CURLOPT_WRITEDATA, flptr));
    codes.push(curl_easy_setopt(curl, CURLOPT_URL, url.c_str()));
    //
    // set timeout
    codes.push(curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout));
    return flptr;
}

void EzCurl::set_url(const URL& url)
{
    codes.push(curl_easy_setopt(curl, CURLOPT_URL, url.c_str()));
    check_codes();
}

void EzCurl::finish()
{
    CURLcode res = curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &response_time);
    if (CURLE_OK==res) {
        cout << "Total CURL response_time = " << response_time << ".\n";
    }
//    if (flptr) fclose(flptr);
}

void EzCurl::init()
{
    //
    curl = curl_easy_init();
    if (!curl) throw std::runtime_error{"Failure in curl_easy_init\n"};

    errbuf[0] = 0;
    //
    // follow redirects
    codes.push(curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L));
    //
    // write response to file
    codes.push(curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, nullptr));
    //
    // buffer to return error code strings
    codes.push(curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf));
    check_codes();
}

// Iterate through urls from the beginning
// Return a CURLMap with size >= urls.size() and size < Multi:max
// After completion, urls contains any remaining unhandled URLs
//
ResVec Multi::do_perform(URLVec& urls)
{
    ResVec tmp;
    int transfers_running{0};
    auto t = steady_clock::now();
    do {
        int transfers = transfers_running;
        curl_multi_wait(curlm, NULL, 0, 1000, NULL);  // 1000 ms max wait
        curl_multi_perform(curlm, &transfers_running);
        if (transfers>transfers_running) {
            CURLMsg* m;
            do {
                int msgq = 0;
                m = curl_multi_info_read(curlm, &msgq);
                if (m && (m->msg==CURLMSG_DONE)) {
                    CURLcode res = m->data.result;
                    CURL* e = m->easy_handle;
                    char* urlc = NULL;
                    curl_easy_getinfo(e, CURLINFO_EFFECTIVE_URL, &urlc);
                    URL url{urlc};
                    if (res!=CURLE_OK) {
                        cout << "Perform failed for url " << url << ": " << codemap.at(res) << '\n';
                    }
                    if (!url.empty()) cout << "CURLMSG_DONE for url " << url << "\n";
                    auto fnd = std::find(urls.begin(), urls.end(), url);
                    if(fnd != urls.end())
                        urls.erase(fnd);
                    transfers--;

                    // using CURLMap = std::map<CURL*, tuple<URL, FILE*, DocFile>>;
                    auto f = get<1>(curl_map.find(e)->second);
                    if (f) fclose(f);
                    curl_multi_remove_handle(curlm, e);
                    auto d = get<2>(curl_map.find(e)->second);
                    tmp.emplace_back(url, d);
                    continue;
                }
            }
            while (m);
        }
    }
    while (transfers_running);
    cout << "multi_perform took " << duration_cast<milliseconds>(steady_clock::now()-t).count() << " ms\n";//<<
    return tmp;
}

void Multi::close_files()
{
    for (auto& e : curl_map) {
        auto& t = e.second;
        auto& f = std::get<1>(t);
        if (f) {
            fclose(f);
            f = nullptr;
        }
    }
}
Multi::~Multi()
{
    for (auto& e : curl_map) {
        curl_easy_cleanup(e.first);
    }
    curl_multi_cleanup(curlm);
    curl_global_cleanup();
}

Multi::Multi(const string& nm)
        :curlm{curl_multi_init()}, name{nm}
{
    for (int i = 0; i<max_curls; ++i) {
        CURL* curl = curl_easy_init();
        //
        // user agent
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "DrK");
        //
        // follow redirects
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        //
        // set timeout
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
        //
        // write response to file
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, nullptr);

        curl_multi_add_handle(curlm, curl);

        //
        // using CURLMap = std::map<CURL*, tuple<URL, FILE*, DocFile>>;
        curl_map.emplace(curl, std::make_tuple("", nullptr, ""));
    }
}

// Initialize curl_map for multi_curl call for some or all of urls
// Step through urls
void Multi::load(URLVec& urls)
{
    auto u = urls.begin();
    auto ue = urls.end();
    auto e = curl_map.begin();
    auto ee = curl_map.end();

    int i{0};
    while (u!=ue && e!=ee) {
        auto url = std::get<0>(e->second) = *u;

        string flnm{froot+name+std::to_string(i++) += suff};
        FILE* flptr = fopen(flnm.c_str(), "w");
        if (!flptr) throw std::runtime_error{"Failed to open curl response file\n"};

        get<1>(e->second) = flptr;
        get<2>(e->second) = flnm;
        curl_easy_setopt(e->first, CURLOPT_WRITEDATA, flptr);
        curl_easy_setopt(e->first, CURLOPT_URL, url.c_str());
        curl_multi_add_handle(curlm, e->first);
        ++e;
        ++u;
    }
    while (e != ee) {
        curl_multi_remove_handle(curlm, e->first);
        ++e;
    }
    urls = {u,ue};
}


void Multi::multi_perform()
{
    for (auto& e : curl_map);

}

Resp Multi::do_one(URL url)
{
    auto e{curl_map.begin()};
    auto c{e->first};
    string flnm{froot+name += suff};
    FILE* flptr = fopen(flnm.c_str(), "w");
    if (!flptr || !c) throw std::runtime_error{"Multi::do_one Failed\n"};

    curl_easy_setopt(e->first, CURLOPT_WRITEDATA, flptr);
    curl_easy_setopt(e->first, CURLOPT_URL, url.c_str());
    CURLcode cd{curl_easy_perform(c)};
    if(cd != CURLE_OK) {
        string err{"Multi::do_one failed for url " + url + codemap.at(cd)};
        throw std::runtime_error{err};
    }
    if(flptr) fclose(flptr);
    return {url, flnm};
}


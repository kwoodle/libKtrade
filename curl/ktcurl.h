//
// Created by kwoodle on 1/10/18.
//
// Encapsulate CURL
//

#ifndef LIBKTRADE_LIBRARY_H
#define LIBKTRADE_LIBRARY_H

#include <curl/curl.h>
#include <curl/multi.h>
#include <stdexcept>
#include <stack>
#include <vector>
#include <set>
#include <map>
#include <utility>
#include <array>
#include <unordered_map>

using std::string;
using std::stack;
using std::vector;
using std::tuple;
using std::pair;

namespace drk {

using Xpath = std::string;
using URL = std::string;
using DocFile = std::string;
using Resp = pair<URL, DocFile>;
using ResVec = vector<Resp>;
using CURLVec = vector<tuple<CURL*, URL, FILE*, DocFile>>;
using CURLMap = std::unordered_map<CURL*, tuple<URL, FILE*, DocFile>>;
using URLVec = vector<URL>;
using NmMap = std::map<URL, string>;
using FILEVec = vector<FILE*>;
using Errbuf = std::array<char, CURL_ERROR_SIZE>;
//extern URLVec test_urls;

// Encapsulate easy_curl interface
//
class EzCurl {
public:
    explicit EzCurl(string name);

    explicit EzCurl(string name, long to);

    explicit EzCurl(string name, bool verbose);

    EzCurl(EzCurl &ez) = delete;

    EzCurl &operator=(EzCurl &) = delete;

    ~EzCurl();

    CURLcode get_code();

    FILE *setup(URL);

    void set_url(const URL &);

    string GetResponseFile(const URL& url);

private:
    CURL* curl;
    stack<CURLcode> codes;
    string flnm;
    char errbuf[CURL_ERROR_SIZE];

    void print_error(CURLcode);

    void check_codes();

    CURL *get_curl() {
        return curl;
    }

    void init(bool verb = false);

    void finish();

    double response_time{9999.};
    long timeout{0L};
public:
    static string froot;
    static string suff;

};

    class Multi {
    public:
        explicit Multi(const string &nm, bool verb = false);

        ~Multi();

        void load(URLVec &v);

        CURLM *curl() { return curlm; }

        ResVec do_perform(URLVec &urls);

        Resp do_one(URL);

        void multi_perform();

        const CURLMap &curlmap() const {
            return curl_map;
        }

        static const string froot;
        static const string suff;

    private:
        CURLM *curlm;
        string name;
        long timeout{1L}; // max wait time for all easy curls
        CURLMap curl_map;
        URLVec ToDo;
        int max_curls{10};        // max number of easy handles

        void close_files();
    };
}

#endif

/*
 *     curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, nullptr);
                string fname{"nasd.html"};
                FILE* filptr = fopen(fname.c_str(), "w");
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, filptr);
                curl_easy_setopt(curl, CURLOPT_URL, loc.c_str());
                CURLcode res = curl_easy_perform(curl);

 */
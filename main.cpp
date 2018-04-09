//
// Created by kwoodle on 1/22/18.
//

#include <Python.h>
#include <map>
#include <iostream>
#include <sstream>
#include "curl/ktcurl.h"
#include "curl/HtmlDoc.h"
#include "curl/CodeMap.h"
#include "curl/parse.h"
#include "curl/TestURLs.h"
#include "Util.h"
#include <boost/filesystem.hpp>

using std::string;
using std::cout;
using std::get;
using std::stringstream;
namespace boofs = boost::filesystem;        // to get name of executable
using namespace drk;

int main(int argc, char* argv[])
{
    URLVec urls{"http://example.com", "http://examplex@.com", "https://www.iana.org"};
    // Get name of this application
    // to be used to create somewhat unique filename
    // in which to store data
    //
    boofs::path app_path{argv[0]};
    string app_name = app_path.filename().string();
    Multi curlm(app_name);


    // Initialize multi_curl with vector of urls to connect to
    // and a name which acts as a base for files to contain received data
    //
    // urls = test_urls;
    while(!urls.empty()) {
        curlm.load(urls);
        ResVec results = curlm.do_perform(urls);

        // using CURLMap = std::map<CURL*, tuple<URL, FILE*, DocFile>>;
        for (auto e : results) {
            auto u = e.first;
            auto d = e.second;
            HtmlDoc doc{d};

            Xpath xpath{"//p"}; // xpath for HTML paragraph <p> anywhere in document
            doc.GetNodeset(xpath);
            string para;
            if (doc.result()) {
                // get content string for first paragraph
                para = doc.GetNodeString(doc.nodeset()->nodeTab[0]->xmlChildrenNode, 1);
                cout << "From first HTML paragraph in " << u << "\n";
                cout << para << "\n\n";
            }
            else {
                cout << "No match for xpath " << xpath << " in doc from " << u << "\n\n";
            }
        }
    }
    // Test histogram using input data file from gaussian gnuplot_test.data
    string tstfile{"../gnuplot/gnuplot_test.data"};
    std::ifstream ifile{tstfile};
    if (!ifile) {
        cout << "Failed to open "+tstfile+"\n";
        return 1;
    }
    vector<double> test_hist;
    string line;
    while (getline(ifile, line)) {
        float wd;
        stringstream s(line);
        while (s >> wd)
            test_hist.push_back(wd);
    }
//    test_hist = {1.,2.,3.,4.,5.5,2.2,3.};
    pair<vector<int>, string> test_out = drk::histogram("test", test_hist);
    return 0;
}

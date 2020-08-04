//
// Created by kwoodle on 1/22/18.
//

#include <map>
#include <iostream>
#include <ktrade/ktcurl.h>
#include <ktrade/HtmlDoc.h>
#include <ktrade/CodeMap.h>
//#include <ktrade/parse.h>
#include <ktrade/TestURLs.h>
#include <ktrade/Util.h>
#include <boost/filesystem.hpp>

using std::string;
using std::cout;
using std::get;
using std::stringstream;
namespace boofs = boost::filesystem;        // to get name of executable
using namespace drk;

int main(int argc, char *argv[]) {
    URLVec urls{"http://example.com", "http://examplex@.com", "https://www.iana.org"};
//    urls.push_back("http://old.nasdaq.com/symbol/AAPL/time-sales?time=1");
    urls.push_back("http://old.nasdaq.com/symbol/AAPL");
    urls = test_urls;
    // Get name of this application
    // to be used to create somewhat unique filename
    // in which to store data
    //
    boofs::path app_path{argv[0]};
    string app_name = app_path.filename().string();
    Multi curlm(app_name/*, true*/);


    // Initialize multi_curl with vector of urls to connect to
    // and a name which acts as a base for files to contain received data
    //
    // urls = test_urls;
    while (!urls.empty()) {
        curlm.load(urls);
        ResVec results = curlm.do_perform(urls);

        // using CURLMap = std::map<CURL*, tuple<URL, FILE*, DocFile>>;
        for (auto e : results) {
            auto u = e.first;
            auto d = e.second;
            HtmlDoc doc{d};

            Xpath xpath{"//title"}; // xpath for HTML <title>
            doc.GetNodeset(xpath);
            string titl;
            if (doc.result()) {
                // get content string for first paragraph
                titl = doc.GetNodeString(doc.nodeset()->nodeTab[0]->xmlChildrenNode, 1);
                cout << "From HTML title in " << u << "\n";
                cout << titl << "\n\n";
            } else {
                cout << "No match for xpath " << xpath << " in doc from " << u << "\n\n";
            }
        }
    }
    std::string string1{"../gnuplot/gnuplot_test.data"};
    ifstream ifstream1(string1);
    if (!ifstream1) {
        cout << "Failed to open " << string1 << "\n";
    }
    string test_slurp{drk::slurp(ifstream1)};
    cout << "Slurped string from " << string1 << " has length " << test_slurp.length() << "\n";

    bool tst = drk::is_workday();
    string tst_out = tst ? " is " : " is not ";
    string t_out = "\nToday" + tst_out + "a workday\n";
    cout << t_out;
    return 0;
}

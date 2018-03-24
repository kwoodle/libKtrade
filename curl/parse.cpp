//
// Created by kwoodle on 1/28/18.
//
// This file executed on 1/28/18
// created CodeMap.h by running write_file()
// no need to run again
// CodeMap.h being exported

#include "parse.h"
#include <fstream>
#include <iostream>
#include <regex>

using std::ifstream;
using std::ofstream;
using std::regex;
using std::cout;
using std::endl;
using std::smatch;
namespace drk {
static string fnam{"../CURLCodes.cpp"};
CodeMap parse()
{
    CodeMap ret_map;
    std::ifstream is(fnam);
    if (!is) {
        std::cerr << "Failed to open " << fnam << ".\n";
        exit(1);
    }
    string line;
    int lineno{0};
    int codes{0};

//  CURLE_REMOTE_ACCESS_DENIED,    /* 9 */
//  CURLE_FTP_ACCEPT_FAILED,       /* 10 */
//  CURLE_FTP_WEIRD_PASS_REPLY,    /* 11 */

    string pat_str{R"(\s+(CURLE_\w+),.*\/\*\s+(\d+)\s+.*\*\/)"};
    std::regex pat{pat_str};
    while (std::getline(is, line)) {
        ++lineno;
        std::smatch m;
        if (regex_match(line, m, pat)) {
            if (m.size()==3) {
                auto sub1{m[1].str()};
                CURLcode sub2{stoi(m[2].str())};
                ret_map[sub2] = sub1;
                ++codes;
            }
        }
    }
    cout << codes << " codes from " << lineno << " lines in file\n";
    return ret_map;
}

//
// Generate CodeMap files

const string outh{"../CodeMap.h"};
const string head1{R"%%(// This file was created by drk
//

)%%"};

const string head2{R"%%(
#ifndef LIBKTRADE_CODEMAP_H
#define LIBKTRADE_CODEMAP_H

#include <map>
#include <curl/curl.h>
#include <string>

using std::string;

namespace drk {
using CodeMap = std::map<CURLcode, string>;
const CodeMap codemap
{
)%%"};

const string cpp2{R"%%(

#include "CodeMap.h"
using namespace drk;

CodeMap codemap
{
)%%"};

void write_file()
{
    ofstream ofstreamh{outh};
    if (!ofstreamh) {
        std::cerr << "Failed to open " << outh << ".\n";
        exit(1);
    }
    ofstreamh<<head1<<head2;
//    ofstreamh.close();


    CodeMap codeMap{parse()};
    size_t i{0};
    size_t ie{codeMap.size()-1};
    for (auto& item : codeMap) {
        ofstreamh<<"{ "<<item.second<<", "<<'"'<<item.second<<'"'<<"}";
        if(i<ie) {
            ofstreamh << ",\n";
        }
        else {
            ofstreamh << "\n";
        }
        ++i;
    }
    ofstreamh<<"};\n}\n#endif //LIBKTRADE_CODEMAP_H";
    ofstreamh.close();
}
}
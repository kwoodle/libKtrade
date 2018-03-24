//
// Created by kwoodle on 1/28/18.
//
// Parse a file created from CURLcode enum in libcurl's curl.h

#ifndef LIBKTRADE_PARSE_H
#define LIBKTRADE_PARSE_H

#include <map>
#include <curl/curl.h>
#include <string>

using std::string;
using CodeMap = std::map<CURLcode, string>;
namespace drk {
CodeMap parse();
void write_file();
}
#endif //LIBKTRADE_PARSE_H

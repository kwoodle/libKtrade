//
// Created by kwoodle on 1/11/18.
//

#ifndef LIBKTRADE_HTML_H
#define LIBKTRADE_HTML_H

#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>
#include <string>
#include "ktcurl.h"

using std::string;
using drk::Xpath;

class HtmlDoc {
public:
    // create from htmlReadFile
    HtmlDoc(const string& file, const string& enc = string("UTF-8"),
            htmlParserOption h = (HTML_PARSE_RECOVER | HTML_PARSE_NOERROR |
                    HTML_PARSE_NOWARNING | HTML_PARSE_NOBLANKS));

    void GetNodeset(const Xpath& xpath);
    const xmlXPathObjectPtr& result();
    const xmlNodeSetPtr& nodeset();
    //
//    void ParseFile(const string &file, const string &enc = string("UTF-8"),
//                   htmlParserOption h = (HTML_PARSE_RECOVER | HTML_PARSE_NOERROR | HTML_PARSE_NOBLANKS));
    //
    string GetNodeString(const xmlNode* list, int inLine = 0);
    string GetProp(string);
    string GetFirstPar();
    ~HtmlDoc();

private:
    htmlDocPtr DocPtr{nullptr};
    xmlNodeSetPtr NodeSet{nullptr};
    xmlXPathObjectPtr XPathObj{nullptr};
    xmlXPathObjectPtr getnodeset(xmlChar* xpath);
    const Xpath par{"//p"};
};

#endif //LIBKTRADE_HTML_H

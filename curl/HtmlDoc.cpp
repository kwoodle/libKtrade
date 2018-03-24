//
// Created by kwoodle on 1/11/18.
//

#include "HtmlDoc.h"

HtmlDoc::HtmlDoc(const string& f, const string& enc,
        htmlParserOption h)
{
    DocPtr = htmlReadFile(f.c_str(), enc.c_str(), h);
}

HtmlDoc::~HtmlDoc()
{
    if (XPathObj)
        xmlXPathFreeObject(XPathObj);
    xmlFreeDoc(DocPtr);
}

xmlXPathObjectPtr HtmlDoc::getnodeset(xmlChar* xpath)
{
    xmlXPathContextPtr context;
    xmlXPathObjectPtr result;

    context = xmlXPathNewContext(DocPtr);
    if (context==nullptr) {
        printf("Error in xmlXPathNewContext\n");
        return nullptr;
    }
    result = xmlXPathEvalExpression(xpath, context);
    xmlXPathFreeContext(context);
    if (result==nullptr) {
        printf("Error in xmlXPathEvalExpression\n");
        return nullptr;
    }
    if (xmlXPathNodeSetIsEmpty(result->nodesetval)) {
        xmlXPathFreeObject(result);
//        printf("No result\n");
//        throw std::runtime_error{"No result from xpath"};
        return nullptr;
    }
    return result;
}

void HtmlDoc::GetNodeset(const string& xpath)
{
    auto* axp = (xmlChar*) xpath.c_str();
    XPathObj = getnodeset(axp);
    if (XPathObj)
        NodeSet = XPathObj->nodesetval;
}

string HtmlDoc::GetNodeString(const xmlNode* list, int inLine)
{
    xmlChar* tmp = xmlNodeListGetString(DocPtr, list, inLine);
    string ret;
    if (tmp)
        ret = string((const char*) tmp);
    xmlFree(tmp);
    return ret;
}

string HtmlDoc::GetProp(string s)
{
    xmlChar* tmp = xmlGetProp(nodeset()->nodeTab[0], (xmlChar*) s.c_str());
    string ret = string((const char*) tmp);
    xmlFree(tmp);
    return ret;
}

const xmlXPathObjectPtr& HtmlDoc::result()
{
    return XPathObj;
}
const xmlNodeSetPtr& HtmlDoc::nodeset()
{
    return NodeSet;
}

string HtmlDoc::GetFirstPar()
{
    GetNodeset(par);
    string out{"No first paragraph found.\n"};
    if (result()) {
        // get content string for first paragraph
        out = "First paragraph: ";
        out += GetNodeString(nodeset()->nodeTab[0]->xmlChildrenNode, 1) + "\n";
    }
    return out;
}

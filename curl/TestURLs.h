//
// Created by kwoodle on 1/29/18.
//
// test_urls is a set of forty-some web addresses grabbed from the libCurl example "ten at a time"
// and put into the file TestURLs.cpp

#ifndef LIBKTRADE_TESTURLS_H
#define LIBKTRADE_TESTURLS_H

#include "ktcurl.h"
namespace drk {
const URLVec test_urls{
        "http://www.microsoft.com",
        "http://www.opensource.org",
        "http://www.google.com",
        "http://www.yahoo.com",
        "http://www.ibm.com",
        "http://www.mysql.com",
        "http://www.oracle.com",
        "http://www.ripe.net",
        "http://www.iana.org",
        "http://www.amazon.com",
        "http://www.netcraft.com",
        "http://www.heise.de",
        "http://www.chip.de",
        "http://www.ca.com",
        "http://www.cnet.com",
        "http://www.news.com",
        "http://www.cnn.com",
        "http://www.wikipedia.org",
        "http://www.dell.com",
        "http://www.hp.com",
        "http://www.cert.org",
        "http://www.mit.edu",
        "http://www.nist.gov",
        "http://www.ebay.com",
        "http://www.playstation.com",
        "http://www.uefa.com",
        "http://www.ieee.org",
        "http://www.apple.com",
        "http://www.symantec.com",
        "http://www.zdnet.com",
        "http://www.fujitsu.com",
        "http://www.supermicro.com",
        "http://www.hotmail.com",
        "http://www.ecma.com",
        "http://www.bbc.co.uk",
        "http://news.google.com",
        "http://www.foxnews.com",
        "http://www.msn.com",
        "http://www.wired.com",
        "http://www.sky.com",
        "http://www.usatoday.com",
        "http://www.cbs.com",
        "http://www.nbc.com",
        "http://slashdot.org",
        "http://www.bloglines.com",
        "http://www.techweb.com",
        "http://www.newslink.org",
        "http://www.un.org",
};

    const URLVec https_urls{
            "https://www.nasdaq.com/symbol/aapl/time-sales",
            "https://start.fedoraproject.org/",
            "https://en.wikipedia.org"
    };
}
#endif //LIBKTRADE_TESTURLS_H

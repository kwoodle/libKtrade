//
// Created by kwoodle on 3/28/18.
//

#include <cmath>
#include "Util.h"
#include <fstream>
#include <iostream>
#include "boost/date_time/posix_time/posix_time.hpp"
#include <iostream>
//using namespace boost::posix_time
//using namespace boost::gregorian;

using std::ofstream;

string drk::get_from_cmd(string cmd) {
    string reslt;
    const int max_buffer{512};
    char buffer[max_buffer];
    cmd.append(" 2>&1");
    FILE *stream{popen(cmd.c_str(), "r")};
    if (stream) {
        while (!feof(stream))
            if (fgets(buffer, max_buffer, stream) != nullptr) reslt.append(buffer);
        pclose(stream);
    }
    return reslt;
}

string drk::slurp(ifstream &in) {
    return dynamic_cast<stringstream const &>(stringstream() << in.rdbuf()).str();
}


bool drk::is_workday() {
//    string cmd = "/usr/bin/php -R " + drk::holiday_script;
    string cmd = "/usr/bin/php /usr/local/include/ktrade/workday_script.php";
    string res = drk::get_from_cmd(cmd);
    return (res == "true" ? true : false);
}

string drk::Logger::time_now() {
    time_t tim = time(NULL);
    tm *now = localtime(&tim);
    stringstream sbuft;
    sbuft << now->tm_hour << ":" << now->tm_min;
    return sbuft.str();
}

string drk::Logger::logdir="/usr/local/lib/ktrade/logs/";

string drk::Logger::date_time_now(bool date_only) {
    time_t tim = time(NULL);
    tm *now = localtime(&tim);
    stringstream sbuf;
    sbuf << now->tm_year + 1900 << "-" << now->tm_mon + 1 << "-" << now->tm_mday;
    if (date_only) {
        return sbuf.str();
    }
    stringstream sbuft;
    sbuft << now->tm_hour << ":" << now->tm_min;
    return sbuf.str() + "::" + sbuft.str();
}

string drk::Logger::concat_strings(initializer_list<string> items, string sep) {
    string ret;
    auto itr=items.begin();
    for(itr;itr!=items.end()-1;++itr){
        ret+=*itr + sep;
    }
    ret+=*itr;
    return ret;
}

string drk::Logger::get_host() {
    string ret("UNKNOWN_HOST");
    int host = gethostname(hostbuffer, sizeof(hostbuffer));
    if(!host)
        ret = string(hostbuffer);
    return ret;
}
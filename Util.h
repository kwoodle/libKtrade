//
// Created by kwoodle on 3/28/18.
//

#ifndef LIBKTRADE_UTIL_H
#define LIBKTRADE_UTIL_H

#include <string>
#include <cstdio>
#include <vector>
#include <utility>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <initializer_list>
#include <boost/filesystem.hpp>

namespace boofs = boost::filesystem;        // to get name of executable

using std::string;
using std::vector;
using std::pair;
using std::ifstream;
using std::stringstream;
using std::initializer_list;
namespace drk {
    string get_from_cmd(string cmd);

    string slurp(ifstream &in);

    bool is_workday();

    const string holiday_script{R"%%(
require_once("Date/Holidays.php");
$today = new Date;
$holidays = Date_Holidays::factory("USA");
$hol = $holidays->isHoliday($today);
$dow = $today->getDayofWeek();
$isWorkday = ($dow !=0 and $dow != 6 and !hol);
$os = $isWorkday ? $today->getDate() . " is a workday\n" : $today->getDate() . " is not a workday\n";
#echo $os;
$os2 = $isWorkday ? "true\n" : "false\n";
echo $os2;
)%%"};

    class Logger : public std::fstream {
    public:
        explicit Logger(char *argv[], std::initializer_list<string> name = {""}) {
            boofs::path app_path{argv[0]};
            app_name = app_path.filename().string();
            set_host();
            if (name.begin()->length() == 0) {
                use_default_name();
            } else {
                filename = concat_strings(name);
            }
            std::fstream::open(filename.c_str(), std::fstream::out);
            *this << tstamp() << " Starting " << std::endl;
        }

        ~Logger() {
            *this << tstamp() << " Done " << std::endl;
            std::fstream::close();
        }

        void log(const string &msg) {
            *this << tstamp() << " " << msg << std::endl;
        }

        string get_app_name() {
            return app_name;
        }

        string get_filename() { return filename; }
        string show_host() {
            return host;
        }

    protected:
        string time_now();
        string tstamp() {
            return concat_strings({app_name, time_now()});
        }
        string date_time_now(bool date_only = false);
        string concat_strings(initializer_list<string>, string sep = "::");

        string get_host();

        void set_filename(string f) {
            filename = logdir;
            filename.append(f);
        }

        void set_filename(std::initializer_list<string> list) {
            filename = logdir;
            filename.append(concat_strings(list));
        }

        void use_default_name() {
            filename = logdir;
            filename.append(concat_strings({date_time_now(), get_app_name(), get_host()}));
        }
        void set_host() {
            host = get_host();
        }
        string app_name = "dflt_app";
        char hostbuffer[256];
        string filename = "dflt_file";
        string host = "dflt_host";
        static string logdir;
    };
}
#endif //LIBKTRADE_UTIL_H

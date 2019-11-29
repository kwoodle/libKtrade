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

using std::string;
using std::vector;
using std::pair;
using std::ifstream;
using std::stringstream;

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
}
#endif //LIBKTRADE_UTIL_H

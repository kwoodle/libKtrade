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
    extern string get_from_cmd(string cmd);

    string slurp(ifstream& in);
}
#endif //LIBKTRADE_UTIL_H

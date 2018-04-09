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

using std::string;
using std::vector;
using std::pair;
namespace drk {
    extern string get_from_cmd(string cmd);

    template<typename T>
    pair<vector<int>, string> histogram(const string& name, const vector<T>& vec, int nbins = 100);

    class GPlot {

    };
}

#include "Util.cpp"
#endif //LIBKTRADE_UTIL_H

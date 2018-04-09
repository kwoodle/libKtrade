//
// Created by kwoodle on 3/28/18.
//
#ifndef LIBKTRADE_UTIL_C
#define LIBKTRADE_UTIL_C

#include <cmath>
#include "Util.h"
#include <fstream>
#include <iostream>

using std::ofstream;
string drk::get_from_cmd(string cmd)
{
    string reslt;
    const int max_buffer{512};
    char buffer[max_buffer];
    cmd.append(" 2>&1");
    FILE* stream{popen(cmd.c_str(), "r")};
    if (stream) {
        while (!feof(stream))
            if (fgets(buffer, max_buffer, stream)!=nullptr) reslt.append(buffer);
        pclose(stream);
    }
    return reslt;
}

template<typename T>
pair<vector<int>, string> drk::histogram(const string& name, const vector<T>& vec, int nbins)
{
//    nbins = nbins > vec.size() ? vec.size() : nbins;
    vector<int> outv(nbins+1, 0);
    auto res{std::minmax_element(vec.begin(), vec.end())};
    T min{vec.at((res.first-vec.begin()))};
    T max{vec.at((res.second-vec.begin()))};
    T delta{max-min};
    T width{delta/nbins};
    for (auto x: vec) {
        T frac{(x-min)/delta};
        int inc{floor(frac*(nbins))};
        ++outv.at(inc);
    }
    ofstream gout{"../test.gp"};
    if (!gout) {
        std::cerr << "Failed to open test.gp\n";
    }
    for (auto l:outv) {
        gout << l << "\n";
    }
    pair<vector<int>, string> out;
    return out;
}

#endif //LIBKTRADE_UTIL_C


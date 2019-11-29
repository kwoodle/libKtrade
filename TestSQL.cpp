//
// Created by kwoodle on 2/15/18.
//

#include "sql/Ksql.h"
#include "TestSQL.h"
#include "Util.h"
#include <ktrade/nlohmann/json.hpp>

using namespace drk;
using json = nlohmann::json;

//using namespace ::mysqlx;
using std::cout;
using std::string;

int main(int argc, char** argv)
{
    cout << "Testing SQL from libKtrade\n";
    KSql kSql;
    auto res = kSql.ExcuteQuery("select now() as t, version() as v");
    while (res->next()) {
        std::cout << "now() = " << res->getString("t") << std::endl;
        std::cout << "version() = " << res->getString("v") << std::endl;
    }
    cout << "\n";
    delete res;
    test_cols(kSql, "cookbook", "profile");
    cout << "Testing DisplayTable from libKtrade\n";
    string prof = kSql.DisplayTable("cookbook", "profile");
    cout << prof;
    /*
    prof = kSql.DisplayTable("test", "CD1_2018");
    cout<<prof;
     */
    cout << "Testing DispalyTable done\n";
    cout << "Testing Pipes to mysql and gnuplot from libKtrade\n";
    test_pipes();
    return 0;
}

void test_pipes()
{
    string mysq{"/usr/bin/mysql -e 'select year(dob) as `DOB`, year(lastvoteddate) as `lastvoted` from ward4active "
                "where lastvoteddate is not NULL' test > ../gnuplot/votedata"};
    string myout{get_from_cmd(mysq)};
    cout << myout << "\n";
//    cout << "Testing Pipe to mysql from libKtrade done\n";
    string gplot = R"%%(/usr/bin/gnuplot --persist)%%";
    cout << "\n" << gplot << "\n";
    FILE* GNU;
    GNU = popen(gplot.c_str(), "w");
    if (GNU==nullptr) {
        std::cerr << "Error opening pipe to GNU plot!\n";
        exit(1);
    }
    string cmd = R"%%(
col = "DOB"
col2 = "lastvoted"
stats '../gnuplot/votedata' using col name "D" nooutput
stats '../gnuplot/votedata' using col2 name "l" nooutput
min = D_min
max = D_max
minl = l_min
maxl = l_max
)%%";
    fprintf(GNU, cmd.c_str());
    string cmd2 = R"%%(set boxwidth 1
set style fill pattern 4
set ylabel 'counts'
set multiplot layout 2,1 title 'ward4active'
#unset key
set key left
set yrange [0:*]
set xrange [min-1:max+1]
plot '../gnuplot/votedata' using (column(col)):(1) smooth frequency with boxes lc 1 title col
set xrange [minl-1:maxl+1]
set style fill pattern 5
plot '../gnuplot/votedata' using (column(col2)):(1) smooth frequency with boxes lc 2 title col2
unset multiplot
#pause -1 "Hit return to continue"
)%%";
    fprintf(GNU, cmd2.c_str());
    pclose(GNU);
    string script{cmd+cmd2};
    string scrname{"../gnuplot/script.gnu"};
    std::ofstream scrs(scrname);
    if (!scrs) {
        std::cerr << "Failed to open " << script << "\n";
        return;
    }
    scrs << script;
};

void test_cols(KSql& kSql, const string& schema, const string& table)
{
    Cols pcols{kSql.get_cols(schema, table)};
    cout << "Testing get_cols from libKtrade\n";
    cout << cols_str << "\n\n";
    for (int i = 0; i<pcols.size(); ++i) {
        cout << i << "\t" << pcols[i].first << "\t" << pcols[i].second << "\n";
    }
    cout << "test_cols done\n";
    cout << "\n";
}

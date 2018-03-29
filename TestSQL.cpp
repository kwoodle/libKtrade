//
// Created by kwoodle on 2/15/18.
//

#include "sql/Ksql.h"
#include "TestSQL.h"
#include "Util.h"

using namespace drk;

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
    cout << "Testing Pipe to mysql from libKtrade\n";
    string mysq{"/usr/bin/mysql -H -e 'select * from profile limit 10' cookbook > test.html"};
    string myout{get_from_cmd(mysq)};
    cout << myout << "\n";
    cout << "Testing Pipe to mysql from libKtrade done\n";
    return 0;
}

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

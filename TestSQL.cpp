//
// Created by kwoodle on 2/15/18.
//

#include "sql/Ksql.h"

using namespace drk;

//using namespace ::mysqlx;
using std::cout;
using std::string;
int main(int argc, char** argv)
{
    KSql kSql;
    auto res = kSql.ExcuteQuery("select now() as t, version() as v");
    while (res->next()) {
        std::cout << "now() = " << res->getString("t") << std::endl;
        std::cout << "version() = " << res->getString("v") << std::endl;
    }

    Cols pcols{kSql.get_cols("cookbook", "profile")};
    for(int i=0; i<pcols.size(); ++i) {
        cout<<i<<"\t"<<pcols[i].first<<"\t"<<pcols[i].second<<"\n";
    }
    string q{"insert into cookbook.profile set "};
    q += pcols[1].first + " = 'Betty'";
    kSql.Execute(q);
    q = "select name from cookbook.profile";
    res = kSql.ExcuteQuery(q);
    while (res->next()){
        cout<<res->getString("name")<<"\n";
    }
    return 0;
}


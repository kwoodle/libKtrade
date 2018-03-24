//
// Created by kwoodle on 2/15/18.
//

#include <cstring>
#include "Ksql.h"
#include <regex>

using namespace drk;

KSql::KSql(const string& h, const string& u,
        const string& p, const string& d)
        :
        host{h}, user{u}, pass{p},
        con{get_driver_instance()->connect(h, u, p)},
        stmt{con->createStatement()}
{
    if (!d.empty()) {
        db = d;
        con->setSchema(db);
    }
}

void KSql::set_table(const string& s)
{
    con->setSchema(s);
}

void KSql::Execute(const string& s)
{
    stmt->execute(s);
}

RsltSet* KSql::ExcuteQuery(const string& s)
{
    return stmt->executeQuery(s);
}

Cols KSql::get_cols(const string& table_schema, const string& table_name)
{
    Cols out;
    string s1{cols_str};
    std::regex pat1{tbl_sch};
    std::regex pat2{tbl_nam};
    std::smatch matches;
    string s2{std::regex_replace(s1, pat1, table_schema)};
    string sql{std::regex_replace(s2, pat2, table_name)};
    auto res{ExcuteQuery(sql)};
    while(res->next()) {
        int i{res->getInt("ordinal_position")};
        string cn{res->getString("column_name")};
        string ct{res->getString("column_type")};
        out.push_back(std::make_pair(cn,ct));
    }
    delete res;
    return out;
}


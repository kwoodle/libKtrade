//
// Created by kwoodle on 2/15/18.
//

#include <cstring>
#include "Ksql.h"
#include <regex>
//#include <vector>

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

void KSql::set_schema(const string& s)
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
    while (res->next()) {
        int i{res->getInt("ordinal_position")};
        string cn{res->getString("column_name")};
        string ct{res->getString("column_type")};
        out.push_back(std::make_pair(cn, ct));
    }
    delete res;
    return out;
}

string KSql::DisplayTable(const string& schema, const string& table, int limit)
{
    Cols cols{get_cols(schema, table)};
    vector<string> hdrs;
    string q = "select ";
    for (auto c:cols) {
        string col{c.first};
        hdrs.push_back(col);
        q += col += ", ";
    }
    q.erase(q.length()-2);
    q += " from "+schema+"."+table+" limit "+std::to_string(limit);
    string outstr = q += "\n";
    auto res{ExcuteQuery(q)};

    auto is{cols.size()};
    vector<vector<string>> rows;
    vector<u_long> mls(is);
    while (res->next()) {
        vector<string> fields;
        for (u_int i{0}; i<is; ++i) {
            string c{res->getString(cols[i].first)};
            fields.push_back(c);
            mls[i] = c.length()>mls[i] ? c.length() : mls[i];
            mls[i] = hdrs[i].length()>mls[i] ? hdrs[i].length() : mls[i];
        }
        rows.push_back(fields);
    }
    outstr += "\n";
    for (auto i = 0; i<is; ++i) {
        auto h = hdrs[i];
        h.insert(h.begin(), mls[i]+4-h.length(), ' ');
        outstr += h;
    }
    outstr += "\n";
    for (auto r:rows) {
        for (auto i = 0; i<is; ++i) {
            auto o = r[i];
            o.insert(o.begin(), mls[i]+4-o.length(), ' ');
            outstr += o;
        }
        outstr += "\n";
    }
    outstr += "\n";
    return outstr;
}


//
// Created by kwoodle on 2/15/18.
//

#include <cstring>
#include "Ksql.h"
#include <regex>
//#include <vector>

using namespace drk;

MySqlOptions::MySqlOptions() {
    string filename = string(string(std::getenv("HOME")) + "/.my.cnf");
    std::ifstream cfg(filename);
    optionsDescription.add_options()
            ("client.host", boost::program_options::value<string>()->default_value("localhost"))
            ("client.user", boost::program_options::value<string>())
            ("client.password", boost::program_options::value<string>())
            ("client.database", boost::program_options::value<string>()->default_value(""));

    boost::program_options::variables_map vm;
    // set third parameter to true to allow unregistered options
    // in config file.
    store(parse_config_file(cfg, optionsDescription, true), vm);
    notify(vm);

    host = vm["client.host"].as<string>();
    user = vm["client.user"].as<string>();
    password = vm["client.password"].as<string>();
    db = vm["client.database"].as<string>();
    cfg.close();
}

MySqlOptions::MySqlOptions(const string &filename) {
    std::ifstream cfg(filename);
    optionsDescription.add_options()
            ("client.host", boost::program_options::value<string>())
            ("client.user", boost::program_options::value<string>())
            ("client.password", boost::program_options::value<string>())
            ("client.database", boost::program_options::value<string>()->default_value(""));

    boost::program_options::variables_map vm;
    // set third parameter to true to allow unregistered options
    // in config file.
    store(parse_config_file(cfg, optionsDescription, true), vm);
    notify(vm);

    host = vm["client.host"].as<string>();
    user = vm["client.user"].as<string>();
    password = vm["client.password"].as<string>();
    db = vm["client.database"].as<string>();
    cfg.close();
}


KSql::KSql(MySqlOptions ops) :
        con{get_driver_instance()->connect(ops.get_host(),
                                           ops.get_user(),
                                           ops.get_pass())},
        stmt{con->createStatement()} {}

void KSql::set_schema(const string &s) {
    con->setSchema(s);
}

void KSql::Execute(const string &s) {
    stmt->execute(s);
}

RsltSet *KSql::ExecuteQuery(const string &s) {
    return stmt->executeQuery(s);
}

//      using Cols = vector<std::pair<string column_name, string column_type>>;
/*  const string cols_str {R"%%(select column_name, column_type from information_schema.columns
    where table_schema = '<TABLE_SCHEMA>'
    and table_name = '<TABLE_NAME>')%%"};
*/
Cols KSql::get_cols(const string &table_schema, const string &table_name) {

    // cols_str is a template. Use regex_replace to fill in schema and table
    string s1{cols_str};
    std::regex pat1{tbl_sch};
    std::regex pat2{tbl_nam};

    string s2{std::regex_replace(s1, pat1, table_schema)};
    string sql{std::regex_replace(s2, pat2, table_name)};
    auto res{ExecuteQuery(sql)};
    size_t sz{res->rowsCount()};
    Cols out(sz);
    while (res->next()) {
        int ord{res->getInt("ordinal_position")};
        string cn{res->getString("column_name")};
        string ct{res->getString("column_type")};
        out.at(ord - 1) = std::make_pair(cn, ct);
//        out.push_back(std::make_pair(cn, ct));
    }
    delete res;
    return out;
}

// Do sql select on all columns in schema.table; limit output to limit
//
string KSql::DisplayTable(const string &schema, const string &table, int limit) {
    Cols cols{get_cols(schema, table)};

    // vector of column_names to serve as first line of output (header for table)
    vector<string> hdrs;

    // sql for select statement
    string q = "select ";
    for (auto c:cols) {
        string col{c.first};
        hdrs.push_back(col);
        q += col += ", ";
    }
    // erase last comma and space
    q.erase(q.length() - 2);

    // append limit
    q += " from " + schema + "." + table + " limit " + std::to_string(limit);

    // outstring will be part of output documenting what query was executed
    string outstr = q += "\n";

    // execute the query; result in res
    auto res{ExecuteQuery(q)};
    auto is{cols.size()};

    // rows will be output
    vector<vector<string>> rows;

    // vector of maximum field lengths
    vector<u_long> mls(is);
    while (res->next()) {
        // keep track of individual fields so we can find maximum field length
        // for output formatting
        vector<string> fields;
        for (u_int i{0}; i < is; ++i) {
            // getString(column_name)
            string c{res->getString(cols[i].first)};
            fields.push_back(c);
            mls[i] = c.length() > mls[i] ? c.length() : mls[i];
            mls[i] = hdrs[i].length() > mls[i] ? hdrs[i].length() : mls[i];
        }
        rows.push_back(fields);
    }
    if (rows.size() < 1) {
        outstr += "\nNo rows returned!\n";
        return outstr;
    }
    outstr += "\n";
    for (auto i = 0; i < is; ++i) {
        auto h = hdrs[i];

        // pad fields to align column headers to the right
        h.insert(h.begin(), mls[i] + 4 - h.length(), ' ');
        outstr += h;
    }
    outstr += "\n";
    for (auto r:rows) {
        for (auto i = 0; i < is; ++i) {
            auto o = r[i];

            // pad fields to align column data to the right
            o.insert(o.begin(), mls[i] + 4 - o.length(), ' ');
            outstr += o;
        }
        outstr += "\n";
    }
    outstr += "\n";
    return outstr;
}


string drk::mysql_replace(string cmd, string out, string db) {
    string s1{mysql_tmplt};
    std::regex pat1{"<MYSQL_CMD>"};
    std::regex pat2{"<MYSQL_DB>"};
    std::regex pat3{"<MYSQL_OUT>"};
    string s2{std::regex_replace(s1, pat1, cmd)};
    string s3{std::regex_replace(s2, pat2, db)};
    string s4{std::regex_replace(s3, pat3, out)};
    return s4;
}

const string xml_head{R"%%(<?xml version="1.0" encoding="UTF-8"?>)%%"};
const string drk::cols_str{R"%%(select ordinal_position, column_name, column_type from information_schema.columns
    where table_schema = '<TABLE_SCHEMA>'
    and table_name = '<TABLE_NAME>')%%"};
const string drk::tbl_sch{R"%%(<TABLE_SCHEMA>)%%"};
const string drk::tbl_nam{R"%%(<TABLE_NAME>)%%"};
const string drk::mysql_tmplt{R"%%(/usr/bin/mysql -e '<MYSQL_CMD>' <MYSQL_DB> <MYSQL_OUT>)%%"};

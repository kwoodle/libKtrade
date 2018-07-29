//
// Created by kwoodle on 2/15/18.
//

#ifndef LIBKTRADE_KSQL_H
#define LIBKTRADE_KSQL_H

#include <mysql_connection.h>
//#include<mysql_devapi.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <string>
//#include <mysql.h>
#include <iostream>
#include <vector>

namespace drk {
    using std::string;
    using std::vector;

    using Con = std::shared_ptr<sql::Connection>;
    using PrepStmtPtr = std::shared_ptr<sql::PreparedStatement>;
    using StmtPtr = std::shared_ptr<sql::Statement>;
    using RsltSet = sql::ResultSet;
    using Cols = vector<std::pair<string, string>>;

    class KSql {

    private:
        Con con;
        StmtPtr stmt;
        string host;
        string user;
        string pass;
        string db;
    public:
        explicit KSql(const string& h = "localhost", const string& u = "kwoodle",
                const string& p = "Rancity1!", const string& d = "");

        // execute a statement that returns no results
        void Execute(const string&);

        // execute a query
        RsltSet* ExcuteQuery(const string& s);

        void set_schema(const string& s);
        Cols get_cols(const string& table_schema, const string& table_name);
        void set_autocommit (bool b) {
            con->setAutoCommit(b);
        }
        bool get_autocommit() const {
            con->getAutoCommit();
        }
        void commit() {
            con->commit();
        }

        string DisplayTable(const string& schema, const string& table, int limit = 10);
    };

    extern const string xml_head;
    extern const string cols_str;
    extern const string tbl_sch;
    extern const string tbl_nam;
    extern const string mysql_tmplt;

    extern string mysql_replace(string cmd, string out, string db = "");

}
#endif //LIBKTRADE_KSQL_H

//
// Created by kwoodle on 3/25/18.
//

#ifndef LIBKTRADE_TESTSQL_H
#define LIBKTRADE_TESTSQL_H

#include "sql/Ksql.h"

using namespace drk;

extern void test_cols(KSql& kSql, const string& schema, const string& table);

extern void test_pipes();
#endif //LIBKTRADE_TESTSQL_H

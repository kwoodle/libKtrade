# KSql lib
add_library(Ksql Ksql.cpp)
# For FindMySQL
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")

set(MYSQL_EXTRA_LIBRARIES mysqlcppconn)
#set(MYSQL_EXTRA_LIBRARIES mysqlcppconn2)
find_package(PkgConfig)
pkg_check_modules(MYSQL REQUIRED mysqlclient)

target_include_directories(Ksql PUBLIC ${MYSQL_INCLUDE_DIRS})
target_link_libraries(Ksql ${MYSQL_LIBRARIES} ${MYSQL_EXTRA_LIBRARIES})

target_include_directories(Ksql
        INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

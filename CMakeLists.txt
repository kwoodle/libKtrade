cmake_minimum_required(VERSION 3.9)
project(libKtrade)

set(CMAKE_CXX_STANDARD 17)
# For FindMySQL
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
message(STATUS "CMAKE_MODULE_PATH = " ${CMAKE_MODULE_PATH})
if (NOT CMAKE_DEBUG_POSTFIX)
    set(CMAKE_DEBUG_POSTFIX _d)
endif ()

# for errors from libxml
set(COMPILE_OPTIONS -Wno-deprecated-declarations -fpermissive)
add_compile_options(${COMPILE_OPTIONS})

set(LIB_SOURCE_FILES curl/ktcurl.cpp curl/ktcurl.h curl/HtmlDoc.cpp curl/HtmlDoc.h
      curl/CodeMap.h curl/TestURLs.h sql/Ksql.cpp sql/Ksql.h Util.cpp Util.h Dialogue.cpp Dialogue.h)
add_library(Ktrd SHARED ${LIB_SOURCE_FILES} curl/parse.cpp curl/parse.h Util.cpp Util.h
        workday_script.php)

# curl
find_package(CURL)
if (CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIRS})
    target_include_directories(Ktrd PUBLIC ${CURL_INCLUDE_DIRS})
    target_link_libraries(Ktrd INTERFACE ${CURL_LIBRARIES})
    message(STATUS "curl include dirs  " ${CURL_INCLUDE_DIRS})
    message(STATUS "curl libs  " ${CURL_LIBRARIES})
endif (CURL_FOUND)

# libxml
find_package(LibXml2)
if (LIBXML2_FOUND)
    include_directories(${LIBXML2_INCLUDE_DIR})
    target_include_directories(Ktrd PUBLIC ${LIBXML2_INCLUDE_DIR})
    target_compile_options(Ktrd PUBLIC ${COMPILE_OPTIONS})
    target_link_libraries(Ktrd PUBLIC ${LIBXML2_LIBRARIES})
    message(STATUS "LibXml2_include_dir " ${LIBXML2_INCLUDE_DIR})
endif (LIBXML2_FOUND)

find_package(Boost 1.64 COMPONENTS date_time system filesystem program_options)
#include(/usr/local/lib/ktrade/cmake/ktrade-targets.cmake)
target_link_libraries(Ktrd INTERFACE Boost::date_time Boost::system Boost::filesystem Boost::program_options)

set(MYSQL_EXTRA_LIBRARIES mysqlcppconn)

find_package(MySQL)
include_directories(${MYSQL_INCLUDE_DIR})
target_link_libraries(Ktrd INTERFACE ${MYSQL_LIBRARIES} ${MYSQL_EXTRA_LIBRARIES})

add_executable(Test main.cpp)
target_link_libraries(Test PUBLIC Ktrd ${PYTHON_LIBRARIES})

add_executable(TestSQL TestSQL.cpp TestSQL.h)
target_link_libraries(TestSQL Ktrd  ${Boost_LIBRARIES})

add_executable(TestLog TestLog.cpp)
target_link_libraries(TestLog Ktrd)

add_executable(TestOptions TestOptions.cpp)
target_link_libraries(TestOptions Ktrd)

set(SOURCE_FILES curl/ktcurl.h curl/HtmlDoc.h curl/CodeMap.h curl/TestURLs.h
      sql/Ksql.h Util.h Dialogue.h workday_script.php)
install(TARGETS Ktrd DESTINATION lib/ktrade CONFIGURATIONS Debug Release EXPORT ktrade-targets)
install(FILES ${SOURCE_FILES} DESTINATION include/ktrade)
install(EXPORT ktrade-targets DESTINATION lib/ktrade/cmake)

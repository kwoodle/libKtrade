cmake_minimum_required(VERSION 3.9)
project(libKtrade)

set(CMAKE_CXX_STANDARD 17)
message(STATUS "CMAKE_MODULE_PATH = " ${CMAKE_MODULE_PATH})
# libxml
set(COMPILE_OPTIONS -Wno-deprecated-declarations -fpermissive)
add_compile_options(${COMPILE_OPTIONS})

add_subdirectory(curl)
add_subdirectory(sql)

add_library(Ktrd SHARED  Dialogue.cpp Util.cpp workday_script.php)

find_package(Boost 1.64 COMPONENTS date_time system filesystem program_options)
# include(/usr/local/lib/ktrade/cmake/ktrade-targets.cmake)
target_link_libraries(Ktrd Boost::date_time Boost::system Boost::filesystem Boost::program_options)

add_executable(Test main.cpp)
target_link_libraries(Test Ktrd KCurl Ksql ${PYTHON_LIBRARIES})

add_executable(TestSQL TestSQL.cpp TestSQL.h)
target_link_libraries(TestSQL Ktrd Ksql ${Boost_LIBRARIES})

add_executable(TestLog TestLog.cpp)
target_link_libraries(TestLog Ktrd)

add_executable(TestOptions TestOptions.cpp)
target_link_libraries(TestOptions Ktrd)

set(SOURCE_FILES curl/ktcurl.h curl/HtmlDoc.h curl/CodeMap.h curl/TestURLs.h
      sql/Ksql.h Util.h Dialogue.h workday_script.php)
install(TARGETS Ktrd DESTINATION lib/ktrade CONFIGURATIONS Debug Release EXPORT ktrade-targets)
install(FILES ${SOURCE_FILES} DESTINATION include/ktrade)
install(EXPORT ktrade-targets DESTINATION lib/ktrade/cmake)

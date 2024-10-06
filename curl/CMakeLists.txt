# KCurl lib
add_library(KCurl HtmlDoc.cpp ktcurl.cpp parse.cpp)

# libxml
set(COMPILE_OPTIONS -Wno-deprecated-declarations -fpermissive)

find_package(LibXml2)
if (LIBXML2_FOUND)
    target_include_directories(KCurl PUBLIC ${LIBXML2_INCLUDE_DIR})
    message("LIBXML2_INCLUDE_DIR ${LIBXML2_INCLUDE_DIR}")
##    target_include_directories(Ktrd INTERFACE ${LIBXML2_INCLUDE_DIR})
    target_compile_options(KCurl INTERFACE ${COMPILE_OPTIONS})
    target_link_libraries(KCurl ${LIBXML2_LIBRARIES})
    message(STATUS "LibXml2_include_dir " ${LIBXML2_INCLUDE_DIR})
endif (LIBXML2_FOUND)

find_package(CURL)
if (CURL_FOUND)
 #   include_directories(${CURL_INCLUDE_DIRS})
    target_include_directories(KCurl INTERFACE ${CURL_INCLUDE_DIRS})
    target_link_libraries(KCurl ${CURL_LIBRARIES})
    message(STATUS "curl include dirs  " ${CURL_INCLUDE_DIRS})
    message(STATUS "curl libs  " ${CURL_LIBRARIES})
endif (CURL_FOUND)

target_include_directories(KCurl
        INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)


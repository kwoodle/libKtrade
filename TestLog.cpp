//
// Created by kwoodle on 5/2/20.
//
#include <iostream>
#include "Util.h"


using std::string;
using std::cout;
using std::get;
using std::stringstream;

int main(int argc, char *argv[]) {
    cout<<"\nTesting Logging"<<std::endl;
//    drk::Logger logger(argv, {"Name"});
    drk::Logger logger(argv);
    cout<<"app_name is "<<logger.get_app_name()<<std::endl;
    cout<<"host is "<<logger.show_host()<<std::endl;
    cout<<"Using filename "<<logger.get_filename()<<std::endl;
    logger.log("Message");
    return 0;
}

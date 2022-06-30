//
// Created by kwoodle on 5/13/20.
//

#include <boost/program_options.hpp>
#include <fstream>
#include <iostream>

using std::string;
using std::ifstream;
using std::cout;
namespace po = boost::program_options;

int main() {

    // Get parameters from config file
    string service, user, pass;
    string database, table, source_file;
    ifstream cfg("../test_options.ini");
    if (!cfg.is_open()) {
        cout << "Failed to open config file\n";
        return 1;
    }
    po::options_description desc("Config");
    desc.add_options()
            ("mysql.service", po::value<string>())
            ("mysql.user", po::value<string>())
            ("mysql.password", po::value<string>())
            ("mysql.database", po::value<string>());

    po::variables_map vm;

    // set third parameter to true to allow unregistered options
    // in config file.
    store(parse_config_file(cfg, desc, true), vm);
    notify(vm);

    service = vm["mysql.service"].as<string>();
    user = vm["mysql.user"].as<string>();
    pass = vm["mysql.password"].as<string>();
    database = vm["mysql.database"].as<string>();
    cfg.close();

    cout<<"service = "<<service<<"\t\t user = "<<user<<"\t\t database = "<<database<<std::endl;
    return 0;
}
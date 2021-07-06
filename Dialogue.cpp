//
// Created by kwoodle on 4/28/21.
//

#include "Dialogue.h"

string drk::Dialogue::file_select_str = "/usr/bin/zenity --title='Select a File' --file-selection";
string drk::Dialogue::zenity_str = "/usr/bin/zenity";

string drk::Dialogue::do_command(string s) {
    string ret{get_from_cmd(s)};
    ret.erase(std::remove(ret.begin(), ret.end(), '\n'), ret.end());
    return ret;
}

string drk::Dialogue::FileDialogue() {
    return do_command(file_select_str);
}

string drk::Dialogue::FileDialogue(const string filename) {
    string cmdstr{file_select_str};
    cmdstr += " --filename='" + filename + "'";
    return do_command(cmdstr);
}

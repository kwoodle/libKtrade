//
// Created by kwoodle on 3/28/18.
//

#include "Util.h"

string drk::get_from_cmd(string cmd)
{
    string reslt;
    const int max_buffer{512};
    char buffer[max_buffer];
    cmd.append(" 2>&1");
    FILE* stream{popen(cmd.c_str(), "r")};
    if (stream) {
        while (!feof(stream))
            if (fgets(buffer, max_buffer, stream)!=nullptr) reslt.append(buffer);
        pclose(stream);
    }
    return reslt;
}


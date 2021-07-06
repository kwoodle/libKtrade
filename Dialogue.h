//
// Created by kwoodle on 4/28/21.
//

#ifndef LIBKTRADE_DIALOGUE_H
#define LIBKTRADE_DIALOGUE_H

#include "Util.h"

namespace drk {

    class Dialogue {
        string FileDialogue();

        string FileDialogue(string);

        static string do_command(string);

    public:
        Dialogue() = default;

        string file_dialogue() { return FileDialogue(); }

        string file_dialogue(string s) { return FileDialogue(s); }

        static string file_select_str;
        static string zenity_str;
    };
}

#endif //LIBKTRADE_DIALOGUE_H

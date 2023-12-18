#include <fstream>
#include <iostream>
#include <string>

#include "Parser.h"

Options FillOptions(int argc, char* argv[]) {
    Options opt;
    size_t i = 1;
    while (i < argc) {
        std::string s = argv[i];
        if (s == "-c" || s == "--create") {
            opt.create = true;
        } else if (s == "-f") {
            opt.archive = argv[i + 1];
            ++i;
        } else if (s.substr(0, 6) == "--file=") {
            opt.archive = s.substr(7);
        } else if (s == "-l" || s == "--list") {
            opt.list_f = true;
        } else if (s == "-x" || s == "--extract") {
            opt.extract = true;
        } else if (s == "-a" || s == "--append") {
            opt.append = true;
        } else if (s == "-d" || s == "--delete") {
            opt.delete_f = true;
        } else if (s == "-A" || s == "--concatenate") {
            opt.concatenate = true;
        } else if (s == "-L" | s == "--length") {
            opt.len = std::stoi(argv[i + 1]);
            ++i;
        } else {
            opt.files.push_back(s);
        }
        ++i;
    }

    return opt;
}

#pragma once

#include <string>
#include <vector>

const int kDefaultLen = 8;

struct Options {
    bool create = false;
    bool list_f = false;
    bool extract = false;
    bool append = false;
    bool delete_f = false;
    bool concatenate = false;
    uint16_t len = kDefaultLen;
    std::vector<std::string> files;
    std::string archive;
};

Options FillOptions(int argc, char* argv[]);

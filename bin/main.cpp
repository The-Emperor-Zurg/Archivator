#include <iostream>
#include <fstream>

#include "../lib/Parser.h"
#include "../lib/Functions.h"
#include "../lib/HammingCode.h"

int main(int argc, char* argv[]) {
    Options opt = FillOptions(argc, argv);
    if (opt.create) {
        std::ofstream arch(opt.archive, std::ios::out | std::ios::binary);
        for (auto s : opt.files) {
            ConvertingToHamarc_f(arch, s, opt.len);
        }
    } else if (opt.list_f) {
        std::vector<std::string> files;
        files = List(opt.archive);
        for (auto s : files) {
            std::cout << s << std::endl;
        }
    } else if (opt.append) {
        std::ofstream arch(opt.archive, std::ios::out | std::ios::binary | std::ios::app);
        for (auto file : opt.files) {
            ConvertingToHamarc_f(arch, file, opt.len);
        }
    } else if (opt.concatenate) {
        Merge(opt.archive, opt.files[0], opt.files[1]);
    } else if (opt.delete_f) {
        Delete(opt.archive, opt.files);
    } else if (opt.extract) {
        Extract(opt.archive, opt.files);
    }

    return 0;
}

#include <string>
#include <fstream>
#include <iostream>
#include "Parser.h"
#include "HammingCode.h"

void Merge(std::string arch, std::string File1, std::string File2) {
    std::ifstream file1;
    file1.open(File1, std::ios::in | std::ios::binary);

    std::ifstream file2;
    file2.open(File2, std::ios::in | std::ios::binary);

    std::ofstream file;
    file.open(arch, std::ios::out | std::ios::binary);

    char c;
    while (file1.get(c)) {
        file << c;
    }
    file1.close();

    while (file2.get(c)) {
        file << c;
    }
    file2.close();
    file.close();
}

void Delete(std::string Arch, std::vector<std::string> files) {

    std::string arch = "*" + Arch;

    std::ifstream fin;
    fin.open(Arch, std::ios::in | std::ios::binary);
    std::ofstream fout;
    fout.open(arch, std::ios::out | std::ios::binary);

    std::string name;
    std::string size;
    std::string len;
    uint64_t Size;
    bool flag;
    std::vector<bool> v;
    std::string str = " ";
    v = BitesToHam(str, 1);
    while (fin.tellg() < std::filesystem::file_size(Arch)) {
        name = TakeWord(fin);
        if (name == "") {
            break;
        }
        for (auto s : files) {
            if (name == s) {
                flag = true;
            }
        }
        if (flag) {
            size = TakeWord(fin);
            len = TakeWord(fin);
            Size = std::stoi(size);
            Size += fin.tellg();
            Size += 2;
            fin.seekg(Size);
        } else {
            Size = fin.tellg();
            Size = Size - 2 * name.length() - 2;
            fin.seekg(Size);
            name = TakeWordNotDecode(fin);
            for (size_t i = 0; i < name.length(); ++i) {
                fout << name[i];
            }

            fout << HamToSymbols(v);

            size = TakeWordNotDecode(fin);
            for (size_t i = 0; i < size.length(); ++i) {
                fout << size[i];
            }
            Size = fin.tellg();
            Size = Size - 2 - size.length();
            fin.seekg(Size);
            size = TakeWord(fin);
            Size = std::stoi(size);

            fout << HamToSymbols(v);

            len = TakeWordNotDecode(fin);
            for (size_t i = 0; i < len.length(); ++i) {
                fout << len[i];
            }

            fout << HamToSymbols(v);

            char c;
            for (size_t i = 0; i < Size; ++i) {
                fin.get(c);
                fout << c;
            }

            fout << HamToSymbols(v);
        }
        flag = false;
    }
    std::filesystem::remove(std::filesystem::path(Arch));
    fout.close();
}

std::vector<std::string> List(std::string Arch) {
    std::ifstream arch;
    arch.open(Arch, std::ios::in | std::ios::binary);
    std::vector<std::string> files;
    std::string name;
    std::string size;
    std::string len;
    int Size_f;
    while(arch.tellg() < std::filesystem::file_size(Arch)) {
        name = TakeWord(arch);
        files.push_back(name);
        size = TakeWord(arch);
        Size_f = std::strtol(size.c_str(), nullptr, 10);
        len = TakeWord(arch);
        Size_f += arch.tellg();
        Size_f += 2;
        arch.seekg(Size_f);
    }
    arch.close();
    return files;
}

void Extract(std::string Arch, std::vector<std::string> files) {
    std::ifstream arch;
    arch.open(Arch, std::ios::in | std::ios::binary);

    if (files.empty()) {
        files = List(Arch);
    }

    std::string name;
    std::string size;
    std::string len;
    while (arch.tellg() < std::filesystem::file_size(Arch)) {
        bool flag;
        name = TakeWord(arch);
        for (auto s : files) {
            if (s == name) {
                flag = true;
                break;
            }
        }
        size = TakeWord(arch);
        len = TakeWord(arch);
        uint64_t Size = std::stoi(size);
        if (flag) {
            std::string new_name = "Unpacked";
            new_name += name;
            std::ofstream f_out;
            f_out.open(new_name, std::ios::out | std::ios::binary);
            uint16_t k_len = std::stoi(len);
            TakeHammingCode(f_out, arch, k_len, Size);
            Size = arch.tellg();
            Size += 2;
            arch.seekg(Size);
        } else {
            Size += arch.tellg();
            Size += 2;
            arch.seekg(Size);
        }
    }
    arch.close();
}

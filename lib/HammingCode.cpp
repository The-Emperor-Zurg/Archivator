#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <bitset>

const uint8_t kBitesPerByte = 8;

int BinToInt(std::vector<bool> s) {
    int ans = 0;
    int pow = 1;
    for (short int i = s.size() - 1; i >= 0; --i) {
        ans = ans + pow * s[i];
        pow *= 2;
    }

    return ans;
}

std::vector<bool> BitesToHam(std::string& s, uint16_t k) {
    std::vector<bool> ans(k * kBitesPerByte + ceil(log2(k * kBitesPerByte)) + 1);

    std::vector<bool> bn(s.length() * kBitesPerByte);
    char c;
    size_t counter = 0;
    for (size_t i = 0; i < s.length(); ++i) {
        c = s[i];
        std::bitset<kBitesPerByte> tmp(c);
        std::string Tmp;
        Tmp = tmp.to_string();
        for (size_t j = 0; j < kBitesPerByte; ++j) {
            if (Tmp[j] == '1') {
                bn[counter] = true;
            } else {
                bn[counter] = false;
            }
            ++counter;
        }
    }

    counter = 0;
    uint16_t pow2 = 1;
    for (size_t i = 0; i < ans.size(); ++i) {
        if (i == pow2 - 1) {
            ans[i] = false;
            pow2 *= 2;
        } else if (counter <= bn.size() - 1) {
            if (bn[counter]) {
                ans[i] = true;
            } else {
                ans[i] = false;
            }
            ++counter;
        } else {
            ans[i] = false;
        }
    }

    for (size_t i = 1; i <= ans.size(); i *= 2) {
        counter = 0;
        for (size_t j = i - 1; j < ans.size(); j = j + 2 * i) {
            for (size_t cc = 0; cc < i; ++cc) {
                if (j + cc <= ans.size() - 1 && ans[j + cc]) {
                    ++counter;
                }
            }
        }
        if (counter % 2 == 1) {
            ans[i - 1] = true;
        }
    }

    uint8_t ost = (8 - ans.size() % 8) % 8;
    for (size_t i = 1; i <= ost; ++i) {
        ans.push_back(false);
    }
    return ans;
}

std::string HamToSymbols(std::vector<bool> v) {
    std::string s;
    int number;
    for (size_t i = 0; i < ceil( (double) v.size()  / kBitesPerByte); ++i) {
        std::vector<bool> str;
        for (size_t j = kBitesPerByte * i; j < kBitesPerByte * i + kBitesPerByte; ++j) {
            str.push_back(v[j]);
        }
        number = BinToInt(str);
        unsigned char tmp = number;
        s += tmp;
    }
    return s;
}

void ConvertingToHamarc_f(std::ofstream& arch, std::string File, uint16_t len) {
    std::ifstream file;
    file.open(File, std::ios::in | std::ios::binary);

    std::vector <bool> v;
    for (size_t i = 0; i < File.length(); ++i) {
        std::string S;
        S += File[i];
        std::string t;
        t = S[0];
        v = BitesToHam(t, 1);
        arch << HamToSymbols(v);
    }

    std::string str = " ";
    v = BitesToHam(str, 1);
    arch << HamToSymbols(v);

    double SymbolsFile = std::filesystem::file_size(std::filesystem::path(File));
    uint64_t SizeFile = (ceil (SymbolsFile / len)) * (len * kBitesPerByte + (ceil(log2(len * kBitesPerByte))) + (kBitesPerByte - uint64_t(ceil(log2(len * kBitesPerByte))) % kBitesPerByte ) % kBitesPerByte) / kBitesPerByte;
    std::string Str = std::to_string(SizeFile);
    for (size_t i = 0; i < Str.length(); ++i) {
        std::string S;
        S += Str[i];
        std::string t;
        t = S[0];
        v = BitesToHam(t, 1);
        arch << HamToSymbols(v);
    }

    v = BitesToHam(str, 1);
    arch << HamToSymbols(v);

    std::string ln = std::to_string(len);
    for (size_t i = 0; i < ln.length(); ++i) {
        std::string S;
        S += ln[i];
        std::string t;
        t = S[0];
        v = BitesToHam(t, 1);
        arch << HamToSymbols(v);
    }

    v = BitesToHam(str, 1);
    arch << HamToSymbols(v);

    char c;
    uint16_t counter = 0;
    std::string cur;
    while (file.get(c)) {
        cur += c;
        ++counter;
        if (counter == len) {
            counter = 0;
            std::vector <bool> v;
            v = BitesToHam(cur, len);
            std::string s;
            s = HamToSymbols(v);
            arch << s;
            cur = "";
        }
    }
    if (cur.length() != 0) {
        if (cur.length() == 1) {
            c = cur[0];
            std::bitset<kBitesPerByte> v(c);
        }
        std::vector <bool> v;
        v = BitesToHam(cur, len);
        std::string s;
        s = HamToSymbols(v);
        arch << s;
    }

    v = BitesToHam(str, 1);
    arch << HamToSymbols(v);

    file.close();
}

void CheckHammingCode(std::vector<bool>& bit_vector) {
    size_t false_index = 0;
    size_t degree = 0;
    bool mistake = false;

    while ((1 << degree) < bit_vector.size()) {
        size_t counter = 0;
        for (size_t it1 = (1 << degree); it1 < bit_vector.size() + 1; it1 += 2 * (1 << degree)) {
            for (size_t it2 = 0; it2 < (1 << degree); it2++) {
                if (it1 + it2 - 1 < bit_vector.size()) {
                    counter += bit_vector[it1 + it2 - 1];
                }
            }
        }
        counter -= bit_vector[(1 << degree) - 1];
        if (counter % 2 == 0) {
            if (bit_vector[(1 << degree) - 1] != 0) {
                false_index += (1 << degree);
                mistake = true;
            }
        } else {
            if (bit_vector[(1 << degree) - 1] != 1) {
                false_index += (1 << degree);
                mistake = true;
            }
        }
        degree++;
    }

    if (mistake) {
        std::cerr<<("The file is broken!\n");
        false_index--;
        bit_vector[false_index] = !bit_vector[false_index];
    }
}

char DecodeToChar(std::string s) {
    size_t length_byte = s.size();
    size_t current_length = 0;
    std::string str;

    for (size_t i = 0; i < length_byte; i++) {
        std::bitset<kBitesPerByte> tmp(s[i]);
        str += tmp.to_string();
        current_length++;
        if (current_length == length_byte) {
            current_length = 0;
            std::vector<bool> bit_vector;
            for (char it: str) {
                if (it == '1') {
                    bit_vector.push_back(true);
                } else  {
                    bit_vector.push_back(false);
                }
            }
            str = "";
            CheckHammingCode(bit_vector);
            std::vector<bool> ans(kBitesPerByte);
            int pow = 1;
            size_t it1 = 0;
            size_t j = 0;
            while (it1 < kBitesPerByte) {
                if (pow - 1 == j) {
                    pow *= 2;
                } else {
                    ans[it1] = bit_vector[j];
                    ++it1;
                }
                ++j;
            }

            for (size_t it = 0; it < kBitesPerByte; it++) {
                if (ans[it]) {
                    str += '1';
                } else {
                    str += '0';
                }
            }
            char result = std::strtol(str.c_str(), nullptr, 2);
            return result;
        }
    }
}

void TakeHammingCode(std::ofstream& output_file, std::ifstream& input_file, size_t length, size_t length_file) {
    size_t length_bit;
    char byte[1];
    length_bit = length * kBitesPerByte + ceil(log2(length * kBitesPerByte)) + 1;
    if (length_bit % kBitesPerByte != 0) {
        length_bit = length_bit + (kBitesPerByte - length_bit % kBitesPerByte);
    }
    size_t length_byte = length_bit / kBitesPerByte;
    size_t cur_len = 0;
    std::string str;

    for (size_t i = 0; i < length_file; ++i) {
        input_file.read(byte, 1);
        std::bitset<kBitesPerByte> tmp(byte[0]);
        str += tmp.to_string();
        ++cur_len;
        if (cur_len == length_byte) {
            cur_len = 0;
            std::vector<bool> bit_vector;
            for (size_t it = 0; it < str.size(); it++) {
                if (str[it] == '1') {
                    bit_vector.push_back(true);
                } else {
                    bit_vector.push_back(false);
                }
            }
            str = "";
            CheckHammingCode(bit_vector);
            size_t pow = 1;
            for (size_t j = 0; j < bit_vector.size() - 1; ++j) {
                if (pow - 1 == j) {
                    pow *= 2;
                } else {
                    if (bit_vector[j]) {
                        str += "1";
                    } else {
                        str += "0";
                    }
                }
            }
            if (bit_vector[bit_vector.size() - 1]) {
                str += "1";
            } else {
                str += "0";
            }
            std::string res;
            for (size_t c = kBitesPerByte; c <= str.length(); c += kBitesPerByte) {
                std::string cur = str.substr(c - kBitesPerByte, kBitesPerByte);
                std::vector<bool> chr;
                for (size_t j = 0; j < kBitesPerByte; ++j) {
                    if (cur[j] == '1') {
                        chr.push_back(true);
                    } else {
                        chr.push_back(false);
                    }
                }
                res += char(BinToInt(chr));
            }

            for (size_t c = 0; c < res.length(); ++c) {
                output_file << res[c];
            }
            str = "";
        }

    }
}

std::string TakeWord(std::ifstream& archive_file) {
    char byte[1];
    std::string tmp;
    archive_file.read(byte, 1);
    tmp += byte[0];
    archive_file.read(byte, 1);
    tmp += byte[0];
    std::string res;
    while (DecodeToChar(tmp) != ' ') {
        res += DecodeToChar(tmp);
        tmp = "";
        archive_file.read(byte, 1);
        tmp += byte[0];
        archive_file.read(byte, 1);
        tmp += byte[0];
    }
    return res;
}

std::string TakeWordNotDecode(std::ifstream& archieve) {
    char byte[1];
    std::string tmp;
    archieve.read(byte, 1);
    tmp += byte[0];
    archieve.read(byte, 1);
    tmp += byte[0];
    std::string res;
    while (DecodeToChar(tmp) != ' ') {
        res += tmp;
        tmp = "";
        archieve.read(byte, 1);
        tmp += byte[0];
        archieve.read(byte, 1);
        tmp += byte[0];
    }
    return res;
}

#pragma once

int BinToInt(std::vector<bool> s);

std::vector<bool> BitesToHam(std::string& s, uint16_t k);

std::string HamToSymbols(std::vector<bool> v);

void ConvertingToHamarc_f(std::ofstream& arch, std::string File, uint16_t len);

void CheckHammingCode(std::vector<bool>& bit_vector);

char DecodeToChar(std::string s);

void TakeHammingCode(std::ofstream& output_file, std::ifstream& input_file, size_t length, size_t length_file);

std::string TakeWord(std::ifstream& archive_file);

std::string TakeWordNotDecode(std::ifstream& archieve);

#pragma once

void Merge(std::string arch, std::string File1, std::string File2);

void Delete(std::string arch, std::vector<std::string> files);

std::vector<std::string> List(std::string Arch);

void Extract(std::string Arch, std::vector<std::string> files);

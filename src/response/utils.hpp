#pragma once

#include <string>
#include <vector>
#include <sys/stat.h>

int findPosition(const std::vector<int>& vec);
int longestCommonPrefix(const std::string& str, const std::vector<std::string>& vec);
int findIndexOfMax(const std::vector<int>& vec);
bool is_file_exists(const std::string &path);
bool is_regular_file(const std::string &path);
bool is_dir(const std::string &path);
std::string getDirContentHTML(const std::string &dir_path); 
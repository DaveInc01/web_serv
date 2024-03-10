#include "IResponseParser.hpp"
#include <sys/stat.h>

int findPosition(const std::vector<int>& vec) {
    int minElement = __INT_MAX__;  // Initialize to a large value
    int position = -1;

    for (int i = 0; i < vec.size(); ++i) {
        if (vec[i] > -1 && vec[i] < minElement) {
            minElement = vec[i];
            position = i;
        }
    }
    return position;
}

bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

int longestCommonPrefix(const std::string& str, const std::vector<std::string>& vec) {
    if (vec.empty()) {
        return 0;  // No common prefix if the vector is empty
    }

    int maxCommonPrefix = 0;

    for (const std::string& s : vec) {
        int commonPrefix = 0;
        while (commonPrefix < std::min(s.size(), str.size()) && s[commonPrefix] == str[commonPrefix]) {
            ++commonPrefix;
        }
        maxCommonPrefix = std::max(maxCommonPrefix, commonPrefix);
    }

    return maxCommonPrefix;
}


int findIndexOfMax(const std::vector<int>& vec) {
    int maxElement = vec[0];  // Assume the first element is the maximum
    int maxIndex = 0;
    for (int i = 1; i < vec.size(); ++i) {
        if (vec[i] > maxElement) {
            maxElement = vec[i];
            maxIndex = i;
        }
    }

    return maxIndex;
}

bool is_file_exists(const std::string &path)
{
	struct stat file_stat;

	return (stat(path.c_str(), &file_stat) == 0);
}

bool is_regular_file(const std::string &path)
{
    struct stat file_stat;
	stat(path.c_str(), &file_stat);

	return S_ISREG(file_stat.st_mode);
}

bool is_dir(const std::string &path)
{
    struct stat file_stat;
	stat(path.c_str(), &file_stat);

	return S_ISDIR(file_stat.st_mode);
}
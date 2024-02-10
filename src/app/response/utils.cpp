#include "IResponseParser.hpp"

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



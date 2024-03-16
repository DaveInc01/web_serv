#include "IResponseParser.hpp"
#include <sys/stat.h>
#include <iostream>
#include <dirent.h>
#include <vector>

bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}
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

std::string getDirContentHTML(const std::string &dir_path) 
{
	DIR* dirp;
	struct dirent* direntp;
	std::vector<std::string> vec;
	std::string dir_html;
	struct stat info;

	dirp = opendir(dir_path.c_str());
	if(dirp != NULL)
	{
		for(;;)
		{
			direntp = readdir(dirp);
			if(direntp == NULL)
				break;
			vec.push_back(direntp->d_name);
		}
		closedir(dirp);
		//return EXIT_SUCCESS;
	}
	dir_html += "<html>\n<head><title>List of directory content</title></head>\n<body>\n<h1>Index Of </h1><hr>\n<table style=\"width: 100%\">";
	for (int i = 0; i< vec.size(); i++)
	{
		struct stat tmp_info;
		std::string file_path(dir_path + vec[i]);
		stat(file_path.c_str(), &tmp_info);
		dir_html += "\n<tr>\n\t<td><a href=" + dir_path + vec[i] + ">" + vec[i] + "</a><br>";
		char  *time = ctime(&info.st_mtime);
		dir_html += "</td>\n\t<td>" + static_cast<std::string>(time) + "</td>\n\t<td>";
		std::string tmp = std::to_string(tmp_info.st_size);
		dir_html += tmp + "</td></tr>";
	}
	dir_html += "</table>\n<hr>\n</body>\n</html>";
	return (dir_html);
	// return EXIT_FAILURE;
}

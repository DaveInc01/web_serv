#pragma once

#include <string>
#include <map>

class MimeTypes
{
	public:
		MimeTypes();
		static std::string getContentType(const std::string &path);
		static std::string getContentTypeLine(const std::string &path);

	private:
		static std::map<std::string, std::string> mime_types;
};

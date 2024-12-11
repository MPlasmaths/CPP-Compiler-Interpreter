#pragma once

#include <vector>
#include <string>

class FileHandler {
public:

	static std::vector<std::string> read_lines(const std::string& p_file_name);
	static void write_line(const std::string& p_file_name, const std::string& p_line);
	static void write_lines(const std::string& p_file_name, const std::vector<std::string>& p_lines);
};
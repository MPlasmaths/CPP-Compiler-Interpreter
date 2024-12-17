#include "file_handler.hpp"

#include <iostream>
#include <fstream>
#include <stdexcept>

std::vector<std::string> FileHandler::read_lines(const std::string& p_file_name) {
	std::ifstream input_file(p_file_name);
	if (!input_file.is_open()) {
		throw std::runtime_error("unable to open file \"" + p_file_name + "\"");
	}
	std::string line;
	std::vector<std::string> lines;
	while (std::getline(input_file, line)) {
		lines.push_back(line);
	}
	input_file.close();
	return lines;
}

void FileHandler::write_line(const std::string& p_file_name, const std::string& p_line) {
	if (p_file_name.empty()) {
		throw std::invalid_argument("file name cannot be empty");
	} else if (p_file_name.find_first_of("\\:?\"<>|") != std::string::npos) {
		throw std::invalid_argument("file name contains invalid characters");
	}
	std::ofstream output_file(p_file_name);
	if (!output_file.is_open()) {
		throw std::runtime_error("unable to open file \"" + p_file_name + "\"");
	}
	output_file << p_line << "\n";
	output_file.close();
}

void FileHandler::write_lines(const std::string& p_file_name, const std::vector<std::string>& p_lines) {
	if (p_file_name.empty()) {
		throw std::invalid_argument("file name cannot be empty");
	} else if (p_file_name.find_first_of("\\:?\"<>|") != std::string::npos) {
		throw std::invalid_argument("file name contains invalid characters");
	}
	std::ofstream output_file(p_file_name);
	if (!output_file.is_open()) {
		throw std::runtime_error("unable to open file \"" + p_file_name + "\"");
	}
	for (int i = 0; i < p_lines.size(); i++) {
		output_file << p_lines[i] << "\n";
	}
	output_file.close();
}
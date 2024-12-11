#pragma once

#include "command_line.hpp"
#include "variable_object.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

class Serializer {
public:

	static void write_binary_file(const std::string& p_binary_file_name,
        const std::vector<VariableObject> p_symbol_table, const std::vector<CommandLine> p_portable_code) {
		std::ofstream binary_file(p_binary_file_name, std::ios::binary);
		if (!binary_file) {
			throw std::runtime_error("failed to open binary file for writing");
		}
        const size_t symbol_table_size = p_symbol_table.size();
        binary_file.write(reinterpret_cast<const char*>(&symbol_table_size), sizeof(symbol_table_size));
        for (const VariableObject& variable_object : p_symbol_table) {
            variable_object.serialize(binary_file);
        }
        const size_t portable_code_size = p_portable_code.size();
        binary_file.write(reinterpret_cast<const char*>(&portable_code_size), sizeof(portable_code_size));
        for (const CommandLine& command_line : p_portable_code) {
            command_line.serialize(binary_file);
        }

        binary_file.close();
	}
private:



};
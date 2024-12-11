#pragma once

#include "variable_object.hpp"
#include "command_line.hpp"

#include <fstream>

class Deserializer {
public:

	static void read_binary_file(const std::string& p_binary_file_name,
		std::vector<VariableObject>& p_symbol_table, std::vector<CommandLine>& p_portable_code) {
		
		std::ifstream binary_file(p_binary_file_name, std::ios::binary);
		if (!binary_file) {
			throw std::runtime_error("failed to open binary file for reading");
		}
		/*size_t symbol_table_size = 0;
		binary_file.read(reinterpret_cast<char*>(&symbol_table_size), sizeof(symbol_table_size));
		p_symbol_table.resize(symbol_table_size);
		for (size_t i = 0; i < symbol_table_size; i++) {
			binary_file.read(reinterpret_cast<char*>(&p_symbol_table[i]), sizeof(VariableObject));
		}
		size_t portable_code_size = 0;
		binary_file.read(reinterpret_cast<char*>(&portable_code_size), sizeof(portable_code_size));
		p_portable_code.resize(portable_code_size);
		for (size_t i = 0; i < portable_code_size; i++) {
			binary_file.read(reinterpret_cast<char*>(&p_portable_code[i]), sizeof(CommandLine));
		}
		if (!binary_file) {
			throw std::runtime_error("expected end of binary file");
		}
		binary_file.close();*/

		size_t symbol_table_size = 0;
		binary_file.read(reinterpret_cast<char*>(&symbol_table_size), sizeof(symbol_table_size));
		p_symbol_table.clear();
		for (size_t i = 0; i < symbol_table_size; i++) {
			size_t identifier;
			binary_file.read(reinterpret_cast<char*>(&identifier), sizeof(identifier));

			size_t name_length = 0;
			binary_file.read(reinterpret_cast<char*>(&name_length), sizeof(name_length));
			std::string name(name_length, '\0');
			binary_file.read(&name[0], name_length);
			
			size_t value_index = 0;
			binary_file.read(reinterpret_cast<char*>(&value_index), sizeof(value_index));
			VariableObjectValueVariant value_variant;
			if (value_index == 0) { // int
				int value;
				binary_file.read(reinterpret_cast<char*>(&value), sizeof(value));
				value_variant = value;
			} else if (value_index == 1) { // float
				float value;
				binary_file.read(reinterpret_cast<char*>(&value), sizeof(value));
				value_variant = value;
			}
			p_symbol_table.emplace_back(identifier, name, value_variant);
		}


		size_t portable_code_size = 0;
		binary_file.read(reinterpret_cast<char*>(&portable_code_size), sizeof(portable_code_size));
		p_portable_code.clear();
		for (size_t i = 0; i < portable_code_size; i++) {
			std::underlying_type_t<CommandType> type_value;
			binary_file.read(reinterpret_cast<char*>(&type_value), sizeof(type_value));
			CommandType type = static_cast<CommandType>(type_value);

			size_t result_identifier;
			binary_file.read(reinterpret_cast<char*>(&result_identifier), sizeof(result_identifier));

			size_t source_size = 0;
			binary_file.read(reinterpret_cast<char*>(&source_size), sizeof(source_size));
			std::vector<OperandNodeValueVariant> source_value_variants;
			for (size_t j = 0; j < source_size; j++) {
				size_t source_index = 0;
				binary_file.read(reinterpret_cast<char*>(&source_index), sizeof(source_index));
				OperandNodeValueVariant source;
				if (source_index == 0) { // size_t
					size_t value;
					binary_file.read(reinterpret_cast<char*>(&value), sizeof(value));
					source = value;
				} else if (source_index == 1) { // int
					int value;
					binary_file.read(reinterpret_cast<char*>(&value), sizeof(value));
					source = value;
				} else if (source_index == 2) { // float
					float value;
					binary_file.read(reinterpret_cast<char*>(&value), sizeof(value));
					source = value;
				}
				source_value_variants.push_back(source);
			}

			p_portable_code.emplace_back(type, result_identifier, source_value_variants);
		}

		binary_file.close();
	}

private:

	

};
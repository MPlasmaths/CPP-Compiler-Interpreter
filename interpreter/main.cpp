#include "deserializer.hpp"
#include "command_handler.hpp"

#include <iostream>
#include <stdexcept>

// interpreter.exe output_post_code.bin
int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "English");

	try {
		if (argc != 2) {
			throw std::runtime_error("invalid number of arguments for post code interpretation");
		}
		const std::string post_code_file_name = argv[1];

		std::vector<VariableObject> symbol_table;
		std::vector<CommandLine> portable_code;

		Deserializer::read_binary_file(post_code_file_name, symbol_table, portable_code);

		std::cout << "Enter variables:\n";
		for (size_t i = 0; i < symbol_table.size(); i++) {
			if (symbol_table[i].get_name()[0] != '#') {
				std::cout << symbol_table[i].to_string() << ":\n";
				float value;
				std::cin >> value;
				if (std::holds_alternative<int>(symbol_table[i].get_value_variant())) {
					if (value != static_cast<int>(value)) {
						throw std::runtime_error("integer value expected for variable");
					}
					symbol_table[i].set_value_variant(static_cast<int>(value));
				} else {
					symbol_table[i].set_value_variant(value);
				}
			}
		}

		std::cout << "Portable code:\n";
		for (size_t i = 0; i < portable_code.size(); i++) {
			std::cout << i + 1 << ". " << portable_code[i].to_string() << '\n';
			const size_t result_identifier = portable_code[i].get_result_identifier();
			std::cout << "|: " << symbol_table[result_identifier].get_name() << " = ";
			const CommandType command_type = portable_code[i].get_type();

			float value = CommandHandler::process_command_source(command_type,
				portable_code[i].get_source_value_variants(), symbol_table);

			if (command_type != CommandType::I2F) {
				std::cout << " = ";
				if (std::holds_alternative<int>(symbol_table[result_identifier].get_value_variant())) {
					symbol_table[result_identifier].set_value_variant(static_cast<int>(value));
					std::cout << static_cast<int>(value);
				} else {
					symbol_table[result_identifier].set_value_variant(value);
					std::cout << StringHandler::format_float(value);
				}
			} else {
				symbol_table[result_identifier].set_value_variant(value);
			}
			
			std::cout << ";\n";
		}

	} catch (const std::exception& e) {
		std::cerr << "Exception: " << e.what() << ".\n";
		return 1;
	}

	return 0;
}


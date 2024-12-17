#include "mode_type.hpp"
#include "mode_handler.hpp"

#include <iostream>
#include <string>
#include <format>
#include <stdexcept>

int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "English");

	try {
		if (argc < 3) {
			throw std::runtime_error("not enough arguments");
		}

		const ModeType mode_type = to_mode_type(argv[1]);
		ModeHandler::_optimization_enabled = to_mode_type(argv[2]) == ModeType::OPTIMIZATION;

		if (!ModeHandler::check_arguments_quantity(argc, mode_type)) {
			throw std::runtime_error(std::format("invalid number of arguments for {} mode", describe(mode_type)));
		}

		if (mode_type == ModeType::LEXICAL_ANALYSIS) {
			// compiler.exe lex files/input_expression.txt files/output_tokens.txt files/output_symbols.txt
			ModeHandler::lexical_analysis(argv[2], argv[3], argv[4]);
		} else if (mode_type == ModeType::SYNTACTIC_ANALYSIS) {
			// compiler.exe syn files/input_expression.txt files/output_syntax_tree.txt
			ModeHandler::syntactic_analysis(argv[2], argv[3]);
		} else if (mode_type == ModeType::SEMANTIC_ANALYSIS) {
			// compiler.exe sem files/input_expression.txt files/output_modified_syntax_tree.txt
			ModeHandler::semantic_analysis(argv[2], argv[3]);
		} else if (mode_type == ModeType::PORTABLE_CODE_GENERATION) {
			if (!ModeHandler::_optimization_enabled) {
				// compiler.exe gen1 files/input_expression.txt files/output_portable_code.txt files/output_symbols.txt
				ModeHandler::portable_code_generation(argv[2], argv[3], argv[4]);
			} else {
				// compiler.exe gen1 opt files/input_expression.txt files/output_optimized_portable_code.txt files/output_optimized_symbols.txt
				ModeHandler::portable_code_generation(argv[3], argv[4], argv[5]);
			}
		} else if (mode_type == ModeType::POSTFIX_NOTATION_GENERATION) {
			if (!ModeHandler::_optimization_enabled) {
				// compiler.exe gen2 files/input_expression.txt files/output_postfix_notation.txt files/output_symbols.txt
				ModeHandler::postfix_notation_generation(argv[2], argv[3], argv[4]);
			} else {
				// compiler.exe gen2 opt files/input_expression.txt files/output_optimized_postfix_notation.txt files/output_optimized_symbols.txt
				ModeHandler::postfix_notation_generation(argv[3], argv[4], argv[5]);
			}
		} else if (mode_type == ModeType::POST_CODE_GENERATION) {
			// compiler.exe gen3 files/input_expression.txt files/output_post_code.bin
			ModeHandler::post_code_generation(argv[2], argv[3]);
		} else {
			throw std::runtime_error("unknown mode");
		}

	} catch (const std::exception& e) {
		std::cerr << "Exception: " << e.what() << ".\n";
		return 1;
	}

	return 0;

}

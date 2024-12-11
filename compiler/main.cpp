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
			// compiler.exe lex input_expression.txt output_tokens.txt output_symbols.txt
			ModeHandler::lexical_analysis(argv[2], argv[3], argv[4]);
		} else if (mode_type == ModeType::SYNTACTIC_ANALYSIS) {
			// compiler.exe syn input_expression.txt output_syntax_tree.txt
			ModeHandler::syntactic_analysis(argv[2], argv[3]);
		} else if (mode_type == ModeType::SEMANTIC_ANALYSIS) {
			// compiler.exe sem input_expression.txt output_modified_syntax_tree.txt
			ModeHandler::semantic_analysis(argv[2], argv[3]);
		} else if (mode_type == ModeType::PORTABLE_CODE_GENERATION) {
			if (!ModeHandler::_optimization_enabled) {
				// compiler.exe gen1 input_expression.txt output_portable_code.txt output_symbols.txt
				ModeHandler::portable_code_generation(argv[2], argv[3], argv[4]);
			} else {
				// compiler.exe gen1 opt input_expression.txt output_portable_code.txt output_symbols.txt
				ModeHandler::portable_code_generation(argv[3], argv[4], argv[5]);
			}
		} else if (mode_type == ModeType::POSTFIX_NOTATION_GENERATION) {
			if (!ModeHandler::_optimization_enabled) {
				// compiler.exe gen2 input_expression.txt output_postfix_notation.txt output_symbols.txt
				ModeHandler::postfix_notation_generation(argv[2], argv[3], argv[4]);
			} else {
				// compiler.exe gen2 opt input_expression.txt output_postfix_notation.txt output_symbols.txt
				ModeHandler::postfix_notation_generation(argv[3], argv[4], argv[5]);
			}
		} else if (mode_type == ModeType::POST_CODE_GENERATION) {
			// compiler.exe gen3 input_expression.txt output_post_code.bin
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

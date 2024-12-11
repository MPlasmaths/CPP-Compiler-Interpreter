#pragma once

#include "mode_type.hpp"

#include <string>

class ModeHandler {
public:
	static bool _optimization_enabled;

	static bool check_arguments_quantity(const int p_arguments_quantity, const ModeType p_mode_type);

	static void lexical_analysis(const std::string& p_input_expression_file_name,
		const std::string& p_output_tokens_file_name, const std::string& p_output_symbols_file_name);
	static void syntactic_analysis(const std::string& p_input_expression_file_name,
		const std::string& p_output_syntax_tree_file_name);
	static void semantic_analysis(const std::string& p_input_expression_file_name,
		const std::string& p_output_modified_syntax_tree_file_name);

	static void portable_code_generation(const std::string& p_input_expression_file_name, 
		const std::string& p_output_portable_code_file_name, const std::string& p_output_symbols_file_name);
	static void postfix_notation_generation(const std::string& p_input_expression_file_name,
		const std::string& p_output_postfix_notation_file_name, const std::string& p_output_symbols_file_name);
	static void post_code_generation(const std::string& p_input_expression_file_name,
		const std::string& p_output_post_code_file_name);
};
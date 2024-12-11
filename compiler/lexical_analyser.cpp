#include "lexical_analyser.hpp"
#include "error_logger.hpp"
#include "token.hpp"

#include <format>

bool LexicalAnalyser::process_integer_constant(const std::string& p_lexeme) {
	if (p_lexeme.empty()) {
		ErrorLogger::log("empty integer constant", ErrorType::LEXICAL);
		return false;
	}
	bool is_valid = true;
	if (p_lexeme[0] == '0' && p_lexeme.size() > 1) {
		ErrorLogger::log(std::format("extra zero at the beginning of the integer constant \"{}\"", p_lexeme), ErrorType::LEXICAL);
		is_valid = false;
	}
	for (size_t i = 0; i < p_lexeme.size(); i++) {
		char c = p_lexeme[i];
		if (!isdigit(c)) {
			ErrorLogger::log(std::format("invalid character \'{}\' in integer constant \"{}\"", c, p_lexeme), ErrorType::LEXICAL);
			return false;
		}
	}
	return is_valid;
}

bool LexicalAnalyser::process_float_constant(const std::string& p_lexeme) {
	if (p_lexeme.empty()) {
		ErrorLogger::log("empty float constant", ErrorType::LEXICAL);
		return false;
	}
	bool is_valid = true;
	bool has_invalid_character = false;
	bool has_one_point = false;
	bool has_two_point = false;
	if (p_lexeme[0] == '0' && p_lexeme.size() > 1 && p_lexeme[1] != '.') {
		ErrorLogger::log(std::format("extra zero at the beginning of the float constant \"{}\"", p_lexeme), ErrorType::LEXICAL);
		is_valid = false;
	}
	for (size_t i = 0; i < p_lexeme.size(); i++) {
		char c = p_lexeme[i];
		if (isdigit(c)) {
			continue;
		} else if (c == '.') {
			if (has_one_point && !has_two_point) {
				has_two_point = true;
				ErrorLogger::log(std::format("points quantity greater than one in float constant \"{}\"", p_lexeme), ErrorType::LEXICAL);
				is_valid = false;
			} else {
				has_one_point = true;
			}
		} else {
			if (!has_invalid_character) {
				has_invalid_character = true;
				ErrorLogger::log(std::format("invalid character \'{}\' in float constant \"{}\"", c, p_lexeme), ErrorType::LEXICAL);
				is_valid = false;
			}
		}
	}
	if (has_one_point && !has_two_point && p_lexeme.back() == '.') {
		ErrorLogger::log(std::format("no numbers after point in the float constant \"{}\"", p_lexeme), ErrorType::LEXICAL);
		is_valid = false;
	}
	return is_valid;
}

bool LexicalAnalyser::process_identifier(const std::string& p_lexeme) {
	if (p_lexeme.empty()) {
		ErrorLogger::log("empty float identifier", ErrorType::LEXICAL);
		return false;
	}
	bool is_valid = true;
	if (!isalpha(p_lexeme[0]) && p_lexeme[0] != '_') {
		ErrorLogger::log(std::format("invalid character at the beginning of the identifier \"{}\"", p_lexeme), ErrorType::LEXICAL);
		is_valid = false;
	}
	for (size_t i = 1; i < p_lexeme.size(); i++) {
		if (!isalnum(p_lexeme[i]) && p_lexeme[i] != '_') {
			ErrorLogger::log(std::format("invalid character \'{}\' in identifier \"{}\"", p_lexeme[i], p_lexeme), ErrorType::LEXICAL);
			return false;
		}
	}
	return is_valid;
}

bool LexicalAnalyser::analyse(const std::vector<std::string>& p_lexemes) {
	size_t current_position = 0;
	bool is_valid = true;
	bool has_invalid_character = false;
	for (size_t i = 0; i < p_lexemes.size(); i++) {
		std::string lexeme = p_lexemes[i];
		if (!is_bracket(lexeme) && !is_operator(lexeme) && !is_explicit_variable_type(lexeme)) {
			if (is_expected_integer_constant(lexeme)) {
				is_valid &= process_integer_constant(lexeme);
			} else if (is_expected_float_constant(lexeme)) {
				is_valid &= process_float_constant(lexeme);
			} else if (is_expected_variable(lexeme)) {
				is_valid &= process_identifier(lexeme);
			} else if (!has_invalid_character) {
				has_invalid_character = true;
				ErrorLogger::log(std::format("invalid character \'{}\' at position {}", lexeme[0], current_position), ErrorType::LEXICAL);
			}
		}
		current_position += lexeme.size();
	}
	return is_valid;
}
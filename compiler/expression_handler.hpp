#pragma once

#include "token_container.hpp"

#include <string>
#include <vector>
#include <stdexcept>

class ExpressionHandler {
public:

	static std::vector<std::string> lexemize(const std::string& p_expression) {
		if (p_expression.empty()) {
			throw std::invalid_argument("expression length must be greater than zero");
		}
		std::string lexeme;
		std::vector<std::string> lexemes;
		for (size_t i = 0; i < p_expression.size(); i++) {
			char c = p_expression[i];
			if (isspace(c)) {
				if (!lexeme.empty()) {
					lexemes.push_back(lexeme);
					lexeme.clear();
				}
			} else {
				if (is_operator(c) || is_bracket(c)) {
					if (!lexeme.empty()) {
						lexemes.push_back(lexeme);
						lexeme.clear();
					}
					lexemes.push_back(std::string(1, c));
				} else {
					lexeme += c;
				}
			}
		}
		if (!lexeme.empty()) {
			lexemes.push_back(lexeme);
		}
		return lexemes;
	};

	static TokenContainer tokenize(const std::vector<std::string>& p_lexemes) {
		TokenContainer token_container;
		size_t char_position = 0;
		for (size_t i = 0; i < p_lexemes.size(); i++) {
			std::string lexeme = p_lexemes[i];
			if (is_operator(lexeme)) {
				token_container.insert(Token(to_operator_type(lexeme), char_position));
			} else if (is_round_bracket(lexeme)) {
				token_container.insert(Token(to_round_bracket_type(lexeme), char_position));
			} else if (is_square_bracket(lexeme)) {
				token_container.insert(Token(to_square_bracket_type(lexeme), char_position));
			} else if (is_explicit_variable_type(lexeme)) {
				token_container.insert(Token(to_explicit_variable_type(lexeme), char_position));
			} else if (is_expected_integer_constant(lexeme)) {
				token_container.insert(Token(std::stoi(lexeme), char_position));
			} else if (is_expected_float_constant(lexeme)) {
				token_container.insert(Token(std::stof(lexeme), char_position));
			} else if (is_expected_variable(lexeme)) {
				token_container.insert(lexeme, char_position);
			} else {
				throw std::invalid_argument("invalid lexeme");
			}
			char_position += lexeme.size();
		}
		return token_container;
	};
};
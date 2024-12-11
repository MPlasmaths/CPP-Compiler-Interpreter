#include "token.hpp"

#include <format>

std::string describe(const int p_value) {
	return "integer constant";
};
std::string describe(const float p_value) {
	return "floating point constant";
};
std::string describe(const size_t p_identifier) {
	return "identifier";
}
std::string describe(const Token& p_token) {
	TokenValueVariant token_value_variant = p_token.get_value_variant();
	if (std::holds_alternative<int>(token_value_variant)) {
		const int value = std::get<int>(token_value_variant);
		return std::format("{} <{}>", describe(value), value);
	} else if (std::holds_alternative<float>(token_value_variant)) {
		const float value = std::get<float>(token_value_variant);
		return std::format("{} <{}>", describe(value), value);
	} else if (std::holds_alternative<RoundBracketType>(token_value_variant)) {
		const RoundBracketType round_bracket_type = std::get<RoundBracketType>(token_value_variant);
		return std::format("{} <{}>", describe(round_bracket_type), to_char(round_bracket_type));
	} else if (std::holds_alternative<SquareBracketType>(token_value_variant)) {
		const SquareBracketType square_bracket_type = std::get<SquareBracketType>(token_value_variant);
		return std::format("{} <{}>", describe(square_bracket_type), to_char(square_bracket_type));
	} else if (std::holds_alternative<OperatorType>(token_value_variant)) {
		const OperatorType operator_type = std::get<OperatorType>(token_value_variant);
		return std::format("{} <{}>", describe(operator_type), to_char(operator_type));
	} else if (std::holds_alternative<size_t>(token_value_variant)) {
		const size_t identifier = std::get<size_t>(token_value_variant);
		return std::format("{} <id, {}>", describe(identifier), identifier);
	} else {
		return "unknown token";
	}
};

bool is_expected_integer_constant(const std::string& p_lexeme) {
	if (p_lexeme.empty()) {
		return false;
	}
	if (std::isdigit(p_lexeme[0])) {
		for (size_t i = 1; i < p_lexeme.size(); i++) {
			if (p_lexeme[i] == '.') {
				return false;
			}
		}
		return true;
	}
	return false;
};

bool is_expected_float_constant(const std::string& p_lexeme) {
	if (p_lexeme.empty()) {
		return false;
	}
	for (size_t i = 0; i < p_lexeme.size(); i++) {
		if (p_lexeme[i] == '.') {
			return true;
		}
	}
	return false;
};

bool is_expected_variable(const std::string& p_lexeme) {
	if (p_lexeme.empty()) {
		return false;
	}
	for (size_t i = 0; i < p_lexeme.size(); i++) {
		if (p_lexeme[i] == '_' || std::isalpha(p_lexeme[i])) {
			return true;
		}
	}
	return false;
};
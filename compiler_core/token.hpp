#pragma once

#include "operator_type.hpp"
#include "bracket_type.hpp"
#include "explicit_variable_type.hpp"

#include <variant>
#include <string>
#include <format>

using TokenValueVariant = std::variant<size_t, int, float,
	OperatorType, RoundBracketType, SquareBracketType, ExplicitVariableType>;

class Token {
public:

	Token(TokenValueVariant p_value_variant, size_t p_char_position)
		: _value_variant(p_value_variant), _char_position(p_char_position) {}
	Token(TokenValueVariant p_value_variant)
		: _value_variant(p_value_variant), _char_position(0) {}
	Token() : _value_variant(0), _char_position(0) {}

	const TokenValueVariant& get_value_variant() const {
		return _value_variant;
	}
	size_t get_char_position() const {
		return _char_position;
	}

	void set_value_variant(const TokenValueVariant& p_value_variant) {
		_value_variant = p_value_variant;
	}
	void set_char_position(const size_t p_char_position) {
		_char_position = p_char_position;
	}

private:

	TokenValueVariant _value_variant;
	size_t _char_position;
};

std::string describe(const int p_value);
std::string describe(const float p_value);
std::string describe(const size_t p_identifier);
std::string describe(const Token& p_token);

bool is_expected_integer_constant(const std::string& p_lexeme);
bool is_expected_float_constant(const std::string& p_lexeme);
bool is_expected_variable(const std::string& p_lexeme);
#pragma once

#include <string>

enum class RoundBracketType : char {
	OPENING = '(',
	CLOSING = ')'
};
enum class SquareBracketType : char {
	OPENING = '[',
	CLOSING = ']'
};

char to_char(const RoundBracketType p_round_bracket_type);
char to_char(const SquareBracketType p_square_bracket_type);

RoundBracketType to_round_bracket_type(const char p_char);
RoundBracketType to_round_bracket_type(const std::string& p_string);
SquareBracketType to_square_bracket_type(const char p_char);
SquareBracketType to_square_bracket_type(const std::string& p_string);

bool is_round_bracket(const char p_char);
bool is_round_bracket(const std::string& p_string);
bool is_square_bracket(const char p_char);
bool is_square_bracket(const std::string& p_string);
bool is_bracket(const char p_char);
bool is_bracket(const std::string& p_string);

std::string describe(const RoundBracketType p_round_bracket_type);
std::string describe(const SquareBracketType p_square_bracket_type);
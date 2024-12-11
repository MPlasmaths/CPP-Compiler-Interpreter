#include "bracket_type.hpp"

#include <stdexcept>

char to_char(const RoundBracketType p_round_bracket_type) {
	return static_cast<char>(p_round_bracket_type);
};
char to_char(const SquareBracketType p_square_bracket_type) {
	return static_cast<char>(p_square_bracket_type);
};

RoundBracketType to_round_bracket_type(const char p_char) {
	if (p_char == static_cast<char>(RoundBracketType::OPENING)) {
		return RoundBracketType::OPENING;
	} else if (p_char == static_cast<char>(RoundBracketType::CLOSING)) {
		return RoundBracketType::CLOSING;
	} else {
		throw std::invalid_argument("invalid round bracket character");
	}
};
RoundBracketType to_round_bracket_type(const std::string& p_string) {
	if (p_string.size() == 1) {
		return to_round_bracket_type(p_string[0]);
	} else {
		throw std::invalid_argument("invalid round bracket string");
	}
};

SquareBracketType to_square_bracket_type(const char p_char) {
	if (p_char == static_cast<char>(SquareBracketType::OPENING)) {
		return SquareBracketType::OPENING;
	} else if (p_char == static_cast<char>(SquareBracketType::CLOSING)) {
		return SquareBracketType::CLOSING;
	} else {
		throw std::invalid_argument("invalid square bracket character");
	}
};
SquareBracketType to_square_bracket_type(const std::string& p_string) {
	if (p_string.size() == 1) {
		return to_square_bracket_type(p_string[0]);
	} else {
		throw std::invalid_argument("invalid square bracket string");
	}
};

bool is_round_bracket(const char p_char) {
	return p_char == to_char(RoundBracketType::OPENING) ||
		p_char == to_char(RoundBracketType::CLOSING);
};
bool is_round_bracket(const std::string& p_string) {
	return p_string.size() == 1 && is_round_bracket(p_string[0]);
};
bool is_square_bracket(const char p_char) {
	return p_char == to_char(SquareBracketType::OPENING) ||
		p_char == to_char(SquareBracketType::CLOSING);
};
bool is_square_bracket(const std::string& p_string) {
	return p_string.size() == 1 && is_square_bracket(p_string[0]);
};
bool is_bracket(const char p_char) {
	return is_round_bracket(p_char) || is_square_bracket(p_char);
};
bool is_bracket(const std::string& p_string) {
	return p_string.size() == 1 && is_bracket(p_string[0]);
};

std::string describe(const RoundBracketType p_round_bracket_type) {
	switch (p_round_bracket_type) {
	case RoundBracketType::OPENING:
		return "round opening bracket";
	case RoundBracketType::CLOSING:
		return "round closing bracket";
	default:
		return "unknown";
	}
};
std::string describe(const SquareBracketType p_square_bracket_type) {
	switch (p_square_bracket_type) {
	case SquareBracketType::OPENING:
		return "square opening bracket";
	case SquareBracketType::CLOSING:
		return "square closing bracket";
	default:
		return "unknown";
	}
};
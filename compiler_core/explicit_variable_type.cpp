#include "explicit_variable_type.hpp"

#include <cctype>

std::string to_string(const ExplicitVariableType _explicit_variable_type) {
	switch (_explicit_variable_type) {
	case ExplicitVariableType::INT:
		return "int";
	case ExplicitVariableType::FLOAT:
		return "float";
	default:
		return "unknown";
	}
}
char to_char(const ExplicitVariableType _explicit_variable_type) {
	switch (_explicit_variable_type) {
	case ExplicitVariableType::INT:
		return 'i';
	case ExplicitVariableType::FLOAT:
		return 'f';
	default:
		return '?';
	}
}
ExplicitVariableType to_explicit_variable_type(const std::string& _string) {
	std::string lower_string = _string;
	lower_string[0] = tolower(lower_string[0]);
	if (lower_string == "int" || lower_string == "i") {
		return ExplicitVariableType::INT;
	} else if (lower_string == "float" || lower_string == "f") {
		return ExplicitVariableType::FLOAT;
	} else {
		return ExplicitVariableType::UNKNOWN;
	}
}
ExplicitVariableType to_explicit_variable_type(const char _char) {
	return to_explicit_variable_type(std::string(1, _char));
}
bool is_explicit_variable_type(const std::string& _string) {
	return to_explicit_variable_type(_string) != ExplicitVariableType::UNKNOWN;
}

std::string describe(const ExplicitVariableType _explicit_variable_type) {
	switch (_explicit_variable_type) {
	case ExplicitVariableType::INT:
		return "integer variable type";
	case ExplicitVariableType::FLOAT:
		return "floating point variable type";
	default:
		return "unknown variable type";
	}
}
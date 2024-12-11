#pragma once

#include <string>

enum class ExplicitVariableType {
	INT, FLOAT, UNKNOWN
};

std::string to_string(const ExplicitVariableType _explicit_variable_type);
char to_char(const ExplicitVariableType _explicit_variable_type);

ExplicitVariableType to_explicit_variable_type(const std::string& _string);
ExplicitVariableType to_explicit_variable_type(const char _char);

bool is_explicit_variable_type(const std::string& _string);

std::string describe(const ExplicitVariableType _explicit_variable_type);
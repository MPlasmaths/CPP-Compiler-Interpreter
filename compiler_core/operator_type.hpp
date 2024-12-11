#pragma once

#include <string>
#include <stdexcept>

enum class OperatorType : char {
	ADD = '+',
	SUBTRACT = '-',
	MULTIPLY = '*',
	DIVIDE = '/'
};

float calculate(const float, const float, const OperatorType);

char to_char(const OperatorType p_operator_type);
std::string to_string(const OperatorType p_operator_type);

std::string to_operator_code(const OperatorType p_operator_type);
int precedence(const OperatorType p_operator_type);

OperatorType to_operator_type(const char p_char);
OperatorType to_operator_type(const std::string& p_string);

bool is_operator(const char p_char);
bool is_operator(const std::string& p_string);

std::string describe(const OperatorType p_operator_type);
#include "operator_type.hpp"

#include <stdexcept>

float calculate(const float p_first, const float p_second, const OperatorType p_operator_type) {
	switch (p_operator_type) {
	case OperatorType::ADD:
		return p_first + p_second;
	case OperatorType::SUBTRACT:
		return p_first - p_second;
	case OperatorType::MULTIPLY:
		return p_first * p_second;
	case OperatorType::DIVIDE: {
		if (p_second == 0.0) throw std::runtime_error("division by zero detected");
		return p_first / p_second;
	};
	default:
		throw std::runtime_error("unknown operator type");
	}
}

char to_char(const OperatorType p_operator_type) {
	return static_cast<char>(p_operator_type);
}

std::string to_string(const OperatorType p_operator_type) {
	return std::string(1, to_char(p_operator_type));
}

int precedence(const OperatorType p_operator_type) {
	switch (p_operator_type) {
	case OperatorType::ADD:
	case OperatorType::SUBTRACT:
		return 1;
	case OperatorType::MULTIPLY:
	case OperatorType::DIVIDE:
		return 2;
	default:
		return 0;
	}
}

std::string to_operator_code(const OperatorType p_operator_type) {
	switch (p_operator_type) {
	case OperatorType::ADD:
		return "add";
	case OperatorType::SUBTRACT:
		return "sub";
	case OperatorType::MULTIPLY:
		return "mul";
	case OperatorType::DIVIDE:
		return "div";
	default:
		return "unknown";
	}
}

OperatorType to_operator_type(const char p_char) {
	if (p_char == static_cast<char>(OperatorType::ADD)) {
		return OperatorType::ADD;
	} else if (p_char == static_cast<char>(OperatorType::SUBTRACT)) {
		return OperatorType::SUBTRACT;
	} else if (p_char == static_cast<char>(OperatorType::MULTIPLY)) {
		return OperatorType::MULTIPLY;
	} else if (p_char == static_cast<char>(OperatorType::DIVIDE)) {
		return OperatorType::DIVIDE;
	} else {
		throw std::invalid_argument("invalid operator character");
	}
};
OperatorType to_operator_type(const std::string& p_string) {
	if (p_string.size() == 1) {
		return to_operator_type(p_string[0]);
	} else {
		throw std::invalid_argument("invalid operator string");
	}
};

bool is_operator(const char p_char) {
	return p_char == to_char(OperatorType::ADD) ||
		p_char == to_char(OperatorType::SUBTRACT) ||
		p_char == to_char(OperatorType::MULTIPLY) ||
		p_char == to_char(OperatorType::DIVIDE);
};
bool is_operator(const std::string& p_string) {
	return p_string.size() == 1 && is_operator(p_string[0]);
};

std::string describe(const OperatorType p_operator_type) {
	switch (p_operator_type) {
	case OperatorType::ADD:
		return "addition operator";
	case OperatorType::SUBTRACT:
		return "subtraction operator";
	case OperatorType::MULTIPLY:
		return "multiplication operator";
	case OperatorType::DIVIDE:
		return "division operator";
	default:
		return "unknown";
	}
};
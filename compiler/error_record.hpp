#pragma once

#include <string>
#include <cctype>

enum class ErrorType {
	LEXICAL, SYNTACTIC, SEMANTIC, UNKNOWN
};

inline std::string to_string(const ErrorType p_error_type) {
	switch (p_error_type) {
	case ErrorType::LEXICAL:
		return "lexical";
	case ErrorType::SYNTACTIC:
		return "syntactic";
	case ErrorType::SEMANTIC:
		return "syntactic";
	default:
		return "unknown";
	}
}

class ErrorRecord {
public:

	ErrorRecord()
		: _message("empty message"), _type(ErrorType::UNKNOWN) {}
	ErrorRecord(const std::string p_message, const ErrorType p_type)
		: _message(p_message), _type(p_type) {}

	std::string compose() const {
		std::string result = to_string(_type);
		result[0] = std::toupper(result[0]);
		result += " error: " + _message + ".";
		return result;
	};
private:

	std::string _message;
	ErrorType _type;
};
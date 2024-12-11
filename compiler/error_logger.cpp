#include "error_logger.hpp"

#include <iostream>

std::vector<ErrorRecord> ErrorLogger::_error_records;

void ErrorLogger::log(const std::string& p_message, const ErrorType p_type) {
	_error_records.push_back(ErrorRecord(p_message, p_type));
}

void ErrorLogger::print() {
	for (size_t i = 0; i < _error_records.size(); i++) {
		std::cerr << _error_records[i].compose() << "\n";
	}
}

bool ErrorLogger::empty() {
	return _error_records.empty();
}

void ErrorLogger::clear() {
	_error_records.clear();
}
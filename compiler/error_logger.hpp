#pragma once

#include "error_record.hpp"

#include <string>
#include <vector>

class ErrorLogger {
public:

	static void log(const std::string& p_message, const ErrorType p_type);
	static void print();
	static bool empty();
	static void clear();
private:

	static std::vector<ErrorRecord> _error_records;
};
#pragma once

#include <string>
#include <iomanip>
#include <sstream>

class StringHandler {
public:
	static std::string format_float(const float p_value) {
        std::ostringstream oss;
        if (p_value == static_cast<int>(p_value)) {
            oss << std::fixed << std::setprecision(1);
        }
        oss << p_value;
        return oss.str();
	}
};
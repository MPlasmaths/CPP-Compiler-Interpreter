#pragma once

#include <string>
#include <vector>

class LexicalAnalyser {
public:

	static bool analyse(const std::vector<std::string>& p_lexemes);
private:

	static bool process_integer_constant(const std::string& p_lexeme);
	static bool process_float_constant(const std::string& p_lexeme);
	static bool process_identifier(const std::string& p_lexeme);
};
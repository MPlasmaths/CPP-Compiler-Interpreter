#pragma once

#include "token.hpp"
#include "variable_object.hpp"

#include <string>
#include <vector>

class TokenContainer {
public:
	TokenContainer() {};
	TokenContainer(const std::vector<Token>& p_tokens, const std::vector<VariableObject>& p_symbol_table)
		: _token_list(p_tokens), _symbol_table(p_symbol_table) {}

	std::vector<Token> get_token_list() const {
		return _token_list;
	}
	std::vector<VariableObject> get_symbol_table() const {
		return _symbol_table;
	}
	void set_token_list(const std::vector<Token>& p_token_list) {
		_token_list = p_token_list;
	}
	void set_symbol_table(const std::vector<VariableObject>& p_symbol_table) {
		_symbol_table = p_symbol_table;
	}

	void insert(const Token& p_token);
	void insert(const std::string p_name, const size_t p_char_position);

	VariableObject get_symbol(const size_t p_identifier) {
		return _symbol_table[p_identifier];
	}

	void create_symbol(const VariableObject& p_variable_object);

	void set_symbol(const VariableObject& p_variable_object) {
		_symbol_table[p_variable_object.get_identifier()] = p_variable_object;
	}

	void save_used_symbols(const std::vector<bool>& p_presences);

	std::vector<std::string> describe_token_list() const;
	std::vector<std::string> describe_symbol_table() const;

private:

	std::vector<Token> _token_list;
	std::vector<VariableObject> _symbol_table;
};
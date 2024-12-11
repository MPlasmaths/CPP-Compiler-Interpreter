#include "token_container.hpp"
#include "string_handler.hpp"

#include <algorithm>

void TokenContainer::insert(const Token& p_token) {
	_token_list.push_back(p_token);
}

void TokenContainer::insert(const std::string p_name, const size_t p_char_position) {
	for (int i = 0; i < _symbol_table.size(); i++) {
		auto it = std::find_if(_symbol_table.begin(), _symbol_table.end(),
			[p_name](const VariableObject& p_variable_object) { return p_variable_object.get_name() == p_name; });
		if (it != _symbol_table.end()) {
			VariableObject variable_object = VariableObject(*it);
			_token_list.push_back(Token(variable_object.get_identifier(), p_char_position));
			return;
		}
	}
	VariableObject variable_object = VariableObject(_symbol_table.size(), p_name);
	_symbol_table.push_back(variable_object);
	_token_list.push_back((Token(variable_object.get_identifier(), p_char_position)));
}

void TokenContainer::create_symbol(const VariableObject& p_variable_object) {
	_symbol_table.push_back(p_variable_object);
}

void TokenContainer::save_used_symbols(const std::vector<bool>& p_presences) {
	std::vector<VariableObject> new_symbol_table;
	for (size_t i = 0; i < p_presences.size(); i++) {
		if (p_presences[i]) new_symbol_table.push_back(_symbol_table[i]);
	}
	for (size_t i = 0; i < new_symbol_table.size(); i++) {
		new_symbol_table[i].set_identifier(i);
	}
	_symbol_table = new_symbol_table;
}


std::vector<std::string> TokenContainer::describe_token_list() const {
	std::vector<std::string> result;
	for (size_t i = 0; i < _token_list.size(); i++) {
		TokenValueVariant token_value_variant = _token_list[i].get_value_variant();
		if (std::holds_alternative<int>(token_value_variant)) {
			const int value = std::get<int>(token_value_variant);
			result.push_back(std::format("<{}> - {}", value, describe(value)));
		} else if (std::holds_alternative<float>(token_value_variant)) {
			const float value = std::get<float>(token_value_variant);
			result.push_back(std::format("<{}> - {}", StringHandler::format_float(value), describe(value)));
		} else if (std::holds_alternative<RoundBracketType>(token_value_variant)) {
			const RoundBracketType round_bracket_type = std::get<RoundBracketType>(token_value_variant);
			result.push_back(std::format("<{}> - {}", to_char(round_bracket_type), describe(round_bracket_type)));
		} else if (std::holds_alternative<SquareBracketType>(token_value_variant)) {
			const SquareBracketType square_bracket_type = std::get<SquareBracketType>(token_value_variant);
			result.push_back(std::format("<{}> - {}", to_char(square_bracket_type), describe(square_bracket_type)));
		} else if (std::holds_alternative<OperatorType>(token_value_variant)) {
			const OperatorType operator_type = std::get<OperatorType>(token_value_variant);
			result.push_back(std::format("<{}> - {}", to_char(operator_type), describe(operator_type)));
		} else if (std::holds_alternative<ExplicitVariableType>(token_value_variant)) {
			const ExplicitVariableType explicit_variable_type = std::get<ExplicitVariableType>(token_value_variant);
			result.push_back(std::format("<{}> - {}", to_char(explicit_variable_type), describe(explicit_variable_type)));
		} else if (std::holds_alternative<size_t>(token_value_variant)) {
			const size_t identifier = std::get<size_t>(token_value_variant);
			const VariableObject& variable_object = VariableObject(*std::find_if(_symbol_table.begin(), _symbol_table.end(),
				[identifier](const VariableObject& p_variable_object) { return p_variable_object.get_identifier() == identifier; }));
			result.push_back(std::format("<id, {}> - {} \"{}\"", identifier, describe(variable_object), variable_object.get_name()));
		} else {
			result.push_back("unknown token");
		}
	}
	return result;
}

std::vector<std::string> TokenContainer::describe_symbol_table() const {
	std::vector<std::string> result;
	for (size_t i = 0; i < _symbol_table.size(); i++) {
		if (_symbol_table[i].has_undefined_value_type()) {
			result.push_back(std::format("{} - {}", _symbol_table[i].get_identifier(), _symbol_table[i].get_name()));
		} else {
			result.push_back(std::format("{} - {} [{}]", _symbol_table[i].get_identifier(),
				_symbol_table[i].get_name(), describe(_symbol_table[i].get_value_variant())));
		}
		
	}
	return result;
}
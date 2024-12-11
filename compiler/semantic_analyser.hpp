#pragma once

#include "token_container.hpp"
#include "error_logger.hpp"

#include <format>
#include <vector>
#include <string>

class SemanticAnalyser {
public:

	static bool analyse(TokenContainer& p_token_container) {
		std::vector<Token> token_list = p_token_container.get_token_list();
		std::vector<VariableObject> symbol_table = p_token_container.get_symbol_table();

		size_t last_identifier_index = 0;
		for (size_t i = 0; i < token_list.size(); i++) {
			const Token& token = token_list[i];
			const TokenValueVariant& token_value_variant = token.get_value_variant();

			if (std::holds_alternative<size_t>(token_value_variant)) {
				const size_t identifier = std::get<size_t>(token_value_variant);
				VariableObject variable_object = p_token_container.get_symbol(identifier);
				if (i + 2 < token_list.size()) {
					const TokenValueVariant& type_token_value_variant = token_list[i + 2].get_value_variant();
					if (std::holds_alternative<ExplicitVariableType>(type_token_value_variant)) {
						ExplicitVariableType explicit_variable_type = std::get<ExplicitVariableType>(type_token_value_variant);
						bool is_updated = false;
						if (explicit_variable_type == ExplicitVariableType::INT) {
							is_updated = variable_object.update_value_variant(int());
						} else if (explicit_variable_type == ExplicitVariableType::FLOAT) {
							is_updated = variable_object.update_value_variant(float());
						} else {
							ErrorLogger::log(std::format("unknown explicit variable type at position {}",
								token.get_char_position()), ErrorType::SEMANTIC);
							return false;
						}
						if (is_updated) {
							p_token_container.set_symbol(variable_object);
						} else {
							ErrorLogger::log(std::format("attempt to redefine variable type at position {}", i), ErrorType::SEMANTIC);
							return false;
						}
					} else {
						if (variable_object.update_value_variant(int())) {
							p_token_container.set_symbol(variable_object);
						} else {
							ErrorLogger::log(std::format("attempt to redefine variable type at position {}", i), ErrorType::SEMANTIC);
							return false;
						}
					}
				} else {
					if (variable_object.update_value_variant(int())) {
						p_token_container.set_symbol(variable_object);
					} else {
						ErrorLogger::log(std::format("attempt to redefine variable type at position {}", i), ErrorType::SEMANTIC);
						return false;
					}
				}
			}
		}
		return true;
	}
};
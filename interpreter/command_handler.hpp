#pragma once

#include "variable_object.hpp"
#include "command_line.hpp"

#include <iostream>
#include <stdexcept>

class CommandHandler {
public:

	static float process_command_source(const CommandType p_command_type,
		const std::vector<OperandNodeValueVariant>& p_source_variants, std::vector<VariableObject>& p_symbol_table) {
		OperatorType operator_type;
		if (p_command_type != CommandType::I2F) {
			operator_type = to_operator_type(p_command_type);
		}
		OperandNodeValueVariant value_variant;
		bool has_floating_point;
		float result, value;
		for (size_t i = 0; i < p_source_variants.size(); i++) {
			value_variant = p_source_variants[i];
			has_floating_point = false;
			if (std::holds_alternative<int>(value_variant)) {
				value = static_cast<float>(std::get<int>(value_variant));
			} else if (std::holds_alternative<float>(value_variant)) {
				value = std::get<float>(value_variant);
				has_floating_point = true;
			} else if (std::holds_alternative<size_t>(value_variant)) {
				const VariableObjectValueVariant variable_variant =
					p_symbol_table[std::get<size_t>(value_variant)].get_value_variant();
				if (std::holds_alternative<int>(variable_variant)) {
					value = static_cast<float>(std::get<int>(variable_variant));
				} else {
					value = std::get<float>(variable_variant);
					has_floating_point = true;
				}
			} else {
				throw std::runtime_error("unknown source value");
			}

			if (i == 0) {
				result = value;
			} else {
				result = calculate(result, value, operator_type);
				std::cout << ' ' + to_string(operator_type) + ' ';
			}
			if (has_floating_point || p_command_type == CommandType::I2F) {
				std::cout << StringHandler::format_float(value);
			} else {
				std::cout << static_cast<int>(value);
			}
		}
		return result;
	}
};
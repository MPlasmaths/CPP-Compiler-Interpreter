#include "portable_code_generator.hpp"
#include "type_conversion_node.hpp"

#include <vector>
#include <algorithm>

bool PortableCodeGenerator::is_optimized;
std::vector<CommandLine> PortableCodeGenerator::_command_lines;
std::vector<VariableObject> PortableCodeGenerator::_temporary_symbol_table;
std::vector<bool> PortableCodeGenerator::_temporary_usage;
size_t PortableCodeGenerator::_symbol_table_size;

std::vector<CommandLine> PortableCodeGenerator::generate(
	const std::unique_ptr<SyntaxTreeNode>& p_root, TokenContainer& p_token_container, const bool p_is_optimized) {
	is_optimized = p_is_optimized;
	_command_lines.clear();
	_temporary_symbol_table.clear();
	_symbol_table_size = p_token_container.get_symbol_table().size();
	process_node(p_root, p_token_container);
	for (size_t i = 0; i < _temporary_symbol_table.size(); i++) {
		p_token_container.create_symbol(_temporary_symbol_table[i]);
	}
	return _command_lines;
}

std::vector<CommandLine> PortableCodeGenerator::generate(
	const std::unique_ptr<SyntaxTreeNode>& p_root, TokenContainer& p_token_container) {
	return generate(p_root, p_token_container, false);
}

OperandNodeValueVariant PortableCodeGenerator::process_node(
	const std::unique_ptr<SyntaxTreeNode>& p_node, TokenContainer& p_token_container) {
	if (const auto* operand_node = dynamic_cast<const OperandNode*>(p_node.get())) {
		return operand_node->get_value_variant();
	} else if (const auto* type_conversion_node = dynamic_cast<const TypeConversionNode*>(p_node.get())) {
		const OperandNodeValueVariant& operand_node_value_variant = process_node(type_conversion_node->_child, p_token_container);
		size_t result_identifier;
		if (is_optimized) {
			if (std::holds_alternative<size_t>(operand_node_value_variant)) {
				size_t identifier = std::get<size_t>(operand_node_value_variant);
				if (identifier >= p_token_container.get_symbol_table().size()) {
					use_temporary_symbol(identifier);
				}
				result_identifier = find_unused_temporary_symbol_identifier(float());
				if (identifier >= p_token_container.get_symbol_table().size()) {
					free_used_temporary_symbol(identifier);
				}
			} else {
				result_identifier = find_unused_temporary_symbol_identifier(float());
			}
		} else {
			create_temporary_symbol(float());
			result_identifier = _temporary_symbol_table.back().get_identifier();
		}
		use_temporary_symbol(result_identifier);
		_command_lines.push_back(CommandLine(CommandType::I2F, result_identifier, {operand_node_value_variant}));
		return result_identifier;
	} else if (const auto* operator_node = dynamic_cast<const OperatorNode*>(p_node.get())) {
		const OperandNodeValueVariant& left = process_node(operator_node->_left, p_token_container);
		const OperandNodeValueVariant& right = process_node(operator_node->_right, p_token_container);
		
		if (is_optimized) {
			if (std::holds_alternative<size_t>(left)) {
				size_t identifier = std::get<size_t>(left);
				if (identifier >= p_token_container.get_symbol_table().size()) {
					use_temporary_symbol(identifier);
				}
			}
			if (std::holds_alternative<size_t>(right)) {
				size_t identifier = std::get<size_t>(right);
				if (identifier >= p_token_container.get_symbol_table().size()) {
					use_temporary_symbol(identifier);
				}
			}
		}

		ExplicitVariableType left_variable_type;
		if (std::holds_alternative<size_t>(left)) {
			const size_t identifier = std::get<size_t>(left);
			VariableObjectValueVariant variable_object_value_variant;
			if (identifier < p_token_container.get_symbol_table().size()) {
				variable_object_value_variant = p_token_container.get_symbol(identifier).get_value_variant();
			} else {
				variable_object_value_variant = find_temporary_symbol(identifier).get_value_variant();
			}
			if (std::holds_alternative<int>(variable_object_value_variant)) {
				left_variable_type = ExplicitVariableType::INT;
			} else if (std::holds_alternative<float>(variable_object_value_variant)) {
				left_variable_type = ExplicitVariableType::FLOAT;
			} else {
				throw std::runtime_error("unknown variable value");
			}
		} else if (std::holds_alternative<int>(left)) {
			left_variable_type = ExplicitVariableType::INT;
		} else if (std::holds_alternative<float>(left)) {
			left_variable_type = ExplicitVariableType::FLOAT;
		} else {
			throw std::runtime_error("unknown operand node");
		}

		size_t result_identifier;
		if (left_variable_type == ExplicitVariableType::FLOAT) {
			if (is_optimized) {
				result_identifier = find_unused_temporary_symbol_identifier(float());
			} else {
				create_temporary_symbol(float());
				result_identifier = _temporary_symbol_table.back().get_identifier();
			}
		} else {
			ExplicitVariableType right_variable_type;
			if (std::holds_alternative<size_t>(right)) {
				const size_t identifier = std::get<size_t>(right);
				VariableObjectValueVariant variable_object_value_variant;
				if (identifier < p_token_container.get_symbol_table().size()) {
					variable_object_value_variant = p_token_container.get_symbol(identifier).get_value_variant();
				} else {
					variable_object_value_variant = find_temporary_symbol(identifier).get_value_variant();
				}
				if (std::holds_alternative<int>(variable_object_value_variant)) {
					right_variable_type = ExplicitVariableType::INT;
				} else if (std::holds_alternative<float>(variable_object_value_variant)) {
					right_variable_type = ExplicitVariableType::FLOAT;
				} else {
					throw std::runtime_error("unknown variable value");
				}
			} else if (std::holds_alternative<int>(right)) {
				right_variable_type = ExplicitVariableType::INT;
			} else if (std::holds_alternative<float>(right)) {
				right_variable_type = ExplicitVariableType::FLOAT;
			} else {
				throw std::runtime_error("unknown operand node");
			}

			if (right_variable_type == ExplicitVariableType::FLOAT) {
				if (is_optimized) {
					result_identifier = find_unused_temporary_symbol_identifier(float());
				} else {
					create_temporary_symbol(float());
					result_identifier = _temporary_symbol_table.back().get_identifier();
				}
			} else {
				if (is_optimized) {
					result_identifier = find_unused_temporary_symbol_identifier(int());
				} else {
					create_temporary_symbol(int());
					result_identifier = _temporary_symbol_table.back().get_identifier();
				}
			}
		}
		use_temporary_symbol(result_identifier);
		_command_lines.push_back(CommandLine(to_command_type(operator_node->get_operator_type()),
			result_identifier, std::vector<OperandNodeValueVariant>{left, right}));
		if (std::holds_alternative<size_t>(left)) {
			size_t identifier = std::get<size_t>(left);
			if (identifier >= p_token_container.get_symbol_table().size()) {
				free_used_temporary_symbol(identifier);
			}
		}
		if (std::holds_alternative<size_t>(right)) {
			size_t identifier = std::get<size_t>(right);
			if (identifier >= p_token_container.get_symbol_table().size()) {
				free_used_temporary_symbol(identifier);
			}
		}
		return result_identifier;
	} else {
		throw std::runtime_error("unknown node");
	}
}

void PortableCodeGenerator::create_temporary_symbol(const VariableObjectValueVariant& p_value_variant) {
	size_t identifier = _temporary_symbol_table.size();
	_temporary_symbol_table.push_back(VariableObject(
		_symbol_table_size + identifier, "#T" + std::to_string(identifier), p_value_variant));
	_temporary_usage.push_back(true);
}

VariableObject& PortableCodeGenerator::find_temporary_symbol(const size_t p_identifier) {
	auto it = std::find_if(
		_temporary_symbol_table.begin(), _temporary_symbol_table.end(),
		[p_identifier](const VariableObject& p_variable_object) { return p_variable_object.get_identifier() == p_identifier; }
	);

	if (it == _temporary_symbol_table.end()) {
		throw std::runtime_error("temporary symbol not found");
	}

	return *it;
}

size_t PortableCodeGenerator::find_unused_temporary_symbol_identifier(const VariableObjectValueVariant& p_value_variant) {
	
	for (size_t i = 0; i < _temporary_usage.size(); i++) {
		if (_temporary_usage[i] && _temporary_symbol_table[i].get_value_variant() == p_value_variant) {
			return _symbol_table_size + i;
		}
	}
	create_temporary_symbol(p_value_variant);
	return _symbol_table_size + _temporary_symbol_table.size() - 1;
}

void PortableCodeGenerator::use_temporary_symbol(const size_t p_identifier) {
	_temporary_usage[p_identifier - _symbol_table_size] = false;
}

void PortableCodeGenerator::free_used_temporary_symbol(const size_t p_identifier) {
	_temporary_usage[p_identifier - _symbol_table_size] = true;
}
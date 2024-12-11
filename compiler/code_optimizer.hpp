#pragma once

#include "syntax_tree_node.hpp"
#include "type_conversion_node.hpp"
#include "token_container.hpp"

class CodeOptimizer {
public:

	static void optimize(std::unique_ptr<SyntaxTreeNode>& p_root, TokenContainer& p_token_container) {
		traverse(p_root, p_token_container);
		std::vector<bool> identifier_presences(p_token_container.get_symbol_table().size(), false);
		check_identifiers(p_root, p_token_container, identifier_presences);
		p_token_container.save_used_symbols(identifier_presences);
		update_identifiers(p_root, p_token_container, identifier_presences);
	}

private:
	static void traverse(std::unique_ptr<SyntaxTreeNode>& p_node, const TokenContainer& p_token_container) {
		if (!p_node) return;

		if (auto* operator_node = dynamic_cast<OperatorNode*>(p_node.get())) {

			traverse(operator_node->_left, p_token_container);
			traverse(operator_node->_right, p_token_container);

			auto* left_operand_node = dynamic_cast<OperandNode*>(operator_node->_left.get());
			if (left_operand_node) {
				OperandNodeValueVariant left_value_variant = left_operand_node->get_value_variant();
				float value;
				if (!std::holds_alternative<size_t>(left_value_variant)) {
					if (std::holds_alternative<int>(left_value_variant)) {
						value = static_cast<float>(std::get<int>(left_value_variant));
					} else {
						value = std::get<float>(left_value_variant);
					}
					if (value == 0.0) {
						if (operator_node->get_operator_type() == OperatorType::ADD) {
							p_node = std::move(operator_node->_right);
							return;
						} else if (operator_node->get_operator_type() == OperatorType::MULTIPLY ||
							operator_node->get_operator_type() == OperatorType::DIVIDE) {
							p_node = std::move(operator_node->_left);
							return;
						}
					} else if (value == 1.0 && operator_node->get_operator_type() == OperatorType::MULTIPLY) {
						p_node = std::move(operator_node->_right);
						return;
					}
				}
			}

			auto* right_operand_node = dynamic_cast<OperandNode*>(operator_node->_right.get());
			if (right_operand_node) {
				OperandNodeValueVariant right_value_variant = right_operand_node->get_value_variant();
				float value;
				if (!std::holds_alternative<size_t>(right_value_variant)) {
					if (std::holds_alternative<int>(right_value_variant)) {
						value = static_cast<float>(std::get<int>(right_value_variant));
					} else {
						value = std::get<float>(right_value_variant);
					}
					if (value == 0.0) {
						if (operator_node->get_operator_type() == OperatorType::ADD ||
							operator_node->get_operator_type() == OperatorType::SUBTRACT) {
							p_node = std::move(operator_node->_left);
							return;
						} else if (operator_node->get_operator_type() == OperatorType::MULTIPLY) {
							p_node = std::move(operator_node->_right);
							return;
						} else {
							throw std::runtime_error("division by zero detected");
						}
					} else if (value == 1.0 && (operator_node->get_operator_type() == OperatorType::MULTIPLY ||
						operator_node->get_operator_type() == OperatorType::DIVIDE)) {
						p_node = std::move(operator_node->_left);
						return;
					}
				}
			}

			if (left_operand_node && right_operand_node) {
				OperandNodeValueVariant left_value_variant = left_operand_node->get_value_variant();
				OperandNodeValueVariant right_value_variant = right_operand_node->get_value_variant();

				if (std::holds_alternative<size_t>(left_value_variant) &&
					std::holds_alternative<size_t>(right_value_variant)) {
					const size_t left_identifier = std::get<size_t>(left_value_variant);
					if (left_identifier == std::get<size_t>(right_value_variant) &&
						operator_node->get_operator_type() == OperatorType::SUBTRACT) {
						VariableObjectValueVariant left_variable_object_value_variant =
							p_token_container.get_symbol_table()[left_identifier].get_value_variant();
						if (std::holds_alternative<int>(left_variable_object_value_variant)) {
							p_node = std::make_unique<OperandNode>(int());
						} else {
							p_node = std::make_unique<OperandNode>(float());
						}
						return;
					}
				} else if (std::holds_alternative<size_t>(left_value_variant) || std::holds_alternative<size_t>(right_value_variant)) {
					float value;
					if (!std::holds_alternative<size_t>(left_value_variant)) {
						if (std::holds_alternative<int>(left_value_variant)) {
							value = static_cast<float>(std::get<int>(left_value_variant));
						} else {
							value = std::get<float>(left_value_variant);
						}
						if (value == 0.0) {
							if (operator_node->get_operator_type() == OperatorType::ADD) {
								p_node = std::move(operator_node->_right);
								return;
							} else if (operator_node->get_operator_type() == OperatorType::MULTIPLY ||
								operator_node->get_operator_type() == OperatorType::DIVIDE) {
								p_node = std::move(operator_node->_left);
								return;
							}
						} else if (value == 1.0 && operator_node->get_operator_type() == OperatorType::MULTIPLY) {
							p_node = std::move(operator_node->_right);
							return;
						}
					}
					if (!std::holds_alternative<size_t>(right_value_variant)) {
						if (std::holds_alternative<int>(right_value_variant)) {
							value = static_cast<float>(std::get<int>(right_value_variant));
						} else {
							value = std::get<float>(right_value_variant);
						}
						if (value == 0.0) {
							if (operator_node->get_operator_type() == OperatorType::ADD ||
								operator_node->get_operator_type() == OperatorType::SUBTRACT) {
								p_node = std::move(operator_node->_left);
								return;
							} else if (operator_node->get_operator_type() == OperatorType::MULTIPLY) {
								p_node = std::move(operator_node->_right);
								return;
							} else {
								throw std::runtime_error("division by zero detected");
							}
						} else if (value == 1.0 && (operator_node->get_operator_type() == OperatorType::MULTIPLY ||
							operator_node->get_operator_type() == OperatorType::DIVIDE)) {
							p_node = std::move(operator_node->_left);
							return;
						}
					}
				} else {
					ExplicitVariableType variable_type = ExplicitVariableType::INT;
					float left_value;
					float right_value;
					if (std::holds_alternative<int>(left_value_variant)) {
						left_value = static_cast<float>(std::get<int>(left_value_variant));
					} else {
						left_value = std::get<float>(left_value_variant);
						variable_type = ExplicitVariableType::FLOAT;
					}
					if (std::holds_alternative<int>(right_value_variant)) {
						right_value = static_cast<float>(std::get<int>(right_value_variant));
					} else {
						right_value = std::get<float>(right_value_variant);
						variable_type = ExplicitVariableType::FLOAT;
					}
					float new_value = calculate(left_value, right_value, operator_node->get_operator_type());
					if (variable_type == ExplicitVariableType::INT) {
						p_node = std::make_unique<OperandNode>(static_cast<int>(new_value));
					} else {
						p_node = std::make_unique<OperandNode>(new_value);
					}
				}
			}

		} else if (auto* operand_node = dynamic_cast<OperandNode*>(p_node.get())) {

		} else if (auto* type_conversion_node = dynamic_cast<TypeConversionNode*>(p_node.get())) {
			traverse(type_conversion_node->_child, p_token_container);
			if (auto* child_operand_node = dynamic_cast<OperandNode*>(type_conversion_node->_child.get())) {
				OperandNodeValueVariant value_variant = child_operand_node->get_value_variant();
				if (std::holds_alternative<int>(value_variant)) {
					p_node = std::make_unique<OperandNode>(static_cast<float>(std::get<int>(value_variant)));
				}
			}
		} else {
			throw std::runtime_error("unknown node");
		}
	}

	static void check_identifiers(std::unique_ptr<SyntaxTreeNode>& p_node, TokenContainer& p_token_container,
		std::vector<bool>& p_identifier_presences) {
		if (!p_node) return;

		if (auto* operator_node = dynamic_cast<OperatorNode*>(p_node.get())) {
			check_identifiers(operator_node->_left, p_token_container, p_identifier_presences);
			check_identifiers(operator_node->_right, p_token_container, p_identifier_presences);
		} else if (auto* type_conversion_node = dynamic_cast<TypeConversionNode*>(p_node.get())) {
			check_identifiers(type_conversion_node->_child, p_token_container, p_identifier_presences);
		} else if (auto* operand_node = dynamic_cast<OperandNode*>(p_node.get())) {
			OperandNodeValueVariant value_variant = operand_node->get_value_variant();
			if (std::holds_alternative<size_t>(value_variant)) {
				p_identifier_presences[std::get<size_t>(value_variant)] = true;
			}
		} else {
			throw std::runtime_error("unknown node");
		}
	}

	static void update_identifiers(std::unique_ptr<SyntaxTreeNode>& p_node, TokenContainer& p_token_container,
		const std::vector<bool>& p_identifier_presences) {
		if (!p_node) return;

		if (auto* operator_node = dynamic_cast<OperatorNode*>(p_node.get())) {
			update_identifiers(operator_node->_left, p_token_container, p_identifier_presences);
			update_identifiers(operator_node->_right, p_token_container, p_identifier_presences);
		} else if (auto* type_conversion_node = dynamic_cast<TypeConversionNode*>(p_node.get())) {
			update_identifiers(type_conversion_node->_child, p_token_container, p_identifier_presences);
		} else if (auto* operand_node = dynamic_cast<OperandNode*>(p_node.get())) {
			OperandNodeValueVariant value_variant = operand_node->get_value_variant();
			if (std::holds_alternative<size_t>(value_variant)) {
				const size_t identifier = std::get<size_t>(value_variant);
				size_t diff = 0;
				for (size_t i = 0; i < identifier; i++) {
					if (!p_identifier_presences[i]) diff++;
				}
				p_node = std::make_unique<OperandNode>(identifier - diff);
			}
		} else {
			throw std::runtime_error("unknown node");
		}
	}
};
#include "syntax_tree_handler.hpp"
#include "type_conversion_node.hpp"

#include <stack>
#include <format>
#include <typeindex>
#include <optional>

std::unique_ptr<SyntaxTreeNode> SyntaxTreeHandler::build(const TokenContainer& p_token_container) {
	const std::vector<Token>& token_list = p_token_container.get_token_list();
	std::stack<std::variant<OperatorType, RoundBracketType>> operator_stack;
	std::stack<std::unique_ptr<SyntaxTreeNode>> operand_stack;

	for (const Token& token : token_list) {
		TokenValueVariant token_value_variant = token.get_value_variant();
		if (std::holds_alternative<int>(token_value_variant)) {
			operand_stack.push(std::make_unique<OperandNode>(std::get<int>(token_value_variant)));
		} else if (std::holds_alternative<float>(token_value_variant)) {
			operand_stack.push(std::make_unique<OperandNode>(std::get<float>(token_value_variant)));
		} else if (std::holds_alternative<size_t>(token_value_variant)) {
			operand_stack.push(std::make_unique<OperandNode>(std::get<size_t>(token_value_variant)));
		} else if (std::holds_alternative<OperatorType>(token_value_variant)) {
			OperatorType operator_type = std::get<OperatorType>(token_value_variant);

			while (!operator_stack.empty() &&
				std::holds_alternative<OperatorType>(operator_stack.top()) &&
				precedence(std::get<OperatorType>(operator_stack.top())) >= precedence(operator_type)) {
				auto right = std::move(operand_stack.top());
				operand_stack.pop();
				auto left = std::move(operand_stack.top());
				operand_stack.pop();

				operand_stack.push(std::make_unique<OperatorNode>(
					std::get<OperatorType>(operator_stack.top()), std::move(left), std::move(right)));
				operator_stack.pop();
			}
			operator_stack.push(operator_type);
		} else if (std::holds_alternative<RoundBracketType>(token_value_variant)) {
			RoundBracketType round_bracket_type = std::get<RoundBracketType>(token_value_variant);
			if (round_bracket_type == RoundBracketType::OPENING) {
				operator_stack.push(round_bracket_type);
			} else if (round_bracket_type == RoundBracketType::CLOSING) {
				while (!operator_stack.empty() &&
					!(std::holds_alternative<RoundBracketType>(operator_stack.top()) &&
					std::get<RoundBracketType>(operator_stack.top()) == RoundBracketType::OPENING)) {
					auto right = std::move(operand_stack.top());
					operand_stack.pop();
					auto left = std::move(operand_stack.top());
					operand_stack.pop();
					operand_stack.push(std::make_unique<OperatorNode>(
						std::get<OperatorType>(operator_stack.top()), std::move(left), std::move(right)));
					operator_stack.pop();
				}

				if (!operator_stack.empty()) {
					operator_stack.pop();
				}
			}
		}
	}

	while (!operator_stack.empty()) {
		auto right = std::move(operand_stack.top());
		operand_stack.pop();
		auto left = std::move(operand_stack.top());
		operand_stack.pop();

		operand_stack.push(std::make_unique<OperatorNode>(
			std::get<OperatorType>(operator_stack.top()), std::move(left), std::move(right)));
		operator_stack.pop();
	}

	return std::move(operand_stack.top());
};

std::unique_ptr<SyntaxTreeNode> SyntaxTreeHandler::modify(
	const std::unique_ptr<SyntaxTreeNode>& p_root, const TokenContainer& p_token_container) {

	auto get_value_variant = [&](size_t p_identifier) -> VariableObjectValueVariant {
		return p_token_container.get_symbol_table()[p_identifier].get_value_variant();
	};

	auto process_node = [&](const auto& self, const std::unique_ptr<SyntaxTreeNode>& node) -> std::unique_ptr<SyntaxTreeNode> {
		if (auto operand_node = dynamic_cast<OperandNode*>(node.get())) {
			return std::make_unique<OperandNode>(operand_node->get_value_variant());
		} else if (auto operator_node = dynamic_cast<OperatorNode*>(node.get())) {
			auto left = self(self, operator_node->_left);
			auto right = self(self, operator_node->_right);


			auto left_type = SyntaxTreeHandler::infer_type(left, p_token_container);
			auto right_type = SyntaxTreeHandler::infer_type(right, p_token_container);

			if (operator_node->get_operator_type() == OperatorType::DIVIDE) {
				if (auto* right_operand = dynamic_cast<OperandNode*>(operator_node->_right.get())) {
					const auto& right_value_variant = right_operand->get_value_variant();
					if (std::holds_alternative<int>(right_value_variant) && std::get<int>(right_value_variant) == 0) {
						throw std::runtime_error("division by zero detected");
					}
					if (std::holds_alternative<float>(right_value_variant) && std::get<float>(right_value_variant) == 0.0f) {
						throw std::runtime_error("division by zero detected");
					}
				}
			}

			if (std::holds_alternative<int>(left_type) && std::holds_alternative<float>(right_type)) {
				left = std::make_unique<TypeConversionNode>(std::move(left));
			} else if (std::holds_alternative<float>(left_type) && std::holds_alternative<int>(right_type)) {
				right = std::make_unique<TypeConversionNode>(std::move(right));
			}

			return std::make_unique<OperatorNode>(operator_node->get_operator_type(), std::move(left), std::move(right));
		}

		throw std::runtime_error("unknown node");
	};

	return process_node(process_node, p_root);
}

std::variant<int, float> SyntaxTreeHandler::infer_type(
	const std::unique_ptr<SyntaxTreeNode>& p_node,
	const TokenContainer& p_token_container) {

	if (const auto* operand_node = dynamic_cast<OperandNode*>(p_node.get())) {
		const auto& value_variant = operand_node->get_value_variant();

		if (std::holds_alternative<int>(value_variant)) {
			return int();
		} else if (std::holds_alternative<float>(value_variant)) {
			return float();
		} else if (std::holds_alternative<size_t>(value_variant)) {
			size_t identifier = std::get<size_t>(value_variant);
			const auto& variable_value_variant = p_token_container.get_symbol_table()[identifier].get_value_variant();

			if (std::holds_alternative<int>(variable_value_variant)) {
				return int();
			} else if (std::holds_alternative<float>(variable_value_variant)) {
				return float();
			} else {
				throw std::runtime_error("unsupported variable type");
			}
		}
	} else if (const auto* operator_node = dynamic_cast<OperatorNode*>(p_node.get())) {
		auto left_type = infer_type(operator_node->_left, p_token_container);
		auto right_type = infer_type(operator_node->_right, p_token_container);

		if (std::holds_alternative<float>(left_type) || std::holds_alternative<float>(right_type)) {
			return float();
		}
		return int();
	}
	return float();
}

void SyntaxTreeHandler::collect(const std::unique_ptr<SyntaxTreeNode>& p_node,
	std::vector<std::string>& p_lines, const int p_depth) {
	if (!p_node) {
		return;
	}

	std::string prefix = "";
	
	if (p_depth > 0) {
		prefix = std::string((p_depth - 1) * 4, ' ') + "|---";
	}

	p_lines.push_back(prefix + p_node->to_string());

	if (const auto* operator_node = dynamic_cast<const OperatorNode*>(p_node.get())) {
		collect(operator_node->_left, p_lines, p_depth + 1);
		collect(operator_node->_right, p_lines, p_depth + 1);
	} else if (const auto* type_conversion_node = dynamic_cast<const TypeConversionNode*>(p_node.get())) {
		collect(type_conversion_node->_child, p_lines, p_depth + 1);
	}
};

std::vector<std::string> SyntaxTreeHandler::compose(const std::unique_ptr<SyntaxTreeNode>& p_root) {
	std::vector<std::string> lines;
	collect(p_root, lines);
	return lines;
}
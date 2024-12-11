#pragma once

#include "operator_type.hpp"
#include "string_handler.hpp"

#include <variant>
#include <format>

class SyntaxTreeNode {
public:
	virtual ~SyntaxTreeNode() = default;
	virtual std::string to_string() const = 0;
	virtual std::unique_ptr<SyntaxTreeNode> clone() const = 0;
};

using OperandNodeValueVariant = std::variant<size_t, int, float>;

class OperandNode : public SyntaxTreeNode {
public:

	OperandNode() : _value_variant(int()) {}
	OperandNode(const OperandNodeValueVariant p_value_variant) :
		_value_variant(p_value_variant) {}

	OperandNodeValueVariant get_value_variant() const {
		return _value_variant;
	}

	std::string to_string() const override {
		if (std::holds_alternative<int>(_value_variant)) {
			return std::format("<{}>", std::get<int>(_value_variant));
		} else if (std::holds_alternative<float>(_value_variant)) {
			return std::format("<{}>", StringHandler::format_float(std::get<float>(_value_variant)));
		} else if (std::holds_alternative<size_t>(_value_variant)) {
			return std::format("<id, {}>", std::get<size_t>(_value_variant));
		} else {
			return "<?>";
		}
	}
	std::unique_ptr<SyntaxTreeNode> clone() const override {
		return std::make_unique<OperandNode>(*this);
	}
private:

	OperandNodeValueVariant _value_variant;
};

inline std::string to_string(const OperandNodeValueVariant p_operand_node_value_variant) {
	if (std::holds_alternative<int>(p_operand_node_value_variant)) {
		return std::format("{}", std::get<int>(p_operand_node_value_variant));
	} else if (std::holds_alternative<float>(p_operand_node_value_variant)) {
		return std::format("{}", StringHandler::format_float(std::get<float>(p_operand_node_value_variant)));
	} else if (std::holds_alternative<size_t>(p_operand_node_value_variant)) {
		return std::format("<id, {}>", std::get<size_t>(p_operand_node_value_variant));
	} else {
		return "<?>";
	}
}

class OperatorNode : public SyntaxTreeNode {
public:

	std::unique_ptr<SyntaxTreeNode> _left;
	std::unique_ptr<SyntaxTreeNode> _right;

	OperatorNode(const OperatorType p_operator_type,
		std::unique_ptr<SyntaxTreeNode> p_left, std::unique_ptr<SyntaxTreeNode> p_right)
		: _operator_type(p_operator_type), _left(std::move(p_left)), _right(std::move(p_right)) {}

	OperatorType get_operator_type() const {
		return _operator_type;
	}

	std::string to_string() const override {
		return std::format("<{}>", to_char(_operator_type));
	}
	void set_left(std::unique_ptr<SyntaxTreeNode> p_left) { _left = std::move(p_left); }
	void set_right(std::unique_ptr<SyntaxTreeNode> p_right) { _right = std::move(p_right); }

	std::unique_ptr<SyntaxTreeNode> clone() const override {
		auto cloned_node = std::make_unique<OperatorNode>(_operator_type, nullptr, nullptr);
		if (_left) {
			cloned_node->_left = _left->clone();
		}
		if (_right) {
			cloned_node->_right = _right->clone();
		}
		return cloned_node;
	}
private:

	OperatorType _operator_type;
};
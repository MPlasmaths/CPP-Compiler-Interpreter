#pragma once

#include "syntax_tree_node.hpp"
#include "type_conversion_node.hpp"

#include <string>
#include <vector>

class PostfixNotationGenerator {
public:

	static std::vector<std::unique_ptr<SyntaxTreeNode>> generate(const std::unique_ptr<SyntaxTreeNode>& p_root) {
		std::vector<std::unique_ptr<SyntaxTreeNode>> result;
		traverse(p_root, result);
		return result;
	}
private:

	static void traverse(const std::unique_ptr<SyntaxTreeNode>& p_node,
		std::vector<std::unique_ptr<SyntaxTreeNode>>& p_result) {
		if (!p_node) {
			return;
		}
		if (const auto* operator_node = dynamic_cast<const OperatorNode*>(p_node.get())) {
			traverse(operator_node->_left, p_result);
			traverse(operator_node->_right, p_result);
		} else if (const auto* type_conversion_node = dynamic_cast<const TypeConversionNode*>(p_node.get())) {
			traverse(type_conversion_node->_child, p_result);
		}
		p_result.push_back(p_node->clone());
	}
};

inline std::string to_string(const std::vector<std::unique_ptr<SyntaxTreeNode>>& p_postfix_notation) {
	std::string result;
	for (size_t i = 0; i < p_postfix_notation.size(); i++) {
		result += p_postfix_notation[i]->to_string() + ' ';
	}
	if (!result.empty()) {
		result.pop_back();
	}
	return result;
}
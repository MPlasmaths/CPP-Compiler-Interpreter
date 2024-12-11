#pragma once

#include "token_container.hpp"
#include "syntax_tree_node.hpp"

#include <string>
#include <vector>
#include <variant>

class SyntaxTreeHandler {
public:

	static std::unique_ptr<SyntaxTreeNode> build(const TokenContainer& p_token_container);
	static std::unique_ptr<SyntaxTreeNode> modify(const std::unique_ptr<SyntaxTreeNode>& p_root, const TokenContainer& p_token_container);
	static std::vector<std::string> compose(const std::unique_ptr<SyntaxTreeNode>& p_root);
private:
	static std::variant<int, float> infer_type(const std::unique_ptr<SyntaxTreeNode>& p_node, const TokenContainer& p_token_container);
	static void collect(const std::unique_ptr<SyntaxTreeNode>& p_node, std::vector<std::string>& p_lines, const int p_depth = 0);
};
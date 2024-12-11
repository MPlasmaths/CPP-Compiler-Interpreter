#pragma once

#include "syntax_tree_node.hpp"

class TypeConversionNode : public SyntaxTreeNode {
public:
    explicit TypeConversionNode(std::unique_ptr<SyntaxTreeNode> p_child)
        : _child(std::move(p_child)) {}

    std::unique_ptr<SyntaxTreeNode> _child;

    std::string to_string() const override {
        return "<i2f>";
    }

    std::unique_ptr<SyntaxTreeNode> clone() const override {
        if (_child) {
            return std::make_unique<TypeConversionNode>(_child->clone());
        }
        return std::make_unique<TypeConversionNode>(nullptr);
    }

private:
    
    
};
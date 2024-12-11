#pragma once

#include "token_container.hpp"
#include "command_line.hpp"

#include <string>
#include <variant>

class PortableCodeGenerator {
public:

	static std::vector<CommandLine> generate(
		const std::unique_ptr<SyntaxTreeNode>& p_root, TokenContainer& p_token_container, const bool p_is_optimized);
	static std::vector<CommandLine> generate(
		const std::unique_ptr<SyntaxTreeNode>& p_root, TokenContainer& p_token_container);
private:

	static bool is_optimized;
	static std::vector<CommandLine> _command_lines;
	static std::vector<VariableObject> _temporary_symbol_table;
	static std::vector<bool> _temporary_usage;
	static size_t _symbol_table_size;

	static OperandNodeValueVariant process_node(
		const std::unique_ptr<SyntaxTreeNode>& p_node, TokenContainer& p_token_container);

	static void create_temporary_symbol(const VariableObjectValueVariant& p_value_variant);
	
	static VariableObject& find_temporary_symbol(const size_t p_identifier);
	static size_t find_unused_temporary_symbol_identifier(const VariableObjectValueVariant& p_value_variant);
	
	static void use_temporary_symbol(const size_t p_identifier);
	static void free_used_temporary_symbol(const size_t p_identifier);
};
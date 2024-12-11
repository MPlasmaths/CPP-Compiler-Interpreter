#pragma once

#include "syntax_tree_node.hpp"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

enum class CommandType {
	ADD,
	SUB,
	MUL,
	DIV,
	I2F
};

std::string to_string(const CommandType p_command_type);

CommandType to_command_type(const OperatorType p_operator_type);
OperatorType to_operator_type(const CommandType p_command_type);

class CommandLine {
public:

	CommandLine(const CommandType p_type, const size_t p_result_identifier,
		const std::vector<OperandNodeValueVariant>& p_source_value_variants) {
		_type = p_type;
		_result_identifier = p_result_identifier;
		_source_value_variants = p_source_value_variants;
	}

	CommandType get_type() const {
		return _type;
	}

	size_t get_result_identifier() const {
		return _result_identifier;
	}

	OperandNodeValueVariant get_source_value_variant(const size_t p_identifier) const {
		return _source_value_variants[p_identifier];
	}

	std::vector<OperandNodeValueVariant> get_source_value_variants() const {
		return _source_value_variants;
	}

	std::string to_string() const;

	void serialize(std::ofstream& p_out) const;
private:

	CommandType _type;
	size_t _result_identifier;
	std::vector<OperandNodeValueVariant> _source_value_variants;
};

std::vector<std::string> to_string(const std::vector<CommandLine>& p_command_lines);
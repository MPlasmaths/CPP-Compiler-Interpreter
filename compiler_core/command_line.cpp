#pragma once

#include "command_line.hpp"

#include <variant>
#include <format>
#include <stdexcept>

std::string to_string(const CommandType p_command_type) {
	switch (p_command_type) {
	case CommandType::ADD:
		return "add";
	case CommandType::SUB:
		return "sub";
	case CommandType::MUL:
		return "mul";
	case CommandType::DIV:
		return "div";
	case CommandType::I2F:
		return "i2f";
	default:
		return "unknown command type";
	}
}

CommandType to_command_type(const OperatorType p_operator_type) {
	switch (p_operator_type) {
	case OperatorType::ADD:
		return CommandType::ADD;
	case OperatorType::SUBTRACT:
		return CommandType::SUB;
	case OperatorType::MULTIPLY:
		return CommandType::MUL;
	case OperatorType::DIVIDE:
		return CommandType::DIV;
	default:
		throw std::invalid_argument("unknown operator type");
	}
}

OperatorType to_operator_type(const CommandType p_command_type) {
	switch (p_command_type) {
	case CommandType::ADD:
		return OperatorType::ADD;
	case CommandType::SUB:
		return OperatorType::SUBTRACT;
	case CommandType::MUL:
		return OperatorType::MULTIPLY;
	case CommandType::DIV:
		return OperatorType::DIVIDE;
	default:
		throw std::invalid_argument("unknown command type for operator type translation");
	}
}

std::string CommandLine::to_string() const {
	std::string result = ::to_string(_type) + " <id, " + std::to_string(_result_identifier) + ">";
	for (OperandNodeValueVariant value_variant : _source_value_variants) {
		result += ' ';
		if (std::holds_alternative<int>(value_variant)) {
			result += std::format("{}", std::get<int>(value_variant));
		} else if (std::holds_alternative<float>(value_variant)) {
			result += std::format("{}", StringHandler::format_float(std::get<float>(value_variant)));
		} else if (std::holds_alternative<size_t>(value_variant)) {
			result += std::format("<id, {}>", std::get<size_t>(value_variant));
		} else {
			result += '?';
		}
	}
	return result;
}

void CommandLine::serialize(std::ofstream& p_out) const {

	auto type_value = static_cast<std::underlying_type_t<CommandType>>(_type);
	p_out.write(reinterpret_cast<const char*>(&type_value), sizeof(type_value));

	p_out.write(reinterpret_cast<const char*>(&_result_identifier), sizeof(_result_identifier));

	size_t source_size = _source_value_variants.size();
	p_out.write(reinterpret_cast<const char*>(&source_size), sizeof(source_size));
	
	for (const OperandNodeValueVariant& source : _source_value_variants) {
		size_t source_index = source.index();
		p_out.write(reinterpret_cast<const char*>(&source_index), sizeof(source_index));
		
		if (source_index == 0) { // size_t
			size_t value = std::get<size_t>(source);
			p_out.write(reinterpret_cast<const char*>(&value), sizeof(value));
		} else if (source_index == 1) { // int
			int value = std::get<int>(source);
			p_out.write(reinterpret_cast<const char*>(&value), sizeof(value));
		} else if (source_index == 2) { // float
			float value = std::get<float>(source);
			p_out.write(reinterpret_cast<const char*>(&value), sizeof(value));
		}
	}
}

std::vector<std::string> to_string(const std::vector<CommandLine>& p_command_lines) {
	std::vector<std::string> result;
	for (CommandLine command_line : p_command_lines) {
		result.push_back(command_line.to_string());
	}
	return result;
}
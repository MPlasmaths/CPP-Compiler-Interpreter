#pragma once

#include "explicit_variable_type.hpp"

#include <variant>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using VariableObjectValueVariant = std::variant<int, float>;

class VariableObject {
public:

	VariableObject(const size_t p_identifier, const std::string& p_name, const VariableObjectValueVariant& p_value_variant)
		: _identifier(p_identifier), _name(p_name), _value_variant(p_value_variant), _has_undefined_value_type(false) {}
	VariableObject(const size_t p_identifier, const std::string& p_name)
		: _identifier(p_identifier), _name(p_name), _value_variant(int()), _has_undefined_value_type(true) {}

	size_t get_identifier() const {
		return _identifier;
	}
	std::string get_name() const {
		return _name;
	}
	VariableObjectValueVariant get_value_variant() const {
		return _value_variant;
	}
	bool has_undefined_value_type() const {
		return _has_undefined_value_type;
	}

	void set_identifier(size_t p_identifier) {
		_identifier = p_identifier;
	}
	void set_name(const std::string& p_name) {
		_name = p_name;
	}
	void set_value_variant(const VariableObjectValueVariant p_value_variant) {
		_value_variant = p_value_variant;
	}
	bool update_value_variant(const VariableObjectValueVariant& p_value_variant);

	std::string to_string() const;

	void serialize(std::ofstream& p_out) const;
private:

	size_t _identifier;
	std::string _name;
	VariableObjectValueVariant _value_variant;
	bool _has_undefined_value_type;
};

std::string describe(const VariableObject& p_variable_object);
std::string describe(const VariableObjectValueVariant& p_value_variant);
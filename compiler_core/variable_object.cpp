#include "variable_object.hpp"
#include "string_handler.hpp"

#include <stdexcept>

bool VariableObject::update_value_variant(const VariableObjectValueVariant& p_value_variant) {
	if (_has_undefined_value_type) {
		_value_variant = p_value_variant;
		_has_undefined_value_type = false;
		return true;
	}

	if (std::holds_alternative<int>(_value_variant) && std::holds_alternative<int>(p_value_variant)) {
		_value_variant = p_value_variant;
		return true;
	}

	if (std::holds_alternative<float>(_value_variant) && std::holds_alternative<float>(p_value_variant)) {
		_value_variant = p_value_variant;
		return true;
	}

	return false;
}

std::string VariableObject::to_string() const {
	std::string result = "<id, " + std::to_string(_identifier) + "> " + _name + '[';
	if (std::holds_alternative<int>(_value_variant)) {
		result += 'i';
	} else {
		result += 'f';
	}
	result += ']';
	return result;
}

void VariableObject::serialize(std::ofstream& p_out) const {
	p_out.write(reinterpret_cast<const char*>(&_identifier), sizeof(_identifier));
	
	size_t name_length = _name.size();
	p_out.write(reinterpret_cast<const char*>(&name_length), sizeof(name_length));
	p_out.write(_name.data(), name_length);

	size_t value_index = _value_variant.index();
	p_out.write(reinterpret_cast<const char*>(&value_index), sizeof(value_index));
	if (value_index == 0) { // int
		int value = std::get<int>(_value_variant);
		p_out.write(reinterpret_cast<const char*>(&value), sizeof(value));
	} else if (value_index == 1) { // float
		float value = std::get<float>(_value_variant);
		p_out.write(reinterpret_cast<const char*>(&value), sizeof(value));
	}
}

std::string describe(const VariableObject& p_variable_object) {
	std::string result = "";
	const VariableObjectValueVariant& value_variant = p_variable_object.get_value_variant();
	if (!p_variable_object.has_undefined_value_type()) {
		if (std::holds_alternative<int>(value_variant)) {
			result += "integer ";
		} else if (std::holds_alternative<float>(value_variant)) {
			result += "floating point ";
		} else {
			result += "unknown ";
		}
	}
	result += "variable";
	return result;
}
std::string describe(const VariableObjectValueVariant& p_value_variant) {
	if (std::holds_alternative<int>(p_value_variant)) {
		return "int";
	} else if (std::holds_alternative<float>(p_value_variant)) {
		return "float";
	} else {
		return "unknown";
	}
}
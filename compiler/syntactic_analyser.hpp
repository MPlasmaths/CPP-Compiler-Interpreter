#pragma once

#include "token_container.hpp"
#include "error_logger.hpp"

#include <format>
#include <vector>
#include <string>

class SyntacticAnalyser {
public:
	static bool analyse(const TokenContainer& p_token_container) {
		const std::vector<Token>& token_list = p_token_container.get_token_list();

		bool is_valid = true;

		bool has_operator_unbalance = false;
		bool has_round_bracket_unbalance = false;
		bool has_square_bracket_unbalance = false;

		bool is_expected_operator = false;
		Token last_operator_or_operand_token = token_list[0];

		int round_bracket_balance = 0;
		size_t first_round_unclosed_bracket_char_position;

		bool is_expected_opening_square_bracket = false;
		bool is_expected_explicit_variable_type = false;
		bool is_expected_closing_square_bracket = false;

		for (size_t i = 0; i < token_list.size(); i++) {
			const Token& token = token_list[i];
			const TokenValueVariant& token_value_variant = token.get_value_variant();

			if (!has_square_bracket_unbalance) {
				if (!std::holds_alternative<SquareBracketType>(token_value_variant) &&
					is_expected_closing_square_bracket) {
					is_expected_closing_square_bracket = false;
					has_square_bracket_unbalance = true;
					ErrorLogger::log(std::format("{} was encountered at position {}, but {} was expected",
						describe(token), token.get_char_position(),
						describe(SquareBracketType::CLOSING)), ErrorType::SYNTACTIC);
					is_valid = false;
				}
				if (!std::holds_alternative<ExplicitVariableType>(token_value_variant) &&
					is_expected_explicit_variable_type) {
					is_expected_explicit_variable_type = false;
					has_square_bracket_unbalance = true;
					ErrorLogger::log(std::format(
						"{} was encountered at position {}, but explicit variable type was expected",
						describe(token), token.get_char_position()), ErrorType::SYNTACTIC);
					is_valid = false;
				}
			}
			
			if (std::holds_alternative<RoundBracketType>(token_value_variant)) {
				is_expected_opening_square_bracket = false;
				const RoundBracketType round_bracket_type = std::get<RoundBracketType>(token_value_variant);
				if (!has_round_bracket_unbalance) {
					if (round_bracket_type == RoundBracketType::OPENING) {
						if (round_bracket_balance == 0) {
							first_round_unclosed_bracket_char_position = token.get_char_position();
						}
						round_bracket_balance++;
					} else {
						round_bracket_balance--;
					}
					if (round_bracket_balance == -1) {
						has_round_bracket_unbalance = true;
						ErrorLogger::log(std::format("{} at position {} has not {}",
							describe(token), token.get_char_position(),
							describe(RoundBracketType::OPENING)), ErrorType::SYNTACTIC);
						is_valid = false;
					}
				}
				if (!has_operator_unbalance) {
					if (is_expected_operator && round_bracket_type == RoundBracketType::OPENING) {
						has_operator_unbalance = true;
						ErrorLogger::log(std::format("{} at position {} has not operator",
							describe(last_operator_or_operand_token),
							last_operator_or_operand_token.get_char_position()), ErrorType::SYNTACTIC);
						is_valid = false;
					}
					if (!is_expected_operator && round_bracket_type == RoundBracketType::CLOSING) {
						has_operator_unbalance = true;
						ErrorLogger::log(std::format("{} at position {} has not operand",
							describe(last_operator_or_operand_token),
							last_operator_or_operand_token.get_char_position()), ErrorType::SYNTACTIC);
						is_valid = false;
					}
				}
			} else if (std::holds_alternative<SquareBracketType>(token_value_variant)) {
				if (!has_square_bracket_unbalance) {
					const SquareBracketType square_bracket_type = std::get<SquareBracketType>(token_value_variant);
					if (is_expected_opening_square_bracket) {
						if (square_bracket_type == SquareBracketType::OPENING) {
							is_expected_opening_square_bracket = false;
							is_expected_explicit_variable_type = true;
						} else {
							has_square_bracket_unbalance = true;
							ErrorLogger::log(std::format("{} was encountered at position {}, but {} was expected",
								describe(token), token.get_char_position(),
								describe(SquareBracketType::OPENING)), ErrorType::SYNTACTIC);
							is_valid = false;
						}
					} else if (is_expected_closing_square_bracket) {
						if (square_bracket_type == SquareBracketType::CLOSING) {
							is_expected_closing_square_bracket = false;
						} else {
							has_square_bracket_unbalance = true;
							ErrorLogger::log(std::format("{} was encountered at position {}, but {} was expected",
								describe(token), token.get_char_position(),
								describe(SquareBracketType::CLOSING)), ErrorType::SYNTACTIC);
							is_valid = false;
						}
					} else {
						has_square_bracket_unbalance = true;
						ErrorLogger::log(std::format("{} at position {} was unexpected",
							describe(token), token.get_char_position()), ErrorType::SYNTACTIC);
						is_valid = false;
					}
				}
			} else if (std::holds_alternative<ExplicitVariableType>(token_value_variant)) {
				if (!has_square_bracket_unbalance) {
					if (is_expected_explicit_variable_type) {
						is_expected_explicit_variable_type = false;
						is_expected_closing_square_bracket = true;
					} else {
						has_square_bracket_unbalance = true;
						ErrorLogger::log(std::format(
							"{} was encountered at position {}, but explicit variable type was expected",
							describe(token), token.get_char_position()), ErrorType::SYNTACTIC);
						is_valid = false;
					}
				}
			} else if (std::holds_alternative<OperatorType>(token_value_variant)) {
				if (!has_operator_unbalance) {
					if (!is_expected_operator) {
						has_operator_unbalance = true;
						ErrorLogger::log(std::format("{} at position {} has not operand",
							describe(last_operator_or_operand_token),
							last_operator_or_operand_token.get_char_position()), ErrorType::SYNTACTIC);
						is_valid = false;
					} else {
						last_operator_or_operand_token = token;
						is_expected_operator = false;
					}
				}
			} else if (std::holds_alternative<int>(token_value_variant) || std::holds_alternative<float>(token_value_variant)) {
				if (!has_operator_unbalance) {
					if (is_expected_operator) {
						has_operator_unbalance = true;
						ErrorLogger::log(std::format("{} at position {} has not operator",
							describe(last_operator_or_operand_token),
							last_operator_or_operand_token.get_char_position()), ErrorType::SYNTACTIC);
						is_valid = false;
					} else {
						last_operator_or_operand_token = token;
						is_expected_operator = true;
					}
				}
			} else if (std::holds_alternative<size_t>(token_value_variant)) {
				if (!has_operator_unbalance) {
					if (is_expected_operator) {
						has_operator_unbalance = true;
						ErrorLogger::log(std::format("{} at position {} has not operator",
							describe(last_operator_or_operand_token),
							last_operator_or_operand_token.get_char_position()), ErrorType::SYNTACTIC);
						is_valid = false;
					} else {
						last_operator_or_operand_token = token;
						is_expected_operator = true;
					}
				}
				is_expected_opening_square_bracket = true;
			}
		}
		if (round_bracket_balance > 0 && !has_round_bracket_unbalance) {
			has_round_bracket_unbalance = true;
			ErrorLogger::log(std::format("{} <{}> at position {} has not closing bracket",
				describe(RoundBracketType::OPENING), to_char(RoundBracketType::OPENING),
				first_round_unclosed_bracket_char_position), ErrorType::SYNTACTIC);
			is_valid = false;
		}
		if (!is_expected_operator && !has_operator_unbalance) {
			has_operator_unbalance = true;
			ErrorLogger::log("operand was expected at end of expression", ErrorType::SYNTACTIC);
			is_valid = false;
		}
		if (!has_square_bracket_unbalance) {
			if (is_expected_explicit_variable_type) {
				has_square_bracket_unbalance = true;
				ErrorLogger::log(std::format("{} was not expected at end of expression",
					describe(SquareBracketType::OPENING)), ErrorType::SYNTACTIC);
				is_valid = false;
			} else if (is_expected_closing_square_bracket) {
				has_square_bracket_unbalance = true;
				ErrorLogger::log(std::format("{} was expected at end of expression",
					describe(SquareBracketType::CLOSING)), ErrorType::SYNTACTIC);
				is_valid = false;
			}
		}
		return is_valid;
	}
};

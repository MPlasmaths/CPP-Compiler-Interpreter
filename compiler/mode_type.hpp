#pragma once

#include <string>
#include <algorithm>
#include <cctype>
#include <stdexcept>

enum class ModeType {
	LEXICAL_ANALYSIS, SYNTACTIC_ANALYSIS, SEMANTIC_ANALYSIS,
	PORTABLE_CODE_GENERATION, POSTFIX_NOTATION_GENERATION,
	POST_CODE_GENERATION,
	OPTIMIZATION, UNKNOWN
};

inline ModeType to_mode_type(const std::string& p_string) {
	std::string mode = p_string;
	std::transform(mode.begin(), mode.end(), mode.begin(),
		[](unsigned char c) { return std::tolower(c); });
	if (mode == "lex") {
		return ModeType::LEXICAL_ANALYSIS;
	} else if (mode == "syn") {
		return ModeType::SYNTACTIC_ANALYSIS;
	} else if (mode == "sem") {
		return ModeType::SEMANTIC_ANALYSIS;
	} else if (mode == "gen1") {
		return ModeType::PORTABLE_CODE_GENERATION;
	} else if (mode == "gen2") {
		return ModeType::POSTFIX_NOTATION_GENERATION;
	} else if (mode == "gen3") {
		return ModeType::POST_CODE_GENERATION;
	} else if (mode == "opt") {
		return ModeType::OPTIMIZATION;
	} else {
		return ModeType::UNKNOWN;
	}
};

inline std::string describe(const ModeType p_mode_type) {
	switch (p_mode_type) {
	case ModeType::LEXICAL_ANALYSIS:
		return "lexical analysis";
	case ModeType::SYNTACTIC_ANALYSIS:
		return "syntactic analysis";
	case ModeType::SEMANTIC_ANALYSIS:
		return "semantic analysis";
	case ModeType::PORTABLE_CODE_GENERATION:
		return "portable code generation";
	case ModeType::POSTFIX_NOTATION_GENERATION:
		return "postfix notation generation";
	case ModeType::POST_CODE_GENERATION:
		return "post code generation";
	case ModeType::OPTIMIZATION:
		return "optimization";
	default:
		return "unknown";
	}
}
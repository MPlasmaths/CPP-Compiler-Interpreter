#include "mode_handler.hpp"
#include "file_handler.hpp"
#include "error_logger.hpp"
#include "expression_handler.hpp"
#include "lexical_analyser.hpp"
#include "syntactic_analyser.hpp"
#include "syntax_tree_handler.hpp"
#include "semantic_analyser.hpp"
#include "portable_code_generator.hpp"
#include "postfix_notation_generator.hpp"
#include "code_optimizer.hpp"
#include "serializer.hpp"

#include <vector>
#include <iostream>
#include <stdexcept>

bool ModeHandler::_optimization_enabled = false;

bool ModeHandler::check_arguments_quantity(const int p_arguments_quantity, const ModeType p_mode_type) {
	switch (p_mode_type) {
	case ModeType::LEXICAL_ANALYSIS:
		return p_arguments_quantity == 5;
	case ModeType::SYNTACTIC_ANALYSIS:
		return p_arguments_quantity == 4;
	case ModeType::SEMANTIC_ANALYSIS:
		return p_arguments_quantity == 4;
	case ModeType::PORTABLE_CODE_GENERATION:
		return p_arguments_quantity == 5 + _optimization_enabled;
	case ModeType::POSTFIX_NOTATION_GENERATION:
		return p_arguments_quantity == 5 + _optimization_enabled;
	case ModeType::POST_CODE_GENERATION:
		return p_arguments_quantity == 4;
	default:
		return false;
	}
}

void ModeHandler::lexical_analysis(const std::string& p_input_expression_file_name,
	const std::string& p_output_tokens_file_name, const std::string& p_output_symbols_file_name) {
	std::string expression = FileHandler::read_lines(p_input_expression_file_name)[0];
	std::vector<std::string> lexemes = ExpressionHandler::lexemize(expression);
	if (!LexicalAnalyser::analyse(lexemes)) {
		std::cout << "Lexical analysis completed with errors.\n";
		ErrorLogger::print();
		return;
	} else {
		std::cout << "Lexical analysis completed successfully.\n";
		TokenContainer token_container = ExpressionHandler::tokenize(lexemes);
		FileHandler::write_lines(p_output_tokens_file_name, token_container.describe_token_list());
		std::cout << "Tokens file created.\n";
		FileHandler::write_lines(p_output_symbols_file_name, token_container.describe_symbol_table());
		std::cout << "Symbols file created.\n";
	}
}

void ModeHandler::syntactic_analysis(const std::string& p_input_expression_file_name,
	const std::string& p_output_syntax_tree_file_name) {
	std::string expression = FileHandler::read_lines(p_input_expression_file_name)[0];
	std::vector<std::string> lexemes = ExpressionHandler::lexemize(expression);
	
	if (!LexicalAnalyser::analyse(lexemes)) {
		std::cout << "Lexical analysis completed with errors.\n";
		ErrorLogger::print();
		return;
	}
	std::cout << "Lexical analysis completed successfully.\n";
	TokenContainer token_container = ExpressionHandler::tokenize(lexemes);
	if (!SyntacticAnalyser::analyse(token_container)) {
		std::cout << "Syntactic analysis completed with errors.\n";
		ErrorLogger::print();
		return;
	}
	std::cout << "Syntactic analysis completed successfully.\n";
	std::unique_ptr<SyntaxTreeNode> syntax_tree_root = SyntaxTreeHandler::build(token_container);
	FileHandler::write_lines(p_output_syntax_tree_file_name, SyntaxTreeHandler::compose(syntax_tree_root));
	std::cout << "Syntax tree built successfully.\n";
}

void ModeHandler::semantic_analysis(const std::string& p_input_expression_file_name,
	const std::string& p_output_modified_syntax_tree_file_name) {
	std::string expression = FileHandler::read_lines(p_input_expression_file_name)[0];
	std::vector<std::string> lexemes = ExpressionHandler::lexemize(expression);

	if (!LexicalAnalyser::analyse(lexemes)) {
		std::cout << "Lexical analysis completed with errors.\n";
		ErrorLogger::print();
		return;
	}
	std::cout << "Lexical analysis completed successfully.\n";
	TokenContainer token_container = ExpressionHandler::tokenize(lexemes);
	if (!SyntacticAnalyser::analyse(token_container)) {
		std::cout << "Syntactic analysis completed with errors.\n";
		ErrorLogger::print();
		return;
	}
	std::cout << "Syntactic analysis completed successfully.\n";
	std::unique_ptr<SyntaxTreeNode> syntax_tree_root = SyntaxTreeHandler::build(token_container);
	if (!SemanticAnalyser::analyse(token_container)) {
		std::cout << "Semantic analysis completed with errors.\n";
		ErrorLogger::print();
		return;
	}
	std::cout << "Semantic analysis completed successfully.\n";
	std::unique_ptr<SyntaxTreeNode> modified_syntax_tree_root = SyntaxTreeHandler::modify(syntax_tree_root, token_container);
	FileHandler::write_lines(p_output_modified_syntax_tree_file_name, SyntaxTreeHandler::compose(modified_syntax_tree_root));
	std::cout << "Modified syntax tree built successfully.\n";
}

void ModeHandler::portable_code_generation(const std::string& p_input_expression_file_name,
	const std::string& p_output_portable_code_file_name, const std::string& p_output_symbols_file_name) {
	std::string expression = FileHandler::read_lines(p_input_expression_file_name)[0];
	std::vector<std::string> lexemes = ExpressionHandler::lexemize(expression);

	if (!LexicalAnalyser::analyse(lexemes)) {
		std::cout << "Lexical analysis completed with errors.\n";
		ErrorLogger::print();
		return;
	}
	std::cout << "Lexical analysis completed successfully.\n";
	TokenContainer token_container = ExpressionHandler::tokenize(lexemes);
	if (!SyntacticAnalyser::analyse(token_container)) {
		std::cout << "Syntactic analysis completed with errors.\n";
		ErrorLogger::print();
		return;
	}
	std::cout << "Syntactic analysis completed successfully.\n";
	std::unique_ptr<SyntaxTreeNode> syntax_tree_root = SyntaxTreeHandler::build(token_container);
	if (!SemanticAnalyser::analyse(token_container)) {
		std::cout << "Semantic analysis completed with errors.\n";
		ErrorLogger::print();
		return;
	}
	std::cout << "Semantic analysis completed successfully.\n";
	std::unique_ptr<SyntaxTreeNode> modified_syntax_tree_root = SyntaxTreeHandler::modify(syntax_tree_root, token_container);
	if (_optimization_enabled) {
		CodeOptimizer::optimize(modified_syntax_tree_root, token_container);
		std::cout << "Code optimized.\n";
	}
	FileHandler::write_lines(p_output_portable_code_file_name,
		::to_string(PortableCodeGenerator::generate(modified_syntax_tree_root, token_container, _optimization_enabled)));
	std::cout << "Portable code generated successfully.\n";
	FileHandler::write_lines(p_output_symbols_file_name, token_container.describe_symbol_table());
	std::cout << "Symbols file created.\n";
	FileHandler::write_lines("output_optimized_tree.txt", SyntaxTreeHandler::compose(modified_syntax_tree_root));
}
void ModeHandler::postfix_notation_generation(const std::string& p_input_expression_file_name,
	const std::string& p_output_postfix_notation_file_name, const std::string& p_output_symbols_file_name) {
	std::string expression = FileHandler::read_lines(p_input_expression_file_name)[0];
	std::vector<std::string> lexemes = ExpressionHandler::lexemize(expression);

	if (!LexicalAnalyser::analyse(lexemes)) {
		std::cout << "Lexical analysis completed with errors.\n";
		ErrorLogger::print();
		return;
	}
	std::cout << "Lexical analysis completed successfully.\n";
	TokenContainer token_container = ExpressionHandler::tokenize(lexemes);
	if (!SyntacticAnalyser::analyse(token_container)) {
		std::cout << "Syntactic analysis completed with errors.\n";
		ErrorLogger::print();
		return;
	}
	std::cout << "Syntactic analysis completed successfully.\n";
	std::unique_ptr<SyntaxTreeNode> syntax_tree_root = SyntaxTreeHandler::build(token_container);
	if (!SemanticAnalyser::analyse(token_container)) {
		std::cout << "Semantic analysis completed with errors.\n";
		ErrorLogger::print();
		return;
	}
	std::cout << "Semantic analysis completed successfully.\n";
	std::unique_ptr<SyntaxTreeNode> modified_syntax_tree_root = SyntaxTreeHandler::modify(syntax_tree_root, token_container);
	if (_optimization_enabled) {
		CodeOptimizer::optimize(modified_syntax_tree_root, token_container);
		std::cout << "Code optimized.\n";
	}
	FileHandler::write_line(p_output_postfix_notation_file_name,
		::to_string(PostfixNotationGenerator::generate(modified_syntax_tree_root)));
	std::cout << "Postfix notation generated successfully.\n";
	FileHandler::write_lines(p_output_symbols_file_name, token_container.describe_symbol_table());
	std::cout << "Symbols file created.\n";
}

void ModeHandler::post_code_generation(const std::string& p_input_expression_file_name,
	const std::string& p_output_post_code_file_name) {
	std::string expression = FileHandler::read_lines(p_input_expression_file_name)[0];
	std::vector<std::string> lexemes = ExpressionHandler::lexemize(expression);

	if (!LexicalAnalyser::analyse(lexemes)) {
		std::cout << "Lexical analysis completed with errors.\n";
		ErrorLogger::print();
		return;
	}
	std::cout << "Lexical analysis completed successfully.\n";
	TokenContainer token_container = ExpressionHandler::tokenize(lexemes);
	if (!SyntacticAnalyser::analyse(token_container)) {
		std::cout << "Syntactic analysis completed with errors.\n";
		ErrorLogger::print();
		return;
	}
	std::cout << "Syntactic analysis completed successfully.\n";
	std::unique_ptr<SyntaxTreeNode> syntax_tree_root = SyntaxTreeHandler::build(token_container);
	if (!SemanticAnalyser::analyse(token_container)) {
		std::cout << "Semantic analysis completed with errors.\n";
		ErrorLogger::print();
		return;
	}
	std::cout << "Semantic analysis completed successfully.\n";
	std::unique_ptr<SyntaxTreeNode> modified_syntax_tree_root = SyntaxTreeHandler::modify(syntax_tree_root, token_container);
	CodeOptimizer::optimize(modified_syntax_tree_root, token_container);
	std::vector<CommandLine> portable_code = PortableCodeGenerator::generate(modified_syntax_tree_root, token_container, true);
	Serializer::write_binary_file(p_output_post_code_file_name,
		token_container.get_symbol_table(), portable_code);
	std::cout << "Post code generated successfully.\n";
}
#include <iostream>
#include <string>
#include <vector>

#include "Tlang/core.hpp"

int main(int argc, char** argv) {

	std::string src = "const i: boolean = true && false || !false i = false";
	// std::string src = "3 + 100 * .2 / 1 - 2";
	// std::string src = "(3 + 100 * .2) / (1 - 2)";

	// * LEXER

	langgen::lexer::Lexer lexer;

	setup_lexer(lexer);

	std::vector<langgen::lexer::Token> tokens;

	std::cout << src << std::endl;

	langgen::lexer::lex(
		lexer,
		tokens,
		src
	);

	for(const langgen::lexer::Token& token : tokens) {
		std::cout << repr(token) << "\n\n";
	}

	// * PARSER

	langgen::parser::Parser parser;

	setup_parser(parser);

	std::shared_ptr<langgen::ast::Scope> program = std::make_shared<langgen::ast::Scope>();

	langgen::parser::parse(
		parser,
		program,
		tokens
	);

	std::cout << program->repr() << std::endl;

	// * INTERPRETER

	langgen::interpreter::Interpreter interpreter;

	setup_interpreter(interpreter);

	std::unordered_map<std::string, std::string> results;

	langgen::interpreter::interpret(
		interpreter,
		results,
		program
	);

	for(const auto& [node, result] : results) {
		std::cout << node << " -> " << result << std::endl;
	}

	return 0;
}
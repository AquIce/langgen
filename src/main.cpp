#include <iostream>
#include <string>

#include <AquIce/langgen/lexer.hpp>
#include <AquIce/langgen/parser.hpp>

#pragma region Prototypes

void setup_lexer(langgen::lexer::Lexer& lexer);

std::shared_ptr<langgen::ast::Expression> parse_multiplicative_expression(std::vector<langgen::lexer::Token>& tokens);
std::shared_ptr<langgen::ast::Expression> parse_additive_expression(std::vector<langgen::lexer::Token>& tokens);
std::shared_ptr<langgen::ast::Expression> parse_binary_expression(std::vector<langgen::lexer::Token>& tokens);

void setup_parser(langgen::parser::Parser& parser);

#pragma endregion

#pragma region Classes

class BinaryExpression : public langgen::ast::Expression {
public:
	BinaryExpression(std::shared_ptr<langgen::ast::Expression> left, std::string operator_symbol, std::shared_ptr<langgen::ast::Expression> right) {
		this->left = left;
		this->operator_symbol = operator_symbol;
		this->right = right;
	}
	
	virtual std::string type() override {
		return "BinaryExpression";
	}

	virtual std::string repr(int indent = 0) {
		return
			std::string(indent, '\t') + "(\n" +
			this->left->repr(indent + 1) + "\n" +
			std::string(indent + 1, '\t') + this->operator_symbol + "\n" +
			this->right->repr(indent + 1) + "\n" +
			std::string(indent, '\t') + ")";
	}

private:
	std::shared_ptr<langgen::ast::Expression> left;
	std::string operator_symbol;
	std::shared_ptr<langgen::ast::Expression> right;
};

#pragma endregion

int main(int argc, char** argv) {

	std::string src = "3 + 100 * .2 / 1 - 2";
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

	return 0;
}

#pragma region Lexer Implementation

void setup_lexer(langgen::lexer::Lexer& lexer) {
	std::vector<langgen::lexer::TokenType> tokenTypes = std::vector<langgen::lexer::TokenType>({
		langgen::lexer::make_token_type("("),
		langgen::lexer::make_token_type(")"),
		langgen::lexer::make_token_type("OPERATOR", "+"),
		langgen::lexer::make_token_type("OPERATOR", "-"),
		langgen::lexer::make_token_type("OPERATOR", "*"),
		langgen::lexer::make_token_type("OPERATOR", "/"),
		langgen::lexer::make_token_type(
			"NUMBER",
			[lexer](std::string src) -> std::string {
				std::string number = "";
				size_t i = 0;
				bool isInteger = true;
				while(isdigit(src.at(i)) || src.at(i) == lexer.decimalSeparator) {
					if(src.at(i) == lexer.decimalSeparator) {
						if(!isInteger) {
							throw std::runtime_error("Invalid number format");
						}
						isInteger = false;
					}
					number += src.at(i);
					if(++i == src.length()) {
						break;
					}
				}
				return number;
			}
		)
	});

	langgen::lexer::setup_lexer(
		lexer,
		tokenTypes
	);
}

#pragma endregion

#pragma region Parser Implementation

std::shared_ptr<langgen::ast::Expression> parse_multiplicative_expression(std::vector<langgen::lexer::Token>& tokens) {
	std::shared_ptr<langgen::ast::Expression> left = langgen::parser::parse_number_expression(tokens);

	if(left == nullptr) {
		return nullptr;
	}

	if(
		peek(tokens).type == "OPERATOR" &&
		(peek(tokens).value == "*" || peek(tokens).value == "/")
	) {
		std::string operator_symbol = eat(tokens).value;
		std::shared_ptr<langgen::ast::Expression> right = parse_multiplicative_expression(tokens);
		
		return std::make_shared<BinaryExpression>(
			left,
			operator_symbol,
			right
		);
	}
	
	return left;
}

std::shared_ptr<langgen::ast::Expression> parse_additive_expression(std::vector<langgen::lexer::Token>& tokens) {
	std::shared_ptr<langgen::ast::Expression> left = parse_multiplicative_expression(tokens);

	if(left == nullptr) {
		return nullptr;
	}

	if(
		peek(tokens).type == "OPERATOR" &&
		(peek(tokens).value == "+" || peek(tokens).value == "-")
	) {
		std::string operator_symbol = eat(tokens).value;
		std::shared_ptr<langgen::ast::Expression> right = parse_additive_expression(tokens);
		
		return std::make_shared<BinaryExpression>(
			left,
			operator_symbol,
			right
		);
	}
	
	return left;
}

std::shared_ptr<langgen::ast::Expression> parse_binary_expression(std::vector<langgen::lexer::Token>& tokens) {
	return parse_additive_expression(tokens);
}

void setup_parser(langgen::parser::Parser& parser) {

	langgen::parser::setup_parser(
		parser,
		{
			{
				"BinaryExpression",
				langgen::parser::make_node(&parse_binary_expression)
			}
		}
	);

	langgen::parser::demoteTopNode(parser, "NumberExpression");
}

#pragma endregion
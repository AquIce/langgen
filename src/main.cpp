#include <iostream>
#include <string>
#include <vector>

#include <AquIce/langgen/lexer.hpp>
#include <AquIce/langgen/parser.hpp>
#include <AquIce/langgen/interpreter.hpp>

#pragma region Prototypes

void setup_lexer(langgen::lexer::Lexer& lexer);

std::shared_ptr<langgen::ast::Expression> parse_multiplicative_expression(std::vector<langgen::lexer::Token>& tokens);
std::shared_ptr<langgen::ast::Expression> parse_additive_expression(std::vector<langgen::lexer::Token>& tokens);
std::shared_ptr<langgen::ast::Expression> parse_binary_expression(std::vector<langgen::lexer::Token>& tokens);

void setup_parser(langgen::parser::Parser& parser);

void setup_interpreter(langgen::interpreter::Interpreter& interpreter);

#pragma endregion

#pragma region Classes

class BinaryExpression : public langgen::ast::Expression {
public:
	BinaryExpression(std::shared_ptr<langgen::ast::Expression> left, std::string operator_symbol, std::shared_ptr<langgen::ast::Expression> right) {
		this->left = left;
		this->operator_symbol = operator_symbol;
		this->right = right;
	}

	std::shared_ptr<langgen::ast::Expression> get_left() {
		return this->left;
	}
	std::string get_operator_symbol() {
		return this->operator_symbol;
	}
	std::shared_ptr<langgen::ast::Expression> get_right() {
		return this->right;
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

#pragma region Interpreter Implementation

bool mutabilityValidationFunction(langgen::env::EnvValue value) {
	try {
		return value.properties.at("isMutable") == "true";
	} catch(const std::exception& e) {
		return false;
	}
}

std::shared_ptr<langgen::values::RuntimeValue> evaluateBinaryExpression (
	langgen::interpreter::Interpreter& interpreter,
	std::shared_ptr<langgen::ast::Statement> statement,
	std::shared_ptr<langgen::env::Environment> env
) {
	std::shared_ptr<BinaryExpression> binaryExpression = std::dynamic_pointer_cast<BinaryExpression>(statement);

	std::shared_ptr<langgen::values::RuntimeValue> left = langgen::interpreter::evaluateStatement(interpreter, binaryExpression->get_left(), env);
	std::shared_ptr<langgen::values::RuntimeValue> right = langgen::interpreter::evaluateStatement(interpreter, binaryExpression->get_right(), env);

	if(
		left->type() != "NumberValue" ||
		right->type() != "NumberValue"
	) {
		throw std::runtime_error("Trying to add non-number operands");
	}

	double left_nb = std::dynamic_pointer_cast<langgen::values::NumberValue>(left)->get();
	double right_nb = std::dynamic_pointer_cast<langgen::values::NumberValue>(right)->get();

	std::string operator_symbol = binaryExpression->get_operator_symbol();

	if(operator_symbol == "+") {
		return std::make_shared<langgen::values::NumberValue>(left_nb + right_nb);
	}
	if(operator_symbol == "-") {
		return std::make_shared<langgen::values::NumberValue>(left_nb - right_nb);
	}
	if(operator_symbol == "*") {
		return std::make_shared<langgen::values::NumberValue>(left_nb * right_nb);
	}
	if(operator_symbol == "/") {
		return std::make_shared<langgen::values::NumberValue>(left_nb / right_nb);
	}

	throw std::runtime_error("Unknown operator " + operator_symbol);
};

void setup_interpreter(langgen::interpreter::Interpreter& interpreter) {

	std::vector<std::string> valueTypes = std::vector<std::string>();

	std::vector<std::string> envValuesProperties = std::vector<std::string>({
		"isMutable"
	});

	langgen::env::EnvValidationRule mutabilityValidation = {
		&mutabilityValidationFunction,
		std::vector<langgen::env::ValidationRuleSensitivity>({
			langgen::env::ValidationRuleSensitivity::SET
		})
	};

	std::vector<langgen::env::EnvValidationRule> validationRules = std::vector<langgen::env::EnvValidationRule>({
		mutabilityValidation
	});

	std::unordered_map<std::string, langgen::interpreter::ParseStatementFunction> nodeEvaluationFunctions = std::unordered_map<std::string, langgen::interpreter::ParseStatementFunction>({
		{ "BinaryExpression", &evaluateBinaryExpression }
	});

	langgen::interpreter::setup_interpreter(
		interpreter,
		valueTypes,
		envValuesProperties,
		validationRules,
		nodeEvaluationFunctions
	);

}

#pragma endregion
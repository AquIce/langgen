
#include "parser.hpp"

void setup_parser(langgen::parser::Parser& parser) {

	langgen::parser::setup_parser(
		parser,
		{
			{
				"BooleanExpression",
				langgen::parser::make_node(&langgen::parser::parse_boolean_expression, false)
			},
			{
				"UnaryExpression",
				langgen::parser::make_node(&langgen::parser::parse_unary_expression, false)
			},
			{
				"BinaryExpression",
				langgen::parser::make_node(&langgen::parser::parse_binary_expression)
			},
		}
	);

	langgen::parser::demoteTopNode(parser, "NumberExpression");
}

std::shared_ptr<langgen::ast::Expression> langgen::parser::parse_boolean_expression(std::vector<langgen::lexer::Token>& tokens) {
	if(peek(tokens).type == "BOOL") {
		return std::make_shared<langgen::ast::BooleanExpression>(eat(tokens).value == "true");
	}
	return langgen::parser::parse_number_expression(tokens);
}

std::shared_ptr<langgen::ast::Expression> langgen::parser::parse_unary_expression(std::vector<langgen::lexer::Token>& tokens) {

	std::string operator_symbol = "";

	if(peek(tokens).type == "UNARY_OPERATOR") {
		operator_symbol = eat(tokens).value;
	}
	
	std::shared_ptr<langgen::ast::Expression> term = langgen::parser::parse_boolean_expression(tokens);

	return operator_symbol.size() == 0 ? term : std::make_shared<langgen::ast::UnaryExpression>(term, operator_symbol);
}

std::shared_ptr<langgen::ast::Expression> langgen::parser::parse_multiplicative_expression(std::vector<langgen::lexer::Token>& tokens) {
	std::shared_ptr<langgen::ast::Expression> left = langgen::parser::parse_unary_expression(tokens);

	if(left == nullptr) {
		return nullptr;
	}

	if(
		peek(tokens).type == "OPERATOR" &&
		(peek(tokens).value == "*" || peek(tokens).value == "/")
	) {
		std::string operator_symbol = eat(tokens).value;
		std::shared_ptr<langgen::ast::Expression> right = parse_multiplicative_expression(tokens);
		
		return std::make_shared<langgen::ast::BinaryExpression>(
			left,
			operator_symbol,
			right
		);
	}
	
	return left;
}

std::shared_ptr<langgen::ast::Expression> langgen::parser::parse_additive_expression(std::vector<langgen::lexer::Token>& tokens) {
	std::shared_ptr<langgen::ast::Expression> left = langgen::parser::parse_multiplicative_expression(tokens);

	if(left == nullptr) {
		return nullptr;
	}

	if(
		peek(tokens).type == "OPERATOR" &&
		(peek(tokens).value == "+" || peek(tokens).value == "-")
	) {
		std::string operator_symbol = eat(tokens).value;
		std::shared_ptr<langgen::ast::Expression> right = parse_additive_expression(tokens);
		
		return std::make_shared<langgen::ast::BinaryExpression>(
			left,
			operator_symbol,
			right
		);
	}
	
	return left;
}

std::shared_ptr<langgen::ast::Expression> langgen::parser::parse_logical_expression(std::vector<langgen::lexer::Token>& tokens) {
	std::shared_ptr<langgen::ast::Expression> left = langgen::parser::parse_additive_expression(tokens);

	if(left == nullptr) {
		return nullptr;
	}

	if(
		peek(tokens).type == "OPERATOR" &&
		(peek(tokens).value == "&&" || peek(tokens).value == "||")
	) {
		std::string operator_symbol = eat(tokens).value;
		std::shared_ptr<langgen::ast::Expression> right = parse_logical_expression(tokens);
		
		return std::make_shared<langgen::ast::BinaryExpression>(
			left,
			operator_symbol,
			right
		);
	}
	
	return left;
}

std::shared_ptr<langgen::ast::Expression> langgen::parser::parse_binary_expression(std::vector<langgen::lexer::Token>& tokens) {
	return langgen::parser::parse_logical_expression(tokens);
}
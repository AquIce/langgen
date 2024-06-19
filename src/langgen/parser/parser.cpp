#include <AquIce/langgen/parser.hpp>

[[nodiscard]] langgen::lexer::Token peek(std::vector<langgen::lexer::Token>& tokens) {
	return tokens.front();
}

[[nodiscard]] langgen::lexer::Token eat(std::vector<langgen::lexer::Token>& tokens) {
	langgen::lexer::Token first = peek(tokens);
	tokens.erase(tokens.begin());
	return first;
}

langgen::parser::Node langgen::parser::make_node(
	langgen::parser::ParseNodeFunction parse_node,
	bool isTopNode
) {
	return langgen::parser::Node{
		parse_node,
		isTopNode
	};
}

void langgen::parser::demoteTopNode(
	langgen::parser::Parser& parser,
	std::string key
) {
	parser.nodesRegister.at(key).isTopNode = false;
}

std::shared_ptr<langgen::ast::Expression> langgen::parser::parse_number_expression(std::vector<langgen::lexer::Token>& tokens) {
	if(peek(tokens).type == "NUMBER") {
		return std::make_shared<langgen::ast::NumberExpression>(std::stod(eat(tokens).value));
	}
	return nullptr;
}

void langgen::parser::register_node(
	langgen::parser::Parser& parser,
	std::string key,
	langgen::parser::Node node
) {
	parser.nodesRegister[key] = node;
}

void langgen::parser::setup_parser(
	Parser& parser,
	std::unordered_map<std::string, Node> nodesRegister
) {
	parser.nodesRegister = nodesRegister;
	langgen::parser::register_node(
		parser,
		"NumberExpression",
		langgen::parser::make_node(&parse_number_expression)
	);
}

std::shared_ptr<langgen::ast::Statement> langgen::parser::parse_statement(
	langgen::parser::Parser& parser,
	std::vector<langgen::lexer::Token>& tokens
) {
	std::shared_ptr<langgen::ast::Statement> statement = nullptr;

	for(auto& [key, node] : parser.nodesRegister) {
		if(node.isTopNode) {
			statement = node.parse_node(tokens);
		}
	}

	if(statement == nullptr) {
		throw std::runtime_error("Unknown token : \"" + peek(tokens).type + "\"");
	}

	return statement;
}

void langgen::parser::parse(
	langgen::parser::Parser& parser,
	std::shared_ptr<langgen::ast::Scope> program,
	std::vector<langgen::lexer::Token>& tokens
) {
	while(peek(tokens).type != "EOF") {
		program->body.push_back(
			parse_statement(parser, tokens)
		);
	}
}
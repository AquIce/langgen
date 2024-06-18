#ifndef __LANGGEN_PARSER__
#define __LANGGEN_PARSER__

#include "lexer.hpp"
#include "ast.hpp"

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>

#define generate_parser(size)

[[nodiscard]] langgen::lexer::Token peek(std::vector<langgen::lexer::Token>& tokens);

[[nodiscard]] langgen::lexer::Token eat(std::vector<langgen::lexer::Token>& tokens);

namespace langgen {
	namespace parser {

		typedef std::function<std::shared_ptr<langgen::ast::Statement> (std::vector<langgen::lexer::Token>&)> ParseNodeFunction;

		typedef struct Node {
			ParseNodeFunction parse_node;
			bool isTopNode;
		} Node;

		typedef struct Parser {
			std::unordered_map<std::string, Node> nodesRegister;
		} Parser;

		Node make_node(
			ParseNodeFunction parse_node,
			bool isTopNode = true
		);

		void demoteTopNode(
			Parser& parser,
			std::string key
		);

		std::shared_ptr<langgen::ast::Expression> parse_number_expression(std::vector<langgen::lexer::Token>& tokens);

		void register_node(
			Parser& parser,
			std::string key,
			Node node
		);

		void setup_parser(
			Parser& parser,
			std::unordered_map<std::string, Node> nodesRegister
		);

		std::shared_ptr<langgen::ast::Statement> parse_statement(
			Parser& parser,
			std::vector<langgen::lexer::Token>& tokens
		);

		void parse(
			Parser& parser,
			std::shared_ptr<langgen::ast::Scope> program,
			std::vector<langgen::lexer::Token>& tokens
		);
	}
}

#endif // __LANGGEN_PARSER__
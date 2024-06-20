#ifndef __T_PARSER__
#define __T_PARSER__

#include <AquIce/langgen/parser.hpp>

#include "ast.hpp"

void setup_parser(langgen::parser::Parser& parser);

namespace langgen {
	namespace parser {
		std::shared_ptr<langgen::ast::Expression> parse_boolean_expression(std::vector<langgen::lexer::Token>& tokens);

		std::shared_ptr<langgen::ast::Expression> parse_unary_expression(std::vector<langgen::lexer::Token>& tokens);

		std::shared_ptr<langgen::ast::Expression> parse_multiplicative_expression(std::vector<langgen::lexer::Token>& tokens);

		std::shared_ptr<langgen::ast::Expression> parse_additive_expression(std::vector<langgen::lexer::Token>& tokens);

		std::shared_ptr<langgen::ast::Expression> parse_logical_expression(std::vector<langgen::lexer::Token>& tokens);

		std::shared_ptr<langgen::ast::Expression> parse_binary_expression(std::vector<langgen::lexer::Token>& tokens);
	}
}

#endif // __T_PARSER__
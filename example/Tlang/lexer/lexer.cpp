#include "lexer.hpp"

void setup_lexer(langgen::lexer::Lexer& lexer) {
	std::vector<langgen::lexer::TokenType> tokenTypes = std::vector<langgen::lexer::TokenType>({
		langgen::lexer::make_token_type("ASSIGN_KEYWORD", "let"),
		langgen::lexer::make_token_type("ASSIGN_KEYWORD", "const"),
		langgen::lexer::make_token_type("SEMICOLON", ";"),
		langgen::lexer::make_token_type("COLON", ":"),
		langgen::lexer::make_token_type("ASSIGN", "="),
		langgen::lexer::make_token_type("OPEN_PAREN", "("),
		langgen::lexer::make_token_type("CLOSE_PAREN", ")"),
		langgen::lexer::make_token_type("OPERATOR", "+"),
		langgen::lexer::make_token_type("OPERATOR", "-"),
		langgen::lexer::make_token_type("OPERATOR", "*"),
		langgen::lexer::make_token_type("OPERATOR", "/"),
		langgen::lexer::make_token_type("OPERATOR", "&&"),
		langgen::lexer::make_token_type("OPERATOR", "||"),
		langgen::lexer::make_token_type("UNARY_OPERATOR", "!"),
		langgen::lexer::make_token_type("BOOL", "true"),
		langgen::lexer::make_token_type("BOOL", "false"),
		langgen::lexer::make_token_type("TYPE", "number"),
		langgen::lexer::make_token_type("TYPE", "boolean"),
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
		),
		langgen::lexer::make_token_type(
			"IDENTIFIER",
			[lexer](std::string src) -> std::string {
				std::string identifier = "";
				size_t i = 0;
				while(isalpha(src.at(i)) || src.at(i) == '_') {
					identifier += src.at(i);
					if(++i == src.length()) {
						break;
					}
				}
				return identifier;
			}
		)
	});

	langgen::lexer::setup_lexer(
		lexer,
		tokenTypes
	);
}
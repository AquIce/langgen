#include <AquIce/langgen/lexer.hpp>

[[nodiscard]] char peek(std::string str) {
	return str.at(0);
}

[[nodiscard]] std::string shift(std::string& str) {
	std::string first = std::string(1, peek(str));
	str = str.substr(1, str.length() - 1);
	return first;
}

[[nodiscard]] std::string shift(
	std::string& str,
	int n
) {
	std::string first = str.substr(0, n);
	str = str.substr(n, str.length() - n);
	return first;
}

bool startswith(std::string str, std::string substr) {
	return str.rfind(substr, 0) == 0;
}

std::string langgen::lexer::repr(const langgen::lexer::Token& token) {
	std::string pretty = "Type: " + token.type;
	pretty += "\nValue: " + token.value;
	return pretty;
}

langgen::lexer::TokenType langgen::lexer::make_token_type(std::string name) {
	return langgen::lexer::TokenType{
		name,
		[name](std::string src) -> std::string {
			if(startswith(src, name)) {
				return name;
			}
			return "";
		}
	};
}

langgen::lexer::TokenType langgen::lexer::make_token_type(std::string name, std::string value) {
	return langgen::lexer::TokenType{
		name,
		[value](std::string src) -> std::string {
			if(startswith(src, value)) {
				return value;
			}
			return "";
		}
	};
}

langgen::lexer::TokenType langgen::lexer::make_token_type(std::string name, std::function<std::string(std::string)> lex_token) {
	return langgen::lexer::TokenType{
		name,
		lex_token
	};
}

void langgen::lexer::setup_lexer(
	langgen::lexer::Lexer& lexer,
	const std::vector<langgen::lexer::TokenType>& tokenTypes,
	std::vector<char> whitespaces,
	std::string singleLineComment,
	std::pair<std::string, std::string> multiLineComment,
	char decimalSeparator,
	char charDelimiter,
	char stringDelimiter,
	char escapeCharacter
) {
	lexer.tokenTypes = tokenTypes;
	lexer.whitespaces = whitespaces;
	lexer.singleLineComment = singleLineComment;
	lexer.multiLineComment = multiLineComment;
	lexer.decimalSeparator = decimalSeparator;
	lexer.charDelimiter = charDelimiter;
	lexer.stringDelimiter = stringDelimiter;
	lexer.escapeCharacter = escapeCharacter;
}

void langgen::lexer::lex(
	langgen::lexer::Lexer& lexer,
	std::vector<langgen::lexer::Token>& tokens,
	std::string src
) {
	while(src.length() > 0) {

		// * Check for skippable characters

		while(std::find(lexer.whitespaces.begin(), lexer.whitespaces.end(), peek(src)) != lexer.whitespaces.end()) {
			(void)shift(src);
		}

		// * Check for comments

		// Single line
		if(startswith(src, lexer.singleLineComment)) {
			while(peek(src) != '\n' && src.length() > 0) {
				(void)shift(src);
			}
			continue;
		}

		// Multi line open
		if(startswith(src, lexer.multiLineComment.first)) {
			while(!startswith(src, lexer.multiLineComment.second)) {
				try {
					(void)shift(src);
				} catch(const std::exception& e) {
					throw std::runtime_error("Comment being opened and not closed before EOF");
				}
			}
			(void)shift(src, lexer.multiLineComment.second.length());
			continue;
		}
		if(startswith(src, lexer.multiLineComment.second)) {
			(void)shift(src, lexer.multiLineComment.second.length());
			throw std::runtime_error("Comment being closed without being opened");
		}

		// Lex token

		bool tokenFound = false;

		for(const langgen::lexer::TokenType& tokenType : lexer.tokenTypes) {
			std::string tokenValue = tokenType.lex_token(src);
			if(tokenValue.length() != 0) {
				(void)shift(src, tokenValue.length());
				tokens.push_back(
					langgen::lexer::Token{
						tokenType.name,
						tokenValue
					}
				);
				tokenFound = true;
				break;
			}
		}

		if(tokenFound) {
			continue;
		}

		throw std::runtime_error("Unknown token in \"" + src + "\"");
	}

	tokens.push_back(Token{
		"EOF",
		""
	});
}
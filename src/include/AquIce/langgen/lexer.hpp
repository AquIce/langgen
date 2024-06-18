#ifndef __LANGGEN_LEXER__
#define __LANGGEN_LEXER__

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <functional>

/**
 * Peek at the first character a string
 * @param str The string to peek
 * @return The first character
 * @note This function uses `[[nodiscard]]`, you need to explicitly cast the value to `void` if not used
 */
[[nodiscard]] char peek(std::string str);

/**
 * Shift the first character a string
 * @param str The string to shift
 * @return The shifted character
 * @note This function uses `[[nodiscard]]`, you need to explicitly cast the value to `void` if not used
 */
[[nodiscard]] std::string shift(std::string& str);

/**
 * Shift the `n` first characters of a string
 * @param str The string to shift
 * @param n The number of characters to shift
 * @return The shifted string
 * @note This function uses `[[nodiscard]]`, you need to explicitly cast the value to `void` if not used
 */
[[nodiscard]] std::string shift(std::string& str, int n);

/**
 * Check if a string stats with a substring
 * @param str The string to check in
 * @param substr The substring to look for
 * @return Whether `str` starts with `substr`
 */
bool startswith(std::string str, std::string substr);

namespace langgen {
	namespace lexer {
		typedef struct TokenType {
			std::string name;
			std::function<std::string(std::string)> lex_token;
		} TokenType;

		typedef struct Lexer {
			std::vector<TokenType> tokenTypes;
			std::vector<char> whitespaces = std::vector<char>({
				' ',
				'\t',
				'\n',
				'\r',
			});
			std::string singleLineComment = "//";
			std::pair<std::string, std::string> multiLineComment = { "/*", "*/" };
			char decimalSeparator = '.';
			char charDelimiter = '\'';
			char stringDelimiter = '"';
			char escapeCharacter = '\\';
		} Lexer;

		typedef struct Token {
			std::string type;
			std::string value;
		} Token;

		std::string repr(const Token& token);

		TokenType make_token_type(std::string name);

		TokenType make_token_type(std::string name, std::string value);

		TokenType make_token_type(std::string name, std::function<std::string(std::string)> lex_token);

		void setup_lexer(
			Lexer& lexer,
			const std::vector<TokenType>& tokenTypes,
			std::vector<char> whitespaces = std::vector<char>({
				' ',
				'\t',
				'\n',
				'\r',
			}),
			std::string singleLineComment = "//",
			std::pair<std::string, std::string> multiLineComment = { "/*", "*/" },
			char decimalSeparator = '.',
			char charDelimiter = '\'',
			char stringDelimiter = '"',
			char escapeCharacter = '\\'
		);

		void lex(
			Lexer& lexer,
			std::vector<Token>& tokens,
			std::string src
		);
	}
}

#endif // __LANGGEN_LEXER__
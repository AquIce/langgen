# Summary

- [Summary](#summary)
- [Setup](#setup)
	- [Entry point](#entry-point)
	- [Lexer](#lexer)
		- [Configuration](#configuration)
			- [Custom Token Types](#custom-token-types)
			- [More Settings](#more-settings)
		- [Execution](#execution)
		- [Result](#result)
		- [Code](#code)
	- [Parser](#parser)
		- [Configuration](#configuration-1)
			- [Custom Node Classes](#custom-node-classes)
			- [Custom Parsing Functions](#custom-parsing-functions)
			- [Actual Configuration](#actual-configuration)
		- [Execution](#execution-1)
		- [Result](#result-1)
		- [Code](#code-1)
	- [Interpreter](#interpreter)

# Setup

## Entry point

To start, you need a basic C++ program which contains a source string :

```cpp
// main.cpp

int main(void) {
	std::string src = "3 + 100 * .2 / 1 - 2";
	return 0;
}
```

## Lexer

### Configuration

To run your custom lexer, you first need to import the header file.

```cpp
// main.cpp

#include <AquIce/langgen/lexer.hpp>
```

Then, instanciate the `Lexer` struct, found in `langgen::lexer`.

```cpp
// main.cpp > main

langgen::lexer::Lexer lexer;
```

After that, you have to load a custom configuration into this lexer.

For the sake of code readability, we will move this code in its own function : `setup_lexer`.

Here is this function's header :

```cpp
// main.cpp

void setup_lexer(langgen::lexer::Lexer& lexer);
```

#### Custom Token Types

Inside this function, we will first create a list of all valid token types.

We can create token types 3 different ways:

1. A Token Type whose value is the same as its type.
   
	For example, an open parenthesis :
	
	```cpp
	langgen::lexer::make_token_type("(")
	```

2. A Token Type who has a value and a name.

	For example, a `+` operator :

	```cpp
	langgen::lexer::make_token_type("OPERATOR", "+")
	```

3. A Token type who has a name, and a custom value lexing function.

	For example, a number :

	```cpp
	langgen::lexer::make_token_type(
		"NUMBER",
		&lex_number
	)
	```

	> Note: `lex_number` is a function that lexes the value of a number.
	>
	> It can also be input as a lamba function

Finally, we put all those tokens in a list we can use later.

```cpp
// main.cpp > setup_lexer

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
```

> Note: This implementation example uses a lamba function as the lexing function for the `NUMBER` Token Type

Then, we can call the `setup_lexer` function, found in `langgen::lexer`, to configure our tokens.

```cpp
// main.cpp > setup_lexer

langgen::lexer::setup_lexer(
	lexer,
	tokenTypes
);
```

#### More Settings

If you want to go further, you can customize the following things :

- Whitespaces
- Single line comments
- Multi line comments

```cpp
// main.cpp > setup_lexer

langgen::lexer::setup_lexer(
	lexer,
	tokenTypes,
	std::vector<char>({
		' ',
		'\t',
		'\n',
		'\r',
	}),
	"//",
	{ "/*", "*/" }
);
```

> Note: The values shown above are the default values.

### Execution

To execute our lexer, we first need to call the function we just created.

```cpp
// main.cpp > main

setup_lexer(lexer);
```

Then, we create an empty token list, and call the `lex` function from `langgen::lexer` to lex the source string.

```cpp
// main.cpp > main

std::vector<langgen::lexer::Token> tokens;

langgen::lexer::lex(
	lexer,
	tokens,
	src
);
```

### Result

We can then iterate through the same list to display the result of the lexing.

```cpp
for(const langgen::lexer::Token& token : tokens) {
	std::cout << repr(token) << "\n\n";
}
```

### Code

Here is the entire file containing the full lexer configuration :

```cpp
// main.cpp

#include <iostream>
#include <string>

#include <AquIce/langgen/lexer.hpp>

void setup_lexer(langgen::lexer::Lexer& lexer);

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

	return 0;
}

void setup_lexer(langgen::lexer::Lexer& lexer) {
	std::vector<langgen::lexer::TokenType> tokenTypes = std::vector<langgen::lexer::TokenType>({
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
```

## Parser

### Configuration

To run your custom lexer, you first need to import the header file.

```cpp
// main.cpp

#include <AquIce/langgen/parser.hpp>
```

Then, instanciate the `Parser` struct, found in `langgen::parser`.

```cpp
// main.cpp > main

langgen::parser::Parser parser;
```

After that, you have to load a custom configuration into this parser.

As for the lexer, we will move this code in its own function : `setup_parser`.

Here is this function's header :

```cpp
// main.cpp

void setup_parser(langgen::parser::Parser& parser);
```

#### Custom Node Classes

To add a new Node to the AST, we need to create a class representing it.

There are 4 built-in Nodes :

1. `Statement`
	
	The base class, only used for inheritance.

2. `Scope`

	Inherits from `Statement` and is used to store the program.

3. `Expression`

	Inherits from `Statement`, only used for inheritance.

4. `NumberExpression`

	Inherits from `Expression`, holds a single number value.

In order to create a custom Node class, you need to do a few things:

1. Your class must inherit from `Statement` or one of its derived classes

	```cpp
	// main.cpp

	class BinaryExpression : public langgen::ast::Expression {}
	```

2. You need to implement the virtual method `type`

	```cpp
	// main.cpp > BinaryExpression

	virtual std::string type() override {
		return "BinaryExpression";
	}
	```

3. And the `repr` method as well

	```cpp
	virtual std::string repr(int indent = 0) {
		return
			std::string(indent, '\t') + "(\n" +
			this->left->repr(indent + 1) + "\n" +
			std::string(indent + 1, '\t') + this->operator_symbol + "\n" +
			this->right->repr(indent + 1) + "\n" +
			std::string(indent, '\t') + ")";
	```

Then, you can add any data your class might need, as well as a custom constructor :

```cpp
// main.cpp > BinaryExpression

public:
	BinaryExpression(
		std::shared_ptr<langgen::ast::Expression> left,
		std::string operator_symbol,
		std::shared_ptr<langgen::ast::Expression> right
	) {
		this->left = left;
		this->operator_symbol = operator_symbol;
		this->right = right;
	}
```
```cpp
// main.cpp > BinaryExpression

private:
	std::shared_ptr<langgen::ast::Expression> left;
	std::string operator_symbol;
	std::shared_ptr<langgen::ast::Expression> right;
```

#### Custom Parsing Functions

Now that we have the Node, we need a way to parse it.

We have to declare a function that takes a reference to a vector of tokens as argument and returns a shared pointer to an Expression.

As a convention, we will name the functions `parse_<thing_to_parse>`

```cpp
std::shared_ptr<langgen::ast::Expression> parse_any(std::vector<langgen::lexer::Token>& tokens);
```

To simplify the implementation, as well as add the concept of precedence, we will split the implementation between three methods: 

- `parse_multiplicative_expression`
- `parse_additive_expression`
- `parse_binary_expression`

```cpp
std::shared_ptr<langgen::ast::Expression> parse_multiplicative_expression(std::vector<langgen::lexer::Token>& tokens);
std::shared_ptr<langgen::ast::Expression> parse_additive_expression(std::vector<langgen::lexer::Token>& tokens);
std::shared_ptr<langgen::ast::Expression> parse_binary_expression(std::vector<langgen::lexer::Token>& tokens);
```

On the other hand, to keep this explanation concise, we will only look at the implementation of `parse_multiplicative_expression`, as the other functions are basically the same.

First, we need to parse the left of the expression as a Number Expression.

```cpp
// main.cpp > parse_multiplicative_expression

std::shared_ptr<langgen::ast::Expression> left = langgen::parser::parse_number_expression(tokens);
```

Then, if the left is `nullptr`, we return it, as it means we encountered an unknown token.

```cpp
// main.cpp > parse_multiplicative_expression

if(left == nullptr) {
	return nullptr;
}
```

Then, we need to check if the next token is a valid multiplication operator.

```cpp
// main.cpp > parse_multiplicative_expression

if(
	peek(tokens).type == "OPERATOR" &&
	(peek(tokens).value == "*" || peek(tokens).value == "/")
)
```

- If the token was valid, we can eat it, and parse the right as a Binary Expression

	We can then return a new Binary Expression containing the left, the operator and the right.

	```cpp
	// main.cpp > parse_multiplicative_expression

	std::string operator_symbol = eat(tokens).value;
	std::shared_ptr<langgen::ast::Expression> right = parse_multiplicative_expression(tokens);
	return std::make_shared<BinaryExpression>(
		left,
		operator_symbol,
		right
	);
	```

- If the token is not valid, we return the left of the Expression

	```cpp
	// main.cpp > parse_multiplicative_expression

	return left;
	```

#### Actual Configuration

Now that we have everything, we just need to link it all together in our `setup_parser` function.

First, we call the `setup_parser` function found in `langgen::parser`.

We pass the Node Types we want to add as parameters (in our example only `BinaryExpression`).

```cpp
langgen::parser::setup_parser(
	parser,
	{
		{
			"BinaryExpression",
			langgen::parser::make_node(&parse_binary_expression)
		}
	}
);
```

A really important notion to understand is whether a Node Type is a `topNode` or not.

Being a top Node means that this Node has to be parsed by `langgen::parser::parse_statement`.

A non-top Node is a Node that is parsed by another (for example `NumberExpression`).

We use the `demoteTopNode` function to make `NumberExpression` a non-top Node.

```cpp
langgen::parser::demoteTopNode(parser, "NumberExpression");
```

### Execution

To execute our parser, we first need to call the function we just created.

```cpp
// main.cpp > main

setup_parser(parser);
```

Then, we create a Program Node, and call the `parse` function from `langgen::parser` to parse the token list.

```cpp
// main.cpp > main

std::shared_ptr<langgen::ast::Scope> program = std::make_shared<langgen::ast::Scope>();

langgen::parser::parse(
	parser,
	program,
	tokens
);
```

### Result

We can display the result of the parsing with 

```cpp
std::cout << program->repr() << std::endl;
```

### Code

Here is the entire file containing the full parser and lexer configuration :

```cpp
// main.cpp > main

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
```

## Interpreter

`[NOT IMPLEMENTED YET]`
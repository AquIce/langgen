default: run

build:
	g++ -Iinclude -Wall -Wpedantic -Werror langgen/lexer/lexer.cpp langgen/parser/ast.cpp langgen/parser/parser.cpp main.cpp -o lg

run: build
	./lg
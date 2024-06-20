default: run

build:
	g++ -I../src/include .\langgen.dll .\Tlang\lexer\lexer.cpp .\Tlang\parser\ast.cpp .\Tlang\parser\parser.cpp .\Tlang\interpreter\env.cpp .\Tlang\interpreter\values.cpp .\Tlang\interpreter\interpreter.cpp main.cpp -o tlang

run: build
	./tlang
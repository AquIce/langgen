.ONESHELL:

default: run

build_obj:
	cd build
	g++ -I../include -Wall -Wpedantic -Werror ../langgen/lexer/lexer.cpp ../langgen/parser/ast.cpp ../langgen/parser/parser.cpp ../langgen/interpreter/values.cpp ../langgen/interpreter/env.cpp ../langgen/interpreter/interpreter.cpp -c
	cd ..

build_lib: build_obj
	cd build
	g++ -shared -o ../langgen.dll *.o
	cd ..

build_wlib: build_lib
	g++ -Iinclude -Wall -Wpedantic -Werror langgen.dll main.cpp -o lg

run: build_wlib
	./lg
#ifndef __T_INTERPRETER__
#define __T_INTERPRETER__

#include "../parser/ast.hpp"
#include "values.hpp"
#include "env.hpp"
#include <AquIce/langgen/interpreter.hpp>

void setup_interpreter(langgen::interpreter::Interpreter& interpreter);

namespace tlang {
	namespace interpreter {
		std::shared_ptr<langgen::values::RuntimeValue> evaluate_identifier(
			langgen::interpreter::Interpreter& interpreter,
			std::shared_ptr<langgen::ast::Statement> statement,
			std::shared_ptr<langgen::env::Environment> env
		);
		std::shared_ptr<langgen::values::RuntimeValue> evaluate_boolean_expression(
			langgen::interpreter::Interpreter& interpreter,
			std::shared_ptr<langgen::ast::Statement> statement,
			std::shared_ptr<langgen::env::Environment> env
		);
		std::shared_ptr<langgen::values::RuntimeValue> evaluate_unary_expression(
			langgen::interpreter::Interpreter& interpreter,
			std::shared_ptr<langgen::ast::Statement> statement,
			std::shared_ptr<langgen::env::Environment> env
		);
		std::shared_ptr<langgen::values::RuntimeValue> evaluate_binary_expression(
			langgen::interpreter::Interpreter& interpreter,
			std::shared_ptr<langgen::ast::Statement> statement,
			std::shared_ptr<langgen::env::Environment> env
		);
		std::shared_ptr<langgen::values::RuntimeValue> evaluate_assignation_expression(
			langgen::interpreter::Interpreter& interpreter,
			std::shared_ptr<langgen::ast::Statement> statement,
			std::shared_ptr<langgen::env::Environment> env
		);
		std::shared_ptr<langgen::values::RuntimeValue> evaluate_declaration_expression(
			langgen::interpreter::Interpreter& interpreter,
			std::shared_ptr<langgen::ast::Statement> statement,
			std::shared_ptr<langgen::env::Environment> env
		);
	}
}

#endif // __T_INTERPRETER__
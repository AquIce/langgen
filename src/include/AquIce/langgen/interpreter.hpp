#ifndef __LANGGEN_INTERPRETER__
#define __LANGGEN_INTERPRETER__

#include <AquIce/langgen/ast.hpp>
#include <AquIce/langgen/values.hpp>
#include <AquIce/langgen/env.hpp>

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>

namespace langgen {
	namespace interpreter {

		struct Interpreter;

		typedef std::function<std::shared_ptr<langgen::values::RuntimeValue> (
			langgen::interpreter::Interpreter&,
			std::shared_ptr<langgen::ast::Statement>,
			std::shared_ptr<langgen::env::Environment>
		)> ParseStatementFunction;
		
		typedef struct Interpreter {
			std::vector<std::string> envValuesProperties;
			std::vector<langgen::env::EnvValidationRule> validationRules;
			std::unordered_map<std::string, ParseStatementFunction> nodeEvaluationFunctions;
		} Interpreter;

		void setup_interpreter(
			Interpreter& interpreter,
			std::vector<std::string> envValuesProperties,
			std::vector<langgen::env::EnvValidationRule> validationRules,
			std::unordered_map<std::string, ParseStatementFunction> nodeEvaluationFunctions
		);

		std::shared_ptr<langgen::values::RuntimeValue> evaluate_statement(
			Interpreter& interpreter,
			std::shared_ptr<langgen::ast::Statement> statement,
			std::shared_ptr<langgen::env::Environment> env
		);

		std::shared_ptr<langgen::values::RuntimeValue> evaluate_scope(
			Interpreter& interpreter,
			std::shared_ptr<langgen::ast::Scope> scope,
			std::unordered_map<std::string, std::string>& results,
			std::shared_ptr<langgen::env::Environment> scope_env = nullptr,
			std::shared_ptr<langgen::env::Environment> parent_env = nullptr
		);

		void interpret(
			Interpreter& interpreter,
			std::unordered_map<std::string, std::string>& results, 
			std::shared_ptr<langgen::ast::Scope> program
		);
	}
}

#endif // __LANGGEN_INTERPRETER__
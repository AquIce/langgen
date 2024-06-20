#include <AquIce/langgen/interpreter.hpp>

void langgen::interpreter::setup_interpreter(
	langgen::interpreter::Interpreter& interpreter,
	std::unordered_map<std::string, ParseStatementFunction> nodeEvaluationFunctions,
	std::vector<std::string> envValuesProperties,
	std::vector<langgen::env::EnvValidationRule> validationRules
) {
	interpreter.envValuesProperties = envValuesProperties;

	interpreter.validationRules = validationRules;

	interpreter.nodeEvaluationFunctions = nodeEvaluationFunctions;
	interpreter.nodeEvaluationFunctions["NumberExpression"] = [] (
		langgen::interpreter::Interpreter& interpreter,
		std::shared_ptr<langgen::ast::Statement> statement,
		std::shared_ptr<langgen::env::Environment> env
	) -> std::shared_ptr<langgen::values::RuntimeValue> {
		return std::make_shared<langgen::values::NumberValue>(
			std::dynamic_pointer_cast<langgen::ast::NumberExpression>(statement)->value
		);
	};
}

std::shared_ptr<langgen::values::RuntimeValue> langgen::interpreter::evaluate_statement(
	langgen::interpreter::Interpreter& interpreter,
	std::shared_ptr<langgen::ast::Statement> statement,
	std::shared_ptr<langgen::env::Environment> env
) {
	for(const auto& [nodeType, evaluateFn] : interpreter.nodeEvaluationFunctions) {
		if(statement->type() == nodeType) {
			return evaluateFn(interpreter, statement, env);
		}
	}
	throw std::runtime_error("Trying to evaluate unknown statement " + statement->type());
}

std::shared_ptr<langgen::values::RuntimeValue> langgen::interpreter::evaluate_scope(
	langgen::interpreter::Interpreter& interpreter,
	std::shared_ptr<langgen::ast::Scope> scope,
	std::unordered_map<std::string, std::string>& results,
	std::shared_ptr<langgen::env::Environment> scope_env,
	std::shared_ptr<langgen::env::Environment> parent_env
) {
	if(scope_env == nullptr) {
		scope_env = std::make_shared<langgen::env::Environment>(
			interpreter.envValuesProperties,
			parent_env
		);
	}

	std::shared_ptr<langgen::values::RuntimeValue> result;
	std::shared_ptr<langgen::values::RuntimeValue> prev_result = std::make_shared<langgen::values::NullValue>();

	for(std::shared_ptr<langgen::ast::Statement> statement : scope->body) {
		result = langgen::interpreter::evaluate_statement(interpreter, statement, scope_env);
		prev_result = result;
		results[statement->repr()] = result->repr();
	}
	return result;
}

void langgen::interpreter::interpret(
	langgen::interpreter::Interpreter& interpreter,
	std::unordered_map<std::string, std::string>& results,
	std::shared_ptr<langgen::ast::Scope> program
) {
	std::shared_ptr<langgen::env::Environment> env = std::make_shared<langgen::env::Environment>(
		interpreter.envValuesProperties
	);

	langgen::interpreter::evaluate_scope(
		interpreter,
		program,
		results,
		env
	);
}
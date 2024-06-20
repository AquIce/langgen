#include "interpreter.hpp"

std::shared_ptr<langgen::values::RuntimeValue> tlang::interpreter::evaluate_boolean_expression (
	langgen::interpreter::Interpreter& interpreter,
	std::shared_ptr<langgen::ast::Statement> statement,
	std::shared_ptr<langgen::env::Environment> env
) {
	std::shared_ptr<langgen::ast::BooleanExpression> booleanExpression = std::dynamic_pointer_cast<langgen::ast::BooleanExpression>(statement);

	return std::make_shared<langgen::values::BooleanValue>(booleanExpression->value);
};

std::shared_ptr<langgen::values::RuntimeValue> tlang::interpreter::evaluate_unary_expression (
	langgen::interpreter::Interpreter& interpreter,
	std::shared_ptr<langgen::ast::Statement> statement,
	std::shared_ptr<langgen::env::Environment> env
) {
	std::shared_ptr<langgen::ast::UnaryExpression> unaryExpression = std::dynamic_pointer_cast<langgen::ast::UnaryExpression>(statement);

	std::shared_ptr<langgen::values::RuntimeValue> term = langgen::interpreter::evaluate_statement(interpreter, unaryExpression->get_term(), env);
	std::string operator_symbol = unaryExpression->get_operator_symbol();

	if(operator_symbol == "!") {
		return std::make_shared<langgen::values::BooleanValue>(!term->IsTrue());
	}

	throw std::runtime_error("Unknown unary operator " + operator_symbol);
}

std::shared_ptr<langgen::values::RuntimeValue> tlang::interpreter::evaluate_binary_expression (
	langgen::interpreter::Interpreter& interpreter,
	std::shared_ptr<langgen::ast::Statement> statement,
	std::shared_ptr<langgen::env::Environment> env
) {
	std::shared_ptr<langgen::ast::BinaryExpression> binaryExpression = std::dynamic_pointer_cast<langgen::ast::BinaryExpression>(statement);

	std::shared_ptr<langgen::values::RuntimeValue> left = langgen::interpreter::evaluate_statement(interpreter, binaryExpression->get_left(), env);
	std::shared_ptr<langgen::values::RuntimeValue> right = langgen::interpreter::evaluate_statement(interpreter, binaryExpression->get_right(), env);

	std::string operator_symbol = binaryExpression->get_operator_symbol();

	if(operator_symbol == "+") {
		if(
			left->type() == "NumberValue" &&
			right->type() == "NumberValue"
		) {
			throw std::runtime_error("Trying to add invalid operands");
		}

		double left_nb = std::dynamic_pointer_cast<langgen::values::NumberValue>(left)->get();
		double right_nb = std::dynamic_pointer_cast<langgen::values::NumberValue>(right)->get();
		
		return std::make_shared<langgen::values::NumberValue>(left_nb + right_nb);
	}
	if(operator_symbol == "-") {
		if(
			left->type() == "NumberValue" &&
			right->type() == "NumberValue"
		) {
			throw std::runtime_error("Trying to subtract invalid operands");
		}
		
		double left_nb = std::dynamic_pointer_cast<langgen::values::NumberValue>(left)->get();
		double right_nb = std::dynamic_pointer_cast<langgen::values::NumberValue>(right)->get();
		
		return std::make_shared<langgen::values::NumberValue>(left_nb - right_nb);
	}
	if(operator_symbol == "*") {
		if(
			left->type() == "NumberValue" &&
			right->type() == "NumberValue"
		) {
			throw std::runtime_error("Trying to multiply invalid operands");
		}
		
		double left_nb = std::dynamic_pointer_cast<langgen::values::NumberValue>(left)->get();
		double right_nb = std::dynamic_pointer_cast<langgen::values::NumberValue>(right)->get();
		
		return std::make_shared<langgen::values::NumberValue>(left_nb * right_nb);
	}
	if(operator_symbol == "/") {
		if(
			left->type() == "NumberValue" &&
			right->type() == "NumberValue"
		) {
			throw std::runtime_error("Trying to divide invalid operands");
		}
		
		double left_nb = std::dynamic_pointer_cast<langgen::values::NumberValue>(left)->get();
		double right_nb = std::dynamic_pointer_cast<langgen::values::NumberValue>(right)->get();

		if(right_nb == 0) {
			throw std::runtime_error("Trying to divide by zero");
		}
		
		return std::make_shared<langgen::values::NumberValue>(left_nb / right_nb);
	}
	if(operator_symbol == "&&") {
		return std::make_shared<langgen::values::BooleanValue>(left->IsTrue() && right->IsTrue());
	}
	if(operator_symbol == "||") {
		return std::make_shared<langgen::values::BooleanValue>(left->IsTrue() || right->IsTrue());
	}

	throw std::runtime_error("Unknown operator " + operator_symbol);
}

void setup_interpreter(langgen::interpreter::Interpreter& interpreter) {

	std::unordered_map<std::string, langgen::interpreter::ParseStatementFunction> nodeEvaluationFunctions = std::unordered_map<std::string, langgen::interpreter::ParseStatementFunction>({
		{
			"BooleanExpression",
			&tlang::interpreter::evaluate_boolean_expression
		},
		{
			"UnaryExpression",
			&tlang::interpreter::evaluate_unary_expression
		},
		{
			"BinaryExpression",
			&tlang::interpreter::evaluate_binary_expression
		},
	});

	std::vector<std::string> envValuesProperties = std::vector<std::string>({
		"isMutable"
	});

	langgen::env::EnvValidationRule mutabilityValidation = {
		&tlang::env::validate_mutability,
		std::vector<langgen::env::ValidationRuleSensitivity>({
			langgen::env::ValidationRuleSensitivity::SET
		})
	};

	std::vector<langgen::env::EnvValidationRule> validationRules = std::vector<langgen::env::EnvValidationRule>({
		mutabilityValidation
	});

	langgen::interpreter::setup_interpreter(
		interpreter,
		nodeEvaluationFunctions,
		envValuesProperties,
		validationRules
	);
}
#include "ast.hpp"

#pragma region BooleanExpression

langgen::ast::BooleanExpression::BooleanExpression(bool value) {
	this->value = value;
}

std::string langgen::ast::BooleanExpression::BooleanExpression::type() {
	return "BooleanExpression";
}
std::string langgen::ast::BooleanExpression::repr(int indent) {
	return std::string(indent, '\t') + std::string(this->value ? "true" : "false");
}

#pragma endregion

#pragma region UnaryExpression

langgen::ast::UnaryExpression::UnaryExpression(
	std::shared_ptr<Expression> term,
	std::string operator_symbol
) {
	this->term = term;
	this->operator_symbol = operator_symbol;
}

std::shared_ptr<langgen::ast::Expression> langgen::ast::UnaryExpression::get_term() {
	return this->term;
}

std::string langgen::ast::UnaryExpression::get_operator_symbol() {
	return this->operator_symbol;
}

std::string langgen::ast::UnaryExpression::type() {
	return "UnaryExpression";
}
std::string langgen::ast::UnaryExpression::repr(int indent) {
	return
		std::string(indent, '\t') + "(\n" +
		std::string(indent + 1, '\t') + this->operator_symbol + "\n" +
		this->term->repr(indent + 1) + "\n" +
		std::string(indent, '\t') + ")";
}

#pragma endregion

#pragma region BinaryExpression

langgen::ast::BinaryExpression::BinaryExpression(
	std::shared_ptr<langgen::ast::Expression> left,
	std::string operator_symbol,
	std::shared_ptr<langgen::ast::Expression> right
) {
	this->left = left;
	this->operator_symbol = operator_symbol;
	this->right = right;
}

std::shared_ptr<langgen::ast::Expression> langgen::ast::BinaryExpression::get_left() {
	return this->left;
}
std::string langgen::ast::BinaryExpression::get_operator_symbol() {
	return this->operator_symbol;
}
std::shared_ptr<langgen::ast::Expression> langgen::ast::BinaryExpression::get_right() {
	return this->right;
}

std::string langgen::ast::BinaryExpression::type() {
	return "BinaryExpression";
}
std::string langgen::ast::BinaryExpression::repr(int indent) {
	return
		std::string(indent, '\t') + "(\n" +
		this->left->repr(indent + 1) + "\n" +
		std::string(indent + 1, '\t') + this->operator_symbol + "\n" +
		this->right->repr(indent + 1) + "\n" +
		std::string(indent, '\t') + ")";
}

#pragma endregion
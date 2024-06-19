#include <AquIce/langgen/ast.hpp>

std::string langgen::ast::Statement::type() {
	return "Statement";
}
std::string langgen::ast::Statement::repr(int indent) {
	return std::string(indent, '\t') + "Statement";
}

langgen::ast::Scope::Scope(std::vector<std::shared_ptr<langgen::ast::Statement>> body) {
	this->body = body;
}
			
std::string langgen::ast::Scope::type() {
	return "Scope";
}
std::string langgen::ast::Scope::repr(int indent) {
	std::string pretty = std::string(indent, '\t') + "{\n";

	for(std::shared_ptr<langgen::ast::Statement> statement : this->body) {
		pretty += statement->repr(indent + 1);
	}

	pretty += std::string(indent, '\t') + "\n}";

	return pretty;
}

langgen::ast::NumberExpression::NumberExpression(double value) {
	this->value = value;
}
std::string langgen::ast::NumberExpression::type() {
	return "NumberExpression";
}
std::string langgen::ast::NumberExpression::repr(int indent) {
	return std::string(indent, '\t') + "NumberExpression(" + std::to_string(this->value) + ")";
}
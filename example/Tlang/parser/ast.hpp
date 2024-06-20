#ifndef __T_AST__
#define __T_AST__

#include <AquIce/langgen/ast.hpp>

namespace tlang {
	namespace ast {

		class Identifier : public langgen::ast::Expression {
		public:
			Identifier(std::string name);
			
			std::string get_name();

			virtual std::string type() override;
			virtual std::string repr(int indent = 0);

		private:
			std::string name;
		};

		class AssignationExpression : public langgen::ast::Expression {
		public:
			AssignationExpression(
				std::shared_ptr<Identifier> identifier,
				std::shared_ptr<langgen::ast::Expression> value
			);

			std::shared_ptr<Identifier> get_identifier();
			std::shared_ptr<langgen::ast::Expression> get_value();

			virtual std::string type() override;
			virtual std::string repr(int indent = 0);

		protected:
			std::shared_ptr<Identifier> identifier;
			std::shared_ptr<langgen::ast::Expression> value;
		};

		class DeclarationExpression : public AssignationExpression {
		public:
			DeclarationExpression(std::shared_ptr<Identifier> identifier, std::shared_ptr<langgen::ast::Expression> value, std::string value_type, bool isMutable);

			std::string get_value_type();
			bool get_mutability();

			virtual std::string type() override;
			virtual std::string repr(int indent = 0);

		private:
			std::string value_type;
			bool isMutable;
		};
		
		class BooleanExpression : public langgen::ast::Expression {
		public:
			bool value;

			BooleanExpression(bool value);
			
			virtual std::string type() override;
			virtual std::string repr(int indent = 0);
		};

		class UnaryExpression : public langgen::ast::Expression {
		public:
			UnaryExpression(
				std::shared_ptr<Expression> term,
				std::string operator_symbol
			);

			std::shared_ptr<Expression> get_term();
			std::string get_operator_symbol();

			virtual std::string type() override;
			virtual std::string repr(int indent = 0);

		private:
			std::shared_ptr<Expression> term;
			std::string operator_symbol;
		};

		class BinaryExpression : public langgen::ast::Expression {
		public:
			BinaryExpression(
				std::shared_ptr<Expression> left,
				std::string operator_symbol,
				std::shared_ptr<Expression> right
			);

			std::shared_ptr<Expression> get_left();
			std::string get_operator_symbol();
			std::shared_ptr<Expression> get_right();
			
			virtual std::string type() override;
			virtual std::string repr(int indent = 0);

		private:
			std::shared_ptr<Expression> left;
			std::string operator_symbol;
			std::shared_ptr<Expression> right;
		};
	}
}

#endif // __T_AST__
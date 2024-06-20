#ifndef __T_AST__
#define __T_AST__

#include <AquIce/langgen/ast.hpp>

namespace langgen {
	namespace ast {
		
		class BooleanExpression : public Expression {
		public:
			bool value;

			BooleanExpression(bool value);
			
			virtual std::string type() override;
			virtual std::string repr(int indent = 0);
		};

		class UnaryExpression : public Expression {
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

		class BinaryExpression : public Expression {
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
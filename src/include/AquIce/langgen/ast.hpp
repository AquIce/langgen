#ifndef __LANGGEN_AST__
#define __LANGGEN_AST__

#include <string>
#include <vector>
#include <memory>

namespace langgen {
	namespace ast {

		class Statement;
		class Scope;
		class Expression;
		class NumberExpression;

		/**
		 * Statement
		 * @note This class is only used for inheritance purpose, never as a value
		 */
		class Statement {
		public:

			/**
			 * Get the NodeType of the Statement
			 */
			virtual std::string type();
			
			/**
			 * Get the string representation of the Statement
			 */
			virtual std::string repr(int indent = 0);
		};

		/**
		 * Scope < Statement
		 * @note This class is only used for inheritance purpose or as a general wrapper for a program
		 */
		class Scope : public Statement {
		public:
			std::vector<std::shared_ptr<Statement>> body;
			
			Scope(std::vector<std::shared_ptr<Statement>> body = std::vector<std::shared_ptr<Statement>>());
			
			virtual std::string type() override;
			virtual std::string repr(int indent = 0) override;
		};

		/**
		 * Expression < Statement
		 * @note This class is only used for inheritance purpose, never as a value
		 */
		class Expression : public Statement {};

		/**
		 * NumberExpression < Expression < Statement
		 */
		class NumberExpression : public Expression {
		public:
			double value;

			NumberExpression(double value);
			
			virtual std::string type() override;
			virtual std::string repr(int indent = 0);
		};
	}
}

#endif // __LANGGEN_AST__
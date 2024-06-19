#ifndef __LANGGEN_VALUES__
#define __LANGGEN_VALUES__

#include <string>
#include <memory>

namespace langgen {
	namespace values {

		#pragma region Classes

		/**
		 * RuntimeValue
		 * @note This class is only used for inheritance purpose, never as a value
		 */
		class RuntimeValue {
		public:
			/**
			 * Get the type of the value
			 * @return The type
			 */
			virtual std::string type();

			/**
			 * Get the string representation of the value
			 * @return The string representation
			 */
			virtual std::string repr();
			
			/**
			 * Checks whether the value is true or false
			 */
			virtual bool IsTrue();
		};

		/**
		 * NullValue < RuntimeValue
		 */
		class NullValue: public RuntimeValue {
		public:
			/**
			 * Create a new Null Value
			 */
			NullValue();
			
			virtual std::string type() override;
			
			virtual std::string repr() override;
			
			virtual bool IsTrue() override;
		};

		/**
		 * NumberValue < RuntimeValue
		 */
		class NumberValue: public RuntimeValue {
		public:
			/**
			 * Create a new Null Value
			 */
			NumberValue(double value = 0);
			
			double get();

			virtual std::string type() override;
			
			virtual std::string repr() override;
			
			virtual bool IsTrue() override;

		private:
			double value;
		};

		#pragma endregion
	}
}

#endif // __LANGGEN_VALUES__
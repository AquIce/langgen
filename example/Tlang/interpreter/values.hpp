#ifndef __T_VALUES__
#define __T_VALUES__

#include <AquIce/langgen/values.hpp>

namespace tlang {
	namespace values {
		class BooleanValue: public langgen::values::RuntimeValue {
		public:
			/**
			 * Create a new Boolean Value
			 */
			BooleanValue(bool value = false);
			
			bool get();

			virtual std::string type() override;
			virtual std::string repr() override;
			virtual bool IsTrue() override;

		private:
			bool value;
		};
	}
}

#endif
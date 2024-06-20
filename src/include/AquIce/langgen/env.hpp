#ifndef __LANGGEN_ENV__
#define __LANGGEN_ENV__

#include <AquIce/langgen/values.hpp>

#include <string>
#include <memory>
#include <algorithm>
#include <unordered_map>
#include <stdexcept>
#include <functional>

namespace langgen {
	namespace env {

		enum class ValidationRuleSensitivity {
			INIT,
			SET,
			GET
		};

		/**
		 * An environnment value
		 */
		typedef struct EnvValue {
			/**
			 * The actual runtime value
			 */
			std::shared_ptr<langgen::values::RuntimeValue> value;
			/**
			 * The value properties
			 */
			std::unordered_map<std::string, std::string> properties;
		} EnvValue;

		typedef struct EnvValidationRule {
			std::function<bool (EnvValue)> validationFunction;
			std::vector<ValidationRuleSensitivity> sensitivity;
		} EnvValidationRule;

		#pragma region Classes

		/**
		 * An Environment
		 */
		class Environment {
		public:
			/**
			 * Create a new Environment
			 */
			Environment(std::vector<std::string> envValuesProperties, std::vector<EnvValidationRule> validationRules = std::vector<EnvValidationRule>(), std::shared_ptr<Environment> parent = nullptr);

			/**
			 * Check whether this environment has a given key (variable / constant)
			 */
			bool has_value(std::string key);

			/**
			 * Set a value in the environment or its parents
			 * @param key The key to set the value for
			 * @param value The value to set
			 * @return The result value
			 */
			std::shared_ptr<langgen::values::RuntimeValue> set_value(
				std::string key,
				std::shared_ptr<langgen::values::RuntimeValue> value
			);
			
			/**
			 * Initialize a value at a given key
			 * @param key The key of the value
			 * @param value The value
			 * @param isMutable Whether the value is mutable
			 */
			std::shared_ptr<langgen::values::RuntimeValue> init_value(
				std::string key,
				std::shared_ptr<langgen::values::RuntimeValue> value,
				std::unordered_map<std::string, std::string> properties
			);
			
			/**
			 * Get a value at a given key in the environment or its parents
			 * @param key The key of the value
			 * @return The value
			 */
			std::shared_ptr<langgen::values::RuntimeValue> get_value(std::string key);
			
		private:
			/**
			 * The parent environment
			 */
			std::shared_ptr<Environment> parent = nullptr;
			/**
			 * The values held by the environment (variables / constants)
			 */
			std::unordered_map<std::string, EnvValue> values;

			std::vector<std::string> envValuesProperties;

			std::vector<EnvValidationRule> validationRules;
		};

		#pragma endregion
	}
}

#endif // __LANGGEN_ENV__
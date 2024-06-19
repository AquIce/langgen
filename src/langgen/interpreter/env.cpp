#include <AquIce/langgen/env.hpp>

langgen::env::Environment::Environment(
	std::vector<std::string> envValuesProperties,
	std::shared_ptr<Environment> parent
) {
	this->envValuesProperties = envValuesProperties;
	this->parent = parent;
	this->values = std::unordered_map<std::string, std::shared_ptr<EnvValue>>();
}

bool langgen::env::Environment::has_value(std::string key) {
	return this->values.find(key) != this->values.end();
}

std::shared_ptr<langgen::values::RuntimeValue> langgen::env::Environment::set_value(
	std::string key,
	std::shared_ptr<langgen::values::RuntimeValue> value
) {
	if(!this->has_value(key)) {
		if(this->parent != nullptr) {
			return this->parent->set_value(key, value);
		}
		throw std::runtime_error("Trying to set non-declared variable " + key);
	}
	
	std::shared_ptr<langgen::values::RuntimeValue> old_val = this->get_value(key);

	this->values.at(key)->value = value;

	return old_val;
}

std::shared_ptr<langgen::values::RuntimeValue> langgen::env::Environment::init_value(
	std::string key,
	std::shared_ptr<langgen::values::RuntimeValue> value,
	std::unordered_map<std::string, std::string> properties
) {

	for(const auto& [prop_key, prop_value] : properties) {
		if(std::find(this->envValuesProperties.begin(), this->envValuesProperties.end(), prop_key) == this->envValuesProperties.end()) {
			throw std::runtime_error("Invalid variable property " + prop_key);
		}
	}

	if(this->has_value(key)) {
		throw std::runtime_error("Trying to redeclare an existing variable " + key);
	}
	this->values[key] = std::make_shared<langgen::env::EnvValue>(langgen::env::EnvValue{value, properties});

	return value;
}

std::shared_ptr<langgen::values::RuntimeValue> langgen::env::Environment::get_value(std::string key) {
	if(!this->has_value(key)) {
		if(this->parent != nullptr) {
			return this->parent->get_value(key);
		}
		throw std::runtime_error("Trying to get non-declared variable " + key);
	}
	return this->values.at(key)->value;
}
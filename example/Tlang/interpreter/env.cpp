#include "env.hpp"
#include <iostream>

bool tlang::env::validate_mutability(langgen::env::EnvValue value) {
	std::cout << (value.properties.at("isMutable") == "true") << std::endl;
	try {
		return value.properties.at("isMutable") == "true";
	} catch(const std::exception& e) {
		throw std::runtime_error("Trying to access undeclared property \"isMutable\"");
	}
}
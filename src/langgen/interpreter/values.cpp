#include <AquIce/langgen/values.hpp>

#include <stdexcept>

#pragma region RuntimeValue

std::string langgen::values::RuntimeValue::type() {
	return "RuntimeValue";
};
std::string langgen::values::RuntimeValue::repr() {
	return "RuntimeValue";
}
bool langgen::values::RuntimeValue::IsTrue() {
	return false;
}

#pragma endregion

#pragma region NullValue

langgen::values::NullValue::NullValue() {}
std::string langgen::values::NullValue::type() {
	return "NullValue";
}
std::string langgen::values::NullValue::repr() {
	return "null";
}
bool langgen::values::NullValue::IsTrue() {
	return false;
}

langgen::values::NumberValue::NumberValue(double value) {
	this->value = value;
}
double langgen::values::NumberValue::get() {
	return this->value;
}
std::string langgen::values::NumberValue::type() {
	return "NumberValue";
}
std::string langgen::values::NumberValue::repr() {
	return std::to_string(this->value);
}
bool langgen::values::NumberValue::IsTrue() {
	return this->value != 0;
}

#pragma endregion
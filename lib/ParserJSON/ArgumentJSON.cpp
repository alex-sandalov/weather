#include "ArgumentJSON.h"

BaseArgumentJSON::BaseArgumentJSON(std::string_view name_argument)
	: name_(name_argument)
{}

std::string_view BaseArgumentJSON::GetNameArgument() {
	return name_;
}

VectorStringArgumentJSON::VectorStringArgumentJSON(std::vector<std::string_view>& data, std::string_view name_argument)
	: BaseArgumentJSON(name_argument)
	, data_(data)
{}

void VectorStringArgumentJSON::AddElement(std::string_view value) {
	data_.push_back(value);
}

const std::string_view& VectorStringArgumentJSON::operator[](size_t index) const {
	return data_[index];
} 

VariableIntArgumentJSON::VariableIntArgumentJSON(int& data, std::string_view name_argument)
	: BaseArgumentJSON(name_argument) 
	, data_(data)
{}

void VariableIntArgumentJSON::SetElement(const int& value) {
	data_ = value;
}

VariableDoubleArgumentJSON::VariableDoubleArgumentJSON(double& data, std::string_view name_argument)
	: BaseArgumentJSON(name_argument)
	, data_(data)
{}

void VariableDoubleArgumentJSON::SetElement(const double& value) {
	data_ = value;
}

VectorDoubleArgumentJSON::VectorDoubleArgumentJSON(std::vector<double>& data, std::string_view name_argument)
	: BaseArgumentJSON(name_argument)
	, data_(data)
{}

void VectorDoubleArgumentJSON::AddElement(const double& value) {
	data_.push_back(value);
}

const double& VectorDoubleArgumentJSON::operator[](size_t index) const {
	return data_[index];
}
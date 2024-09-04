#include "ParserJSON.h"

#include <iostream>

ParserJSON::ParserJSON(const fs::path& file, std::string_view main_path)
	: file_(file)
	, main_path_(main_path)
{}

ParserJSON::ParserJSON(const json& response_json, std::string_view main_path)
	: response_json_(response_json)
	, main_path_(main_path)
{}

ParserJSON& ParserJSON::AddIntVariable(int& reference_variable, std::string_view name_argument,
										const std::vector<std::string_view>& path) noexcept
{
	SetPath(path);
	arguments_json_.push_back(new VariableIntArgumentJSON(reference_variable, name_argument));
	return *this;
}

ParserJSON& ParserJSON::AddStringArray(std::vector<std::string_view>& reference_array, 
										std::string_view name_argument,
										const std::vector<std::string_view>& path) noexcept 
{	
	SetPath(path);
	arguments_json_.push_back(new VectorStringArgumentJSON(reference_array, name_argument));
	return *this;
}

ParserJSON& ParserJSON::AddDoubleVariable(double& reference_variable, std::string_view name_argument,
										const std::vector<std::string_view>& path) noexcept
{
	SetPath(path);
	arguments_json_.push_back(new VariableDoubleArgumentJSON(reference_variable, name_argument));
	return *this;
}

ParserJSON& ParserJSON::AddDoubleArray(std::vector<double>& reference_array, std::string_view name_argument,
								const std::vector<std::string_view>& path) noexcept
{
	SetPath(path);
	arguments_json_.push_back(new VectorDoubleArgumentJSON(reference_array, name_argument));
	return* this;
}

void ParserJSON::SetPath(const std::vector<std::string_view>& path) noexcept {
	for (size_t i = 0; i < path.size(); ++i) {
		if (i + 1 == path.size()) {
			continue;
		}
		path_json_[AsString(path[i])].push_back(AsString(path[i + 1]));
	}
}

void ParserJSON::Parse() {
	try {
		if (file_.is_open()) {
			response_json_ = json::parse(file_);
		}
	} catch (const std::exception& error) {
    	throw std::invalid_argument("Error parsing config: " + std::string(error.what()));
	}
	Parse(response_json_);

	std::unordered_map<std::string, bool> used;
	Parse(main_path_, response_json_, used);
}

void ParserJSON::Parse(const json& json_path) noexcept {
	for (size_t i = 0; i < arguments_json_.size(); ++i) {		
		if (!json_path.contains(arguments_json_[i]->GetNameArgument())) {
			continue;
		}

		if (dynamic_cast<VectorStringArgumentJSON*>(arguments_json_[i])) {
			std::vector<std::string_view> value = json_path[arguments_json_[i]->GetNameArgument()];			
			for (const std::string_view& element : value) {
				dynamic_cast<VectorStringArgumentJSON*>(arguments_json_[i])->AddElement(element);
			}
		} else if (dynamic_cast<VariableIntArgumentJSON*>(arguments_json_[i])) {
			int value = json_path[arguments_json_[i]->GetNameArgument()];
			dynamic_cast<VariableIntArgumentJSON*>(arguments_json_[i])->SetElement(value);
		} else if (dynamic_cast<VariableDoubleArgumentJSON*>(arguments_json_[i])) {
			double value = json_path[arguments_json_[i]->GetNameArgument()];
			dynamic_cast<VariableDoubleArgumentJSON*>(arguments_json_[i])->SetElement(value);
		} else if (dynamic_cast<VectorDoubleArgumentJSON*>(arguments_json_[i])) {
			std::vector<double> value = json_path[arguments_json_[i]->GetNameArgument()];
			for (const double& element : value) {
				dynamic_cast<VectorDoubleArgumentJSON*>(arguments_json_[i])->AddElement(element);
			}
		}
	}
}

void ParserJSON::Parse(std::string_view path_name, const json& path, std::unordered_map<std::string, bool> used) noexcept {
	for (const std::string& value : path_json_[AsString(path_name)]) {
		if (used[value]) {
			continue;
		}
		used[value] = true;

		Parse(path[value]);
		Parse(value, path[value], used);
	}
}

ParserJSON::~ParserJSON() {
	for (BaseArgumentJSON* value : arguments_json_) {
		delete value;
	}
}
#pragma once

#include <string_view>
#include <string>
#include <vector>
#include <functional>

class BaseArgumentJSON {
public:
	explicit BaseArgumentJSON(std::string_view name_argument);
	std::string_view GetNameArgument();
	virtual ~BaseArgumentJSON() = default;
private:
	std::string_view name_;
};

class VectorStringArgumentJSON : public BaseArgumentJSON {
public:
	VectorStringArgumentJSON() = default;
	VectorStringArgumentJSON(std::vector<std::string_view>& data, std::string_view name_argument);
	void AddElement(std::string_view value);
	const std::string_view& operator[](size_t index) const;
private:
	std::vector<std::string_view>& data_;
};

class VariableIntArgumentJSON : public BaseArgumentJSON {
public:
	VariableIntArgumentJSON() = default;
	VariableIntArgumentJSON(int& data, std::string_view name_argument);
	void SetElement(const int& value);
private:
	int& data_;
};

class VariableDoubleArgumentJSON : public BaseArgumentJSON {
public:
	VariableDoubleArgumentJSON() = default;
	VariableDoubleArgumentJSON(double& data, std::string_view name_argument);
	void SetElement(const double& value);
private:
	double& data_;
};

class VectorDoubleArgumentJSON : public BaseArgumentJSON {
public:
	VectorDoubleArgumentJSON() = default;
	VectorDoubleArgumentJSON(std::vector<double>& data, std::string_view name_argument);
	void AddElement(const double& value);
	const double& operator[](size_t index) const;
private:
	std::vector<double>& data_;
};
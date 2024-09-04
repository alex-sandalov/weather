#pragma once

#include "ArgumentJSON.h"
#include "../ParserAPI/ParserAPI.h"

using namespace Parser;

#include <nlohmann/json.hpp>

#include <unordered_map>
#include <string_view>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;
using json = nlohmann::json;

/*
 * Модуль для работы с парсером JSON.
 * 
 * Пример использования:
 * Есть JSON {"chat": { "id": 1 } }. Задача: получить id.
 * 
 * Для решения этой задачи используется метод `AddIntVariable`. Создаётся переменная `json_id`,
 * и определяется путь до `id` (в данном примере это `{"main", "chat"}`, "main" - главный путь,
 * определяется при создании ParserJSON). Затем запускается метод `AddIntVariable(json_id, {"main", "chat"})`.
 * После выполнения работы метода `Parse` значение `id` будет доступно в переменной `json_id`.
 *
 * Метод `AddIntVariable` принимает два аргумента: переменную, в которую будет сохранено значение,
 * и путь к этому значению в JSON. Например, `AddIntVariable(json_id, {"main", "chat"})` означает,
 * что значение `id` будет сохранено в переменную `json_id`, и путь к нему в JSON составляет `{"main", "chat"}`.
 */
class ParserJSON {
public:
	ParserJSON(const fs::path& file_, std::string_view main_path);
	ParserJSON(const json& response_json, std::string_view main_path);
	void Parse();
	ParserJSON& AddIntVariable(int& reference_variable, std::string_view name_argument,
								const std::vector<std::string_view>& path) noexcept;
	ParserJSON& AddDoubleVariable(double& reference_variable, std::string_view name_argument,
								const std::vector<std::string_view>& path) noexcept;
	ParserJSON& AddStringArray(std::vector<std::string_view>& reference_array, std::string_view name_argument,
								const std::vector<std::string_view>& path) noexcept;
	ParserJSON& AddDoubleArray(std::vector<double>& reference_array, std::string_view name_argument,
								const std::vector<std::string_view>& path) noexcept;
	~ParserJSON();
private:
	void SetPath(const std::vector<std::string_view>& path) noexcept;
	void Parse(const json& json_path) noexcept;
	void Parse(std::string_view path_name, const json& path, std::unordered_map<std::string, bool> used) noexcept;

	std::ifstream file_;
	json response_json_;
	std::string_view main_path_;
	std::vector<BaseArgumentJSON*> arguments_json_;
	std::unordered_map<std::string, std::vector<std::string>> path_json_;
};
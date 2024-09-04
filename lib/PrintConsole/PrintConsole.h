#pragma once

#include "ftxui/component/captured_mouse.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/color.hpp"
#include "ftxui/component/event.hpp"
#include "ftxui/component/loop.hpp"

#include "../ParserAPI/ParserAPI.h"

#include <unordered_map>
#include <string_view>
#include <array>
#include <chrono>
#include <functional>

using namespace ftxui;
using namespace Parser;

/*
 * Решил парсить сразу 16 дней, потому что есть ограничения в кол-ве запросов
 */
namespace Print {

using WeatherInfoForOneCity = std::vector<std::vector<double>>;

static const int kCountMaxWeatherDay = 16;
static const int kCountMinWeatherDay = 1;

struct Config {
	int frequency;
	int days;
	std::vector<std::string_view> name_city;
	std::vector<std::string_view> api_key;

	Config();
};

struct Coordinate {
	double latitude;
	double longitude;

	Coordinate();
};

class PrintConsole {
public:
	static std::unordered_map<int, std::string_view> kWeatherCode;
	static const std::vector<std::string_view> kTimeDay;
	static const int kSizeBox;
	static const int kCountMaxDay;
	static const int kCountMinDay;
	static const std::string kButtonAddDay;
	static const std::string kButtonRemoveDay;
	static const std::string kButtonNextCity;
	static const std::string kButtonPredCity;

	PrintConsole() = delete;
	explicit PrintConsole(const Config& config);
	void AddInfoCity(std::string_view name_city, const WeatherInfoForOneCity& info);
	void StartDraw();
private:
	std::vector<Element> CreateDescriptionDay(std::string_view name_city, size_t index_day,
											size_t index_time_day);
	void ClearScreen();

	Config config_;
	std::unordered_map<std::string, WeatherInfoForOneCity> info_city_;
	std::vector<std::string_view> name_city_;
};

}
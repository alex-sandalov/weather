#pragma once

#include "../ParserJSON/ParserJSON.h"
#include "../ParserAPI/ParserAPI.h"
#include "../PrintConsole/PrintConsole.h"

using namespace Print;
using namespace Parser;

struct WrapperWeatherInfo {
	std::vector<double> temperature_2m;
	std::vector<double> relativehumidity_2m;
	std::vector<double> apparent_temperature;
	std::vector<double> weathercode;
	std::vector<double> windspeed_10m;
};

class ParserWeather {
public:
	static const int kCountBaseDay;
	static const int kStepWeatherInfo;

	explicit ParserWeather(const Config& config);
	std::optional<WeatherInfoForOneCity> GetWeather(std::string_view city);
private:
	Config config_;
};
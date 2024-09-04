#include <iostream>

#include "ParserWeather/ParserWeather.h"

using namespace Print;
using namespace Parser;

int main(int argc, char** argv) {
	if (argc == 1) {
		std::cout << "Error open config.json\n";
	}

	fs::path config_path {argv[1]};

	Config config;
	ParserJSON config_parse(config_path, "main");

	std::vector<std::string_view> path_json = {"main"};
	config_parse.AddIntVariable(config.frequency, "frequency", path_json);
	config_parse.AddIntVariable(config.days, "days", path_json);
	config_parse.AddStringArray(config.name_city, "city", path_json);
	config_parse.AddStringArray(config.api_key, "api_key", path_json);
	config_parse.Parse();

	ParserWeather data_weather(config);
	PrintConsole print(config);

	for (std::string_view city : config.name_city) {
		std::optional<WeatherInfoForOneCity> info_city = data_weather.GetWeather(city);
		if (!info_city) {
			std::cerr << "parsing error";
			return 0;
		}
		print.AddInfoCity(city, info_city.value());
	}

	print.StartDraw();	
}
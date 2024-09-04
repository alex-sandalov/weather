#include "ParserWeather.h"

const int ParserWeather::kCountBaseDay = 16;
const int ParserWeather::kStepWeatherInfo = 6;

ParserWeather::ParserWeather(const Config& config)
	: config_(config)
{}

std::optional<WeatherInfoForOneCity> ParserWeather::GetWeather(std::string_view city) {
	std::optional<cpr::Response> response_optional_coordinate = MakeCoordinateRequest(
																"https://api.api-ninjas.com/v1/city",
																AsString(city),
																AsString(config_.api_key[0]));
	if (!response_optional_coordinate) {
		return std::nullopt;
	}
	cpr::Response response_coordinate = response_optional_coordinate.value();
	json coordinate = json::parse(response_coordinate.text.substr(1, response_coordinate.text.size() - 2));

	ParserJSON coordinate_parser(coordinate, "main");
	Coordinate coordinate_struct;
	coordinate_parser.AddDoubleVariable(coordinate_struct.latitude, "latitude", {"main"});
	coordinate_parser.AddDoubleVariable(coordinate_struct.longitude, "longitude", {"main"});
	coordinate_parser.Parse(); 

	std::optional<cpr::Response> response_optional_weather = MakeWeatherRequest(
															"https://api.open-meteo.com/v1/forecast",
															std::to_string(coordinate_struct.latitude),
															std::to_string(coordinate_struct.longitude),
															std::to_string(kCountBaseDay));
	if (!response_optional_weather) {
		return std::nullopt;
	}
	cpr::Response response_weather = response_optional_weather.value();
	json weather = json::parse(response_weather.text);

	WrapperWeatherInfo wrapper_weather_info;
	ParserJSON wearher_parser(weather, "main");
	wearher_parser.AddDoubleArray(wrapper_weather_info.temperature_2m, "temperature_2m", {"main", "hourly"});
	wearher_parser.AddDoubleArray(wrapper_weather_info.relativehumidity_2m, "relativehumidity_2m", {"main", "hourly"});
	wearher_parser.AddDoubleArray(wrapper_weather_info.apparent_temperature, "apparent_temperature", {"main", "hourly"});
	wearher_parser.AddDoubleArray(wrapper_weather_info.weathercode, "weathercode", {"main", "hourly"});
	wearher_parser.AddDoubleArray(wrapper_weather_info.windspeed_10m, "windspeed_10m", {"main", "hourly"});
	wearher_parser.Parse();

	WeatherInfoForOneCity weather_info;
	for (size_t i = 0; i < wrapper_weather_info.temperature_2m.size(); i += kStepWeatherInfo) {
		std::vector<double> data_weather = {wrapper_weather_info.temperature_2m[i],
										wrapper_weather_info.relativehumidity_2m[i],
										wrapper_weather_info.apparent_temperature[i],
										wrapper_weather_info.weathercode[i],
										wrapper_weather_info.windspeed_10m[i]};
		weather_info.push_back(data_weather);
	}

	return weather_info;
}
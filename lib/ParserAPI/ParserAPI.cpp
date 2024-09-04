#include "ParserAPI.h"

std::optional<cpr::Response> Parser::MakeWeatherRequest(std::string_view link, const std::string& latitude, 
											const std::string& longitude, const std::string& days) {
	cpr::Response request = cpr::Get(cpr::Url{AsString(link)},
				cpr::Parameters{
					{"latitude", latitude}, 
					{"longitude", longitude},
					{"forecast_days", days},
					{"hourly", "temperature_2m,relativehumidity_2m,apparent_temperature,weathercode,windspeed_10m"},
					{"timezone", "auto"},
					{"daily", "weathercode"}
				}
			);
	if (request.status_code == kStatusCodeOK) {
		return request;
	}
	return std::nullopt;
}

std::optional<cpr::Response> Parser::MakeCoordinateRequest(std::string_view link, 
						const std::string& city, const std::string& api_key) {
	cpr::Response request =  cpr::Get(cpr::Url{AsString(link)},
				cpr::Parameters{{"name", city}},
				cpr::Header{{"X-api-Key", api_key}});
	if (request.status_code = kStatusCodeOK) {
		return request;
	}
	return std::nullopt;
}


#pragma once

#include <cpr/cpr.h>
#include <string_view> 
#include <optional>

namespace Parser {

inline std::string AsString(std::string_view v) { 
    return {v.data(), v.size()}; 
}

const int kStatusCodeOK = 200;

std::optional<cpr::Response> MakeWeatherRequest(std::string_view link, const std::string& latitude,
												const std::string& longitude, const std::string& days);

std::optional<cpr::Response> MakeCoordinateRequest(std::string_view link, const std::string& city, const std::string& api_key);

};
add_library(
    WeatherLibrary
    ParserJSON/ParserJSON.cpp
	ParserJSON/ArgumentJSON.cpp
	ParserAPI/ParserAPI.cpp
	PrintConsole/PrintConsole.cpp
	ParserWeather/ParserWeather.cpp
)

target_link_libraries(
	WeatherLibrary 
	PRIVATE cpr::cpr
	PRIVATE nlohmann_json::nlohmann_json
	PRIVATE ftxui::dom ftxui::screen ftxui::component
)

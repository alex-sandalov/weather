add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(
	${PROJECT_NAME}
	PRIVATE WeatherLibrary 
	PRIVATE cpr::cpr
	PRIVATE nlohmann_json::nlohmann_json
	PRIVATE ftxui::dom ftxui::screen ftxui::component
)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/lib)
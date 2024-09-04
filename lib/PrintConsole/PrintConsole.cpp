#include "PrintConsole.h"

using namespace Print;

std::unordered_map<int, std::string_view> PrintConsole::kWeatherCode = {
	{0, "Sunny"},
	{1, "Mainly clear"}, {2, "Party cloudy"}, {3, "Overcast"},
	{45, "Fog"}, {48, "Fog"},
	{51, "Light drizzle"}, {53, "Moderate drizzle"}, {55, "Dense drizzle"},
	{61, "Slight rain"}, {63, "Moderate rain"}, {65, "Heavy rain"},
	{66, "Light Freezing Rain"}, {67, "Heavy Freezing Rain"},
	{71, "Slight snowfall"}, {73, "Moderate snowfall"}, {75, "Heavy snowfall"},
	{77, "Snow grains"},
	{80, "Slight rain showers"}, {81, "Moderate rain showers"}, {82, "Violent rain showers"},
	{85, "Slight snow showers"}, {86, "Heavy snow showers"},
	{95, "Thunderstorm"}, {96, "Thunderstorm"}, {99, "Thunderstorm"}
};
const std::vector<std::string_view> PrintConsole::kTimeDay = {"Night", "Morning", "Noon", "Evening"};
const int PrintConsole::kSizeBox = 80;
const int PrintConsole::kCountMaxDay = 16;
const int PrintConsole::kCountMinDay = 1;
const std::string PrintConsole::kButtonAddDay = "0";
const std::string PrintConsole::kButtonRemoveDay = "-";
const std::string PrintConsole::kButtonNextCity = "n";
const std::string PrintConsole::kButtonPredCity = "p";

Config::Config()
	: frequency(0)
	, days(0)
{}

Coordinate::Coordinate()
	: latitude(0)
	, longitude(0)
{}

PrintConsole::PrintConsole(const Config& config)
	: config_(config)
{}

void PrintConsole::AddInfoCity(std::string_view name_city, const WeatherInfoForOneCity& info) {
	info_city_[AsString(name_city)] = info;
	name_city_.push_back(name_city);
}

std::vector<Element> PrintConsole::CreateDescriptionDay(std::string_view name_city, size_t index_day,
														size_t index_time_day)
{
	WeatherInfoForOneCity weather_forecast = info_city_[AsString(name_city)];
	std::vector<double> weather_time_day = weather_forecast[index_day];
	
	std::vector<Element> description;

	description.push_back(text(
		AsString(kWeatherCode[weather_time_day[3]])
	));

	description.push_back(text(
		std::to_string(weather_time_day[0]) + " °C"
	));

	description.push_back(text(
		std::to_string(weather_time_day[1]) + " %"
	));

	description.push_back(text(
		std::to_string(weather_time_day[2]) + " °C"
	));

	description.push_back(text(
		std::to_string(weather_time_day[4]) + " km/h"
	));

	return description;
}

void PrintConsole::StartDraw() {
	auto screen = ScreenInteractive::TerminalOutput();
	size_t count_city = 0;

	auto component = Renderer([&] {
		Elements windows;
		std::string_view name_city = name_city_[count_city];

		for (size_t day = 0; day < config_.days; ++day) {
			Elements row;
			row.reserve(kTimeDay.size());
			for (size_t i = 0; i < kTimeDay.size(); ++i) {
				row.push_back(vbox({
					text(AsString(kTimeDay[i])) | center | bold,
					separator(),
					hbox({
						vbox(CreateDescriptionDay(name_city, day, i))
					})
				}) | border | size(WIDTH, EQUAL, kSizeBox));
			}

			windows.push_back(
				window(
					text(AsString(name_city)) | center,
					vbox(hbox(row))
				)
			);
		}

		ClearScreen();
		return vbox(windows);
	});

	component = CatchEvent(component, [&](const Event& event) {
		if (event == Event::Escape) {
			screen.ExitLoopClosure()();
		} else if (event.input() == kButtonRemoveDay && config_.days > kCountMinDay) {
			--config_.days;
		} else if (event.input() == kButtonAddDay && config_.days < kCountMaxDay) {
			++config_.days;
		} else if (event.input() == kButtonNextCity && count_city + 1 < name_city_.size()) {
			++count_city;
		} else if (event.input() == kButtonPredCity && count_city > 0) {
			--count_city;
		}

		return false;
	});

	Loop loop(&screen, component);
	auto last_time = std::chrono::steady_clock::now();
	while (!loop.HasQuitted()) {
		auto current_time = std::chrono::steady_clock::now();
		if (std::chrono::duration_cast<std::chrono::seconds>(current_time - last_time).count() > config_.frequency) {
			last_time = current_time;
			screen.Post(ftxui::Event::Custom);
		}
		loop.RunOnce();
	}
}

void PrintConsole::ClearScreen() {
#ifdef WINDOWS
	std::system("cls");
#else
	std::system("clear");
#endif
}
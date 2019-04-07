#pragma once

#include "utils/vector.hpp"
#include "map.hpp"
#include <array>

namespace Game {

	enum Activity {
		Work,
		Hobby,
		Home,
		ACTIVITY_COUNT
	};

	constexpr std::array<float, ACTIVITY_COUNT> TIME_TABLE =
	{
		0.f, 8.f/ 24.f, 16.f / 24.f
	};

	struct Actor
	{
		// personal properties
		std::array<Math::Vec2I, ACTIVITY_COUNT> activityLocations;
		float wakeUpTime;
	
		// current state
		Math::Vec2 position;
		Math::Vec2 positionNoise;
		Activity currentActivity;
		Path currentPath;
		float politic, activity, satisfaction, health;
	};

	struct PopulationClass
	{
		Map::Tile::Info::Quality home;
		std::vector<int> income;
		std::vector<Map::Tile::Info::Hobby> hobbies;
	};

	const extern PopulationClass LOWER_CLASS;
	const extern PopulationClass MIDDLE_CLASS;
	const extern PopulationClass UPPER_CLASS;

	constexpr float LOWER_CLASS_RATIO = 0.25f;
	constexpr float MIDDLE_CLASS_RATIO = 0.6f;
	constexpr float UPPER_CLASS_RATIO = 0.15f;
}
#pragma once

#include "utils/vector.hpp"
#include "map.hpp"
#include <array>

namespace Game {

	enum Activity {
		Home,
		Work,
		Hobby,
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
		Activity currentActivity;
		Path currentPath;
	};

}
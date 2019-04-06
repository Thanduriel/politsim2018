#pragma once

#include "utils/vector.hpp"
#include "map.hpp"

namespace Game {

	enum struct Goal {
		Home,
		Work,
		Hobby
	};

	struct Actor
	{
		// personal properties
		Math::Vec2I home;
		Math::Vec2I workPlace;
		Math::Vec2I hobbyPlace;
		float wakeUpTime;
	
		// current state
		Math::Vec2 position;
		Goal currentGoal;
		Path currentPath;
	};

}
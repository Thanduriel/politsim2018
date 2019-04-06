#pragma once

#include "actor.hpp"
#include "utils/random.hpp"
#include <vector>

namespace Game {

	// 1 is 24h
	using TimeOfDay = float;

	class World
	{
	public:
		World(std::string_view _mapFile, float _timeFactor, int _numActors);

		void Update(float _deltaTime);


	private:
		Math::Vec2I PositionToIndex(Math::Vec2 _position) const;
		// gives position of a tile's center
		Math::Vec2 IndexToPosition(Math::Vec2I _index) const;

		TimeOfDay m_time;
		float m_timeFactor;
		int m_day;

		float m_tileSize;

		std::vector<Actor> m_actors;
		Utils::RandomGenerator m_randomGenerator;
	};

}
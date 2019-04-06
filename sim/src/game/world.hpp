#pragma once

#include "actor.hpp"
#include "map.hpp"
#include "utils/random.hpp"
#include <vector>

namespace Game {

	class World
	{
	public:
		World(std::string_view _mapFile, float _timeFactor, int _numActors);

		void Update(float _deltaTime);


	private:
		Math::Vec2I PositionToIndex(Math::Vec2 _position) const;
		// gives position of a tile's center
		Math::Vec2 IndexToPosition(Math::Vec2I _index) const;

		float m_time;
		float m_timeFactor;
		int m_day;

		float m_tileSize;

		std::vector<Actor> m_actors;
		Map m_map;
		Utils::RandomGenerator m_randomGenerator;
	};

}
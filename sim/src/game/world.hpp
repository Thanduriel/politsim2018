#pragma once

#include "actor.hpp"
#include "map.hpp"
#include "utils/random.hpp"
#include <vector>

namespace Game {

	class World
	{
	public:
		World();

		void Init(Map&& _map);

		void Update(float _deltaTime);

		const std::vector<Actor>& GetActors() const { return m_actors; }

		std::vector<const Actor*>&& GetNear(const Math::Vec2& _position, float _maxDis) const;

		int GetMoney() const { return m_money; }

		float GetPoliticBar() const { return m_politicBar; }
	private:
		Math::Vec2I PositionToIndex(Math::Vec2 _position) const;
		// gives position of a tile's center
		Math::Vec2 IndexToPosition(Math::Vec2I _index) const;

		Actor GenerateActor();

		float m_time;
		float m_timeFactor;
		int m_day;

		float m_tileSize;

		std::vector<Actor> m_actors;
		Map m_map;
		Utils::RandomGenerator m_randomGenerator;
		int m_money;
		float m_politicBar;
	};

}

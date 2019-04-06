#include "world.hpp"

namespace Game {

	using namespace Math;

	World::World(std::string_view _mapFile, float _timeFactor, int _numActors)
		: m_time(0),
		m_timeFactor(_timeFactor),
		m_day(0),
		m_randomGenerator(Utils::RandomSeed())
	{
	}

	void World::Update(float _deltaTime)
	{
		m_time += _deltaTime * m_timeFactor;
		if (m_time >= 1.f)
		{
			++m_day;
			m_time -= 1.f;
		}

		for (Actor& actor : m_actors)
		{
			// reached next tile
			if (actor.currentPath.size() && PositionToIndex(actor.position) == actor.currentPath.back())
				actor.currentPath.pop_back();
			// currently on the way
			if (actor.currentPath.size())
			{
				const Vec2 targetPos = IndexToPosition(actor.currentPath.back());
				const Vec2 dir = (targetPos - actor.position).Normalized();
				const Vec2 noise = m_randomGenerator.Direction()* 0.2f;

				actor.position += (dir + noise).Normalized() * _deltaTime;
			}
			else if(actor.wakeUpTime)
			{

			}
		}
	}

	Vec2I World::PositionToIndex(Vec2 _position) const
	{
		return Vec2I(_position / m_tileSize);
	}

	Vec2 World::IndexToPosition(Vec2I _index) const
	{
		return Vec2(_index) * m_tileSize + Vec2(m_tileSize) * 0.5f;
	}
}
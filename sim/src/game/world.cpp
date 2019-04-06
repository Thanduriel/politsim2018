#include "world.hpp"
#include "utils/time.hpp"
#include <iostream>

namespace Game {

	using namespace Math;

	World::World()
		: m_time(0),
		m_day(0),
		m_randomGenerator(Utils::RandomSeed()),
		m_tileSize(0.25)
	{
		
	}

	void World::Init(Map&& _map)
	{
		m_map = std::move(_map);
		m_timeFactor = 1.f / 60.f;
		const int numActors = 512;

		m_actors.reserve(numActors);

		for (int i = 0; i < numActors; ++i)
		{
			m_actors.push_back(GenerateActor());
		}
	}

	void World::Update(float _deltaTime)
	{
		m_time += _deltaTime * m_timeFactor * 5.f;
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
			// switch activity
			else if(Activity next = static_cast<Activity>((actor.currentActivity + 1) % ACTIVITY_COUNT); 
				m_time > Utils::TimeOfDay(actor.wakeUpTime + TIME_TABLE[next] ))
			{
				std::cout << "switching to activity " << next << " at time " << m_time << "\n";
				actor.currentActivity = next;
				const Vec2I curInd = PositionToIndex(actor.position);
				actor.currentPath = m_map.ComputePath(curInd, actor.activityLocations[next]);
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

	Actor World::GenerateActor()
	{
		auto RandVec = [this]()
		{
			return Math::Vec2I(m_randomGenerator.Uniform(0u, 15u), m_randomGenerator.Uniform(0u, 15u));
		};

		Actor actor{};
		actor.activityLocations = { RandVec(), RandVec(), RandVec() };
		actor.position = IndexToPosition(actor.activityLocations[Activity::Home]);
		actor.wakeUpTime = m_randomGenerator.Uniform(4.f/24.f, 1.f);
		actor.currentActivity = Activity::Home;

		return actor;
	}
}
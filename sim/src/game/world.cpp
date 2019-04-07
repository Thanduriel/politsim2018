#include "world.hpp"
#include "utils/time.hpp"
#include <cassert>
#include <iostream>

namespace Game {

	using namespace Math;

	constexpr float MOVEMENT_SPEED = 0.4f;

	World::World()
		: m_time(0),
		m_day(0),
		m_randomGenerator(Utils::RandomSeed()),
		m_tileSize(0.25),
		m_money(1337),
		m_politicBar(0.3f) { }

	void World::Init(Map&& _map)
	{
		m_map = std::move(_map);
		m_timeFactor = 1.f / 60.f;
		const int numActors = 256;
		const int numLowerClass = static_cast<int>(numActors * LOWER_CLASS_RATIO);
		const int numMiddleClass = static_cast<int>(numActors * MIDDLE_CLASS_RATIO);
		const int numUpperClass = static_cast<int>(numActors * UPPER_CLASS_RATIO);
		m_actors.reserve(numActors);

		PopulationPlaces places;

		places = m_map.GetPlaces(LOWER_CLASS);
		for (int i = 0; i < numLowerClass; ++i)
			m_actors.push_back(GenerateActor(places));

		places = m_map.GetPlaces(MIDDLE_CLASS);
		for (int i = 0; i < numMiddleClass; ++i)
			m_actors.push_back(GenerateActor(places));

		places = m_map.GetPlaces(UPPER_CLASS);
		for (int i = 0; i < numUpperClass; ++i)
			m_actors.push_back(GenerateActor(places));
	}

	void World::Update(float _deltaTime)
	{
		m_time += _deltaTime * m_timeFactor;
		if (m_time >= 1.f)
		{
			++m_day;
			m_time -= 1.f;
			std::cout << "new day " << m_day << "\n";
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

				actor.position += (dir + noise).Normalized() * _deltaTime * MOVEMENT_SPEED;
			}
			// switch activity
			else if(Activity next = static_cast<Activity>((actor.currentActivity + 1) % ACTIVITY_COUNT); 
				m_time > Utils::TimeOfDay(actor.wakeUpTime + TIME_TABLE[next]) )
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
		return Vec2(_index) * m_tileSize + Vec2(m_tileSize * 0.5f);
	}

	Actor World::GenerateActor(const PopulationPlaces& _places)
	{
		assert(_places.hobby.size()); 
		assert(_places.work.size());
		assert(_places.home.size());
		auto RandVec = [this](const std::vector<Vec2I>& _elements)
		{
			return _elements[m_randomGenerator.Uniform(0u, (unsigned)_elements.size()-1)];
		};

		Actor actor{};
		actor.activityLocations = { RandVec(_places.work), RandVec(_places.hobby), RandVec(_places.home) };
		actor.position = IndexToPosition(actor.activityLocations[Activity::Home]);
		actor.wakeUpTime = m_randomGenerator.Uniform(0u, 10u) > 1u ?
			m_randomGenerator.Normal(0.5f / 24.f) + 7.f / 24.f
			: m_randomGenerator.Normal(0.5f / 24.f) + 22.f / 24.f;
		actor.wakeUpTime = Utils::TimeOfDay(actor.wakeUpTime);

		// determine current activity
		float timeDif = m_time - actor.wakeUpTime;
		if (timeDif < 0) timeDif += 1.f;
		for(int i = 0; i >= 0; ++i)
			if (timeDif > TIME_TABLE[i])
			{
				actor.currentActivity = static_cast<Activity>(i);
				break;
			}

		return actor;
	}

	std::vector<const Actor*>&& Game::World::GetNear(const Math::Vec2& _position, float _maxDis) const {
		float _maxDisSq = _maxDis * _maxDis;
		std::vector<const Actor*> res(0);
		for (const Actor& a : m_actors) {
			if ((a.position - _position).LenSqr() <= _maxDisSq)
				res.push_back(&a);
		}
		return std::move(res);
	}
}

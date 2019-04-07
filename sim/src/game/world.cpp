#include "world.hpp"
#include "utils/time.hpp"
#include <cassert>
#include <iostream>
#include <random>

namespace Game {

	using namespace Math;

	constexpr float MOVEMENT_SPEED = 0.4f;

	World::World()
		: m_time(0),
		m_day(0),
		m_randomGenerator(Utils::RandomSeed()),
		m_tileSize(0.25),
		m_money(1337),
		m_politicBar(0.5f) { }

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

			// re-evaluate politicbar
			double pol;
			double dominantParty = m_politicBar > 0.5 ? 1.0 : 0.0;

			for (Actor& actor : m_actors) {
				if (actor.politic <= 0.35) { /* pol += 0; */ }
				else if (actor.politic >= 0.65) pol += 1;
				else if (actor.satisfaction >= 0.5) pol += dominantParty;
				else pol += (1 - dominantParty);
			
			}
			m_politicBar = pol / m_actors.size();
		}
		m_actorsInBuilding.resize(0);
		m_actorsOnStreet.resize(0);
		for (Actor& actor : m_actors)
		{
			// reached next tile
			if (actor.currentPath.size() && PositionToIndex(actor.position) == actor.currentPath.back())
			{
				actor.currentPath.pop_back();
				if (!actor.currentPath.size())
				{
					const Vec2 center = IndexToPosition(PositionToIndex(actor.position));
					const Vec2 noise(m_randomGenerator.Uniform(-0.125f, 0.125f), m_randomGenerator.Uniform(-0.125f, 0.125f));
					actor.positionNoise = center + noise;
				}
			}
			// currently on the way
			if (actor.currentPath.size())
			{
				const Vec2 targetPos = IndexToPosition(actor.currentPath.back());
				const Vec2 dir = (targetPos - actor.position).Normalized();
				const Vec2 noise = m_randomGenerator.Direction()* 0.4f;

				actor.position += (dir + noise).Normalized() * _deltaTime * MOVEMENT_SPEED;
			}
			// switch activity
			else if(Activity next = static_cast<Activity>((actor.currentActivity + 1) % ACTIVITY_COUNT); 
				std::abs(m_time - Utils::TimeOfDay(actor.wakeUpTime + TIME_TABLE[next])) < 0.1f / 24.f)
			{
			//	std::cout << "switching to activity " << next << " at time " << m_time << "\n";
				actor.currentActivity = next;
				const Vec2I curInd = PositionToIndex(actor.position);
				actor.currentPath = m_map.ComputePath(curInd, actor.activityLocations[next]);
			}
			// just move around a little on the current tile
			else
			{
				const Vec2 dir = (actor.positionNoise) - actor.position;
				const float l = dir.Len();
				if (l > 0.01f) actor.position += dir / l * 0.1f * _deltaTime;
			}

			if (actor.currentPath.size() == 0) {
				m_actorsInBuilding.push_back(&actor);
			} else {
				m_actorsOnStreet.push_back(&actor);
			}

			UpdateActor(actor, _deltaTime);
		}
		
		std::sort(m_actorsOnStreet.begin(), m_actorsOnStreet.end(), Game::ActorUpdate::SortedAxisCompare);
		std::sort(m_actorsInBuilding.begin(), m_actorsInBuilding.end(), Game::ActorUpdate::TileSortCompare);

		for (auto itr1 = m_actorsOnStreet.begin(); itr1 != m_actorsOnStreet.end(); ++itr1) {
			for (auto itr2 = itr1 + 1;
				itr2 != m_actorsOnStreet.end() 
				&& (*itr1)->position.x + Game::ActorUpdate::InteractionDisSqr < (*itr2)->position.x;
				++itr2) {
				if (((*itr1)->position - (*itr2)->position).LenSqr() <= Game::ActorUpdate::InteractionDisSqr)
					Interaction(**itr1, **itr2, _deltaTime);
			}
		}
		for (auto itr1 = m_actorsInBuilding.begin(); itr1 != m_actorsInBuilding.end(); ++itr1) {
			for (auto itr2 = itr1 + 1; itr2 != m_actorsInBuilding.end() && (*itr1)->position != (*itr2)->position; ++itr2)
				Interaction(**itr1, **itr2, _deltaTime);
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
		std::normal_distribution<float> daySchedule(6.5f / 24.f, 2.5f / 24.f);
		std::normal_distribution<float> nightSchedule(21.f / 24.f, 2.5f / 24.f);

		Actor actor{};
		actor.activityLocations = { RandVec(_places.work), RandVec(_places.hobby), RandVec(_places.home) };
		actor.position = IndexToPosition(actor.activityLocations[Activity::Home]);
		actor.positionNoise = actor.position;
		actor.wakeUpTime = m_randomGenerator.Uniform(0u, 10u) > 1u ?
			daySchedule(m_randomGenerator)
			: nightSchedule(m_randomGenerator);
		actor.wakeUpTime = Utils::TimeOfDay(actor.wakeUpTime);

		// determine current activity
		float timeDif = m_time - actor.wakeUpTime;
		if (timeDif < 0) timeDif += 1.f;
		for(int i = 2; i >= 0; --i)
			if (timeDif > TIME_TABLE[i])
			{
				actor.currentActivity = static_cast<Activity>(i);
				break;
			}

		return actor;
	}

	std::vector<const Actor*>&& Game::World::GetNearStreet(const Math::Vec2& _position, float _maxDis) const {
		float _maxDisSq = _maxDis * _maxDis;
		std::vector<const Actor*> res(0);
		for (Actor*const & a : m_actorsOnStreet) {
			if ((a->position - _position).LenSqr() <= _maxDisSq)
				res.push_back(a);
		}
		return std::move(res);
	}
}

bool Game::ActorUpdate::SortedAxisCompare(const Actor* ac1, const Actor* ac2) {
	return (ac1->position.x < ac2->position.x);
}

int calcId(const Math::Vec2I& vec) {
	return vec.y * 100 + vec.x;
}
bool Game::ActorUpdate::TileSortCompare(const Actor* ac1, const Actor* ac2) {
	return (calcId(ac1->activityLocations[ac1->currentActivity])
		< calcId(ac2->activityLocations[ac2->currentActivity]));
}

template<typename T>
T capAdd(T value, T diff, T max = 1, T min = 0) {
	T res = value + diff;
	if (res > max) res = max;
	else if (res < min) res = min;
	return res;
}
void Game::World::Interaction(Actor& act1, Actor& act2, float dTime) {
	// ähliche meinung
	if (abs(act1.politic - act2.politic) <= 0.4f) {
		float mean = (act1.politic + act2.politic) / 2;
		// act1.politic = capAdd(act1.politic, );
	}
}
void Game::World::UpdateActor(Actor& act, float dTime) {
	int income = m_map.Get(act.activityLocations[Activity::Work]).info.income;
	
	// unzufriedenheit -> activ
	act.activity = capAdd(act.activity, (.7f - act.satisfaction) * 0.1f * dTime);
	// hoppy -> zufrieden
	if (PositionToIndex(act.position) == (act.activityLocations[Activity::Hobby]))
		act.satisfaction = capAdd(act.satisfaction, 0.1f * dTime);
	// schlafen -> gesundheit
	if (PositionToIndex(act.position) == (act.activityLocations[Activity::Home]))
		act.health = capAdd(act.health, 0.2f * dTime);
	// Arbeitt -> activität sinkt
	if (PositionToIndex(act.position) == (act.activityLocations[Activity::Work]))
		act.activity = capAdd(act.activity, 0.2f * dTime);
}

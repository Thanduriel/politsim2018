#include "map.hpp"
#include "utils/random.hpp"
#include "actor.hpp"
#include <limits>
#include <iostream>
#include <queue>
#include <algorithm>

namespace Game {

	using namespace Math;

	Map::Map(int width, int height, std::vector<Tile>&& tiles)
		: m_size(width, height),
		m_tiles(std::move(tiles)),
		m_flags(m_tiles.size()) 
	{
		for (int y = 0; y < m_size.y; ++y)
		{
			for (int x = 0; x < m_size.x; ++x)
				std::cout << static_cast<int>(m_tiles[x + y * m_size.x].type) << " ";

			std::cout << "\n";
		}
	}

	const Map::Tile& Map::Get(Math::Vec2I _index) const
	{
		return m_tiles[_index.x + _index.y * m_size.x];
	}

	bool Map::IsInside(Math::Vec2I _index) const
	{
		return _index.x >= 0 && _index.x < m_size.x
			&& _index.y >= 0 && _index.y < m_size.y;
	}

	Path Map::ComputePath(Math::Vec2I _begin, Math::Vec2I _end) const
	{
		using namespace Utils;
		// reset buffer
		for (TileFlag& flag : m_flags) flag.distance = std::numeric_limits<int>::max();

		auto cmp = [&](const Vec2I& _lhs, const Vec2I& _rhs)
		{
			return m_flags[GetIndex(_lhs)].distance > m_flags[GetIndex(_rhs)].distance;
		};
		std::priority_queue<Vec2I, std::vector<Vec2I>, decltype(cmp)> queue(cmp);
		auto AddOption = [&](Vec2I _index, Direction _dir, int _dist)
		{
			if (!IsInside(_index)) return;

			const int index = GetIndex(_index);
			const Tile& tile = m_tiles[index];
			const Tile tileBegin = Get(_begin);
			const Tile tileEnd = Get(_end);
			if ((tile.type == Tile::Type::Street 
				|| (tile.type == tileBegin.type && tile.info.income == tileBegin.info.income) 
				|| (tile.type == tileEnd.type && tile.info.income == tileEnd.info.income))
				&& _dist < m_flags[index].distance)
			{
				m_flags[index].distance = _dist;
				m_flags[index].dir = _dir;
				queue.push(_index);
			}
				
		};

		m_flags[GetIndex(_begin)].distance = 0;
		queue.push(_begin);

		while (queue.size())
		{
			const Vec2I current = queue.top();
			queue.pop();
			if (current == _end)
				break;
			int dist = m_flags[GetIndex(current)].distance + 1;

			AddOption(current + Vec2I(1, 0), Right, dist);
			AddOption(current + Vec2I(-1, 0), Left, dist);
			AddOption(current + Vec2I(0, 1), Up, dist);
			AddOption(current + Vec2I(0, -1), Down, dist);
		}

		// build path
		Path path;

		Vec2I current = _end;
		auto v = m_flags[GetIndex(current)].distance;
		if (m_flags[GetIndex(current)].distance == std::numeric_limits<int>::max())
		{
			return path;
		}
		while (current != _begin)
		{
			path.push_back(current);
			switch (m_flags[GetIndex(current)].dir)
			{
			case Up: current -= Vec2I(0, 1); break;
			case Down: current -= Vec2I(0, -1); break;
			case Left: current -= Vec2I(-1, 0); break;
			case Right: current -= Vec2I(1, 0); break;
			}
		}

		return path;
	}

	PopulationPlaces Map::GetPlaces(const PopulationClass& _class) const
	{
		PopulationPlaces places;

		for (int y = 0; y < m_size.y; ++y)
		{
			for (int x = 0; x < m_size.x; ++x)
			{
				const Vec2I pos = Vec2I(x, y);
				const Tile& tile = Get(pos);
				switch (tile.type)
				{
				case Tile::Type::Residence:
					if (tile.info.quality == _class.home)
						places.home.push_back(pos);
					break;
				case Tile::Type::Hobby:
					if (std::find(_class.hobbies.begin(), _class.hobbies.end(), tile.info.hobby) != _class.hobbies.end())
						places.hobby.push_back(pos);
					break;
				case Tile::Type::Work:
					if (std::find(_class.income.begin(), _class.income.end(), tile.info.income) != _class.income.end())
						places.work.push_back(pos);
					break;
				}
			}
		}

		return places;
	}
}

#include "map.hpp"
#include "utils/random.hpp"
#include <limits>
#include <iostream>

namespace Game {

	using namespace Math;

	Map::Map(int width, int height, std::vector<Tile>&& tiles)
		: m_size(width, height),
		m_tiles(std::move(tiles)),
		m_flags(tiles.size()) 
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
		return m_tiles[_index.x + _index.y * m_size.y];
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

		std::vector<Vec2I> stack;
		auto AddOption = [&](Vec2I _index, Direction _dir, int _dist)
		{
			if (!IsInside(_index)) return;

			const int index = GetIndex(_index);
			const Tile& tile = m_tiles[index];
			const Tile::Type typeBegin = Get(_begin).type;
			const Tile::Type typeEnd = Get(_end).type;
			if ((tile.type == Tile::Type::Street 
				|| tile.type == typeBegin || tile.type == typeEnd)
				&& _dist < m_flags[index].distance)
			{
				m_flags[index].distance = _dist;
				m_flags[index].dir = _dir;
				stack.push_back(_index);
			}
				
		};

		m_flags[GetIndex(_begin)].distance = 0;
		stack.push_back(_begin);

		while (stack.size())
		{
			const Vec2I current = stack.back();
			stack.pop_back();
			int dist = m_flags[GetIndex(current)].distance + 1;

			AddOption(current + Vec2I(1, 0), Right, dist);
			AddOption(current + Vec2I(-1, 0), Left, dist);
			AddOption(current + Vec2I(0, 1), Up, dist);
			AddOption(current + Vec2I(0, -1), Down, dist);
		}

		// build path
		Path path;

		Vec2I current = _end;
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
}

#pragma once

#include "utils/vector.hpp"
#include <string_view>
#include <string>
#include <vector>

namespace Game {

	using Path = std::vector<Math::Vec2I>;
	using TileIndex = Math::Vec2I;

	class Map
	{
	public:
		struct Tile
		{
			enum struct Type
			{
				Residence, Street, Work, Hobby
			} type;
			union Info
			{
				enum struct Hobby {
					Skate, Theater, Acarde, Tennis
				} hobby;
				enum struct Quality {
					High, Mid, Low, Poor
				} quality;
				int income;
			} info;
		};

		Map() = default;
		Map(int width, int height, std::vector<Tile>&& tiles);

		const Tile& Get(Math::Vec2I _index) const;
		Math::Vec2I GetSize() const { return m_size; }

		bool IsInside(Math::Vec2I _index) const;
		Path ComputePath(Math::Vec2I _begin, Math::Vec2I _end) const;
	private:
		int GetIndex(Math::Vec2I _index) const { return _index.x + _index.y * m_size.y; }

		Math::Vec2I m_size;

		std::vector<Tile> m_tiles;

		enum Direction {
			Up,
			Down,
			Left,
			Right
		};
		struct TileFlag
		{
			Direction dir;
			int distance;
		};
		// for path finding
		mutable std::vector<TileFlag> m_flags;
	};
}

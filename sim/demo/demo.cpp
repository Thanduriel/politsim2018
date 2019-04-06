#include "game/world.hpp"

int main()
{
	using namespace Game;
	using namespace Math;

	World world;
	std::vector<Map::Tile> tiles(12 * 6);
//	map.ComputePath(Vec2I(3, 1), Vec2I(9, 3));
	world.Init(Map(12, 6, std::move(tiles)));
//	world.Init("foo", 1.f / 60.f, 1);
	for (;;)
	{
		_sleep(16);
		world.Update(0.01666666);
	}
}
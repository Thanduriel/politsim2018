#include "game/map.hpp"

int main()
{
	using namespace Game;
	using namespace Math;

	Map map("foo");
	Path path = map.ComputePath(Vec2I(2, 4), Vec2I(8, 12));

	int i = 42;
}
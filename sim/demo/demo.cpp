#include "game/world.hpp"

int main()
{
	using namespace Game;
	using namespace Math;

	World world;
	world.Init(Map());
//	world.Init("foo", 1.f / 60.f, 1);
	for (;;)
	{
		_sleep(16);
		world.Update(0.01666666);
	}
}
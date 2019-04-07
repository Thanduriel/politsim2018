#include "actor.hpp"

namespace Game {

	using Type = Map::Tile::Type;
	using Quality = Map::Tile::Info::Quality;
	using HobbyT = Map::Tile::Info::Hobby;

	const PopulationClass LOWER_CLASS = {
		{Quality::Low},
		{1,2},
		{HobbyT::Skate}
	};

	const PopulationClass MIDDLE_CLASS = {
		{Quality::Mid},
		{3,4},
		{HobbyT::Arcade, HobbyT::Tennis}
	};

	const PopulationClass UPPER_CLASS = {
		{Quality::High},
		{5,6},
		{HobbyT::Theater, HobbyT::Tennis}
	};
}
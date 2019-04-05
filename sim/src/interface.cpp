#include "interface.h"

extern "C" {

	int Increment(int _x)
	{
		return ++_x;
	}
}
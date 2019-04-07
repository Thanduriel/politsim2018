#include "event.hpp"
#include <iostream>

namespace Game{
	namespace Events {
		void Speech::operator()(Actor& _actor, float _deltaTime) const
		{
			const float change = _deltaTime * m_influence;
			std::cout << "change: " << change << "\n";
			_actor.politic = std::clamp(_actor.politic + change, 0.f, 1.f);
		}

		void Flier::operator()(Actor& _actor, float _deltaTime) const
		{
			const float change = _deltaTime * m_influence;
			if (_actor.politic < 0.5) _actor.politic = std::clamp(_actor.politic + change, 0.f, 0.5f);
		}

}
}

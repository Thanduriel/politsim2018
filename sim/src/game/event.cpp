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
		void Demo::operator()(Actor& _actor, float _deltaTime) const
		{
			if (_actor.activity > 0.1f && (m_contra ? _actor.politic < .5f : _actor.politic > .5f)) {
				_actor.stunned = true;
				_actor.activity = std::clamp(_actor.activity - 1.f * _deltaTime, 0.f, 1.f);
			} else {
				_actor.politic = std::clamp(_actor.politic - (_actor.politic * (m_contra ? -1.f : 1.f) * _deltaTime), .0f, 1.f);
			}
		}
		void Flier::operator()(Actor& _actor, float _deltaTime) const
		{
			const float change = _deltaTime * m_influence;
			if (_actor.politic < 0.5) _actor.politic = std::clamp(_actor.politic + change, 0.f, 0.5f);
		}

}
}

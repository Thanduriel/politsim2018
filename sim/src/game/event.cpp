#include "event.hpp"

namespace Game{
	namespace Events {
		void Speech::operator()(Actor& _actor, float _deltaTime) const
		{
			const float change = _deltaTime * m_influence;
			_actor.politic = std::clamp(_actor.politic, 0.f, 1.f);
		}
}
}
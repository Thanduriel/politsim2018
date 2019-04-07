#pragma once

#include "actor.hpp"
#include "utils/vector.hpp"
#include <functional>

namespace Game {

	struct Event
	{
		Event(float _duration, float _range, Math::Vec2 _position)
			: duration(_duration), rangeSq(_range*_range), position(_position)
		{
		}

		float duration;
		float rangeSq;
		Math::Vec2 position;
		
		virtual void operator() (Actor& _actor, float _deltaTime) const = 0;
	};

	namespace Events {
		struct Speech : public Event
		{
			Speech(float _duration, float _range, float _influence, Math::Vec2 _position)
				: Event(_duration, _range, _position),
				m_influence(_influence)
			{}

			virtual void operator() (Actor& _actor, float _deltaTime) const override;

		private:
			float m_influence;
		};

		struct Flier : public Event
		{
			Flier(float _duration, float _range, float _influence, Math::Vec2 _position)
				: Event(_duration, _range, _position),
				m_influence(_influence)
			{}

			virtual void operator() (Actor& _actor, float _deltaTime) const override;

		private:
			float m_influence;
		};

	}
}

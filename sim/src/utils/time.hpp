#pragma once

#include <cmath>

namespace Utils {

	template<typename T>
	class TimeOfDay
	{
	public:
		TimeOfDay(T _value) : m_value(_value - std::floor(_value)) {}

		operator T () const { return m_value; }
	private:
		T m_value;
	};
}
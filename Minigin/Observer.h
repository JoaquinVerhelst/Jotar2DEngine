#pragma once


namespace Jotar
{
	template <typename T>
	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void OnNotify(const T& eventData) = 0;
	};

}
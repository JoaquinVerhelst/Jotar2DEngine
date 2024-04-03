#pragma once


namespace Jotar
{
	template <typename T>
	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void OnNotify(T* eventData) = 0;
	};

}
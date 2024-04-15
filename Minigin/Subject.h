#pragma once
#include "Observer.h"
#include <vector>


namespace Jotar
{
	template <typename T>
	class Subject
	{
	public:

		void AddObserver(Observer<T>* observer)
		{
			m_Observers.emplace_back(observer);
		}

		void RemoveObserver(Observer<T>* observer)
		{
			auto it = std::find(m_Observers.begin(), m_Observers.end(), observer);
			if (it != m_Observers.end())
				m_Observers.erase(it);
		}

		void RemoveAllObservers()
		{
			m_Observers.clear();
		}

		void NotifyObservers(const T& eventData)
		{
			for (size_t i = 0; i < m_Observers.size(); i++)
			{
				m_Observers[i]->OnNotify(eventData);
			}
		}

	private:
		std::vector<Observer<T>*> m_Observers;
	};

}
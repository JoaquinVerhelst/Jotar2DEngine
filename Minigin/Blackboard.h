#pragma once
#include <vector>
#include <string>
#include <memory>
#include <iostream>

// This is based on Elite Engine from Gameplay Programming

namespace Jotar
{

	class IBlackBoardData
	{
	public:
		IBlackBoardData() = default;
		virtual ~IBlackBoardData() = default;
	};


	// base class needed,
	template<typename T>
	class BlackboardData  : public IBlackBoardData
	{
	public:
		explicit BlackboardField(T data) : m_Data(data)
		{}

		T GetData() { return m_Data; };
		void SetData(T data) { m_Data = data; }

		void SetName(std::string& name) { m_Name = name; }
		bool CompareName(std::string name)
		{
			if (m_Name == name)
				return true;
			return false;
		}
		
	private:
		T m_Data;
		std::string m_Name;
	};



	class Blackboard final
	{
	public:
		Blackboard() = default;
		~Blackboard() = default;


		Blackboard(const Blackboard& other) = delete;
		Blackboard& operator=(const Blackboard& other) = delete;
		Blackboard(Blackboard&& other) = delete;
		Blackboard& operator=(Blackboard&& other) = delete;

		//Add data to the blackboard
		template<typename T>
		bool AddData(const std::string& name, T data)
		{
			for (size_t i = 0; i < m_BlackboardData.size(); i++)
			{
				if (m_BlackboardData[i]->CompareName(name))
				{
					std::cout << "WARNING: Data with the name " << name.c_str() << " of type " << typeid(T).name()
						<< " already in Blackboard" << '\n';
					return false;
				}
			}

			auto data = std::make_unique<BlackboardData<T>>(data);
			data->SetName(name);
			m_BlackboardData.emplace_back(data);
			return true;
		}

		//Change the data of the blackboard
		template<typename T>
		bool ChangeData(const std::string& name, T data)
		{
			for (size_t i = 0; i < m_BlackboardData.size(); i++)
			{
				if (m_BlackboardData[i]->CompareName(name))
				{
					BlackboardData<T>* p = dynamic_cast<BlackboardData<T>*>(m_BlackboardData[i]);
					if (p)
					{
						p->SetData(data);
						return true;
					}
				}
			}

			std::cout << "WARNING: Data " << name.c_str() << " of type "
				<< typeid(T).name() " not found in Blackboard" << '\n';
			
			return false;
		}

		//Get the data from the blackboard
		template<typename T>
		bool GetData(const std::string& name, T& data)
		{
			for (size_t i = 0; i < m_BlackboardData.size(); i++)
			{
				if (m_BlackboardData[i]->CompareName(name))
				{
					m_BlackboardData<T>* p = dynamic_cast<m_BlackboardData<T>*>(m_BlackboardData[i]);
					if (p != nullptr)
					{
						data = p->GetData();
						return true;
					}
				}
			}
			std::cout << "WARNING: Data " << name.c_str() << " of type "
				<< typeid(T).name() " not found in Blackboard" << '\n';
			return false;
		}

	private:
		std::vector<std::unique_ptr<IBlackBoardData>> m_BlackboardData;
	};

}
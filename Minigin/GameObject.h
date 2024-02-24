#pragma once
#include <memory>
#include "Transform.h"
#include "Component.h"

#include <vector>
#include <stdexcept>
#include <algorithm>

namespace dae
{
	class Scene;

	class GameObject final
	{
	public:

		void Update();
		void FixedUpdate();
		void LateUpdate();
		void Render() const;
		void OnDestroy();


		GameObject(Scene* pScene, const std::string& name);
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;



		void Destroy();
		bool IsDestroyed() { return m_IsDestroyed; }

		//void SetTexture(const std::string& filename);
		void SetPosition(float x, float y);
		Transform GetPosition() { return m_Transform; }


		const std::string& GetName() const { return m_Name; };

		//Component System
		template <typename T, typename...TArgs>
		std::shared_ptr<T> AddComponent(TArgs&&... mArgs);

		template <typename T>
		T& GetComponent() const;

		template <typename T>
		void RemoveComponent();

		template <typename T>
		bool HasComponent() const;


	private:
		std::vector<std::shared_ptr<Component>> m_pComponents;
		Transform m_Transform;
		bool m_IsDestroyed;

		std::string m_Name{};
		Scene* m_pScene{};
	};



	template<typename T, typename...TArgs>
	std::shared_ptr<T> GameObject::AddComponent(TArgs&&... mArgs)
	{
		static_assert(std::is_base_of<Component, T>(), "Error: The class needs to be inherited from the Component class");

		if (HasComponent<T>()) {
			throw std::runtime_error("Error: Component of this Type" + std::string(typeid(T).name()) + "is already attached to this GameObject!");
		}

		std::shared_ptr<T> newComponent = std::make_shared<T>(this, std::forward<TArgs>(mArgs)...);
		m_pComponents.emplace_back(newComponent);
		newComponent->Init();

		return newComponent;
	}


	template<typename T>
	T& GameObject::GetComponent() const
	{

		static_assert(std::is_base_of<Component, T>(), "Error: The class needs to be inherited from the Component class");

		// Find the component of type T in the list of components
		auto it = std::find_if(m_pComponents.begin(), m_pComponents.end(), [](const auto& component) {
			return dynamic_cast<T*>(component.get()) != nullptr;
			});

		// If the component is found, return a reference to it
		if (it != m_pComponents.end()) {
			return dynamic_cast<T&>(**it);
		}

		// If the component is not found, throw an exception
		throw std::runtime_error("No component of this type: " + std::string(typeid(T).name()) + "is in this gameobject");
	}


	template<typename T> void GameObject::RemoveComponent()
	{
		static_assert(std::is_base_of<Component, T>(), "Error: The class needs to be inherited from the Component class");

		// Find the component of type T in the list of components
		auto it = std::find_if(m_pComponents.begin(), m_pComponents.end(), [](const auto& component) {
			return dynamic_cast<T*>(component.get()) != nullptr;
			});

		// If the component is found, remove it from the list
		if (it != m_pComponents.end()) {
			(*it)->SetGameObject(nullptr);
			m_pComponents.erase(it);
		}
	}


	template<typename T> bool GameObject::HasComponent() const
	{
		static_assert(std::is_base_of<Component, T>(), "Error: The class needs to be inherited from the Component class");

		// Check if a component of type T is attached to this game object
		return std::any_of(m_pComponents.begin(), m_pComponents.end(), [](const auto& component) {
			return dynamic_cast<T*>(component.get()) != nullptr;
			});
	}
}

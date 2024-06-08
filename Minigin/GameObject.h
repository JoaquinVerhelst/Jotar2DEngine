#pragma once
#include <memory>
#include "TransformComponent.h"
#include "Component.h"

#include <vector>
#include <stdexcept>
#include <algorithm>

namespace Jotar
{
	class Scene;

	class GameObject final : public std::enable_shared_from_this<GameObject>
	{
	public:

		void Start();
		void Reset();
		void Update();
		void FixedUpdate();
		void LateUpdate();
		void Render() const;
		void OnDestroy();

		GameObject(Scene* pScene, const std::string& name, bool isMovingWithCamera = true);
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void Destroy();
		bool IsDestroyed() { return m_IsDestroyed; }
		void CheckDestoryChildren();

		bool IsDestroyOnLoad() const { return m_DestroyOnLoad; }
		void SetDestroyOnLoad(bool destroyIt) { m_DestroyOnLoad = destroyIt; }


		//void SetTexture(const std::string& filename);

		TransformComponent* GetTransform() { return m_pTransform; }


		const std::string& GetName() const { return m_Name; };

		//Component System
		template <typename T, typename...TArgs>
		T* AddComponent(TArgs&&... mArgs);

		template <typename T>
		T* GetComponent() const;

		template <typename T>
		void RemoveComponent();

		template <typename T>
		bool HasComponent() const;

		// scenegraph

		std::shared_ptr<GameObject> CreateChildGameObject(const std::string& name, bool keepWorldPosition, bool isMovingWithCamera = true);

		void SetParent(GameObject* parent, bool keepWorldPosition);
		GameObject* GetParent() const { return m_pParent; }


		size_t GetChildCount() const { return m_pChildren.size(); }
		std::shared_ptr<GameObject> GetChildAt(unsigned int index) const;



		std::vector<std::shared_ptr<GameObject>> GetChildren() const { return m_pChildren; }


	private:

		void RemoveChild(std::shared_ptr<GameObject> child);


		std::vector<std::shared_ptr<Component>> m_pComponents;

		TransformComponent* m_pTransform{};


		bool m_IsDestroyed;
		bool m_DestroyOnLoad;


		std::string m_Name{};
		Scene* m_pScene{};

		std::vector<std::shared_ptr<GameObject>> m_pChildren;
		std::vector<std::shared_ptr<GameObject>> m_pChildrenToDestroy;
		GameObject* m_pParent;
	};



	template<typename T, typename...TArgs>
	T* GameObject::AddComponent(TArgs&&... mArgs)
	{
		static_assert(std::is_base_of<Component, T>(), "Error: The class needs to be inherited from the Component class");

		if (HasComponent<T>()) {
			throw std::runtime_error("Error: Component of this Type" + std::string(typeid(T).name()) + "is already attached to this GameObject!");
		}

		std::shared_ptr<T> newComponent = std::make_shared<T>(this, std::forward<TArgs>(mArgs)...);
		m_pComponents.emplace_back(newComponent);
		newComponent->Init();

		return newComponent.get();
	}


	template<typename T>
	T* GameObject::GetComponent() const
	{

		static_assert(std::is_base_of<Component, T>(), "Error: The class needs to be inherited from the Component class");

		// Find the component of type T in the list of components
		auto it = std::find_if(m_pComponents.begin(), m_pComponents.end(), [](const auto& component) {
			return dynamic_cast<T*>(component.get()) != nullptr;
			});

		// If the component is found, return a pointer to it
		if (it != m_pComponents.end()) {
			return dynamic_cast<T*>(it->get());
		}


		// If the component is not found, throw an exception
		std::string error = "No component of this type: " + std::string(typeid(T).name()) + "is in this gameobject: " + GetName();
		throw std::runtime_error(error);
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

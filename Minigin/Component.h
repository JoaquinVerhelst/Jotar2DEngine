#pragma once

namespace Jotar
{
	class GameObject;

	class Component
	{

	public:
		virtual ~Component() = default;

		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

		virtual void Start() {};
		virtual void Update() {};
		virtual void FixedUpdate() {};
		virtual void LateUpdate() {};
		virtual void Init() {};
		virtual void Render() const {};

		// For objects that dont get destroyed on Load, so they go to next scene
		virtual void Reset() {};

		bool IsDisabled() const { return m_IsDisabled; }
		void SetIsDisabled(bool isDisabled) { m_IsDisabled = isDisabled; }

		virtual void OnDestroy() {};

		GameObject* GetOwner() const { return m_pOwner; }

	protected:

		explicit Component(GameObject* pOwner);

	private:

		GameObject* m_pOwner{};
		bool m_IsDisabled = false;
	};
}
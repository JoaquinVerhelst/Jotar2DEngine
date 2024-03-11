#pragma once
#include <string>
#include <unordered_map>
#include "Component.h"
#include "GameObject.h"

namespace dae
{
	class ShaderComponent : public Component
	{
	public:
		ShaderComponent(GameObject* owner, const std::string& vertex, const std::string& fragment);
		~ShaderComponent();

		void Bind();
		void Unbind();

		void SetUniformInt(const std::string& name, int val);
		void SetUniformFloat(const std::string& name, float val);
		void SetUniformFloat2(const std::string& name, float val1, float val2);
		void SetUniformFloat3(const std::string& name, float val1, float val2, float val3);
		void SetUniformFloat4(const std::string& name, float val1, float val2, float val3, float val4);




	private:

		int GetUniformLocation(const std::string& name);

		unsigned int m_ShaderProgram;
		std::unordered_map<std::string, int> m_UniformLocations;
	};

}
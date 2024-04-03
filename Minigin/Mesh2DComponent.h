#pragma once

#include "Component.h"
#include "GameObject.h"
#include "ShaderComponent.h"

//REFACTOR
#include "glad/glad.h"
#include "GlBuffers.h"

namespace Jotar
{



	/// <This part is made based from this video, All of this will be refactored, this is more for understanding this process and how this works for now>
	/// 
	/// /// https://www.youtube.com/watch?v=jIJFM_pi6gQ&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=31
	/// </summary>
	/// 






	class Mesh2DComponent : public Component
	{
	public: 
		Mesh2DComponent(GameObject* owner, float* vertices, unsigned int vertexCount, unsigned int dimensions);
		Mesh2DComponent(GameObject* owner, float* vertices, unsigned int vertexCount, unsigned int dimensions, unsigned int* indices, unsigned int indicesCount);
		~Mesh2DComponent();

		virtual void Update();

		virtual void Init();
		virtual void Render() const;

		//void SetLayout(const BufferLayout& layout);
		//const BufferLayout& GetLayout() const;

		void Bind() const;
		void Unbind() const;

	private:

		std::unique_ptr<GLVertexArray> m_pVertexArray;

		unsigned int m_VertexCount;
	
		ShaderComponent* m_ShaderComponent;
	};
}

#include "Mesh2DComponent.h"


Jotar::Mesh2DComponent::Mesh2DComponent(GameObject* owner, float* vertices, unsigned int vertexCount, unsigned int dimensions)
	: Component(owner)
	, m_VertexCount{vertexCount}
	, m_VertexArray{0}
	, m_IndexBuffer{}
	, m_VertexBuffer{}
	, m_Layout{}
{


	m_ShaderComponent = &GetOwner()->GetComponent<ShaderComponent>();

	glCreateVertexArrays(1, &m_VertexArray);
	glBindVertexArray(m_VertexArray);

	m_VertexBuffer = std::make_unique<GLVertexBuffer>(vertices, 
		vertexCount * dimensions * sizeof(float));



	m_Layout = {
		{"position", ShaderDataType::Vec3},
		{"color", ShaderDataType::Vec4}
	};



	const auto& elements = m_Layout.GetElements();
	for (GLuint i = 0; i < elements.size(); i++)
	{
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, 
			elements[i].GetElementComponentCount(), 
			ShaderDataTypeToOpenGLBaseType(elements[i].Type), 
			elements[i].Normalized ? GL_TRUE : GL_FALSE,
			m_Layout.GetStride(),
			reinterpret_cast<const void*>(static_cast<uintptr_t>(elements[i].Offset)));

		//glDisableVertexAttribArray(i);
	}






	

	glBindVertexArray(0);
}

Jotar::Mesh2DComponent::Mesh2DComponent(GameObject* owner, float* vertices, unsigned int vertexCount, unsigned int dimensions, unsigned int* indices, unsigned int indicesCount)
	: Mesh2DComponent(owner, vertices, vertexCount, dimensions)
{

	glBindVertexArray(m_VertexArray);


	m_IndexBuffer = std::make_unique<GLIndexBuffer>(indices,
		indicesCount);


	glBindVertexArray(0);
}

Jotar::Mesh2DComponent::~Mesh2DComponent()
{

	glDeleteVertexArrays(1, &m_VertexArray);
}

void Jotar::Mesh2DComponent::Update()
{
}

void Jotar::Mesh2DComponent::Init()
{
}

void Jotar::Mesh2DComponent::Render() const
{
	Bind();

	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_VertexCount);


	//if (m_IndexBuffer->GetCount() > 0)
	//{
	//	glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, 0);
	//}
	//else
	//{
	//	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_VertexCount);
	//}


	Unbind();
}

void Jotar::Mesh2DComponent::SetLayout(const BufferLayout& layout)
{
	m_Layout = layout;
}

const Jotar::BufferLayout& Jotar::Mesh2DComponent::GetLayout() const
{
	return m_Layout;
}

void Jotar::Mesh2DComponent::Bind() const
{
	m_VertexBuffer->Bind();
	m_IndexBuffer->Bind();
	m_ShaderComponent->Bind();


	glBindVertexArray(m_VertexArray);
	//glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);
}

void Jotar::Mesh2DComponent::Unbind() const
{
	m_VertexBuffer->Unbind();
	m_IndexBuffer->Unbind();
	m_ShaderComponent->Unbind();


	//glDisableVertexAttribArray(0);
	//glDisableVertexAttribArray(1);
	glBindVertexArray(0);

}

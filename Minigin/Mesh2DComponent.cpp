
#include "Mesh2DComponent.h"


dae::Mesh2DComponent::Mesh2DComponent(GameObject* owner, float* vertices, unsigned int vertexCount, unsigned int dimensions)
	: Component(owner)
	, m_VertexCount{vertexCount}
	, m_ElementBuffer{0}
	, m_ElementCount{0}
{

	m_ShaderComponent = &GetOwner()->GetComponent<ShaderComponent>();

	glGenVertexArrays(1, &m_VertexArray);
	glBindVertexArray(m_VertexArray);


	glCreateBuffers(1, &m_VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

	//Load the vertices on the gpu
	glBufferData(GL_ARRAY_BUFFER, vertexCount * dimensions * sizeof(float), vertices, GL_STATIC_DRAW);



	//BufferElement layout = {
	//	{"position", ShaderDataType::Vec3},
	//	{"color", ShaderDataType::Vec4}
	//};



	//const auto& elements = m_Layout.GetElements();
	//for (size_t i = 0; i < elements.size(); i++)
	//{
	//	glEnableVertexAttribArray(i);
	//	glVertexAttribPointer(i, elements[i].GetElementComponentCount(), 
	//		ShaderDataTypeToOpenGLBaseType(elements[i].Type), 
	//		elements[i].Normalized ? GL_TRUE : GL_FALSE,
	//		m_Layout.GetStride(),  (const void*)elements[i].Offset);

	//	glDisableVertexAttribArray(i);
	//}






	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

dae::Mesh2DComponent::Mesh2DComponent(GameObject* owner, float* vertices, unsigned int vertexCount, unsigned int dimensions, unsigned int* elementArray, unsigned int elementCount)
	: Mesh2DComponent(owner, vertices, vertexCount, dimensions)
{
	m_ElementCount = elementCount;

	glBindVertexArray(m_VertexArray);

	glCreateBuffers(1, &m_ElementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ElementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementCount * sizeof(unsigned int), elementArray, GL_STATIC_DRAW);

	glBindVertexArray(0);
}

dae::Mesh2DComponent::~Mesh2DComponent()
{
	glDeleteBuffers(1, &m_VertexBuffer);
	if (m_ElementBuffer != 0)
	{
		glDeleteBuffers(1, &m_ElementBuffer);
	}

	glDeleteVertexArrays(1, &m_VertexArray);
}

void dae::Mesh2DComponent::Update()
{
}

void dae::Mesh2DComponent::Init()
{
}

void dae::Mesh2DComponent::Render() const
{
	Bind();
	m_ShaderComponent->Bind();

	if (m_ElementCount > 0)
	{
		glDrawElements(GL_TRIANGLES, m_ElementCount, GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLE_STRIP, 0, m_VertexCount);
	}


	Unbind();
	m_ShaderComponent->Unbind();
}
//
//void dae::Mesh2DComponent::SetLayout(const BufferLayout& layout)
//{
//	m_Layout = layout;
//}
//
//const dae::BufferLayout& dae::Mesh2DComponent::GetLayout() const
//{
//	return m_Layout;
//}

void dae::Mesh2DComponent::Bind() const
{
	glBindVertexArray(m_VertexArray);
	glEnableVertexAttribArray(0);
}

void dae::Mesh2DComponent::Unbind() const
{
	glDisableVertexAttribArray(0);
	glBindVertexArray(m_VertexArray);
}

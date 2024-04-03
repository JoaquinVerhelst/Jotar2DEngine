
#include "Mesh2DComponent.h"


Jotar::Mesh2DComponent::Mesh2DComponent(GameObject* owner, float* vertices, unsigned int vertexCount, unsigned int dimensions)
	: Component(owner)
	, m_VertexCount{vertexCount}
	, m_pVertexArray{}
{
	m_ShaderComponent = GetOwner()->GetComponent<ShaderComponent>();

	m_pVertexArray = std::make_unique<GLVertexArray>();

	BufferLayout layout = BufferLayout{
		{"position", ShaderDataType::Vec3},
		{"color", ShaderDataType::Vec4 }
	};

	m_pVertexArray->CreateVertexBuffer(vertices, vertexCount * dimensions * sizeof(float), layout);
}

Jotar::Mesh2DComponent::Mesh2DComponent(GameObject* owner, float* vertices, unsigned int vertexCount, unsigned int dimensions, unsigned int* indices, unsigned int indicesCount)
	: Mesh2DComponent(owner, vertices, vertexCount, dimensions)
{
	m_pVertexArray->CreateIndexBuffer(indices, indicesCount);
	m_pVertexArray->Unbind();
}

Jotar::Mesh2DComponent::~Mesh2DComponent()
{
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

	if (m_pVertexArray->GetIndexBuffer()->GetCount() > 0)
	{
		glDrawElements(GL_TRIANGLES, m_pVertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}
	else
	{
		glDrawArrays(GL_TRIANGLE_STRIP, 0, m_VertexCount);
	}


	Unbind();
}


void Jotar::Mesh2DComponent::Bind() const
{
	m_ShaderComponent->Bind();
	m_pVertexArray->Bind();

	for (GLuint i = 0; i < m_pVertexArray->GetVertexBuffers()[0]->GetLayout().GetElements().size(); i++)
	{
		glEnableVertexAttribArray(i);
	}
}

void Jotar::Mesh2DComponent::Unbind() const
{
	m_ShaderComponent->Unbind();
	m_pVertexArray->Unbind();

	for (GLuint i = 0; i < m_pVertexArray->GetVertexBuffers()[0]->GetLayout().GetElements().size(); i++)
	{
		glDisableVertexAttribArray(i);
	}

}

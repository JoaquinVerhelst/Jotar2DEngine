#include "GLBuffers.h"


//  ===============================================================================  //
//  							VERTEX BUFFER										 //
//  ===============================================================================  //

Jotar::GLVertexBuffer::GLVertexBuffer(float* vertices, size_t size)
	: m_Layout{}
{
	glCreateBuffers(1, &m_VertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
	//Load the vertices on the gpu
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

Jotar::GLVertexBuffer::~GLVertexBuffer()
{
	glDeleteBuffers(1, &m_VertexBufferID);
}

void Jotar::GLVertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
}

void Jotar::GLVertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Jotar::GLVertexBuffer::SetLayout(const BufferLayout& layout)
{
	m_Layout = layout;
}

const Jotar::BufferLayout& Jotar::GLVertexBuffer::GetLayout() const
{
	return m_Layout;
}

//  ===============================================================================  //
//  							INDEX BUFFER										 //
//  ===============================================================================  //


Jotar::GLIndexBuffer::GLIndexBuffer(unsigned int* vertices, unsigned int count)
{
	m_Count = count;

	glCreateBuffers(1, &m_IndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), vertices, GL_STATIC_DRAW);
}

Jotar::GLIndexBuffer::~GLIndexBuffer()
{
	glDeleteBuffers(1, &m_IndexBufferID);
}

void Jotar::GLIndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID);
}

void Jotar::GLIndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

unsigned int Jotar::GLIndexBuffer::GetCount() const
{
	return m_Count;
}

//  ===============================================================================  //
//  							VERTEX ARRAY										 //
//  ===============================================================================  //

Jotar::GLVertexArray::GLVertexArray()
{
	glCreateVertexArrays(1, &m_VertexArrayID);
	glBindVertexArray(m_VertexArrayID);
}

Jotar::GLVertexArray::~GLVertexArray()
{
	glDeleteVertexArrays(1, &m_VertexArrayID);
}

void Jotar::GLVertexArray::Bind() const
{
	glBindVertexArray(m_VertexArrayID);
	for (auto& buffer : m_pVertexBuffers)
	{
		buffer->Bind();
	}
	m_pIndexBuffer->Bind();
}

void Jotar::GLVertexArray::BindVertexArray() const
{
	glBindVertexArray(m_VertexArrayID);
}

void Jotar::GLVertexArray::Unbind() const
{
	glBindVertexArray(0);
	for (auto& buffer : m_pVertexBuffers)
	{
		buffer->Unbind();
	}
	m_pIndexBuffer->Unbind();
}



void Jotar::GLVertexArray::CreateIndexBuffer(unsigned int* indices, unsigned int indicesCount)
{
	m_pIndexBuffer = std::make_unique<GLIndexBuffer>(indices, indicesCount);
	m_pIndexBuffer->Bind();
}

void Jotar::GLVertexArray::CreateVertexBuffer(float* vertices, size_t size, const BufferLayout& layout)
{
	std::unique_ptr<GLVertexBuffer> vertexBuffer = std::make_unique<GLVertexBuffer>(vertices,
	size);

	vertexBuffer->Bind();
	vertexBuffer->SetLayout(layout);
	const auto& elements = layout.GetElements();

	for (GLuint i = 0; i < elements.size(); i++)
	{
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i,
			elements[i].GetElementComponentCount(),
			ShaderDataTypeToOpenGLBaseType(elements[i].Type),
			elements[i].Normalized ? GL_TRUE : GL_FALSE,
			layout.GetStride(),
			reinterpret_cast<const void*>(static_cast<uintptr_t>(elements[i].Offset)));

		//glDisableVertexAttribArray(i);
	}

	m_pVertexBuffers.emplace_back(std::move(vertexBuffer));
	//Unbind();
}



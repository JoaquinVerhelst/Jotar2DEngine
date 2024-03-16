#include "GLBuffers.h"



// Vertex BUFFER

Jotar::GLVertexBuffer::GLVertexBuffer(float* vertices, size_t size)
{
	glCreateBuffers(1, &m_VertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
	//Load the vertices on the gpu
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
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

// INDEX BUFFER

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


#include "FrameBuffer.h"
#include "glad/glad.h"
#include <stdexcept>


dae::FrameBuffer::FrameBuffer(unsigned int width, unsigned int height)
	: m_TextureId{}
	, m_FBO{}
	, m_BufferId{}
	, m_Width{width}
	, m_Height{height}
	, m_ClearColor{ glm::vec4{.3f,.3f,.3f,1} }
{

}

dae::FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &m_FBO);
	m_FBO = 0;
	m_TextureId = 0;
	m_BufferId = 0;
}

//dae::FrameBuffer& dae::FrameBuffer::Create(float , float )
//{
//	
//}

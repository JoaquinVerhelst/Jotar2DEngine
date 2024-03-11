#pragma once

namespace dae
{

	class GlFrameBuffer
	{
		GlFrameBuffer();
	};

}
//glGenFramebuffers(1, &m_FBO);
//glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
//
//// color texture
//glGenTextures(1, &m_TextureId);
//glBindTexture(GL_TEXTURE_2D, m_TextureId);
//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
//glBindTexture(GL_TEXTURE_2D, 0);
//
////glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
////glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureId, 0);
//
//// depth/stencil render buffer
//
//glGenRenderbuffers(1, &m_BufferId);
//glBindRenderbuffer(GL_RENDERBUFFER, m_BufferId);
//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height);
//glBindRenderbuffer(GL_RENDERBUFFER, 0);
//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_BufferId);
//
//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//throw std::runtime_error(std::string("Failed To Create FrameBuffer! "));
//
//glBindFramebuffer(GL_FRAMEBUFFER, 0);
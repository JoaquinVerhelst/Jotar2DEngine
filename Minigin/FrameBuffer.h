#pragma once
#include <glm/glm.hpp>

namespace dae
{




	class FrameBuffer
	{
	public:
		FrameBuffer(unsigned int width, unsigned int height);
		~FrameBuffer();

		unsigned int GetFBO() const { return m_FBO; }
		unsigned int GetTextureId() const { return m_TextureId; }
		unsigned int GetBufferId() const { return m_BufferId; }
		void GetSize(unsigned int& width, unsigned int& height) { width = m_Width; height = m_Height; }
		void SetClearColor(glm::vec4 color) { m_ClearColor = color; }
		glm::vec4 GetClearColor() { return m_ClearColor; }


		//FrameBuffer& Create(float width, float height);

	private:
		unsigned int m_TextureId; 
		unsigned int m_FBO;
		unsigned int m_BufferId;

		unsigned int m_Width, m_Height;

		glm::vec4 m_ClearColor;
	};
}




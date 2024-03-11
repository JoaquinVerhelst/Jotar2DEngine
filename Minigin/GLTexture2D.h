#pragma once
//#include "GL/glew.h"
#include <string>
#include "glad/glad.h"

class GLTexture2D
{
private:
	GLTexture2D();
	~GLTexture2D();

	//void CreateTexture(const std::string& filePath);


public:
	GLuint m_TextureID;
	int m_Height;
	int m_Width;

};


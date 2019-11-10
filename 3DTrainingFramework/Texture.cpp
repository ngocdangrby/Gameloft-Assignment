#include "stdafx.h"
#include "Texture.h"
#include "../Utilities/TGA.h"


Texture::Texture()
{
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_TextureId);
}


void Texture::LoadTexture(std::vector<const char*> filenames, GLenum tiling)
{
	glGenTextures(1, &m_TextureId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureId);
	for (int i = 0; i < 6; i++)
	{
		int iWidth, iHeight, iBpp;
		auto* imageData = LoadTGA(filenames[i], &iWidth, &iHeight, &iBpp);
		auto internalformat = (iBpp == 32) ? GL_RGBA : GL_RGB;
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalformat, iWidth, iHeight, 0, internalformat, GL_UNSIGNED_BYTE, imageData);
		if (imageData)
		{
			delete[] imageData;
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, tiling);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, tiling);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_Target = GL_TEXTURE_CUBE_MAP;
}


void Texture::LoadTexture(const char* filename, GLenum tiling)
{
	glGenTextures(1, &m_TextureId);
	glBindTexture(GL_TEXTURE_2D, m_TextureId);
	int iWidth, iHeight, iBpp;
	auto* imageData = LoadTGA(filename, &iWidth, &iHeight, &iBpp);
	auto internalformat = (iBpp == 32) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, internalformat, iWidth, iHeight, 0, internalformat, GL_UNSIGNED_BYTE, imageData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, tiling);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tiling);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (imageData)
	{
		delete[] imageData;
	}
	m_Target = GL_TEXTURE_2D;
}


GLuint Texture::GetTextureId() const
{
	return m_TextureId;
}

GLenum Texture::GetTarget() const
{
	return m_Target;
}

int Texture::GetId() const
{
	return m_Id;
}

void Texture::SetId(int id)
{
	m_Id = id;
}

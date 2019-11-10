#pragma once
#include "Model.h"
#include "Texture.h"
#include "Shaders.h"

class ResourceManager
{
public:
	~ResourceManager();
	static ResourceManager* GetInstance();
	int Initialize(const char* filename);
	Model* GetModelById(int id);
	Texture* GetTextureById(int id);
	Shaders* GetShadersById(int id);
private:
	ResourceManager();
	std::vector<Model*> m_ModelList;
	std::vector<Texture*> m_TextureList;
	std::vector<Shaders*> m_ShaderList;
	static ResourceManager* s_Instance;
	static GLenum GetWrappingMode(const char* wrappingMode);
	static GLenum GetShaderState(const char* state);
};

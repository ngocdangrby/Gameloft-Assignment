#include "stdafx.h"
#include "ResourceManager.h"

#define LENGTH	80	
ResourceManager* ResourceManager::s_Instance = nullptr;

Shaders* ResourceManager::GetShadersById(int id)
{
	for (auto it : m_ShaderList)
	{
		if (it->GetId() == id)
		{
			return it;
		}
	}
	return nullptr;
}

Texture* ResourceManager::GetTextureById(int id)
{
	for (auto it : m_TextureList)
	{
		if (it->GetId() == id)
		{
			return it;
		}
	}
	return nullptr;
}

Model* ResourceManager::GetModelById(int id)
{
	for (auto it : m_ModelList)
	{
		if (it->GetId() == id)
		{
			return it;
		}
	}
	return nullptr;
}

ResourceManager::ResourceManager()
{
}


ResourceManager::~ResourceManager()
{
	for (auto it : m_ModelList)
	{
		delete it;
	}
	for (auto it : m_TextureList)
	{
		delete it;
	}
	for (auto it : m_ShaderList)
	{
		delete it;
	}
	m_ModelList.clear();
	m_TextureList.clear();
	m_ShaderList.clear();
}

ResourceManager* ResourceManager::GetInstance()
{
	if (!s_Instance)
	{
		s_Instance = new ResourceManager();
	}
	return s_Instance;
}


int ResourceManager::Initialize(const char* filename)
{
	FILE* f = fopen(filename, "r");
	if (f)
	{
		// Read models information
		int nModels;
		fscanf(f, "#Models: %d\n", &nModels);
		for (int i = 0; i < nModels; i++)
		{
			char path[LENGTH] = "../Resources/";
			int id;
			char file[LENGTH];
			fscanf(f, "ID %d\n", &id);
			fscanf(f, "FILE %s\n", file);
			strcat(path, file);
			Model * model = new Model();
			model->SetId(id);
			model->LoadModel(path);
			m_ModelList.push_back(model);
		}

		// Read 2D textures information
		int nTextures2D;
		fscanf(f, "\n#2D Textures: %d\n", &nTextures2D);
		for (int i = 0; i < nTextures2D; i++)
		{
			char path[LENGTH] = "../Resources/";
			int id;
			char file[LENGTH];
			char wrapping[LENGTH];
			fscanf(f, "ID %d\n", &id);
			fscanf(f, "FILE %s\n", file);
			fscanf(f, "TILING %s\n", wrapping);
			strcat(path, file);
			Texture * texture2d = new Texture();
			texture2d->SetId(id);
			texture2d->LoadTexture(path, GetWrappingMode(wrapping));
			m_TextureList.push_back(texture2d);
		}

		// Read cube textures information
		int nTexturesCube;
		fscanf(f, "\n#Cube Textures: %d\n", &nTexturesCube);

		for (int i = 0; i < nTexturesCube; i++)
		{
			int id;
			fscanf(f, "ID %d\n", &id);
			std::vector<const char *> filenames;
			for (int j = 0; j < 6; j++)
			{
				char * path = new char[LENGTH];
				strcpy(path, "../Resources/");
				char file[LENGTH];
				fscanf(f, "FILE %s\n", file);
				strcat(path, file);
				filenames.push_back(path);
			}
			char wrapping[LENGTH];
			fscanf(f, "TILING %s\n", wrapping);
			Texture * textureCube = new Texture();
			textureCube->SetId(id);
			textureCube->LoadTexture(filenames, GetWrappingMode(wrapping));
			for (auto it : filenames)
			{
				delete it;
			}
			m_TextureList.push_back(textureCube);
		}

		// Read shaders information
		int nShaders;
		fscanf(f, "#Shaders: %d\n", &nShaders);
		for (int i = 0; i < nShaders; i++)
		{
			char path1[LENGTH] = "../Resources/";
			char path2[LENGTH] = "../Resources/";
			int id;
			char file1[LENGTH];
			char file2[LENGTH];
			fscanf(f, "ID %d\n", &id);
			fscanf(f, "VS %s\n", file1);
			fscanf(f, "FS %s\n", file2);
			int nStates;
			fscanf(f, "STATES %d\n", &nStates);
			std::vector<GLenum> states;
			for (int j = 0; j < nStates; j++)
			{
				char state[LENGTH];
				fscanf(f, "STATE %s\n", state);
				states.push_back(GetShaderState(state));
			}
			strcat(path1, file1);
			strcat(path2, file2);
			Shaders * shaders = new Shaders();
			shaders->SetId(id);
			int initStatus;
			initStatus = shaders->Initialize(path1, path2);
			if (initStatus != 0)
			{
				return initStatus;
			}
			shaders->SetStates(states);
			m_ShaderList.push_back(shaders);
		}
	}
	else
	{
		printf("Error while opening file: %s\n", filename);
		return -3;
	}
	fclose(f);
	return 0;
}

GLenum ResourceManager::GetWrappingMode(const char* wrappingMode)
{
	if (strcmp(wrappingMode, "REPEAT") == 0)
	{
		return GL_REPEAT;
	}
	if (strcmp(wrappingMode, "CLAMP") == 0)
	{
		return GL_CLAMP_TO_EDGE;
	}
	return GL_MIRRORED_REPEAT;
}

GLenum ResourceManager::GetShaderState(const char* state)
{
	if (strcmp(state, "CULLING") == 0)
	{
		return GL_CULL_FACE;
	}
	if (strcmp(state, "ALPHA") == 0)
	{
		return GL_ALPHA;
	}
	return GL_DEPTH_TEST;
}

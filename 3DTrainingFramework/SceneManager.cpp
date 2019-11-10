#include "stdafx.h"
#include "SceneManager.h"
#include "Globals.h"
#include "Vertex.h"
#include "ResourceManager.h"
#include "InputManager.h"

SceneManager* SceneManager::s_Instance = nullptr;

SceneManager::SceneManager(): m_fPitch(0), m_fYaw(0)
{
}


SceneManager::~SceneManager()
{
	for (auto it : m_ObjectList)
	{
		delete it;
	}
	m_ObjectList.clear();
}

SceneManager* SceneManager::GetInstance()
{
	if (!s_Instance)
	{
		s_Instance = new SceneManager();
	}
	return s_Instance;
}

void SceneManager::Initialize(const char* filename)
{
	FILE* f = fopen(filename, "r");
	if (f)
	{
		int nObjects;
		fscanf(f, "#Objects: %d\n", &nObjects);
		for (int i = 0; i < nObjects; i++)
		{
			int id, modelId;
			fscanf(f, "ID %d\n", &id);
			fscanf(f, "MODEL %d\n", &modelId);
			int nTextures;
			fscanf(f, "TEXTURES %d\n", &nTextures);
			std::vector<int> textureIds;
			for (int j = 0; j < nTextures; j++)
			{
				int textureId;
				fscanf(f, "TEXTURE %d\n", &textureId);
				textureIds.push_back(textureId);
			}
			int shadersId;
			fscanf(f, "SHADER %d\n", &shadersId);
			Vector3 pos, rot, scale;
			fscanf(f, "POSITION %f, %f, %f\n", &pos.x, &pos.y, &pos.z);
			fscanf(f, "ROTATION %f, %f, %f\n", &rot.x, &rot.y, &rot.z);
			fscanf(f, "SCALE %f, %f, %f\n", &scale.x, &scale.y, &scale.z);
			Object* object = new Object();
			object->SetId(id);
			object->SetModelId(modelId);
			object->SetTextureIds(textureIds);
			object->SetShaderId(shadersId);
			object->SetPosition(pos);
			object->SetRotation(rot);
			object->SetScale(scale);
			object->UpdateMatModel();
			m_ObjectList.push_back(object);
		}

		float fNear, fFar, fFov, fSpeed;
		fscanf(f, "\n#CAMERA\nNEAR %f\nFAR %f\nFOV %f\nSPEED %f\n", &fNear, &fFar, &fFov, &fSpeed);
		m_matProjection.SetPerspective(fFov, Globals::screenWidth / Globals::screenHeight, fNear, fFar);
		m_Camera = Camera(fSpeed);
	}
	else
	{
		printf("Error while opening file: %s\n", filename);
	}
}

void SceneManager::Draw()
{
	Matrix matView = m_Camera.GetViewMatrix();
	for (auto it : m_ObjectList)
	{
		Matrix mvp = it->GetMatModel() * matView * m_matProjection;
		Shaders* shaders = ResourceManager::GetInstance()->GetShadersById(it->GetShaderId());
		Model* model = ResourceManager::GetInstance()->GetModelById(it->GetModelId());
		std::vector<int> textureIds = it->GetTextureIds();

		glUseProgram(shaders->GetProgram());
		shaders->EnableStates();

		glBindBuffer(GL_ARRAY_BUFFER, model->GetVboId());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->GetIboId());
		for (int i = 0; i < textureIds.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			Texture* texture = ResourceManager::GetInstance()->GetTextureById(textureIds[i]);
			glBindTexture(texture->GetTarget(), texture->GetTextureId());
			glUniform1i(shaders->iTexLoc[i], i);
		}
		if (shaders->iTimeLoc != -1)
		{
			glUniform1f(shaders->iTimeLoc, GetTickCount()%1000/1000.f);
		}

		if (shaders->iPosLoc != -1)
		{
			glEnableVertexAttribArray(shaders->iPosLoc);
			glVertexAttribPointer(shaders->iPosLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(POS_OFFSET));
		}

		if (shaders->iUVLoc != -1)
		{
			glEnableVertexAttribArray(shaders->iUVLoc);
			glVertexAttribPointer(shaders->iUVLoc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(UV_OFFSET));
		}

		if (shaders->iMVPLoc != -1)
		{
			glUniformMatrix4fv(shaders->iMVPLoc, 1, GL_FALSE, mvp.m[0]);
		}

		glDrawElements(GL_TRIANGLES, model->GetNIndices(), GL_UNSIGNED_INT, nullptr);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		for (auto i : textureIds)
		{
			Texture* texture = ResourceManager::GetInstance()->GetTextureById(i);
			glBindTexture(texture->GetTarget(), 0);
		}
		shaders->DisableStates();
	}
}

void SceneManager::Update(float deltaTime)
{
	// Update camera
	if (InputManager::GetInstance()->IsPressed(Reset))
	{
		m_Camera.Reset();
	}
	Vector3 moveDirection(InputManager::GetInstance()->IsPressed(Right) - InputManager::GetInstance()->IsPressed(Left),
	                      InputManager::GetInstance()->IsPressed(Up) - InputManager::GetInstance()->IsPressed(Down),
	                      InputManager::GetInstance()->IsPressed(Backward) - InputManager::GetInstance()->IsPressed(Forward));
	m_Camera.Move(moveDirection, deltaTime);
	Vector2 delta = InputManager::GetInstance()->ResetMousePosition();
	float sensitivity = 0.3f;
	m_fPitch -= delta.y * sensitivity;
	m_fYaw += delta.x * sensitivity;
	m_fPitch = m_fPitch < 89.0f ? m_fPitch : 89.0f;
	m_fPitch = m_fPitch > -89.0f ? m_fPitch : -89.0f;
	m_Camera.Rotate(Radians(m_fPitch), Radians(m_fYaw));
	m_Camera.UpdateViewMatrix();
	for (auto it : m_ObjectList)
	{
		it->UpdateMatModel();
	}
}

#pragma once
#include "../Utilities/esUtil.h"

class Model
{
public:
	Model();
	~Model();
	void LoadModel(const char * filename);
	GLuint GetVboId() const;
	GLuint GetIboId() const;
	int GetNIndices() const;
	int GetNVertices() const;
	int GetId() const;
	void SetId(int id);
private:
	int m_Id;
	int m_NIndices;
	int m_NVertices;
	GLuint m_VboId;
	GLuint m_IboId;
};


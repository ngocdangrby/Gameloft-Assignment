#include "stdafx.h"
#include "Model.h"
#include "Vertex.h"
#include "../Utilities/TGA.h"
#include <iostream>


Model::Model(): m_Id(0), m_NIndices(0), m_NVertices(0), m_VboId(0), m_IboId(0)
{
}

Model::~Model()
{
	glDeleteBuffers(1, &m_VboId);
	glDeleteBuffers(1, &m_IboId);
}

void Model::LoadModel(const char* filename)
{
	FILE* f = fopen(filename, "r");
	if (f)
	{
		fscanf(f, "NrVertices: %d\n", &m_NVertices);
		Vertex* vertices = new Vertex[m_NVertices];
		if (strcmp(filename, "../Resources/Models/Terrain.nfg") == 0)
		{
			FILE* file;
			file = fopen("../Resources/Textures/heightmap33.raw", "rb");
			unsigned char* heightMap = new unsigned char[1089];
			fread(heightMap, 1, 1089, file);
			fclose(file);

			for (int i = 0; i < m_NVertices; i++)
			{
				Vertex tmp;
				fscanf(f, "%*d. pos:[%f, %f, %f]; norm:[%*f, %*f, %*f]; binorm:[%*f, %*f, %*f]; tgt:[%*f, %*f, %*f]; uv:[%f, %f];\n",
					&tmp.pos.x, &tmp.pos.y, &tmp.pos.z, &tmp.uv.x, &tmp.uv.y);
				tmp.pos.y = heightMap[i];
				vertices[i] = tmp;
			}
			delete[] heightMap;
		} 
		else
		{
			for (int i = 0; i < m_NVertices; i++)
			{
				Vertex tmp;
				fscanf(f, "%*d. pos:[%f, %f, %f]; norm:[%*f, %*f, %*f]; binorm:[%*f, %*f, %*f]; tgt:[%*f, %*f, %*f]; uv:[%f, %f];\n",
					&tmp.pos.x, &tmp.pos.y, &tmp.pos.z, &tmp.uv.x, &tmp.uv.y);
				vertices[i] = tmp;
			}
		}
		
		fscanf(f, "NrIndices: %d\n", &m_NIndices);
		unsigned int* indices = new unsigned int[m_NIndices];
		for (int i = 0; i < m_NIndices / 3; i++)
		{
			fscanf(f, "%*d.    %d,    %d,    %d\n", &indices[3 * i], &indices[3 * i + 1], &indices[3 * i + 2]);
		}
		glGenBuffers(1, &m_VboId);
		glBindBuffer(GL_ARRAY_BUFFER, m_VboId);
		glBufferData(GL_ARRAY_BUFFER, m_NVertices * sizeof(Vertex), vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &m_IboId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IboId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_NIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		delete[] vertices;
		delete[] indices;
	}
	else
	{
		printf("Failed to open file: %s\n", filename);
	}
}

GLuint Model::GetVboId() const
{
	return m_VboId;
}

GLuint Model::GetIboId() const
{
	return m_IboId;
}

int Model::GetNIndices() const
{
	return m_NIndices;
}

int Model::GetNVertices() const
{
	return m_NVertices;
}

int Model::GetId() const
{
	return m_Id;
}

void Model::SetId(int id)
{
	m_Id = id;
}

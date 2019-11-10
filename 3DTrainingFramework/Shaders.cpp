#include "stdafx.h"
#include "Shaders.h"


Shaders::Shaders(): iPosLoc(-1), iUVLoc(-1), iMVPLoc(-1), m_Id(0), m_Program(0), m_VertexShader(0), m_FragmentShader(0)
{
}


Shaders::~Shaders()
{
	m_States.clear();
	glDeleteProgram(m_Program);
	glDeleteShader(m_VertexShader);
	glDeleteShader(m_FragmentShader);
}

int Shaders::GetId() const
{
	return m_Id;
}

void Shaders::SetId(int id)
{
	m_Id = id;
}

int Shaders::Initialize(char* fileVertexShader, char* fileFragmentShader)
{
	m_VertexShader = esLoadShader(GL_VERTEX_SHADER, fileVertexShader);
	if (m_VertexShader == 0)
	{
		return -1;
	}

	m_FragmentShader = esLoadShader(GL_FRAGMENT_SHADER, fileFragmentShader);
	if (m_FragmentShader == 0)
	{
		return -2;
	}

	m_Program = esLoadProgram(m_VertexShader, m_FragmentShader);

	//finding location of uniforms / attributes
	iPosLoc = glGetAttribLocation(m_Program, "a_posL");
	iUVLoc = glGetAttribLocation(m_Program, "a_uv");
	iMVPLoc = glGetUniformLocation(m_Program, "u_MVP");
	iTexLoc[0] = glGetUniformLocation(m_Program, "u_texture");
	iTexLoc[1] = glGetUniformLocation(m_Program, "u_texture1");
	iTexLoc[2] = glGetUniformLocation(m_Program, "u_texture2");
	iTexLoc[3] = glGetUniformLocation(m_Program, "u_blendmap");
	iTimeLoc = glGetUniformLocation(m_Program, "u_time");
	return 0;
}

GLuint Shaders::GetProgram() const
{
	return m_Program;
}

void Shaders::SetStates(std::vector<GLenum> states)
{
	m_States = states;
}

void Shaders::EnableStates()
{
	for (auto it : m_States)
	{
		glEnable(it);
	}
}

void Shaders::DisableStates()
{
	for (auto it : m_States)
	{
		glDisable(it);
	}
}

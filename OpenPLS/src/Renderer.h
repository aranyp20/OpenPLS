#pragma once
#include <GL/glew.h>
#include "VAO.h"
#include "IBO.h"
#include "Shader.h"

struct RenderData
{
	inline RenderData() { vbo = nullptr; ibo = nullptr; }



	VBO* vbo;
	IBO* ibo;
};

class Renderer
{
public:
	void Draw(const VAO& va, const IBO& ib, const Shader& shader) const;
};
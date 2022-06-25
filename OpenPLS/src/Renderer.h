#pragma once
#include <GL/glew.h>
#include "VAO.h"
#include "IBO.h"
#include "Shader.h"

class Renderer
{
public:
	void Draw(const VAO& va, const IBO& ib, const Shader& shader) const;
};
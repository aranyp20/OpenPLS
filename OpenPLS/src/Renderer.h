#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include "VAO.h"
#include "IBO.h"
#include "Shader.h"

struct RenderData
{
	inline RenderData() { vbo = NULL; ibo = NULL; }

	std::vector<float> raw;

	VBO* vbo;
	IBO* ibo;
};


class Renderer
{
public:

	struct TriangleData {
		const VAO* va; const IBO& ib; const Shader& shader;
		TriangleData(const VAO* _va, const IBO& _ib, const Shader& _shader) : va(_va),ib(_ib),shader(_shader){}
	};
	struct LineData {
		const VAO* va; const Shader& shader;
		LineData(const VAO* _va, const Shader& _shader) : va(_va), shader(_shader) {}
	};
	struct PointData {
		const VAO* va; const Shader& shader;
		PointData(const VAO* _va, const Shader& _shader) : va(_va), shader(_shader) {}
	};

	void Draw(TriangleData,PointData,LineData) const;

	void DrawL(const VAO&, const Shader&) const;
	
};

#endif
#include "Renderer.h"

void Renderer::Draw(const VAO& va, const IBO& ib, const Shader& shader) const
{
	glClear(GL_COLOR_BUFFER_BIT);
	shader.Bind();
	va.Bind();
	ib.Bind();
	glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);

}

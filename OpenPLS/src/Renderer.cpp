#include "Renderer.h"

void Renderer::Draw(TriangleData t,PointData p, LineData l) const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	t.shader.Bind();
	t.va.Bind();
	t.ib.Bind();
	glDrawElements(GL_TRIANGLES, t.ib.GetCount(), GL_UNSIGNED_INT, nullptr);

	glPointSize(7);
	p.shader.Bind();
	p.va.Bind();	
	glDrawArrays(GL_POINTS, 0, p.va.GetCount() / 6);
	
	glLineWidth(4);
	l.shader.Bind();
	l.va.Bind();
	glDrawArrays(GL_LINES, 0, l.va.GetCount() / 3);
}


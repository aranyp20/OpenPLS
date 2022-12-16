#include "Renderer.h"

void Renderer::Draw(TriangleData t,PointData p, LineData l) const
{
	glClear(GL_DEPTH_BUFFER_BIT); //itt csak a depthet kell majd torolni es depthet belerakni a masikba vagy valahogy layert megoldani
	
	
	glPointSize(7);
	p.shader.Bind();
	p.va->Bind();	

	

	glDrawArrays(GL_POINTS, 0, p.va->GetCount() / 6); //ezeket a /6 /3-akat rendesen megoldani
	

 	t.shader.Bind();
	t.va->Bind();
	//t.ib.Bind();
	//glDrawElements(GL_TRIANGLES, t.ib.GetCount(), GL_UNSIGNED_INT, nullptr);
	glDrawArrays(GL_TRIANGLES,0,t.va->GetCount() / 6); 

	glLineWidth(2);
	l.shader.Bind();
	l.va->Bind();
	glDrawArrays(GL_LINES, 0, l.va->GetCount() / 3);
}

void Renderer::DrawL(const VAO& vao, const Shader& shader) const
{

	glLineWidth(4);
	shader.Bind();
	vao.Bind();
	glDrawArrays(GL_LINES, 0, vao.GetCount() / 3);
	
}




#include "VAOStore.hpp"

void VAOStore::init()
{

	const float u[] = {
			0.f, 0.f,
			1.f, 0.f,
			1.f, 1.f,
			0.f, 0.f,
			1.f, 1.f,
			0.f, 1.f
	};
	glGenBuffers(1,&uv1LL);
	glBindBuffer(GL_ARRAY_BUFFER, uv1LL);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * 6, u, GL_STATIC_DRAW);
	
	
	


	glGenVertexArrays(1, &vao2Centre);
	glGenBuffers(1, &vtx2Centre);
		
	glBindVertexArray(vao2Centre);
	
	const float v[] = {
			-1.f, -1.f,
			1.f, -1.f,
			1.f, 1.f,
			-1.f, -1.f,
			1.f, 1.f,
			-1.f, 1.f
	};
	
	
	
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vtx2Centre);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * 6, v, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uv1LL);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * 6, u, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 0, 0);



	glGenVertexArrays(1, &vao1LL);
	glGenBuffers(1, &vtx1LL);
		
	glBindVertexArray(vao1LL);
	
	const float vLL[] = {
			0.f, 0.f,
			1.f, 0.f,
			1.f, 1.f,
			0.f, 0.f,
			1.f, 1.f,
			0.f, 1.f
	};
	
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vtx1LL);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * 6, vLL, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uv1LL);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * 6, u, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	
	glBindVertexArray(0);

}

#include "MouseTrail.hpp"
#include <infoContainer/GlobalInfo.hpp>
#include <Shader/allShaders/allShaders.hpp>

MouseTrail::MouseTrail(GlobalInfo& gi):
globalInfo(gi)
{
	//glData = new std::array<float, 3>[1024];
	memset(glData,0x00,sizeof(std::array<float, 3>) * 1024);
}

void MouseTrail::init()
{
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);
	
	glBindVertexArray(vao);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3, glData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	glBindVertexArray(0);
}

void MouseTrail::update()
{
	CursorTrail ct = globalInfo.getCursorTrail();
	for(int i=0;i<ct.n;i++)
	{
		history.push_back(MouseHistory(ct.x[i], ct.y[i], ct.time[i]));
	}
	double t = glfwGetTime();
	
	//Remove old points
	while(history.size()>0 && history[0].time < t - 1)//If more than 1 second ago
	{
		history.pop_front();
	}

	
	for(int i=0;i<history.size() && i<1024;i++)
	{
		glData[i] = {{history[i].x, history[i].y, 0.5f*pow(float(1 + (history[i].time - t)),2.f)}};
	}	
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * min((int)history.size(), 1024), glData, GL_STATIC_DRAW);
	
	globalInfo.shaderManager->bind(SH_DrawLine);
	globalInfo.bindPixelProjection();
	
	GLuint colourLoc= static_cast<const Shader_drawLine*>(globalInfo.shaderManager->getShader(SH_DrawLine))->getUniforms().colour;
	
	glBindVertexArray(vao);
	
	glLineWidth(3);
	
	glDrawArrays(GL_LINE_STRIP, 0, min((int)history.size(), 1024));
	
	glBindVertexArray(0);
	
	//cout << "DRAW" << endl;
}

void CursorIcon::render()
{
	globalInfo.shaderManager->bind(SH_BlitTexture);
	
	GLuint offsetLoc = static_cast<const Shader_blitTexture*>(globalInfo.shaderManager->getShader(SH_BlitTexture))->getUniforms().offset;
	
	globalInfo.bindPixelProjection();
	
	const GraphicBitmap* gb = static_cast<const GraphicBitmap*>(globalInfo.graphicsDB.fetch(GraphicID::cursor));
	glBindVertexArray(gb->getVAO());
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gb->getTexture());
	
	glUniform2f(offsetLoc, globalInfo.getMouseX(), globalInfo.getMouseY());
	
	glDrawArrays(GL_TRIANGLES, 0,6);
	
	glBindVertexArray(0);

}

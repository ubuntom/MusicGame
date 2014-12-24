#include "LineStream.hpp"
#include <infoContainer/GlobalInfo.hpp>
#include <Shader/allShaders/allShaders.hpp>



LineStream::LineStream(GlobalInfo& gi):
globalInfo(gi)
{

	
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	
	glBindVertexArray(vao);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 32 * sizeof(array<float, 6>), data, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	glBindVertexArray(0);
	
	lines.push_back(LineSt(1368, 200, 300, 900, 1));
	

	
	
}

void LineStream::genLine(double y, double range)
{
	std::normal_distribution<double> distribution(y,range*0.5);
	std::normal_distribution<float> distribution2(300,50);
	
	double liney = min(max(distribution(generator), y-range), y+range);
	float speed = min(max(distribution2(generator)*4, 1200.f), 1600.f);
	float length = min(max(distribution2(generator), 200.f), 400.f);
	
	lines.push_back(LineSt(globalInfo.get_winW()+length, liney, length, speed, 0.5+length/1600.));
	

}

void LineStream::render(double offset)
{
	
	//cout << "Rendering" << endl;
	
	float dt = globalInfo.getDeltaTime();
	
	
	int datan=0;
	std::list<LineSt>::iterator i = lines.begin();
	while (i != lines.end())
	{
		

		(*i).alpha-=0.5*dt;
		(*i).x -= (*i).speed*dt;
		
		
		
		if ((*i).x < 0 || (*i).alpha < 0 )
		{
			lines.erase(i++); 
			
		}
		else
		{
			data[datan] = {{(*i).x, float((*i).y + offset), (*i).alpha, (*i).x-(*i).length, float((*i).y + offset), (*i).alpha}};
			datan++;
			++i;
		}
	}
	

	
	globalInfo.shaderManager->bind(SH_DrawLine);

	globalInfo.bindPixelProjection();
	
	GLuint colourLoc= static_cast<const Shader_drawLine*>(globalInfo.shaderManager->getShader(SH_DrawLine))->getUniforms().colour;
	glUniform3f(colourLoc, 1, 1, 1);
	
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	
	
	glBufferSubData(GL_ARRAY_BUFFER, 0, datan * sizeof(array<float, 6>), data);
	
	glBindVertexArray(vao);
		
	glDrawArrays(GL_LINES, 0, 2*datan);
	
	glBindVertexArray(0);

}

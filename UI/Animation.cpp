#define _USE_MATH_DEFINES
#include <math.h>
#include "Animation.hpp"
#include <Shader/allShaders/allShaders.hpp>


void AnimationBeatPulsar::beatCallback(int beatCount)
{
	for(int i=ghosts.size()-1;i>=0;i--)
	{
			ghosts[i]+=2*M_PI;
	}
	ghosts.push_back(0);
	
}

void AnimationBeatPulsar::render()
{
	const GraphicBitmap* icon = static_cast<const GraphicBitmap*>(globalInfo.graphicsDB.fetch(graphic));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, icon->getTexture());
	
	if(pixel)glBindVertexArray(icon->getVAO());
	else glBindVertexArray(icon->getVAOFl());
	
	globalInfo.shaderManager->bind(SH_BlitTexture);
	
	globalInfo.bindFloatProjection();
	
	GLuint offsetLoc = static_cast<const Shader_blitTexture*>(globalInfo.shaderManager->getShader(SH_BlitTexture))->getUniforms().offset;
	GLuint scaleLoc = static_cast<const Shader_blitTexture*>(globalInfo.shaderManager->getShader(SH_BlitTexture))->getUniforms().scale;
	GLuint alphaLoc= static_cast<const Shader_blitTexture*>(globalInfo.shaderManager->getShader(SH_BlitTexture))->getUniforms().aF;
	
	glUniform2f(offsetLoc, 0.0, 0.0);
	glUniform1f(alphaLoc, 1);
	
	glDrawArrays(GL_TRIANGLES, 0, 6);
	
	float p = globalInfo.getPhasor();
	
	for(int i=ghosts.size()-1;i>=0;i--)
	{
		float beatTime = ghosts[i] + globalInfo.getPhasor();
		//cout << "GH: " << beatTime << endl;
		glUniform2f(scaleLoc, 1 + (beatTime/(6.28*fadeLength))*0.5, 1 + (beatTime/(6.28*fadeLength))*0.5);
		glUniform1f(alphaLoc, 0.4 - beatTime/(6.28*fadeLength)*0.4);
		
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		if(beatTime > M_PI*2*fadeLength)
		{
			ghosts.pop_front();
		}
	}
	
	glUniform1f(alphaLoc, 1.);
	
	glUniform2f(scaleLoc, 1,1);
	
}

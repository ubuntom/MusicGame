#include "Spinner.hpp"
#include <infoContainer/GlobalInfo.hpp>
#include <Shader/allShaders/allShaders.hpp>

Spinner::Spinner(GlobalInfo& gi, float start, float end):
HitObject(gi)
{
	timeStart = start;
	timeEnd = end;
	fadeTime = timeEnd + 0.25;
	rotation=0;
}

void Spinner::renderFuture()
{
	globalInfo.shaderManager->bind(SH_BlitTexture);
	
	globalInfo.bindGridProjection();
	
	cout << "Back to the Future>>>" << fadeTime << endl;
	
	unsigned int a;

	
	GLuint offsetLoc = static_cast<const Shader_blitTexture*>(globalInfo.shaderManager->getShader(SH_BlitTexture))->getUniforms().offset;
	GLuint alphaLoc= static_cast<const Shader_blitTexture*>(globalInfo.shaderManager->getShader(SH_BlitTexture))->getUniforms().aF;
	GLuint colourLoc= static_cast<const Shader_blitTexture*>(globalInfo.shaderManager->getShader(SH_BlitTexture))->getUniforms().colour;
	GLuint scaleLoc= static_cast<const Shader_blitTexture*>(globalInfo.shaderManager->getShader(SH_BlitTexture))->getUniforms().scale;
	GLuint rotationLoc= static_cast<const Shader_blitTexture*>(globalInfo.shaderManager->getShader(SH_BlitTexture))->getUniforms().rotation;
	
	float p=(globalInfo.getFrameStart()-(timeStart-globalInfo.currentBeatmap.getAR_float()));
	float f = p/globalInfo.currentBeatmap.getAR_float();
	
	if(f<1)
	{
		glUniform1f(alphaLoc, f);	
	}
	
	glUniform2f(offsetLoc, 256, 192);
	glUniform2f(scaleLoc, 250, 250);
	
	array<float, 2> mg = globalInfo.getMouseGrid();
	cout << mg[0] << " " << mg[1] << endl;
	if(firstInvoc)
	{
		cursorAngle = atan2(mg[1]-192, mg[0]-256);
		firstInvoc = false;
	}
	else
	{
		float tempAngle = atan2(mg[1]-192, mg[0]-256);
		rotation += tempAngle - cursorAngle;
		cursorAngle = tempAngle;
	}
	
	glUniform1f(rotationLoc, rotation);
	cout << rotation << endl;
	
	const GraphicBitmap* spinner = static_cast<const GraphicBitmap*>(globalInfo.graphicsDB.fetch(GraphicID::spinner_circle));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, spinner->getTexture());
	
	glBindVertexArray(globalInfo.vaoStore.getvao2Centre());
	
	glDrawArrays(GL_TRIANGLES, 0, 6);
	
	glUniform2f(scaleLoc, 1, 1);
	glUniform1f(alphaLoc, 1);	
	glUniform1f(rotationLoc, 0);
}


void Spinner::renderPast()
{
	renderFuture();
}

HitQueuePos Spinner::checkQueue()
{
	if(globalInfo.getSongTime() < timeStart-globalInfo.currentBeatmap.getAR_float())
	{
		return HitQueuePos::Waiting;
	}
	else if(globalInfo.getSongTime()>fadeTime)return HitQueuePos::Vanished;
	else if (globalInfo.getSongTime()>timeEnd)
	{
		addHitScore(HitScore::hit300);
				
		incCombo();
		return HitQueuePos::FadeOut;
	}
	
	return HitQueuePos::Visible;
}

HitFeedback Spinner::getFeedback()
{
	GLuint offsetLoc = static_cast<const Shader_blitTexture*>(globalInfo.shaderManager->getShader(SH_BlitTexture))->getUniforms().offset;
	GLuint alphaLoc= static_cast<const Shader_blitTexture*>(globalInfo.shaderManager->getShader(SH_BlitTexture))->getUniforms().aF;
	
	
	
	
	return HitFeedback(globalInfo, 256, 192, timeEnd, static_cast<const GraphicBitmap*>(globalInfo.graphicsDB.fetch(GraphicID::hit300))->getTexture(), offsetLoc, alphaLoc, 0);
	
}

bool Spinner::click(Button, float mx, float my)
{
	return true;
}

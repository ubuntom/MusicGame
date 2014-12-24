#include "HitCircle.hpp"
#include <infoContainer/GlobalInfo.hpp>
#include <GL/glew.h>
#include <Shader/allShaders/allShaders.hpp>
#include <sstream>
#include <controllerlist.hpp>

const static float fade_in = 5.5f/16.f;		//How long it takes for a circle to fade in
const static float div_fade_in = 1/fade_in;		

bool HitObject::mousePressed()
{
	return static_cast<SinglePlayController*>(globalInfo.getController())->getMouseActive();
}

void HitObject::incCombo()
{
	static_cast<SinglePlayController*>(globalInfo.getController())->combo++;
}

void HitObject::resetCombo()
{
	static_cast<SinglePlayController*>(globalInfo.getController())->combo = 1;
}

void HitObject::addHitScore(HitScore hs)
{
	static_cast<SinglePlayController*>(globalInfo.getController())->addHitScore(hs);
}

HitCircle::HitCircle(GlobalInfo& gi, float _x, float _y, double t):
HitObject(gi)
{
	x = _x;
	y = 512 - _y;
	time = t;
	
	
	float ar_float = globalInfo.currentBeatmap.getAR_float();
	
	appear = time - ar_float;	//When it first appears;
	opaque = appear + fade_in;	//When alpha = 1;
}

HitQueuePos HitCircle::checkQueue()
{
	//cout << globalInfo.getSongTime() << " " << appear << endl;
	if(globalInfo.getSongTime() < appear)
	{
		//Too early
		//cout << "E" << endl;
		return HitQueuePos::Waiting;
	}
	else if(done==true)
	{
		if(globalInfo.getSongTime()>fadeTime)return HitQueuePos::Vanished;
		return HitQueuePos::FadeOut;
	}
	//cout << "N" << endl;
	return HitQueuePos::Visible;
		
}

void HitCircle::renderPast()
{
	//cout << "Rendering p" << endl;
	
	float alpha = 4*(fadeTime - globalInfo.getSongTime());
	//cout << alpha << endl;
	
	float scale = 2-alpha;
	
	
	const GraphicBitmap* menubutton = static_cast<const GraphicBitmap*>(globalInfo.graphicsDB.fetch(GraphicID::hitcircle));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, menubutton->getTexture());
	
	//cout << "TX" << menubutton->getTexture() << endl;
	
	globalInfo.shaderManager->bind(SH_BlitTexture);
	
	GLuint offsetLoc = static_cast<const Shader_blitTexture*>(globalInfo.shaderManager->getShader(SH_BlitTexture))->getUniforms().offset;
	GLuint alphaLoc= static_cast<const Shader_blitTexture*>(globalInfo.shaderManager->getShader(SH_BlitTexture))->getUniforms().aF;
	GLuint colourLoc= static_cast<const Shader_blitTexture*>(globalInfo.shaderManager->getShader(SH_BlitTexture))->getUniforms().colour;
	GLuint scaleLoc= static_cast<const Shader_blitTexture*>(globalInfo.shaderManager->getShader(SH_BlitTexture))->getUniforms().scale;

	glBindVertexArray(menubutton->getVAOFl());
	
	globalInfo.bindGridProjection();
	
	glUniform2f(offsetLoc, x, y);
	glUniform1f(alphaLoc, alpha);
	glUniform3f(colourLoc, 0, 1, 1);
	glUniform2f(scaleLoc, scale*globalInfo.currentBeatmap.getComputedCircleSize(), scale*globalInfo.currentBeatmap.getComputedCircleSize());
	
	glDrawArrays(GL_TRIANGLES, 0, 6);
	
	const GraphicBitmap* overlay = static_cast<const GraphicBitmap*>(globalInfo.graphicsDB.fetch(GraphicID::hitcircleoverlay));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, overlay->getTexture());
	
	glUniform3f(colourLoc, 1, 1, 1);
	
	glDrawArrays(GL_TRIANGLES, 0, 6);
	
	glBindVertexArray(0);
	
	
	glUniform1f(alphaLoc, 1);
	glUniform2f(scaleLoc, 1, 1);
}

HitFeedback HitCircle::getFeedback()
{
	GraphicID g;
	switch(hitScore)
	{
		case HitScore::hit0:
			g = GraphicID::hit0;
			break;
		case HitScore::hit50:
			g = GraphicID::hit50;
			break;
		case HitScore::hit100:
			g = GraphicID::hit100;
			break;
		default:
			g = GraphicID::hit300;
			break;
			
	}

	GLuint offsetLoc = static_cast<const Shader_blitTexture*>(globalInfo.shaderManager->getShader(SH_BlitTexture))->getUniforms().offset;
	GLuint alphaLoc= static_cast<const Shader_blitTexture*>(globalInfo.shaderManager->getShader(SH_BlitTexture))->getUniforms().aF;
	
	return HitFeedback(globalInfo, x, y, hitTime, static_cast<const GraphicBitmap*>(globalInfo.graphicsDB.fetch(g))->getTexture(), offsetLoc, alphaLoc, hitAcc);
}

void HitCircle::renderFuture()
{
	//cout << "Rendering f" << endl;
	float ar_float = globalInfo.currentBeatmap.getAR_float();
	float timeDiff = globalInfo.getSongTime() - time;	//-ve if before hit
	float radius = globalInfo.currentBeatmap.getComputedCircleSize() * (1 + -2 * timeDiff);
	
	float after_appear = globalInfo.getSongTime() - appear;

	//radius = fabs(radius);
	
	float transp = min(1.f, after_appear*div_fade_in);
	
	//cout << globalInfo.getSongTime() - time << endl;
	
	if(timeDiff > globalInfo.currentBeatmap.getHitLimit())
	{
		
		cout << globalInfo.getSongTime() << " " << time << " " << globalInfo.currentBeatmap.getHitLimit() << endl;
		hitAcc = 1;
		hitScore = HitScore::hit0;
		done = true;
		fadeTime = globalInfo.getSongTime() + 0.25;
		hitTime = globalInfo.getSongTime();
		resetCombo();
	}

	const GraphicBitmap* approach = static_cast<const GraphicBitmap*>(globalInfo.graphicsDB.fetch(GraphicID::approachcircle));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, approach->getTexture());
	
	
	
	//cout << "TX" << menubutton->getTexture() << endl;
	
	globalInfo.shaderManager->bind(SH_BlitTexture);
	
	GLuint offsetLoc = static_cast<const Shader_blitTexture*>(globalInfo.shaderManager->getShader(SH_BlitTexture))->getUniforms().offset;
	GLuint alphaLoc= static_cast<const Shader_blitTexture*>(globalInfo.shaderManager->getShader(SH_BlitTexture))->getUniforms().aF;
	GLuint colourLoc= static_cast<const Shader_blitTexture*>(globalInfo.shaderManager->getShader(SH_BlitTexture))->getUniforms().colour;
	GLuint scaleLoc= static_cast<const Shader_blitTexture*>(globalInfo.shaderManager->getShader(SH_BlitTexture))->getUniforms().scale;

	const GraphicBitmap* menubutton = static_cast<const GraphicBitmap*>(globalInfo.graphicsDB.fetch(GraphicID::hitcircle));

	glBindVertexArray(approach->getVAOFl());
	
	globalInfo.bindGridProjection();
	
	glUniform2f(offsetLoc, x, y);
	glUniform1f(alphaLoc, transp);
	
	
	if(timeDiff<=0)
	{
		glUniform2f(scaleLoc, radius, radius);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, menubutton->getTexture());
	
	
	
	
	
	glUniform3f(colourLoc, 0, 1, 1);
	glUniform2f(scaleLoc, globalInfo.currentBeatmap.getComputedCircleSize(), globalInfo.currentBeatmap.getComputedCircleSize());
	
	glDrawArrays(GL_TRIANGLES, 0, 6);
	
	const GraphicBitmap* overlay = static_cast<const GraphicBitmap*>(globalInfo.graphicsDB.fetch(GraphicID::hitcircleoverlay));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, overlay->getTexture());
	
	glUniform3f(colourLoc, 1, 1, 1);
	
	glDrawArrays(GL_TRIANGLES, 0, 6);
	
	glBindVertexArray(0);
	
	
	glUniform1f(alphaLoc, 1);
	glUniform2f(scaleLoc, 1, 1);
		
}

bool HitCircle::click(Button, float mx, float my)
{
	float d = (mx-x)*(mx-x) + (my-y)*(my-y);
	float lim = globalInfo.currentBeatmap.getComputedCircleSize();
	
	if(d<lim*lim)
	{
	
		if(done==true)cout << "UIT RBTEETETYTYTV" << endl;
		hitTime = globalInfo.getSongTime();
		
		hitAcc = (hitTime - time)/globalInfo.currentBeatmap.getHitLimit();	//-ve if early
		
		
		hitScore = globalInfo.currentBeatmap.getScore(fabs(hitTime - time));
		
		if (hitScore!=HitScore::hitEarly)
		{
		

			cout << "Hit Note! " << hitTime - time << " " << hitAcc << " " << (int)hitScore << endl;
			
			done = true;
			fadeTime = hitTime + 0.25;
			
			addHitScore(hitScore);

			
			incCombo();
		}
		//TODO else do the wobble thing
		
		
		return true;
	}
	return false;
	
}

HitObjectManager::HitObjectManager(GlobalInfo& gi):
globalInfo(gi)
{
	
}

HitFeedback::HitFeedback(GlobalInfo& gi, float _x, float _y, double _time, GLuint _tex, GLuint pos, GLuint transp, float d):
globalInfo(gi),
x(_x),
y(_y),
time(_time),
texture(_tex),
offsetLoc(pos),
alphaLoc(transp),
delay(d)
{}

bool HitFeedback::render(double frameTime)
{
	float timeDiff = frameTime - time;
	if(timeDiff > 1.5f)return false;
	
	globalInfo.shaderManager->bind(SH_BlitTexture);
	
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform2f(offsetLoc, x, y);
	glUniform1f(alphaLoc, 1 - max(0.f,(timeDiff-0.5f)));
	
	glDrawArrays(GL_TRIANGLES, 0, 6);
	
	
	/*globalInfo.shaderManager->bind(SH_BlitAlpha);
	GLuint colourLoc = static_cast<const Shader_blitAlpha*>(globalInfo.shaderManager->getShader(SH_BlitAlpha))->getUniforms().colour;

	glUniform3f(colourLoc, 0.f, 1.f, 1.f);
	
	sth_stash* fontStash = globalInfo.getFontStash();
	
	
	sth_begin_draw(fontStash);
	
	stringstream conv;
	conv << "Error: " << delay*100 << "%";
	
	string fs =conv.str();
	float next;
	sth_draw_text(fontStash, 1, 16.0f, x, y, fs.c_str(), &next);

	sth_end_draw(fontStash);*/
	
	//globalInfo.shaderManager->bind(SH_BlitTexture);
	
	return true;
}

void HitObjectManager::render()
{
	//cout << "Render " << future.size() << " " << past.size() << endl;
	while(past.size()!=0 && past.front()->checkQueue()==HitQueuePos::Vanished)
	{
		delete past.front();
		past.pop_front();
	}

	while(future.size()!=0 && future.front()->checkQueue()<HitQueuePos::Visible)
	{
		HitObject* h = future.front();
		past.push_back(h);
		feedback.push_back(h->getFeedback());
		future.pop_front();
	}
	
	int i;
	for(i=0;i<past.size();i++)
	{
		past[i]->renderPast();
	}
	
	
	int s = future.size();
	for(i=0;i<s && future[i]->checkQueue()==HitQueuePos::Visible;i++)
	{
		//future[i]->render();
	}
	cout << s << endl;
	for(int j=i-1;j>=0;j--)
	{
		future[j]->renderFuture();
	}
	
	
	double fs = globalInfo.getSongTime();
	globalInfo.shaderManager->bind(SH_BlitTexture);
	glBindVertexArray(static_cast<const GraphicBitmap*>(globalInfo.graphicsDB.fetch(GraphicID::hit300))->getVAOFl());
	glActiveTexture(GL_TEXTURE0);
	globalInfo.bindGridProjection();
	GLuint scaleLoc = static_cast<const Shader_blitTexture*>(globalInfo.shaderManager->getShader(SH_BlitTexture))->getUniforms().scale;
	GLuint alphaLoc = static_cast<const Shader_blitTexture*>(globalInfo.shaderManager->getShader(SH_BlitTexture))->getUniforms().aF;
	glUniform2f(scaleLoc, globalInfo.currentBeatmap.getComputedCircleSize(), globalInfo.currentBeatmap.getComputedCircleSize());
	
	for(i=0;i<feedback.size();i++)
	{
		if(!feedback[i].render(fs))
		{
			feedback.pop_front();
			i--;
		}
	}
	globalInfo.shaderManager->bind(SH_BlitTexture);
	glUniform2f(scaleLoc, 1.f, 1.f);
	glUniform1f(alphaLoc, 1.f);
}


void HitObjectManager::click(Button b)
{
	array<float, 2> mouse = globalInfo.convPxToGrid({{globalInfo.getMouseX(), globalInfo.getMouseY()}});
	
	int s = future.size();
	int i;
	for(i=0;i<s && future[i]->checkQueue()==HitQueuePos::Visible;i++)
	{
		//future[i]->render();
	}
	for(int j=0; j<i && !future[j]->click(b, mouse[0], mouse[1]); j++);
}

#include "SelectEntry.hpp"
#include <Shader/SwitchShader.hpp>
#include <Shader/allShaders/allShaders.hpp>

int SelectEntry::mouseOverOffset(bool over)
{
	if (over)
	{
		mouseOver  = ((mouseOver - 10)/1.1)+10;
	}
	else mouseOver /= 1.1;
	
	return mouseOver;
}

float SingleSongEntry::getHeight()
{
	return 120;
}

float SingleMapSubEntry::getHeight()
{
	return 120;
}

void SingleSongEntry::render()
{

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

SelectEntryManager::SelectEntryManager(GlobalInfo& gi):
globalInfo(gi),
lineStream(gi)
{
		startTime = glfwGetTime();
		selected=3;
		topOffset=0;
		
		beatCallbackID = globalInfo.addBeatCallback(std::bind(&SelectEntryManager::lineStreamGen, this, std::placeholders::_1));
}

void SelectEntryManager::lineStreamGen(int beatCount)
{
		lineStream.genLine(globalInfo.get_winH()+selectedOffset+60,50);
		lineStream.genLine(globalInfo.get_winH()+selectedOffset+60,50);
		lineStream.genLine(globalInfo.get_winH()+selectedOffset+60,50);
}

void SelectEntryManager::render()
{
	double dt = globalInfo.getDeltaTime();
	
	
	lineStream.render(-topOffset);
	
	int horiz = 0;
	if(dt<100)
	{
		horiz = 100 * (M_PI - 2*atan(globalInfo.getFrameStart()));
	}
	
	int h = globalInfo.get_winH();
	
	int mx = globalInfo.getMouseX();
	int my = globalInfo.getMouseY();
	if(my>768-100)
	{
		topOffset-= (my - 668)/20.;
		//cout << topOffset << " to" << endl;
		if(topOffset < 2*768.f - (120.f * (songEntries.size()+1)))topOffset = 2*768.f - (120.f * (songEntries.size()+1));
		

	}
	else if (my<100)
	{
		topOffset+= (100 - my)/20.;
		
		
		
		if(topOffset > h/2)topOffset = (topOffset-h/2)/1.2 + h/2;
	}
	//cout << "M" << my << endl;
		
	
	const GraphicBitmap* menubutton = static_cast<const GraphicBitmap*>(globalInfo.graphicsDB.fetch(GraphicID::menu_button_background));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, menubutton->getTexture());
	
	//cout << "TX" << menubutton->getTexture() << endl;
	
	globalInfo.shaderManager->bind(SH_BlitTexture);
	
	GLuint offsetLoc = static_cast<const Shader_blitTexture*>(globalInfo.shaderManager->getShader(SH_BlitTexture))->getUniforms().offset;
	GLuint alphaLoc= static_cast<const Shader_blitTexture*>(globalInfo.shaderManager->getShader(SH_BlitTexture))->getUniforms().aF;
	GLuint colourLoc= static_cast<const Shader_blitTexture*>(globalInfo.shaderManager->getShader(SH_BlitTexture))->getUniforms().colour;
	
	glBindVertexArray(menubutton->getVAO());
	
	globalInfo.bindPixelProjection();
	
	int offset = globalInfo.get_winH() - topOffset;
	int offsetInc=0;
	double phasor = globalInfo.getCalc4Phasor();
	for(int i=0;i<songEntries.size();i++)
	{
		int hoffset = -50/(fabs(i - selected)+1) + globalInfo.get_winW()-650-5*pow(cosf(phasor-0.5*i),6) + horiz;
		offsetInc-=80;
		
		int o;
		if(mx>hoffset && my>offset+offsetInc && my<offset+offsetInc+120) o = songEntries[i]->mouseOverOffset(true);
		else o = songEntries[i]->mouseOverOffset(false);


		glUniform2f(offsetLoc, hoffset - offset*0.05 - o*2, offset - o + offsetInc);
		
		if(i == selected)
		{
			glUniform3f(colourLoc, 1., 0.5, 1.);
			glUniform1f(alphaLoc, 0.8+ 0.4*pow(cosf(phasor-0.5*i),20));
			songEntries[i]->render();
			glUniform3f(colourLoc, 1., 1., 1.);
			selectedOffset = offsetInc;
		}
		else
		{
			glUniform1f(alphaLoc, o*0.01 + 0.7 + 0.4*pow(cosf(phasor-0.5*i),20));
			songEntries[i]->render();
		}
		
	}
	glUniform1f(alphaLoc, 1.f);
	
	glBindVertexArray(0);
	
}

bool SelectEntryManager::handleClick(int b, int a)
{
	return true;
}

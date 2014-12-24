#include "GlobalInfo.hpp"
#include <Shader/allShaders/allShaders.hpp>
#include <helpful.hpp>
#include <fontstash/fontstash.h>

HitScore SongSettings::getScore(float time)
{
	cout << "Hi: " << time << endl;
	if(time<hitWindow[0])return HitScore::hit300;
	if(time<hitWindow[1])return HitScore::hit100;
	if(time<hitWindow[2])return HitScore::hit50;
	if(time<hitWindow[3])return HitScore::hit0;
	return HitScore::hitEarly;
}


void SongSettings::setHitWindow(int od)
{
	if(od<5)
	{
		if(od==0) hitWindow = {{0.08, 0.14, 0.20, 0.26}};
		else
		{
			hitWindow[0] = (0.08*(5-od) + 0.05*od)*0.2;
			hitWindow[1] = (0.14*(5-od) + 0.10*od)*0.2;
			hitWindow[2] = (0.20*(5-od) + 0.15*od)*0.2;
			hitWindow[3] = (0.26*(5-od) + 0.20*od)*0.2;
		}
	}
	else if (od==5) hitWindow = {{0.05, 0.10, 0.15, 0.20}};
	else
	{
		if(od==10) hitWindow = {{0.02, 0.06, 0.10, 0.14}};
		else
		{
			hitWindow[0] = (0.08*(od-5) + 0.05*(10-od))*0.2;
			hitWindow[1] = (0.14*(od-5) + 0.10*(10-od))*0.2;
			hitWindow[2] = (0.20*(od-5) + 0.15*(10-od))*0.2;
			hitWindow[3] = (0.26*(od-5) + 0.20*(10-od))*0.2;
		}
	}

			
}

GlobalInfo::GlobalInfo():
graphicsDB(*this),
mouseTrail(*this),
cursorIcon(*this),
mPlayer(*this),
currentBeatmap(*this)
{
	cout << "GlobalInfo::GlobalInfo" << endl;
	shaderManager = new ShaderManager;
	inputHandler = new InputHandler;
	cursorTrail.n = 0;
	
}

void GlobalInfo::init()
{
	shaderManager->init();
	vaoStore.init();
	
	graphicsDB.loadBase();
	
	mouseTrail.init();
	
	
	
	glGenBuffers(1, &uboPixel);
	glBindBuffer(GL_UNIFORM_BUFFER, uboPixel);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(projPx), &projPx, GL_STATIC_DRAW);
	
	glGenBuffers(1, &uboFloat);
	glBindBuffer(GL_UNIFORM_BUFFER, uboFloat);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(projFl), &projFl, GL_STATIC_DRAW);
	
	glGenBuffers(1, &uboGrid);
	glBindBuffer(GL_UNIFORM_BUFFER, uboGrid);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(projGr), &projGr, GL_STATIC_DRAW);
	
	glBindBufferBase(GL_UNIFORM_BUFFER, UB::ProjectionMatrix, uboPixel);
	
	currentBeatmap.setCircleSize(3);
	
	fontStash = sth_create(512,512);
	
	cout << "Font: " << (font=sth_add_font(fontStash,"DroidSerif-Regular.ttf")) << endl;
	
	currentBeatmap.setModSpeed(1.f);
	
	currentBeatmap.alSpeedModifier = 1;
	currentBeatmap.alTimeModifier = 1;
	
	currentBeatmap.setHitWindow(10);
	
	currentBeatmap.currentBPM = 183;
	
}

void GlobalInfo::updateUBO_Float()
{
	cout << "Updating ubo" << endl;
	glBindBuffer(GL_UNIFORM_BUFFER, uboFloat);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(projFl), &projFl);
}

void GlobalInfo::updateUBO_Pixel()
{
	cout << "Updating ubo" << endl;
	glBindBuffer(GL_UNIFORM_BUFFER, uboPixel);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(projPx), &projPx);
}

void GlobalInfo::updateUBO_Grid()
{
	cout << "Updating ubo" << endl;
	glBindBuffer(GL_UNIFORM_BUFFER, uboGrid);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(projGr), &projGr);
}

void GlobalInfo::beginFrame()
{
	
	//cout << "GT: " << glfwGetTime() << " AT " << mPlayer.getTime() << endl;
	
	float changedTime = mPlayer.getChangedTime();		//Do this at beginning or end of frame?
	if(changedTime!=0.f)
	{
		//cout << "Changed: " << glfwGetTime() - changedTime << endl;
		glfwSetTime(changedTime);
		//cout << "g " << changedTime << endl;
		frameSet = changedTime;
	}
	
	mouseGrid = convPxToGrid({{(float)mouseX, (float)mouseY}});

}

void GlobalInfo::endFrame()
{
	unsigned int a;
	
	float next;
	mouseTrail.update();
	cursorIcon.render();
	shaderManager->bind(SH_BlitAlpha);
	GLuint colourLoc = static_cast<const Shader_blitAlpha*>(shaderManager->getShader(SH_BlitAlpha))->getUniforms().colour;

	glUniform3f(colourLoc, 0.f, 1.f, 1.f);
	
	a = glGetError();
	if (a!=0)cout << "FrameEnd0 " << a << endl;
	
	float accum=0;
	for(int i=0;i<8;i++)
	{
		accum+=dTimes[i];
	}
	accum = 8/accum;
	
	sth_begin_draw(fontStash);
	
	a = glGetError();
	if (a!=0)cout << "fontstash0 " << a << endl;
	
	
	string n(intToStr(int(accum)));
	
	stringstream conv;
	conv << frameStart;
	
	string fs =conv.str();
	
	a = glGetError();
	if (a!=0)cout << "fontstash1 " << a << endl;
	
	n+= " fps";
	sth_draw_text(fontStash, font, 24.0f, 10, 10, n.c_str(), &next);
	
	sth_draw_text(fontStash, font, 24.0f, 10, 30, fs.c_str(), &next);
	
	a = glGetError();
	if (a!=0)cout << "fontstash2 " << a << endl;
	
	
	sth_end_draw(fontStash);
	cursorTrail.n = 0;
	
	a = glGetError();
	if (a!=0)cout << "fontstash3 " << a << endl;
	
	int w = fontStash->tw;
	int h = fontStash->th;
	
	
	/*
	GLvoid* pixels = malloc(w*h*4);
	
	png::image<png::rgba_pixel> img(w,h);

	a = glGetError();
	if (a!=0)cout << "gEaa" << a << endl;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//glReadBuffer(GL_DEPTH_STENCIL_ATTACHMENT);
	//glReadPixels(0,0, 1368 ,768, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, pixels);
	
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    
    a = glGetError();
	if (a!=0)cout << "gEab" << a << endl;
    

    
    unsigned char* data = static_cast<unsigned char*>(pixels);
	
	auto pixel = img.get_pixel(0,0);
	pixel.green = 0;
	pixel.blue = 0;
	
	int count=0;
	for (int j=0;j<h;j++)
	{
		for(int i=0;i<w;i++)
		{
			pixel.red = data[count];
			pixel.green = data[count+1];
			pixel.blue = data[count+2];
			pixel.alpha = data[count+3];
			img.set_pixel(i,j,pixel);
			
			count+=4;
		}
	}
	
	img.write("font.png");
	*/


}

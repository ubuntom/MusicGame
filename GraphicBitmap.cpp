#include "Graphic.hpp"
#include <helper/imageLoad.hpp>
#include <DB/GraphicsDB.hpp>
#include <infoContainer/GlobalInfo.hpp>
using namespace std;

GraphicBitmap::GraphicBitmap(GlobalInfo& gi):
Graphic(gi)
{
	glGenTextures(1, &texture);
	glGenBuffers(2, vbo);
	glGenBuffers(1, &vboFl);
	glGenVertexArrays(2, &vao);
}

GraphicBitmap::~GraphicBitmap()
{
	glDeleteTextures(1, &texture);
}

void GraphicBitmap::fillBuffers(const GraphicMetric& metric)
{
	float pr, pl, pu, pd;	//Dimensions of the box (right left up down)
	
	if(metric.pxOrigin == LOWERLEFT)
	{
		pl=0;
		pd=0;
		pr=intSize[0];
		pu=intSize[1];
		
	}
	else if(metric.pxOrigin == CENTRE)
	{
		pl = -intSize[0]/2.;
		pd = -intSize[1]/2.;
		pr = intSize[0]/2.;
		pu = intSize[1]/2.;
	}
	else
	{
		pl = -intSize[0];
		pd = -intSize[1];
		pr = intSize[0];
		pu = intSize[1];
	}
	
	cout << pl << " " << pr << " " << pu << " " << pd << endl;
	
	GLfloat d[]={
		pl, pd,
		pr, pd,
		pr, pu,
		pl, pd,
		pr, pu,
		pl, pu
	};
	
	GLfloat uv[]={
		0,	0,
		1,	0,
		1,	1,
		0,	0,
		1,	1,
		0,	1	
	};
	
	float fw, fh;
	if(metric.forceFloat==FF_ABSOLUTE)
	{
		fw = metric.wFloat;
		fh = metric.hFloat;
	
	}
	else if(metric.forceFloat==FF_LIMITMAX)
	{
		float maxHeight = metric.hFloat;
		float maxWidth = (metric.wFloat * globalInfo.get_winW())/globalInfo.get_winH();
		
		float tryHeight = 1;
		float tryWidth = (float)intSize[0] / intSize[1];
		
		if(tryHeight>maxHeight && tryWidth>maxWidth)
		{
			float Hfactor = maxHeight/tryHeight;
			float Wfactor = maxWidth/tryWidth;
			if(Hfactor>Wfactor)//Image is much too wide
			{
				tryWidth = maxWidth;
				tryHeight*=Wfactor;
			}
			else //Image is much too tall
			{
				tryHeight = maxHeight;
				tryWidth*=Hfactor;
			}
			
		}
		else if(tryHeight>maxHeight)
		{
			float Hfactor = maxHeight/tryHeight;
			tryHeight = maxHeight;
			tryWidth*=Hfactor;
			
		}
		else if(tryWidth>maxWidth)
		{
			float Wfactor = maxWidth/tryWidth;
			tryWidth = maxWidth;
			tryHeight*=Wfactor;
		}
			
		fw=tryWidth/2;
		fh=tryHeight/2;
	}
	else if(metric.forceFloat==FF_LIMITMAX)
	{
		fw = metric.hFloat;
		fh = (metric.wFloat * globalInfo.get_winW())/globalInfo.get_winH();
	}
	else
	{
		fw = intSize[0] * metric.wFloat;
		fh = intSize[1] * metric.hFloat;
	}
	
	
	
	if(metric.flOrigin == LOWERLEFT)
	{
		pl=0;
		pd=0;
		pu=fh;
		pr=fw;
	}
	else if(metric.flOrigin == CENTRE)
	{
		pl = -fw;
		pd = -fh;
		pr = fw;
		pu = fh;
	}
	else
	{
		pl = -2*fw;
		pd = -2*fh;
		pr = 2*fw;
		pu = 2*fh;
	}
	
	GLfloat fl[]={
		pl, pd,
		pr, pd,
		pr, pu,
		pl, pd,
		pr, pu,
		pl, pu
	};
	
	
	cout << "GB " << fw << " " << fh << endl;
	
	glBindVertexArray(vao);
		
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 2, d, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 2, uv, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	
	
	glBindVertexArray(vaoFl);
	
	glBindBuffer(GL_ARRAY_BUFFER, vboFl);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 3, fl, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 2, uv, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	
	
	glBindVertexArray(0);
	
	
}

void GraphicBitmap::setupTextureParams()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}
	

void GraphicBitmapRGB::loadImage(const string fname, const GraphicMetric& metric)
{
	png::image<png::rgb_pixel> image(fname);
	
	glBindTexture(GL_TEXTURE_2D, texture);
	
	GLubyte* data = extractData(image);
	
	bool validSize = true;
	switch(metric.pixelMode)
	{
		case PX_EQUAL:
			if(metric.width!=image.get_width() || metric.height!=image.get_height())
			{
				validSize = false;
			}
			break;
		case PX_LEQUAL:
			if(metric.width<image.get_width() || metric.height<image.get_height())
			{
				validSize = false;
			}
			break;
		case PX_GEQUAL:
			if(metric.width>image.get_width() || metric.height>image.get_height())
			{
				validSize = false;
			}
			break;
	}
	
	
	
	if(validSize==false)
	{
		cout << "Image size mismatch!" << endl;
		
		glTexImage2D(GL_TEXTURE_2D,0, GL_RGB, metric.width, metric.height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		intSize = {{metric.width, metric.height}};
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D,0, GL_RGB, image.get_width(), image.get_height(), 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		intSize = {{(int)image.get_width(), (int)image.get_height()}};
	}
	
	delete data;
	
	setupTextureParams();
	fillBuffers(metric);
}

void GraphicBitmapRGBA::loadImage(const string fname, const GraphicMetric& metric)
{
	png::image<png::rgba_pixel> image(fname);
	
	glBindTexture(GL_TEXTURE_2D, texture);
	
	GLubyte* data = extractData(image);
	
	bool validSize = true;
	switch(metric.pixelMode)
	{
		case PX_EQUAL:
			if(metric.width!=image.get_width() || metric.height!=image.get_height())
			{
				validSize = false;
			}
			break;
		case PX_LEQUAL:
			if(metric.width<image.get_width() || metric.height<image.get_height())
			{
				validSize = false;
			}
			break;
		case PX_GEQUAL:
			if(metric.width>image.get_width() || metric.height>image.get_height())
			{
				validSize = false;
			}
			break;
	}
	
	if(validSize==false)
	{
		
		cout << "Image size mismatch!" << endl;
		uint64_t* newdata = new uint64_t[(metric.width * metric.height) / 2 + 1];
		std::fill_n(newdata, metric.width * metric.height / 2, 0xffff00ffff000000);
		
		glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA, metric.width, metric.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, newdata);
		
		delete[] newdata;
			
		intSize = {{metric.width, metric.height}};
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA, image.get_width(), image.get_height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		intSize = {{(int)image.get_width(), (int)image.get_height()}};
	}
	
	delete data;
	
	setupTextureParams();
	fillBuffers(metric);
}

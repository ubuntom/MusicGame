#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <png++/png.hpp>
#include <array>
#include <string>
#include <GL/glew.h>

class GlobalInfo;

enum ForceFloatMode{
	FF_LIMITMAX,
	FF_SCALE,
	FF_ABSOLUTE,
	FF_RELATIVE
};

enum PixelMode{
	PX_EQUAL,
	PX_IGNORE,
	PX_LEQUAL,
	PX_GEQUAL
};

enum BufferOrigin
{
	LOWERLEFT,
	CENTRE,
	CENTREX2
};
	


//! Recommended or required sizes for an image
struct GraphicMetric
{
		unsigned short width;
		unsigned short height;
		float wFloat;	//!< Max float width as factor of total window width
		float hFloat;	//!< Max float height as factor of total window height (window height always = 1)
		int forceFloat;	//! How to use float sizes
		int pixelMode;
		int pxOrigin;
		int flOrigin;
		
		GraphicMetric(int w,int h, float wF, float wH, int _forceFloat, int _pxMode=PX_IGNORE, int _pxOrigin=LOWERLEFT, int _flOrigin=CENTRE):
			width(w), height(h), wFloat(wF), hFloat(wH), forceFloat(_forceFloat), pixelMode(_pxMode), pxOrigin(_pxOrigin), flOrigin(_flOrigin)
		{}
		
		GraphicMetric():
			width(0), height(0)
		{}
};

//! Base class for Graphics
class Graphic
{
	//GraphicID graphicID;	//!< Type of graphic
	protected:
		GlobalInfo& globalInfo;
	
	public:
		Graphic(GlobalInfo& gi):
			globalInfo(gi)
		{}
		virtual std::array<int, 2> getIntSize()const{return {{0, 0}};}	//!< Return the size of a bitmap in pixels
		virtual ~Graphic(){};
		
		/*virtual void draw();		//!< Draws graphic, assuming shaders are bound
		
		virtual void preDraw();		//!< Sets up drawing state (eg bind vao)
		virtual void postDraw();	//!< Resets drawing state
		virtual void fastDraw();	//!< Quickly draws assuming state is already set up*/
		
};

//! A single bitmap image
class GraphicBitmap: public Graphic
{
	protected:
		GLuint texture;	//!< GL name of texture
		GLuint vao, vaoFl;		
		GLuint vbo[3];	//!< Vertex and UV buffers
		GLuint vboFl;	//!< For floating point sizes
		
		std::array<int, 2> intSize;
	
	public:
		GraphicBitmap(GlobalInfo& gi);
		virtual ~GraphicBitmap();
		
		virtual void loadImage(const std::string, const GraphicMetric&)=0;
		
		GLuint getTexture()const { return texture; }
		
		void fillBuffers(const GraphicMetric& ); //!< Use intSize to create pixel scale buffers
		
		void setupTextureParams();	//!< Some OpengL house keeping
		
		std::array<int, 2> getIntSize()const{return intSize;}	
		
		GLuint getVAO() const{return vao;}	
		GLuint getVAOFl() const{return vaoFl;}
};

//! A single RGB bitmap image
class GraphicBitmapRGB: public GraphicBitmap
{
	public:
		GraphicBitmapRGB(GlobalInfo& gi):
			GraphicBitmap(gi)
		{}
		void loadImage(const std::string, const GraphicMetric&);	
};

//! A single RGBA bitmap image
class GraphicBitmapRGBA: public GraphicBitmap
{
	public:
		GraphicBitmapRGBA(GlobalInfo& gi):
			GraphicBitmap(gi)
		{}
		void loadImage(const std::string, const GraphicMetric&);
};

//! Multiple bitmap images
class GraphicBitmapSet: public Graphic
{
};

#endif

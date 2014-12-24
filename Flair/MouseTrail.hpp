#ifndef MOUSETRAIL_H
#define MOUSETRAIL_H

#include <deque>
#include <GL/glew.h>
#include <array>
#include <deque>

class GlobalInfo;

struct MouseHistory
{
	float x;
	float y;
	double time;
	
	MouseHistory(float _x, float _y, float _t):
		x(_x), y(_y), time(_t)
	{}
};

class MouseTrail
{
	GlobalInfo& globalInfo;
	
	
	std::deque<MouseHistory > history;
	
	GLuint vao;
	GLuint vbo;
	
	std::array<float, 3> glData[1024];
	
	public:
		MouseTrail(GlobalInfo&);
		void init();
		void update();
		
};

class CursorIcon
{
	GlobalInfo& globalInfo;
	
	public:
		CursorIcon(GlobalInfo& gi):
			globalInfo(gi)
		{}
		
		void render();
		
};

#endif

#ifndef LINESTREAM_H
#define LINESTREAM_H

#include <GL/glew.h>
#include <array>
#include <list>
#include <random>

class GlobalInfo;

struct LineSt
{
	float x;
	double y;
	float length;
	float speed;
	float alpha;
	
	LineSt(float _x, double _y, float _l, float _s, float _a):
		x(_x), y(_y), length(_l), speed(_s), alpha(_a)
	{}
	
};

class LineStream
{
	GlobalInfo& globalInfo;
	
	GLuint vao;
	GLuint vbo;
	
	std::default_random_engine generator;
	

	std::list<LineSt> lines;
	
	std::array<float,6> data[32];
	
	public:
	
		LineStream(GlobalInfo&);
	
		void render(double offset);
		
		void genLine(double y, double range);
	
};


#endif

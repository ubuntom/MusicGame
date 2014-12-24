#include <deque>

class MouseTrail
{
	std::deque<std:array<float, 3> > history;
	
	GLuint vao;
	GLuint vbo;
	
	public:
		void update();
		
	
};

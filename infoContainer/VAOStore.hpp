#include <GL/glew.h>

class VAOStore
{
	GLuint vao2Centre;	//A VAO with side length 2 around (0,0)
	GLuint vtx2Centre;	//VBO for above
	
	
	GLuint vao1LL;		//A VAO with side length 1 from (0,0)
	GLuint vtx1LL;		//VBO for above
	
	GLuint uv1LL;		//VBO for uv in lower left
	
	public:
		void init();
		GLuint getvao2Centre(){return vao2Centre;}
		GLuint getvao1LL(){return vao1LL;}
		
		

};

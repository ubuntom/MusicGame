#include "../SwitchShader.hpp"
#include <string>

struct Uniform_blitAlpha
{
	GLuint aF;
	GLuint colour;
};

class Shader_blitAlpha: public SwitchShader
{
	
	GLuint vao;
	GLuint vbo[2];
	
	Uniform_blitAlpha uniforms;

	
	public:
		Shader_blitAlpha();
		
		GLuint getVAO() const {return vao;}
	
		const Uniform_blitAlpha& getUniforms() const {return uniforms;}
		
		

	
};



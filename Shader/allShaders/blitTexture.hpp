#include "../SwitchShader.hpp"
#include <string>

struct Uniform_blitTexture
{
	GLuint offset;
	GLuint aF;
	GLuint scale;
	GLuint colour;
	GLuint rotation;
};

class Shader_blitTexture: public SwitchShader
{
	
	GLuint vao;
	GLuint vbo[2];
	
	Uniform_blitTexture uniforms;

	
	public:
		Shader_blitTexture();
		
		GLuint getVAO() const {return vao;}
	
		const Uniform_blitTexture& getUniforms() const {return uniforms;}
		
		

	
};



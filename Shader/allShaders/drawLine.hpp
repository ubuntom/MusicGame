#include "../SwitchShader.hpp"
#include <string>

struct Uniform_drawLine
{
	GLuint colour;
};

class Shader_drawLine: public SwitchShader
{
	
	
	Uniform_drawLine uniforms;

	
	public:
		Shader_drawLine();
		
	
		const Uniform_drawLine& getUniforms() const {return uniforms;}
		
		

	
};



#include "drawLine.hpp"
#include <helpful.hpp>
using namespace std;

Shader_drawLine::Shader_drawLine()
{
	unsigned int a;
	a = glGetError();
	if(a!=0)cout << "Shaderbl setup error3: " << a << endl;
	
	const string block =
"";

	
	const char* vsText = textFileRead("shaders/drawLine/shader.vert");
	const char* fsText = textFileRead("shaders/drawLine/shader.frag");
	
	
	string vsource = ShaderManager::getVersion() + "\n";
	string fsource = ShaderManager::getVersion() + "\n";
	
	vsource += ShaderManager::getGlobalSettingsUniform() + "\n\n";
	fsource += ShaderManager::getGlobalSettingsUniform() + "\n\n";
	
	vsource += block + "\n\n";
	fsource += block + "\n\n";
	
	vsource += vsText;
	fsource += fsText;
	
	a = glGetError();
	if(a!=0)cout << "Shaderbl setup error2: " << a << endl;

	/*cout << "blitTexture gregmvmwmwemjiomioweiocropcjopqweopr\n\n\n\n" << endl;
	cout << vsource << endl;
	cout << "blitTexture pregmvmwmwemjiomioweiocropcjopqweopr\n\n\n\n" << endl;
	cout << fsource << endl;
	cout << "blitTexture gregmvmwmwemjiomioweiocropcjopqweopr\n\n\n\n" << endl;*/
	
	array<GLuint, 3> out = loadShaders("drawLine",vsource,fsource);
	
	shader_id = out[0];
	shader_vp = out[1];
	shader_fp = out[2];
	a = glGetError();
	if(a!=0)cout << "Shaderbl setup error12: " << a << endl;
	glBindAttribLocation(shader_id, 0, "in_Vertex");
	glBindFragDataLocation(shader_id, 0, "out_Scene");
	
	a = glGetError();
	if(a!=0)cout << "Shaderbl setup error111: " << a << endl;
	
	glAttachShader(shader_id, shader_fp);
	glAttachShader(shader_id, shader_vp);
	glLinkProgram(shader_id);
	
	glUseProgram(shader_id);
	
	a = glGetError();
	if(a!=0)cout << "Shaderbl setup error11: " << a << endl;

	
	uniforms.colour = glGetUniformLocation(shader_id, "colour");
	glUniform3f(uniforms.colour, 1, 1, 1);
	
	a = glGetError();
	if(a!=0)cout << "Shaderbl setup error1: " << a << endl;

	bindGlobalUniformBlock(shader_id);
	bindModelUniformBlock(shader_id);
	
	a = glGetError();
	if(a!=0)cout << "Shaderbl setup error: " << a << endl;
	
	
	
	
}


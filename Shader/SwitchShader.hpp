#ifndef SWITCHSHADER_H
#define SWITCHSHADER_H

#include <GL/glew.h>
#include <string>
#include <array>
#include <iostream>
#include <map>
#include <queue>

//! This shader class is derived to provide access to all game shader programs

std::array<GLuint, 3> loadShaders(std::string, std::string&, std::string&);
void bindGlobalUniformBlock(GLuint);
void bindModelUniformBlock(GLuint);

class SwitchShader
{
	friend class ShaderManager;
	protected:
		GLuint shader_id;
		
		GLuint shader_vp;
		GLuint shader_fp;
		
		static std::string globalSettingsUniform;	//!< Uniform block containing uniforms common to all shaders
		
	
	
	public:
	
		int id(){return shader_id;}
		void bind(){glUseProgram(shader_id);
			//std::cout << "Binding " << shader_id << std::endl;
		}
		
		
};

enum UB
{
	ModelBinding,
	ProjectionMatrix
};

enum
{
	SH_BlitTexture,
	SH_DrawLine,
	SH_BlitAlpha
};

class ShaderManager
{
	std::map<int, SwitchShader*> shaders;
	int bound = -1;
	
	std::queue<char> blockBindings;
	
	public:
		void init();
		void bind(int s){shaders[s]->bind();bound=s;}
		int getBound(){return bound;}
		const SwitchShader* getShader(int i){return shaders[i];}
		
		char getBlockBinding();
		
		static std::string getVersion();
		static std::string getGlobalSettingsUniform();

};


#endif

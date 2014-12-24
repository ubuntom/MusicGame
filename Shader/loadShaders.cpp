#include "SwitchShader.hpp"
#include <fstream>
#include <string.h>
#include <iostream>
#include <stdlib.h>
using namespace std;

array<GLuint, 3> loadShaders(string name, string& v, string& f)
{
	ofstream log;
	char fname [64];
	strcpy(fname,"logs/");
	strcat(fname,name.c_str());
	
	cout << fname << endl;
	
	log.open (fname, ios::out | ios::trunc);
	
	GLuint shader_vp = glCreateShader(GL_VERTEX_SHADER);
	GLuint shader_fp = glCreateShader(GL_FRAGMENT_SHADER);
	
	const char* vsource = v.c_str();
	const char* fsource = f.c_str();
	

	
	glShaderSource(shader_vp, 1, &vsource, 0);
	glShaderSource(shader_fp, 1, &fsource, 0);

	glCompileShader(shader_vp);
	glCompileShader(shader_fp);
	
	const unsigned int BUFFER_SIZE = 8192;
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    GLsizei length = 0;
    
  
    glGetShaderInfoLog(shader_vp, BUFFER_SIZE, &length, buffer);
    if (length > 0) {
		log << "Vertex:" << endl;
		log << buffer << endl;
    }
    
    glGetShaderInfoLog(shader_fp, BUFFER_SIZE, &length, buffer);
    if (length > 0) {
		log << "Fragment:" << endl;
		log << buffer << endl;
    }
    
    log.close();
	
	GLuint shader_id = glCreateProgram();
	
	array<GLuint, 3> out;
	out[0]=shader_id;
	out[1]=shader_vp;
	out[2]=shader_fp;
	
	return out;
	
}

void bindGlobalUniformBlock(GLuint shader_id)
{
	GLuint globalIndex = glGetUniformBlockIndex(shader_id, "GlobalSettings");
	unsigned int a = glGetError();
	if(a!=0)cout << "Shaderbl setup error11as: " << a << endl;
	glUniformBlockBinding(shader_id, globalIndex,UB::ProjectionMatrix);
	a = glGetError();
	if(a!=0)cout << "Shaderbl setup error11ad: " << a << " " << globalIndex << endl;
}
void bindModelUniformBlock(GLuint shader_id)
{
	GLuint globalIndex = glGetUniformBlockIndex(shader_id, "ModelSettings");
	
	unsigned int a = glGetError();
	if(a!=0)cout << "Shaderbl setup error11bs: " << a << endl;
	
	glUniformBlockBinding(shader_id, globalIndex,UB::ModelBinding);
	
	if(a!=0)cout << "Shaderbl setup error11bd: " << a << " " << globalIndex << endl;
}


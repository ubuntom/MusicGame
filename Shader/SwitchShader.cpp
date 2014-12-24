#include "SwitchShader.hpp"
#include "allShaders/allShaders.hpp"

using namespace std;




void ShaderManager::init()
{
	for(int i=3;i<256;i++)		//0,1 & 2 are reserved
	{
		blockBindings.push(i);
	}
	
	shaders[SH_BlitTexture] = new Shader_blitTexture;
	shaders[SH_DrawLine] = new Shader_drawLine;
	shaders[SH_BlitAlpha] = new Shader_blitAlpha;
	
	
	glUseProgram(0);
	//bind(0);
	
	cout << "Set up shaders" << endl;
	
	
}

char ShaderManager::getBlockBinding()
{
	char out = blockBindings.front();
	blockBindings.pop();
	return out;
}

string ShaderManager::getVersion()
{
	return "#version 140";
}

string ShaderManager::getGlobalSettingsUniform()
{
	return "\
layout( std140 ) uniform GlobalSettings {\n\
	mat4 projectionMatrix;\n\
};\n\
\n\
\n\
layout( std140 ) uniform ModelSettings {\n\
	mat4 MVMatrix;\n\
};\n\
";
}

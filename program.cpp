#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <time.h>
#include <iostream>
#include <controllerlist.hpp>
#include <infoContainer/GlobalInfo.hpp>


using namespace std;


class GlobalInfoContainer
{
	static GlobalInfo globalInfo;
	public:
		static GlobalInfo& getInstance()
		{
			return globalInfo;
		}

		static void setWindow(GLFWwindow* w)
		{
			globalInfo.setWindow(w);
		}

		
		static void cursorPosCallback(GLFWwindow* w, double x, double y)
		{
			globalInfo.cursorPosCallback(w, x, y);
		}
		
		static void mouseClickCallback(GLFWwindow* w, int b, int a, int modifiers)
		{
			cout << "Mouse click container callback" << endl;
			globalInfo.mouseClickCallback(w, b, a);
		}
		
		static void keyCallback(GLFWwindow* w, int b, int a, int m, int _)
		{
			cout << "Key container callback " << b << " " << a << " " << m << " " << _ << endl;
			globalInfo.keyCallback(w, b, m);
		}
};



GlobalInfo GlobalInfoContainer::globalInfo;

int main(int args, char* argv[])
{
	unsigned int a;
	glfwInit();

	
	for(int i=0;i<args;i++)
	{
		if(strcmp(argv[i],"--aa")==0)
		{
			if(strcmp(argv[i+1],"2")==0)glfwWindowHint(GLFW_SAMPLES,2);
			if(strcmp(argv[i+1],"4")==0)glfwWindowHint(GLFW_SAMPLES,4);
			if(strcmp(argv[i+1],"8")==0)glfwWindowHint(GLFW_SAMPLES,8);
			if(strcmp(argv[i+1],"16")==0)glfwWindowHint(GLFW_SAMPLES,16);
		}
	}
	glfwWindowHint(GLFW_SAMPLES,4);
	
	
	glfwWindowHint(GLFW_DEPTH_BITS,16);
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	
	GLFWwindow* window = glfwCreateWindow(1280, 720, "", NULL, NULL);//8,8,8, 0, 24, 0, GLFW_FULLSCREEN);
	
	glfwMakeContextCurrent(window);
	
	glewExperimental=GL_TRUE;
	glewInit();
	
	cout << "GL context created" << endl;
	
	a = glGetError();
	if (a!=0)cout << "Context error" << a << endl;
	
	const GLubyte* s = glGetString(GL_VERSION);
	
	cout << s << "OK" << endl;;
	std::cout.flush();
	
	GlobalInfoContainer::setWindow(window);
	GlobalInfo& globalInfo = GlobalInfoContainer::getInstance();
	globalInfo.setWindowSize(1280,720);
	globalInfo.init();
	
	std::cout.precision(8);
	
	//glfwSwapInterval(1);	//Vsync - increase responsiveness, decrease framerate.. good idea?
	
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	
	glViewport(0, 0, 1280, 720);

	
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_CLAMP);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	cout << "Samples: " << glfwGetWindowAttrib(window, GLFW_SAMPLES) << endl;

	glfwSetMouseButtonCallback(window, GlobalInfoContainer::mouseClickCallback);
	glfwSetCursorPosCallback(window, GlobalInfoContainer::cursorPosCallback);
	glfwSetKeyCallback(window, GlobalInfoContainer::keyCallback);
	
	
	globalInfo.setController(new SinglePlayController(globalInfo));



	
	glfwPollEvents();
	
	while (true)
	{

		globalInfo.getController()->update();
	}

}

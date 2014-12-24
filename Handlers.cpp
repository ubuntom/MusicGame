#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Handlers.hpp"
#include <infoContainer/GlobalInfo.hpp>

void nothing(int,int){}

void SimpleInputHandler::handleMouseButton(int button, int action)
{
	cout << "Click handler" << endl;
	//controllerInfo.playerViewport.click();
};


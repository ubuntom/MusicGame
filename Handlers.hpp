#ifndef HANDLERS_H
#define HANDLERS_H

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <map>
#include <functional>

using namespace std;



class Controller;

class InputHandler
{
	
	public:
		virtual void handleMouseButton(int button, int action){exit(0);}
		virtual void handleKeyboardEvent(int key, int action){exit(0);}
		
		virtual ~InputHandler(){}

};

class SimpleInputHandler: public InputHandler
{
	Controller& controller;
	public:
		SimpleInputHandler(Controller& ci):controller(ci){}
		virtual void handleMouseButton(int button, int action);
		virtual void handleKeyboardEvent(int key, int action){};

};

void nothing(int,int);

class BoundInputHandler: public InputHandler
{
	std::function<void(int,int)> keyboardHandler;
	std::function<void(int,int)> mouseHandler;
	public:
		BoundInputHandler()
		{
			keyboardHandler = nothing;
			mouseHandler = nothing;
		}
		void bindKeyboardHandler(std::function<void(int,int)> f){keyboardHandler = f;}
		void bindMouseHandler(std::function<void(int,int)> f){mouseHandler = f;}
		virtual void handleMouseButton(int button, int action)
		{
				mouseHandler(button, action);
		}
		virtual void handleKeyboardEvent(int key, int action)
		{
				keyboardHandler(key, action);
		};

};

#endif

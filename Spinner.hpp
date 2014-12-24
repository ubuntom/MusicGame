#ifndef SPINNER_H
#define SPINNER_H

#include <vector>
#include <array>
#include <GL/glew.h>
#include <Hit/HitCircle.hpp>
#include <Graphic.hpp>
#include <DB/GraphicsDB.hpp>

class GlobalInfo;

class Spinner: public HitObject
{
	float timeStart;
	float timeEnd;
	float fadeTime;
	bool firstInvoc = true;		//Whether it is the first time the spinner has been spun
	float cursorAngle; //Previous frame mouse pos;
	float rotation;
	
	float angle;
	
	public:
		Spinner(GlobalInfo&, float start, float end);
		virtual void renderFuture();
		virtual void renderPast();
		virtual HitQueuePos checkQueue();
		virtual HitFeedback getFeedback();
		virtual bool click(Button, float, float);
		
		
};

#endif

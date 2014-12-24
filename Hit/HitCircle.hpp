#ifndef HITOBJECT_H
#define HITOBJECT_H


#include <queue>
#include <GL/glew.h>
class GlobalInfo;



enum class HitScore
{
	hit0,
	hit50,
	hit100,
	hit300,
	hitEarly
};

class HitFeedback
{
	protected:
		GlobalInfo& globalInfo;	//TODO this isn't required
		
	float x, y;
	double time;
	GLuint texture;
	GLuint offsetLoc;
	GLuint alphaLoc;
	float delay;	//TODO not required
	
	public:
		HitFeedback(GlobalInfo&, float, float, double, GLuint, GLuint, GLuint, float);
		bool render(double);	//Bind texture, set uniforms and drawArrays. Return false if finished
};

enum class Button: int
{
	A,
	B
};

enum HitQueuePos
{
		Vanished,		//After the fade animation has finished. Ready to be removed from the queue
		FadeOut,		//After it has been hit (or missed)
		Visible,		//After it appears onscreen (AR seconds before hit)
		Waiting			//Initial state for all objects
};

class HitObject
{
	
	protected:
		GlobalInfo& globalInfo;
		
		//Interfaces with the event handler to return if a key (Z,X,0,1) is pressed
		bool mousePressed();
		
		//These functions are for neatness. They interface with the score manager
		void incCombo();				//Increase combo by 1
		void resetCombo();				//Reset combo to 0
		void addHitScore(HitScore);		//Add a combo adjusted HitScore to the current score
		
	
	public:
		virtual void renderFuture()=0;			//Render the object before it has been hit (EX: fade in and hold until hit)
		virtual void renderPast()=0;			//Render the object after it has been hit (EX: fade out, expand)
		
		
		virtual HitQueuePos checkQueue()=0;	//-1 is already passed, 0 is on screen, 1 is in the far future
		
		
		virtual HitFeedback getFeedback()=0;
		//Signals that the object's focus is now lost and returns a HitFeedback object for the hitscore. It is at this point which objects should submit their hitscores
		
		
		virtual bool click(Button, float, float)=0;	//Called when the mouse is clicked and if this object is visible
	
		HitObject(GlobalInfo& gi):
			globalInfo(gi)
		{}
		

};

class HitCircle: public HitObject
{
	float x;	//Position in screen floats
	float y;

	double time;	//Time when it should be hit (seconds)
	
	double appear;	//Time when it appears
	double opaque;	//Time when it goes opaque
	//double windowEnd;	//Time when the hit window closes
	
	bool done = false; //Whether the hit window has expired
	double fadeTime;	//Time at which it has faded out
	double hitTime;
	
	double hitAcc;
	HitScore hitScore;
	
	public:
		HitCircle(GlobalInfo&, float, float, double);
		void renderFuture();
		void renderPast();
		HitQueuePos checkQueue();	//This could be split into 2 separate functions for past and future (-2 is long after hit), (-1 is just after hit) - past, (0 is on screen, 1 is yet to be visible) - future
		bool click(Button, float, float);
		HitFeedback getFeedback();
	
};



class HitObjectManager
{
	friend class BeatmapLoader;
	
protected:
	std::deque<HitObject*> future;	//Ghost of HitObjects yet to come
	std::deque<HitObject*> past;	//Ghost of HitObjects past
	GlobalInfo& globalInfo;
	
	std::deque<HitFeedback> feedback;	//50, 100, 300 symbols
	
	public:
		HitObjectManager(GlobalInfo& gi);
		
		void click(Button);

		void render();
};

#endif

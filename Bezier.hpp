#ifndef BEZIER_H
#define BEZIER_H

#include <vector>
#include <array>
#include <GL/glew.h>
#include <Hit/HitCircle.hpp>
#include <Graphic.hpp>
#include <DB/GraphicsDB.hpp>

unsigned int nCr(unsigned int n, unsigned int k);

void calcBezierSpline(const std::vector<std::array<float, 2> >& points, std::vector<std::array<float, 2> >& out_Points, std::vector<float >& out_Gradient, int resolution);

class GlobalInfo;


class BezierSection
{
	
	public:
		float length;
	
		std::vector<std::array<float, 2> > points;
		
		std::vector<std::array<float, 2> > out_Points;
		std::vector<float > out_Gradient;
		std::vector<std::array<float, 2> > out_Homogenous;
		
		std::vector<std::array<float, 2> > inner;
		std::vector<std::array<float, 2> > outer;
		
		std::vector<std::array<float, 2> > innerOutline;
		std::vector<std::array<float, 2> > outerOutline;
		
		void calcBezierSpline(double accum_length, double max_length); //!< Calculate points and gradient
		
		double calcHomogenous(double, double, double&); //!< Calculate homogenous and return length of gap at end of spline
	
	
};

class BezierTest: public HitObject
{
	
	std::vector<BezierSection> beziers;
	
	std::vector<float > out_Gradient;
	std::vector<std::array<float, 2> > out_Homogenous;
	
	int totalPoints;
	int lineLength;
	int tris;
	int repeat;
	
	std::array<float, 2> first;			//First point
	std::array<float, 2> last;			//Last point
	std::array<float, 2> limit_last;	//Last point after max_length limit
	
	
	double windowClose;		//!< when the hit window closes
	bool missedStart;		//!< if the player has missed the first circle
	float beatDuration;		//!< How many beats the slider lasts for (ignoring repeats)
	float timeDuration;		//!< How many seconds the slider lsats for (ignoring repeats)
	float beatDurationR;
	float timeDurationR;
	
	std::vector<float> repeatTimes;	//!< Times at which the slider bounces
	
	
	double time;
	bool done=false;
	double fadeTime;
	double bps;
	bool largeFollow = false;	//The size of the follow circle. If a tick is missed then the circle goes small until the ball gets clicked
	
	int score=0;		//Tracks how many tick points have been hit
	int maxScore;		//Total number of tick points (ticks + 2)
	
	GraphicID feedGraphic;
	
	GLuint vao, vaoL;
	GLuint vbo[2];
	GLuint vboL;
	
	GLuint vaoC;
	GLuint vboC;
	
	GLuint sliderFBOmsaa;
	GLuint sliderTexturemsaa;
	GLuint sliderFBO;
	GLuint sliderTexture;
	GLuint sliderStencil;
	GLuint sliderStencilmsaa;
	
	std::vector<std::array<float, 3> > ticks;
	
	//TODO There is a lot of redundancy in this class, eg storing beatDuration, timeDuration and bps
	
	std::array<int, 2> dimensions;		//Width/height in grid
	std::array<int, 2> originOffset;		//x y offset in grid
	std::array<int, 2> pxDimensions;		//Width/height in px
	
	
	public:
		BezierTest(GlobalInfo&, std::vector<std::vector<std::array<float, 2> > >, float, float, float, float, int);
		virtual void renderFuture();
		virtual void renderPast();
		virtual HitQueuePos checkQueue();
		virtual HitFeedback getFeedback();
		virtual bool click(Button, float, float);
		
		bool checkCursor();
	
};
	
#endif

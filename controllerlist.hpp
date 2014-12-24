#ifndef CONTROLLERLIST_H
#define CONTROLLERLIST_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <time.h>
#include <vector>
#include <infoContainer/GlobalInfo.hpp>
#include <infoContainer/SceneInfo.hpp>
#include <UI/SelectEntry.hpp>
#include <UI/Animation.hpp>
#include <Bezier.hpp>
#include <Hit/HitCircle.hpp>
#include <cmath>

using namespace std;


//! Controller for Single player select
class SingleSelectController: public Controller
{
	unsigned int* vbo;
	int count;
	float start_time;
	unsigned int vao;
	int mx,my;
	bool mouseActive;
	
	float min=10000;
	float fmax=0;
	float rolling=0;
	int frames=0;
	float interval=1/60.f;
	
	GlobalInfo& globalInfo;
	SceneInfo sceneInfo;
	
	public:

		void receiveMouseInput(int, int);
		
		void update();
		SingleSelectController(GlobalInfo&);
		
		GLuint globalUBO;
		GLubyte* globalUBuffer;
		
		GLuint v,u;
		
		SelectEntryManager selectEntryManager;
		
		AnimationBeatPulsar* abp;
		BezierTest* bzt;
		
		
};

//! Contreller for single player
class SinglePlayController: public Controller
{
	unsigned int* vbo;
	int count;
	float start_time;
	unsigned int vao;
	int mx,my;
	bool mouseActive;
	
	float min=10000;
	float fmax=0;
	float rolling=0;
	int frames=0;
	float interval=1/60.f;
	
	GlobalInfo& globalInfo;
	SceneInfo sceneInfo;
	
	public:

		void receiveMouseInput(int, int);
		
		void update();
		SinglePlayController(GlobalInfo&);
		
		GLuint globalUBO;
		GLubyte* globalUBuffer;
		
		GLuint v,u;
		
		
		HitObjectManager* hitObjectManager;
		BezierTest* bzt;
		
		bool getMouseActive()
		{
			return mouseActive;
		}
		
		void addHitScore(HitScore hs)
		{
			perfAcc += 6;
			
			switch(hs)
			{
				case HitScore::hit50:
					score += 50 * (1+max(combo-1, 0)/25.);
					baseAcc += 1;
					break;
				case HitScore::hit100:
					score += 100 * (1+max(combo-1, 0)/25.);
					baseAcc += 2;
					break;
				case HitScore::hit300:
					score += 300 * (1+max(combo-1, 0)/25.);
					baseAcc += 6;
					break;
				case HitScore::hit0:
					break;
				default:
					throw InvalidScore(hs);
					break;
			}
		}
		//Adds a hit score (x,50,100,300) to the total score, accounting for combo.
		
		
		void addTrueScore(int s)
		{
			score += s;
		}
		//Adds a value directly to the score, used for spinners and slider ticks
		
		int combo = 0;
		int score = 0;
		
		int baseAcc = 0;	//Current accuracy score (ie score without combo or mods)
		int perfAcc = 0;	//Potential current accuracy score with an SS play
		
};

#endif

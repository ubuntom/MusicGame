#ifndef GLOBALINFO_H
#define GLOBALINFO_H

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Handlers.hpp>
#include <Shader/SwitchShader.hpp>
#include <DB/GraphicsDB.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <functional>
#include <Flair/MouseTrail.hpp>
#include <MusicPlayer.hpp>
#include <fontstash/fontstash.h>
#include <Hit/HitCircle.hpp>
#include "VAOStore.hpp"
#include <exception.hpp>

class Controller
{	
	public:
		Controller(){}
		virtual void update(){}
};

struct Directories
{
	std::string textures;
	std::string models;
	std::string shaders;
	std::string materials;
};

struct MiniSongInfo
{
		int bpm=186;
};

class SongSettings
{
	friend class GlobalInfo;
	
	GlobalInfo& globalInfo;
	
	protected:
	
		int circleSize;
		float computedCircleSize;
		
		int ar;
		float ar_float=1.3;	//AR in seconds
		
		float modTime = 1;	//0.66 for DT/NC, 2 for HT
		float modSpeed = 1;	//1.5 for DT/NC, 0.5 for HT
		float alTimeModifier = 1;	//1.5 for DT, 0.5 for HT, as AL "incorrectly" reports the time for these modes
		
		float alSpeedModifier = 1;	//1.5 for NC, as AL plays NC 1.5x faster (this is used for interpolation). It is the pitch (speed) al uses for this mod
		//Divide al times by this to get the true time since the start of the song
		
		
		float tickRate = 1;
		
		std::array<float, 4> hitWindow;
		
		float currentBPM; //! Current bpm of the song
	
	public:
		SongSettings(GlobalInfo& gi):
			globalInfo(gi)
		{}
		
		void setCircleSize(int s)
		{
			switch(s)
			{
				case 0:
					computedCircleSize = 54;
					break;
				case 1:
					computedCircleSize = 49;
					break;
				case 2:
					computedCircleSize = 45;
					break;
				case 3:
					computedCircleSize = 41;
					break;
				case 4:
					computedCircleSize = 36;
					break;
				case 5:
					computedCircleSize = 31;
					break;
				case 6:
					computedCircleSize = 27;
					break;
				case 7:
					computedCircleSize = 23;
					break;
				default:
					computedCircleSize = 18;
					break;
			}
			
			//computedCircleSize*=2;
			
			
		}
		float getComputedCircleSize()
		{return computedCircleSize;}
		
		float getAR_float()
		{return ar_float;}
		
		float getModTime(){return modTime;}
		float getModSpeed(){return modSpeed;}
		
		void setModSpeed(float m)
		{
			modSpeed = m;
			modTime = 1/m;
		}
		
		void setHitWindow(int od);
		
		array<float, 4> getHitWindow(){return hitWindow;}
		
		HitScore getScore(float time);
		
		float getHitLimit(){return hitWindow[3];}
		
		float getCurrentBPM(){return currentBPM;}
		
		float getTickRate(){return tickRate;}
};

	
	


//! The past cursor positions for the current frame. Only works for fixed framerate. With unlimited we would only have one event per frame
struct CursorTrail
{
	float x[16];	//!< x coord array
	float y[16];	//!< y coord array
	double time[16];	//!< glfw time of event
	char n;	//!< number of events in the current frame
};

class GlobalInfo	//Information like window size and persistent classes
{
	struct {
		glm::mat4 projectionMatrix;
	}projPx;
	struct {
		glm::mat4 projectionMatrix;
	}projFl;
	struct {
		glm::mat4 projectionMatrix;
	}projGr;
	
	
	
	GLuint uboPixel;
	GLuint uboFloat;
	GLuint uboGrid;


	//ShaderDB& shaderDB;
	friend class GlobalInfoContainer;
	
	GLFWwindow* window;
	int winW, winH;
	int mouseX, mouseY;
	array<float, 2> mouseGrid;
	
	Controller* controller;
	
	Directories directories;
	
	InputHandler* inputHandler;
	
	double phasor=0;	//!< Keeps in time to the beat. Increases by 2pi every 4 beats
	unsigned int beatCount;	//!< Increases by one on every beat
	
	float deltaTime;
	double frameStart=0;
	double frameSet=0;
	double songTime;
	
	MiniSongInfo currentSong;
	
	map<int, std::function<void(int)> > beatCallbacks;
	int beatCallbacksCount=0;
	
	CursorTrail cursorTrail;	//!< Struct for the current frames cursor positions
	
	MouseTrail mouseTrail;	//!< Cursor position over the last 5 seconds
	CursorIcon cursorIcon;
	
	sth_stash* fontStash;
	int font;
	
	float dTimes[8];
	public: int dTime=0;
	
	float pxToGrid;
	float gridToPx;
	
	public:
		MusicPlayer mPlayer;
		
		int get_winW()const {return winW;}
		int get_winH()const {return winH;}
		float getPhasor()const {return phasor;}
		float getCalcPhasor()
		{
			double bt = (60./currentSong.bpm);
			return (fmod(frameStart, bt)*2*M_PI)/bt;
		}
		float getCalc4Phasor()
		{
			double time = glfwGetTime();
			double bt = (4*60./currentSong.bpm);
			return (fmod(frameStart, bt)*2*M_PI)/bt;
		}
		
		float stepPhasor()
		{ 
			double bt = (60./currentSong.bpm);	//Time for 1 beat
			phasor+=2*M_PI*deltaTime/bt;
			
			if(phasor>2*M_PI)
			{
				phasor -= 2*M_PI;
				beatCount ++;
				for (auto iter = beatCallbacks.begin(); iter != beatCallbacks.end(); ++iter) {
					iter->second(beatCount);
				}
			}

			//cout << "Calc: " << (fmod(frameStart, bt)*2*M_PI)/bt << " Inc: " << phasor << endl;
		}
		float getDeltaTime(){return deltaTime;}
		void setFrameStart(float d){deltaTime = d - frameStart; frameStart = d; dTimes[dTime]=deltaTime; if(++dTime>8)dTime=0;/*cout << "Step: " << 1/deltaTime << " fps" << endl;*/}
		
		//"Real" time - time since the start of the song. Not affected by mods
		//"Song" time - time since start of the song. (DT/NC = x1.5,, HT = x0.66)
		//"AL" time - time since the start of pitch adjusted songs (NC = x1.5)
		void setPlaybackFrameStart(double d)
		{
			//OpenAL updates the reported audio time every few frames so we must interpolate the more reliable value from glfw
			
			double frameInc = d - frameSet;				//Real time since last AL update
			
			frameInc*=currentBeatmap.alSpeedModifier;	//AL time since last update (ie NC = 1.5x)
			
			//cout << "fInc " << d  << endl;
			float frameStartn = frameInc + frameSet;	//Current AL time without interpolation
			deltaTime = frameStartn - frameStart;		//Change in AL time since last frame
			frameStart = frameStartn;					
			songTime = frameStart * currentBeatmap.alTimeModifier;
			dTimes[dTime]=deltaTime; if(++dTime>7)dTime=0;
		}
		double getFrameStart(){
			return frameStart;			//Returns AL time since start of song
		}
		
		double getSongTime()			//Returns song time
		{
			return songTime;
		}
		
		MiniSongInfo getCurrentSong() const{return currentSong;}
	
		GraphicsDB graphicsDB;
		
		ShaderManager* shaderManager;
		
		SongSettings currentBeatmap;
		
		VAOStore vaoStore;
		
		GlobalInfo();
		
		int addBeatCallback(std::function<void(int)> const &cb)
		{
			beatCallbacks[beatCallbacksCount] = cb;
			beatCallbacksCount++;
		}
		void removeBeatCallback(int bc)
		{
			beatCallbacks.erase(bc);
		}
		
		sth_stash* getFontStash()
		{
			return fontStash;
		}
		
		InputHandler* getInputHandler(){return inputHandler;}
		
		CursorTrail getCursorTrail(){return cursorTrail;}
		
		float getMouseY(){return mouseY;}
		float getMouseX(){return mouseX;}
		array<float, 2> getMouseGrid(){return mouseGrid;}
		
		//GraphicsOptions graphics;
		//ShaderManager shaderManager;
		//TextureRepository textureRepository;
		
		void updateUBO_Pixel();
		void updateUBO_Float();
		void updateUBO_Grid();
		
		inline void genPixelProjectionMatrix(float w, float h){projPx.projectionMatrix = glm::ortho(0.f, w, 0.f, h, -100.f, 100.f); updateUBO_Pixel();}
		inline void genFloatProjectionMatrix(float w, float h){projFl.projectionMatrix = glm::ortho(-w/2, w/2, -h/2, h/2, -100.f, 100.f); updateUBO_Float();}
		inline void genGridProjectionMatrix()
		{
			float r = winW/float(winH);
			
			float x = 712*r - 512;
			x/=2;
			projGr.projectionMatrix = glm::ortho(-x, 512.f + x, -100.f, 612.f, -100.f, 100.f);
			
			pxToGrid = 712.f/winH;
			gridToPx = 1/pxToGrid;
			
			//projGr.projectionMatrix = glm::ortho(0.f, (float)winW, 0.f, (float)winH, -100.f, 100.f);
			
			updateUBO_Grid();
		}
		
		inline void bindPixelProjection(){glBindBufferBase(GL_UNIFORM_BUFFER, UB::ProjectionMatrix, uboPixel);}
		inline void bindFloatProjection(){glBindBufferBase(GL_UNIFORM_BUFFER, UB::ProjectionMatrix, uboFloat);}
		inline void bindGridProjection(){glBindBufferBase(GL_UNIFORM_BUFFER, UB::ProjectionMatrix, uboGrid);}
	
	
		int getWindowWidth(){return winW;}
		int getWindowHeight(){return winH;}
		void setWindowSize(int w, int h){winW = w; winH = h;}
		void setController(Controller* c){controller = c;}
		Controller* getController(){return controller;}
		const Directories& getDirectory(){return directories;}
		
		void setInputHandler(InputHandler* ih){
			if (inputHandler!=NULL)delete inputHandler;
			inputHandler = ih;
		}
		
		GLFWwindow* getWindow(){return window;}
		
		void init();
		
		void beginFrame();
		void endFrame();
		
		float getPxToGrid(){return pxToGrid;}	//Conversion factor only
		float getGridToPx(){return gridToPx;}	
		
		array<float, 2> convPxToGrid(array<float, 2> pos)
		{
			float r = winW/float(winH);
			
			float x = 712*r - 512;
			x/=2;
			projGr.projectionMatrix = glm::ortho(-x, 512.f + x, -100.f, 612.f, -100.f, 100.f);
			
			//grid is 512 + x wide, 712 high
			//zero point is (x, 100) units from corner of screen
			
			pos[0]*=pxToGrid;
			pos[1]*=pxToGrid;
			
			pos[0]-=x;
			pos[1]-=100;
			
			return pos;
			
		}
			
	protected:
		
		void setWindow(GLFWwindow* w){window = w;}
		
		inline void cursorPosCallback(GLFWwindow*, double x, double y){
			mouseX = x;
			mouseY = winH - y - 1;
			
			if(cursorTrail.n<16)
			{
				cursorTrail.x[cursorTrail.n] = mouseX;
				cursorTrail.y[cursorTrail.n] = mouseY;
				cursorTrail.time[cursorTrail.n] = glfwGetTime();
				cursorTrail.n++;
			}
			
			//cout << mouseX << " " << mouseY << " " << glfwGetTime() << endl;
		}
		
		inline void mouseClickCallback(GLFWwindow*, int button, int action){
			//std::cout << "Mouse click globalinfo callback" << std::endl;
			
			inputHandler->handleMouseButton(button, action);
		}
		
		inline void keyCallback(GLFWwindow*, int button, int action){

			
			inputHandler->handleKeyboardEvent(button, action);
		}
		
		
		
		
};

#endif

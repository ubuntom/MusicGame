#include <DB/GraphicsDB.hpp>
#include <infoContainer/GlobalInfo.hpp>

//! A class that updates every frame
class Animation
{
	protected:
		GlobalInfo& globalInfo;
	public:
		Animation(GlobalInfo& gi):
			globalInfo(gi)
		{}
		
		virtual void render()=0;
		
};

//! Pulses an image
class AnimationBeatPulsar: public Animation
{
	protected:
		GraphicID graphic;
		bool pixel=false;				//!< Whether to use pixel or float values
		float iX=0, iY=0;			//!< X and Y offsets
		float scale=1.5;			//!< Scale factor
		float fadeLength=1;		//!< Beats to fadeout 
		float transp=0.4;			//!< Initial transparency
		
		
		deque<float> ghosts;	//!< In progress fadeouts, and their beat offsets
	
		
		void beatCallback(int beatCount);
		int beatCallbackID;
	
	
	public:
	
		AnimationBeatPulsar(GlobalInfo& gi, GraphicID g):
			Animation(gi), graphic(g)
		{
			beatCallbackID = globalInfo.addBeatCallback(std::bind(&AnimationBeatPulsar::beatCallback, this, std::placeholders::_1));
		}
	
		virtual void render();
};

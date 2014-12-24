#ifndef GRAPHICSDB_H
#define GRAPHICSDB_H


#include "Graphic.hpp"
#include <map>



//!Identifier for each graphic
enum class GraphicID
{
	menu_button_background,
	osu_logo_small,
	cursor,
	slider_background,
	slider_outline,
	hitcircle,
	hitcircleoverlay,
	approachcircle,
	hit0,
	hit50,
	hit100,
	hit300,
	sliderfollowcircle,
	sliderscorepoint,
	reversearrow,
	spinner_circle,
	MAX
};



//! Image sizes



//! Store for all game graphics
class GraphicsDB
{
	Graphic* graphics[(int)GraphicID::MAX-1];
	
	GlobalInfo& globalInfo;
	
	public:
		GraphicsDB(GlobalInfo&);
	
		std::map<GraphicID, GraphicMetric> GraphicMetrics;
		
		const Graphic* fetch(GraphicID gi)
		{
			return graphics[(int)gi];
		}
	
		void loadBase();
};

#endif




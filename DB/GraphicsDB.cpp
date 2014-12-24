#include "GraphicsDB.hpp"

GraphicsDB::GraphicsDB(GlobalInfo& gi):
globalInfo(gi),
GraphicMetrics({
    std::make_pair(GraphicID::menu_button_background, GraphicMetric(724, 127, 0.5, 0.5, FF_LIMITMAX, PX_EQUAL)),
    std::make_pair(GraphicID::osu_logo_small, GraphicMetric(173, 173, 0.5, 0.5, FF_LIMITMAX, PX_EQUAL)),
    std::make_pair(GraphicID::cursor, GraphicMetric(64, 64, 0, 0, FF_ABSOLUTE, PX_IGNORE, CENTRE)),
    std::make_pair(GraphicID::slider_background, GraphicMetric(128, 2, 0, 0, FF_LIMITMAX, PX_EQUAL)),
    std::make_pair(GraphicID::slider_outline, GraphicMetric(128, 2, 0, 0, FF_LIMITMAX, PX_EQUAL)),
    std::make_pair(GraphicID::hitcircle, GraphicMetric(128, 128, 1, 1, FF_ABSOLUTE, PX_EQUAL)),
    std::make_pair(GraphicID::hitcircleoverlay, GraphicMetric(128, 128, 1, 1, FF_ABSOLUTE, PX_EQUAL)),
    std::make_pair(GraphicID::approachcircle, GraphicMetric(128, 128, 1, 1, FF_ABSOLUTE, PX_EQUAL)),
    std::make_pair(GraphicID::hit0, GraphicMetric(256, 256, 2, 2, FF_ABSOLUTE)),
    std::make_pair(GraphicID::hit50, GraphicMetric(256, 256, 2, 2, FF_ABSOLUTE)),
    std::make_pair(GraphicID::hit100, GraphicMetric(256, 256, 2, 2, FF_ABSOLUTE)),
    std::make_pair(GraphicID::hit300, GraphicMetric(256, 256, 2, 2, FF_ABSOLUTE)),
    std::make_pair(GraphicID::sliderfollowcircle, GraphicMetric(256, 256, 1, 1, FF_LIMITMAX, PX_EQUAL)),
    std::make_pair(GraphicID::sliderscorepoint, GraphicMetric(32, 32, 0.5, 0.5, FF_RELATIVE, PX_LEQUAL)),
    std::make_pair(GraphicID::reversearrow, GraphicMetric(128, 128, 1, 1, FF_ABSOLUTE, PX_LEQUAL)),
    std::make_pair(GraphicID::spinner_circle, GraphicMetric(666, 666, 1, 1, FF_ABSOLUTE, PX_IGNORE)),
    std::make_pair(GraphicID::MAX, GraphicMetric(0,0,0,0, FF_ABSOLUTE, PX_EQUAL))
})
{}

void GraphicsDB::loadBase()
{
		GraphicBitmapRGBA* temp;
		
		graphics[(int)GraphicID::menu_button_background] = temp = new GraphicBitmapRGBA(globalInfo);
		temp->loadImage("../oslin-skin/menu-button-background.png", GraphicMetrics.at(GraphicID::menu_button_background));
		
		graphics[(int)GraphicID::osu_logo_small] = temp = new GraphicBitmapRGBA(globalInfo);
		temp->loadImage("../oslin-skin/osu-logo-small.png", GraphicMetrics.at(GraphicID::osu_logo_small));
		
		graphics[(int)GraphicID::cursor] = temp = new GraphicBitmapRGBA(globalInfo);
		temp->loadImage("../oslin-skin/cursor.png", GraphicMetrics.at(GraphicID::cursor));
		
		graphics[(int)GraphicID::slider_background] = temp = new GraphicBitmapRGBA(globalInfo);
		temp->loadImage("../oslin-skin/slider-background.png", GraphicMetrics.at(GraphicID::slider_background));
		
		graphics[(int)GraphicID::slider_outline] = temp = new GraphicBitmapRGBA(globalInfo);
		temp->loadImage("../oslin-skin/slider-outline.png", GraphicMetrics.at(GraphicID::slider_outline));
		
		graphics[(int)GraphicID::hitcircle] = temp = new GraphicBitmapRGBA(globalInfo);
		temp->loadImage("../oslin-skin/hitcircle.png", GraphicMetrics.at(GraphicID::hitcircle));
		
		graphics[(int)GraphicID::hitcircleoverlay] = temp = new GraphicBitmapRGBA(globalInfo);
		temp->loadImage("../oslin-skin/hitcircleoverlay.png", GraphicMetrics.at(GraphicID::hitcircleoverlay));
		
		graphics[(int)GraphicID::approachcircle] = temp = new GraphicBitmapRGBA(globalInfo);
		temp->loadImage("../oslin-skin/approachcircle.png", GraphicMetrics.at(GraphicID::approachcircle));
		
		graphics[(int)GraphicID::hit0] = temp = new GraphicBitmapRGBA(globalInfo);
		temp->loadImage("../oslin-skin/hit0.png", GraphicMetrics.at(GraphicID::hit0));
		
		graphics[(int)GraphicID::hit50] = temp = new GraphicBitmapRGBA(globalInfo);
		temp->loadImage("../oslin-skin/hit50.png", GraphicMetrics.at(GraphicID::hit50));
		
		graphics[(int)GraphicID::hit100] = temp = new GraphicBitmapRGBA(globalInfo);
		temp->loadImage("../oslin-skin/hit100.png", GraphicMetrics.at(GraphicID::hit100));
		
		graphics[(int)GraphicID::hit300] = temp = new GraphicBitmapRGBA(globalInfo);
		temp->loadImage("../oslin-skin/hit300.png", GraphicMetrics.at(GraphicID::hit300));
		
		graphics[(int)GraphicID::sliderfollowcircle] = temp = new GraphicBitmapRGBA(globalInfo);
		temp->loadImage("../oslin-skin/sliderfollowcircle.png", GraphicMetrics.at(GraphicID::sliderfollowcircle));
		
		graphics[(int)GraphicID::sliderscorepoint] = temp = new GraphicBitmapRGBA(globalInfo);
		temp->loadImage("../oslin-skin/sliderscorepoint.png", GraphicMetrics.at(GraphicID::sliderscorepoint));
		
		graphics[(int)GraphicID::reversearrow] = temp = new GraphicBitmapRGBA(globalInfo);
		temp->loadImage("../oslin-skin/reversearrow.png", GraphicMetrics.at(GraphicID::reversearrow));
		
		graphics[(int)GraphicID::spinner_circle] = temp = new GraphicBitmapRGBA(globalInfo);
		temp->loadImage("../oslin-skin/spinner-circle.png", GraphicMetrics.at(GraphicID::spinner_circle));
		
		
}

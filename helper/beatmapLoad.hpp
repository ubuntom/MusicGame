#include <vector>
#include <string>
#include <Hit/HitCircle.hpp>


class GlobalInfo;

class BeatmapLoader
{
protected:
	GlobalInfo& globalInfo;
	
	HitObjectManager* hom;
	
	std::vector<std::string> myLines;
	int lineIndex;
	void loadHitObjects();
	
public:
	BeatmapLoader(GlobalInfo&);
	void load(const std::string);
	
	inline HitObjectManager* getHOM(){return hom;}
	
	
};

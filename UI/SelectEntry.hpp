#ifndef SELECTENTRY_H
#define SELECTENTRY_H

#include <vector>
#include <string>
#include <infoContainer/SceneInfo.hpp>
#include <Flair/LineStream.hpp>

//! Base Interface for map/song menu button
class SelectEntry{

	SceneInfo& sceneInfo;
	
	float mouseOver=0.f;

	
	public:
		SelectEntry(SceneInfo& si):
			sceneInfo(si)
		{}
		
		virtual ~SelectEntry(){}
		
		virtual float getHeight()=0;	//! Calculate the height of this item
		
		virtual void render()=0;
		
		int mouseOverOffset(bool);
	
};

//! Button for individual maps in single player
class SingleMapSubEntry: public SelectEntry
{
	int difficulty; //!< Difficulty from 0 (0 stars) to 9 (5 stars)
	std::string text;	//!< Name of difficulty
	
	public:
		SingleMapSubEntry(SceneInfo& si):
			SelectEntry(si)
		{}
	
		virtual float getHeight();

		virtual void render(){}

};


//! Button for songs in single player
class SingleSongEntry: public SelectEntry
{
	std::string text;	//!< Name of song
	
	std::vector<SingleMapSubEntry> subMaps;	//The maps that make up this song. Size==0 unless clicked
	
	public:
		SingleSongEntry(SceneInfo& si):
			SelectEntry(si)
		{}
	
		virtual float getHeight();
		
		virtual void render();
};

class SelectEntryManager
{
	GlobalInfo& globalInfo;
	
	vector<SelectEntry*> songEntries;
	
	double startTime;
	
	int selected;
	
	float topOffset;
	
	LineStream lineStream;
	
	float selectedOffset=0;
	
	void lineStreamGen(int beatCount);
	int beatCallbackID;
	
	public:
	
		SelectEntryManager(GlobalInfo& gi);
	
		void render();
		
		void push(SelectEntry* se){songEntries.push_back(se);}
		
		bool handleClick(int b, int a);
		
		
	
};

#endif

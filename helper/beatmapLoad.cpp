#include "beatmapLoad.hpp"
#include <infoContainer/GlobalInfo.hpp>
#include <helpful.hpp>
#include <iterator>
#include <Bezier.hpp>
#include <Spinner.hpp>

BeatmapLoader::BeatmapLoader(GlobalInfo& gi):
globalInfo(gi)
{
	hom = new HitObjectManager(globalInfo);
}

void BeatmapLoader::load(const string filename)
{
	
	ifstream mapfile(filename);
	
    if(!mapfile) //Always test the file open.
    {
		throw BeatmapOpenError(filename.c_str());
    }
    
   
	std::copy(std::istream_iterator<std::string>(mapfile),
          std::istream_iterator<std::string>(),
          std::back_inserter(myLines));
          
    
    lineIndex=0;  
	for(;lineIndex<myLines.size();lineIndex++)
	{
		if(myLines[lineIndex][0]=='[')
		{
			if(myLines[lineIndex]=="[HitObjects]")
			{
				lineIndex++;
				loadHitObjects();
			}
		}
	}
          
}


void BeatmapLoader::loadHitObjects()
{
	cout << "Loading hit objects" << endl;
	
	cout << lineIndex << " " << myLines.size() <<  endl;
	
	while(lineIndex<myLines.size() && myLines[lineIndex][0]!='[')
	{
		std::vector<std::string> data;
		split(myLines[lineIndex], ',', data);
		cout << lineIndex << " " << data.size() << endl;
		if(data.size()==5)
		{
			hom->future.push_back(new HitCircle(globalInfo, atoi(data[0].c_str()), atoi(data[1].c_str()), atoi(data[2].c_str())/1000.));
			cout << "pB " << data[0] << " " << data[1] << " " << data[2] << endl;
		}
		else if(data.size()==6)
		{
			hom->future.push_back(new Spinner(globalInfo, atoi(data[2].c_str())/1000., atoi(data[5].c_str())/1000.));
		}
		else if (data.size()>=8)
		{
			vector<vector<array<float, 2> > > points;
			
			points.push_back(vector<array<float, 2> >());
			points[0].push_back({{(float)atoi(data[0].c_str()), (float)atoi(data[1].c_str())}});
			
			std::vector<std::string> data2;
			split(data[5], '|', data2);
			for(int i=1;i<data2.size();i++)
			{
				std::vector<std::string> data3;
				split(data2[i], ':', data3);
			
				array<float, 2> test = {{(float)atoi(data3[0].c_str()), (float)atoi(data3[1].c_str())}};
				if(test == points.back().back())
				{
					points.push_back(vector<array<float, 2> >());
				}
				points.back().push_back({{(float)atoi(data3[0].c_str()), (float)atoi(data3[1].c_str())}});
			}
			
			for(int i=0;i<points.size();i++)
			{
				if(points[i].size()<=1)
				{
					points.erase(points.begin() + (i--));
					cout << "Erase me " << endl;
				}
			}
			

			if(points.size()>0)
			{
				float temp_bpm = 183;	//TODO change this value depending on timing sections
				//float beats = atoi(data[7].c_str()) / 183.;
				hom->future.push_back(new BezierTest(globalInfo, points, atoi(data[2].c_str())/1000.f, 1.f, atoi(data[7].c_str()), temp_bpm, atoi(data[6].c_str())));
				cout << "Bez" << endl;
			}
			
			else cout << "No bex" << endl;
		}
		else
		{
			cout << "Unexpected line in beatmap: " << myLines[lineIndex] << endl;
		}
		lineIndex++;
	}
	


}

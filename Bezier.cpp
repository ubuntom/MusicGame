#include "Bezier.hpp"
#include <cmath>
#include <Shader/allShaders/allShaders.hpp>
#include <infoContainer/GlobalInfo.hpp>
#include <iomanip>
#include <algorithm> 
using namespace std;

unsigned int nCr(unsigned int n, unsigned int k)
{
	if (k == 0 || k == n) return 1;
    if (k > n) return 0;
    if (k * 2 > n) k = n-k;

    int result = n;
    for( int i = 2; i <= k; ++i ) {
        result *= (n-i+1);
        result /= i;
    }
    return result;
}

//Calculate points along a bezier spline
//
//vector of the points
//vector to put the points in 
//vector to put the gradient at each point in
//vector to put the homogenous points in (evenly spaced points)
//the number of points
/*void calcBezierSpline(const vector<array<float, 2> >& points, vector<array<float, 2> >& out_Points, vector<float>& out_Gradient, vector<array<float, 2> >& out_Homogenous, int resolution)
{
	double t_inc = 1/double(resolution);
	int n = points.size()-1;
	
	double pylast;
	double pxlast;
	double length0=0;
	double length1=0;
	
	std::cout << std::setprecision(16);
	
	int t_count=0;
	
	
	for(;t_count<=resolution;t_count++)
	{
		double t = t_count / double(resolution);
		double px=0;
		double py=0;
		double dxdt=0;
		double dydt=0;
		
		for(int i=0;i<=n;i++)
		{
			double bint = nCr(n,i) * pow(t, i) * pow(1-t, n-i);
			px+=points[i][0]*bint;
			py+=points[i][1]*bint;
			
		}

		if(t>0)
		{
			//cout << "T: " << t << " " << sqrt((px-pxlast)*(px-pxlast) + (py-pylast)*(py-pylast)) << endl;
			length0+=sqrt((px-pxlast)*(px-pxlast) + (py-pylast)*(py-pylast));
		}
		
		pxlast = px;
		pylast = py;
		
		
		//dxdt *= n;
		//dydt *= n;
		
		//cout << nCr(n,0) << " " << nCr(n,1) << " " << nCr(n,2) << " " << nCr(n,3) << " " << nCr(n,4) << " " << nCr(n,5) << " " << nCr(n,6) << " " << nCr(n,7) << " " << nCr(n,8) << endl;
		
		for(int i=0;i<=n-1;i++)
		{
			double bint = nCr(n-1,i) * pow(t, i) * pow(1-t, n-i-1);
			dxdt+=(points[i+1][0] - points[i][0])*bint;
			dydt+=(points[i+1][1] - points[i][1])*bint;
		}
		
		double dydx = dxdt/dydt;
		
		out_Points.push_back({{(float)px, (float)py}});
		out_Gradient.push_back(atan2(dxdt, dydt));
		
		
		
	}
	double prev_t=0;
	for(t_count=0;t_count<=resolution;t_count++)
	{
		double try_t;
		if(t_count!=0)try_t = prev_t;
		else try_t = 0;
		
		double t = t_count / double(resolution);
		
		double line_pos = t*length0;
		double line_inc;
		if(t_count!=0)line_inc = line_pos - length0*(t_count-1)/double(resolution);
		else line_inc = 0;
		
		double px=0;
		double py=0;
		double dxdt=0;
		double dydt=0;
		
		bool iter_direction = true;
		double t_test = try_t;
		double last_less = prev_t;
		double last_greater = 1;
		double iter_inc = 0.01;
		
		cout << "LI " << t_test << " " << line_inc << endl;
		
		if(t_count!=0)
		{
		
			for(int j=0;j<15;j++)
			{
				px=0;
				py=0;
				//cout << "II " << t_test << endl;
				for(int i=0;i<=n;i++)
				{
					double bint = nCr(n,i) * pow(t_test, i) * pow(1-t_test, n-i);
					px+=points[i][0]*bint;
					py+=points[i][1]*bint;
					
				}
				double test_inc = sqrt((px-pxlast)*(px-pxlast) + (py-pylast)*(py-pylast));
				cout << "TI " << t_test << " " << test_inc << endl;
				if(test_inc < line_inc)
				{
					last_less = t_test;
					if(iter_direction==true)
					{
						t_test+=iter_inc;
					}
					else
					{
						iter_direction=true;
						iter_inc = (last_greater - t_test)/2;
						t_test+=iter_inc;
					}
				}
				else
				{
					
					last_greater = t_test;
					if(iter_direction==true)
					{
						iter_direction=false;
						iter_inc = (last_less-t_test)/2;
						t_test+=iter_inc;
					}
					else
					{
						
						t_test+=iter_inc;
					}
				}
				
			}
		}
		else
		{
			t_test = 0;
			px = points[0][0];
			py = points[0][1];
		}
		
		pxlast = px;
		pylast = py;
		
		
		//dxdt *= n;
		//dydt *= n;
		
		//cout << nCr(n,0) << " " << nCr(n,1) << " " << nCr(n,2) << " " << nCr(n,3) << " " << nCr(n,4) << " " << nCr(n,5) << " " << nCr(n,6) << " " << nCr(n,7) << " " << nCr(n,8) << endl;
		
		double dydx = dxdt/dydt;
		
		prev_t = t_test;
		
		cout << "P: " << px << " " << py << endl;
		
		out_Homogenous.push_back({{(float)px, (float)py}});
		
		
	}
	
	
	
	
	
	
	cout << "Length: " << length0 << " " << length1 << endl;
}*/

//Slider class
//
//_points is vector of bezier curves (each curve is made out of n arrays of length 2)
// _time The time at which it starts
// speed The speed the ball travels
// gridLength How long the slider is (grid units)
// _bpm bpm of the song during the slider
BezierTest::BezierTest(GlobalInfo& gi, vector<vector<array<float, 2> > > _points, float _time, float speed, float gridLength, float _bpm, int _repeat):
HitObject(gi)
{
	time = _time;
	
	bps = _bpm/60;		//Beats per second
	
	repeat = _repeat;
	
	/*for (int a=0;a<_points.size();a++)
	{
		for (int b=0;b<_points[a].size();b++)
		{
				cout << _points[a][b][0] << " " << _points[a][b][1] << endl;
		}
		cout << endl;
	}*/
	
	beatDuration = gridLength/(100 * speed);
	timeDuration = beatDuration / bps;
	
	beatDurationR = beatDuration * repeat;
	timeDurationR = timeDuration * repeat;
	
	missedStart = false;
	windowClose = time + globalInfo.currentBeatmap.getHitWindow()[2];
	
	for(int i=1;i<repeat;i++)
	{
		repeatTimes.push_back(time + timeDuration*i);
	}
	
	int numTicks = ceil((beatDuration / globalInfo.currentBeatmap.getTickRate()) - 1);
	maxScore = numTicks + 2;
	score=0;
	float tickInterval = globalInfo.currentBeatmap.getTickRate() / beatDuration;	//Gap between ticks as a factor of total bezier length (0-1)
	float tickTime = globalInfo.currentBeatmap.getTickRate() / bps;
	float firstTick = time + tickTime;
	
	cout << "BD " << time << " " << beatDuration << " " << globalInfo.currentBeatmap.getTickRate() << " " << speed << " " << _points.size() << endl;

	
	int max_length = gridLength;//beatDuration*speed*100;	//Ball travels at 100*speed units per beat. TODO int or float?
	cout << "MAX " << max_length << endl;
	float outlineRadius = globalInfo.currentBeatmap.getComputedCircleSize() * 0.9;
	float radius = outlineRadius * 0.9;
	
	beziers.resize(_points.size());
	
	for(int i=0;i<_points.size();i++)
	{
		beziers[i].points.reserve(_points[i].size());
		for(int j=0;j<_points[i].size();j++)
		{
			_points[i][j][1] = 512 - _points[i][j][1];
			beziers[i].points.push_back(_points[i][j]);
			cout << "P: " << _points[i][j][0] << " " << _points[i][j][1] << endl;;
		}
		cout << endl;
	}
	

	
	first = beziers.front().points.front();
	last = beziers.back().points.back();

	double length = 0;		//Current length of the slider as it is built
	totalPoints = 0;		//Number of points in the slider
	
	for(int z=0;z<beziers.size();z++)		//Build all the beziers
	{
		if(length<max_length)	//If the current length is < max then it's ok to build the next bezier
		{
			beziers[z].calcBezierSpline(length, max_length);
			length+=beziers[z].length;
			totalPoints += beziers[z].out_Points.size();
		}
		else
		{
			//Remove the last bezier
			beziers.pop_back();
			break;
		}
	}


	limit_last = beziers.back().out_Points.back();
	
	//cout << "siz: " << beziers[0].out_Points.size() << " " << beziers[1].out_Points.size() << endl;
	//cout << "len: " << length << " " << beziers[0].length << " " << beziers[1].length << endl;
	//cout << "nor: " << beziers[0].out_Gradient.back() << " " << beziers[1].out_Gradient.front()  << endl;

	
	double line_gap = length / 1000; //1000 segments
	
	/*double offset = beziers[0].calcHomogenous(0,line_gap);
	cout << "Boffset: " << offset << endl;
	
	offset = beziers[1].calcHomogenous(offset,line_gap);
	cout << "Boffset: " << offset << endl;*/
	
	double offset = 0;	//Offset to the end of the bezier section
	double homMax = max_length;
	for(int z=0;z<beziers.size();z++)
	{
		offset += beziers[z].calcHomogenous(offset,line_gap, homMax);
	}
	
	
	for(int z=0;z<beziers.size();z++)
	{
		out_Homogenous.insert(out_Homogenous.end(),beziers[z].out_Homogenous.begin(),beziers[z].out_Homogenous.end());
	}
	
	
	//Generate ticks
	cout << "Tick interval " << tickInterval << endl;
	for(int i=0; i<numTicks && (i+1)*tickInterval<1; i++)
	{
		array<float, 2> tickPos = out_Homogenous[int((i+1)*tickInterval*out_Homogenous.size())];
		
		ticks.push_back({{tickPos[0], tickPos[1], firstTick + tickTime*i}});
	}


	
	//calcBezierSpline(points, out_Points, out_Gradient, out_Homogenous, 1000);
	
	glGenVertexArrays(1, &vao);
	glGenVertexArrays(1, &vaoL);
	glGenVertexArrays(1, &vaoC);
	glGenBuffers(2, vbo);
	glGenBuffers(1, &vboL);
	glGenBuffers(1, &vboC);
	
	
	tris = totalPoints*2 - 2 + (beziers.size()-1)*12 - (beziers.size()-1)*2;
	
	float* n = new float[tris * 3 * 2];
	float* uv = new float[tris * 3 * 2];
	float* nl = new float[tris * 3 * 2];
	float* cap = new float[2*8*3*2];
	
	cout << "Max I " << 2*tris*2*3 << endl;
	
	int len = totalPoints*3;
	
	//memcpy(n, &out_Points[0], out_Points.size()*2*sizeof(float));
	
	//vector<array<float, 2> > inner;
	//vector<array<float, 2> > outer;
	
	vector<array<float, 2> > innerOutline;
	vector<array<float, 2> > outerOutline;
	
	int ioffset=0;
	for(int j=0;j<beziers.size();j++)
	{
		vector<array<float, 2> >& out_Points = beziers[j].out_Points;
		vector<float>& out_Gradient = beziers[j].out_Gradient;
		int i;
		for(i=0;i<beziers[j].out_Points.size();i++)
		{
			//float xoffset = sqrt(100/(1 + pow(out_Gradient[i],2)));
			//float yoffset = xoffset * out_Gradient[i];
			
			float xoffset = cos(out_Gradient[i]+1.57);
			float yoffset = sin(out_Gradient[i]+1.57);
			
			beziers[j].inner.push_back({{out_Points[i][0] - xoffset*radius, out_Points[i][1] - yoffset*radius}});
			beziers[j].outer.push_back({{out_Points[i][0] + xoffset*radius, out_Points[i][1] + yoffset*radius}});
			
			beziers[j].innerOutline.push_back({{out_Points[i][0] - xoffset*outlineRadius, out_Points[i][1] - yoffset*outlineRadius}});
			beziers[j].outerOutline.push_back({{out_Points[i][0] + xoffset*outlineRadius, out_Points[i][1] + yoffset*outlineRadius}});
			
			/*l[(ioffset + i)*3] = out_Points[i][0];
			l[(ioffset + i)*3+1] = out_Points[i][1];
			l[(ioffset + i)*3+2] = 1;*/
			
			
			
		}
		ioffset+=i;
	}
	cout << "tp " << totalPoints << endl;
	ioffset=0;
	for(int j=0;j<beziers.size();j++)
	{
		vector<array<float, 2> >& out_Points = beziers[j].out_Points;
		vector<array<float, 2> >& inner = beziers[j].inner;
		vector<array<float, 2> >& outer = beziers[j].outer;
		vector<array<float, 2> >& innerOutline = beziers[j].innerOutline;
		vector<array<float, 2> >& outerOutline = beziers[j].outerOutline;
		
		int i;
		for(i=0;i<out_Points.size()-1;i++)
		{
			innerOutline.push_back(inner[i]);
			outerOutline.push_back(outer[i]);
			
			//cout << "Wx " << ioffset << endl;
			
			n[(ioffset + i)*12] = inner[i][0];
			n[(ioffset + i)*12+1] = inner[i][1];
			n[(ioffset + i)*12+2] = outer[i][0];
			n[(ioffset + i)*12+3] = outer[i][1];
			n[(ioffset + i)*12+4] = outer[i+1][0];
			n[(ioffset + i)*12+5] = outer[i+1][1];
			n[(ioffset + i)*12+6] = inner[i][0];
			n[(ioffset + i)*12+7] = inner[i][1];
			n[(ioffset + i)*12+8] = outer[i+1][0];
			n[(ioffset + i)*12+9] = outer[i+1][1];
			n[(ioffset + i)*12+10] = inner[i+1][0];
			n[(ioffset + i)*12+11] = inner[i+1][1];
			
			nl[(ioffset + i)*12] = innerOutline[i][0];
			nl[(ioffset + i)*12+1] = innerOutline[i][1];
			nl[(ioffset + i)*12+2] = outerOutline[i][0];
			nl[(ioffset + i)*12+3] = outerOutline[i][1];
			nl[(ioffset + i)*12+4] = outerOutline[i+1][0];
			nl[(ioffset + i)*12+5] = outerOutline[i+1][1];
			nl[(ioffset + i)*12+6] = innerOutline[i][0];
			nl[(ioffset + i)*12+7] = innerOutline[i][1];
			nl[(ioffset + i)*12+8] = outerOutline[i+1][0];
			nl[(ioffset + i)*12+9] = outerOutline[i+1][1];
			nl[(ioffset + i)*12+10] = innerOutline[i+1][0];
			nl[(ioffset + i)*12+11] = innerOutline[i+1][1];
			
			
			uv[(ioffset + i)*12] = 0;
			uv[(ioffset + i)*12+1] = 0;
			uv[(ioffset + i)*12+2] = 1;
			uv[(ioffset + i)*12+3] = 0;
			uv[(ioffset + i)*12+4] = 1;
			uv[(ioffset + i)*12+5] = 1;
			
			uv[(ioffset + i)*12+6] = 0;
			uv[(ioffset + i)*12+7] = 0;
			uv[(ioffset + i)*12+8] = 1;
			uv[(ioffset + i)*12+9] = 1;
			uv[(ioffset + i)*12+10] = 0;
			uv[(ioffset + i)*12+11] = 1;
				
			//cout << "last: " << (ioffset + i)*12+11 << endl;
		}
		ioffset+=i;
		
		if(j<beziers.size()-1)	//Add in the pie slice between bezier segments
		{
			
			float g1 = beziers[j].out_Gradient.back();
			float g2 = beziers[j+1].out_Gradient.front();
						
			//cout << "Lx " << ioffset << endl;
			
			if(g1 > g2)
			{
				//Curve on inner
				cout << "INNER" << endl;
				array<float, 2> pivot = out_Points.back();
				float gradient_inc = (g2 - g1)/12;
				
				for(int z=0;z<6;z++)
				{
					float xoffset0 = sin(g1);
					float yoffset0 = cos(g1);
					g1+=gradient_inc;
					float xoffset1 = sin(g1);
					float yoffset1 = cos(g1);
					g1+=gradient_inc;
					float xoffset2 = sin(g1);
					float yoffset2 = cos(g1);
					
					n[(ioffset)*12] = pivot[0];
					n[(ioffset)*12+1] = pivot[1];
					n[(ioffset)*12+2] = pivot[0] - radius*xoffset0;
					n[(ioffset)*12+3] = pivot[1] + radius*yoffset0;
					n[(ioffset)*12+4] = pivot[0] - radius*xoffset1;
					n[(ioffset)*12+5] = pivot[1] + radius*yoffset1;
					n[(ioffset)*12+6] = pivot[0];
					n[(ioffset)*12+7] = pivot[1];
					n[(ioffset)*12+8] = pivot[0] - radius*xoffset1;
					n[(ioffset)*12+9] = pivot[1] + radius*yoffset1;
					n[(ioffset)*12+10] = pivot[0] - radius*xoffset2;
					n[(ioffset)*12+11] = pivot[1] + radius*yoffset2;
					
					nl[(ioffset)*12] = pivot[0];
					nl[(ioffset)*12+1] = pivot[1];
					nl[(ioffset)*12+2] = pivot[0] - outlineRadius*xoffset0;
					nl[(ioffset)*12+3] = pivot[1] + outlineRadius*yoffset0;
					nl[(ioffset)*12+4] = pivot[0] - outlineRadius*xoffset1;
					nl[(ioffset)*12+5] = pivot[1] + outlineRadius*yoffset1;
					nl[(ioffset)*12+6] = pivot[0];
					nl[(ioffset)*12+7] = pivot[1];
					nl[(ioffset)*12+8] = pivot[0] - outlineRadius*xoffset1;
					nl[(ioffset)*12+9] = pivot[1] + outlineRadius*yoffset1;
					nl[(ioffset)*12+10] = pivot[0] - outlineRadius*xoffset2;
					nl[(ioffset)*12+11] = pivot[1] + outlineRadius*yoffset2;

					uv[(ioffset)*12] = 0.5;
					uv[(ioffset)*12+1] = 0;
					uv[(ioffset)*12+2] = 1;
					uv[(ioffset)*12+3] = 0;
					uv[(ioffset)*12+4] = 1;
					uv[(ioffset)*12+5] = 1;
					
					uv[(ioffset)*12+6] = 0.5;
					uv[(ioffset)*12+7] = 0;
					uv[(ioffset)*12+8] = 1;
					uv[(ioffset)*12+9] = 0;
					uv[(ioffset)*12+10] = 1;
					uv[(ioffset)*12+11] = 1;
					
					ioffset++;
				}
			}
			else
			{
				//Curve on outer
				cout << "OUTER" << endl;
				array<float, 2> pivot = out_Points.back();
				float gradient_inc = (g2 - g1)/12;
				
				for(int z=0;z<6;z++)
				{
					
					float xoffset0 = sin(g1);
					float yoffset0 = cos(g1);
					g1+=gradient_inc;
					float xoffset1 = sin(g1);
					float yoffset1 = cos(g1);
					g1+=gradient_inc;
					float xoffset2 = sin(g1);
					float yoffset2 = cos(g1);
					
					n[(ioffset)*12] = pivot[0];
					n[(ioffset)*12+1] = pivot[1];
					n[(ioffset)*12+2] = pivot[0] + radius*xoffset0;
					n[(ioffset)*12+3] = pivot[1] - radius*yoffset0;
					n[(ioffset)*12+4] = pivot[0] + radius*xoffset1;
					n[(ioffset)*12+5] = pivot[1] - radius*yoffset1;
					n[(ioffset)*12+6] = pivot[0];
					n[(ioffset)*12+7] = pivot[1];
					n[(ioffset)*12+8] = pivot[0] + radius*xoffset1;
					n[(ioffset)*12+9] = pivot[1] - radius*yoffset1;
					n[(ioffset)*12+10] = pivot[0] + radius*xoffset2;
					n[(ioffset)*12+11] = pivot[1] - radius*yoffset2;
					
					nl[(ioffset)*12] = pivot[0];
					nl[(ioffset)*12+1] = pivot[1];
					nl[(ioffset)*12+2] = pivot[0] + outlineRadius*xoffset0;
					nl[(ioffset)*12+3] = pivot[1] - outlineRadius*yoffset0;
					nl[(ioffset)*12+4] = pivot[0] + outlineRadius*xoffset1;
					nl[(ioffset)*12+5] = pivot[1] - outlineRadius*yoffset1;
					nl[(ioffset)*12+6] = pivot[0];
					nl[(ioffset)*12+7] = pivot[1];
					nl[(ioffset)*12+8] = pivot[0] + outlineRadius*xoffset1;
					nl[(ioffset)*12+9] = pivot[1] - outlineRadius*yoffset1;
					nl[(ioffset)*12+10] = pivot[0] + outlineRadius*xoffset2;
					nl[(ioffset)*12+11] = pivot[1] - outlineRadius*yoffset2;

					uv[(ioffset)*12] = 0.5;
					uv[(ioffset)*12+1] = 0;
					uv[(ioffset)*12+2] = 1;
					uv[(ioffset)*12+3] = 0;
					uv[(ioffset)*12+4] = 1;
					uv[(ioffset)*12+5] = 1;
					
					uv[(ioffset)*12+6] = 0.5;
					uv[(ioffset)*12+7] = 0;
					uv[(ioffset)*12+8] = 1;
					uv[(ioffset)*12+9] = 0;
					uv[(ioffset)*12+10] = 1;
					uv[(ioffset)*12+11] = 1;
					
					ioffset++;
				}
				
			}
		}
		
		cout << "Index: " << (ioffset)*12+11 << endl;
		cout << "io " << ioffset << endl;
	}
	
	
	//Generate the 2 end caps for the stencil test
	array<float, 2> pivot = first;
	float angle = beziers.front().out_Gradient.front();
	float gapRadius = outlineRadius*0.95;
	for(int i=0;i<8;i++)
	{
		float c = outlineRadius * cos(angle);
		float s = outlineRadius * sin(angle);
		
		cap[i*6 + 0] = pivot[0];
		cap[i*6 + 1] = pivot[1];
		cap[i*6 + 2] = pivot[0] - gapRadius * cos(angle);
		cap[i*6 + 3] = pivot[1] + gapRadius * sin(angle);
		cap[i*6 + 4] = pivot[0] - gapRadius * cos(angle+M_PI/(4*2));
		cap[i*6 + 5] = pivot[1] + gapRadius * sin(angle+M_PI/(4*2));
		
		angle += M_PI/(4*2);
	}
	pivot = limit_last;
	angle = beziers.back().out_Gradient.back();
	for(int i=8;i<16;i++)
	{
		float c = outlineRadius * cos(angle);
		float s = outlineRadius * sin(angle);
		
		cap[i*6 + 0] = pivot[0];
		cap[i*6 + 1] = pivot[1];
		cap[i*6 + 2] = pivot[0] + gapRadius * cos(angle);
		cap[i*6 + 3] = pivot[1] - gapRadius * sin(angle);
		cap[i*6 + 4] = pivot[0] + gapRadius * cos(angle+M_PI/(4*2));
		cap[i*6 + 5] = pivot[1] - gapRadius * sin(angle+M_PI/(4*2));
		
		angle += M_PI/(4*2);
	}
	
	glBindVertexArray(vao);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, tris * 3 * 2 * sizeof(GLfloat), n, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, out_Points.size() * 3 * sizeof(GLfloat) * 3, l, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, tris * 3 * 2 * sizeof(GLfloat), uv, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	
	glBindVertexArray(0);

	glBindVertexArray(vaoL);
	
	glBindBuffer(GL_ARRAY_BUFFER, vboL);
	glBufferData(GL_ARRAY_BUFFER, tris * 3 * 2 * sizeof(GLfloat), nl, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	
	glBindVertexArray(0);
	
	glBindVertexArray(vaoC);
	glBindBuffer(GL_ARRAY_BUFFER, vboC);
	glBufferData(GL_ARRAY_BUFFER, 2 * 8 * 3 * 2 * sizeof(GLfloat), cap, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);
	
	cout << "Tris: " << tris << endl;
	
	float xMin, yMin;
	float xMax = xMin = nl[0];
	float yMax = yMin = nl[1];
	
	for(int i=1;i<tris*3;i++)
	{
		if(nl[i*2]<xMin)		xMin=nl[i*2];
		else if(nl[i*2]>xMax)	xMax=nl[i*2];
		
		if(nl[i*2+1]<yMin)		yMin=nl[i*2+1];
		else if(nl[i*2+1]>yMax)	yMax=nl[i*2+1];
		
	}
	
	{
		float capXmin = first[0] - globalInfo.currentBeatmap.getComputedCircleSize();
		float capXmax = first[0] + globalInfo.currentBeatmap.getComputedCircleSize();
		float capYmin = first[1] - globalInfo.currentBeatmap.getComputedCircleSize();
		float capYmax = first[1] + globalInfo.currentBeatmap.getComputedCircleSize();
		
		if(capXmin<xMin)	xMin=capXmin;
		if(capXmax>xMax)	xMax=capXmax;
		
		if(capYmin<yMin)	yMin=capYmin;
		if(capYmax>yMax)	yMax=capYmax;

	}
	
	{
		float capXmin = limit_last[0] - globalInfo.currentBeatmap.getComputedCircleSize();
		float capXmax = limit_last[0] + globalInfo.currentBeatmap.getComputedCircleSize();
		float capYmin = limit_last[1] - globalInfo.currentBeatmap.getComputedCircleSize();
		float capYmax = limit_last[1] + globalInfo.currentBeatmap.getComputedCircleSize();
		
		if(capXmin<xMin)	xMin=capXmin;
		if(capXmax>xMax)	xMax=capXmax;
		
		if(capYmin<yMin)	yMin=capYmin;
		if(capYmax>yMax)	yMax=capYmax;
	}
		


	
	cout << xMax << " max/min " << xMin << endl;
	
	
	
	dimensions[0] = xMax - xMin;
	dimensions[1] = yMax - yMin;
	originOffset[0] = xMin;
	originOffset[1] = yMin;
	pxDimensions[0] = dimensions[0] * globalInfo.getGridToPx();
	pxDimensions[1] = dimensions[1] * globalInfo.getGridToPx();
	
	
	glGenFramebuffers(1, &sliderFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, sliderFBO);

	glGenTextures(1, &sliderTexture);
	glBindTexture(GL_TEXTURE_2D, sliderTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pxDimensions[0], pxDimensions[1], 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, sliderTexture, 0);
	
	glGenRenderbuffers(1, &sliderStencil);
	glBindRenderbuffer(GL_RENDERBUFFER, sliderStencil);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, pxDimensions[0], pxDimensions[1]);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, sliderStencil);
	
	
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){throw SliderFramebufferError(glCheckFramebufferStatus(GL_FRAMEBUFFER));}
	
	
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	

	
	glGenFramebuffers(1, &sliderFBOmsaa);
	glBindFramebuffer(GL_FRAMEBUFFER, sliderFBOmsaa);
	


	glGenTextures(1, &sliderTexturemsaa);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, sliderTexturemsaa);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA, pxDimensions[0], pxDimensions[1], 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, sliderTexturemsaa, 0);
	
	
	glGenTextures(1, &sliderStencilmsaa);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, sliderStencilmsaa);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_DEPTH24_STENCIL8, pxDimensions[0], pxDimensions[1], 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, sliderStencilmsaa, 0);
	




	unsigned int a = glGetError();
	if (a!=0)cout << "CLEAa " << a << endl;
	
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){throw SliderFramebufferError(glCheckFramebufferStatus(GL_FRAMEBUFFER));}
	
	glStencilMask(0xFF);
	glClearStencil(0x00);
	glClear(GL_STENCIL_BUFFER_BIT);
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	

	
	fadeTime = time + timeDurationR + 0.25;
}

void BezierTest::renderFuture()
{
	globalInfo.shaderManager->bind(SH_BlitTexture);
	
	globalInfo.bindPixelProjection();
	
	
	cout << "REndering" << endl;
	unsigned int a;
	//cout << -originOffset[0] << " " << -originOffset[1] << " " << dimensions[0] << " " << dimensions[1] << endl;
	//glViewport(0, 0, 1600,768);
	
	GLuint offsetLoc = static_cast<const Shader_blitTexture*>(globalInfo.shaderManager->getShader(SH_BlitTexture))->getUniforms().offset;
	GLuint alphaLoc= static_cast<const Shader_blitTexture*>(globalInfo.shaderManager->getShader(SH_BlitTexture))->getUniforms().aF;
	GLuint colourLoc= static_cast<const Shader_blitTexture*>(globalInfo.shaderManager->getShader(SH_BlitTexture))->getUniforms().colour;
	GLuint scaleLoc= static_cast<const Shader_blitTexture*>(globalInfo.shaderManager->getShader(SH_BlitTexture))->getUniforms().scale;
	GLuint rotationLoc= static_cast<const Shader_blitTexture*>(globalInfo.shaderManager->getShader(SH_BlitTexture))->getUniforms().rotation;

	//cout << "D: " << dimensions[0] << endl;

	glUniform2f(offsetLoc, -originOffset[0]*globalInfo.getGridToPx(), -originOffset[1]*globalInfo.getGridToPx());
	glUniform2f(scaleLoc, globalInfo.getGridToPx(), globalInfo.getGridToPx());
	glUniform1f(alphaLoc, 1);
	
	glLineWidth(1);
	
	float timeDiff = globalInfo.getSongTime() - time;
	
	float p=(globalInfo.getFrameStart()-(time-globalInfo.currentBeatmap.getAR_float()));
	float f = p/globalInfo.currentBeatmap.getAR_float();
	
	
	
	int i = f*tris;
	if(i>tris) i = tris;
	else if(i<0)i=0;
	
	float radius = globalInfo.currentBeatmap.getComputedCircleSize() * (1 + -2 * timeDiff);
	
	//i=tris;
	
	//cout << "F! " << f << endl;
	
	glBindFramebuffer(GL_FRAMEBUFFER, sliderFBOmsaa);
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT);
	glBlendFuncSeparate(GL_ONE, GL_ONE, GL_ONE, GL_ONE);
	//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);

	//glDisable(GL_BLEND);
	
	a = glGetError();
	if (a!=0)cout << "Berr00 " << a << endl;
	
	
	glBlendEquationSeparate(GL_FUNC_ADD, GL_MAX);
	
	glEnable(GL_STENCIL_TEST);
	
	glStencilMask(~0);
	glClearStencil(0x00);
	glClear(GL_STENCIL_BUFFER_BIT);
	
	a = glGetError();
	if (a!=0)cout << "Berr01 " << a << endl;
	
	glStencilFunc(GL_ALWAYS, 2, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilMask(0xFF);
	
	glDisable(GL_CULL_FACE);
	
	glBindVertexArray(vaoC);
	glDrawArrays(GL_TRIANGLES,0,3*16);
	glBindVertexArray(0);
	
	
	a = glGetError();
	if (a!=0)cout << "Berr020 " << a << endl;
	
	
	glStencilFunc(GL_GEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilMask(0xFF);
	
	
	a = glGetError();
	if (a!=0)cout << "Berr021 " << a << endl;
	
	const GraphicBitmap* icon2 = static_cast<const GraphicBitmap*>(globalInfo.graphicsDB.fetch(GraphicID::slider_background));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, icon2->getTexture());
	
	a = glGetError();
	if (a!=0)cout << "Berr022 " << a << endl;
	
	glBindVertexArray(vao);
	
	a = glGetError();
	if (a!=0)cout << "Berr023 " << a << endl;
	
	//cout << i << " tris" << endl;
	glDrawArrays(GL_TRIANGLES,0,i*3);
	
	a = glGetError();
	if (a!=0)cout << "Berr030 " << a << endl;
	
	
	glStencilFunc(GL_EQUAL, 0, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glStencilMask(0x00);

	const GraphicBitmap* icon = static_cast<const GraphicBitmap*>(globalInfo.graphicsDB.fetch(GraphicID::slider_outline));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, icon->getTexture());
	
	a = glGetError();
	if (a!=0)cout << "Berr04 " << a << endl;
	
	glBlendEquationSeparate(GL_FUNC_ADD, GL_MAX);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glBindVertexArray(vaoL);
	glDrawArrays(GL_TRIANGLES,0,i*3);
	
	glDisable(GL_STENCIL_TEST);
	
	//cout << "Tx: " << (tris*3)/2 << " " << i*3 << " " << f << endl;
	
	
	a = glGetError();
	if (a!=0)cout << "Berr1 " << a << endl;
	
	/*GLvoid* pixels = malloc(1368*768);
	
	png::image<png::rgb_pixel> img(1368,768);

	unsigned int a = glGetError();
	if (a!=0)cout << "Eaa" << a << endl;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//glReadBuffer(GL_DEPTH_STENCIL_ATTACHMENT);
	glReadPixels(0,0, 1368 ,768, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, pixels);

    
    a = glGetError();
	if (a!=0)cout << "Eab" << a << endl;
    

    
    unsigned char* data = static_cast<unsigned char*>(pixels);
	
	auto pixel = img.get_pixel(0,0);
	pixel.green = 0;
	pixel.blue = 0;
	
	int count=0;
	for (int j=0;j<768;j++)
	{
		for(int i=0;i<1368;i++)
		{
			pixel.red = data[count];
			img.set_pixel(i,j,pixel);
			
			count+=1;
		}
	}
	
	img.write("stencil.png");
	
	int d;
	scanf("%d",&d);
	*/
	
	globalInfo.shaderManager->bind(SH_BlitTexture);
	
	glEnable(GL_BLEND);
	
	
	
	const GraphicBitmap* hit = static_cast<const GraphicBitmap*>(globalInfo.graphicsDB.fetch(GraphicID::hitcircle));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, hit->getTexture());
	
	const GraphicBitmap* hitoverlay = static_cast<const GraphicBitmap*>(globalInfo.graphicsDB.fetch(GraphicID::hitcircleoverlay));
	
	//TODO use grid and save the grid->px conversion?
	
	glBindVertexArray(hit->getVAOFl());
	glUniform2f(scaleLoc, globalInfo.currentBeatmap.getComputedCircleSize()*globalInfo.getGridToPx(), globalInfo.currentBeatmap.getComputedCircleSize()*globalInfo.getGridToPx());
	glUniform3f(colourLoc, 0, 1, 1);
	
	glUniform2f(offsetLoc, first[0]*globalInfo.getGridToPx()-originOffset[0]*globalInfo.getGridToPx(), first[1]*globalInfo.getGridToPx()-originOffset[1]*globalInfo.getGridToPx());	
	glDrawArrays(GL_TRIANGLES,0,6);
	
	if(f<1)
	{
		int io = out_Homogenous.size()*f;
		if(io>=out_Homogenous.size())io=out_Homogenous.size()-1;
		glUniform2f(offsetLoc, out_Homogenous[io][0]*globalInfo.getGridToPx()-originOffset[0]*globalInfo.getGridToPx(), out_Homogenous[io][1]*globalInfo.getGridToPx()-originOffset[1]*globalInfo.getGridToPx());
	}
	else
	{
		glUniform2f(offsetLoc, limit_last[0]*globalInfo.getGridToPx()-originOffset[0]*globalInfo.getGridToPx(), limit_last[1]*globalInfo.getGridToPx()-originOffset[1]*globalInfo.getGridToPx());
	}
	glDrawArrays(GL_TRIANGLES,0,6);
	
	glUniform3f(colourLoc, 1, 1, 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, hitoverlay->getTexture());
	
	glUniform2f(offsetLoc, first[0]*globalInfo.getGridToPx()-originOffset[0]*globalInfo.getGridToPx(), first[1]*globalInfo.getGridToPx()-originOffset[1]*globalInfo.getGridToPx());	
	glDrawArrays(GL_TRIANGLES,0,6);
	
	if(f<1)
	{
		int io = out_Homogenous.size()*f;
		if(io>=out_Homogenous.size())io=out_Homogenous.size()-1;
		glUniform2f(offsetLoc, out_Homogenous[io][0]*globalInfo.getGridToPx()-originOffset[0]*globalInfo.getGridToPx(), out_Homogenous[io][1]*globalInfo.getGridToPx()-originOffset[1]*globalInfo.getGridToPx());
	}
	else
	{
		glUniform2f(offsetLoc, limit_last[0]*globalInfo.getGridToPx()-originOffset[0]*globalInfo.getGridToPx(), limit_last[1]*globalInfo.getGridToPx()-originOffset[1]*globalInfo.getGridToPx());
	}
	glDrawArrays(GL_TRIANGLES,0,6);
	
	
	
	glBindVertexArray(0);
	
	a = glGetError();
	if (a!=0)cout << "Berr2 " << a << endl;
	
	//cout << "L " << last[0] << " " << last[1] << " . " << limit_last[0] << " " << limit_last[1] << endl;
	
	glUniform2f(offsetLoc, 0,0);
	
	/*glDrawArrays(GL_LINE_STRIP, 0, out_Points.size());
	glDrawArrays(GL_LINE_STRIP, out_Points.size(), out_Points.size());
	glDrawArrays(GL_LINE_STRIP, out_Points.size()*2, out_Points.size());*/
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindVertexArray(0);
	
	glViewport( 0, 0, globalInfo.getWindowWidth(), globalInfo.getWindowHeight());
	
	glBlendEquationSeparate(GL_FUNC_ADD, GL_MAX);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, sliderFBOmsaa);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, sliderFBO);
	
	glBlitFramebuffer(	0, 0, pxDimensions[0], pxDimensions[1],
						0, 0, pxDimensions[0], pxDimensions[1],
						GL_COLOR_BUFFER_BIT, GL_NEAREST
						);
	
	//glClearColor(1,0,0,0.2);
	//glClear(GL_COLOR_BUFFER_BIT);
	
	
	
	
	
	
	
	
	
	
	
	glBindFramebuffer(GL_FRAMEBUFFER, sliderFBO);

	/*
	GLvoid* pixels = malloc(pxDimensions[0]* pxDimensions[1]*4);
	
	png::image<png::rgba_pixel> img(pxDimensions[0], pxDimensions[1]);

	a = glGetError();
	if (a!=0)cout << "Eaa" << a << endl;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glReadBuffer(GL_COLOR_ATTACHMENT0);
    
    a = glGetError();
	if (a!=0)cout << "Eab" << a << endl;
    
    glReadPixels(0,0, pxDimensions[0], pxDimensions[1], GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    
    unsigned char* data = static_cast<unsigned char*>(pixels);
	
	auto pixel = img.get_pixel(0,0);
	
	int count=0;
	for (int j=0;j<pxDimensions[1];j++)
	{
		for(int i=0;i<pxDimensions[0];i++)
		{
			pixel.red = data[count];
			pixel.green = data[count+1];
			pixel.blue = data[count+2];
			pixel.alpha = data[count+3];
			img.set_pixel(i,j,pixel);
			
			count+=4;
		}
	}
	
	img.write("testrb.png");
	
	
	free(pixels);
	*/
						
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	
	
	
	
	
	
	
	
	const GraphicBitmap* gb = static_cast<const GraphicBitmap*>(globalInfo.graphicsDB.fetch(GraphicID::cursor));
	glBindVertexArray(gb->getVAO());
	
	
	
	globalInfo.bindGridProjection();
	
	glBindVertexArray(globalInfo.vaoStore.getvao1LL());
	glUniform2f(scaleLoc, dimensions[0], dimensions[1]);
	glUniform2f(offsetLoc, originOffset[0], originOffset[1]);
	//cout << originOffset[0] << " " << originOffset[1] << endl;
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sliderTexture);
	
	
	
	
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, gb->getTexture());
	
	if(globalInfo.getFrameStart() < time+timeDurationR)
	{
		glDrawArrays(GL_TRIANGLES,0,6);
	}
	else
	{
		glUniform1f(alphaLoc, 4*(fadeTime - globalInfo.getFrameStart()));
		glDrawArrays(GL_TRIANGLES,0,6);
		glUniform1f(alphaLoc, 1);
	}
	
	globalInfo.bindGridProjection();
	glUniform2f(scaleLoc, 1, 1);
	
	const GraphicBitmap* scorep = static_cast<const GraphicBitmap*>(globalInfo.graphicsDB.fetch(GraphicID::sliderscorepoint));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, scorep->getTexture());
	glBindVertexArray(scorep->getVAOFl());
	
	for(int i=0; i<ticks.size(); i++)
	{
		if(globalInfo.getFrameStart() > ticks[i][2])
		{
			float lim = globalInfo.currentBeatmap.getComputedCircleSize()*2;
			array<float, 2> mg = globalInfo.getMouseGrid();
			if((mg[0]-ticks[i][0])*(mg[0]-ticks[i][0]) + (mg[1]-ticks[i][1])*(mg[1]-ticks[i][1]) < lim*lim && mousePressed())
			{
				score++;
				incCombo();
			}
			else
			{
				//Break combo
				largeFollow = false;
				resetCombo();
			}
			cout << "Tick at " << time << " " << ticks[0][2] << " " << windowClose << endl;
			ticks.erase(ticks.begin());
			i=-1;
			continue;
		}
		
		glUniform2f(offsetLoc, ticks[i][0], ticks[i][1]);
		glDrawArrays(GL_TRIANGLES,0,6);
	}
	
	//TODO for short sliders the windowClose may come after the slider has ended
	if(globalInfo.getFrameStart() > windowClose && score==0 && missedStart == false)
	{
		cout << "You have missed the start" << endl;
		missedStart=true;
		resetCombo();
	}
	
	
	glBindVertexArray(globalInfo.vaoStore.getvao2Centre());
	
	
	
	
	if(timeDiff<0)
	{
		const GraphicBitmap* approach = static_cast<const GraphicBitmap*>(globalInfo.graphicsDB.fetch(GraphicID::approachcircle));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, approach->getTexture());
		
		glUniform2f(offsetLoc, first[0], first[1]);
		//cout << radius << "  RR " << endl;
		glUniform2f(scaleLoc, radius, radius);
		
		glDrawArrays(GL_TRIANGLES, 0,6);
		
		//glUniform2f(scaleLoc, 1, 1);
	}
	
	glUniform2f(scaleLoc, globalInfo.currentBeatmap.getComputedCircleSize(), globalInfo.currentBeatmap.getComputedCircleSize());

	
	float progress=(globalInfo.getFrameStart()-time);
	float fa = progress/timeDuration;
	
	
	
	if(fa>0 && fa<repeat)
	{
		
		if(int(fa)%2==0)
		{
			float n;
			
			//Ball is going towards the end
			if(fa+1<repeat) //We have 1 more repeat
			{
				
				const GraphicBitmap* repeatarr = static_cast<const GraphicBitmap*>(globalInfo.graphicsDB.fetch(GraphicID::reversearrow));
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, repeatarr->getTexture());
				glUniform2f(offsetLoc, limit_last[0], limit_last[1]);
				glUniform1f(rotationLoc, beziers.back().out_Gradient.back() + 3.142);
				glDrawArrays(GL_TRIANGLES, 0, 6);
				glUniform1f(rotationLoc, 0);
			}
			
			fa = modf(fa, &n);
			
		}
		else
		{
			float n;
			
			if(fa+1<repeat) //We have 1 more repeat
			{
				const GraphicBitmap* repeat = static_cast<const GraphicBitmap*>(globalInfo.graphicsDB.fetch(GraphicID::reversearrow));
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, repeat->getTexture());
				glUniform2f(offsetLoc, first[0], first[1]);
				glUniform1f(rotationLoc, beziers.front().out_Gradient.front());
				glDrawArrays(GL_TRIANGLES, 0, 6);
				glUniform1f(rotationLoc, 0);
			}
			
			
			fa = 1-modf(fa, &n);
			
			
			
			
		}
		
		i = fa*out_Homogenous.size();
		if(i>=out_Homogenous.size())
		{
			i=out_Homogenous.size()-1;
		}
		
		glUniform2f(offsetLoc, out_Homogenous[i][0],  out_Homogenous[i][1]);

		
		if(largeFollow)
		{
			const GraphicBitmap* follow = static_cast<const GraphicBitmap*>(globalInfo.graphicsDB.fetch(GraphicID::sliderfollowcircle));
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, follow->getTexture());
			
			//cout << radius << "  RR " << endl;
			float lim = globalInfo.currentBeatmap.getComputedCircleSize();
			glUniform2f(scaleLoc, lim*2, lim*2);
			
			glDrawArrays(GL_TRIANGLES, 0,6);
		}
		
		//cout << f << endl;
		
		glUniform2f(scaleLoc, 1,1);
		
		glBindVertexArray(gb->getVAO());
	
	
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gb->getTexture());
		
		
		
		glDrawArrays(GL_TRIANGLES, 0,6);
	}
	
	glBindVertexArray(0);
	
	a = glGetError();
	if (a!=0)cout << "Berr3 " << a << endl;
		
	//exit(0);
}

//Calculates a spline for a BezierSection class.
//The length is tracked as the spline as built. If length exceeds max_length then no new points will be added
//
void BezierSection::calcBezierSpline(double length_accum, double max_length)
{
	int resolution = 1000;					//Hardcoded resolution. May be overkill. TODO remove points if res turns out to be too high
	double t_inc = 1/double(resolution);
	int n = points.size()-1;
	
	double pylast;
	double pxlast;
	double length0=0;
	double length1=0;
	
	std::cout << std::setprecision(16);
	
	int t_count=0;	//Current number of points added
	
	
	for(;t_count<=resolution;t_count++)
	{
		double t = t_count / double(resolution);
		double px=0;
		double py=0;
		double dxdt=0;
		double dydt=0;
		
		for(int i=0;i<=n;i++)//Calculate the point at position t
		{
			double bint = nCr(n,i) * pow(t, i) * pow(1-t, n-i);
			px+=points[i][0]*bint;
			py+=points[i][1]*bint;
			
		}

		if(t>0)
		{
			//Start calculating the delta distance
			//cout << "T: " << t << " " << sqrt((px-pxlast)*(px-pxlast) + (py-pylast)*(py-pylast)) << endl;
			length0+=sqrt((px-pxlast)*(px-pxlast) + (py-pylast)*(py-pylast));
		}
		
		pxlast = px;
		pylast = py;
		
		
		//dxdt *= n;
		//dydt *= n;
		
		//cout << nCr(n,0) << " " << nCr(n,1) << " " << nCr(n,2) << " " << nCr(n,3) << " " << nCr(n,4) << " " << nCr(n,5) << " " << nCr(n,6) << " " << nCr(n,7) << " " << nCr(n,8) << endl;
		
		
		
		if(length0+length_accum < max_length)
		{
			for(int i=0;i<=n-1;i++) //Calc the gradient
			{
				double bint = nCr(n-1,i) * pow(t, i) * pow(1-t, n-i-1);
				dxdt+=(points[i+1][0] - points[i][0])*bint;
				dydt+=(points[i+1][1] - points[i][1])*bint;
			}
			
			double dydx = dxdt/dydt;
			
			
			out_Points.push_back({{(float)px, (float)py}});
			out_Gradient.push_back(atan2(dydt, dxdt));		//Store the angle in the vector
		}
		
		//TODO else break; ?
		
	}
	length = length0;
}


//Calc homogenous points for the bezier
//
//offset is the distance between points
//line_gap is the distance from the start of the bezier
//TODO what happens if the bezier is shorter than offset and it shouldn't harbour a point
double BezierSection::calcHomogenous(double offset, double line_gap, double& homMax)
{
	int n = points.size()-1;
	
	double pylast;
	double pxlast;
	double length0=0;
	double length1=0;
	
	std::cout << std::setprecision(16);
	
	int t_count=0;
	
	double length_accum=0;
	double prev_t=0;
	for(;prev_t<1;)
	{
		double try_t;
		if(t_count!=0)try_t = prev_t;
		else try_t = 0;
		
		double line_inc;
		if(t_count!=0)line_inc = line_gap;
		else line_inc = offset;
		
		double px=0;
		double py=0;
		double dxdt=0;
		double dydt=0;
		
		bool iter_direction = true;
		double t_test = try_t;
		double last_less = prev_t;
		double last_greater = 1;
		double iter_inc = 0.01;
		
		//cout << "LI " << t_test << " " << line_inc << endl;
		
		if(t_count!=0)
		{
		
			for(int j=0;j<15;j++)
			{
				px=0;
				py=0;
				//cout << "II " << t_test << endl;
				for(int i=0;i<=n;i++)
				{
					double bint = nCr(n,i) * pow(t_test, i) * pow(1-t_test, n-i);
					px+=points[i][0]*bint;
					py+=points[i][1]*bint;
					
				}
				double test_inc = sqrt((px-pxlast)*(px-pxlast) + (py-pylast)*(py-pylast));
				//cout << "TI " << t_test << " " << test_inc << endl;
				if(test_inc < line_inc)
				{
					last_less = t_test;
					if(iter_direction==true)
					{
						t_test+=iter_inc;
					}
					else
					{
						iter_direction=true;
						iter_inc = (last_greater - t_test)/2;
						t_test+=iter_inc;
					}
				}
				else
				{
					
					last_greater = t_test;
					if(iter_direction==true)
					{
						iter_direction=false;
						iter_inc = (last_less-t_test)/2;
						t_test+=iter_inc;
					}
					else
					{
						
						t_test+=iter_inc;
					}
				}
				
			}
		}
		else
		{
			t_test = 0;
			px = points[0][0];
			py = points[0][1];
		}
		length_accum += line_inc;//sqrt((px-pxlast)*(px-pxlast) + (py-pylast)*(py-pylast));
		pxlast = px;
		pylast = py;
		
		if(homMax - length_accum < 0)
		{
			break;
		}
		//dxdt *= n;
		//dydt *= n;
		
		//cout << nCr(n,0) << " " << nCr(n,1) << " " << nCr(n,2) << " " << nCr(n,3) << " " << nCr(n,4) << " " << nCr(n,5) << " " << nCr(n,6) << " " << nCr(n,7) << " " << nCr(n,8) << endl;
		
		double dydx = dxdt/dydt;
		
		prev_t = t_test;
		
		//cout << "P: " << px << " " << py << " " << t_test << endl;
		
		t_count ++;
		
		out_Homogenous.push_back({{(float)px, (float)py}});
		
	}
	length_accum = (t_count-1) * line_gap + offset;
	cout << " LA "  << length_accum << endl;
	homMax-=length_accum;
	return length - length_accum;
}

void BezierTest::renderPast()
{
	renderFuture();
}

HitQueuePos BezierTest::checkQueue()
{
	if(globalInfo.getSongTime() < time-globalInfo.currentBeatmap.getAR_float())
	{
		return HitQueuePos::Waiting;
	}
	else if(globalInfo.getSongTime()>fadeTime)return HitQueuePos::Vanished;
	else if (globalInfo.getSongTime()>time+timeDurationR)
	{
		if(done==false)
		{
			float lim = globalInfo.currentBeatmap.getComputedCircleSize()*2;
			array<float, 2> mg = globalInfo.getMouseGrid();
			bool hitLast = false;
			if((mg[0]-limit_last[0])*(mg[0]-limit_last[0]) + (mg[1]-limit_last[1])*(mg[1]-limit_last[1]) < lim*lim && mousePressed())
			{
				score++;
				hitLast = true;
				cout << "HL" << endl;
			}
			else cout << "ML" << endl;
			
			cout << "Bezier score: " << score << " / " << maxScore << endl;
			
			done=true;
			
			HitScore sliderScore= HitScore::hit0;
			
			if(score == maxScore){feedGraphic = GraphicID::hit300; sliderScore=HitScore::hit300;}
			else if(score*2 >= maxScore){feedGraphic = GraphicID::hit100; sliderScore=HitScore::hit100;}
			else if(score > 0){feedGraphic = GraphicID::hit50; sliderScore=HitScore::hit50;}
			else feedGraphic = GraphicID::hit0;	
			
			addHitScore(sliderScore);
				
			if(hitLast)incCombo();
		}
		return HitQueuePos::FadeOut;
	}
	
	return HitQueuePos::Visible;
}

HitFeedback BezierTest::getFeedback()
{
	GLuint offsetLoc = static_cast<const Shader_blitTexture*>(globalInfo.shaderManager->getShader(SH_BlitTexture))->getUniforms().offset;
	GLuint alphaLoc= static_cast<const Shader_blitTexture*>(globalInfo.shaderManager->getShader(SH_BlitTexture))->getUniforms().aF;
	
	
	
	
	return HitFeedback(globalInfo, limit_last[0], limit_last[1], time+timeDurationR, static_cast<const GraphicBitmap*>(globalInfo.graphicsDB.fetch(feedGraphic))->getTexture(), offsetLoc, alphaLoc, 0);
	
}

bool BezierTest::click(Button, float mx, float my)
{
	
	
	float d = (mx-first[0])*(mx-first[0]) + (my-first[1])*(my-first[1]);
	float lim = globalInfo.currentBeatmap.getComputedCircleSize();
	
	if(d<lim*lim)
	{
	
		//if(done==true)cout << "UIT RBTEETETYTYTV" << endl;
		float hitTime = globalInfo.getSongTime();
		
		//hitAcc = (hitTime - time)/globalInfo.currentBeatmap.getHitLimit();	//-ve if early
		
		
		HitScore hitScore = globalInfo.currentBeatmap.getScore(fabs(hitTime - time));
		
		if (hitScore!=HitScore::hitEarly && hitScore!=HitScore::hit0)
		{
		

			cout << "Hit Bezier Note! " << hitTime - time << " " << (int)hitScore << endl;
			largeFollow = true;
			score++;
			incCombo();

		}
		//TODO else do the wobble thing
		
		
		//return true;
	}
	
	
	if(largeFollow==false)
	{
		float progress=(globalInfo.getFrameStart()-time);
		float fa = progress/timeDuration;
		
		if(fa>0)
		{
			int i = fa*out_Homogenous.size();
			if(i>=out_Homogenous.size())
			{
				i=out_Homogenous.size()-1;
			}
			
			float x = out_Homogenous[i][0];  
			float y = out_Homogenous[i][1];
		
			
			
			float d = (mx-x)*(mx-x) + (my-y)*(my-y);
			float lim = globalInfo.currentBeatmap.getComputedCircleSize();
			
			if(d<lim*lim)
			{
					largeFollow = true;
			}
		
		}
		
	}
}

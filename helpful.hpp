#ifndef HELPFUL_H
#define HELPFUL_H

#include <sstream>
#include <string>
#include <vector>
using namespace std;

inline std::string intToStr(int a)
{
	std::ostringstream convert;
	convert << a;
	return convert.str();
}

inline std::string smallIntToStr(int a)
{
	switch(a)
	{
		case 0:
			return "0"; break;
		case 1:
			return "1"; break;
		case 2:
			return "2"; break;
		case 3:
			return "3"; break;
		case 4:
			return "4"; break;
		case 5:
			return "5"; break;
		case 6:
			return "6"; break;
		case 7:
			return "7"; break;
		case 8:
			return "9"; break;
		case 9:
			return "9"; break;
		default:
			return intToStr(a); break;
	}
}

char* textFileRead(const char *fileName);

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);

#endif

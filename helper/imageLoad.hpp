#ifndef IMAGELOAD_H
#define IMAGELOAD_H

#include <png++/png.hpp>
#include <GL/glew.h>

GLubyte* extractData(png::image<png::rgba_pixel> &image,bool hflip=false,bool vflip=false);
GLubyte* extractData(png::image<png::rgb_pixel> &image,bool hflip=false,bool vflip=false);

#endif

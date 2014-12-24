#include <helper/imageLoad.hpp>

GLubyte* extractData(png::image<png::rgba_pixel> &image, bool hflip,bool vflip)
{
	int width = image.get_width();
	int height = image.get_height();
	GLubyte* image_data = new GLubyte[width * height * 4];

	int p = 0;
	
	if (hflip==false && vflip==false)
	{
		for ( int j = 0; j < height; ++j ){
			for ( int i = 0; i < width; ++i ){
				int x = i;
				int y = height - j - 1;
				//cout << x << " " << y << endl;
				const decltype( image.get_pixel(x,y) ) & pixel = image.get_pixel(x,y);
				image_data[p  ] = pixel.red;
				image_data[p+1] = pixel.green;
				image_data[p+2] = pixel.blue;
				image_data[p+3] = pixel.alpha;
				//cout << (int)image_data[p] << " " << (int)image_data[p+1] << " " << (int)image_data[p+2] << endl;
				p += 4;
			}
		}
	}
	else
	{
		for ( int j = 0; j < height; ++j ){
			for ( int i = 0; i < width; ++i ){
				int x,y;
				if(hflip)x = width - i - 1;
				else x = i;
				
				if(hflip)y = j;
				else y = height - j - 1;;
				//cout << x << " " << y << endl;
				const decltype( image.get_pixel(x,y) ) & pixel = image.get_pixel(x,y);
				image_data[p  ] = pixel.red;
				image_data[p+1] = pixel.green;
				image_data[p+2] = pixel.blue;
				image_data[p+3] = pixel.alpha;
				//cout << (int)image_data[p] << " " << (int)image_data[p+1] << " " << (int)image_data[p+2] << endl;
				p += 4;
			}
		}
	}
		
	return image_data;
	
}

GLubyte* extractData(png::image<png::rgb_pixel> &image, bool hflip,bool vflip)
{
	int width = image.get_width();
	int height = image.get_height();
	GLubyte* image_data = new GLubyte[width * height * 3];

	int p = 0;
	if (hflip==false && vflip==false)
	{
		for ( int j = 0; j < height; ++j ){
			for ( int i = 0; i < width; ++i ){
				int x = i;
				int y = height - j - 1;
				//cout << x << " " << y << endl;
				const decltype( image.get_pixel(x,y) ) & pixel = image.get_pixel(x,y);
				image_data[p  ] = pixel.red;
				image_data[p+1] = pixel.green;
				image_data[p+2] = pixel.blue;
				//cout << (int)image_data[p] << " " << (int)image_data[p+1] << " " << (int)image_data[p+2] << endl;
				p += 3;
			}
		}
	}
	else
	{
		for ( int j = 0; j < height; ++j ){
			for ( int i = 0; i < width; ++i ){
				int x,y;
				if(hflip)x = width - i - 1;
				else x = i;
				
				if(hflip)y = j;
				else y = height - j - 1;;
				//cout << x << " " << y << endl;
				const decltype( image.get_pixel(x,y) ) & pixel = image.get_pixel(x,y);
				image_data[p  ] = pixel.red;
				image_data[p+1] = pixel.green;
				image_data[p+2] = pixel.blue;
				//cout << (int)image_data[p] << " " << (int)image_data[p+1] << " " << (int)image_data[p+2] << endl;
				p += 3;
			}
		}
	}
		
	return image_data;
	
}

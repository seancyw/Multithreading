
#ifndef PPM_H
#define PPM_H

//references from https://github.com/sol-prog/threads/blob/master/image_processing/ppm.cpp
#include <string>
#include <vector>

class PPM
{
public:
	PPM();

	//create a PPM object and fill it with data stored in source
	PPM(const std::string & fileName);

	//create an "empty" PPM image with a given width and height
	PPM(const unsigned int width, const unsigned int height);

	//free memory when object is destroyed
	~PPM();

	//read PPM image from filename
	void read(const std::string & fileName);

	//write PPM image to filename
	void write(const std::string & fileName);
	
private:
	bool isAllocated;
	void initialize();

	//information about PPM file(height and width)
	unsigned int _lineNumber;
	unsigned int _columnsNumber;

	//arrays for storing R,G,B values
	std::vector< unsigned char > _r;
	std::vector< unsigned char > _g;
	std::vector< unsigned char > _b;

	unsigned int _height;
	unsigned int _width;
	unsigned int _maximumColumn;

	//total number of elements(pixels)
	unsigned int _size;

};

#endif
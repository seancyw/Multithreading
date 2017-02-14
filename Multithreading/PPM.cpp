#include "PPM.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>

PPM::PPM()
{
	initialize();
}

//create a PPM object and fill it with data stored in source
PPM::PPM(const std::string & fileName)
{
	initialize();
	read(fileName);
}

//create an "empty" PPM image with a given width and height
PPM::PPM(const unsigned int width, const unsigned int height)
{
	initialize();

	_width         = width;
	_height        = height;
	_lineNumber    = height;
	_columnsNumber = width;
	_size		   = width * height;

	_r.resize(_size, 0);
	_g.resize(_size, 0);
	_b.resize(_size, 0);
}

//free memory when object is destroyed
PPM::~PPM()
{

}

//read PPM image from filename
void PPM::read(const std::string & fileName)
{
	//initialize stream
	std::ifstream input(fileName.c_str(), std::ios::in | std::ios::binary);

	//if file open successfully
	if (input.is_open()) {
		std::string line;
		std::getline(input, line);

		if (line != "P6") {
			std::cout << "Error. Unrecognized file format" << std::endl;
		}

		std::getline(input, line);
		while (line[0] == '#') {
			std::getline(input, line);
		}

		std::stringstream dimension(line);

		try {
			dimension >> _width;
			dimension >> _height;
			_lineNumber    = _height;
			_columnsNumber = _width;
		}
		catch (std::exception & exception) {
			std::cout << "Header file format error." << exception.what() << std::endl;
			return;
		}

		std::getline(input, line);
		std::stringstream maximumValue(line);

		try {
			maximumValue >> _maximumColumn;
		}
		catch (std::exception & exception) {
			std::cout << "Header file format error." << exception.what() << std::endl;
			return;
		}

		_size = _width * _height;

		_r.reserve(_size);
		_g.reserve(_size);
		_b.reserve(_size);

		//Set data for R,G,B
		char data;
		for (unsigned int i = 0; i < _size; ++i) {
			input.read(&data, 1);
			_r[i] = (unsigned char)data;
			_g[i] = (unsigned char)data;
			_b[i] = (unsigned char)data;
		}
	}
	else {
		std::cout << "Error. Unable to open " << fileName << std::endl;
	}

	//Close the stream
	input.close();
}

//write PPM image to filename
void PPM::write(const std::string & fileName)
{
	//Initialize the stream
	std::ofstream output(fileName.c_str(), std::ios::out | std::ios::binary);

	if (output.is_open()) {

		output << "P6\n";
		output << _width;
		output << " ";
		output << _height << "\n";
		output << _maximumColumn << "\n";

		char data;
		for (unsigned int i = 0; i < _size; ++i) {
			data = (char)_r[i];
			output.write(&data, 1);
			data = (char)_g[i];
			output.write(&data, 1);
			data = (char)_b[i];
			output.write(&data, 1);
		}
	}
	else {
		std::cout << "Error. Unable to open " << fileName << std::endl;
	}

	//Close the stream
	output.close();
}

void PPM::initialize()
{
	_width         = 0;
	_height        = 0;
	_maximumColumn = 255;
}

unsigned int PPM::getHeight() const
{
	return _height;
}

unsigned int PPM::getWidth() const
{
	return _width;
}

unsigned int PPM::getMaximumColumns() const
{
	return _maximumColumn;
}

unsigned int PPM::getSize() const
{
	return _size;
}

std::vector< unsigned char > PPM::getRValue() const
{
	return _r;
}

std::vector< unsigned char > PPM::getGValue() const
{
	return _g;
}

std::vector< unsigned char > PPM::getBValue() const
{
	return _b;
}
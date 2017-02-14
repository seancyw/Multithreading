#include <iostream>
#include <thread>
#include <ctime>
#include <vector>
#include <string>
#include "PPM.h"

//Program reference from https://github.com/sol-prog/threads/blob/master/image_processing/ppm_05.cpp

//Split "mem" into "parts", e.g. if memory = 10 and parts = 4 you will have: 0,2,4,6,10
//if possible the function will split memory into equal chuncks, if not 
//the last chunck will be slightly larger
std::vector<int> bounds(int parts, int memory)
{
	std::vector<int> bnd;
	int delta = memory / parts;
	int remainder = memory % parts;

	int n1 = 0;
	int n2 = 0;

	bnd.push_back(n1);

	for (int i = 0; i < parts; ++i) {
		n2 = n1 + delta;

		if (i == parts - 1)
			n2 += remainder;

		bnd.push_back(n2);
		n1 = n2;
	}

	return bnd;
}

//Test if a given position (x, y) is "inside" the limits 0..totalLine and 0..totalColumns
bool border(int x, int y, int totalLines, int totalColumns)
{
	return (x >= 0 && x <= totalColumns && y >= 0 && y <= totalLines);
}

//Blur the pixel at (x, y) using information from the neighbour pixels
void process(PPM & image, PPM & image2, int x, int y)
{
	int row, column, totalColumns, totalRows, id;
	unsigned int r, g, b;
	float r_sum, g_sum, b_sum;

	//Filter used for bluring an image
	float filter[] = {
		0.10179640718562874, 0.11377245508982035, 0.10179640718562874,
		0.11377245508982035, 0.1377245508982036, 0.11377245508982035,
		0.10179640718562874, 0.11377245508982035, 0.10179640718562874
	};

	totalRows    = image.getHeight();
	totalColumns = image.getWidth();

	//Apply filter
	r_sum = 0;
	g_sum = 0;
	b_sum = 0;

	//check North-West
	row    = y - 1;
	column = x - 1;
	if (border(row, column, totalRows, totalColumns)) {
		id = row * image.getWidth() + column;

		r = (unsigned int)image.getRValue()[id];
		g = (unsigned int)image.getGValue()[id];
		b = (unsigned int)image.getBValue()[id];

		r_sum += r * filter[0];
		g_sum += g * filter[0];
		b_sum += b * filter[0];
	}

	//check North
	row = y - 1;
	column = x;
	if (border(row, column, totalRows, totalColumns)) {
		id = row * image.getWidth() + column;

		r = (unsigned int)image.getRValue()[id];
		g = (unsigned int)image.getGValue()[id];
		b = (unsigned int)image.getBValue()[id];

		r_sum += r * filter[1];
		g_sum += g * filter[1];
		b_sum += b * filter[1];
	}

	//check North-East
	row = y - 1;
	column = x + 1;
	if (border(row, column, totalRows, totalColumns)) {
		id = row * image.getWidth() + column;

		r = (unsigned int)image.getRValue()[id];
		g = (unsigned int)image.getGValue()[id];
		b = (unsigned int)image.getBValue()[id];

		r_sum += r * filter[2];
		g_sum += g * filter[2];
		b_sum += b * filter[2];
	}

	//check West
	row = y;
	column = x - 1;
	if (border(row, column, totalRows, totalColumns)) {
		id = row * image.getWidth() + column;

		r = (unsigned int)image.getRValue()[id];
		g = (unsigned int)image.getGValue()[id];
		b = (unsigned int)image.getBValue()[id];

		r_sum += r * filter[3];
		g_sum += g * filter[3];
		b_sum += b * filter[3];
	}

	//center
	row = y;
	column = x;
	id = row * image.getWidth() + column;

	r = (unsigned int)image.getRValue()[id];
	g = (unsigned int)image.getGValue()[id];
	b = (unsigned int)image.getBValue()[id];

	r_sum += r * filter[4];
	g_sum += g * filter[4];
	b_sum += b * filter[4];


	//check East
	row = y;
	column = x + 1;
	if (border(row, column, totalRows, totalColumns)) {
		id = row * image.getWidth() + column;

		r = (unsigned int)image.getRValue()[id];
		g = (unsigned int)image.getGValue()[id];
		b = (unsigned int)image.getBValue()[id];

		r_sum += r * filter[5];
		g_sum += g * filter[5];
		b_sum += b * filter[5];
	}
	//check South-West
	row = y + 1;
	column = x - 1;
	if (border(row, column, totalRows, totalColumns)) {
		id = row * image.getWidth() + column;

		r = (unsigned int)image.getRValue()[id];
		g = (unsigned int)image.getGValue()[id];
		b = (unsigned int)image.getBValue()[id];

		r_sum += r * filter[6];
		g_sum += g * filter[6];
		b_sum += b * filter[6];
	}
	//check South
	row = y + 1;
	column = x;
	if (border(row, column, totalRows, totalColumns)) {
		id = row * image.getWidth() + column;

		r = (unsigned int)image.getRValue()[id];
		g = (unsigned int)image.getGValue()[id];
		b = (unsigned int)image.getBValue()[id];

		r_sum += r * filter[7];
		g_sum += g * filter[7];
		b_sum += b * filter[7];
	}
	//check South-East
	row = y + 1;
	column = x + 1;
	if (border(row, column, totalRows, totalColumns)) {
		id = row * image.getWidth() + column;

		r = (unsigned int)image.getRValue()[id];
		g = (unsigned int)image.getGValue()[id];
		b = (unsigned int)image.getBValue()[id];

		r_sum += r * filter[8];
		g_sum += g * filter[8];
		b_sum += b * filter[8];
	}

	//Save the modifed pixel value in image2 
	id = y * image.getWidth() + x;
	image2.getRValue()[id] = (unsigned char)r_sum;
	image2.getGValue()[id] = (unsigned char)g_sum;
	image2.getBValue()[id] = (unsigned char)b_sum;

}

//Blur a chunck of an image
void tst(PPM & image, PPM & image2, int left, int right)
{
	for (int i = left; i < right; ++i) {
		int x = i / image.getWidth();
		int y = i - x * image.getWidth();

		process(image, image2, x, y);
	}
}

int main()
{
	std::string fileName = std::string("someFile.ppm");

	PPM image(fileName);
	PPM image2(image.getWidth(), image.getHeight());

	//Number of thread to use(image will be divided between threads)
	int parts = 4;

	std::vector<int> bnd = bounds(parts, image.getSize());

	std::vector<std::thread> threads;

	time_t start, end;
	time(&start);

	//Launch parts-1 threads
	for (int i = 0; i < parts - 1; ++i) {
		threads.push_back(std::thread(tst, std::ref(image), std::ref(image2), bnd[i], bnd[i + 1]));
	}

	//Use the main thread to do part of the work
	for (int i = parts - 1; i < parts; ++i) {
		tst(image, image2, bnd[i], bnd[i + 1]);
	}

	//Join parts-1 threads
	for (auto & e : threads)
		e.join();

	time(&end);

	std::cout << difftime(end, start) << " seconds" << std::endl;

	//Save the result
	image2.write("test2.ppm");

	std::cin.ignore();

	return 0;
}
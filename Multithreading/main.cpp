#include <iostream>
#include <thread>
#include <ctime>
#include <vector>
#include <string>
#include <mutex>
#include <atomic>
#include <fstream>
#include <deque>
#include "Lesson3.h"
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

int PPMmain()
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

//Threads Synchronization
std::mutex barrier;

void dotProducts(const std::vector<int> & v1, const std::vector<int>& v2, std::atomic< int >& result, int L, int R)
{
	int partialSum = 0;
	for (int i = L; i < R; ++i)
		partialSum += v1[i] * v2[i];

	std::lock_guard<std::mutex> blockThreadsUntilFinishJob(barrier);
	result += partialSum;
}

int dotProductsmain()
{
	int totalElements = 100000;
	int threadNo = 2;
	
	//Allows safe concurrent reading/writing, and synchronizatio
	//is done under the hood
	std::atomic<int> result = 0;

	std::vector<std::thread> threads;

	//Fill two vectors with some constant values for a quick verification
	// v1={1,1,1,1,...,1}
	// v2={2,2,2,2,...,2}
	// The result of the dot_product should be 200000 for this particular case
	std::vector<int> v1(totalElements, 1), v2(totalElements, 2);

	//split totalElements into threadNo parts
	std::vector<int> limits = bounds(threadNo, totalElements);

	//Launch nr threads
	for (int i = 0; i < threadNo; ++i)
		threads.push_back(std::thread(dotProducts, std::ref(v1), std::ref(v2), std::ref(result), limits[i], limits[i + 1]));

	//Join the thread with main threads
	for (auto &id : threads)
		id.join();

	//Print the result
	std::cout << result << std::endl;

	std::cin.ignore();

	return 0;
}

int main()
{
	int x;
	
	std::promise<int> p;
	std::future<int> f = p.get_future();
	
	//use share_future if there are more than 1 future.get is running
	//for the functions and change the arguments from pass by references to
	//pass by value
	//Example: std::future<int> fu = std::async(std::launch::async, factorial, sharedFuture);
	//std::shared_future<int> sharedFuture = f.share();

	//async is a function and it return future
	//future is a channel where result can be get from child thread
	//std::future<int> fu = std::async(factorial, 4);
	
	//std::launch::deferred will not create a new thread until fu.get is called
	//where it will invokes factorial function and return value in one thread
	//std::launch::async will create a new thread
	//By default, the parameters is std::launch::deferred | std::launch::async which depends
	//on user implementation to decide whether to create a new thread
	//std::future<int> fu = std::async(std::launch::async, factorial);
	std::future<int> fu = std::async(std::launch::async, factorial, std::ref(f));
	
	//do something else
	std::this_thread::sleep_for(std::chrono::microseconds(1));
	p.set_value(4);

	//if the promise if broken, exception: std::future_errc::broken_promise will be invoke
	//p.set_exception(std::make_exception_ptr(std::runtime_error("To err is human")));
	
	//fu.get will wait until child thread and return value from child
	//thread
	//it will only able to get the value once
	x = fu.get();

	std::cout << "Get from child: " << x << std::endl;

	std::cin.ignore();
	return 0;
}
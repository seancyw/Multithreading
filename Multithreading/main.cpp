#include <iostream>
#include <thread>

static const int totalThreads = 10;

//function will be called from a thread
void callFromThread(int id)
{
	std::cout << "Hello!" << id << std::endl;
}

int main()
{
	//Initialize groups of threads
	std::thread threads[totalThreads];

	//Launch groups of threads
	for (int i = 0; i < totalThreads; ++i)
		threads[i] = std::thread(callFromThread, i);

	std::cout << "Lauched from the main\n";

	//Join the thread with the main thread
	for (int i = 0; i < totalThreads; ++i)
		threads[i].join();

	std::cin.ignore();

	return 0;
}
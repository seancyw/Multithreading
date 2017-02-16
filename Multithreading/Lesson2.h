
#include <iostream>
#include <mutex>
#include <deque>
#include <fstream>

std::deque<int> queue;
std::mutex mu;
std::condition_variable conditional;

class LogFile
{
public:

	LogFile()
	{
	}

	void shared_print(std::string id, int value)
	{
		//Lazy initialization - Initialization upon first use idiom
		std::call_once(_flag, [&]() { _file.open("somethings.txt"); }); //file will be opened only once

																		//initialize unique lock with apply fine-grain lock
																		//std::defer_lock is to unlock the locker while initialization
																		//unique_lock incurred more overhead compare to lock_guard,
																		//if flexibility is not a issue, recommended to use lock_guard
																		//std::lock_guard<std::mutex> locker(_mu);
		std::unique_lock<std::mutex> locker(_mu, std::defer_lock);

		//unique lock 
		locker.lock();
		_file << "From " << id << ": " << value << std::endl;
		locker.unlock();

		//transfer ownership
		std::unique_lock<std::mutex> locker2 = std::move(locker);
	}

private:
	std::mutex _mu;

	std::once_flag _flag;
	std::ofstream _file;
};

void function1()
{
	int count = 10;
	while (count > 0) {
		std::unique_lock<std::mutex> locker(mu);
		queue.push_front(count);
		locker.unlock();
		conditional.notify_one();  //notify one waiting thread, if there is one
		std::this_thread::sleep_for(std::chrono::seconds(1));
		count--;
	}
}

void function2()
{
	int data = 0;
	while (data != 1) {
		//Before sleep, lock need to be unlock
		std::unique_lock<std::mutex> locker(mu);

		//locker will unlock before going to sleep and lock it back
		//when thread 1 from (function1) notify it
		//spurious wake when the thread is wake up by itself,
		//so we use a predicate function to put it back to sleep
		conditional.wait(locker, [&]() { return !queue.empty(); });

		data = queue.back();
		queue.pop_back();
		locker.unlock();
		std::cout << "t2 got a value from t1: " << data << std::endl;
	}
}
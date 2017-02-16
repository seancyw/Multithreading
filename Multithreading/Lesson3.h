
#include <iostream>
#include <future>
#include <mutex>

//int factorial(int N)
//{
//	int result = 1;
//
//	for (int i = N; i > 1; i--)
//		result += i;
//
//	std::cout << "Result is " << result << std::endl;
//	return result;
//}

int factorial(std::future<int>& f)
{
	int result = 1;

	int N = f.get();
	for (int i = N; i > 1; i--)
		result *= i;

	std::cout << "Result is " << result << std::endl;
	return result;
}

#include <iostream>

class A
{
public:
	void f(int x, char c) {}

	long g(double x) { return 0; }

	int operator() (int N)
	{
		return 0;
	}
};

int factorial2(int N)
{
	int result = 1;
	
	for (int i = N; i > 1; i--)
		result *= i;

	std::cout << "Result is: " << result << std::endl;

	return result;
}
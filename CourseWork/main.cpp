#include <iostream>
#include <sstream>
#include <string>
#include <functional>
#include <future>
#include <chrono>

#include "ThreadPull.h"


static void func1(std::string str)
{
	std::stringstream sstr;
	sstr << str;
	sstr << "void func1()\n";
	std::cout << sstr.str();
	std::this_thread::sleep_for(std::chrono::milliseconds(555));
};

static void func2(std::string str)
{
	std::stringstream sstr;
	sstr << str;
	sstr << "void func2()\n";
	std::cout << sstr.str();
	std::this_thread::sleep_for(std::chrono::milliseconds(777));
};

static void func3(std::string str)
{
	std::stringstream sstr;
	sstr << str;
	sstr << "void func3()\n";
	std::cout << sstr.str();
	std::this_thread::sleep_for(std::chrono::milliseconds(1110));
};


int main()
{
	ak::ThreadPull threadPull(std::thread::hardware_concurrency() - 21);

	for (int idx{}; idx < 4; idx++)
	{
		threadPull.submit(func1);
		threadPull.submit(func2);
		threadPull.submit(func3);
		threadPull.submit(func1);
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	threadPull.stop();

	return EXIT_SUCCESS;
}
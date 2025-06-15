#include <vector>
#include <iostream>
#include <future>
#include <thread>
#include <algorithm>


template <typename Iterator, typename Func>
Func parallel_for_each(Iterator itStart, Iterator itEnd, Func func)
{
	unsigned long const len = std::distance(itStart, itEnd);
	unsigned long const max—hunkSize{ 3 };

	if (len <= max—hunkSize)
	{
		return std::for_each(itStart, itEnd, func);
	}

	auto itMid = itStart;
	std::advance(itMid, len / 2);

	std::future<Func> firstHalfResFut = std::async(std::launch::async,
		parallel_for_each<Iterator, Func>, itStart, itMid, func);
 
	Func secondHalfRes = parallel_for_each(itMid, itEnd, func);
	Func firstHalfRes = firstHalfResFut.get();

	return firstHalfRes;
}


int main()
{
	std::vector<int> vec{ 3, 5, 7, 9, 11, 21 };

	std::cout << "Vector [" << vec.size() << "] : ";
	for (const auto elem : vec)
	{
		std::cout << elem << " ";
	}
	std::cout << std::endl << std::endl;

	std::for_each(vec.begin(), vec.end(), [](int& n) { n++; });

	std::cout << "Vector [" << vec.size() << "] : ";
	for (const auto elem : vec)
	{
		std::cout << elem << " ";
	}
	std::cout << std::endl << std::endl;

	parallel_for_each(vec.begin(), vec.end(), [](int& n) { n++; });

	std::cout << "Vector [" << vec.size() << "] : ";
	for (const auto elem : vec)
	{
		std::cout << elem << " ";
	}
	std::cout << std::endl << std::endl;

	system("pause");
	return EXIT_SUCCESS;
}
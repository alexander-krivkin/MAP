#include <vector>
#include <chrono>
#include <iostream>
#include <future>
#include <thread>


void min(std::vector<int>& srcVector, std::promise<int> prom)
{
	int min{ srcVector[0] };
	std::vector<int>::iterator idxMin{ srcVector.begin() };

	for (size_t idx{ 1 }; idx < srcVector.size(); idx++)
	{
		if (srcVector[idx] < min)
		{
			min = srcVector[idx];
			idxMin = srcVector.begin() + idx;
		}
	}

	srcVector.erase(idxMin);
	prom.set_value(min);
}


std::vector<int> srcVector{};
std::vector<int> dstVector{};


int main()
{
	auto tpStart = std::chrono::system_clock::now();
	auto tpNow = std::chrono::system_clock::now();
	auto t_msec = static_cast<unsigned int>
		(11 + std::chrono::duration_cast<std::chrono::nanoseconds>(tpNow - tpStart).count());
	srand(t_msec);

	srcVector.resize(static_cast<size_t>(rand() % 21 + 11));
	std::cout << "Src vector [" << srcVector.size() << "] : ";

	tpNow = std::chrono::system_clock::now();
	t_msec = static_cast<unsigned int>
		(11 + std::chrono::duration_cast<std::chrono::nanoseconds>(tpNow - tpStart).count());
	srand(t_msec);

	for (auto& elem : srcVector)
	{
		elem = rand() % 98 + 1;
		std::cout << elem << " ";
	}
	std::cout << std::endl;

	while (true)
	{
		std::promise<int> prom;
		std::future<int> fut = prom.get_future();

		auto tsk = std::async(min, std::ref(srcVector), std::move(prom));
		//fut.wait();

		dstVector.push_back(fut.get());
		if (!srcVector.size())
		{
			break;
		}
	}

	std::cout << "Dst vector [" << dstVector.size() << "] : ";
	for (const auto elem : dstVector)
	{
		std::cout << elem << " ";
	}

	std::cout << std::endl << std::endl;
	system("pause");
	return EXIT_SUCCESS;
}
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <random>
#include <algorithm>


std::vector<int> v1_1(1'000), v2_1(1'000), v3_1(1'000);
std::vector<int> v1_2(10'000), v2_2(10'000), v3_2(10'000);
std::vector<int> v1_3(100'000), v2_3(100'000), v3_3(100'000);
std::vector<int> v1_4(1'000'000), v2_4(1'000'000), v3_4(1'000'000);


void generateVectors(std::vector<int>& v1, std::vector<int>& v2)
{
	std::mt19937 gen;
	std::uniform_int_distribution<int> dis(0, 1'000'000);
	auto randNum([=]() mutable {return dis(gen); });

	std::generate(v1.begin(), v1.end(), randNum);
	std::generate(v2.begin(), v2.end(), randNum);
}

void sumVectors(std::vector<int>::iterator v1_begin, std::vector<int>::iterator v1_end,
	std::vector<int>::iterator v2_begin, std::vector<int>::iterator v2_end,
	std::vector<int>::iterator res_begin)
{
	while ((v1_begin < v1_end) && (v2_begin < v2_end))
	{
		*res_begin = *v1_begin + *v2_begin;

		v1_begin++;
		v2_begin++;
		res_begin++;
	}
}

auto sumVectorsPar(int numThreads, std::vector<int>& v1, std::vector<int>& v2, std::vector<int>& res)
{
	auto start = std::chrono::steady_clock::now();

	std::vector<std::unique_ptr<std::thread>> upThreads(numThreads);

	int idx{};
	for (auto& upThread : upThreads)
	{
		int begin = static_cast<int>(v1.size()) * idx / numThreads;
		int end = static_cast<int>(v1.size()) * (idx + 1) / numThreads;

		auto v1_begin = (v1.begin() + begin);
		auto v1_end = (v1.begin() + end);
		auto v2_begin = (v2.begin() + begin);
		auto v2_end = (v2.begin() + end);
		auto res_begin = (res.begin() + begin);

		upThread = std::unique_ptr<std::thread>(
			new std::thread{ sumVectors, v1_begin, v1_end, v2_begin, v2_end, res_begin });
		idx++;
	}

	for (auto& upThread : upThreads)
	{
		upThread->join();
	}

	auto end = std::chrono::steady_clock::now();
	auto elapsed = end - start;

	return std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
}


int main()
{
	std::cout << "Hardware threads limit: " << std::thread::hardware_concurrency() << std::endl << std::endl;

	generateVectors(v1_1, v2_1);
	generateVectors(v1_2, v2_2);
	generateVectors(v1_3, v2_3);
	generateVectors(v1_4, v2_4);

	std::cout << std::setw(26) << "1'000" << std::setw(16) << "10'000" <<
		std::setw(15) << "100'000" << std::setw(15) << "1'000'000" << std::endl << std::endl;

	std::cout << "1 thread:  " << std::setw(12) << sumVectorsPar(1, v1_1, v2_1, v3_1) << " mcs" <<
		std::setw(12) << sumVectorsPar(1, v1_2, v2_2, v3_2) << " mcs" <<
		std::setw(12) << sumVectorsPar(1, v1_3, v2_3, v3_3) << " mcs" <<
		std::setw(12) << sumVectorsPar(1, v1_4, v2_4, v3_4) << " mcs" << std::endl;

	std::cout << "2 threads: " << std::setw(12) << sumVectorsPar(2, v1_1, v2_1, v3_1) << " mcs" <<
		std::setw(12) << sumVectorsPar(2, v1_2, v2_2, v3_2) << " mcs" <<
		std::setw(12) << sumVectorsPar(2, v1_3, v2_3, v3_3) << " mcs" <<
		std::setw(12) << sumVectorsPar(2, v1_4, v2_4, v3_4) << " mcs" << std::endl;

	std::cout << "4 threads: " << std::setw(12) << sumVectorsPar(4, v1_1, v2_1, v3_1) << " mcs" <<
		std::setw(12) << sumVectorsPar(4, v1_2, v2_2, v3_2) << " mcs" <<
		std::setw(12) << sumVectorsPar(4, v1_3, v2_3, v3_3) << " mcs" <<
		std::setw(12) << sumVectorsPar(4, v1_4, v2_4, v3_4) << " mcs" << std::endl;

	std::cout << "8 threads: " << std::setw(12) << sumVectorsPar(8, v1_1, v2_1, v3_1) << " mcs" <<
		std::setw(12) << sumVectorsPar(8, v1_2, v2_2, v3_2) << " mcs" <<
		std::setw(12) << sumVectorsPar(8, v1_3, v2_3, v3_3) << " mcs" <<
		std::setw(12) << sumVectorsPar(8, v1_4, v2_4, v3_4) << " mcs" << std::endl;

	std::cout << "16 threads: " << std::setw(11) << sumVectorsPar(16, v1_1, v2_1, v3_1) << " mcs" <<
		std::setw(12) << sumVectorsPar(16, v1_2, v2_2, v3_2) << " mcs" <<
		std::setw(12) << sumVectorsPar(16, v1_3, v2_3, v3_3) << " mcs" <<
		std::setw(12) << sumVectorsPar(16, v1_4, v2_4, v3_4) << " mcs" << std::endl;

	std::cout << "24 threads: " << std::setw(11) << sumVectorsPar(24, v1_1, v2_1, v3_1) << " mcs" <<
		std::setw(12) << sumVectorsPar(24, v1_2, v2_2, v3_2) << " mcs" <<
		std::setw(12) << sumVectorsPar(24, v1_3, v2_3, v3_3) << " mcs" <<
		std::setw(12) << sumVectorsPar(24, v1_4, v2_4, v3_4) << " mcs" << std::endl;

	std::cout << std::endl;

	return EXIT_SUCCESS;
}
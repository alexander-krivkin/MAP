#include <iostream>
#include <chrono>
#include <thread>
#include <random>


int v1_1[5'000], v2_1[5'000], v3_1[5'000];
int v1_2[50'000], v2_2[50'000], v3_2[50'000];
int v1_3[500'000], v2_3[500'000], v3_3[500'000];
int v1_4[5'000'000], v2_4[5'000'000], v3_4[5'000'000];


void generateArrays(int* v1, int* v2, int size)
{
	std::mt19937 gen;
	std::uniform_int_distribution<int> dis1(0, 5'000'000);
	std::uniform_int_distribution<int> dis2(0, 5'000'000);

	for (int idx{}; idx < size; idx++)
	{
		v1[idx] = dis1(gen);
		v1[idx] = dis2(gen);
	}
}

void multArrays(int* v1, int* v2, int* res, int begin, int end)
{
	for (int idx{ begin }; idx < end; idx++)
	{
		res[idx] = v1[idx] * v2[idx];
	}
}

auto multVectorsPar(int numThreads, int* v1, int* v2, int* res, int size)
{
	auto start = std::chrono::steady_clock::now();

	std::vector<std::unique_ptr<std::thread>> upThreads(numThreads);

	int idx{};
	for (auto& upThread : upThreads)
	{
		int begin = size * idx / numThreads;
		int end = size * (idx + 1) / numThreads;

		upThread = std::unique_ptr<std::thread>(
			new std::thread{ multArrays, v1, v2, res, begin, end });
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

	generateArrays(v1_1, v2_1, 5'000);
	generateArrays(v1_2, v2_2, 50'000);
	generateArrays(v1_3, v2_3, 500'000);
	generateArrays(v1_4, v2_4, 5'000'000);

	std::cout << std::setw(26) << "5'000" << std::setw(16) << "50'000" <<
		std::setw(15) << "500'000" << std::setw(15) << "5'000'000" << std::endl << std::endl;

	std::cout << "1 thread:  " << std::setw(12) << multVectorsPar(1, v1_1, v2_1, v3_1, 5'000) << " mcs" <<
		std::setw(12) << multVectorsPar(1, v1_2, v2_2, v3_2, 50'000) << " mcs" <<
		std::setw(12) << multVectorsPar(1, v1_3, v2_3, v3_3, 500'000) << " mcs" <<
		std::setw(12) << multVectorsPar(1, v1_4, v2_4, v3_4, 5'000'000) << " mcs" << std::endl;

	std::cout << "2 threads: " << std::setw(12) << multVectorsPar(2, v1_1, v2_1, v3_1, 5'000) << " mcs" <<
		std::setw(12) << multVectorsPar(2, v1_2, v2_2, v3_2, 50'000) << " mcs" <<
		std::setw(12) << multVectorsPar(2, v1_3, v2_3, v3_3, 500'000) << " mcs" <<
		std::setw(12) << multVectorsPar(2, v1_4, v2_4, v3_4, 5'000'000) << " mcs" << std::endl;

	std::cout << "4 threads: " << std::setw(12) << multVectorsPar(4, v1_1, v2_1, v3_1, 5'000) << " mcs" <<
		std::setw(12) << multVectorsPar(4, v1_2, v2_2, v3_2, 50'000) << " mcs" <<
		std::setw(12) << multVectorsPar(4, v1_3, v2_3, v3_3, 500'000) << " mcs" <<
		std::setw(12) << multVectorsPar(4, v1_4, v2_4, v3_4, 5'000'000) << " mcs" << std::endl;

	std::cout << "8 threads: " << std::setw(12) << multVectorsPar(8, v1_1, v2_1, v3_1, 5'000) << " mcs" <<
		std::setw(12) << multVectorsPar(8, v1_2, v2_2, v3_2, 50'000) << " mcs" <<
		std::setw(12) << multVectorsPar(8, v1_3, v2_3, v3_3, 500'000) << " mcs" <<
		std::setw(12) << multVectorsPar(8, v1_4, v2_4, v3_4, 5'000'000) << " mcs" << std::endl;

	std::cout << "16 threads: " << std::setw(11) << multVectorsPar(16, v1_1, v2_1, v3_1, 5'000) << " mcs" <<
		std::setw(12) << multVectorsPar(16, v1_2, v2_2, v3_2, 50'000) << " mcs" <<
		std::setw(12) << multVectorsPar(16, v1_3, v2_3, v3_3, 500'000) << " mcs" <<
		std::setw(12) << multVectorsPar(16, v1_4, v2_4, v3_4, 5'000'000) << " mcs" << std::endl;

	std::cout << "24 threads: " << std::setw(11) << multVectorsPar(24, v1_1, v2_1, v3_1, 5'000) << " mcs" <<
		std::setw(12) << multVectorsPar(24, v1_2, v2_2, v3_2, 50'000) << " mcs" <<
		std::setw(12) << multVectorsPar(24, v1_3, v2_3, v3_3, 500'000) << " mcs" <<
		std::setw(12) << multVectorsPar(24, v1_4, v2_4, v3_4, 5'000'000) << " mcs" << std::endl;

	std::cout << std::endl;

	return EXIT_SUCCESS;
}
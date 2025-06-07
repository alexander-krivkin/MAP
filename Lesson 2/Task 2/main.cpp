#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <atomic>
#include <mutex>
#include <windows.h>


std::mutex mtx{};
constexpr short numThreads{ 16 };
constexpr short numCalculations{ 21 };

std::vector<std::unique_ptr<std::thread>> upThreads(numThreads);


void runCalculation(short idx, int numCalculations, int calculationTime)
{
	auto tStart = std::chrono::steady_clock::now();
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	std::unique_lock ul(mtx);
	COORD position = { 0, idx };
	SetConsoleCursorPosition(hConsole, position);
	std::cout << std::setw(2) << idx << ": ID = " << std::this_thread::get_id();
	ul.unlock();

	for (int idxCalc{}; idxCalc < numCalculations; idxCalc++)
	{
		try
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(calculationTime)); // Calculation

			int exceptionRnd = idx * rand() % 50 - 35;
			if (!exceptionRnd)
			{
				throw(std::exception{ "zero division" });
			}

			ul.lock();

			position = { static_cast<short>(21 + idxCalc), idx };
			SetConsoleCursorPosition(hConsole, position);
			SetConsoleTextAttribute(hConsole, 0xFF);
			std::cout << '_';
			SetConsoleTextAttribute(hConsole, 0x0F);

			ul.unlock();
		}
		catch (const std::exception& ex)
		{
			ul.lock();
			position = { static_cast<short>(21 + numCalculations + 35), idx };
			SetConsoleCursorPosition(hConsole, position);
			std::cout << "Exception: " << ex.what();

			position = { static_cast<short>(21 + idxCalc), idx };
			SetConsoleCursorPosition(hConsole, position);
			SetConsoleTextAttribute(hConsole, 0xAA);
			std::cout << '_';
			SetConsoleTextAttribute(hConsole, 0x0F);

			ul.unlock();
		}
	}

	auto tEnd = std::chrono::steady_clock::now();
	auto tElapsed = tEnd - tStart;

	ul.lock();
	position = { static_cast<short>(21 + numCalculations + 5), idx };
	SetConsoleCursorPosition(hConsole, position);
	std::cout << "Elapsed time = " << std::chrono::duration_cast<std::chrono::milliseconds>(tElapsed).count() << " ms";
	ul.unlock();
}


int main()
{
	int idx{};
	for (auto& upThread : upThreads)
	{
		upThread = std::unique_ptr<std::thread>(new std::thread{runCalculation, idx, numCalculations, rand() % 500 + 100});
		idx++;
	}

	for (auto& upThread : upThreads)
	{
		upThread->join();
	}

	COORD position = { 0, numThreads };
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsole, position);

	std::cout << std::endl;

	return EXIT_SUCCESS;
}
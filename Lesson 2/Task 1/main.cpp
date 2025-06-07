#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>


std::atomic<int> numClients{ 0 };


void addClients(int maxClients)
{
	for (int idx{}; idx < maxClients; idx++)
	{
		numClients.fetch_add(1, std::memory_order_seq_cst);
		std::string str{};
		str = "Client " + std::to_string(idx + 1) + " added to queue" + '\n';
		std::cout << str;

		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

void releaseClients()
{
	int idx{};
	while (numClients.load(std::memory_order_seq_cst) > 0)
	{
		std::this_thread::sleep_for(std::chrono::seconds(2));

		numClients.fetch_sub(1, std::memory_order_seq_cst);
		std::string str{};
		str = "-- Client " + std::to_string(++idx) + " released" + '\n';
		std::cout << str;
	}
}


int main()
{
	std::cout << std::boolalpha << "Atomic<int> numClients.is_lock_free() = " << numClients.is_lock_free() << std::endl;

	std::thread thread1{ addClients , 11 };
	thread1.detach();

	std::this_thread::sleep_for(std::chrono::milliseconds(1));
	std::thread thread2{ releaseClients };
	thread2.join();

	std::cout << std::endl;

	return EXIT_SUCCESS;
}
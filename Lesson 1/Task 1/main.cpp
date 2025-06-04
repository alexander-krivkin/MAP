#include <iostream>
#include <chrono>
#include <thread>

int numClients{};

void addClients(int maxClients)
{
	for (int idx{}; idx < maxClients; idx++)
	{
		numClients++;
		std::cout << "Client " << idx + 1 << " added to queue" << std::endl;

		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

void releaseClients()
{
	int idx{};
	while (numClients > 0)
	{
		std::this_thread::sleep_for(std::chrono::seconds(2));

		numClients--;
		std::cout << "-- Client " << ++idx << " released" << std::endl;
	}
}


int main()
{
	std::thread thread1{ addClients , 21 };
	thread1.detach();

	std::thread thread2{ releaseClients };
	thread2.join();

	std::cout << std::endl;

	return EXIT_SUCCESS;
}
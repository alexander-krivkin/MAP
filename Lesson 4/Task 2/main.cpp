#include <iostream>
#include <string>
#include <thread>

#include "TcpServer.h"
#include "TcpClient.h"


const int numClients{ 7 };


int main()
{
	TcpServer tcpServer{};

	std::thread thr0(&TcpServer::run, tcpServer);

	TcpClient tcpClients[numClients]{};
	std::thread thr[numClients]{};

	for (int idx{0}; idx < numClients; idx++)
	{
		thr[idx] = std::thread(&TcpClient::connectAndSendMessage, tcpClients[idx], idx);
	}
	for (int idx{ 0 }; idx < numClients; idx++)
	{
		thr[idx].join();
	}

	thr0.join();

	return EXIT_SUCCESS;
}
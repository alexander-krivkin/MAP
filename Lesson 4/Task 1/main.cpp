#include <iostream>
#include <string>
#include <thread>

#include "TcpServer.h"
#include "TcpClient.h"


int main()
{
	TcpServer tcpServer{};
	TcpClient tcpClient{};

	auto thr1 = std::thread(&TcpServer::receiveMessageAndAnswer, tcpServer);
	auto thr2 = std::thread(&TcpClient::sendMessage, tcpClient);

	thr1.join();
	thr2.join();

	return EXIT_SUCCESS;
}
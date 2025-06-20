#include <iostream>
#include <string>
#include <thread>

#include "UdpServer.h"
#include "UdpClient.h"


int main()
{
	UdpServer udpServer{};
	UdpClient udpClient{};

	auto thr1 = std::thread(&UdpServer::receiveMessageAndAnswer, udpServer);
	auto thr2 = std::thread(&UdpClient::sendMessage, udpClient);

	thr1.join();
	thr2.join();

	return EXIT_SUCCESS;
}
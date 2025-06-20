#pragma once

#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#pragma comment (lib, "Ws2_32.lib")


class UdpServer
{
public:
	UdpServer()
	{
		if (WSAStartup(MAKEWORD(2, 2), &wsaData_))
		{
			std::cout << "server WSAStartup() error: " << WSAGetLastError() << std::endl;
			system("pause");
		}

		socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		serverAddr_.sin_family = AF_INET;
		serverAddr_.sin_addr.s_addr = INADDR_ANY;
		serverAddr_.sin_port = htons(port_);

		if (SOCKET_ERROR == bind(socket_, (sockaddr*)&serverAddr_, sizeof(serverAddr_)))
		{
			std::cout << "server bind() error: " << WSAGetLastError() << std::endl;
			system("pause");
			closesocket(socket_);
			WSACleanup();
		}
	}

	~UdpServer()
	{
		closesocket(socket_);
		WSACleanup();
	}

	void receiveMessageAndAnswer()
	{
		int clientAddrSize = sizeof(clientAddr_);
		while (true)
		{
			std::cout << "Waiting request...\n";
			int receivedDataSize = recvfrom(socket_, buffer_, 32, 0,
				(sockaddr*)&clientAddr_, &clientAddrSize);

			if (receivedDataSize > 0)
			{
				std::cout << buffer_ << std::endl;
				std::string answer{ "Hello from server" };

				if (SOCKET_ERROR == sendto(socket_, answer.c_str(), (int)answer.size(), 0,
					(sockaddr*)&clientAddr_, clientAddrSize))
				{
					std::cout << "server sendto() error: " << WSAGetLastError() << std::endl;
					system("pause");
				}
			}
			else if (receivedDataSize == SOCKET_ERROR)
			{
				std::cout << "server recvfrom() error: " << WSAGetLastError() << std::endl;
				system("pause");
			}
		}
	}

private:
	WSAData wsaData_{};
	SOCKET socket_{};
	sockaddr_in serverAddr_{}, clientAddr_{};
	int port_{ 22110 };
	char buffer_[32]{};
};
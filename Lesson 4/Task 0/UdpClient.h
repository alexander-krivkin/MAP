#pragma once

#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#pragma comment (lib, "Ws2_32.lib")


class UdpClient
{
public:
	UdpClient()
	{
		if (WSAStartup(MAKEWORD(2, 2), &wsaData_))
		{
			std::cout << "client WSAStartup() error: " << WSAGetLastError() << std::endl;
			system("pause");
		}

		socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		serverAddr_.sin_family = AF_INET;
		InetPton(AF_INET, L"127.0.0.1", &serverAddr_.sin_addr.s_addr);
		serverAddr_.sin_port = htons(port_);
	}

	~UdpClient()
	{
		closesocket(socket_);
		WSACleanup();
	}

	void sendMessage()
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));

		int serverAddrSize = sizeof(serverAddr_);
		std::string message{ "Hello from client" };

		if (SOCKET_ERROR == sendto(socket_, message.c_str(), (int)message.size(), 0,
			(sockaddr*)&serverAddr_, serverAddrSize))
		{
			std::cout << "client sendto() error: " << WSAGetLastError() << std::endl;
			system("pause");
		}

		int receivedDataSize = recvfrom(socket_, buffer_, 32, 0,
			(sockaddr*)&serverAddr_, &serverAddrSize);
		if (receivedDataSize > 0)
		{
			std::cout << buffer_ << std::endl;
		}
		else if (receivedDataSize == SOCKET_ERROR)
		{
			std::cout << "client recvfrom() error: " << WSAGetLastError() << std::endl;
			system("pause");
		}
	}

private:
	WSAData wsaData_{};
	SOCKET socket_{};
	sockaddr_in serverAddr_{}, clientAddr_{};
	int port_{ 22110 };
	char buffer_[32]{};
};
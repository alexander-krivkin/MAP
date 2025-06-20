#pragma once

#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#pragma comment (lib, "Ws2_32.lib")


class TcpClient
{
public:
	TcpClient()
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));

		if (WSAStartup(MAKEWORD(2, 2), &wsaData_))
		{
			std::cout << "client WSAStartup() error: " << WSAGetLastError() << std::endl;
			system("pause");
		}

		clientSocket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		serverAddr_.sin_family = AF_INET;
		InetPton(AF_INET, L"127.0.0.1", &serverAddr_.sin_addr.s_addr);
		serverAddr_.sin_port = htons(port_);

		if (connect(clientSocket_, (sockaddr*)&serverAddr_, sizeof(serverAddr_)) == SOCKET_ERROR)
		{
			std::cout << "client connect() error: " << WSAGetLastError() << std::endl;
			system("pause");
			closesocket(clientSocket_);
			WSACleanup();
		}
		std::cout << "client connect() ok\n";
	}

	~TcpClient()
	{
		closesocket(clientSocket_);
		WSACleanup();
	}

	void sendMessage()
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));

		std::string message{ "<<< Hello Dasha Sasha Esya" };

		if (send(clientSocket_, message.c_str(), (int)message.size(), 0) == SOCKET_ERROR)
		{
			std::cout << "client send() error: " << WSAGetLastError() << std::endl;
			system("pause");
		}

		int receivedDataSize = recv(clientSocket_, buffer_, 32, 0);
		if (receivedDataSize > 0)
		{
			std::cout << buffer_ << std::endl;
		}
		else if (receivedDataSize == SOCKET_ERROR)
		{
			std::cout << "client recv() error: " << WSAGetLastError() << std::endl;
			system("pause");
		}
	}

private:
	WSAData wsaData_{};
	SOCKET clientSocket_{};
	sockaddr_in serverAddr_{};
	int port_{ 22110 };
	char buffer_[32]{};
};
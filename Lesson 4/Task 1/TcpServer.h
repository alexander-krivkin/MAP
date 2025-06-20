#pragma once

#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#pragma comment (lib, "Ws2_32.lib")


class TcpServer
{
public:
	TcpServer()
	{
		if (WSAStartup(MAKEWORD(2, 2), &wsaData_))
		{
			std::cout << "server WSAStartup() error: " << WSAGetLastError() << std::endl;
			system("pause");
		}

		serverSocket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		serverAddr_.sin_family = AF_INET;
		//InetPton(AF_INET, L"127.0.0.1", &serverAddr_.sin_addr.s_addr);
		serverAddr_.sin_addr.s_addr = INADDR_ANY;
		serverAddr_.sin_port = htons(port_);

		if (bind(serverSocket_, (sockaddr*)&serverAddr_, sizeof(serverAddr_)) == SOCKET_ERROR)
		{
			std::cout << "server bind() error: " << WSAGetLastError() << std::endl;
			system("pause");
			closesocket(serverSocket_);
			WSACleanup();
		}
		std::cout << "server bind() ok\n";

		if (listen(serverSocket_, 7) == SOCKET_ERROR)
		{
			std::cout << "server listen() error: " << WSAGetLastError() << std::endl;
			system("pause");
			closesocket(serverSocket_);
			WSACleanup();
		}
		std::cout << "server listen() ok\nWaiting connection request...\n";
	}

	~TcpServer()
	{
		closesocket(clientSocket_);
		closesocket(serverSocket_);
		WSACleanup();
	}

	void receiveMessageAndAnswer()
	{
		int clientAddrLen = sizeof(clientAddr_);
		clientSocket_ = accept(serverSocket_, (sockaddr*)&clientAddr_, &clientAddrLen);
		std::cout << "server accept() ok\nConnection request accepted...\n";

		while (true)
		{
			int receivedDataSize = recv(clientSocket_, buffer_, 32, 0);

			if (receivedDataSize > 0)
			{
				std::cout << buffer_ << std::endl;
				std::string answer{ "<<< Hello Sasha!" };

				if (send(clientSocket_, answer.c_str(), (int)answer.size(), 0) == SOCKET_ERROR)
				{
					std::cout << "server send() error: " << WSAGetLastError() << std::endl;
					system("pause");
				}
			}
			else if (receivedDataSize == SOCKET_ERROR)
			{
				std::cout << "server recv() error: " << WSAGetLastError() << std::endl;
				system("pause");
			}
		}
	}

private:
	WSAData wsaData_{};
	SOCKET serverSocket_{}, clientSocket_{};
	sockaddr_in serverAddr_{}, clientAddr_{};
	int port_{ 22110 };
	char buffer_[32]{};
};
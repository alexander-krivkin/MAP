#pragma once

#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <set>
#include <map>
#include <thread>
#include <chrono>
#include <mutex>
#pragma comment (lib, "Ws2_32.lib")


std::mutex mtx;

class TcpServer
{
public:
	TcpServer()
	{
		if (WSAStartup(MAKEWORD(2, 2), &wsaData_))
		{
			std::cout << "server WSAStartup() error: " << WSAGetLastError() << std::endl;
		}

		serverSocket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		serverAddress_.sin_family = AF_INET;
		//InetPton(AF_INET, L"127.0.0.1", &serverAddress_.sin_addr.s_addr);
		serverAddress_.sin_addr.s_addr = INADDR_ANY;
		serverAddress_.sin_port = htons(port_);

		if (bind(serverSocket_, (sockaddr*)&serverAddress_, sizeof(sockaddr_in)) == SOCKET_ERROR)
		{
			std::cout << "server bind() error: " << WSAGetLastError() << std::endl;
			closesocket(serverSocket_);
			WSACleanup();
		}
		std::cout << "server bind() ok\n";

		if (listen(serverSocket_, maxClients_) == SOCKET_ERROR)
		{
			std::cout << "server listen() error: " << WSAGetLastError() << std::endl;
			closesocket(serverSocket_);
			WSACleanup();
		}
		std::cout << "server listen() ok\nWaiting connection requests...\n\n";
	}

	~TcpServer()
	{
		for (auto& clientSocket : clientSockets_)
		{
			closesocket(clientSocket);
		}
		closesocket(serverSocket_);
		WSACleanup();
	}

	void run()
	{
		while (true)
		{
			sockaddr_in clientAddress{};
			int clientAddressLength = sizeof(sockaddr_in);
			char buffer[32]{};

			SOCKET clientSocket = accept(serverSocket_, (sockaddr*)&clientAddress, &clientAddressLength);
			if (clientSocket != INVALID_SOCKET)
			{
				std::cout << "server accept() ok\n";
				clientSockets_.insert(clientSocket);
				clientAddresses_[clientSocket] = clientAddress;
				clientAddressLengths_[clientSocket] = clientAddressLength;
				buffers_[clientSocket][0] = '\0';

				auto thr = std::thread(&TcpServer::receiveMessageAndAnswer, this, clientSocket);
				thr.detach();
			}
			else
			{
				std::cout << "server accept() error: " << WSAGetLastError() << std::endl;
			}
		}
	}

	void receiveMessageAndAnswer(SOCKET clientSocket)
	{
		auto uLock = std::unique_lock<std::mutex>(mtx);
		//uLock.lock();
		int receivedDataSize = recv(clientSocket, buffers_[clientSocket], 32, 0);
		uLock.unlock();

		if (receivedDataSize > 0)
		{
			std::cout << buffers_[clientSocket];
			std::string answer{ ">>> Server answer\n" };

			if (send(clientSocket, answer.c_str(), (int)answer.size(), 0) == SOCKET_ERROR)
			{
				std::cout << "server send() error: " << WSAGetLastError() << std::endl;
			}
		}
		else if (receivedDataSize == SOCKET_ERROR)
		{
			std::cout << "server recv() error: " << WSAGetLastError() << std::endl;
		}
	}

private:
	WSAData wsaData_{};
	SOCKET serverSocket_{};
	sockaddr_in serverAddress_{};
	int port_{ 22110 };

	int maxClients_{ 11 };
	std::set<SOCKET> clientSockets_{};
	std::map<SOCKET, sockaddr_in> clientAddresses_{};
	std::map<SOCKET, int> clientAddressLengths_{};
	std::map<SOCKET, char[32]> buffers_{};

};
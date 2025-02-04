#include <iostream>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>

#pragma comment(lib, "WSock32.lib")

#include <array>

#include <string>

/// ===========================================================================================================================================================================================
/// クライアント側
/// ===========================================================================================================================================================================================

int main() {
	try {
		WSADATA wsaData;

		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
			
			throw std::runtime_error("WinSock initialize failed");
		}
		
		// ソケット作成
		SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);
		if (s == INVALID_SOCKET) {
			throw std::runtime_error("Create socket failed");
		}


		sockaddr_in send_addr;
		memset(&send_addr, 0, sizeof(send_addr));
		send_addr.sin_family = AF_INET;
		send_addr.sin_port = htons(8000);
		send_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

		sockaddr_in recv_addr;
		int fromlen = sizeof(recv_addr);

		while (1) {
			int32_t nRcv;
			std::string szBuf;
			szBuf.clear();
			szBuf.resize(1024);

			std::cout << "送信 -->";
			std::cin >> szBuf;

			sendto(s, szBuf.data(), static_cast<int32_t>(szBuf.size()), 0, reinterpret_cast<const sockaddr*>(&send_addr), sizeof(send_addr));
			if (szBuf == "end") {
				break;
			}

			szBuf.clear();
			szBuf.resize(1024);

			std::cout << "受信待機中" << std::endl;

			nRcv = recvfrom(s, szBuf.data(), static_cast<int32_t>(szBuf.size()) - 1, 0, reinterpret_cast<struct sockaddr*>(&recv_addr), &fromlen);
			
			szBuf[nRcv] = '\0';
			std::cout << "受信 -->" << szBuf << std::endl;
			if (szBuf == "end") {
				break; // なぜか通らない
			}
		}

		closesocket(s);

		WSACleanup();
	}
	catch (const std::exception& err) {
		std::cerr << err.what() << std::endl;
		return 1;
	}

	return 0;
}
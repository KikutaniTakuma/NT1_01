#include <iostream>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>

#pragma comment(lib, "WSock32.lib")

#include <array>

#include <string>


/// ===========================================================================================================================================================================================
/// �T�[�o�[��
/// ===========================================================================================================================================================================================

int main() {
	try {
		WSADATA wsaData;

		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
			
			throw std::runtime_error("WinSock initialize failed");
		}

		std::cout << "�|�[�g�ԍ��̓��� : ";

		uint32_t uport = 0;
		std::cin >> uport;
		
		// �\�P�b�g�쐬
		SOCKET conectSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (conectSocket == INVALID_SOCKET) {
			throw std::runtime_error("Create socket failed");
		}


		SOCKADDR_IN saddr;
		memset(&saddr, 0, sizeof(saddr));
		saddr.sin_family = AF_INET;
		saddr.sin_port = htons(uport);
		saddr.sin_addr.s_addr = INADDR_ANY;

		int32_t bindNum = bind(conectSocket, reinterpret_cast<sockaddr*>(&saddr), sizeof(saddr));
		if (bindNum == SOCKET_ERROR) {
			throw std::runtime_error("bind socket failed");
		}

		int32_t listenNum = listen(conectSocket, 0);
		if (listenNum == SOCKET_ERROR) {
			throw std::runtime_error("listen failed");
		}

		SOCKADDR_IN clientSaddr;
		int32_t size = static_cast<int32_t>(sizeof(clientSaddr));

		std::cout << "accept�ҋ@��" << std::endl;
		SOCKET s = accept(conectSocket, reinterpret_cast<sockaddr*>(&clientSaddr), &size);
		if (s == INVALID_SOCKET) {
			throw std::runtime_error("accept failed");
		}

		std::cout << "client�ڑ���" << std::endl;

		closesocket(conectSocket);

		while (1) {
			int32_t nRcv;
			std::string szBuf;
			szBuf.clear();
			szBuf.resize(1024);

			std::cout << "��M�ҋ@��" << std::endl;

			nRcv = recv(s, szBuf.data(), static_cast<int32_t>(szBuf.size()) - 1, 0);
			szBuf[nRcv] = '\0';
			std::cout << "��M -->" << szBuf << std::endl;
			if (szBuf == "end") {
				break; // �Ȃ����ʂ�Ȃ�
			}

			szBuf.clear();
			szBuf.resize(1024);

			std::cout << "���M -->";
			std::cin >> szBuf;

			send(s, szBuf.data(), static_cast<int32_t>(szBuf.size()), 0);
			if (szBuf == "end") {
				break;
			}
		}

		closesocket(s);

		WSACleanup();
	}
	catch (const std::exception& err) {
		std::cout << err.what() << std::endl;
		return 1;
	}

	return 0;
}
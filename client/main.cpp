#include <iostream>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>

#pragma comment(lib, "WSock32.lib")

#include <array>

#include <string>

/// ===========================================================================================================================================================================================
/// �N���C�A���g��
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
		SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
		if (s == INVALID_SOCKET) {
			throw std::runtime_error("Create socket failed");
		}

		std::string szServer;
		szServer.resize(256);
		szServer = "192.168.0.60";

		HOSTENT* lpHost = nullptr;
		lpHost = gethostbyname(szServer.data());


		SOCKADDR_IN saddr;
		memset(&saddr, 0, sizeof(saddr));
		saddr.sin_family = lpHost->h_addrtype;
		saddr.sin_port = htons(uport);
		saddr.sin_addr.s_addr = *((u_long*)lpHost->h_addr);

		if (connect(s, reinterpret_cast<SOCKADDR*>(&saddr), sizeof(saddr)) == SOCKET_ERROR) {
			closesocket(s);
			throw std::runtime_error("server conectted faile");
		}
		else {
			std::cout << "�ڑ�����" << std::endl;
		}

		while (1) {
			int32_t nRcv;
			std::string szBuf;
			szBuf.clear();
			szBuf.resize(1024);

			std::cout << "���M -->";
			std::cin >> szBuf;

			send(s, szBuf.data(), static_cast<int32_t>(szBuf.size()), 0);
			if (szBuf == "end") {
				break;
			}

			szBuf.clear();
			szBuf.resize(1024);

			std::cout << "��M�ҋ@��" << std::endl;

			nRcv = recv(s, szBuf.data(), static_cast<int32_t>(szBuf.size()) - 1, 0);
			szBuf[nRcv] = '\0';
			std::cout << "��M -->" << szBuf << std::endl;
			if (szBuf == "end") {
				break; // �Ȃ����ʂ�Ȃ�
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
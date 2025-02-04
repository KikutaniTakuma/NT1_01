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
		
		// �\�P�b�g�쐬
		SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);
		if (s == INVALID_SOCKET) {
			throw std::runtime_error("Create socket failed");
		}


		sockaddr_in saddr;
		saddr.sin_family = AF_INET;
		saddr.sin_port = htons(8000);
		saddr.sin_addr.s_addr = INADDR_ANY;

		int32_t bindNum = bind(s, reinterpret_cast<sockaddr*>(&saddr), sizeof(saddr));
		if (bindNum == SOCKET_ERROR) {
			throw std::runtime_error("bind socket failed");
		}

		sockaddr_in out_saddr;

		int fromlen = sizeof(out_saddr);

		while (1) {
			int32_t nRcv;
			std::string szBuf;
			szBuf.clear();
			szBuf.resize(1024);

			std::cout << "��M�ҋ@��" << std::endl;

			nRcv = recvfrom(s, szBuf.data(), static_cast<int32_t>(szBuf.size()) - 1, 0, reinterpret_cast<sockaddr*>(&out_saddr), &fromlen);
			
			szBuf[nRcv] = '\0';
			std::cout << "��M -->" << szBuf << std::endl;
			if (szBuf == "end") {
				break; // �Ȃ����ʂ�Ȃ�
			}

			szBuf.clear();
			szBuf.resize(1024);

			std::cout << "���M -->";
			std::cin >> szBuf;

			sendto(s, szBuf.data(), static_cast<int32_t>(szBuf.size()), 0, reinterpret_cast<sockaddr*>(&out_saddr), fromlen);
			if (szBuf == "end") {
				break;
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
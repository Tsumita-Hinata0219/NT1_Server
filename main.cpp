#define _WINDOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <stdio.h>

#pragma comment(lib, "WSock32.lib")



// �|�[�g�ԍ���ݒ肷��
static unsigned short Get_PortNumber()
{
	unsigned short portNumber = 0;

	while (1) { // �������[�v�Ő��������͂�҂�
		// �v�����v�g�̕\��
		printf("�|�[�g�ԍ��̐ݒ� (0 �` 65535): ");

		// ���͎擾�ƃG���[�`�F�b�N
		if (scanf_s("%hu", &portNumber) == 1) {
			return portNumber; // ����ȓ��͂Ȃ�Ԃ�
		}
		else {
			// ���̓G���[���̏���
			printf("�����ȓ��͂ł��B������x���͂��Ă��������B\n");

			// ���̓o�b�t�@�̃N���A
			while (getchar() != '\n');
		}
	}
}

// �\�P�b�g���쐬����֐�
static SOCKET CreateSocket() {
	// �\�P�b�g�̍쐬
	SOCKET mySocket = socket(AF_INET, SOCK_STREAM, 0);
	if (mySocket == INVALID_SOCKET) {
		printf("�\�P�b�g�쐬�Ɏ��s���܂����B�G���[�R�[�h: %d\n", WSAGetLastError());
		return INVALID_SOCKET;
	}
	return mySocket;
}

// �T�[�o�[�A�h���X��ݒ肷��֐�
static void SetServerAddress(sockaddr_in& server_addr, unsigned short portNumber) {
	memset(&server_addr, 0, sizeof(sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(portNumber);
	server_addr.sin_addr.s_addr = INADDR_ANY;
}

// �\�P�b�g���o�C���h����֐�
static bool BindSocket(SOCKET sockfd, const sockaddr_in& server_addr) {
	if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
		printf("�o�C���h�Ɏ��s���܂����B�G���[�R�[�h: %d\n", WSAGetLastError());
		return false;
	}
	return true;
}

// �\�P�b�g�����b�X����Ԃɂ���֐�
static bool ListenSocket(SOCKET sockfd) {
	if (listen(sockfd, SOMAXCONN) == SOCKET_ERROR) {
		printf("���b�X���Ɏ��s���܂����B�G���[�R�[�h: %d\n", WSAGetLastError());
		return false;
	}
	return true;
}

// �\�P�b�g�ɖ��O��t���Đڑ�
static bool BindAndConnectSocket(SOCKET sockfd, unsigned short portNumber) {
	sockaddr_in server_addr{};
	SetServerAddress(server_addr, portNumber);

	if (!BindSocket(sockfd, server_addr)) {
		return false;
	}

	if (!ListenSocket(sockfd)) {
		return false;
	}

	return true;
}

// �N���C�A���g����̐ڑ����󂯓����
static SOCKET AcceptClientConnection(SOCKET listenSock) {
	sockaddr_in client_addr{};
	int client_addr_size = sizeof(client_addr);

	// �N���C�A���g����̐ڑ����󂯓����
	SOCKET clientSock = accept(listenSock, (struct sockaddr*)&client_addr, &client_addr_size);
	if (clientSock == INVALID_SOCKET) {
		printf("�N���C�A���g�̐ڑ��󂯓���Ɏ��s���܂����B�G���[�R�[�h: %d\n", WSAGetLastError());
		return INVALID_SOCKET;
	}

	printf("�N���C�A���g���ڑ����܂���\n");
	return clientSock;
}




int main()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("WSAStartup�Ɏ��s���܂����B�G���[�R�[�h: %d\n", WSAGetLastError());
		return 1;
	}

	SOCKET sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == INVALID_SOCKET) {
		printf("�\�P�b�g�̍쐬�Ɏ��s���܂����B�G���[�R�[�h: %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	unsigned short portNumber = 8080;
	if (!BindAndConnectSocket(sockfd, portNumber)) {
		closesocket(sockfd);
		WSACleanup();
		return 1;
	}

	printf("�T�[�o�[���|�[�g %d �Ń��b�X�����J�n���܂����B\n", portNumber);


	while (1) {
		SOCKET clientSock = AcceptClientConnection(sockfd);
		if (clientSock == INVALID_SOCKET) {
			continue;
		}

		while (1) {
			int nRcv = 0;
			char szBuf[1024]{};

			nRcv = recv(clientSock, szBuf, sizeof(szBuf) - 1, 0);
			if (nRcv == SOCKET_ERROR) {
				printf("�f�[�^��M�Ɏ��s���܂����B�G���[�R�[�h: %d\n", WSAGetLastError());
				closesocket(clientSock);
				break;
			}
			else if (nRcv == 0) {
				printf("�N���C�A���g���ڑ�����܂����B\n");
				closesocket(clientSock);
				break;
			}

			szBuf[nRcv] = '\0'; // ��M�f�[�^�𕶎���Ƃ��Ĉ������߂ɏI�[������ǉ�
			printf("��M�f�[�^: %s\n", szBuf);

			// ��M�f�[�^�ɑ΂��鏈���������ɒǉ�
		}
	}


	closesocket(sockfd);
	WSACleanup();
	return 0;
}



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

// �\�P�b�g�ɖ��O��t���Đڑ�
static bool BindAndConnectSocket(SOCKET sockfd, unsigned short portNumber) {
	sockaddr_in server_addr{};
	memset(&server_addr, 0, sizeof(SOCKADDR_IN));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(portNumber);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	// sockaddr_in��sockaddr�ɃL���X�g����connect�֐��ɓn��
	if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		printf("�ڑ��Ɏ��s���܂���\n");
		return false;
	}
	return true;
}



int main()
{
	WSADATA wsaData{};

	/* WinSock������ */
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
	{
		// �������G���[
		printf("WinSock�̏������Ɏ��s���܂���\n");
		return FALSE;
	}

	/* �|�[�g�ԍ��̐ݒ� */
	unsigned short portNumber = Get_PortNumber();

	/* �\�P�b�g�쐬 */
	SOCKET sockfd = CreateSocket();

	/* �\�P�b�g�ɖ��O��t���Đڑ� */
	if (!BindAndConnectSocket(sockfd, portNumber)) {
		closesocket(sockfd);
		WSACleanup();
		return FALSE;
	}





	/* �\�P�b�g����� */
	closesocket(sockfd);

	/* WinSock�I�� */
	WSACleanup();

	return TRUE;
}



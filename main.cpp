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
	SOCKET mySocket = socket(AF_INET, SOCK_STREAM, 0);

	/* �\�P�b�g�ɖ��O��t���� */
	//mySocket = bind(mySocket, )


	/* WinSock�I�� */
	WSACleanup();

	return TRUE;
}



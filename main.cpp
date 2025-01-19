#define _WINDOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <stdio.h>

#pragma comment(lib, "WSock32.lib")



// ポート番号を設定する
static unsigned short Get_PortNumber()
{
	unsigned short portNumber = 0;

	while (1) { // 無限ループで正しい入力を待つ
		// プロンプトの表示
		printf("ポート番号の設定 (0 ～ 65535): ");

		// 入力取得とエラーチェック
		if (scanf_s("%hu", &portNumber) == 1) {
			return portNumber; // 正常な入力なら返す
		}
		else {
			// 入力エラー時の処理
			printf("無効な入力です。もう一度入力してください。\n");

			// 入力バッファのクリア
			while (getchar() != '\n');
		}
	}
}

// ソケットを作成する関数
static SOCKET CreateSocket() {
	// ソケットの作成
	SOCKET mySocket = socket(AF_INET, SOCK_STREAM, 0);
	if (mySocket == INVALID_SOCKET) {
		printf("ソケット作成に失敗しました。エラーコード: %d\n", WSAGetLastError());
		return INVALID_SOCKET;
	}
	return mySocket;
}

// サーバーアドレスを設定する関数
static void SetServerAddress(sockaddr_in& server_addr, unsigned short portNumber) {
	memset(&server_addr, 0, sizeof(sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(portNumber);
	server_addr.sin_addr.s_addr = INADDR_ANY;
}

// ソケットをバインドする関数
static bool BindSocket(SOCKET sockfd, const sockaddr_in& server_addr) {
	if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
		printf("バインドに失敗しました。エラーコード: %d\n", WSAGetLastError());
		return false;
	}
	return true;
}

// ソケットをリッスン状態にする関数
static bool ListenSocket(SOCKET sockfd) {
	if (listen(sockfd, SOMAXCONN) == SOCKET_ERROR) {
		printf("リッスンに失敗しました。エラーコード: %d\n", WSAGetLastError());
		return false;
	}
	return true;
}

// ソケットに名前を付けて接続
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

// クライアントからの接続を受け入れる
static SOCKET AcceptClientConnection(SOCKET listenSock) {
	sockaddr_in client_addr{};
	int client_addr_size = sizeof(client_addr);

	// クライアントからの接続を受け入れる
	SOCKET clientSock = accept(listenSock, (struct sockaddr*)&client_addr, &client_addr_size);
	if (clientSock == INVALID_SOCKET) {
		printf("クライアントの接続受け入れに失敗しました。エラーコード: %d\n", WSAGetLastError());
		return INVALID_SOCKET;
	}

	printf("クライアントが接続しました\n");
	return clientSock;
}




int main()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("WSAStartupに失敗しました。エラーコード: %d\n", WSAGetLastError());
		return 1;
	}

	SOCKET sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == INVALID_SOCKET) {
		printf("ソケットの作成に失敗しました。エラーコード: %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	unsigned short portNumber = 8080;
	if (!BindAndConnectSocket(sockfd, portNumber)) {
		closesocket(sockfd);
		WSACleanup();
		return 1;
	}

	printf("サーバーがポート %d でリッスンを開始しました。\n", portNumber);


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
				printf("データ受信に失敗しました。エラーコード: %d\n", WSAGetLastError());
				closesocket(clientSock);
				break;
			}
			else if (nRcv == 0) {
				printf("クライアントが接続を閉じました。\n");
				closesocket(clientSock);
				break;
			}

			szBuf[nRcv] = '\0'; // 受信データを文字列として扱うために終端文字を追加
			printf("受信データ: %s\n", szBuf);

			// 受信データに対する処理をここに追加
		}
	}


	closesocket(sockfd);
	WSACleanup();
	return 0;
}



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



int main()
{
	WSADATA wsaData{};

	/* WinSock初期化 */
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
	{
		// 初期化エラー
		printf("WinSockの初期化に失敗しました\n");
		return FALSE;
	}

	/* ポート番号の設定 */
	unsigned short portNumber = Get_PortNumber();

	/* ソケット作成 */
	SOCKET mySocket = socket(AF_INET, SOCK_STREAM, 0);

	/* ソケットに名前を付ける */
	//mySocket = bind(mySocket, )


	/* WinSock終了 */
	WSACleanup();

	return TRUE;
}



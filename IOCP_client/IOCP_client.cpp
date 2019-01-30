// IOCP_client.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <string.h>

#pragma comment(lib, "Ws2_32.lib")

#define MAX_BUFFER 1024
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6666

using namespace std;

char messageBuffer[MAX_BUFFER];
char *ID = new char;
char *PWD = new char;
char *bufID = new char;
char *name = new char;
int age = 0;
char *gender = new char;
char *macAddress = new char;

void Register();

struct SOCKETINFO {
	WSAOVERLAPPED overlapped;
	WSABUF dataBuffer;
	int receiveBytes;
	int sendBytes;
};

int main()
{
	// Winsock Start - winsock.dll load
	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0) {
		printf("Error - Can not load 'winsock.dll' file \n");
		return 1;
	}

	// 1. 소캣 생성
	SOCKET listenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (listenSocket == INVALID_SOCKET) {
		printf("Error - Invalid socket\n");
		return 1;
	}

	// 서버 정보 객체 설정
	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(SOCKADDR_IN));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

	// 2. 연결 요청
	if (connect(listenSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		printf("Error - Fail to connect\n");
		// 4. 소켓 종료
		closesocket(listenSocket);
		// Winsock End
		WSACleanup();
		return 1;
	}
	else {
		printf("Server Connected\n* Enter Message\n->");
	}

	SOCKETINFO *socketInfo;
	DWORD sendBytes;
	DWORD receiveBytes;
	DWORD flags;

	while (1) {
		// 메시지 입력
		ZeroMemory(messageBuffer, MAX_BUFFER);
		int i, bufferLen;
		int j = 0;
		bool status = false;

		cout << "Enter 1 to login or Enter 2 to register> ";

		for (i = 0; 1; i++) {
			messageBuffer[i] = getchar();
			if (messageBuffer[i] == '\n') {
				messageBuffer[i++] = '\0';
				break;
			}
		}
		bufferLen = i;

		if (bufferLen == 1) {
			// 4. 소캣 종료
			closesocket(listenSocket);
			// Winsock End
			WSACleanup();
			return 0;
		}

		if (strcmp(messageBuffer, "1") == 0) { // 1. login
			int i = 0;
			cout << "Enter ID, PW" << endl;
			cout << "ID> ";
			cin.getline(ID, 20, '\n');
			cout << "PWD> ";
			cin.getline(PWD, 45, '\n');
			
			i = sprintf(messageBuffer, "AAAA");
			i += sprintf(messageBuffer + i, "|%s", ID);
			i += sprintf(messageBuffer + i, "|%s", PWD);

			socketInfo = (struct SOCKETINFO*)malloc(sizeof(struct SOCKETINFO));
			memset((void*)socketInfo, 0x00, sizeof(struct SOCKETINFO));
			socketInfo->dataBuffer.len = (unsigned)strlen(messageBuffer);
			socketInfo->dataBuffer.buf = messageBuffer;

			int sendBytes = send(listenSocket, messageBuffer, strlen(messageBuffer), 0);
			if (sendBytes > 0) {
				ZeroMemory(messageBuffer, MAX_BUFFER);
				int receiveBytes = recv(listenSocket, messageBuffer, MAX_BUFFER, 0);
				//recv 한 데이터 내용이 뭐인지에 따라서 status를 잘 바꿔져서 뭐 하면 될 것 같은데
				if (receiveBytes > 0) { // 23 bytes는 오는데
					//if (strcmp(messageBuffer, "Login Success!") == 0) { // login Success
					//	status = true;
					//}
					printf("TRACE - Receive message : %s (%d bytes) \n * Enter Message\n->", messageBuffer, receiveBytes);
				
				}
			}
		}
		else if (strcmp(messageBuffer, "2") == 0) {
			Register();
			socketInfo = (struct SOCKETINFO*)malloc(sizeof(struct SOCKETINFO));
			memset((void*)socketInfo, 0x00, sizeof(struct SOCKETINFO));
			socketInfo->dataBuffer.len = (unsigned)strlen(messageBuffer);
			socketInfo->dataBuffer.buf = messageBuffer;

			// 3-1. 데이터 쓰기
			int sendBytes = send(listenSocket, messageBuffer, strlen(messageBuffer), 0);
			if (sendBytes > 0) {
				// 3-2. 데이터 읽기
				int receiveBytes = recv(listenSocket, messageBuffer, MAX_BUFFER, 0);
				if (receiveBytes > 0) {
					printf("TRACE - Receive message : %s\n (%d)bytes * Enter Message\n->", messageBuffer,receiveBytes);  //login success가 여기서 나오는 것이여
				}
			}
		}
		else {
			cout << "잘못된 입력 " << endl;
		}
		// 정보 조회같은 것을 요구할 때 id를 같이 보내야지 자신만의 정보를 조회 할 수 있다.
		// receive 했을 때 login이 성공했다고 한다면 if=>로 해서 
		// if( status == true ) { while(1) {  } }
		// login Success!|dlek1011
		// login Success! 따로 dlek1011 따로 분리해서 내가 어떤 상태에서 로그인을 한 것인지 알고있어야함
		// while(1){send 해서 CCCC로하던지 해서 정보 조회가 먼지 확인해보면 돼}
		// 조회, 수정, logout?
		if (status == true) {
			while (1) {
				// cout해서 어떤 메뉴가 있는지 선언을 해주고 1. 나의 정보 조회 2. 정보 수정, 3. 로그아웃
				cout << "1. 나의 정보 조회, 2. 정보 수정, 3. 로그아웃> ";
				for (i = 0; 1; i++) {
					messageBuffer[i] = getchar();
					if (messageBuffer[i] == '\n') {
						messageBuffer[i++] = '\0';
						break;
					}
				}
				if (strcmp(messageBuffer, "1") == 0) {

				}
				else if (strcmp(messageBuffer, "2") == 0) {

				}
				else if (strcmp(messageBuffer, "3") == 0) {

				}
				else {
					cout << "잘못된 입력" << endl;
				}
			}
		 }
	}
    return 0;
}

void Register() {
	int i;
	cout << "Enter ID, PW what you want" << endl;
	cout << "ID> ";
	cin.getline(ID, 20, '\n');
	cout << "PWD> ";
	cin.getline(PWD, 45, '\n');
	cout << "Enter your name, age, gender, macAddress" << endl;
	cout << "Enter your name> ";
	ZeroMemory(messageBuffer, MAX_BUFFER);
	cin.getline(name, 100, '\n');
	cout << "Enter your age> ";
	cin >> age;
	getchar();
	cout << "Enter your gender(Female : F, Male : M)> ";
	cin.getline(gender, 5, '\n');
	cout << "Enter your macAddress> ";
	cin.getline(macAddress, 100, '\n');

	i = sprintf(messageBuffer, "BBBB "); // Header
	i += sprintf(messageBuffer + i, "('%s', ", ID);
	i += sprintf(messageBuffer + i, "SHA1('%s'), ", PWD);
	i += sprintf(messageBuffer + i, "'%s', ", name);

	i += sprintf(messageBuffer + i, "%d, ", age);
	i += sprintf(messageBuffer + i, "'%s', ", gender);
	i += sprintf(messageBuffer + i, "'%s');", macAddress);
}
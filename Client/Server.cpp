#include "stdafx.h"
#include "Server.h"



// # server variables
bool server_on = false;
char ip[16];
UINT myID;
UINT monsterID;
int partyIDs[4] = { -1, -1, -1,-1 };
UINT partyNum = 0;
SOCKET	sock;
bool first_login = true;
BYTE player_status = 0;
int select1 = 0;

HANDLE	recv_thread;
WSABUF	recv_wsabuf;
char	send_buffer[MAX_BUFF_SIZE];
char	recv_buffer[MAX_BUFF_SIZE];
char	packet_buffer[MAX_BUFF_SIZE];
DWORD	in_packet_size = 0;
int		saved_packet_size = 0;



void err_quit(char *msg) {
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	//MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	wchar_t* pConvertedMsg;
	int msgSize = (int)strlen(msg) + 1;
	pConvertedMsg = new wchar_t[msgSize];
	mbstowcs(pConvertedMsg, msg, msgSize);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, pConvertedMsg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(-1);
}

void err_display(char *msg) {
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %Ls", msg, (LPCTSTR)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

void ServerConnect() {
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return;

	// socket()
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(ip);
	serveraddr.sin_port = htons(SERVER_PORT);
	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");
	player_status = static_cast<BYTE>(ePlayer_State::eLOBBY);

	// active receive_thread
	recv_thread = CreateThread(NULL, 0, recvThread, NULL, 0, 0);
}

DWORD WINAPI recvThread(LPVOID arg) {
	while (1) {
		recv_wsabuf.buf = recv_buffer;
		recv_wsabuf.len = MAX_BUFF_SIZE;
		DWORD iobyte, ioflag = 0;

		int ret = WSARecv(sock, &recv_wsabuf, 1, &iobyte, &ioflag, NULL, NULL);
		if (ret) {
			int err_code = WSAGetLastError();
			printf("Recv Error [%d]\n", err_code);
			return 0;
		}

		BYTE *ptr = reinterpret_cast<BYTE *>(recv_buffer);
		while (0 != iobyte) {
			if (0 == in_packet_size)  in_packet_size = ptr[0];
			if (iobyte + saved_packet_size >= in_packet_size) {
				memcpy(packet_buffer + saved_packet_size, ptr, in_packet_size - saved_packet_size);
				ProcessPacket(packet_buffer);
				ptr += in_packet_size - saved_packet_size;
				iobyte -= in_packet_size - saved_packet_size;
				in_packet_size = 0;
				saved_packet_size = 0;
			}
			else {
				memcpy(packet_buffer + saved_packet_size, ptr, iobyte);
				saved_packet_size += iobyte;
				iobyte = 0;
			}
		}
	}
	return 0;
}




//void game_thread() {
//	char ch;
//	int retval;
//	while (1) {
//		if (player_status == LOBBY) {
//			cout << "1. 방생성 2. 방입장 3. 채팅하기" << endl;
//			cin >> select1;
//			switch (select1) {
//			case 1:
//			{
//				printf("방을 생성합니다\n");
//				printf("방이름을 입력해주세요\n");
//				create_room* pp = reinterpret_cast<create_room*>(send_buffer);
//				pp->type = CREATE_ROOM;
//				pp->id = myID;
//				gets_s(pp->room_name);
//				gets_s(pp->room_name);
//				printf("입력한방이름:%s\n", pp->room_name);
//				printf("플레이할 스테이지를 입력해주세요\n");
//				scanf("%d", &pp->stage);
//				printf("입력한스테이지:%d\n", pp->stage);
//				pp->size = sizeof(*pp);
//				printf("전송하는사이즈:%d\n", pp->size);
//				retval = send(sock, (char*)pp, sizeof(*pp), 0);
//				if (retval == SOCKET_ERROR)
//					printf("send ERROR\n");
//				Sleep(100);
//				break;
//			}
//			case 2:
//			{
//				printf("방에 입장합니다\n");
//				printf("입장할 방이름을 입력해주세요\n");
//				join_room* pp = reinterpret_cast<join_room*>(send_buffer);
//				pp->type = JOIN_ROOM;
//				pp->id = myID;
//				gets_s(pp->room_name);
//				gets_s(pp->room_name);
//				printf("입력한방이름:%s\n", pp->room_name);
//				pp->size = sizeof(*pp);
//				printf("전송하는사이즈:%d\n", pp->size);
//				retval = send(sock, (char*)pp, sizeof(*pp), 0);
//				if (retval == SOCKET_ERROR)
//					printf("send ERROR\n");
//				Sleep(100);
//				break;
//			}
//			case 3:
//			{
//				printf("로비에서 채팅합니다\n");
//				printf("무슨 말을 보내겠습니까?\n");
//				packet_chat* pp = reinterpret_cast<packet_chat*>(send_buffer);
//				pp->type = LOBBY_CHAT;
//				pp->size = sizeof(*pp);
//				gets_s(pp->message);
//				gets_s(pp->message);
//				printf("보낼 메세지:%s\n", pp->message);
//
//				printf("전송하는사이즈:%d\n", pp->size);
//				retval = send(sock, (char*)pp, sizeof(*pp), 0);
//				if (retval == SOCKET_ERROR)
//					printf("send ERROR\n");
//				break;
//			}
//			}
//		}
//		else if (player_status == ROOM) {
//			cout << "1. 채팅하기 2. 게임시작하기 3. 나가기" << endl;
//			cin >> select1;
//			switch (select1) {
//			case 1: {
//				printf("룸에서 채팅합니다\n");
//				printf("무슨 말을 보내겠습니까?\n");
//				packet_chat* pp = reinterpret_cast<packet_chat*>(send_buffer);
//				pp->type = ROOM_CHAT;
//				pp->size = sizeof(*pp);
//				gets_s(pp->message);
//				gets_s(pp->message);
//				printf("보낼 메세지:%s\n", pp->message);
//
//				printf("전송하는사이즈:%d\n", pp->size);
//				retval = send(sock, (char*)pp, sizeof(*pp), 0);
//				if (retval == SOCKET_ERROR)
//					printf("send ERROR\n");
//				break;
//			}
//			case 2: {
//				printf("게임을 시작합니다\n");
//				game_start* pp = reinterpret_cast<game_start*>(send_buffer);
//				pp->type = GAME_START;
//				pp->id = myID;
//				pp->size = sizeof(*pp);
//				printf("전송하는사이즈:%d\n", pp->size);
//				retval = send(sock, (char*)pp, sizeof(*pp), 0);
//				if (retval == SOCKET_ERROR)
//					printf("send ERROR\n");
//				Sleep(100);
//				break;
//			}
//			case 3: {
//				printf("방에서 나갑니다\n");
//				exit_room* pp = reinterpret_cast<exit_room*>(send_buffer);
//				pp->type = ROOM_EXIT;
//				pp->id = myID;
//				pp->size = sizeof(*pp);
//				printf("전송하는사이즈:%d\n", pp->size);
//				retval = send(sock, (char*)pp, sizeof(*pp), 0);
//				if (retval == SOCKET_ERROR)
//					printf("send ERROR\n");
//				player_status = LOBBY;
//				Sleep(100);
//				break;
//			}
//			default:
//				break;
//			}
//		}
//		else if (player_status == FIGHT) {
//			if (_kbhit()) {
//				ch = getch();
//				switch (tolower(ch)) {
//				case 'q':
//				{
//					game_exit* pp = reinterpret_cast<game_exit*>(send_buffer);
//					pp->size = sizeof(*pp);
//					pp->type = GAME_EXIT;
//					pp->id = myID;
//					retval = send(sock, (char*)pp, sizeof(*pp), 0);
//					if (retval == SOCKET_ERROR)
//						printf("event data send ERROR\n");
//					Sleep(100);
//					break;
//				}
//				case 'w':
//				{
//					packet_player_move* pp = reinterpret_cast<packet_player_move*>(send_buffer);
//					pp->size = sizeof(*pp);
//
//					pp->type = PLAYER_MOV;
//					pp->move_type = 1;
//					retval = send(sock, (char*)pp, sizeof(*pp), 0);
//					if (retval == SOCKET_ERROR)
//						printf("event data send ERROR\n");
//					Sleep(100);
//					break;
//				}
//				case 's':
//				{
//					packet_player_move* pp = reinterpret_cast<packet_player_move*>(send_buffer);
//					pp->size = sizeof(*pp);
//
//					pp->type = PLAYER_MOV;
//					pp->move_type = 2;
//					retval = send(sock, (char*)pp, sizeof(*pp), 0);
//					if (retval == SOCKET_ERROR)
//						printf("event data send ERROR\n");
//					Sleep(100);
//					break;
//				}
//
//				case 'a':
//				{
//					packet_player_move* pp = reinterpret_cast<packet_player_move*>(send_buffer);
//
//					pp->size = sizeof(*pp);
//					pp->type = PLAYER_MOV;
//					pp->move_type = 3;
//					retval = send(sock, (char*)pp, sizeof(*pp), 0);
//					if (retval == SOCKET_ERROR)
//						printf("event data send ERROR\n");
//					Sleep(100);
//					break;
//				}
//
//				case 'd':
//				{
//					packet_player_move* pp = reinterpret_cast<packet_player_move*>(send_buffer);
//
//					pp->size = sizeof(*pp);
//					pp->type = PLAYER_MOV;
//					pp->move_type = 4;
//					retval = send(sock, (char*)pp, sizeof(*pp), 0);
//					if (retval == SOCKET_ERROR)
//						printf("event data send ERROR\n");
//					Sleep(100);
//					break;
//				}
//				}
//			}
//		}
//	}
//}
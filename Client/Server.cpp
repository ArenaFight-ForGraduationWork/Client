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

	// ���� �ʱ�ȭ
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
//			cout << "1. ����� 2. ������ 3. ä���ϱ�" << endl;
//			cin >> select1;
//			switch (select1) {
//			case 1:
//			{
//				printf("���� �����մϴ�\n");
//				printf("���̸��� �Է����ּ���\n");
//				create_room* pp = reinterpret_cast<create_room*>(send_buffer);
//				pp->type = CREATE_ROOM;
//				pp->id = myID;
//				gets_s(pp->room_name);
//				gets_s(pp->room_name);
//				printf("�Է��ѹ��̸�:%s\n", pp->room_name);
//				printf("�÷����� ���������� �Է����ּ���\n");
//				scanf("%d", &pp->stage);
//				printf("�Է��ѽ�������:%d\n", pp->stage);
//				pp->size = sizeof(*pp);
//				printf("�����ϴ»�����:%d\n", pp->size);
//				retval = send(sock, (char*)pp, sizeof(*pp), 0);
//				if (retval == SOCKET_ERROR)
//					printf("send ERROR\n");
//				Sleep(100);
//				break;
//			}
//			case 2:
//			{
//				printf("�濡 �����մϴ�\n");
//				printf("������ ���̸��� �Է����ּ���\n");
//				join_room* pp = reinterpret_cast<join_room*>(send_buffer);
//				pp->type = JOIN_ROOM;
//				pp->id = myID;
//				gets_s(pp->room_name);
//				gets_s(pp->room_name);
//				printf("�Է��ѹ��̸�:%s\n", pp->room_name);
//				pp->size = sizeof(*pp);
//				printf("�����ϴ»�����:%d\n", pp->size);
//				retval = send(sock, (char*)pp, sizeof(*pp), 0);
//				if (retval == SOCKET_ERROR)
//					printf("send ERROR\n");
//				Sleep(100);
//				break;
//			}
//			case 3:
//			{
//				printf("�κ񿡼� ä���մϴ�\n");
//				printf("���� ���� �����ڽ��ϱ�?\n");
//				packet_chat* pp = reinterpret_cast<packet_chat*>(send_buffer);
//				pp->type = LOBBY_CHAT;
//				pp->size = sizeof(*pp);
//				gets_s(pp->message);
//				gets_s(pp->message);
//				printf("���� �޼���:%s\n", pp->message);
//
//				printf("�����ϴ»�����:%d\n", pp->size);
//				retval = send(sock, (char*)pp, sizeof(*pp), 0);
//				if (retval == SOCKET_ERROR)
//					printf("send ERROR\n");
//				break;
//			}
//			}
//		}
//		else if (player_status == ROOM) {
//			cout << "1. ä���ϱ� 2. ���ӽ����ϱ� 3. ������" << endl;
//			cin >> select1;
//			switch (select1) {
//			case 1: {
//				printf("�뿡�� ä���մϴ�\n");
//				printf("���� ���� �����ڽ��ϱ�?\n");
//				packet_chat* pp = reinterpret_cast<packet_chat*>(send_buffer);
//				pp->type = ROOM_CHAT;
//				pp->size = sizeof(*pp);
//				gets_s(pp->message);
//				gets_s(pp->message);
//				printf("���� �޼���:%s\n", pp->message);
//
//				printf("�����ϴ»�����:%d\n", pp->size);
//				retval = send(sock, (char*)pp, sizeof(*pp), 0);
//				if (retval == SOCKET_ERROR)
//					printf("send ERROR\n");
//				break;
//			}
//			case 2: {
//				printf("������ �����մϴ�\n");
//				game_start* pp = reinterpret_cast<game_start*>(send_buffer);
//				pp->type = GAME_START;
//				pp->id = myID;
//				pp->size = sizeof(*pp);
//				printf("�����ϴ»�����:%d\n", pp->size);
//				retval = send(sock, (char*)pp, sizeof(*pp), 0);
//				if (retval == SOCKET_ERROR)
//					printf("send ERROR\n");
//				Sleep(100);
//				break;
//			}
//			case 3: {
//				printf("�濡�� �����ϴ�\n");
//				exit_room* pp = reinterpret_cast<exit_room*>(send_buffer);
//				pp->type = ROOM_EXIT;
//				pp->id = myID;
//				pp->size = sizeof(*pp);
//				printf("�����ϴ»�����:%d\n", pp->size);
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
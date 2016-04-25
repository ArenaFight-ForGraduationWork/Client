#include "stdafx.h"
#include "protocol.h"
#include "ObjectManager.h"


WSABUF   send_wsabuf;
char    send_buffer[MAX_BUFF_SIZE];
WSABUF   recv_wsabuf;
char   recv_buffer[MAX_BUFF_SIZE];
char   packet_buffer[MAX_BUFF_SIZE];
DWORD   in_packet_size = 0;
int      saved_packet_size = 0;
int      myid;
SOCKET sock;
////////////////////////////////////////////////////////

// 소켓 함수 오류 출력 후 종료
void err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	printf("Connect error : %d\n", WSAGetLastError());
	LocalFree(lpMsgBuf);
}

// 소켓 함수 오류 출력
void err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}



void Send_Packet(void *packet)
{
	network_info *over_ex = new network_info;
	int packet_size = reinterpret_cast<char *>(packet)[0];
	memcpy(over_ex->IOCPbuf, packet, packet_size);
	over_ex->is_recv = false;
	ZeroMemory(&over_ex->overlapped, sizeof(WSAOVERLAPPED));
	over_ex->wsabuf.buf = over_ex->IOCPbuf;
	over_ex->wsabuf.len = packet_size;
	unsigned long IOsize;
	WSASend(sock, &over_ex->wsabuf, 1,
		&IOsize, NULL, &over_ex->overlapped, NULL);
}

void ServerConnect()
{
	
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)	{
		exit(-1);
	}
		

	// socket()
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		err_quit("socket()");
	}
	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serveraddr.sin_port = htons(SERVER_PORT);
	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) {
		err_quit("connect()");
	}

	
}

/*
int main(int argc, char *argv[])
{
	// closesocket()
	closesocket(sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}
*/

CObjectManager *m_pObjectManager;

void ProcessPacket(char *ptr)
{
	cout << "ProcessPacket" << endl;

	switch (ptr[1])
	{
	case 1:
		break;
	case 2:
		break;
	case LOGIN:
	{
		try {
			all_user_view *my_packet = reinterpret_cast<all_user_view *>(ptr);
			printf("id:%d\n", my_packet->yourid);
		}
		catch (exception e) {
			cout << e.what() << endl;
		}
		break;
	}
	default:
		printf("Unknown PACKET type [%d]\n", ptr[1]);
	}
}


void SendPosPacket(float x, float y, float z)
{

	int retval;
	packet_player_move* pp = reinterpret_cast<packet_player_move*>(send_buffer);
	pp->size = sizeof(*pp);

	pp->type = PLAYER_MOVE;
	pp->id = 0;
	pp->x = 3;
	pp->y = 200;
	pp->z = 2;
	pp->direction = 0;
	retval = send(sock, (char*)pp, sizeof(*pp), 0);
	cout << "send 중 : " << pp->x << ", " << pp->y << ", " << pp->z << endl;
	if (retval == SOCKET_ERROR)
		printf("SendPosPacket\n");
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
			if (0 == in_packet_size) in_packet_size = ptr[0];
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
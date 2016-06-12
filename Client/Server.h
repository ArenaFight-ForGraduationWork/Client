#ifndef SERVER_H_
#define SERVER_H_

#include "stdafx.h"
#include "protocol.h"



// # server variables
extern bool		server_on;
extern char		ip[16];
extern UINT		myID;
extern SOCKET	sock;
extern bool		first_login;

extern WSABUF	recv_wsabuf;
extern char		send_buffer[MAX_BUFF_SIZE];
extern char		recv_buffer[MAX_BUFF_SIZE];
extern char		packet_buffer[MAX_BUFF_SIZE];
extern DWORD	in_packet_size;
extern int		saved_packet_size;

extern bool key_up;
extern bool key_down;
extern bool key_left;
extern bool key_right;



// # server funcions
void err_quit(char *msg);
void err_display(char *msg);
void ServerConnect();
void ProcessPacket(char *ptr);
DWORD WINAPI recvThread(LPVOID arg);




#endif
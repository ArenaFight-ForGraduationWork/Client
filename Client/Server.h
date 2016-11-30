#ifndef SERVER_H_
#define SERVER_H_

#include "stdafx.h"
#include <conio.h>
#include "protocol.h"



// # server variables
extern bool		server_on;
extern char		ip[16];
extern UINT		myID;
extern UINT		monsterID;
extern int		partyIDs[4];
extern UINT		partyNum;
extern SOCKET	sock;
extern bool		first_login;
extern BYTE		player_status;
extern int		select1;

extern HANDLE	recv_thread;
extern WSABUF	recv_wsabuf;
extern char		send_buffer[MAX_BUFF_SIZE];
extern char		recv_buffer[MAX_BUFF_SIZE];
extern char		packet_buffer[MAX_BUFF_SIZE];
extern DWORD	in_packet_size;
extern int		saved_packet_size;




// # server funcions
void err_quit(char *msg);
void err_display(char *msg);
void ServerConnect();
void ProcessPacket(char *ptr);
DWORD WINAPI recvThread(LPVOID arg);

//void game_thread();




#endif
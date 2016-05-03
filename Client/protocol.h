#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include "stdafx.h"

#pragma pack (push, 1)
struct network_info {
	WSAOVERLAPPED overlapped;
	bool is_recv;
	WSABUF wsabuf;
	char IOCPbuf[MAX_BUFF_SIZE];
	char PacketBuf[MAX_PACKET_SIZE];
	int prev_data_size;
	int curr_packet_size;
};

struct Player {
	float x = 0, y = 0, z = 0;
	float dx = 0, dy = 0, dz = 0;
	short direction = 0;
};
struct create_room {
	BYTE size;
	BYTE type;
	BYTE id;
	//int room_name;
	char room_name[MAX_CHAT_SIZE];
	BYTE stage;
};
struct join_room {
	BYTE size;
	BYTE type;
	int id;
	char room_name[MAX_CHAT_SIZE];
};

//struct packet_player_move {
//	BYTE size;
//	BYTE type;
//	BYTE id;
//	BYTE x;
//	BYTE y;
//	BYTE z;
//	BYTE direction;
//};

struct all_user_view {
	BYTE size;
	BYTE type;
	BYTE id[100];
	BYTE count;
	BYTE yourid;
};

struct packet_player_move {
	BYTE size;
	BYTE type;
	int id;
	float x;
	float y;
	float z;
	float dx;
	float dy;
	float dz;
	short direction;
};

struct packet_chat {
	BYTE size;
	BYTE type;
	WCHAR message[MAX_CHAT_SIZE];
};
struct packet_put_player {
	BYTE size;
	BYTE type;
	WORD id;
};
struct packet_remove_player {
	BYTE size;
	BYTE type;
	WORD id;
};
#pragma pack (pop)


void ServerConnect();
void ReadPacket();
void SendPosPacket(float, float, float);

DWORD WINAPI recvThread(LPVOID);


#endif
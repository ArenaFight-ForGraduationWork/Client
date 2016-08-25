#ifndef PROTOCOL_H_
#define PROTOCOL_H_



#define MAX_BUFF_SIZE   4096
#define MAX_PACKET_SIZE  4096
#define MAX_USER 1000
#define MAX_CHAT_SIZE  100

#define SERVER_PORT  4000


// 클라 -> 서버 누른 키확인
#define W		1
#define S		2
#define A		3
#define D		4

//// 플레이어 스킬
//#define NORMAL_ATTACK	1
//#define PLAYER_SKILL1	2
//#define PLAYER_SKILL2	3
//#define PLAYER_SKILL3	4

// 서버 -> 클라
#define LOGIN         1
#define PUT_PLAYER      2
#define PLAYER_POS      3
#define REMOVE_PLAYER   4
#define CREATE_ROOM_S   5
#define CREATE_ROOM_F   6
#define JOIN_ROOM_S      7
#define JOIN_ROOM_F      8
#define GAME_START_S    9
#define GAME_START_F    10
#define CHATTING       11
#define ROOM_PUT_PLAYER 12
#define OUT_ROOM      13
#define BOSS_POS      14
#define GAME_EXIT_S      15
#define SC_PLAYER_ATTACK	16
#define SC_BOSS_ATTACK		17
#define SC_PLAYER_ATTACK_S	18
#define SC_PLAYER_MOV_END	19
#define BOSS_DEAD			20
#define LOBBY_PUT_PLAYER 21

// 클라 -> 서버
#define CREATE_ROOM      1
#define JOIN_ROOM      2
#define PLAYER_MOV      3
#define ROOM_EXIT      4
#define GAME_START      5
#define LOBBY_CHAT      6
#define ROOM_CHAT      7
#define GAME_EXIT      8
#define PLAYER_MOV_END	9
#define PLAYER_ATTACK	10

#define LOBBY 1
#define ROOM  2
#define FIGHT 3

//// 보스상태
//#define BOSS_NORMAL	1
//#define BOSS_MOVE	2
//#define BOSS_ATTACK	3
//
//// 보스공격
//#define BOSS_NORMAL_ATTACK 1
//#define BOSS_SKILL1 2
//#define BOSS_SKILL2 3
//#define BOSS_SKILL3 4

#pragma pack (push, 1)
struct create_room {
	BYTE size;
	BYTE type;
	int id;
	//int room_name;
	char room_name[MAX_CHAT_SIZE];
	BYTE stage;
};
struct create_room_s {
	BYTE size;
	BYTE type;
	int id;
};
struct create_room_f {
	BYTE size;
	BYTE type;
	int id;
};
struct exit_room {
	BYTE size;
	BYTE type;
	int id;
};
struct join_room {
	BYTE size;
	BYTE type;
	int id;
	char room_name[MAX_CHAT_SIZE];
};
struct join_room_s {
	BYTE size;
	BYTE type;
	int id[4] = { -1, -1, -1, -1 };
};
struct join_room_f {
	BYTE size;
	BYTE type;
	int id = -1;
};
struct out_room {
	BYTE size;
	BYTE type;
	int id = -1;
	int room_master;
};

struct game_start {
	BYTE size;
	BYTE type;
	int id;
};

struct game_exit {
	BYTE size;
	BYTE type;
	int id;
};

struct game_start_s {
	BYTE size;
	BYTE type;
	int id[4] = { -1, -1, -1, -1 };

	// 임시
	int bossx = 5;
	int bossz = 2000;
	int bossid = 1000;
	int bossdis = 5;
};
struct game_start_f {
	BYTE size;
	BYTE type;
};
struct login {
	BYTE size;
	BYTE type;
	int id;
};
struct packet_chat {
	BYTE size;
	BYTE type;
	int id;
	char message[MAX_CHAT_SIZE];
};
//struct sc_packet_chat {
//   BYTE size;
//   BYTE type;
//   int id;
//   char message[MAX_CHAT_SIZE];
//};
struct lobby_put_player {
	BYTE size;
	BYTE type;
	int id;
};
struct player_position {
	BYTE size;
	BYTE type;
	int id;
	float x;
	float z;
	float direction;
	bool isMoving;
};
struct put_player {
	BYTE size;
	BYTE type;
	int id;
};
struct remove_player {
	BYTE size;
	BYTE type;
	int id;
};
#pragma pack (pop)
#endif
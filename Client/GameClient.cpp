#include "stdafx.h"
#include "GameClient.h"
#include "GameFramework.h"
#include "protocol.h"
#include "ObjectManager.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;						// 현재 인스턴스
TCHAR szTitle[MAX_LOADSTRING];			// 제목 표시줄 텍스트
TCHAR szWindowClass[MAX_LOADSTRING];	// 기본 창 클래스 이름

CGameFramework	gGameFramework;
HINSTANCE		ghInstance;

// # server variables
bool server_on = false;
char ip[16];
UINT myID;

WSABUF	recv_wsabuf;
char	send_buffer[MAX_BUFF_SIZE];
char	recv_buffer[MAX_BUFF_SIZE];
char	packet_buffer[MAX_BUFF_SIZE];
DWORD	in_packet_size = 0;
int		saved_packet_size = 0;

SOCKET	sock;




// 이 코드 모듈에 들어 있는 함수의 정방향 선언
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

// # server funcions
void err_quit(char *msg);
void err_display(char *msg);
void ServerConnect();
void ProcessPacket(char *ptr);
DWORD WINAPI recvThread(LPVOID arg);


// ==============임시라 쓸모 없음
//
//
bool volatile dead = true;
//
//
// ==============================


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 여기에 코드를 입력합니다.
	MSG msg;
	HACCEL hAccelTable;

	// 전역 문자열을 초기화합니다.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_GAMECLIENT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 응용 프로그램 초기화를 수행합니다.
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GAMECLIENT));

	// 기본 메시지 루프입니다.
	while (1)
	{
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;
			if (!::TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}
		else
		{
			gGameFramework.FrameAdvance();
		}
	}
	gGameFramework.OnDestroy();

	return (int)msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GAMECLIENT));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_GAMECLIENT);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	ghInstance = hInstance;

	// Connect Server
	cout << "ip : ";
	cin >> ip;
	ServerConnect();

	DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_BORDER;
	RECT rc = { 0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT };
	AdjustWindowRect(&rc, dwStyle, FALSE);
	HWND hMainWnd = CreateWindow(szWindowClass, szTitle, dwStyle, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance, NULL);
	if (!hMainWnd) return FALSE;

	gGameFramework.OnCreate(hInstance, hMainWnd);

	::ShowWindow(hMainWnd, nCmdShow);
	::UpdateWindow(hMainWnd);

	return TRUE;
}


//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND	- 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT	- 주 창을 그립니다.
//  WM_DESTROY	- 종료 메시지를 게시하고 반환합니다.
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int retval;
	static HANDLE recv_thread;
	packet_player_move *move_packet;

	switch (message)
	{
	case WM_CREATE:
		SetTimer(hWnd, 1, 10, NULL);
		break;
	case WM_SIZE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
	case WM_MOUSEWHEEL:
		gGameFramework.OnProcessingWindowMessage(hWnd, message, wParam, lParam);
		break;
	case WM_KEYDOWN:
		if (server_on)
		{
			if (dead)
			{
				move_packet = reinterpret_cast<packet_player_move*>(send_buffer);
				if (wParam == VK_UP) {
					move_packet->size = sizeof(*move_packet);
					move_packet->type = PLAYER_MOV;
					move_packet->move_type = 1;
					retval = send(sock, (char*)move_packet, sizeof(*move_packet), 0);
				}
				else if (wParam == VK_DOWN) {
					move_packet->size = sizeof(*move_packet);
					move_packet->type = PLAYER_MOV;
					move_packet->move_type = 2;
					retval = send(sock, (char*)move_packet, sizeof(*move_packet), 0);
				}
				else if (wParam == VK_LEFT) {
					move_packet->size = sizeof(*move_packet);
					move_packet->type = PLAYER_MOV;
					move_packet->move_type = 3;
					retval = send(sock, (char*)move_packet, sizeof(*move_packet), 0);
				}
				else if (wParam == VK_RIGHT) {
					move_packet->size = sizeof(*move_packet);
					move_packet->type = PLAYER_MOV;
					move_packet->move_type = 4;
					retval = send(sock, (char*)move_packet, sizeof(*move_packet), 0);
				}
				dead = false;
			}
		}
		gGameFramework.OnProcessingWindowMessage(hWnd, message, wParam, lParam);
		break;
	case WM_KEYUP:
		if (server_on)
		{
			move_packet = reinterpret_cast<packet_player_move*>(send_buffer);
			if (wParam == VK_UP) {
				move_packet->size = sizeof(*move_packet);
				move_packet->type = PLAYER_MOV_END;
				move_packet->move_type = 1;
				retval = send(sock, (char*)move_packet, sizeof(*move_packet), 0);
			}
			else if (wParam == VK_DOWN) {
				move_packet->size = sizeof(*move_packet);
				move_packet->type = PLAYER_MOV_END;
				move_packet->move_type = 2;
				retval = send(sock, (char*)move_packet, sizeof(*move_packet), 0);
			}
			else if (wParam == VK_LEFT) {
				move_packet->size = sizeof(*move_packet);
				move_packet->type = PLAYER_MOV_END;
				move_packet->move_type = 3;
				retval = send(sock, (char*)move_packet, sizeof(*move_packet), 0);
			}
			else if (wParam == VK_RIGHT) {
				move_packet->size = sizeof(*move_packet);
				move_packet->type = PLAYER_MOV_END;
				move_packet->move_type = 4;
				retval = send(sock, (char*)move_packet, sizeof(*move_packet), 0);
			}
			dead = true;
		}
		gGameFramework.OnProcessingWindowMessage(hWnd, message, wParam, lParam);
		break;
	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			recv_thread = CreateThread(NULL, 0, recvThread, NULL, 0, 0);
			break;
		default:
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// 정보 대화 상자의 메시지 처리기
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}






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

// 소켓 함수 오류 출력
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
	server_on = true;
}

void ProcessPacket(char *ptr) {
	CObjectManager *pObjectManager = CObjectManager::GetSingleton();

	switch (ptr[1]) {
	case LOGIN:
	{	/* here : 내가 들어올 때, 나와 이미 들어와있던 사람들 */
		login *my_packet = reinterpret_cast<login *>(ptr);
		myID = my_packet->yourid;

		/* here : 플레이어 좌표는 어디로 가져와야 하는가 > 안 주니까 000으로 설정 */
		pObjectManager->Insert((UINT)myID, eResourceType::User, gGameFramework.GetDevice(), gGameFramework.GetDeviceContext(),
			D3DXVECTOR3(0, 0, 0));

		for (int i = 0; i < my_packet->count; ++i)
		{
			pObjectManager->Insert(my_packet->id[i], eResourceType::User, gGameFramework.GetDevice(), gGameFramework.GetDeviceContext(),
				D3DXVECTOR3(my_packet->x[i], 0, my_packet->z[i]));
		}
		break;
	}
	case PUT_PLAYER:
	{ /* here : 내가 들어오고 나서 들어온 사람 */
		player_position *my_packet = reinterpret_cast<player_position *>(ptr);

		pObjectManager->Insert(my_packet->id, eResourceType::User, gGameFramework.GetDevice(), gGameFramework.GetDeviceContext(), 
			D3DXVECTOR3(my_packet->x, 0, my_packet->z));
		break;
	}
	case PLAYER_POS:
	{
		player_position *my_packet = reinterpret_cast<player_position *>(ptr);

		pObjectManager->FindObject(my_packet->id)->SetPosition(new D3DXVECTOR3(my_packet->x, 0, my_packet->z));
		break;
	}
	case BOSS_POS:
	{
		player_position *my_packet = reinterpret_cast<player_position *>(ptr);

		if (pObjectManager->FindObject(my_packet->id))
			pObjectManager->FindObject(my_packet->id)->SetPosition(new D3DXVECTOR3(my_packet->x, 0, my_packet->z));
		else
			pObjectManager->Insert(my_packet->id, eResourceType::Monster1, gGameFramework.GetDevice(), gGameFramework.GetDeviceContext(),
				D3DXVECTOR3(my_packet->x, 0, my_packet->z));
		break;
	}
	default:
		printf("Unknown PACKET type [%d]\n", ptr[1]);
	}
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




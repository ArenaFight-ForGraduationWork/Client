#include "stdafx.h"
#include "GameClient.h"
#include "GameFramework.h"
#include "protocol.h"
#include "ObjectManager.h"

#define MAX_LOADSTRING 100

// ���� ����:
HINSTANCE hInst;						// ���� �ν��Ͻ�
TCHAR szTitle[MAX_LOADSTRING];			// ���� ǥ���� �ؽ�Ʈ
TCHAR szWindowClass[MAX_LOADSTRING];	// �⺻ â Ŭ���� �̸�

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




// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ ����
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


// ==============�ӽö� ���� ����
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

	// TODO: ���⿡ �ڵ带 �Է��մϴ�.
	MSG msg;
	HACCEL hAccelTable;

	// ���� ���ڿ��� �ʱ�ȭ�մϴ�.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_GAMECLIENT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GAMECLIENT));

	// �⺻ �޽��� �����Դϴ�.
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
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
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
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
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
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����:  �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND	- ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT	- �� â�� �׸��ϴ�.
//  WM_DESTROY	- ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
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

// ���� ��ȭ ������ �޽��� ó����
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

// ���� �Լ� ���� ���
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
	server_on = true;
}

void ProcessPacket(char *ptr) {
	CObjectManager *pObjectManager = CObjectManager::GetSingleton();

	switch (ptr[1]) {
	case LOGIN:
	{	/* here : ���� ���� ��, ���� �̹� �����ִ� ����� */
		login *my_packet = reinterpret_cast<login *>(ptr);
		myID = my_packet->yourid;

		/* here : �÷��̾� ��ǥ�� ���� �����;� �ϴ°� > �� �ִϱ� 000���� ���� */
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
	{ /* here : ���� ������ ���� ���� ��� */
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




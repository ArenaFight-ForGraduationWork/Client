#include "stdafx.h"
#include "GameClient.h"
#include "Server.h"
#include "ObjectManager.h"
#include "GameFramework.h"



#define MAX_LOADSTRING 100

// # global variables
HINSTANCE hInst;						// ���� �ν��Ͻ�
TCHAR szTitle[MAX_LOADSTRING];			// ���� ǥ���� �ؽ�Ʈ
TCHAR szWindowClass[MAX_LOADSTRING];	// �⺻ â Ŭ���� �̸�

CGameFramework	gGameFramework;
HINSTANCE		ghInstance;

// # functions
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);





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
	case WM_KEYDOWN:
	case WM_KEYUP:
		gGameFramework.OnProcessingWindowMessage(hWnd, message, wParam, lParam);
		break;
	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			recv_thread = CreateThread(NULL, 0, recvThread, NULL, 0, 0);
			break;
		default:break;
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

		if (pObjectManager->FindObject(my_packet->bossid))
			pObjectManager->FindObject(my_packet->bossid)->SetPositionAbsolute(new D3DXVECTOR3(my_packet->bossx, 0, my_packet->bossy));
		else
			pObjectManager->Insert(my_packet->bossid, eResourceType::Monster1, gGameFramework.GetDevice(), gGameFramework.GetDeviceContext(),
				D3DXVECTOR3(my_packet->bossx, 0, my_packet->bossy));
	}break;
	case PUT_PLAYER:
	{ /* here : ���� ������ ���� ���� ��� */
		player_position *my_packet = reinterpret_cast<player_position *>(ptr);

		pObjectManager->Insert(my_packet->id, eResourceType::User, gGameFramework.GetDevice(), gGameFramework.GetDeviceContext(),
			D3DXVECTOR3(my_packet->x, 0, my_packet->z));
	}break;
	case PLAYER_POS:
	{
		player_position *my_packet = reinterpret_cast<player_position *>(ptr);

		if (my_packet->id == myID)
		{
			cout << "ID : " << my_packet->id << ", "
				<< my_packet->x << ", "
				<< my_packet->z << endl;
		}

		if (pObjectManager->FindObject(my_packet->id))
			pObjectManager->FindObject(my_packet->id)->SetPositionAbsolute(new D3DXVECTOR3(my_packet->x, 0, my_packet->z));
		else
			pObjectManager->Insert(my_packet->id, eResourceType::User, gGameFramework.GetDevice(), gGameFramework.GetDeviceContext(),
				D3DXVECTOR3(my_packet->x, 0, my_packet->z));
	}break;
	case BOSS_POS:
	{
		player_position *my_packet = reinterpret_cast<player_position *>(ptr);

		if (pObjectManager->FindObject(my_packet->id))
			pObjectManager->FindObject(my_packet->id)->SetPositionAbsolute(new D3DXVECTOR3(my_packet->x, 0, my_packet->z));
		else
			pObjectManager->Insert(my_packet->id, eResourceType::Monster1, gGameFramework.GetDevice(), gGameFramework.GetDeviceContext(),
				D3DXVECTOR3(my_packet->x, 0, my_packet->z));
	}break;
	case REMOVE_PLAYER:
	{
		remove_player *my_packet = reinterpret_cast<remove_player*>(ptr);
		pObjectManager->DeleteObject(my_packet->id);
	}break;
	case SC_PLAYER_ATTACK:
	{
		player_attack *my_packet = reinterpret_cast<player_attack *>(ptr);
		pObjectManager->FindObject(my_packet->id)->PlayAnimation(static_cast<CObject::eAnimationType>(static_cast<BYTE>(my_packet->attack_type + 2)));
		// attack_type : 1,2,3,4
		// eAnimationType : 3,4,5,6
	}break;
	case SC_BOSS_ATTACK:
	{
		boss_attack *my_packet = reinterpret_cast<boss_attack *>(ptr);
		pObjectManager->FindObject(my_packet->id)->PlayAnimation(static_cast<CObject::eAnimationType>(static_cast<BYTE>(my_packet->attack_type + 2)));
		// attack_type : 1,2,3,4
		// eAnimationType : 3,4,5,6
	}break;
	default:
		printf("Unknown PACKET type [%d]\n", ptr[1]);
	}
}



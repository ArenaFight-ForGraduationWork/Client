#include "stdafx.h"
#include "GameClient.h"
#include "Server.h"
#include "ObjectManager.h"
#include "GameFramework.h"



#define MAX_LOADSTRING 100

// # global variables
HINSTANCE hInst;						// 현재 인스턴스
TCHAR szTitle[MAX_LOADSTRING];			// 제목 표시줄 텍스트
TCHAR szWindowClass[MAX_LOADSTRING];	// 기본 창 클래스 이름

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

		if (pObjectManager->FindObject(my_packet->bossid))
			pObjectManager->FindObject(my_packet->bossid)->SetPositionAbsolute(new D3DXVECTOR3(my_packet->bossx, 0, my_packet->bossy));
		else
			pObjectManager->Insert(my_packet->bossid, eResourceType::Monster1, gGameFramework.GetDevice(), gGameFramework.GetDeviceContext(),
				D3DXVECTOR3(my_packet->bossx, 0, my_packet->bossy));
	}break;
	case PUT_PLAYER:
	{ /* here : 내가 들어오고 나서 들어온 사람 */
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



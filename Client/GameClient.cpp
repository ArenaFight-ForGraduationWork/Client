#include "stdafx.h"
#include "GameClient.h"
#include "Server.h"
#include "ObjectManager.h"
#include "GameFramework.h"



#define MAX_LOADSTRING 100

// # global variables
HINSTANCE hInst;						// current instnace handle
TCHAR szTitle[MAX_LOADSTRING];			// 제목 표시줄 텍스트
TCHAR szWindowClass[MAX_LOADSTRING];	// default window class name

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

	DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_BORDER;
	RECT rc = { 0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT };
	AdjustWindowRect(&rc, dwStyle, FALSE);
	HWND hMainWnd = CreateWindow(szWindowClass, szTitle, dwStyle, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance, NULL);
	if (!hMainWnd) return FALSE;

	gGameFramework.OnCreate(hInstance, hMainWnd);

	// Connect Server
	cout << "ip : ";
	cin >> ip;
	ServerConnect();

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

	switch (message)
	{
	case WM_CREATE:
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
	//case WM_TIMER:
	//	break;
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
	static CObjectManager *pObjectManager = CObjectManager::GetSingleton();
	static CObject *pObject;
	static bool first_time = true;

	switch (ptr[1]) {
	case LOGIN:
	{
		login *my_packet = reinterpret_cast<login *>(ptr);
		if (first_time) {
			printf("my id:%d\n", my_packet->id);
			myID = my_packet->id;
			pObjectManager->Insert((UINT)myID, eResourceType::User, gGameFramework.GetDevice(), gGameFramework.GetDeviceContext(),
				D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0));
			first_time = false;
		}
		else {
			printf("플레이어id:%d\n", myID);
		}
	}break;
	// 로비에 새로운 유저가 들어올때마다 받는 패킷
	case LOBBY_PUT_PLAYER:
	{
		put_player *my_packet = reinterpret_cast<put_player *>(ptr);
		printf("새로운 플레이어 로비 입장 id:%d\n", my_packet->id);

		pObjectManager->Insert(my_packet->id, eResourceType::User, gGameFramework.GetDevice(), gGameFramework.GetDeviceContext(),
			D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0));
	}break;
	case PLAYER_POS:
	{
		player_position *my_packet = reinterpret_cast<player_position *>(ptr);

		pObject = pObjectManager->FindObject(my_packet->id);
		if (pObject)
		{
			// here
			pObject->SetPositionRelative(&D3DXVECTOR3(my_packet->x, 0, my_packet->z));
			pObject->SetDirectionAbsolute(&D3DXVECTOR3(0, my_packet->direction, 0));
		}
	}break;
	case REMOVE_PLAYER:
	{
		break;
	}

	// 방을 제대로 만들었을때 받는 패킷 클라는 자기의 상태를 바꿈
	case CREATE_ROOM_S:
	{
		printf("방만들기에 성공했습니다\n");
		player_status = ROOM;
		break;
	}
	// 방을 제대로 못 만들었을때 받는 패킷 클라는 자기의 상태를 유지함
	case CREATE_ROOM_F:
	{
		printf("방만들기에 실패했습니다\n");
		player_status = LOBBY;
		break;
	}
	// 방제대로만들었을때
	case JOIN_ROOM_S:
	{
		join_room_s *my_packet = reinterpret_cast<join_room_s *>(ptr);

		for (int i = 0; i < 4; ++i) {
			if (my_packet->id[i] != -1) {
				pObjectManager->Insert(my_packet->id[i], eResourceType::User, gGameFramework.GetDevice(), gGameFramework.GetDeviceContext(),
					D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0));
				printf("룸에 있는 플레이어 id:%d\n", my_packet->id[i]);
			}

		}
		printf("방입장에 성공했습니다\n");
		player_status = ROOM;
		break;
	}
	// 방제대로 못 만들었을때
	case JOIN_ROOM_F:
	{
		printf("방입장에 실패했습니다\n");
		player_status = LOBBY;
		break;
	}

	// 게임이 제대로 시작하면 우리 게임이 시작
	// 여기서 게임이 시작
	case GAME_START_S:
	{
		game_start_s *my_packet = reinterpret_cast<game_start_s *>(ptr);

		printf("게임을 시작합니다\n");
		player_status = FIGHT;
		break;
	}
	// 게임 시작 안됐을때
	case GAME_START_F:
	{
		printf("방장만 게임을 시작할 수 있습니다\n");
		break;
	}
	// 채팅
	case CHATTING:
	{
		packet_chat *my_packet = reinterpret_cast<packet_chat *>(ptr);
		printf("플레이어id:%d님의 채팅:%s\n", my_packet->id, my_packet->message);
		break;
	}
	// 방에 새로운 유저가 왔을 때
	case ROOM_PUT_PLAYER:
	{
		// 룸에 새로운애 온거 알려주는건데 패킷 구조체 또 만들기그래서 이거씀
		join_room_f *my_packet = reinterpret_cast<join_room_f *>(ptr);
		for (int i = 0; i < 4; ++i) {
			pObject = pObjectManager->FindObject(my_packet->id);
			if (!pObject)
			{	// 해당 id가 존재하지 않으면
				pObjectManager->Insert(my_packet->id, eResourceType::User, gGameFramework.GetDevice(), gGameFramework.GetDeviceContext(),
					D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0));
			}
			else
			{ //  해당 id가 존재하면
				pObject->SetPositionAbsolute(&D3DXVECTOR3(0, 0, 0));
				pObject->SetDirectionAbsolute(&D3DXVECTOR3(0, 0, 0));
			}
			printf("플레이어id:%d님이 방에 오셨습니다\n", my_packet->id);
		}
		break;
	}

	// 방에서 나갈때
	case OUT_ROOM:
	{
		out_room *my_packet = reinterpret_cast<out_room *>(ptr);

		if (pObjectManager->FindObject(my_packet->id))
		{ //  해당 id가 존재하면
			pObjectManager->DeleteObject(my_packet->id);
		}
		printf("플레이어id:%d가 방을 떠났습니다\n", my_packet->id);
		printf("현재 방장:%d\n", my_packet->room_master);
		break;
	}
	// 아직 안해도 됨
	case BOSS_POS:
	{
		player_position *my_packet = reinterpret_cast<player_position *>(ptr);
		pObject = pObjectManager->FindObject(my_packet->id);
		if (pObject)
		{
			pObject->SetPositionAbsolute(&D3DXVECTOR3(my_packet->x, 0, my_packet->z));
			pObject->SetDirectionAbsolute(&D3DXVECTOR3(0, my_packet->direction, 0));
		}
		else
		{
			pObjectManager->Insert(my_packet->id, eResourceType::Monster1, gGameFramework.GetDevice(), gGameFramework.GetDeviceContext(),
				D3DXVECTOR3(my_packet->x, 0, my_packet->z), D3DXVECTOR3(0, my_packet->direction, 0));
		}
	}		break;
	// 게임 끝내고 유저 상태 변경
	case GAME_EXIT_S:
	{
		game_start_s *my_packet = reinterpret_cast<game_start_s *>(ptr);
		for (int i = 0; i < 4; ++i) {
			if (pObjectManager->FindObject(my_packet->id[i]))
			{ //  해당 id가 존재하면
				pObjectManager->DeleteObject(my_packet->id[i]);
			}
		}
		printf("게임을 끝납니다\n");
		player_status = ROOM;
	} break;
	default:
		printf("Unknown PACKET type [%d]\n", ptr[1]);



	//case LOGIN:
	//{	
	//	login *my_packet = reinterpret_cast<login *>(ptr);

	//	// 내가 접속
	//	if (first_login)
	//	{
	//		myID = my_packet->id;
	//		/* here : 플레이어 좌표는 어디로 가져와야 하는가 > 안 주니까 000으로 설정 */
	//		pObjectManager->Insert((UINT)myID, eResourceType::User, gGameFramework.GetDevice(), gGameFramework.GetDeviceContext(),
	//			D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, my_packet->direction, 0));

	//		// 보스
	//		pObject = pObjectManager->FindObject(my_packet->bossid);
	//		if (pObject)
	//		{
	//			pObject->SetPositionAbsolute(new D3DXVECTOR3(my_packet->bossx, 0, my_packet->bossy));
	//			pObject->SetDirectionAbsolute(new D3DXVECTOR3(0, my_packet->bossdirection, 0));
	//		}
	//		else
	//		{
	//			pObject = pObjectManager->Insert(my_packet->bossid, eResourceType::Monster1, gGameFramework.GetDevice(), gGameFramework.GetDeviceContext(),
	//				D3DXVECTOR3(my_packet->bossx, 0, my_packet->bossy));
	//			pObject->SetPositionAbsolute(new D3DXVECTOR3(my_packet->bossx, 0, my_packet->bossy));
	//			pObject->SetDirectionAbsolute(new D3DXVECTOR3(0, my_packet->bossdirection, 0));
	//		}

	//		first_login = false;
	//	}

	//	// 다른 사람이 접속. 기존/신규
	//	if (my_packet->id != myID) {
	//		pObjectManager->Insert(my_packet->id, eResourceType::User, gGameFramework.GetDevice(), gGameFramework.GetDeviceContext(),
	//			D3DXVECTOR3(my_packet->x, 0, my_packet->z), D3DXVECTOR3(0, my_packet->direction, 0));
	//	}
	//}break;
	//case PUT_PLAYER:
	//{ /* 필요가 없는듯 하다 */
	//	player_position *my_packet = reinterpret_cast<player_position *>(ptr);

	//	pObjectManager->Insert(my_packet->id, eResourceType::User, gGameFramework.GetDevice(), gGameFramework.GetDeviceContext(),
	//		D3DXVECTOR3(my_packet->x, 0, my_packet->z));
	//}break;
	//case PLAYER_POS:
	//{
	//	player_position *my_packet = reinterpret_cast<player_position *>(ptr);

	//	pObject = pObjectManager->FindObject(my_packet->id);
	//	if (pObject)
	//	{
	//		pObject->SetDirectionAbsolute(new D3DXVECTOR3(0, my_packet->direction, 0));
	//		pObject->MoveForward(my_packet->distance);
	//	}
	//	else
	//		pObject = pObjectManager->Insert(my_packet->id, eResourceType::User, gGameFramework.GetDevice(), gGameFramework.GetDeviceContext(),
	//			D3DXVECTOR3(my_packet->x, 0, my_packet->z));
	//	pObject->PlayAnimation(CObject::eAnimationType::Move);
	//}break;
	//case BOSS_POS:
	//{
	//	player_position *my_packet = reinterpret_cast<player_position *>(ptr);

	//	pObject = pObjectManager->FindObject(my_packet->id);
	//	if (pObject)
	//	{
	//		pObject->SetDirectionAbsolute(new D3DXVECTOR3(0, my_packet->direction, 0));
	//		pObject->MoveForward(my_packet->distance);
	//	}
	//	else
	//		pObject = pObjectManager->Insert(my_packet->id, eResourceType::Monster1, gGameFramework.GetDevice(), gGameFramework.GetDeviceContext(),
	//			D3DXVECTOR3(my_packet->x, 0, my_packet->z));
	//	pObject->PlayAnimation(CObject::eAnimationType::Move);
	//}break;
	//case REMOVE_PLAYER:
	//{
	//	remove_player *my_packet = reinterpret_cast<remove_player*>(ptr);
	//	pObjectManager->DeleteObject(my_packet->id);
	//}break;
	//case SC_PLAYER_ATTACK_S:
	//{
	//	player_attack *my_packet = reinterpret_cast<player_attack *>(ptr);
	//	pObjectManager->FindObject(my_packet->id)->PlayAnimation(static_cast<CObject::eAnimationType>(static_cast<BYTE>(my_packet->attack_type + 2)));
	//	// attack_type : 1,2,3,4
	//	// eAnimationType : 3,4,5,6
	//}break;
	//case SC_BOSS_ATTACK:
	//{
	//	boss_attack *my_packet = reinterpret_cast<boss_attack *>(ptr);
	//	pObjectManager->FindObject(my_packet->id)->PlayAnimation(static_cast<CObject::eAnimationType>(static_cast<BYTE>(my_packet->attack_type + 2)));
	//	// attack_type : 1,2,3,4
	//	// eAnimationType : 3,4,5,6
	//}break;
	//case SC_PLAYER_MOV_END:
	//{
	//	packet_player_move_end *my_packet = reinterpret_cast<packet_player_move_end *>(ptr);

	//	pObject = pObjectManager->FindObject(my_packet->id);
	//	if (pObject)
	//		pObject->PlayAnimation(CObject::eAnimationType::Idle);
	//}break;
	//case BOSS_DEAD:
	//{
	//	boss_dead *my_packet = reinterpret_cast<boss_dead *>(ptr);
	//	pObjectManager->FindObject(my_packet->id)->PlayAnimation(CObject::eAnimationType::Dead);
	//}break;
	//default:
	//	cout << "Unknown PACKET type [" << ptr[1] << "]" << endl;
	//	break;
	}
}



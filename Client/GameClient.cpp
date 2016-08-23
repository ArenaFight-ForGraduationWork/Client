#include "stdafx.h"
#include "GameClient.h"
#include "Server.h"
#include "ObjectManager.h"
#include "GameFramework.h"



#define MAX_LOADSTRING 100

// # global variables
HINSTANCE hInst;						// current instnace handle
TCHAR szTitle[MAX_LOADSTRING];			// ���� ǥ���� �ؽ�Ʈ
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
			printf("�÷��̾�id:%d\n", myID);
		}
	}break;
	// �κ� ���ο� ������ ���ö����� �޴� ��Ŷ
	case LOBBY_PUT_PLAYER:
	{
		put_player *my_packet = reinterpret_cast<put_player *>(ptr);
		printf("���ο� �÷��̾� �κ� ���� id:%d\n", my_packet->id);

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

	// ���� ����� ��������� �޴� ��Ŷ Ŭ��� �ڱ��� ���¸� �ٲ�
	case CREATE_ROOM_S:
	{
		printf("�游��⿡ �����߽��ϴ�\n");
		player_status = ROOM;
		break;
	}
	// ���� ����� �� ��������� �޴� ��Ŷ Ŭ��� �ڱ��� ���¸� ������
	case CREATE_ROOM_F:
	{
		printf("�游��⿡ �����߽��ϴ�\n");
		player_status = LOBBY;
		break;
	}
	// ������θ��������
	case JOIN_ROOM_S:
	{
		join_room_s *my_packet = reinterpret_cast<join_room_s *>(ptr);

		for (int i = 0; i < 4; ++i) {
			if (my_packet->id[i] != -1) {
				pObjectManager->Insert(my_packet->id[i], eResourceType::User, gGameFramework.GetDevice(), gGameFramework.GetDeviceContext(),
					D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0));
				printf("�뿡 �ִ� �÷��̾� id:%d\n", my_packet->id[i]);
			}

		}
		printf("�����忡 �����߽��ϴ�\n");
		player_status = ROOM;
		break;
	}
	// ������� �� ���������
	case JOIN_ROOM_F:
	{
		printf("�����忡 �����߽��ϴ�\n");
		player_status = LOBBY;
		break;
	}

	// ������ ����� �����ϸ� �츮 ������ ����
	// ���⼭ ������ ����
	case GAME_START_S:
	{
		game_start_s *my_packet = reinterpret_cast<game_start_s *>(ptr);

		printf("������ �����մϴ�\n");
		player_status = FIGHT;
		break;
	}
	// ���� ���� �ȵ�����
	case GAME_START_F:
	{
		printf("���常 ������ ������ �� �ֽ��ϴ�\n");
		break;
	}
	// ä��
	case CHATTING:
	{
		packet_chat *my_packet = reinterpret_cast<packet_chat *>(ptr);
		printf("�÷��̾�id:%d���� ä��:%s\n", my_packet->id, my_packet->message);
		break;
	}
	// �濡 ���ο� ������ ���� ��
	case ROOM_PUT_PLAYER:
	{
		// �뿡 ���ο�� �°� �˷��ִ°ǵ� ��Ŷ ����ü �� �����׷��� �̰ž�
		join_room_f *my_packet = reinterpret_cast<join_room_f *>(ptr);
		for (int i = 0; i < 4; ++i) {
			pObject = pObjectManager->FindObject(my_packet->id);
			if (!pObject)
			{	// �ش� id�� �������� ������
				pObjectManager->Insert(my_packet->id, eResourceType::User, gGameFramework.GetDevice(), gGameFramework.GetDeviceContext(),
					D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0));
			}
			else
			{ //  �ش� id�� �����ϸ�
				pObject->SetPositionAbsolute(&D3DXVECTOR3(0, 0, 0));
				pObject->SetDirectionAbsolute(&D3DXVECTOR3(0, 0, 0));
			}
			printf("�÷��̾�id:%d���� �濡 ���̽��ϴ�\n", my_packet->id);
		}
		break;
	}

	// �濡�� ������
	case OUT_ROOM:
	{
		out_room *my_packet = reinterpret_cast<out_room *>(ptr);

		if (pObjectManager->FindObject(my_packet->id))
		{ //  �ش� id�� �����ϸ�
			pObjectManager->DeleteObject(my_packet->id);
		}
		printf("�÷��̾�id:%d�� ���� �������ϴ�\n", my_packet->id);
		printf("���� ����:%d\n", my_packet->room_master);
		break;
	}
	// ���� ���ص� ��
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
	// ���� ������ ���� ���� ����
	case GAME_EXIT_S:
	{
		game_start_s *my_packet = reinterpret_cast<game_start_s *>(ptr);
		for (int i = 0; i < 4; ++i) {
			if (pObjectManager->FindObject(my_packet->id[i]))
			{ //  �ش� id�� �����ϸ�
				pObjectManager->DeleteObject(my_packet->id[i]);
			}
		}
		printf("������ �����ϴ�\n");
		player_status = ROOM;
	} break;
	default:
		printf("Unknown PACKET type [%d]\n", ptr[1]);



	//case LOGIN:
	//{	
	//	login *my_packet = reinterpret_cast<login *>(ptr);

	//	// ���� ����
	//	if (first_login)
	//	{
	//		myID = my_packet->id;
	//		/* here : �÷��̾� ��ǥ�� ���� �����;� �ϴ°� > �� �ִϱ� 000���� ���� */
	//		pObjectManager->Insert((UINT)myID, eResourceType::User, gGameFramework.GetDevice(), gGameFramework.GetDeviceContext(),
	//			D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, my_packet->direction, 0));

	//		// ����
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

	//	// �ٸ� ����� ����. ����/�ű�
	//	if (my_packet->id != myID) {
	//		pObjectManager->Insert(my_packet->id, eResourceType::User, gGameFramework.GetDevice(), gGameFramework.GetDeviceContext(),
	//			D3DXVECTOR3(my_packet->x, 0, my_packet->z), D3DXVECTOR3(0, my_packet->direction, 0));
	//	}
	//}break;
	//case PUT_PLAYER:
	//{ /* �ʿ䰡 ���µ� �ϴ� */
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



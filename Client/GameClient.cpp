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
	static CObject *pObject = nullptr;

	switch (ptr[1])
	{
		// ==========================================================================
		// LOBBY
		// ==========================================================================
	case LOGIN:
	{
		login *my_packet = reinterpret_cast<login *>(ptr);

		if (first_login) {
			printf("my id:%d\n", my_packet->id);
			myID = my_packet->id;
			first_login = false;
		}
		else {
			printf("�÷��̾�id:%d\n", myID);
		}
	}break;
	case LOBBY_PUT_PLAYER:
	{	/* �κ� ���� ������ �� �� ������ ������ �޴� ��Ŷ */
		put_player *my_packet = reinterpret_cast<put_player *>(ptr);

		printf("���ο� �÷��̾� �κ� ���� id:%d\n", my_packet->id);
	}break;
	case CREATE_ROOM_S:
	{	/* �� ���� ������ �޴� ��Ŷ. ���� ����(player_status)�� LOBBY > ROOM ���� ���� */
		printf("�游��⿡ �����߽��ϴ�\n");
		player_status = ROOM;
	}break;
	case CREATE_ROOM_F:
	{	/* �� ���� ���н� �޴� ��Ŷ. ���� ����(player_status)�� ����(LOBBY) */
		printf("�游��⿡ �����߽��ϴ�\n");
		player_status = LOBBY;
	}break;
	case JOIN_ROOM_S:
	{	/* �� ���� ������ �޴� ��Ŷ.
		1) ���� �濡 �� ���� �� �̹� ���ִ� ��� ������ �޴´� == ���� ����
		2) �� ���Ŀ� ������ ����� ROOM_PUT_PLAYER ��Ŷ���� �´�
		���� ����(player_status)�� LOBBY > ROOM ���� ����	*/
		join_room_s *my_packet = reinterpret_cast<join_room_s *>(ptr);
		for (unsigned int i = 0; i < 4; ++i)
		{
			if (my_packet->id[i] != -1) {
				if (myID != my_packet->id[i])	/* �濡 �̹� �����ִ� ��� ���� �߿� myID�� �����̹Ƿ� �����Ѵ� */
					pObjectManager->Insert(my_packet->id[i], eResourceType::User, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), true);
				printf("�뿡 �ִ� �÷��̾� id:%d\n", my_packet->id[i]);
			}
		}
		printf("�����忡 �����߽��ϴ�\n");
		player_status = ROOM;
	}break;
	case JOIN_ROOM_F:
	{	/* �� ���� ���н� �޴� ��Ŷ. ���� ����(player_status)�� ����(LOBBY)	*/
		printf("�����忡 �����߽��ϴ�\n");
		player_status = LOBBY;
	}break;
	case ROOM_PUT_PLAYER:
	{	/* ���� ������ �濡 ���� ������ �ڿ� ���� ��� ������ �޴� ��Ŷ.
		���� �濡 ���� �� �̹� �ִ� ��� ������ JOIN_ROOM_S ��Ŷ���� �޾ƿ´�.	*/
		join_room_f *my_packet = reinterpret_cast<join_room_f *>(ptr);

		pObject = pObjectManager->FindObject(my_packet->id);
		if (!pObject)
		{	// �ش� id�� �������� ������
			pObjectManager->Insert(my_packet->id, eResourceType::User, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), true);
		}
		else
		{ //  �ش� id�� �����ϸ�
			pObject->SetPositionAbsolute(&D3DXVECTOR3(0, 0, 0));
			pObject->SetDirectionAbsolute(&D3DXVECTOR3(0, 0, 0));
		}
		printf("�÷��̾�id:%d���� �濡 ���̽��ϴ�\n", my_packet->id);
		pObject = nullptr;
	}break;
	case CHATTING:
	{	/* �ٸ� �̰� �Է��� ä���� �޾ƿ��� ��Ŷ */
		packet_chat *my_packet = reinterpret_cast<packet_chat *>(ptr);

		printf("�÷��̾�id:%d���� ä��: %s\n", my_packet->id, my_packet->message);
	}break;


	// ==========================================================================
	// change player data
	// ==========================================================================
	case PLAYER_POS:
	{
		player_position *my_packet = reinterpret_cast<player_position *>(ptr);

		cout << my_packet->id << endl;
		pObject = pObjectManager->FindObject(static_cast<UINT>(my_packet->id));
		if (pObject)
		{
			cout << "myID : " << myID << ", ";
			cout << "packet_id : " << my_packet->id << my_packet->x << ", " << my_packet->z << endl;
			pObject->SetPositionAbsolute(&D3DXVECTOR3(my_packet->x, 0, my_packet->z));
			pObject->SetDirectionAbsolute(&D3DXVECTOR3(0, my_packet->direction, 0));
			if (my_packet->isMoving)
				pObject->PlayAnimation(CObject::eAnimationType::Move);
			else
				pObject->PlayAnimation(CObject::eAnimationType::Idle);
		}
		pObject = nullptr;
	}break;
	case REMOVE_PLAYER:
	{	/* Ŭ���̾�Ʈ�� ������ ���. �濡 �������� �� �ȴ�. */
	}break;
	// ����
	//case BOSS_POS:
	//{
	//	player_position *my_packet = reinterpret_cast<player_position *>(ptr);
	//}break;



	case GAME_START_S:
	{	/* ���� ������ �˸��� ��Ŷ. �� ��Ŷ�� ������ ������ ���۵ȴ�
		���� ����(player_status)�� ROOM > FIGHT �� �����Ѵ�
		*/
		game_start_s *my_packet = reinterpret_cast<game_start_s *>(ptr);

		printf("������ �����մϴ�\n");
		player_status = FIGHT;

		pObjectManager->Insert((UINT)myID, eResourceType::User, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), true);

		printf("���� ID:%d\n", my_packet->bossid);

		pObject = pObjectManager->FindObject(my_packet->bossid);
		if (pObject)
		{	// if boss already exists
			pObject->SetPositionAbsolute(&D3DXVECTOR3(static_cast<float>(my_packet->bossx), 0.0f, static_cast<float>(my_packet->bossz)));
			pObject->SetDirectionAbsolute(&D3DXVECTOR3(0.0f, static_cast<float>(my_packet->bossdis), 0.0f));
		}
		else
		{	// if boss doesn't exist
			pObjectManager->Insert(my_packet->bossid, eResourceType::Monster1, 
				D3DXVECTOR3(static_cast<float>(my_packet->bossx), 0.0f, static_cast<float>(my_packet->bossz)), 
				D3DXVECTOR3(0.0f, static_cast<float>(my_packet->bossdis), 0.0f), true);
		}
		pObject = nullptr;
	}break;
	case GAME_START_F:
	{	/* ���� ���� ���н� �޴� ��Ŷ. */
		printf("���常 ������ ������ �� �ֽ��ϴ�\n");
	}break;
	case OUT_ROOM:
	{	/* ROOM���¿��� Ư�� id�� ���� ������ �޴� ��Ŷ. �� ���¿��� ���� ����� �ٽ� �濡 ���� �� �ִ�.
			FIGHT���¿����� ����ó���� ���� */
		out_room *my_packet = reinterpret_cast<out_room *>(ptr);

		if (pObjectManager->FindObject(my_packet->id))
		{ //  �ش� id�� �����ϸ�
			pObjectManager->DeleteObject(my_packet->id);
		}
		printf("�÷��̾�id:%d�� ���� �������ϴ�\n", my_packet->id);
		printf("���� ����:%d\n", my_packet->room_master);
	}break;
	case GAME_EXIT_S:
	{	/* ���� ���� �� ���� ����. ���� �����ߴ� �̵� ������ �����, ���� ������
		���� ����(player_status)�� FIGHT > ROOM���� ������ */

		// herehere. �κ�� ���ư����ϴ� �� �ƴѰ�?
		// �� �ڵ�� ������ �������� �� �ȿ� ���������� �׸����� �ʴ� ������
		// �������� �׷��� �ĵ� �ƴ� ����� �� �濡 ���������� �ʾҴµ� ROOM �����ΰ�
		game_start_s *my_packet = reinterpret_cast<game_start_s *>(ptr);

		for (unsigned int i = 0; i < 4; ++i)
		{
			if (pObjectManager->FindObject(my_packet->id[i]))
			{ //  �ش� id�� �����ϸ�
				pObjectManager->DeleteObject(my_packet->id[i]);
			}
		}
		printf("������ �����ϴ�\n");
		player_status = ROOM;
	}break;



	default:	printf("Unknown PACKET type [%d]\n", ptr[1]); break;
	}
}



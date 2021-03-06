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
			partyIDs[0] = myID;
			partyNum += 1;
			first_login = false;
		}
		else {
			printf("플레이어id:%d\n", myID);
		}
	}break;
	case LOBBY_PUT_PLAYER:
	{	/* 로비에 내가 접속한 뒤 새 유저가 들어오면 받는 패킷 */
		put_player *my_packet = reinterpret_cast<put_player *>(ptr);

		printf("새로운 플레이어 로비 입장 id:%d\n", my_packet->id);
	}break;
	case CREATE_ROOM_S:
	{	/* 방 생성 성공시 받는 패킷. 현재 상태(player_status)를 LOBBY > ROOM 으로 변경 */
		printf("방만들기에 성공했습니다\n");
		player_status = static_cast<BYTE>(ePlayer_State::eROOM);
		CSceneManager::GetSingleton()->GetNowScene()->ChangeState();
	}break;
	case CREATE_ROOM_F:
	{	/* 방 생성 실패시 받는 패킷. 현재 상태(player_status)를 유지(LOBBY) */
		printf("방만들기에 실패했습니다\n");
		//player_status = LOBBY;
		CSceneManager::GetSingleton()->GetNowScene()->ChangeState();
	}break;
	case JOIN_ROOM_S:
	{	/* 방 접속 성공시 받는 패킷.
		1) 내가 방에 들어가 있을 때 이미 와있는 사람 정보를 받는다 == 나도 포함
		2) 나 이후에 들어오는 사람은 ROOM_PUT_PLAYER 패킷으로 온다
		현재 상태(player_status)를 LOBBY > ROOM 으로 변경	*/
		join_room_s *my_packet = reinterpret_cast<join_room_s *>(ptr);
		for (unsigned int i = 0; i < 4; ++i)
		{
			if (my_packet->id[i] != -1) {
				if (myID != my_packet->id[i])	/* 방에 이미 들어와있는 사람 정보 중에 myID도 포함이므로 제외한다 */
				{
					pObjectManager->Insert(my_packet->id[i], eResourceType::User, XMVectorZero(), XMVectorZero(), true);
					for (unsigned int id = 0; id < 4; ++id)
					{
						if (-1 == partyIDs[id])
						{
							partyIDs[id] = my_packet->id[i];
							++partyNum;
							break;
						}
					}
				}
				printf("룸에 있는 플레이어 id:%d\n", my_packet->id[i]);
			}
		}
		printf("방입장에 성공했습니다\n");
		player_status = static_cast<BYTE>(ePlayer_State::eROOM);
	}break;
	case JOIN_ROOM_F:
	{	/* 방 입장 실패시 받는 패킷. 현재 상태(player_status)를 유지(LOBBY)	*/
		printf("방입장에 실패했습니다\n");
		player_status = static_cast<BYTE>(ePlayer_State::eLOBBY);
	}break;
	case ROOM_PUT_PLAYER:
	{	/* 내가 접속한 방에 내가 접속한 뒤에 들어온 사람 정보를 받는 패킷.
		내가 방에 들어갔을 때 이미 있는 사람 정보는 JOIN_ROOM_S 패킷에서 받아온다.	*/
		join_room_f *my_packet = reinterpret_cast<join_room_f *>(ptr);

		pObject = pObjectManager->FindObject(my_packet->id);
		if (!pObject)
		{	// 해당 id가 존재하지 않으면
			pObjectManager->Insert(my_packet->id, eResourceType::User, XMVectorZero(), XMVectorZero(), true);
			for (unsigned int id = 0; id < 4; ++id)
			{
				if (-1 == partyIDs[id])
				{
					partyIDs[id] = my_packet->id;
					++partyNum;
					break;
				}
			}
		}
		else
		{ //  해당 id가 존재하면
			XMFLOAT3 zero(0, 0, 0);
			pObject->SetPositionAbsolute(XMLoadFloat3(&zero));
			pObject->SetDirectionAbsolute(XMLoadFloat3(&zero));
		}
		printf("플레이어id:%d님이 방에 오셨습니다\n", my_packet->id);
		pObject = nullptr;
	}break;
	case CHATTING:
	{	/* 다른 이가 입력한 채팅을 받아오는 패킷 */
		packet_chat *my_packet = reinterpret_cast<packet_chat *>(ptr);

		printf("플레이어id:%d님의 채팅: %s\n", my_packet->id, my_packet->message);
	}break;


	// ==========================================================================
	// change player data
	// ==========================================================================
	case PLAYER_POS:
	{
		player_position *my_packet = reinterpret_cast<player_position *>(ptr);

		pObject = pObjectManager->FindObject(static_cast<UINT>(my_packet->id));
		if (pObject)
		{
			XMFLOAT3 position(my_packet->x, 0, my_packet->z);
			XMFLOAT3 direction(0, my_packet->direction, 0);

			pObject->SetPositionAbsolute(XMLoadFloat3(&position));
			pObject->SetDirectionAbsolute(XMLoadFloat3(&direction));
			if (my_packet->isMoving)
				pObject->PlayAnimation(CObject::eAnimationType::Move);
			else
				pObject->PlayAnimation(CObject::eAnimationType::Idle);
		}
		pObject = nullptr;
	} break;
	case SC_PLAYER_ATTACK:
	{
		player_attack *my_packet = reinterpret_cast<player_attack *>(ptr);

		cout << "receive attack packet : " << my_packet->id << ", " << static_cast<UINT>(my_packet->attack_type) << endl;

		pObject = pObjectManager->FindObject(static_cast<UINT>(my_packet->id));
		if (pObject)
		{
			switch (my_packet->attack_type)
			{
			case NORMAL_ATTACK:
				pObject->PlayAnimation(CObject::eAnimationType::Attack);
				break;
			case PLAYER_SKILL1:
				pObject->PlayAnimation(CObject::eAnimationType::Skill1);
				break;
			case PLAYER_SKILL2:
				pObject->PlayAnimation(CObject::eAnimationType::Skill2);
				break;
			case PLAYER_SKILL3:
				pObject->PlayAnimation(CObject::eAnimationType::Skill3);
				break;
			}
		}
		pObject = nullptr;
	} break;
	case REMOVE_PLAYER:
	{	/* 클라이언트를 종료한 사람. 방에 재접속이 안 된다. */
	} break;
	case BOSS_POS:
	{
		player_position *my_packet = reinterpret_cast<player_position *>(ptr);

		cout << "보스 움직임" << endl;
		cout << "x:" << my_packet->x << ", " << "y:" << my_packet->z << endl;
		pObject = pObjectManager->FindObject(static_cast<UINT>(my_packet->id));
		if (pObject)
		{
			XMFLOAT3 position(my_packet->x, 0, my_packet->z);
			XMFLOAT3 direction(0, my_packet->direction, 0);

			pObject->SetPositionAbsolute(XMLoadFloat3(&position));
			pObject->SetDirectionAbsolute(XMLoadFloat3(&direction));
			if (my_packet->isMoving)
				pObject->PlayAnimation(CObject::eAnimationType::Move);
			else
				pObject->PlayAnimation(CObject::eAnimationType::Idle);
		}
		pObject = nullptr;
	} break;
	case SC_BOSS_ATTACK:
	{
		player_attack *my_packet = reinterpret_cast<player_attack *>(ptr);

		//cout << "receive attack packet : " << my_packet->id << ", " << static_cast<UINT>(my_packet->attack_type) << endl;

		pObject = pObjectManager->FindObject(static_cast<UINT>(my_packet->id));
		if (pObject)
		{
			switch (my_packet->attack_type)
			{
			case BOSS_NORMAL_ATTACK:
				pObject->PlayAnimation(CObject::eAnimationType::Attack);
				break;
			case BOSS_SKILL1:
				pObject->PlayAnimation(CObject::eAnimationType::Skill1);
				break;
			case BOSS_SKILL2:
				pObject->PlayAnimation(CObject::eAnimationType::Skill2);
				break;
			case BOSS_SKILL3:
				pObject->PlayAnimation(CObject::eAnimationType::Skill3);
				break;
			}
		}
		pObject = nullptr;
	} break;
	case BOSS_DEAD:
	{
		cout << "보스 사망" << endl;
	}
	case STATUS_CHANGE:
	{
		status_change *my_packet = reinterpret_cast<status_change *>(ptr);

		printf("플레이어id:%d 스텟 변경\n", my_packet->id);
		printf("현재 hp:%d\n", my_packet->hp);
		pObject = pObjectManager->FindObject(static_cast<UINT>(my_packet->id));
		if (pObject)
		{
			float hp = static_cast<float>(my_packet->hp);
			pObject->GetComponent()->SetHealthPoint(hp);
		}
	} break;



	case GAME_START_S:
	{	/* 전투 시작을 알리는 패킷. 이 패킷을 받으면 전투가 시작된다
		현재 상태(player_status)를 ROOM > FIGHT 로 변경한다
		*/
		game_start_s *my_packet = reinterpret_cast<game_start_s *>(ptr);

		printf("게임을 시작합니다\n");
		player_status = static_cast<BYTE>(ePlayer_State::eFight);
		CSceneManager::GetSingleton()->Change(CSceneManager::eSceneType::FIRST);

		monsterID = my_packet->bossid;
		pObjectManager->Insert((UINT)myID, eResourceType::User, XMVectorZero(), XMVectorZero(), true);

		printf("보스 ID:%d\n", my_packet->bossid);

		pObject = pObjectManager->FindObject(my_packet->bossid);
		if (pObject)
		{	// if boss already exists
			XMFLOAT3 position(static_cast<float>(my_packet->bossx), 0.0f, static_cast<float>(my_packet->bossz));
			XMFLOAT3 direction(0.0f, static_cast<float>(my_packet->bossdis), 0.0f);

			pObject->SetPositionAbsolute(XMLoadFloat3(&position));
			pObject->SetDirectionAbsolute(XMLoadFloat3(&direction));
		}
		else
		{	// if boss doesn't exist
			XMFLOAT3 pos(static_cast<float>(my_packet->bossx), 0.0f, static_cast<float>(my_packet->bossz));
			XMFLOAT3 dir(0.0f, static_cast<float>(my_packet->bossdis), 0.0f);
			pObjectManager->Insert(my_packet->bossid, eResourceType::Monster1, XMLoadFloat3(&pos), XMLoadFloat3(&dir), true);
		}
		pObject = nullptr;
	}break;
	case GAME_START_F:
	{	/* 전투 시작 실패시 받는 패킷. */
		printf("방장만 게임을 시작할 수 있습니다\n");
	}break;
	case OUT_ROOM:
	{	/* ROOM상태에서 특정 id가 방을 나가면 받는 패킷. 이 상태에서 나간 사람은 다시 방에 들어올 수 있다.
			FIGHT상태에서의 예외처리가 아직 */
		out_room *my_packet = reinterpret_cast<out_room *>(ptr);

		if (pObjectManager->FindObject(my_packet->id))
		{ //  해당 id가 존재하면
			pObjectManager->DeleteObject(my_packet->id);
			for (unsigned int id = 0; id < 4; ++id)
			{
				if (my_packet->id == partyIDs[id])
				{
					partyIDs[id] = -1;
					--partyNum;
					break;
				}
			}
		}
		printf("플레이어id:%d가 방을 떠났습니다\n", my_packet->id);
		printf("현재 방장:%d\n", my_packet->room_master);
	}break;
	case GAME_EXIT_S:
	{	/* 전투 종료 후 상태 변경. 같이 게임했던 이들 정보를 지우고, 방을 나간다
		현재 상태(player_status)를 FIGHT > ROOM으로 돌린다 */

		// herehere. 로비로 돌아가야하는 거 아닌가?
		// 이 코드는 여전히 팀원들이 방 안에 남아있지만 그리지만 않는 상태임
		// 방주인은 그렇다 쳐도 아닌 사람은 왜 방에 접속하지도 않았는데 ROOM 상태인가
		game_start_s *my_packet = reinterpret_cast<game_start_s *>(ptr);

		for (unsigned int i = 0; i < 4; ++i)
		{
			if (pObjectManager->FindObject(my_packet->id[i]))
			{ //  해당 id가 존재하면
				pObjectManager->DeleteObject(my_packet->id[i]);
				for (unsigned int id = 0; id < 4; ++id)
				{
					if (my_packet->id[i] == partyIDs[id])
					{
						partyIDs[id] = -1;
						--partyNum;
						break;
					}
				}
			}
		}
		printf("게임을 끝납니다\n");
		player_status = static_cast<BYTE>(ePlayer_State::eROOM);
	}break;



	default:	printf("Unknown PACKET type [%d]\n", ptr[1]); break;
	}
}



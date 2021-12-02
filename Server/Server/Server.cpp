//#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#define _CRT_SECURE_NO_WRANINGS
#include "stdafx.h"
#include "TileManager.h"
#include "Obj.h"
#include "ObjManager.h"
#include "Mbape.h"
#include "Messi.h"

#define BUFSIZE 500
static int iClientID = 0;				// 클라이언트의 ID
map<int, CLIENTINFO> WorldInfo;			// 클라이언트로 보낼 패킷
map<USHORT, int> mapClientPort;			// 클라이언트의 포트번호와 클라이언트ID 저장
map<int, bool> mapIsRecv;				// 클라이언트에서 데이터를 전송받았는지 판단하기 위한 맵
map<int, bool> mapIsCollision;			// 버프 판단을 위한 충돌 확인 맵

HANDLE hRecvEvent;		// 각 클라이언트와의 수신 결과를 알려주기 위한 이벤트
HANDLE hSendEvent;		// 각 클라이언트와의 송신 결과를 알려주기 위한 이벤트
vector<CObj*>	vecMapTile;				// 맵 타일
vector<USHORT> vecIsFirstConnect;		// 클라이언트가 접속하면 클라이언트의 포트번호를 저장함 (처음 접속인지 확인용)
vector<MONSTERINFO>	vecMonster;

bool isStart = false;

#define SERVERPORT 9000

void Receive_Data(LPVOID arg, map<int, ClientInfo> _worldInfo);
void Send_Data(LPVOID arg);
void Send_InitMap(LPVOID arg);

void Init_Monster(LPVOID arg);

// 소켓 함수 오류 출력 후 종료
void err_quit(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL
	);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// 소켓 함수 오류 출력
void err_display(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL
	);
	printf("[%s] %s", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

// 사용자 정의 데이터 수신 함수
int recvn(SOCKET s, char* buf, int len, int flags)
{
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}

// 클라이언트와 데이터 통신
DWORD WINAPI ProcessClient(LPVOID arg)
{
	SOCKET client_sock = (SOCKET)arg;
	int retval;
	SOCKADDR_IN clientaddr;
	int addrlen;
	char buf[BUFSIZE + 1];

	// 클라이언트 정보 얻기
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);

	// 현재 클라이언트가 처음 접속했는지 확인
	//auto iter = find(mapClientPort.begin(), mapClientPort.end(), clientaddr.sin_port);
	auto iter = mapClientPort.find(clientaddr.sin_port);
	if (iter == mapClientPort.end())
	{
		// 처음 접속이면 포트 번호를 저장
		mapClientPort.insert({ clientaddr.sin_port, iClientID });
		// ClientID 보내기 (후에 처음 들어왔을때만 보내게 처리)
		retval = send(client_sock, (char*)&iClientID, sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
		}

		// 맵 정보 전송 -> 클라이언트는 이 정보를 바탕으로 맵 초기화
		Send_InitMap((LPVOID)client_sock);

		// 몬스터 정보 전송
		Init_Monster((LPVOID)client_sock);

		//Receive_Data((LPVOID)client_sock, WorldInfo);

		// 캐릭터 종류, 초기 위치 정해서 Client로 전송
		//Send_Data((LPVOID)client_sock);

		printf("포트 번호=%d 에게 ClientID: %d 전송 성공\n", ntohs(clientaddr.sin_port), iClientID);
		iClientID++;		// 다음 접속할 클라이언트 ID는 +1 해서 관리

	}

	while (1) {
		// 몬스터 정보 업데이트
		//CObjManager::Get_Instance()->Update_Monster();

		// 데이터 받기
		Receive_Data((LPVOID)client_sock, WorldInfo);

		// 데이터 보내기
		Send_Data((LPVOID)client_sock);
	}

	closesocket(client_sock);
	printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
		inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	return 0;
}

int main(int argc, char* argv[])
{
	int retval;

	// 맵 생성
	CTileManager::Get_Instance()->Load_Tile();

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET)
		err_quit("socket()");

	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	HANDLE hThread;

	// 이벤트 생성
	// TRUE로 신호상태로 놔야할듯
	hRecvEvent = CreateEvent(NULL, FALSE, FALSE, NULL);	// 자동 리셋, 비신호
	if (hRecvEvent == NULL) return 1;
	hSendEvent = CreateEvent(NULL, FALSE, TRUE, NULL);	// 자동 리셋, 신호
	if (hSendEvent == NULL) return 1;

	while (1)
	{
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		// 접속한 클라이언트 정보 출력
		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		// 스레드 생성
		hThread = CreateThread(NULL, 0, ProcessClient,
			(LPVOID)client_sock, 0, NULL);
		if (hThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hThread); }

		// mapIsReceive컨테이너에 ClientID를 key로 가진 bool변수를 false로 초기화 한 다음 삽입해준다.
		mapIsRecv.insert({ iClientID, false });
		mapIsCollision.insert({ iClientID, false });
	}

	// 이벤트 제거 
	CloseHandle(hRecvEvent);
	CloseHandle(hSendEvent);

	closesocket(listen_sock);

	// 윈속 종료
	WSACleanup();
	return 0;

}

void Receive_Data(LPVOID arg, map<int, ClientInfo> _worldInfo)
{
	// 전송 완료 대기
	DWORD EventRetval;
	EventRetval = WaitForSingleObject(hSendEvent, INFINITE);
	if (EventRetval != WAIT_OBJECT_0) return;

	// 연결된 클라이언트로부터 각 플레이어의 ClientInfo를 받는다.
	SOCKET client_sock = (SOCKET)arg;
	int retval;
	SOCKADDR_IN clientaddr;
	int addrlen;
	CLIENTINFO ClientInfo;
	bool isSend = false;

	// 클라이언트 정보 얻기
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);

	// 고정 길이 데이터 받아오기
	retval = recvn(client_sock, (char*)&ClientInfo, sizeof(CLIENTINFO), 0);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
	}

	auto iter = mapClientPort.find(clientaddr.sin_port);
	// WorldInfo의 ClientID 키값에 ClientInfo를 저장한다.
	WorldInfo.insert({ iter->second, ClientInfo });
	//if (iter != mapClientPort.end())
	//	WorldInfo[iter->second] = ClientInfo;
	//else
	//	WorldInfo.insert({ iter->second, ClientInfo });

	// 클라이언트로부터 수신이 끝나면 mapIsReceive컨테이너에 ClientID에 맞는 value를 true로 바꿔준다.
	mapIsRecv[iter->second] = true;

	// mapIsRecv 안의 모든 값이 true이면 Send 이벤트 신호 상태로 변경
	for (auto iter = mapIsRecv.begin(); iter != mapIsRecv.end(); ++iter)
	{
		if (!iter->second) {
			isSend = false;
			break;
		}

		else isSend = true;
	}

	if (isSend)
		SetEvent(hRecvEvent);
}

void Send_Data(LPVOID arg)
{
	// 수신 완료 대기
	DWORD EventRetval;
	EventRetval = WaitForSingleObject(hRecvEvent, INFINITE);
	if (EventRetval != WAIT_OBJECT_0) return;

	SOCKET client_sock = (SOCKET)arg;
	int retval;
	SOCKADDR_IN clientaddr;
	int addrlen;
	bool isRecv = false;

	// 클라이언트 정보 얻기
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);

	auto iter = mapClientPort.find(clientaddr.sin_port);
	int iClientKey = iter->second;


	//CLIENTINFO	tTest;
	// 본인 클라이언트 정보
	CLIENTINFO	tTest = WorldInfo[iter->second];
	retval = send(client_sock, (char*)&tTest, sizeof(CLIENTINFO), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}
	//int k = 0;
	//retval = send(client_sock, (char*)&k, sizeof(int), 0);
	//if (retval == SOCKET_ERROR) {
	//	err_display("send()");
	//}
	// 
	//CLIENTINFO	tTest;
	//retval = send(client_sock, (char*)&tTest, sizeof(CLIENTINFO), 0);
	//if (retval == SOCKET_ERROR) {
	//	err_display("send()");
	//}
	int k = 100;
	retval = send(client_sock, (char*)&k, sizeof(int), 0);

	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}
	/*retval = send(client_sock, (char*)&WorldInfo, sizeof(WorldInfo), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}*/

	//// 상대방 클라이언트 개수, 정보
	//int nClientNum = WorldInfo.size();
	//for (int i = 0; i < nClientNum; ++i) {
	//	if (i != iClientKey) {
	//		CLIENTINFO	tTest = WorldInfo[i];
	//		retval = send(client_sock, (char*)&tTest, sizeof(CLIENTINFO), 0);
	//		if (retval == SOCKET_ERROR) {
	//			err_display("send()");
	//		}
	//	}
	//}

	//// 업데이트된 몬스터들 위치
	//list<CObj*>	listMonster = CObjManager::Get_Instance()->Get_MonsterList();
	//int i = 0;
	//int iCnt = listMonster.size();

	//retval = send(client_sock, (char*)&iCnt, sizeof(int), 0);

	//for (auto iter = listMonster.begin(); iter != listMonster.end(); ++iter)
	//{
	//	vecMonster[i].MonsterPos.fX = (*iter)->Get_Info().fX;
	//	vecMonster[i].MonsterPos.fY = (*iter)->Get_Info().fY;
	//	vecMonster[i].MonsterDir = (*iter)->GetDir();
	//	vecMonster[i].Monsterframe = (*iter)->Get_Frame();

	//	retval = send(client_sock, (char*)&vecMonster[i], sizeof(MONSTERINFO), 0);
	//	if (retval == SOCKET_ERROR) {
	//		err_display("send()");
	//	}
	//	++i;
	//}

	//else
	//{
		// 전송 성공 -> mapIsReceive의 현재 ClientID의 value값을 false로 설정
		/*auto */iter = mapClientPort.find(clientaddr.sin_port);
		mapIsRecv[iter->second] = false;
	//}

	for (auto iter = mapIsRecv.begin(); iter != mapIsRecv.end(); ++iter)
	{
		if (iter->second) {
			isRecv = false;
			break;
		}

		else isRecv = true;
	}

	if (isRecv)
		SetEvent(hSendEvent);
}

void CheckBuff()
{
	// 모든 클라이언트에게 정보를 받은 후
	// 모든 플레이어가 충돌했다면 PlayInfo 안의 b_isContactPlayer를 true로 설정
	RECT rc = {};
	auto Dst = WorldInfo.begin();
	bool isBuffOn = false;

	// 충돌 판단
	for (auto Src = WorldInfo.begin(); Src != WorldInfo.end(); ++Src) 
	{
		if (Src != WorldInfo.begin()) 
		{
			if (IntersectRect(&rc, &Dst->second.PlayerInfo.PlayerSize, &Src->second.PlayerInfo.PlayerSize)) 
			{
				auto iter = mapIsCollision.find(Dst->first);
				iter->second = true;
				iter = mapIsCollision.find(Src->first);
				iter->second = true;
			}
		}
	}

	// 모든 플레이어가 충돌했다면 isBuffOn을 true로 설정
	for (auto iter = mapIsCollision.begin(); iter != mapIsCollision.end(); ++iter) 
	{
		if (iter->second)
		{
			isBuffOn = false;
			break;
		}

		else isBuffOn = true;
	}

	// isBuffOn이 true라면 CLIENTINFO 속 b_isContactPlayer를 true로 설정한 후
	// mapIsCollision은 다시 false로 설정
	if (isBuffOn)
	{
		for (auto iter = WorldInfo.begin(); iter != WorldInfo.end(); ++iter)
			iter->second.PlayerInfo.b_isContactPlayer = true;

		for (auto iter = mapIsCollision.begin(); iter != mapIsCollision.end(); ++iter)
			iter->second = false;
	}
}

void Send_InitMap(LPVOID arg)
{
	SOCKET client_sock = (SOCKET)arg;
	int retval;
	char buf[BUFSIZE + 1];

	ifstream    in{ "Tile.dat", ios::in | ios::binary }; // video.mp4
	if (!in) {
		printf("파일 열기 실패\n");
		exit(0);
	}
	in.seekg(0, ios::end);
	int iSize = in.tellg();
	in.seekg(0, ios::beg);

	int len = strlen("Tile.dat");
	strncpy_s(buf, "Tile.dat", len);

	// 고정 - 파일 제목 크기
	retval = send(client_sock, (char*)&len, sizeof(int), 0);
	if (retval == SOCKET_ERROR) err_display("send()");

	// 가변 - 파일 제목
	retval = send(client_sock, buf, len, 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}

	// 고정 - 파일 내용 크기
	retval = send(client_sock, (char*)&iSize, sizeof(int), 0);
	if (retval == SOCKET_ERROR) err_display("send()");

	char* FileData = new char[iSize];
	in.read(&FileData[0], iSize);

	// 가변 - 파일 데이터
	retval = send(client_sock, &FileData[0], iSize, 0);
	if (retval == SOCKET_ERROR)
		err_display("send()");
}

void Init_Monster(LPVOID arg)
{
	SOCKET client_sock = (SOCKET)arg;
	int retval;

	CObj* pObj = nullptr;
	MONSTERINFO	tInfo;

	pObj = CAbstractFactory<CMessi>::Create_Monster(MAPSTARTX + (TILECX * 14) + (TILECX >> 1), MAPSTARTY + (TILECY * 6) + (TILECY >> 1), OBJDIR::BOTTOM);
	CObjManager::Get_Instance()->Add_Object(pObj, OBJID::MONSTER);
	tInfo.MonsterName = MONSTERNAME::MESSI;
	tInfo.MonsterPos.fX = MAPSTARTX + (TILECX * 14) + (TILECX >> 1);
	tInfo.MonsterPos.fY = MAPSTARTY + (TILECY * 6) + (TILECY >> 1);
	tInfo.MonsterDir = OBJDIR::BOTTOM;
	tInfo.MonsterID = 1;
	vecMonster.emplace_back(tInfo);

	pObj = CAbstractFactory<CMessi>::Create_Monster(MAPSTARTX + (TILECX * 12) + (TILECX >> 1), MAPSTARTY + (TILECY * 6) + (TILECY >> 1), OBJDIR::BOTTOM);
	CObjManager::Get_Instance()->Add_Object(pObj, OBJID::MONSTER);
	tInfo.MonsterName = MONSTERNAME::MESSI;
	tInfo.MonsterPos.fX = MAPSTARTX + (TILECX * 12) + (TILECX >> 1);
	tInfo.MonsterPos.fY = MAPSTARTY + (TILECY * 6) + (TILECY >> 1);
	tInfo.MonsterDir = OBJDIR::BOTTOM;
	tInfo.MonsterID = 2;
	vecMonster.emplace_back(tInfo);

	pObj = CAbstractFactory<CMessi>::Create_Monster(MAPSTARTX + (TILECX * 8) + (TILECX >> 1), MAPSTARTY + (TILECY * 0) + (TILECY >> 1), OBJDIR::LEFT);
	CObjManager::Get_Instance()->Add_Object(pObj, OBJID::MONSTER);
	tInfo.MonsterName = MONSTERNAME::MESSI;
	tInfo.MonsterPos.fX = MAPSTARTX + (TILECX * 8) + (TILECX >> 1);
	tInfo.MonsterPos.fY = MAPSTARTY + (TILECY * 0) + (TILECY >> 1);
	tInfo.MonsterDir = OBJDIR::LEFT;
	tInfo.MonsterID = 3;
	vecMonster.emplace_back(tInfo);

	pObj = CAbstractFactory<CMessi>::Create_Monster(MAPSTARTX + (TILECX * 8) + (TILECX >> 1), MAPSTARTY + (TILECY * 2) + (TILECY >> 1), OBJDIR::BOTTOM);
	CObjManager::Get_Instance()->Add_Object(pObj, OBJID::MONSTER);
	tInfo.MonsterName = MONSTERNAME::MESSI;
	tInfo.MonsterPos.fX = MAPSTARTX + (TILECX * 8) + (TILECX >> 1);
	tInfo.MonsterPos.fY = MAPSTARTY + (TILECY * 2) + (TILECY >> 1);
	tInfo.MonsterDir = OBJDIR::BOTTOM;
	tInfo.MonsterID = 4;
	vecMonster.emplace_back(tInfo);

	pObj = CAbstractFactory<CMessi>::Create_Monster(MAPSTARTX + (TILECX * 0) + (TILECX >> 1), MAPSTARTY + (TILECY * 6) + (TILECY >> 1), OBJDIR::RIGHT);
	CObjManager::Get_Instance()->Add_Object(pObj, OBJID::MONSTER);
	tInfo.MonsterName = MONSTERNAME::MESSI;
	tInfo.MonsterPos.fX = MAPSTARTX + (TILECX * 0) + (TILECX >> 1);
	tInfo.MonsterPos.fY = MAPSTARTY + (TILECY * 6) + (TILECY >> 1);
	tInfo.MonsterDir = OBJDIR::RIGHT;
	tInfo.MonsterID = 5;
	vecMonster.emplace_back(tInfo);

	pObj = CAbstractFactory<CMessi>::Create_Monster(MAPSTARTX + (TILECX * 3) + (TILECX >> 1), MAPSTARTY + (TILECY * 6) + (TILECY >> 1), OBJDIR::BOTTOM);
	CObjManager::Get_Instance()->Add_Object(pObj, OBJID::MONSTER);
	tInfo.MonsterName = MONSTERNAME::MESSI;
	tInfo.MonsterPos.fX = MAPSTARTX + (TILECX * 3) + (TILECX >> 1);
	tInfo.MonsterPos.fY = MAPSTARTY + (TILECY * 6) + (TILECY >> 1);
	tInfo.MonsterDir = OBJDIR::BOTTOM;
	tInfo.MonsterID = 6;
	vecMonster.emplace_back(tInfo);
	//
	pObj = CAbstractFactory<CMessi>::Create_Monster(MAPSTARTX + (TILECX * 7) + (TILECX >> 1), MAPSTARTY + (TILECY * 10) + (TILECY >> 1), OBJDIR::LEFT);
	CObjManager::Get_Instance()->Add_Object(pObj, OBJID::MONSTER);
	tInfo.MonsterName = MONSTERNAME::MESSI;
	tInfo.MonsterPos.fX = MAPSTARTX + (TILECX * 7) + (TILECX >> 1);
	tInfo.MonsterPos.fY = MAPSTARTY + (TILECY * 10) + (TILECY >> 1);
	tInfo.MonsterDir = OBJDIR::LEFT;
	tInfo.MonsterID = 7;
	vecMonster.emplace_back(tInfo);

	pObj = CAbstractFactory<CMessi>::Create_Monster(MAPSTARTX + (TILECX * 7) + (TILECX >> 1), MAPSTARTY + (TILECY * 12) + (TILECY >> 1), OBJDIR::TOP);
	CObjManager::Get_Instance()->Add_Object(pObj, OBJID::MONSTER);
	tInfo.MonsterName = MONSTERNAME::MESSI;
	tInfo.MonsterPos.fX = MAPSTARTX + (TILECX * 7) + (TILECX >> 1);
	tInfo.MonsterPos.fY = MAPSTARTY + (TILECY * 12) + (TILECY >> 1);
	tInfo.MonsterDir = OBJDIR::TOP;
	tInfo.MonsterID = 8;
	vecMonster.emplace_back(tInfo);

	pObj = CAbstractFactory<CMbape>::Create_Monster(MAPSTARTX + (TILECX * 4) + (TILECX >> 1), MAPSTARTY + (TILECY * 6) + (TILECY >> 1), OBJDIR::TOP);
	CObjManager::Get_Instance()->Add_Object(pObj, OBJID::MONSTER);
	tInfo.MonsterName = MONSTERNAME::MBAPE;
	tInfo.MonsterPos.fX = MAPSTARTX + (TILECX * 4) + (TILECX >> 1);
	tInfo.MonsterPos.fY = MAPSTARTY + (TILECY * 6) + (TILECY >> 1);
	tInfo.MonsterDir = OBJDIR::TOP;
	tInfo.MonsterID = 9;
	vecMonster.emplace_back(tInfo);

	pObj = CAbstractFactory<CMbape>::Create_Monster(MAPSTARTX + (TILECX * 10) + (TILECX >> 1), MAPSTARTY + (TILECY * 6) + (TILECY >> 1), OBJDIR::BOTTOM);
	CObjManager::Get_Instance()->Add_Object(pObj, OBJID::MONSTER);
	tInfo.MonsterName = MONSTERNAME::MBAPE;
	tInfo.MonsterPos.fX = MAPSTARTX + (TILECX * 10) + (TILECX >> 1);
	tInfo.MonsterPos.fY = MAPSTARTY + (TILECY * 6) + (TILECY >> 1);
	tInfo.MonsterDir = OBJDIR::BOTTOM;
	tInfo.MonsterID = 10;
	vecMonster.emplace_back(tInfo);

	int iNum = 10;

	retval = send(client_sock, (char*)&iNum, sizeof(int), 0);
	if (retval == SOCKET_ERROR) err_display("send()");

	//for (int i = 0; i < iNum; ++i) {
	//	retval = send(client_sock, (char*)&vecMonster[i], sizeof(MONSTERINFO), 0);
	//	if (retval == SOCKET_ERROR) {
	//		err_display("send()");
	//	}
	//}
	retval = send(client_sock, (char*)&vecMonster[0], sizeof(MONSTERINFO), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}
	retval = send(client_sock, (char*)&vecMonster[1], sizeof(MONSTERINFO), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}
	retval = send(client_sock, (char*)&vecMonster[2], sizeof(MONSTERINFO), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}
	retval = send(client_sock, (char*)&vecMonster[3], sizeof(MONSTERINFO), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}
	retval = send(client_sock, (char*)&vecMonster[4], sizeof(MONSTERINFO), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}
	retval = send(client_sock, (char*)&vecMonster[5], sizeof(MONSTERINFO), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}
	retval = send(client_sock, (char*)&vecMonster[6], sizeof(MONSTERINFO), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}
	retval = send(client_sock, (char*)&vecMonster[7], sizeof(MONSTERINFO), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}
	retval = send(client_sock, (char*)&vecMonster[8], sizeof(MONSTERINFO), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}
	retval = send(client_sock, (char*)&vecMonster[9], sizeof(MONSTERINFO), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}
}
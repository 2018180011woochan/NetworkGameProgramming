//#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#define _CRT_SECURE_NO_WRANINGS
#include "stdafx.h"
#include "TileManager.h"
#include "Obj.h"
#include "ObjManager.h"
#include "Mbape.h"
#include "Messi.h"
#include "TimeManager.h"

#define BUFSIZE 500
static int iClientID = 0;				// 클라이언트의 ID
map<int, CLIENTINFO> WorldInfo;			// 클라이언트로 보낼 패킷
map<USHORT, int> mapClientPort;			// 클라이언트의 포트번호와 클라이언트ID 저장
										//map<int, bool> mapIsRecv;				// 클라이언트에서 데이터를 전송받았는지 판단하기 위한 맵
map<int, bool> mapIsCollision;			// 버프 판단을 위한 충돌 확인 맵
vector<USHORT>	vecClientPort;

vector<CObj*>	vecMapTile;				// 맵 타일
vector<USHORT> vecIsFirstConnect;		// 클라이언트가 접속하면 클라이언트의 포트번호를 저장함 (처음 접속인지 확인용)

HANDLE hRecvEvent;
HANDLE hSendEvent;

bool isStart = false;
bool isSetTimer = false;
#define SERVERPORT 9000
//CRITICAL_SECTION cs;

void Receive_Data(LPVOID arg, map<int, ClientInfo> _worldInfo);
void Send_Data(LPVOID arg);
void Send_InitMap(LPVOID arg);

void CheckBuff();

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

	CLIENTINFO ClientInfo;

	// 클라이언트 정보 얻기
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);

	// 현재 클라이언트가 처음 접속했는지 확인
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


		printf("포트 번호=%d 에게 ClientID: %d 전송 성공\n", ntohs(clientaddr.sin_port), iClientID);

		// 각 map에 insert
		mapIsCollision.insert({ iClientID, false });
		WorldInfo.insert({ iClientID, ClientInfo });
		WorldInfo[iClientID] = ClientInfo;
		iClientID++;		// 다음 접속할 클라이언트 ID는 +1 해서 관리
	}

	while (1) {
		// 데이터 받기
		Receive_Data((LPVOID)client_sock, WorldInfo);

		// 버프 확인
		//CheckBuff();

		// 데이터 보내기
		Send_Data((LPVOID)client_sock);


		CTimeManager::Get_Instance()->Update_CTimeManager();
	}

	closesocket(client_sock);
	printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
		inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	return 0;
}

int main(int argc, char* argv[])
{
	//InitializeCriticalSection(&cs);

	int retval;

	// 맵 생성
	CTileManager::Get_Instance()->Load_Tile();
	CObjManager::Get_Instance()->Load_Object();

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET)
		err_quit("socket()");
	//int	option = TRUE;	// Nagle On
	//setsockopt(listen_sock, IPPROTO_TCP, TCP_NODELAY, (const char*)&option, sizeof(option));

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
	hRecvEvent = CreateEvent(NULL, FALSE, TRUE, NULL);		// 자동 리셋, 신호
	if (hRecvEvent == NULL) return 1;

	hSendEvent = CreateEvent(NULL, FALSE, TRUE, NULL);		// 자동 리셋, 비신호
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
	}

	// 이벤트 제거
	CloseHandle(hRecvEvent);
	CloseHandle(hSendEvent);

	//DeleteCriticalSection(&cs);

	closesocket(listen_sock);

	// 윈속 종료
	WSACleanup();
	return 0;

}

void Receive_Data(LPVOID arg, map<int, ClientInfo> _worldInfo)
{
	//EnterCriticalSection(&cs);

	// 연결된 클라이언트로부터 각 플레이어의 ClientInfo를 받는다.
	SOCKET client_sock = (SOCKET)arg;
	int retval;
	SOCKADDR_IN clientaddr;
	int addrlen;
	CLIENTINFO ClientInfo;

	bool isFinishRecv = false;

	// 클라이언트 정보 얻기
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);
	printf("R : %d\n", clientaddr.sin_port);

	// 고정 길이 데이터 받아오기
	retval = recvn(client_sock, (char*)&ClientInfo, sizeof(CLIENTINFO), 0);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
	}

	// 받아온 데이터 map에 저장
	auto iter = mapClientPort.find(clientaddr.sin_port);

	//////////////////////////////////////////////////////////////////////////////////////////////////
	


	// # 4. 변화된 타일 정보
	int nTileCnt = -1;
	retval = recvn(client_sock, (char*)&nTileCnt, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
	}

	if (nTileCnt > 0) {
		vector<int>	vecTileKey;
		vecTileKey.resize(nTileCnt);

		for (int i = 0; i < nTileCnt; ++i) {
			retval = recvn(client_sock, (char*)&vecTileKey[i], sizeof(int), 0);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
			}

			// 버블 상태가 될 블럭의 오브젝트num을 받아와서 저장
			CObjManager::Get_Instance()->Add_CollBlock(vecTileKey[i]);

			// 아이템을 갖고 있는 블럭인지 확인
			CObjManager::Get_Instance()->Check_ItemBlock(vecTileKey[i]);
		}

		// 아이템 생성
		CObjManager::Get_Instance()->Make_Item();
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////

	auto Portiter = mapClientPort.find(clientaddr.sin_port);
	// WorldInfo의 ClientID 키값에 ClientInfo를 저장한다.
	WorldInfo.insert({ Portiter->second, ClientInfo });
	WorldInfo[Portiter->second] = ClientInfo;

	//LeaveCriticalSection(&cs);
}

void Send_Data(LPVOID arg)
{
	//EnterCriticalSection(&cs);

	SOCKET client_sock = (SOCKET)arg;
	int retval;
	SOCKADDR_IN clientaddr;
	int addrlen;

	bool isFinishSend = false;

	// 클라이언트 정보 얻기
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);

	printf("S : %d\n", clientaddr.sin_port);

	retval = send(client_sock, (char*)&iClientID, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}

	for (int i = 0; i < iClientID; ++i)
	{
		//auto iter = mapClientPort.find(clientaddr.sin_port);
		WorldInfo[i].ClientID_Number = iClientID;

		retval = send(client_sock, (char*)&WorldInfo[i], sizeof(CLIENTINFO), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	

	// 맵 블럭
	vector<int>	vecDeadTileKey = CObjManager::Get_Instance()->Get_DeadTile();
	int	iTileNum = vecDeadTileKey.size();

	// - 없어질 타일
	retval = send(client_sock, (char*)&iTileNum, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}

	if (iTileNum > 0) {
		for (int i = 0; i < iTileNum; ++i) {
			retval = send(client_sock, (char*)&vecDeadTileKey[i], sizeof(int), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
			}
		}
	}

	// - 아이템이 생길 타일
	vector<ITEMINFO>	vecItem = CObjManager::Get_Instance()->Get_vecItemInfo();
	int	iItemNum = vecItem.size();

	retval = send(client_sock, (char*)&iItemNum, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}

	if (iItemNum > 0) {
		for (int i = 0; i < iItemNum; ++i) {
			retval = send(client_sock, (char*)&vecItem[i], sizeof(ITEMINFO), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
			}
		}
	}

	CObjManager::Get_Instance()->Clear_DeadTile();


	//////////////////////////////////////////////////////////////////////////////////////////////////

	//LeaveCriticalSection(&cs);
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

	ifstream    in{ "newTile2.dat", ios::in | ios::binary };
	if (!in) {
		printf("파일 열기 실패\n");
		exit(0);
	}
	in.seekg(0, ios::end);
	int iSize = in.tellg();
	in.seekg(0, ios::beg);

	int len = strlen("newTile2.dat");
	strncpy_s(buf, "newTile2.dat", len);

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
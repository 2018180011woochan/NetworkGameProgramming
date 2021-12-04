//#define _WINSOCK_DEPRECATED_NO_WARNINGS // �ֽ� VC++ ������ �� ��� ����
#define _CRT_SECURE_NO_WRANINGS
#include "stdafx.h"
#include "TileManager.h"
#include "Obj.h"
#include "ObjManager.h"
#include "Mbape.h"
#include "Messi.h"
#include "TimeManager.h"

#define BUFSIZE 500
static int iClientID = 0;				// Ŭ���̾�Ʈ�� ID
map<int, CLIENTINFO> WorldInfo;			// Ŭ���̾�Ʈ�� ���� ��Ŷ
map<USHORT, int> mapClientPort;			// Ŭ���̾�Ʈ�� ��Ʈ��ȣ�� Ŭ���̾�ƮID ����
map<int, bool> mapIsRecv;				// Ŭ���̾�Ʈ���� �����͸� ���۹޾Ҵ��� �Ǵ��ϱ� ���� ��
map<int, bool> mapIsCollision;			// ���� �Ǵ��� ���� �浹 Ȯ�� ��

HANDLE hRecvEvent;		// �� Ŭ���̾�Ʈ���� ���� ����� �˷��ֱ� ���� �̺�Ʈ
HANDLE hSendEvent;		// �� Ŭ���̾�Ʈ���� �۽� ����� �˷��ֱ� ���� �̺�Ʈ
vector<CObj*>	vecMapTile;				// �� Ÿ��
vector<USHORT> vecIsFirstConnect;		// Ŭ���̾�Ʈ�� �����ϸ� Ŭ���̾�Ʈ�� ��Ʈ��ȣ�� ������ (ó�� �������� Ȯ�ο�)
vector<MONSTERINFO>	vecMonster;

bool isStart = false;
bool isSetTimer = false;
#define SERVERPORT 9000

void Receive_Data(LPVOID arg, map<int, ClientInfo> _worldInfo);
void Send_Data(LPVOID arg);
void Send_InitMap(LPVOID arg);

void CheckBuff();
void Init_Monster(LPVOID arg);

// ���� �Լ� ���� ��� �� ����
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

// ���� �Լ� ���� ���
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

// ����� ���� ������ ���� �Լ�
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

// Ŭ���̾�Ʈ�� ������ ���
DWORD WINAPI ProcessClient(LPVOID arg)
{
	SOCKET client_sock = (SOCKET)arg;
	int retval;
	SOCKADDR_IN clientaddr;
	int addrlen;
	char buf[BUFSIZE + 1];

	// Ŭ���̾�Ʈ ���� ���
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);

	// ���� Ŭ���̾�Ʈ�� ó�� �����ߴ��� Ȯ��
	//auto iter = find(mapClientPort.begin(), mapClientPort.end(), clientaddr.sin_port);
	auto iter = mapClientPort.find(clientaddr.sin_port);
	if (iter == mapClientPort.end())
	{
		// ó�� �����̸� ��Ʈ ��ȣ�� ����
		mapClientPort.insert({ clientaddr.sin_port, iClientID });
		// ClientID ������ (�Ŀ� ó�� ���������� ������ ó��)
		retval = send(client_sock, (char*)&iClientID, sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
		}

		// �� ���� ���� -> Ŭ���̾�Ʈ�� �� ������ �������� �� �ʱ�ȭ
		Send_InitMap((LPVOID)client_sock);

		// ���� ���� ����
		//Init_Monster((LPVOID)client_sock);

		printf("��Ʈ ��ȣ=%d ���� ClientID: %d ���� ����\n", ntohs(clientaddr.sin_port), iClientID);

		// mapIsReceive�����̳ʿ� ClientID�� key�� ���� bool������ false�� �ʱ�ȭ �� ���� �������ش�.
		mapIsRecv.insert({ iClientID, false });
		mapIsRecv[0] = false;
		//SetEvent(hSendEvent);
		iClientID++;		// ���� ������ Ŭ���̾�Ʈ ID�� +1 �ؼ� ����


		SetEvent(hSendEvent);
	}

	while (1) {

		// ���� ���� ������Ʈ
		//if (isStart)
			//CObjManager::Get_Instance()->Update_Monster();

		// ������ �ޱ�
		Receive_Data((LPVOID)client_sock, WorldInfo);

		// ���� Ȯ��
		//CheckBuff();

		// ������ ������
		Send_Data((LPVOID)client_sock);

		CTimeManager::Get_Instance()->Update_CTimeManager();
	}

	closesocket(client_sock);
	printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
		inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	return 0;
}

int main(int argc, char* argv[])
{
	int retval;

	// �� ����
	CTileManager::Get_Instance()->Load_Tile();

	// ���� �ʱ�ȭ
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

	// ������ ��ſ� ����� ����
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	HANDLE hThread;

	// �̺�Ʈ ����
	// TRUE�� ��ȣ���·� �����ҵ�
	hRecvEvent = CreateEvent(NULL, FALSE, FALSE, NULL);	// �ڵ� ����, ���ȣ
	if (hRecvEvent == NULL) return 1;
	hSendEvent = CreateEvent(NULL, FALSE, TRUE, NULL);	// �ڵ� ����, ��ȣ
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

		// ������ Ŭ���̾�Ʈ ���� ���
		printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		// ������ ����
		hThread = CreateThread(NULL, 0, ProcessClient,
			(LPVOID)client_sock, 0, NULL);

		if (hThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hThread); }

		// mapIsReceive�����̳ʿ� ClientID�� key�� ���� bool������ false�� �ʱ�ȭ �� ���� �������ش�.
		mapIsRecv.insert({ iClientID, false });
		mapIsCollision.insert({ iClientID, false });
	}

	// �̺�Ʈ ����
	CloseHandle(hRecvEvent);
	CloseHandle(hSendEvent);

	closesocket(listen_sock);

	// ���� ����
	WSACleanup();
	return 0;

}

void Receive_Data(LPVOID arg, map<int, ClientInfo> _worldInfo)
{
	// ����� Ŭ���̾�Ʈ�κ��� �� �÷��̾��� ClientInfo�� �޴´�.
	SOCKET client_sock = (SOCKET)arg;
	int retval;
	SOCKADDR_IN clientaddr;
	int addrlen;
	CLIENTINFO ClientInfo;
	bool isSend = false;

	// Ŭ���̾�Ʈ ���� ���
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);

	// ���� ���� ������ �޾ƿ���
	retval = recvn(client_sock, (char*)&ClientInfo, sizeof(CLIENTINFO), 0);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
	}

	// ���� �����ߴ��� Ȯ���ϴ� ���� �޾ƿ���
	//int iStart;
	//retval = recvn(client_sock, (char*)&iStart, sizeof(int), 0);
	//if (retval == SOCKET_ERROR) {
	//	err_display("recv()");
	//}
	//if (iStart == 1 && !isSetTimer) {
	//	isStart = true;
	//	isSetTimer = true;
	//	CTimeManager::Get_Instance()->Ready_CTimeManager();
	//}

	auto iter = mapClientPort.find(clientaddr.sin_port);
	// WorldInfo�� ClientID Ű���� ClientInfo�� �����Ѵ�.
	WorldInfo.insert({ iter->second, ClientInfo });

	// �ǽð����� ���� ClientInfo ���� �ٲ��ش�1
	WorldInfo[iter->second] = ClientInfo;
}

void Send_Data(LPVOID arg)
{
	SOCKET client_sock = (SOCKET)arg;
	int retval;
	SOCKADDR_IN clientaddr;
	int addrlen;
	bool isRecv = false;

	// Ŭ���̾�Ʈ ���� ���
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);

	for (int i = 0; i < iClientID; ++i)
	{
		WorldInfo[i].ClientID_Number = iClientID;
		retval = send(client_sock, (char*)&WorldInfo[i], sizeof(CLIENTINFO), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
		}
	}

	//if (isStart) {
	//	// ������Ʈ�� ���͵� ��ġ
	//	list<CObj*>	listMonster = CObjManager::Get_Instance()->Get_MonsterList();
	//	int i = 0;
	//	int iCnt = listMonster.size();

	//	for (auto iter = listMonster.begin(); iter != listMonster.end(); ++iter)
	//	{
	//		vecMonster[i].MonsterPos.fX = (*iter)->Get_Info().fX;
	//		vecMonster[i].MonsterPos.fY = (*iter)->Get_Info().fY;
	//		vecMonster[i].MonsterDir = (*iter)->GetDir();
	//		vecMonster[i].Monsterframe = (*iter)->Get_Frame();
	//		++i;
	//	}

	//	retval = send(client_sock, (char*)&vecMonster[0], sizeof(MONSTERINFO), 0);
	//	if (retval == SOCKET_ERROR) {
	//		err_display("send()");
	//	}
	//	retval = send(client_sock, (char*)&vecMonster[1], sizeof(MONSTERINFO), 0);
	//	if (retval == SOCKET_ERROR) {
	//		err_display("send()");
	//	}
	//	retval = send(client_sock, (char*)&vecMonster[2], sizeof(MONSTERINFO), 0);
	//	if (retval == SOCKET_ERROR) {
	//		err_display("send()");
	//	}
	//	retval = send(client_sock, (char*)&vecMonster[3], sizeof(MONSTERINFO), 0);
	//	if (retval == SOCKET_ERROR) {
	//		err_display("send()");
	//	}
	//	retval = send(client_sock, (char*)&vecMonster[4], sizeof(MONSTERINFO), 0);
	//	if (retval == SOCKET_ERROR) {
	//		err_display("send()");
	//	}
	//	retval = send(client_sock, (char*)&vecMonster[5], sizeof(MONSTERINFO), 0);
	//	if (retval == SOCKET_ERROR) {
	//		err_display("send()");
	//	}
	//	retval = send(client_sock, (char*)&vecMonster[6], sizeof(MONSTERINFO), 0);
	//	if (retval == SOCKET_ERROR) {
	//		err_display("send()");
	//	}
	//	retval = send(client_sock, (char*)&vecMonster[7], sizeof(MONSTERINFO), 0);
	//	if (retval == SOCKET_ERROR) {
	//		err_display("send()");
	//	}
	//}
}

void CheckBuff()
{
	// ��� Ŭ���̾�Ʈ���� ������ ���� ��
	// ��� �÷��̾ �浹�ߴٸ� PlayInfo ���� b_isContactPlayer�� true�� ����
	RECT rc = {};
	auto Dst = WorldInfo.begin();
	bool isBuffOn = false;

	// �浹 �Ǵ�
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

	// ��� �÷��̾ �浹�ߴٸ� isBuffOn�� true�� ����
	for (auto iter = mapIsCollision.begin(); iter != mapIsCollision.end(); ++iter)
	{
		if (iter->second)
		{
			isBuffOn = false;
			break;
		}

		else isBuffOn = true;
	}

	// isBuffOn�� true��� CLIENTINFO �� b_isContactPlayer�� true�� ������ ��
	// mapIsCollision�� �ٽ� false�� ����
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
		printf("���� ���� ����\n");
		exit(0);
	}
	in.seekg(0, ios::end);
	int iSize = in.tellg();
	in.seekg(0, ios::beg);

	int len = strlen("Tile.dat");
	strncpy_s(buf, "Tile.dat", len);

	// ���� - ���� ���� ũ��
	retval = send(client_sock, (char*)&len, sizeof(int), 0);
	if (retval == SOCKET_ERROR) err_display("send()");

	// ���� - ���� ����
	retval = send(client_sock, buf, len, 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}

	// ���� - ���� ���� ũ��
	retval = send(client_sock, (char*)&iSize, sizeof(int), 0);
	if (retval == SOCKET_ERROR) err_display("send()");

	char* FileData = new char[iSize];
	in.read(&FileData[0], iSize);

	// ���� - ���� ������
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

	int iNum = 8;

	retval = send(client_sock, (char*)&iNum, sizeof(int), 0);
	if (retval == SOCKET_ERROR) err_display("send()");

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
}
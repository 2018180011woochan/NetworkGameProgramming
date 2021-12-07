#include "stdafx.h"
#include "ClientManager.h"
#include "TileManager.h"
#include "ObjManager.h"
#include "Player.h"
#include "SceneManager.h"


#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000

CClientManager* CClientManager::m_pInstance = nullptr;

CClientManager::CClientManager()
{
	ZeroMemory(&tClientInfo, sizeof(tClientInfo));
	tClientInfo.PlayerInfo.PlayerPos.fX = 100.0f;
	tClientInfo.PlayerInfo.PlayerPos.fY = 100.0f;
}

CClientManager::~CClientManager()
{
}

int CClientManager::connectToServer()
{
	// ������ �ʱ�ȭ�� ��, ������ ����� �ϱ� ���� ������ �����ϰ� �������� ������ ��û�ϴ� �Լ�
	// ���ϰ�: retval - connect()�� ���ϰ�

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket ����
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");


	// connect() - ������ ���� ��û
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");


	return retval;
}

void CClientManager::recvClientID()
{
	retval = recvn(sock, (char*)&iClientID, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		exit(1);
	}
	tClientInfo.ClientID = iClientID;
}

int CClientManager::sendInfo()
{
	//ZeroMemory(&tClientInfo, sizeof(CLIENTINFO));
	// ClientID�� ���� ĳ���͸� ���ؼ� ������ ����
	// ClientID : 0 -> ����,	ClientID : 1 -> �ٿ�,	ClientID : 2 -> �����

	switch (iClientID)
	{
	case 0:
		tClientInfo.PlayerInfo.PlayerName = CHARNAME::BAZZI;
		break;
	case 1:
		tClientInfo.PlayerInfo.PlayerName = CHARNAME::DAO;
		break;
	case 2:
		tClientInfo.PlayerInfo.PlayerName = CHARNAME::DIGENIE;
		break;
	default:
		tClientInfo.PlayerInfo.PlayerName = CHARNAME::UNI;
		break;
	}

	// �÷��̾� ��ġ tClientInfo�� ����
	tClientInfo.BombPos.fX = 0.f;
	tClientInfo.BombPos.fY = 0.f;
	setPlayerInfo();

	// ���� �𸣰ڴµ� ClientID�� �ʱ�ȭ��; �ٽ� ��������
	tClientInfo.ClientID = iClientID;

	// ������ �� ���۵ƴ��� �����غ��� ���� (���� �� ������ ��)
	retval = send(sock, (char*)&tClientInfo, sizeof(CLIENTINFO), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}


	int iStart = -1;
	if (bisStart)	iStart = 1;
	else	iStart = 2;
	// ���� �����ߴ��� Ȯ��
	retval = send(sock, (char*)&iStart, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}

	// ��ȭ�� ���� ���� ����
	if (bisStart) {
		int iNum = 0;
		list<CObj*> monsterList = CObjManager::Get_Instance()->Get_MonsterList();
		for (auto iter = monsterList.begin(); iter != monsterList.end(); ++iter) {
			if ((*iter)->GetState() == OBJSTATE::HIT) {
				//tMonsterInfo.erase(tMonsterInfo.begin() + iNum);
				tMonsterInfo[iNum].MonsterDead = true;
				SAFE_DELETE(*iter);
				iter = monsterList.erase(iter);
			}
			++iNum;
		}
		iMonsterCnt = monsterList.size();
		CObjManager::Get_Instance()->Set_MonsterList(monsterList);


		retval = send(sock, (char*)&iNum, sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
		}

		for (int i = 0; i < iNum; ++i) {
			retval = send(sock, (char*)&tMonsterInfo[i], sizeof(MONSTERINFO), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
			}
		}
	}

	return retval;
}

int CClientManager::recvInfo()
{
	// �����κ��� ��� Ŭ���̾�Ʈ�� 
	// �÷��̾� ����, ������ ����, ���� ������ ��� �ִ� 
	// WorldInfo �� �����̳ʸ� �޴´�.

	AllClientNum = tClientInfo.ClientID_Number;	// �� ������ Ŭ���̾�Ʈ�� ����

	if (AllClientNum == 0)
		AllClientNum = 1;

	for (int i = 0; i < AllClientNum; ++i)
	{
		ZeroMemory(&tClientInfo, sizeof(CLIENTINFO));
		retval = recvn(sock, (char*)&tClientInfo, sizeof(CLIENTINFO), 0);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
		}
		cout << "������ Ŭ���̾�Ʈ �� : " << AllClientNum << endl;
		cout << "Player" << tClientInfo.ClientID << "-> X:" << tClientInfo.PlayerInfo.PlayerPos.fX
			<< " Y:" << tClientInfo.PlayerInfo.PlayerPos.fY << endl;
		cout << "-------------------------------------------" << endl;
		tWorldInfo.insert({ tClientInfo.ClientID, tClientInfo });
		tWorldInfo[tClientInfo.ClientID] = tClientInfo;

		// ������ �ٸ� Ŭ���̾�Ʈ�� objManager�� �̿��Ͽ� obj��Ͽ� ����
		if (iClientID != tClientInfo.ClientID)
		{
			auto iter = find(vecClientID.begin(), vecClientID.end(), tClientInfo.ClientID);
			// ó������ ����Ʈ�� �߰��ϰ� ����
			if (iter == vecClientID.end())
			{
				CObjManager::Get_Instance()->Add_NetWorkPlayer(tClientInfo);
				vecClientID.push_back(tClientInfo.ClientID);
			}
			// �ι�°���� �÷��̾� ���� ������Ʈ
			else
			{
				CObjManager::Get_Instance()->Update_NetWorkPlayer(tClientInfo);
			}
		}
	}
	
	// �����κ��� ���� ���� ����
	if (bisStart) {
		retval = recvn(sock, (char*)&iMonsterCnt, sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
		}

		for (int i = 0; i < iMonsterCnt; ++i) {
			retval = recvn(sock, (char*)&tMonsterInfo[i], sizeof(MONSTERINFO), 0);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
			}
		}

		if (CSceneManager::Get_Instance()->Get_CurScene() == CSceneManager::SCENEID::SCENE_STAGE_NETWORK) {
			CObjManager::Get_Instance()->Update_MonsterInfo(tMonsterInfo);
		}
	}

	return retval;
}

void CClientManager::applyInfo()
{
	// �޾ƿ� ����, �� ���, ������ ����

}

void CClientManager::set_buffOn()
{
}

void CClientManager::recvInitPlayerPos()
{
	retval = recvn(sock, (char*)&tClientInfo, sizeof(CLIENTINFO), 0);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
	}


}

void CClientManager::recvInitMapTile()
{
	// ���� - ���� �̸� ũ��
	retval = recvn(sock, (char*)&iNameLen, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
	}

	char* pName = new char[iNameLen + 1];

	// ���� - ���� �̸�
	retval = recvn(sock, &pName[0], iNameLen, 0);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
	}
	pName[iNameLen] = '\0';


	retval = recvn(sock, (char*)&iFileSize, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
	}

	char* fileD = new char[iFileSize];

	retval = recvn(sock, &fileD[0], iFileSize, 0);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
	}

	std::ofstream    out{ pName, std::ios::out };
	out.write(&fileD[0], iFileSize);

	strcat_s(buf, pName);
	CTileManager::Get_Instance()->Set_DataFile(buf, strlen(buf));
}

void CClientManager::recvInitMonster()
{
	int iNum = 0;
	retval = recvn(sock, (char*)&iNum, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
	}

	tMonsterInfo.resize(iNum);

	retval = recvn(sock, (char*)&tMonsterInfo[0], sizeof(MONSTERINFO), 0);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
	}
	retval = recvn(sock, (char*)&tMonsterInfo[1], sizeof(MONSTERINFO), 0);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
	}
	retval = recvn(sock, (char*)&tMonsterInfo[2], sizeof(MONSTERINFO), 0);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
	}
	retval = recvn(sock, (char*)&tMonsterInfo[3], sizeof(MONSTERINFO), 0);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
	}
	retval = recvn(sock, (char*)&tMonsterInfo[4], sizeof(MONSTERINFO), 0);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
	}
	retval = recvn(sock, (char*)&tMonsterInfo[5], sizeof(MONSTERINFO), 0);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
	}
	retval = recvn(sock, (char*)&tMonsterInfo[6], sizeof(MONSTERINFO), 0);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
	}
	retval = recvn(sock, (char*)&tMonsterInfo[7], sizeof(MONSTERINFO), 0);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
	}
}

void CClientManager::InitMonster()
{
	for (int i = 0; i < tMonsterInfo.size(); ++i) {
		CObjManager::Get_Instance()->Add_Monster(tMonsterInfo[i], i);
	}
}

void CClientManager::setPlayerInfo()
{
	// ��ġ ����
	tClientInfo.PlayerInfo.PlayerPos.fX = CObjManager::Get_Instance()->Get_PlayerX();
	tClientInfo.PlayerInfo.PlayerPos.fY = CObjManager::Get_Instance()->Get_PlayerY();

	// ���� ����
	tClientInfo.PlayerInfo.PlayerDir = CObjManager::Get_Instance()->Get_PlayerDir();

	// ��ź ��ġ ����
	tClientInfo.BombPos.fX = CObjManager::Get_Instance()->Get_BombX();
	tClientInfo.BombPos.fY = CObjManager::Get_Instance()->Get_BombY();
}

void CClientManager::setPlayerPosToClientInfo(float fX, float fY)
{
	tClientInfo.PlayerInfo.PlayerPos.fX = fX;
	tClientInfo.PlayerInfo.PlayerPos.fY = fY;
}

void CClientManager::recvIsGameStart()
{
	recvn(sock, (char*)&bisAnoterPlayerGameStart, sizeof(bool), 0);
}

void CClientManager::err_quit(char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCWSTR)msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

void CClientManager::err_display(char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

int CClientManager::recvn(SOCKET s, char* buf, int len, int flags)
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

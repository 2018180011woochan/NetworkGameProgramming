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
	// 윈속을 초기화한 후, 서버와 통신을 하기 위한 소켓을 생성하고 서버에게 접속을 요청하는 함수
	// 리턴값: retval - connect()의 리턴값

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket 생성
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	
	// connect() - 서버에 접속 요청
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

	
}

int CClientManager::sendInfo()
{
	// 서버에 잘 전송됐는지 시험해보기 위해 (성공 후 삭제할 것)
	retval = send(sock, (char*)&tClientInfo, sizeof(CLIENTINFO), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}

	int iStart = -1;
	if (bisStart)	iStart = 1;
	else	iStart = 2;
	// 게임 시작했는지 확인
	retval = send(sock, (char*)&iStart, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}

	// 변화된 몬스터 정보 전송
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
	// 서버로부터 모든 클라이언트의 
	// 플레이어 정보, 아이템 정보, 몬스터 정보를 담고 있는 
	// WorldInfo 맵 컨테이너를 받는다.

	// 몇 개의 ClientInfo가 있는지 알아야 한다.
	retval = recvn(sock, (char*)&tClientInfo, sizeof(CLIENTINFO), 0);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
	}
	
	// 서버로부터 받을 몬스터 개수
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
	//CObjManager::Get_Instance()->Set_PlayerX(tClientInfo.PlayerInfo.PlayerPos.fX);
	//CObjManager::Get_Instance()->Set_PlayerX(tClientInfo.PlayerInfo.PlayerPos.fY);

	return retval;
}

void CClientManager::applyInfo()
{
	// 받아온 몬스터, 맵 블록, 아이템 적용

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
	// 고정 - 파일 이름 크기
	retval = recvn(sock, (char*)&iNameLen, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
	}
	
	char* pName = new char[iNameLen+1];

	// 가변 - 파일 이름
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
	retval = recvn(sock, (char*)&iMonsterCnt, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
	}

	tMonsterInfo.resize(iMonsterCnt);

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
	tClientInfo.PlayerInfo.PlayerPos.fX = CObjManager::Get_Instance()->Get_PlayerX();
	tClientInfo.PlayerInfo.PlayerPos.fY = CObjManager::Get_Instance()->Get_PlayerY();
}

void CClientManager::setPlayerPosToClientInfo(float fX, float fY)
{
	tClientInfo.PlayerInfo.PlayerPos.fX = fX;
	tClientInfo.PlayerInfo.PlayerPos.fY = fY;
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

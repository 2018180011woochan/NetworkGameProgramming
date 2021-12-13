#include "stdafx.h"
#include "ClientManager.h"
#include "TileManager.h"
#include "ObjManager.h"
#include "Player.h"
#include "SceneManager.h"
#include "TimeManager.h"


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
	//int	option = TRUE;	// Nagle On
	//setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char*)&option, sizeof(option));
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
	tClientInfo.ClientID = iClientID;
}

int CClientManager::sendInfo()
{
	//ZeroMemory(&tClientInfo, sizeof(CLIENTINFO));
	// ClientID에 따라서 캐릭터를 정해서 서버로 보냄
	// ClientID : 0 -> 배찌,	ClientID : 1 -> 다오,	ClientID : 2 -> 디즈니

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

	// 플레이어 위치 tClientInfo에 저장
	tClientInfo.BombPos.fX = 0.f;
	tClientInfo.BombPos.fY = 0.f;
	setPlayerInfo();

	// 왠지 모르겠는데 ClientID가 초기화됨; 다시 설정해줌
	tClientInfo.ClientID = iClientID;

	// 서버에 잘 전송됐는지 시험해보기 위해 (성공 후 삭제할 것)
	retval = send(sock, (char*)&tClientInfo, sizeof(CLIENTINFO), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}

	// 변화된 타일 정보 전송
	vector<int>	vecTileKey = CTileManager::Get_Instance()->Get_vecCollTileKey();
	int	nSize = vecTileKey.size();
	retval = send(sock, (char*)&nSize, sizeof(int), 0);

	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}

	if (nSize > 0) {
		for (int i = 0; i < nSize; ++i) {
			retval = send(sock, (char*)&vecTileKey[i], sizeof(int), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
			}
		}
	}
	CTileManager::Get_Instance()->Clear_vecCollTileKey();


	return retval;
}

int CClientManager::recvInfo()
{
	// 서버로부터 모든 클라이언트의 
	// 플레이어 정보, 아이템 정보, 몬스터 정보를 담고 있는 
	// WorldInfo 맵 컨테이너를 받는다.

	//AllClientNum = tClientInfo.ClientID_Number;	// 총 접속한 클라이언트의 개수

	//if (AllClientNum == 0)
	//	AllClientNum = 1;

	//
	retval = recvn(sock, (char*)&AllClientNum, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
	}
	//


	for (int i = 0; i < AllClientNum; ++i)
	{
		ZeroMemory(&tClientInfo, sizeof(CLIENTINFO));
		retval = recvn(sock, (char*)&tClientInfo, sizeof(CLIENTINFO), 0);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
		}

		//cout << "접속한 클라이언트 수 : " << AllClientNum << endl;
		//cout << "Player" << tClientInfo.ClientID << "-> X:" << tClientInfo.PlayerInfo.PlayerPos.fX
		//	<< " Y:" << tClientInfo.PlayerInfo.PlayerPos.fY << endl;
		//cout << "-------------------------------------------" << endl;

		tWorldInfo.insert({ tClientInfo.ClientID, tClientInfo });
		tWorldInfo[tClientInfo.ClientID] = tClientInfo;

		// 접속한 다른 클라이언트를 objManager를 이용하여 obj목록에 저장
		if (iClientID != tClientInfo.ClientID)
		{
			auto iter = find(vecClientID.begin(), vecClientID.end(), tClientInfo.ClientID);
			// 처음에는 리스트에 추가하고 저장
			if (iter == vecClientID.end())
			{
				CObjManager::Get_Instance()->Add_NetWorkPlayer(tClientInfo);
				vecClientID.push_back(tClientInfo.ClientID);
			}
			// 두번째부턴 플레이어 정보 업데이트
			else
			{
				CObjManager::Get_Instance()->Update_NetWorkPlayer(tClientInfo);
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// #. 맵 블럭

	// - 상태가 바뀔 타일
	vector<int>	vecDeadTileKey;
	int	nTileNum = -1;
	retval = recvn(sock, (char*)&nTileNum, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
	}

	if (nTileNum > 0) {
		vecDeadTileKey.resize(nTileNum);

		for (int i = 0; i < nTileNum; ++i) {
			retval = recvn(sock, (char*)&vecDeadTileKey[i], sizeof(int), 0);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
			}
			CObjManager::Get_Instance()->Set_BlockBubble(vecDeadTileKey[i]);
		}
	}

	// - 새로 생길 아이템
	vector<ITEMINFO>	vecItem;
	int	nItemNum = -1;
	retval = recvn(sock, (char*)&nItemNum, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
	}

	if (nItemNum > 0) {
		vecItem.resize(nItemNum);

		for (int i = 0; i < nItemNum; ++i) {
			retval = recvn(sock, (char*)&vecItem[i], sizeof(ITEMINFO), 0);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
			}
			CObjManager::Get_Instance()->Make_Add_Item(vecItem[i]);
		}
	}
	if (nTileNum > 0) {
		//vecDeadTileKey.resize(nTileNum);

		for (int i = 0; i < nTileNum; ++i) {
			CObjManager::Get_Instance()->Add_NoItemBlock(vecDeadTileKey[i]);
		}
	}


	return retval;
}

void CClientManager::applyInfo()
{
	// 받아온 몬스터, 맵 블록, 아이템 적용

}

void CClientManager::set_buffOn()
{
	if (tClientInfo.PlayerInfo.b_isContactPlayer) {
		isBuff = true;
		if (isInit) {
			OriginalBombPower = CObjManager::Get_Instance()->Get_Player()->Get_Info().iBombPower;
			OriginalSpeed = CObjManager::Get_Instance()->Get_Player()->Get_Info().fSpeed;
			isInit = false;
		}
	}

	if (isBuff) {
		dBuffTime += CTimeManager::Get_Instance()->Get_DeltaTime();

		if (dBuffTime > 1.0 && dBuffTime <= 5.0) {
			// 버프 on - 스피드, 물줄기 최대치
			dynamic_cast<CPlayer*>(CObjManager::Get_Instance()->Get_Player())->Set_PlayerSpeed(5.f);    // 최고속도 몰라서 임시로 넣어둠
			dynamic_cast<CPlayer*>(CObjManager::Get_Instance()->Get_Player())->Set_PlayerBombMax();
		}

		else if (dBuffTime > 5.0) {
			dynamic_cast<CPlayer*>(CObjManager::Get_Instance()->Get_Player())->SetBombPower(OriginalBombPower);
			dynamic_cast<CPlayer*>(CObjManager::Get_Instance()->Get_Player())->Set_PlayerSpeed(3.f);

			isBuff = false;
			isInit = true;
			tClientInfo.PlayerInfo.b_isContactPlayer = false;
			dBuffTime = 0.0;
		}
	}


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

	char* pName = new char[iNameLen + 1];

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

void CClientManager::setPlayerInfo()
{
	// 위치 저장
	tClientInfo.PlayerInfo.PlayerPos.fX = CObjManager::Get_Instance()->Get_PlayerX();
	tClientInfo.PlayerInfo.PlayerPos.fY = CObjManager::Get_Instance()->Get_PlayerY();

	// 방향 저장
	tClientInfo.PlayerInfo.PlayerDir = CObjManager::Get_Instance()->Get_PlayerDir();

	// 폭탄 위치 저장
	tClientInfo.BombPos.fX = CObjManager::Get_Instance()->Get_BombX();
	tClientInfo.BombPos.fY = CObjManager::Get_Instance()->Get_BombY();


	// 플레이어 상태 저장
	tClientInfo.PlayerInfo.PlayerState = CObjManager::Get_Instance()->Get_PlayerState();

	// 플레이어 크기 저장
	//tClientInfo.PlayerInfo.PlayerSize = CObjManager::Get_Instance()->Get_PlayerSize();

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

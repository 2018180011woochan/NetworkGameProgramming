#pragma once
#include "Obj.h"
class CClientManager
{
public:
	CClientManager();
	~CClientManager();

public:
	static CClientManager* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CClientManager;

		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

public:
	int		connectToServer();
	void	recvClientID();

	int		sendInfo();
	int		recvInfo();
	void	applyInfo();

	void	set_buffOn();

	void	recvInitPlayerPos();
	void	recvInitMapTile();

public:
	void	setPlayerInfo();		// 네트워크 통신용 플레이어 위치 전송
	void	setPlayerPosToClientInfo(float fX, float fY);
	void	setGameStart() { bisStart = true; }
	void	setPlayerName(CHARNAME::NAME _CharName) { tClientInfo.PlayerInfo.PlayerName = _CharName; } // WaitingRoom에서 캐릭터 정해줌
	void	recvIsGameStart();

	int				GetClientID() { return iClientID; }		// 클라이언트 아이디를 반환
	vector<CObj*>	Get_MapTile() { return vecMapTile; }
	int				GetAllClientNum() { return AllClientNum; }

	map<int, CLIENTINFO> GetWorldInfo() { return tWorldInfo; }

private:
	void	err_quit(char* msg);		// 소켓 함수 오류 출력 후 종료
	void	err_display(char* msg);		// 소켓 함수 오류 출력

	int		recvn(SOCKET s, char* buf, int len, int flags);

private:
	static CClientManager* m_pInstance;
	
	// TCP 클라이언트 함수의 리턴값을 확인하기 위한 변수
	int			retval = 0;
	// 서버와 통신할 때 사용할 소켓
	SOCKET		sock;
	// ClientID
	int			iClientID = -10;

	PLAYERINFO	tPlayerInfo;	// 플레이어
	ITEMINFO	tItemInfo;		// 아이템
	//MONSTERINFO	tMonsterInfo;	// 몬스터
	CLIENTINFO	tClientInfo;	// 클라이언트 정보

	bool		isBuff = false;	// 버프 효과 판단
	bool		isInit = false;
	double		dBuffTime = 0.0;	// 버프 지속 시간
	int			OriginalBombPower;
	float		OriginalSpeed;

	// 서버로부터 받는 패킷 
	// - 플레이어, 상대 플레이어, 몬스터, 아이템 정보가 모두 들어있음
	map<int, CLIENTINFO>	tWorldInfo;

	// 서버로부터 받은 맵 정보
	vector<CObj*> vecMapTile;
	char name[100];

	int iNameLen = 0;
	int iFileSize = 0;
	char buf[900] = "../111A_API_GameProject/";

	bool bisStart = false;		// 게임 시작했는가
	bool bisAnoterPlayerGameStart = false;
	int AllClientNum = 0;		// 현재 접속한 모든 클라이언트의 수

	vector<int> vecClientID;
};


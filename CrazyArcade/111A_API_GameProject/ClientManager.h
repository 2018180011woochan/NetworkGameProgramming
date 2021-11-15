#pragma once
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

private:
	static CClientManager* m_pInstance;
	
	// TCP 클라이언트 함수의 리턴값을 확인하기 위한 변수
	int			retval = 0;
	// 서버와 통신할 때 사용할 소켓
	SOCKET		sock;

	PLAYERINFO	tPlayerInfo;	// 플레이어
	ITEMINFO	tItemInfo;		// 아이템
	MONSTERINFO	tMonsterInfo;	// 몬스터
	CLIENTINFO	tClientInfo;	// 클라이언트 정보

	bool		isBuff = false;	// 버프 효과 판단
	DWORD		dwBuffTime = 0;	// 버프 지속 시간
};


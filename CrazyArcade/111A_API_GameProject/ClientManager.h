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

public:
	void	setPlayerInfo(const INFO& tPInfo);

	int		GetClientID() { return iClientID; }		// Ŭ���̾�Ʈ ���̵� ��ȯ

private:
	void	err_quit(char* msg);		// ���� �Լ� ���� ��� �� ����
	void	err_display(char* msg);		// ���� �Լ� ���� ���

	int		recvn(SOCKET s, char* buf, int len, int flags);

private:
	static CClientManager* m_pInstance;
	
	// TCP Ŭ���̾�Ʈ �Լ��� ���ϰ��� Ȯ���ϱ� ���� ����
	int			retval = 0;
	// ������ ����� �� ����� ����
	SOCKET		sock;
	// ClientID
	int			iClientID = -10;

	PLAYERINFO	tPlayerInfo;	// �÷��̾�
	ITEMINFO	tItemInfo;		// ������
	MONSTERINFO	tMonsterInfo;	// ����
	CLIENTINFO	tClientInfo;	// Ŭ���̾�Ʈ ����

	bool		isBuff = false;	// ���� ȿ�� �Ǵ�
	DWORD		dwBuffTime = 0;	// ���� ���� �ð�
};


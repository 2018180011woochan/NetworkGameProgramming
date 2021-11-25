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
	void	setPlayerInfo(const INFO& tPInfo);

	int		GetClientID() { return iClientID; }		// Ŭ���̾�Ʈ ���̵� ��ȯ
	vector<CObj*> Get_MapTile() { return vecMapTile; }

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

	// �����κ��� �޴� ��Ŷ 
	// - �÷��̾�, ��� �÷��̾�, ����, ������ ������ ��� �������
	map<int, CLIENTINFO>	tWorldInfo;

	// �����κ��� ���� �� ����
	vector<CObj*> vecMapTile;
	char name[100];

	int iNameLen = 0;
	int iFileSize = 0;
	char buf[100] = "../111A_API_GameProject/";
};


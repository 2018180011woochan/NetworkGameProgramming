#include "stdafx.h"
#include "ClientManager.h"

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000

CClientManager* CClientManager::m_pInstance = nullptr;

CClientManager::CClientManager()
{
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

	int iMapTileSize = 0;

	retval = recvn(sock, (char*)&iMapTileSize, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		exit(1);
	}

	retval = recvn(sock, (char*)&vecMapTile, sizeof(iMapTileSize), 0);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
	}
}

int CClientManager::sendInfo()
{
	ZeroMemory(&tClientInfo, sizeof(tClientInfo));

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
	}

	// ������ �� ���۵ƴ��� �����غ��� ���� (���� �� ������ ��)
	retval = send(sock, (char*)&tClientInfo, sizeof(tClientInfo), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}
	return retval;
}

int CClientManager::recvInfo()
{
	// �����κ��� ��� Ŭ���̾�Ʈ�� 
	// �÷��̾� ����, ������ ����, ���� ������ ��� �ִ� 
	// WorldInfo �� �����̳ʸ� �޴´�.

	// �� ���� ClientInfo�� �ִ��� �˾ƾ� �Ѵ�.
	retval = recvn(sock, (char*)&tClientInfo, sizeof(CLIENTINFO), 0);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
	}

	return retval;
}

void CClientManager::applyInfo()
{
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
	int iMapTileSize = 0;

	retval = recvn(sock, (char*)&iMapTileSize, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		exit(1);
	}

	retval = recvn(sock, (char*)&vecMapTile, sizeof(iMapTileSize), 0);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
	}
}

void CClientManager::setPlayerInfo(const INFO& tPInfo)
{
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

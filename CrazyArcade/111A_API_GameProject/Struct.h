#pragma once
#ifndef __STRUCT_H__
#define __STRUCT_H__

typedef struct tagInfo
{
	float		fX;
	float		fY;
	int			iCX;
	int			iCY;

	int			iHP;

	int			iBombPower;
	int			iMaxBomb;

	float		fSpeed;
	bool		bisDead;
}INFO;

/////////////////// ��Ʈ��ũ ������ ���� ��Ŷ ���� ///////////////////////////
typedef struct ObjPos
{
	float		fX;							// �÷��̾��� x��ġ ����
	float		fY;							// �÷��̾��� y��ġ ����
}OBJPOS;

typedef struct PlayerInfo
{
	CHARNAME::NAME		PlayerName;			// �÷��̾� �̸�(����,�ٿ�,���)
	OBJDIR::DIR			PlayerDir;			// �÷��̾��� ���� ����
	OBJPOS				PlayerPos;			// �÷��̾��� ��ġ
	FRAME				PlayerFrame;		// �÷��̾� ���� ������
	bool				b_isContactPlayer;	// �÷��̾ ��� �浹�ߴ°�? �������� �Ǵ�
}PLAYERINFO;

typedef struct ItemInfo
{
	GAMEITEM::ITEM		ItemName;			// �������� �̸�(����, ��ź, ������Ʈ)
	OBJPOS				ItemPos;			// �������� ��ġ
}ITEMINFO;

typedef struct MonsterInfo
{
	MONSTERNAME::NAME	MonsterName;		// ������ �̸�(��, �Ǿ�, ����)
	OBJPOS				MonsterPos;			// ������ ��ġ
}MONSTERINFO;

typedef struct ClientInfo					// ������ ������ �� Ŭ���̾�Ʈ�� ����
{
	PLAYERINFO			PlayerInfo;			// �÷��̾��� ����(�̸�, ����, ��ġ
	ITEMINFO			ItemInfo[10];		// �������� ����(�̸�, ��ġ) MAX=10
	MONSTERINFO			MonsterInfo[10];	// ������ ����(�̸�, ��ġ) MAX=10	
	OBJPOS				BombPos[10];		// ��ź�� ����(��ġ)		 MAX=10
}CLIENTINFO;
////////////////////////////////////////////////////////////////////////////

typedef struct tagLinePos
{
	tagLinePos() { ZeroMemory(this, sizeof(tagLinePos)); }
	tagLinePos(float _x, float _y) : fX(_x), fY(_y) {}

	float		fX;
	float		fY;
}LINEPOS;

typedef struct tagLineInfo
{
	tagLineInfo() { ZeroMemory(this, sizeof(tagLineInfo)); }
	tagLineInfo(LINEPOS& _tLeft, LINEPOS& _tRight) : tLeftPos(_tLeft), tRightPos(_tRight) {}

	LINEPOS tLeftPos;
	LINEPOS tRightPos;
}LINEINFO;

typedef struct tagFrame
{
	int		iFrameStart;
	int		iFrameEnd;
	int		iFrameScene;
	DWORD	dwFrameTime;
	DWORD	dwFrameSpeed;
}FRAME;
#endif // !__STRUCT_H__

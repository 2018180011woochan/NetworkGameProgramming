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
	//FRAME				PlayerFrame;		// �÷��̾� ���� ������
	RECT				PlayerSize;			// �÷��̾� ������(ũ��)
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

#endif // !__STRUCT_H__

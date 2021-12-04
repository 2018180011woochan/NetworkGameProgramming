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

/////////////////// 네트워크 구현을 위한 패킷 정보 ///////////////////////////
typedef struct ObjPos
{
	float		fX;							// 플레이어의 x위치 정보
	float		fY;							// 플레이어의 y위치 정보
}OBJPOS;

typedef struct PlayerInfo
{
	CHARNAME::NAME		PlayerName;			// 플레이어 이름(배찌,다오,우니)
	OBJDIR::DIR			PlayerDir;			// 플레이어의 방향 정보
	OBJPOS				PlayerPos;			// 플레이어의 위치
	//FRAME				PlayerFrame;		// 플레이어 현재 프레임
	RECT				PlayerSize;			// 플레이어 사이즈(크기)
	bool				b_isContactPlayer;	// 플레이어가 모두 충돌했는가? 서버에서 판단
}PLAYERINFO;

typedef struct ItemInfo
{
	GAMEITEM::ITEM		ItemName;			// 아이템의 이름(포션, 폭탄, 스케이트)
	OBJPOS				ItemPos;			// 아이템의 위치
}ITEMINFO;

typedef struct tagFrame
{
	int		iFrameStart;
	int		iFrameEnd;
	int		iFrameScene;
	DWORD	dwFrameTime;
	DWORD	dwFrameSpeed;
}FRAME;

typedef struct MonsterInfo
{
	MONSTERNAME::NAME	MonsterName;		// 몬스터의 이름(귤, 악어, 물개)
	OBJPOS				MonsterPos;			// 몬스터의 위치
	OBJDIR::DIR			MonsterDir;			// 몬스터 방향
	int					MonsterID;
	FRAME				Monsterframe;
	bool				MonsterDead;
}MONSTERINFO;



typedef struct ClientInfo					// 서버로 전송할 각 클라이언트의 정보
{
	PLAYERINFO			PlayerInfo;			// 플레이어의 정보(이름, 방향, 위치
	ITEMINFO			ItemInfo[10];		// 아이템의 정보(이름, 위치) MAX=10
	MONSTERINFO			MonsterInfo[10];	// 몬스터의 정보(이름, 위치) MAX=10	
	OBJPOS				BombPos[10];		// 폭탄의 정보(위치)		 MAX=10
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


#endif // !__STRUCT_H__

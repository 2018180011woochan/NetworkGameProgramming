#include "stdafx.h"
#include "Stage_Boss.h"
#include "Player.h"
#include "BmpManager.h"
#include "ObjManager.h"
#include "TileManager.h"
#include "Basket.h"
#include "Ballon.h"
#include "Potion.h"
#include "Skate.h"
#include "MaxPotion.h"
#include "Rock1.h"
#include "Rock2.h"
#include "Rock3.h"
#include "Leaf1.h"
#include "Leaf2.h"
#include "Flower.h"
#include "Grass.h"
#include "Messi.h"
#include "Mbape.h"
#include "HMSon.h"
#include "SoundMgr.h"
#include "SceneManager.h"
#include "Bazzi.h"
#include "Dao.h"
#include "Digenie.h"
#include "Uni.h"
#include "KeyManager.h"

CStage_Boss::CStage_Boss()
{
}


CStage_Boss::~CStage_Boss()
{
	Release();
}

void CStage_Boss::Initialize()
{

	CSoundMgr::Get_Instance()->PlaySound(L"game_start.mp3", CSoundMgr::GAMESTART);
	//CSoundMgr::Get_Instance()->PlaySound(L"boss_1.wav", CSoundMgr::STAGE_BOSS);
	CSoundMgr::Get_Instance()->PlayBGM(L"boss_1.wav");

	//배경
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Map/Stage.bmp", L"Stage");
	//타일
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Map/forest/tile/tile1.bmp", L"Tile1");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Map/forest/tile/tile2.bmp", L"Tile2");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Map/forest/tile/tile3.bmp", L"Tile3");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Map/forest/tile/tile4.bmp", L"Tile4");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Map/forest/tile/tile5.bmp", L"Tile5");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Map/forest/tile/tile6.bmp", L"Tile6");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Map/forest/tile/tile7.bmp", L"Tile7");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Map/forest/tile/tile8.bmp", L"Tile8");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Map/forest/tile/tile9.bmp", L"Tile9");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Map/forest/tile/tile10.bmp", L"Tile10");
	//블록
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Map/forest/block/Basket.bmp", L"Basket");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Map/forest/block/Basket_pop.bmp", L"BasketPop");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Map/forest/block/Leaf1.bmp", L"Leaf1");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Map/forest/block/Leaf2.bmp", L"Leaf2");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Map/forest/block/Flower.bmp", L"Flower");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Map/forest/block/Grass.bmp", L"Grass");

	//오브젝트 블록
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Map/forest/object/Rock1.bmp", L"Rock1");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Map/forest/object/Rock2.bmp", L"Rock2");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Map/forest/object/Rock3.bmp", L"Rock3");

	//아이템
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Obj/Item/Ballon.bmp", L"Ballon");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Obj/Item/skate.bmp", L"Skate");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Obj/Item/Potion.bmp", L"Potion");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Obj/Item/MaxPotion.bmp", L"MaxPotion");

	CTileManager::Get_Instance()->Load_Tile();
	CObjManager::Get_Instance()->Load_Object_Boss();

	//플레이어 생성
	CObj* pObj = nullptr;
	if (CSceneManager::Get_Instance()->Get_Char() == CHARNAME::BAZZI)
	{
		CObj* pObj = CAbstractFactory<CBazzi>::Create(MAPSTARTX + (TILECX * 13) + (TILECX >> 1), MAPSTARTY + (TILECY * 11) + (TILECY >> 1));
		CObjManager::Get_Instance()->Add_Object(pObj, OBJID::PLAYER);
	}
	if (CSceneManager::Get_Instance()->Get_Char() == CHARNAME::DAO)
	{
		CObj* pObj = CAbstractFactory<CDao>::Create(MAPSTARTX + (TILECX * 13) + (TILECX >> 1), MAPSTARTY + (TILECY * 11) + (TILECY >> 1));
		CObjManager::Get_Instance()->Add_Object(pObj, OBJID::PLAYER);
	}
	if (CSceneManager::Get_Instance()->Get_Char() == CHARNAME::DIGENIE)
	{
		CObj* pObj = CAbstractFactory<CDigenie>::Create(MAPSTARTX + (TILECX * 13) + (TILECX >> 1), MAPSTARTY + (TILECY * 11) + (TILECY >> 1));
		CObjManager::Get_Instance()->Add_Object(pObj, OBJID::PLAYER);
	}
	if (CSceneManager::Get_Instance()->Get_Char() == CHARNAME::UNI)
	{
		CObj* pObj = CAbstractFactory<CUni>::Create(MAPSTARTX + (TILECX * 13) + (TILECX >> 1), MAPSTARTY + (TILECY * 11) + (TILECY >> 1));
		CObjManager::Get_Instance()->Add_Object(pObj, OBJID::PLAYER);
	}

	//몬스터 생성
	pObj = CAbstractFactory<CMessi>::Create_Monster(MAPSTARTX + (TILECX * 1) + (TILECX >> 1), MAPSTARTY + (TILECY * 1) + (TILECY >> 1), OBJDIR::RIGHT);
	CObjManager::Get_Instance()->Add_Object(pObj, OBJID::MONSTER);

	pObj = CAbstractFactory<CMbape>::Create_Monster(MAPSTARTX + (TILECX * 2) + (TILECX >> 1), MAPSTARTY + (TILECY * 2) + (TILECY >> 1), OBJDIR::LEFT);
	CObjManager::Get_Instance()->Add_Object(pObj, OBJID::MONSTER);

	pObj = CAbstractFactory<CMessi>::Create_Monster(MAPSTARTX + (TILECX * 13) + (TILECX >> 1), MAPSTARTY + (TILECY * 1) + (TILECY >> 1), OBJDIR::LEFT);
	CObjManager::Get_Instance()->Add_Object(pObj, OBJID::MONSTER);

	pObj = CAbstractFactory<CMbape>::Create_Monster(MAPSTARTX + (TILECX * 12) + (TILECX >> 1), MAPSTARTY + (TILECY * 2) + (TILECY >> 1), OBJDIR::TOP);
	CObjManager::Get_Instance()->Add_Object(pObj, OBJID::MONSTER);

	// 보스 생성
	pObj = CAbstractFactory<CHMSon>::Create_Monster(MAPSTARTX + (TILECX * 7) + (TILECX >> 1), MAPSTARTY + (TILECY * 1) + (TILECY >> 1), OBJDIR::BOTTOM);
	CObjManager::Get_Instance()->Add_Object(pObj, OBJID::BOSS);

}

void CStage_Boss::Update()
{
	CObjManager::Get_Instance()->Update();

	if (CKeyManager::Get_Instance()->Key_Down('C'))
		CObjManager::Get_Instance()->Set_Cheat(false);




	if (CObjManager::Get_Instance()->Get_Stage1Clear() >= 18)
	{
		CSoundMgr::Get_Instance()->StopAll();
		CSoundMgr::Get_Instance()->PlaySound(L"win.mp3", CSoundMgr::PLAYER_WIN);
		CObjManager::Get_Instance()->Release();
		CSceneManager::Get_Instance()->Scene_Change(CSceneManager::SCENE_MENU);
	}
}

void CStage_Boss::Late_update()
{
	CObjManager::Get_Instance()->Late_Update();
}

void CStage_Boss::Render(HDC _DC)
{
	HDC hMemDC = CBmpManager::Get_Instance()->Find_Image(L"Stage");
	BitBlt(_DC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);

	CTileManager::Get_Instance()->Render(_DC);
	CObjManager::Get_Instance()->Render(_DC);
}

void CStage_Boss::Release()
{
}

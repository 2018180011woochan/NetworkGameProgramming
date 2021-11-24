#include "stdafx.h"
#include "Stage1.h"
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

CStage1::CStage1()
{
}


CStage1::~CStage1()
{
	Release();
}

void CStage1::Initialize()
{
	CSoundMgr::Get_Instance()->PlaySound(L"game_start.mp3", CSoundMgr::GAMESTART);
	CSoundMgr::Get_Instance()->PlaySound(L"Stage.mp3", CSoundMgr::STAGE1);

	//���
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Map/Stage.bmp", L"Stage");
	//Ÿ��
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
	//���
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Map/forest/block/Basket.bmp", L"Basket");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Map/forest/block/Basket_pop.bmp", L"BasketPop");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Map/forest/block/Leaf1.bmp", L"Leaf1");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Map/forest/block/Leaf2.bmp", L"Leaf2");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Map/forest/block/Flower.bmp", L"Flower");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Map/forest/block/Grass.bmp", L"Grass");

	//������Ʈ ���
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Map/forest/object/Rock1.bmp", L"Rock1");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Map/forest/object/Rock2.bmp", L"Rock2");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Map/forest/object/Rock3.bmp", L"Rock3");

	//������
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Obj/Item/Ballon.bmp", L"Ballon");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Obj/Item/skate.bmp", L"Skate");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Obj/Item/Potion.bmp", L"Potion");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Obj/Item/MaxPotion.bmp", L"MaxPotion");

	//CTileManager::Get_Instance()->Load_Tile();
	CTileManager::Get_Instance()->Load_TileFromServer();
	CObjManager::Get_Instance()->Load_Object();

	//�÷��̾� ����
	CObj* pObj = nullptr;
	if (CSceneManager::Get_Instance()->Get_Char() == CHARNAME::BAZZI)
	{
		CObj* pObj = CAbstractFactory<CBazzi>::Create();
		CObjManager::Get_Instance()->Add_Object(pObj, OBJID::PLAYER);
	}
	if (CSceneManager::Get_Instance()->Get_Char() == CHARNAME::DAO)
	{
		CObj* pObj = CAbstractFactory<CDao>::Create();
		CObjManager::Get_Instance()->Add_Object(pObj, OBJID::PLAYER);
	}
	if (CSceneManager::Get_Instance()->Get_Char() == CHARNAME::DIGENIE)
	{
		CObj* pObj = CAbstractFactory<CDigenie>::Create();
		CObjManager::Get_Instance()->Add_Object(pObj, OBJID::PLAYER);
	}
	if (CSceneManager::Get_Instance()->Get_Char() == CHARNAME::UNI)
	{
		CObj* pObj = CAbstractFactory<CUni>::Create();
		CObjManager::Get_Instance()->Add_Object(pObj, OBJID::PLAYER);
	}
	
	//���� ����
	pObj = CAbstractFactory<CMessi>::Create_Monster(MAPSTARTX + (TILECX * 14) + (TILECX >> 1), MAPSTARTY + (TILECY * 0) + (TILECY >> 1), OBJDIR::LEFT);
	CObjManager::Get_Instance()->Add_Object(pObj, OBJID::MONSTER);

	pObj = CAbstractFactory<CMessi>::Create_Monster(MAPSTARTX + (TILECX * 14) + (TILECX >> 1), MAPSTARTY + (TILECY * 6) + (TILECY >> 1), OBJDIR::BOTTOM);
	CObjManager::Get_Instance()->Add_Object(pObj, OBJID::MONSTER);

	pObj = CAbstractFactory<CMessi>::Create_Monster(MAPSTARTX + (TILECX * 8) + (TILECX >> 1), MAPSTARTY + (TILECY * 12) + (TILECY >> 1), OBJDIR::LEFT);
	CObjManager::Get_Instance()->Add_Object(pObj, OBJID::MONSTER);

	pObj = CAbstractFactory<CMessi>::Create_Monster(MAPSTARTX + (TILECX * 0) + (TILECX >> 1), MAPSTARTY + (TILECY * 6) + (TILECY >> 1), OBJDIR::TOP);
	CObjManager::Get_Instance()->Add_Object(pObj, OBJID::MONSTER);

	pObj = CAbstractFactory<CMbape>::Create_Monster(MAPSTARTX + (TILECX * 4) + (TILECX >> 1), MAPSTARTY + (TILECY * 6) + (TILECY >> 1), OBJDIR::TOP);
	CObjManager::Get_Instance()->Add_Object(pObj, OBJID::MONSTER);

	pObj = CAbstractFactory<CMbape>::Create_Monster(MAPSTARTX + (TILECX * 10) + (TILECX >> 1), MAPSTARTY + (TILECY * 6) + (TILECY >> 1), OBJDIR::BOTTOM);
	CObjManager::Get_Instance()->Add_Object(pObj, OBJID::MONSTER);
}

void CStage1::Update()
{
	if (CKeyManager::Get_Instance()->Key_Down('C'))
		CObjManager::Get_Instance()->Set_Cheat(true);



	CObjManager::Get_Instance()->Update();
	if (CObjManager::Get_Instance()->Get_Stage1Clear() >= 6)
	{
		CSoundMgr::Get_Instance()->StopAll();
		CSoundMgr::Get_Instance()->PlaySound(L"win.mp3", CSoundMgr::PLAYER_WIN);
		CObjManager::Get_Instance()->Release();
		CSceneManager::Get_Instance()->Scene_Change(CSceneManager::SCENE_STAGE2);

		
	}
}

void CStage1::Late_update()
{
	CObjManager::Get_Instance()->Late_Update();
}

void CStage1::Render(HDC _DC)
{
	HDC hMemDC = CBmpManager::Get_Instance()->Find_Image(L"Stage");
	BitBlt(_DC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);

	CTileManager::Get_Instance()->Render(_DC);
	CObjManager::Get_Instance()->Render(_DC);
}

void CStage1::Release()
{

}

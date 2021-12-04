#include "stdafx.h"
#include "ObjManager.h"
#include "Obj.h"
//#include "CollidManager.h"
//#include "StageMap.h"
#include "Player.h"
//#include "Bomb.h"
//#include "TileManager.h"
//#include "Basket.h"
//#include "Leaf1.h"
//#include "Leaf2.h"
//#include "Flower.h"
//#include "Grass.h"
//#include "Rock1.h"
//#include "Rock2.h"
//#include "Rock3.h"
#include "Messi.h"
#include "Mbape.h"
//#include "HMSon.h"
//#include "BossBomb.h"
//#include "TimeManager.h"

CObjManager* CObjManager::m_pInstance = nullptr;

CObjManager::CObjManager()
	: m_bisBombPlayerCollid(false), m_iStage1Clear(0), m_BossHP(0), m_bisCheat(false)
{
}


CObjManager::~CObjManager()
{
	Release();
}

void CObjManager::Update()
{
	/*for (int i = 0; i < OBJID::END; ++i)
	{
		for (auto& iter = m_listObj[i].begin(); iter != m_listObj[i].end();)
		{
			int iEvent = (*iter)->Update();
			if (iEvent == OBJ_DEAD)
			{
				SAFE_DELETE(*iter);
				iter = m_listObj[i].erase(iter);
			}
			else
				++iter;
		}
	}
	for (int i = 0; i < MAPBLOCK::END; ++i)
	{
		for (auto& iter = m_listMapBLOCK[i].begin(); iter != m_listMapBLOCK[i].end();)
		{
			int iEvent = (*iter)->Update();
			if (iEvent == OBJ_DEAD)
			{
				SAFE_DELETE(*iter);
				iter = m_listMapBLOCK[i].erase(iter);
			}
			else
				++iter;
		}
	}

	for (int i = 0; i < GAMEITEM::END; ++i)
	{
		for (auto& iter = m_listItem[i].begin(); iter != m_listItem[i].end();)
		{
			int iEvent = (*iter)->Update();
			if (iEvent == OBJ_DEAD)
			{
				SAFE_DELETE(*iter);
				iter = m_listItem[i].erase(iter);
			}
			else
				++iter;
		}
	}*/





	//for (auto& iter = m_listObj[OBJID::BOMB].begin(); iter != m_listObj[OBJID::BOMB].end(); ++iter)
	//{
	//	if (dynamic_cast<CBomb*>(*iter)->Get_isBombCollid())
	//		CCollidManager::Collision_Rect_NoPushBombToPlayer(m_listObj[OBJID::PLAYER], m_listObj[OBJID::BOMB]);
	//}
	//CCollidManager::Collision_Rect_BombToBombWave(m_listObj[OBJID::BOMBWAVE], m_listObj[OBJID::BOMB]);


	//CCollidManager::Collision_Rect_NoPush(m_listObj[OBJID::PLAYER], m_listMapBLOCK[MAPBLOCK::BASKET]);
	//CCollidManager::Collision_Rect_NoPush(m_listObj[OBJID::PLAYER], m_listMapBLOCK[MAPBLOCK::LEAF1]);
	//CCollidManager::Collision_Rect_NoPush(m_listObj[OBJID::PLAYER], m_listMapBLOCK[MAPBLOCK::LEAF2]);
	//CCollidManager::Collision_Rect_NoPush(m_listObj[OBJID::PLAYER], m_listMapBLOCK[MAPBLOCK::FLOWER]);
	//CCollidManager::Collision_Rect_NoPush(m_listObj[OBJID::PLAYER], m_listMapBLOCK[MAPBLOCK::GRASS]);

	//CCollidManager::Collision_Rect_NoPush(m_listObj[OBJID::PLAYER], m_listMapBLOCK[MAPBLOCK::ROCK1]);
	//CCollidManager::Collision_Rect_NoPush(m_listObj[OBJID::PLAYER], m_listMapBLOCK[MAPBLOCK::ROCK2]);
	//CCollidManager::Collision_Rect_NoPush(m_listObj[OBJID::PLAYER], m_listMapBLOCK[MAPBLOCK::ROCK3]);

	//CCollidManager::Collision_Rect_NoPush(m_listObj[OBJID::MONSTER], m_listObj[OBJID::BOMB]);



	//// bombwave 충돌실험을 위해 잠시 주석
	//CCollidManager::Collision_Rect_BombWaveToBlock(m_listObj[OBJID::BOMBWAVE], m_listMapBLOCK[MAPBLOCK::BASKET]);
	//CCollidManager::Collision_Rect_BombWaveToBlock(m_listObj[OBJID::BOMBWAVE], m_listMapBLOCK[MAPBLOCK::LEAF1]);
	//CCollidManager::Collision_Rect_BombWaveToBlock(m_listObj[OBJID::BOMBWAVE], m_listMapBLOCK[MAPBLOCK::LEAF2]);
	//CCollidManager::Collision_Rect_BombWaveToBlock(m_listObj[OBJID::BOMBWAVE], m_listMapBLOCK[MAPBLOCK::FLOWER]);
	//CCollidManager::Collision_Rect_BombWaveToBlock(m_listObj[OBJID::BOMBWAVE], m_listMapBLOCK[MAPBLOCK::GRASS]);

	////CCollidManager::Collision_Rect_BombWaveToBlock(m_listObj[OBJID::BOMBWAVE], m_listObj[OBJID::MONSTER]);
	//CCollidManager::Collision_Rect_BombWaveToMonster(m_listObj[OBJID::BOMBWAVE], m_listObj[OBJID::MONSTER]);
	//CCollidManager::Collision_Rect_BombWaveToMonster(m_listObj[OBJID::BOMBWAVE], m_listObj[OBJID::BOSS]);

	/////////////////////////////////// 치트
	//if (!m_bisCheat)
	//{
	//	CCollidManager::Collision_Rect_PlayerToBoss(m_listObj[OBJID::PLAYER], m_listObj[OBJID::BOSS]);
	//	CCollidManager::Collision_Rect_PlayerToMonster(m_listObj[OBJID::PLAYER], m_listObj[OBJID::MONSTER]);
	//	CCollidManager::Collision_Rect(m_listObj[OBJID::BOMBWAVE], m_listObj[OBJID::PLAYER]);
	//}
	////////////////////////////////////////////////////////

	//CCollidManager::Collision_Rect_PlayerToBallon(m_listObj[OBJID::PLAYER], m_listItem[GAMEITEM::BALLON]);
	//CCollidManager::Collision_Rect_PlayerToSkate(m_listObj[OBJID::PLAYER], m_listItem[GAMEITEM::SKATE]);
	//CCollidManager::Collision_Rect_PlayerToPotion(m_listObj[OBJID::PLAYER], m_listItem[GAMEITEM::POTION]);
	//CCollidManager::Collision_Rect_PlayerToMaxPotion(m_listObj[OBJID::PLAYER], m_listItem[GAMEITEM::MAXPOTION]);
	
}

void CObjManager::Update_Monster()
{
	for (int i = 0; i < OBJID::END; ++i)
	{
		if (i == OBJID::MONSTER) {
			for (auto&& iter = m_listObj[i].begin(); iter != m_listObj[i].end();)
			{
				int iEvent = (*iter)->Update();
				
				if (iEvent == OBJ_DEAD)
				{
					SAFE_DELETE(*iter);
					iter = m_listObj[i].erase(iter);
				}
				else
					++iter;
			}
		}
	}

	for (int i = 0; i < OBJID::END; ++i)
	{
		if (i == OBJID::MONSTER) {
			for (auto& pObj : m_listObj[i])
				pObj->Late_Update();
		}
	}
	//CTimeManager::Get_Instance()->Update_CTimeManager();
}

void CObjManager::Late_Update()
{
	for (int i = 0; i < OBJID::END; ++i)
	{
		for (auto& pObj : m_listObj[i])
			pObj->Late_Update();
	}
	for (int i = 0; i < MAPBLOCK::END; ++i)
	{
		for (auto& pObj : m_listMapBLOCK[i])
			pObj->Late_Update();
	}
	for (int i = 0; i < GAMEITEM::END; ++i)
	{
		for (auto& pObj : m_listItem[i])
			pObj->Late_Update();
	}

}

void CObjManager::Render(HDC _DC)
{

	for (int i = 0; i < MAPBLOCK::END; ++i)
	{
		for (auto& pObj : m_listMapBLOCK[i])
		{
			if (!pObj)
				continue;
			pObj->Render(_DC);
		}
	}
	for (int i = 0; i < GAMEITEM::END; ++i)
	{
		for (auto& pObj : m_listItem[i])
			pObj->Render(_DC);
	}
	for (int i = 0; i < OBJID::END; ++i)
	{
		for (auto& pObj : m_listObj[i])
		{
			if (!pObj)
				continue;
			pObj->Render(_DC);
		}
		
	}


}

void CObjManager::Release()
{
	for (int i = 0; i < OBJID::END; ++i)
	{
		for_each(m_listObj[i].begin(), m_listObj[i].end(), Safe_Delete<CObj*>);
		m_listObj[i].clear();
	}

	for (int i = 0; i < MAPBLOCK::END; ++i)
	{
		for_each(m_listMapBLOCK[i].begin(), m_listMapBLOCK[i].end(), Safe_Delete<CObj*>);
		m_listMapBLOCK[i].clear();
	}

	for (int i = 0; i < GAMEITEM::END; ++i)
	{
		for_each(m_listItem[i].begin(), m_listItem[i].end(), Safe_Delete<CObj*>);
		m_listItem[i].clear();
	}
}

float CObjManager::Get_PlayerX()
{
	for (auto&& iter = m_listObj[OBJID::PLAYER].begin(); iter != m_listObj[OBJID::PLAYER].end(); ++iter)
	{
		return dynamic_cast<CPlayer*>(*iter)->Get_PlayerX();
	}
}

float CObjManager::Get_PlayerY()
{
	for (auto&& iter = m_listObj[OBJID::PLAYER].begin(); iter != m_listObj[OBJID::PLAYER].end(); ++iter)
	{
		return dynamic_cast<CPlayer*>(*iter)->Get_PlayerY();
	}
}

float CObjManager::Get_BombX()
{
	//for (auto& iter = m_listObj[OBJID::BOMB].begin(); iter != m_listObj[OBJID::BOMB].end(); ++iter)
	//{
	//	return dynamic_cast<CBomb*>(*iter)->Get_BombX();
	//}
	return 0;
}

float CObjManager::Get_BombY()
{
	//for (auto& iter = m_listObj[OBJID::BOMB].begin(); iter != m_listObj[OBJID::BOMB].end(); ++iter)
	//{
	//	return dynamic_cast<CBomb*>(*iter)->Get_BombY();
	//}
	return 0;
}

void CObjManager::Set_PlayerX(float fX)
{
	for (auto&& iter = m_listObj[OBJID::PLAYER].begin(); iter != m_listObj[OBJID::PLAYER].end(); ++iter)
	{
		dynamic_cast<CPlayer*>(*iter)->Set_PlayerX(fX);
	}
}

void CObjManager::Set_PlayerY(float fY)
{
	for (auto&& iter = m_listObj[OBJID::PLAYER].begin(); iter != m_listObj[OBJID::PLAYER].end(); ++iter)
	{
		dynamic_cast<CPlayer*>(*iter)->Set_PlayerY(fY);
	}
}

void CObjManager::Picking_Object(CObj * _pObj, MAPBLOCK::BLOCK _block)
{
	/*POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	int x = (pt.x - MAPSTARTX) / TILECX;
	int y = (pt.y - MAPSTARTY) / TILECY;

	switch (_block)
	{
	case MAPBLOCK::BASKET:
		_pObj = CAbstractFactory<CBasket>::Create(MAPSTARTX + (TILECX * x) + (TILECX >> 1),MAPSTARTY + (TILECY * y) + (TILECY >> 1));
		CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, MAPBLOCK::BASKET);
		CTileManager::Get_Instance()->SetTileBlockType(MAPSTARTX + (TILECX * x) + (TILECX >> 1), MAPSTARTY + (TILECY * y) + (TILECY >> 1), MAPBLOCK::BASKET);
		break;
	case MAPBLOCK::LEAF1:
		_pObj = CAbstractFactory<CLeaf1>::Create(MAPSTARTX + (TILECX * x) + (TILECX >> 1), MAPSTARTY + (TILECY * y) + (TILECY >> 1));
		CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, MAPBLOCK::LEAF1);
		CTileManager::Get_Instance()->SetTileBlockType(MAPSTARTX + (TILECX * x) + (TILECX >> 1), MAPSTARTY + (TILECY * y) + (TILECY >> 1), MAPBLOCK::LEAF1);
		break;
	case MAPBLOCK::LEAF2:
		_pObj = CAbstractFactory<CLeaf2>::Create(MAPSTARTX + (TILECX * x) + (TILECX >> 1), MAPSTARTY + (TILECY * y) + (TILECY >> 1));
		CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, MAPBLOCK::LEAF2);
		CTileManager::Get_Instance()->SetTileBlockType(MAPSTARTX + (TILECX * x) + (TILECX >> 1), MAPSTARTY + (TILECY * y) + (TILECY >> 1), MAPBLOCK::LEAF2);
		break;
	case MAPBLOCK::FLOWER:
		_pObj = CAbstractFactory<CFlower>::Create(MAPSTARTX + (TILECX * x) + (TILECX >> 1), MAPSTARTY + (TILECY * y) + (TILECY >> 1));
		CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, MAPBLOCK::FLOWER);
		CTileManager::Get_Instance()->SetTileBlockType(MAPSTARTX + (TILECX * x) + (TILECX >> 1), MAPSTARTY + (TILECY * y) + (TILECY >> 1), MAPBLOCK::FLOWER);
		break;
	case MAPBLOCK::GRASS:
		_pObj = CAbstractFactory<CGrass>::Create(MAPSTARTX + (TILECX * x) + (TILECX >> 1), MAPSTARTY + (TILECY * y) + (TILECY >> 1));
		CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, MAPBLOCK::GRASS);
		CTileManager::Get_Instance()->SetTileBlockType(MAPSTARTX + (TILECX * x) + (TILECX >> 1), MAPSTARTY + (TILECY * y) + (TILECY >> 1), MAPBLOCK::GRASS);
		break;
	case MAPBLOCK::ROCK1:
		_pObj = CAbstractFactory<Rock1>::Create(MAPSTARTX + (TILECX * x) + (TILECX >> 1), MAPSTARTY + (TILECY * y) + (TILECY >> 1));
		CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, MAPBLOCK::ROCK1);
		CTileManager::Get_Instance()->SetTileBlockType(MAPSTARTX + (TILECX * x) + (TILECX >> 1), MAPSTARTY + (TILECY * y) + (TILECY >> 1), MAPBLOCK::ROCK1);
		break;
	case MAPBLOCK::ROCK2:
		_pObj = CAbstractFactory<Rock2>::Create(MAPSTARTX + (TILECX * x) + (TILECX >> 1), MAPSTARTY + (TILECY * y) + (TILECY >> 1));
		CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, MAPBLOCK::ROCK2);
		CTileManager::Get_Instance()->SetTileBlockType(MAPSTARTX + (TILECX * x) + (TILECX >> 1), MAPSTARTY + (TILECY * y) + (TILECY >> 1), MAPBLOCK::ROCK2);
		break;
	case MAPBLOCK::ROCK3:
		_pObj = CAbstractFactory<Rock3>::Create(MAPSTARTX + (TILECX * x) + (TILECX >> 1), MAPSTARTY + (TILECY * y) + (TILECY >> 1));
		CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, MAPBLOCK::ROCK3);
		CTileManager::Get_Instance()->SetTileBlockType(MAPSTARTX + (TILECX * x) + (TILECX >> 1), MAPSTARTY + (TILECY * y) + (TILECY >> 1), MAPBLOCK::ROCK3);
		break;
	}*/
}

// 네트워크 텀프를 위한 수정
void CObjManager::Save_Object()
{
	//HANDLE hFile = CreateFile(L"../Data/Object.dat", GENERIC_WRITE,
	//	NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	//HANDLE hFile = CreateFile(L"../Data/NetworkObject.dat", GENERIC_WRITE,
	//	NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	//if (INVALID_HANDLE_VALUE == hFile)
	//{
	//	MessageBox(g_hWnd, L"오브젝트 저장 실패", L"실패", MB_OK);
	//	return;
	//}

	//DWORD dwByte = 0;

	//for (int i = 0; i < MAPBLOCK::END; ++i)
	//{
	//	for (auto& pObj : m_listMapBLOCK[i])
	//	{
	//		// 애드 오브젝트랑 타일 속성 저장
	//		WriteFile(hFile, &pObj->Get_Info(), sizeof(INFO), &dwByte, NULL);
	//		WriteFile(hFile, &pObj->Get_BlockType(), sizeof(MAPBLOCK::BLOCK), &dwByte, NULL);
	//		/*switch (i)	
	//		{
	//		case MAPBLOCK::BASKET:
	//			WriteFile(hFile, &pObj->Get_BlockType(), sizeof(MAPBLOCK::BLOCK), &dwByte, NULL);
	//			break;
	//		}*/

	//	}
	//}

	//CloseHandle(hFile);
	//MessageBox(g_hWnd, L"저장 성공", L"성공", MB_OK);
}

// 네트워크 텀프를 위한 수정
void CObjManager::Load_Object()
{
	//HANDLE hFile = CreateFile(L"../Data/NetworkObject.dat", GENERIC_READ,
	//	NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	//if (INVALID_HANDLE_VALUE == hFile)
	//{
	//	MessageBox(g_hWnd, L"오브젝트 불러오기 실패", L"실패", MB_OK);
	//	return;
	//}

	//Release();
	//CObj* _pObj = nullptr;
	//DWORD dwByte =		 0;
	//INFO tTemp =		{};
	//MAPBLOCK::BLOCK		 BlockInfo;

	//while (true)
	//{
	//	ReadFile(hFile, &tTemp, sizeof(INFO), &dwByte, NULL);
	//	ReadFile(hFile, &BlockInfo, sizeof(MAPBLOCK::BLOCK), &dwByte, NULL);

	//	if (0 == dwByte)
	//		break;

	//	switch (BlockInfo)
	//	{
	//	case MAPBLOCK::BASKET:
	//		_pObj = CAbstractFactory<CBasket>::Create(tTemp.fX, tTemp.fY);
	//		CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, BlockInfo);
	//		CTileManager::Get_Instance()->SetTileBlockType(tTemp.fX, tTemp.fY, BlockInfo);
	//		break;
	//	case MAPBLOCK::LEAF1:
	//		_pObj = CAbstractFactory<CLeaf1>::Create(tTemp.fX, tTemp.fY);
	//		CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, BlockInfo);
	//		CTileManager::Get_Instance()->SetTileBlockType(tTemp.fX, tTemp.fY, BlockInfo);
	//		break;
	//	case MAPBLOCK::LEAF2:
	//		_pObj = CAbstractFactory<CLeaf2>::Create(tTemp.fX, tTemp.fY);
	//		CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, BlockInfo);
	//		CTileManager::Get_Instance()->SetTileBlockType(tTemp.fX, tTemp.fY, BlockInfo);
	//		break;
	//	case MAPBLOCK::FLOWER:
	//		_pObj = CAbstractFactory<CFlower>::Create(tTemp.fX, tTemp.fY);	
	//		CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, BlockInfo);
	//		CTileManager::Get_Instance()->SetTileBlockType(tTemp.fX, tTemp.fY, BlockInfo);
	//		break;
	//	case MAPBLOCK::GRASS:
	//		_pObj = CAbstractFactory<CGrass>::Create(tTemp.fX, tTemp.fY);
	//		CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, BlockInfo);
	//		CTileManager::Get_Instance()->SetTileBlockType(tTemp.fX, tTemp.fY, BlockInfo);
	//		break;
	//	case MAPBLOCK::ROCK1:
	//		_pObj = CAbstractFactory<Rock1>::Create(tTemp.fX, tTemp.fY);	
	//		CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, BlockInfo);
	//		CTileManager::Get_Instance()->SetTileBlockType(tTemp.fX, tTemp.fY, BlockInfo);
	//		break;
	//	case MAPBLOCK::ROCK2:
	//		_pObj = CAbstractFactory<Rock2>::Create(tTemp.fX, tTemp.fY);
	//		CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, BlockInfo);
	//		CTileManager::Get_Instance()->SetTileBlockType(tTemp.fX, tTemp.fY, BlockInfo);
	//		break;
	//	case MAPBLOCK::ROCK3:
	//		_pObj = CAbstractFactory<Rock3>::Create(tTemp.fX, tTemp.fY);
	//		CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, BlockInfo);
	//		CTileManager::Get_Instance()->SetTileBlockType(tTemp.fX, tTemp.fY, BlockInfo);
	//		break;
	//	}


	//}

	//CloseHandle(hFile);
	////MessageBox(g_hWnd, L"오브젝트 불러오기 성공", L"성공", MB_OK);

}

void CObjManager::Save_Object_Stage2()
{
	//HANDLE hFile = CreateFile(L"../Data/Object_Stage2.dat", GENERIC_WRITE,
	//	NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	//if (INVALID_HANDLE_VALUE == hFile)
	//{
	//	MessageBox(g_hWnd, L"오브젝트 저장 실패", L"실패", MB_OK);
	//	return;
	//}

	//DWORD dwByte = 0;

	//for (int i = 0; i < MAPBLOCK::END; ++i)
	//{
	//	for (auto& pObj : m_listMapBLOCK[i])
	//	{
	//		// 애드 오브젝트랑 타일 속성 저장
	//		WriteFile(hFile, &pObj->Get_Info(), sizeof(INFO), &dwByte, NULL);
	//		WriteFile(hFile, &pObj->Get_BlockType(), sizeof(MAPBLOCK::BLOCK), &dwByte, NULL);

	//	}
	//}

	//CloseHandle(hFile);
	//MessageBox(g_hWnd, L"저장 성공", L"성공", MB_OK);
}

void CObjManager::Load_Object_Stage2()
{
	//HANDLE hFile = CreateFile(L"../Data/Object_Stage2.dat", GENERIC_READ,
	//	NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	//if (INVALID_HANDLE_VALUE == hFile)
	//{
	//	MessageBox(g_hWnd, L"오브젝트 불러오기 실패", L"실패", MB_OK);
	//	return;
	//}

	//Release();
	//CObj* _pObj = nullptr;
	//DWORD dwByte = 0;
	//INFO tTemp = {};
	//MAPBLOCK::BLOCK		 BlockInfo;

	//while (true)
	//{
	//	ReadFile(hFile, &tTemp, sizeof(INFO), &dwByte, NULL);
	//	ReadFile(hFile, &BlockInfo, sizeof(MAPBLOCK::BLOCK), &dwByte, NULL);

	//	if (0 == dwByte)
	//		break;

	//	switch (BlockInfo)
	//	{
	//	case MAPBLOCK::BASKET:
	//		_pObj = CAbstractFactory<CBasket>::Create(tTemp.fX, tTemp.fY);
	//		CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, BlockInfo);
	//		CTileManager::Get_Instance()->SetTileBlockType(tTemp.fX, tTemp.fY, BlockInfo);
	//		break;
	//	case MAPBLOCK::LEAF1:
	//		_pObj = CAbstractFactory<CLeaf1>::Create(tTemp.fX, tTemp.fY);
	//		CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, BlockInfo);
	//		CTileManager::Get_Instance()->SetTileBlockType(tTemp.fX, tTemp.fY, BlockInfo);
	//		break;
	//	case MAPBLOCK::LEAF2:
	//		_pObj = CAbstractFactory<CLeaf2>::Create(tTemp.fX, tTemp.fY);
	//		CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, BlockInfo);
	//		CTileManager::Get_Instance()->SetTileBlockType(tTemp.fX, tTemp.fY, BlockInfo);
	//		break;
	//	case MAPBLOCK::FLOWER:
	//		_pObj = CAbstractFactory<CFlower>::Create(tTemp.fX, tTemp.fY);
	//		CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, BlockInfo);
	//		CTileManager::Get_Instance()->SetTileBlockType(tTemp.fX, tTemp.fY, BlockInfo);
	//		break;
	//	case MAPBLOCK::GRASS:
	//		_pObj = CAbstractFactory<CGrass>::Create(tTemp.fX, tTemp.fY);
	//		CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, BlockInfo);
	//		CTileManager::Get_Instance()->SetTileBlockType(tTemp.fX, tTemp.fY, BlockInfo);
	//		break;
	//	case MAPBLOCK::ROCK1:
	//		_pObj = CAbstractFactory<Rock1>::Create(tTemp.fX, tTemp.fY);
	//		CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, BlockInfo);
	//		CTileManager::Get_Instance()->SetTileBlockType(tTemp.fX, tTemp.fY, BlockInfo);
	//		break;
	//	case MAPBLOCK::ROCK2:
	//		_pObj = CAbstractFactory<Rock2>::Create(tTemp.fX, tTemp.fY);
	//		CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, BlockInfo);
	//		CTileManager::Get_Instance()->SetTileBlockType(tTemp.fX, tTemp.fY, BlockInfo);
	//		break;
	//	case MAPBLOCK::ROCK3:
	//		_pObj = CAbstractFactory<Rock3>::Create(tTemp.fX, tTemp.fY);
	//		CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, BlockInfo);
	//		CTileManager::Get_Instance()->SetTileBlockType(tTemp.fX, tTemp.fY, BlockInfo);
	//		break;
	//	}


	//}

	//CloseHandle(hFile);
	////MessageBox(g_hWnd, L"오브젝트 불러오기 성공", L"성공", MB_OK);
}

void CObjManager::Save_Object_Boss()
{
	//HANDLE hFile = CreateFile(L"../Data/Object_Boss.dat", GENERIC_WRITE,
	//	NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	//if (INVALID_HANDLE_VALUE == hFile)
	//{
	//	MessageBox(g_hWnd, L"오브젝트 저장 실패", L"실패", MB_OK);
	//	return;
	//}

	//DWORD dwByte = 0;

	//for (int i = 0; i < MAPBLOCK::END; ++i)
	//{
	//	for (auto& pObj : m_listMapBLOCK[i])
	//	{
	//		// 애드 오브젝트랑 타일 속성 저장
	//		WriteFile(hFile, &pObj->Get_Info(), sizeof(INFO), &dwByte, NULL);
	//		WriteFile(hFile, &pObj->Get_BlockType(), sizeof(MAPBLOCK::BLOCK), &dwByte, NULL);

	//	}
	//}

	//CloseHandle(hFile);
	//MessageBox(g_hWnd, L"저장 성공", L"성공", MB_OK);
}

void CObjManager::Load_Object_Boss()
{
	//HANDLE hFile = CreateFile(L"../Data/Object_Boss.dat", GENERIC_READ,
	//	NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	//if (INVALID_HANDLE_VALUE == hFile)
	//{
	//	MessageBox(g_hWnd, L"오브젝트 불러오기 실패", L"실패", MB_OK);
	//	return;
	//}

	//Release();
	//CObj* _pObj = nullptr;
	//DWORD dwByte = 0;
	//INFO tTemp = {};
	//MAPBLOCK::BLOCK		 BlockInfo;


	//while (true)
	//{
	//	ReadFile(hFile, &tTemp, sizeof(INFO), &dwByte, NULL);
	//	ReadFile(hFile, &BlockInfo, sizeof(MAPBLOCK::BLOCK), &dwByte, NULL);

	//	if (0 == dwByte)
	//		break;

	//	switch (BlockInfo)
	//	{
	//	case MAPBLOCK::BASKET:
	//		_pObj = CAbstractFactory<CBasket>::Create(tTemp.fX, tTemp.fY);
	//		CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, BlockInfo);
	//		CTileManager::Get_Instance()->SetTileBlockType(tTemp.fX, tTemp.fY, BlockInfo);
	//		break;
	//	case MAPBLOCK::LEAF1:
	//		_pObj = CAbstractFactory<CLeaf1>::Create(tTemp.fX, tTemp.fY);
	//		CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, BlockInfo);
	//		CTileManager::Get_Instance()->SetTileBlockType(tTemp.fX, tTemp.fY, BlockInfo);
	//		break;
	//	case MAPBLOCK::LEAF2:
	//		_pObj = CAbstractFactory<CLeaf2>::Create(tTemp.fX, tTemp.fY);
	//		CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, BlockInfo);
	//		CTileManager::Get_Instance()->SetTileBlockType(tTemp.fX, tTemp.fY, BlockInfo);
	//		break;
	//	case MAPBLOCK::FLOWER:
	//		_pObj = CAbstractFactory<CFlower>::Create(tTemp.fX, tTemp.fY);
	//		CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, BlockInfo);
	//		CTileManager::Get_Instance()->SetTileBlockType(tTemp.fX, tTemp.fY, BlockInfo);
	//		break;
	//	case MAPBLOCK::GRASS:
	//		_pObj = CAbstractFactory<CGrass>::Create(tTemp.fX, tTemp.fY);
	//		CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, BlockInfo);
	//		CTileManager::Get_Instance()->SetTileBlockType(tTemp.fX, tTemp.fY, BlockInfo);
	//		break;
	//	case MAPBLOCK::ROCK1:
	//		_pObj = CAbstractFactory<Rock1>::Create(tTemp.fX, tTemp.fY);
	//		CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, BlockInfo);
	//		CTileManager::Get_Instance()->SetTileBlockType(tTemp.fX, tTemp.fY, BlockInfo);
	//		break;
	//	case MAPBLOCK::ROCK2:
	//		_pObj = CAbstractFactory<Rock2>::Create(tTemp.fX, tTemp.fY);
	//		CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, BlockInfo);
	//		CTileManager::Get_Instance()->SetTileBlockType(tTemp.fX, tTemp.fY, BlockInfo);
	//		break;
	//	case MAPBLOCK::ROCK3:
	//		_pObj = CAbstractFactory<Rock3>::Create(tTemp.fX, tTemp.fY);
	//		CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, BlockInfo);
	//		CTileManager::Get_Instance()->SetTileBlockType(tTemp.fX, tTemp.fY, BlockInfo);
	//		break;
	//	}


	//}

	//CloseHandle(hFile);
	////MessageBox(g_hWnd, L"오브젝트 불러오기 성공", L"성공", MB_OK);
}

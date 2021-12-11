#include "stdafx.h"
#include "ObjManager.h"
#include "Obj.h"
#include "CollidManager.h"
#include "StageMap.h"
#include "Player.h"
#include "Bomb.h"
#include "TileManager.h"
#include "Basket.h"
#include "Leaf1.h"
#include "Leaf2.h"
#include "Flower.h"
#include "Grass.h"
#include "Rock1.h"
#include "Rock2.h"
#include "Rock3.h"
#include "Messi.h"
#include "Mbape.h"
#include "HMSon.h"
#include "BossBomb.h"
#include "Bazzi.h"
#include "Dao.h"
#include "Digenie.h"
#include "Uni.h"
#include "Bomb.h"
#include "Ballon.h"
#include "Potion.h"
#include "Skate.h"
#include "MaxPotion.h"

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
	for (int i = 0; i < OBJID::END; ++i)
	{
		if (i != OBJID::MONSTER) {
			for (auto& iter = m_listObj[i].begin(); iter != m_listObj[i].end();)
			{
				if (!(*iter))
					continue;
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
	}





	for (auto& iter = m_listObj[OBJID::BOMB].begin(); iter != m_listObj[OBJID::BOMB].end(); ++iter)
	{
		if (dynamic_cast<CBomb*>(*iter)->Get_isBombCollid())
			CCollidManager::Collision_Rect_NoPushBombToPlayer(m_listObj[OBJID::PLAYER], m_listObj[OBJID::BOMB]);
	}
	CCollidManager::Collision_Rect_BombToBombWave(m_listObj[OBJID::BOMBWAVE], m_listObj[OBJID::BOMB]);


	CCollidManager::Collision_Rect_NoPush(m_listObj[OBJID::PLAYER], m_listMapBLOCK[MAPBLOCK::BASKET]);
	CCollidManager::Collision_Rect_NoPush(m_listObj[OBJID::PLAYER], m_listMapBLOCK[MAPBLOCK::LEAF1]);
	CCollidManager::Collision_Rect_NoPush(m_listObj[OBJID::PLAYER], m_listMapBLOCK[MAPBLOCK::LEAF2]);
	CCollidManager::Collision_Rect_NoPush(m_listObj[OBJID::PLAYER], m_listMapBLOCK[MAPBLOCK::FLOWER]);
	CCollidManager::Collision_Rect_NoPush(m_listObj[OBJID::PLAYER], m_listMapBLOCK[MAPBLOCK::GRASS]);

	CCollidManager::Collision_Rect_NoPush(m_listObj[OBJID::PLAYER], m_listMapBLOCK[MAPBLOCK::ROCK1]);
	CCollidManager::Collision_Rect_NoPush(m_listObj[OBJID::PLAYER], m_listMapBLOCK[MAPBLOCK::ROCK2]);
	CCollidManager::Collision_Rect_NoPush(m_listObj[OBJID::PLAYER], m_listMapBLOCK[MAPBLOCK::ROCK3]);

	CCollidManager::Collision_Rect_NoPush(m_listObj[OBJID::MONSTER], m_listObj[OBJID::BOMB]);



	// bombwave �浹������ ���� ��� �ּ�
	CCollidManager::Collision_Rect_BombWaveToBlock(m_listObj[OBJID::BOMBWAVE], m_listMapBLOCK[MAPBLOCK::BASKET]);
	CCollidManager::Collision_Rect_BombWaveToBlock(m_listObj[OBJID::BOMBWAVE], m_listMapBLOCK[MAPBLOCK::LEAF1]);
	CCollidManager::Collision_Rect_BombWaveToBlock(m_listObj[OBJID::BOMBWAVE], m_listMapBLOCK[MAPBLOCK::LEAF2]);
	CCollidManager::Collision_Rect_BombWaveToBlock(m_listObj[OBJID::BOMBWAVE], m_listMapBLOCK[MAPBLOCK::FLOWER]);
	CCollidManager::Collision_Rect_BombWaveToBlock(m_listObj[OBJID::BOMBWAVE], m_listMapBLOCK[MAPBLOCK::GRASS]);

	//CCollidManager::Collision_Rect_BombWaveToBlock(m_listObj[OBJID::BOMBWAVE], m_listObj[OBJID::MONSTER]);
	CCollidManager::Collision_Rect_BombWaveToMonster(m_listObj[OBJID::BOMBWAVE], m_listObj[OBJID::MONSTER]);
	CCollidManager::Collision_Rect_BombWaveToMonster(m_listObj[OBJID::BOMBWAVE], m_listObj[OBJID::BOSS]);

	///////////////////////////////// ġƮ
	// �׽�Ʈ������ ġƮ �ѳ��� ����
	if (m_bisCheat)
	{
		CCollidManager::Collision_Rect_PlayerToBoss(m_listObj[OBJID::PLAYER], m_listObj[OBJID::BOSS]);
		CCollidManager::Collision_Rect_PlayerToMonster(m_listObj[OBJID::PLAYER], m_listObj[OBJID::MONSTER]);
		CCollidManager::Collision_Rect(m_listObj[OBJID::BOMBWAVE], m_listObj[OBJID::PLAYER]);
	}
	//////////////////////////////////////////////////////

	CCollidManager::Collision_Rect_PlayerToBallon(m_listObj[OBJID::PLAYER], m_listItem[GAMEITEM::BALLON]);
	CCollidManager::Collision_Rect_PlayerToSkate(m_listObj[OBJID::PLAYER], m_listItem[GAMEITEM::SKATE]);
	CCollidManager::Collision_Rect_PlayerToPotion(m_listObj[OBJID::PLAYER], m_listItem[GAMEITEM::POTION]);
	CCollidManager::Collision_Rect_PlayerToMaxPotion(m_listObj[OBJID::PLAYER], m_listItem[GAMEITEM::MAXPOTION]);

	CCollidManager::Collision_Rect_PlayerToBallon(m_listObj[OBJID::MULTIPLAYER], m_listItem[GAMEITEM::BALLON]);
	CCollidManager::Collision_Rect_PlayerToSkate(m_listObj[OBJID::MULTIPLAYER], m_listItem[GAMEITEM::SKATE]);
	CCollidManager::Collision_Rect_PlayerToPotion(m_listObj[OBJID::MULTIPLAYER], m_listItem[GAMEITEM::POTION]);
	CCollidManager::Collision_Rect_PlayerToMaxPotion(m_listObj[OBJID::MULTIPLAYER], m_listItem[GAMEITEM::MAXPOTION]);

}

void CObjManager::Late_Update()
{
	for (int i = 0; i < OBJID::END; ++i)
	{
		if (i != OBJID::MONSTER) {
			for (auto& pObj : m_listObj[i])
				pObj->Late_Update();
		}

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
			if (pObj->GetState() != OBJSTATE::BUBBLE)
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
		if (i != OBJID::MULTIPLAYER) {
			for_each(m_listObj[i].begin(), m_listObj[i].end(), Safe_Delete<CObj*>);
			m_listObj[i].clear();
		}
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

void CObjManager::Make_Add_Item(ITEMINFO tItem)
{
	if (!Check_BlockState(tItem.iBlockObjNum)) {
		CObj* pObj = nullptr;

		switch (tItem.ItemName)
		{
		case GAMEITEM::BALLON:
			pObj = CAbstractFactory<CBallon>::Create(tItem.ItemPos.fX, tItem.ItemPos.fY);
			CObjManager::Get_Instance()->Add_Object_Item(pObj, GAMEITEM::BALLON);
			break;
		case GAMEITEM::POTION:
			pObj = CAbstractFactory<CPotion>::Create(tItem.ItemPos.fX, tItem.ItemPos.fY);
			CObjManager::Get_Instance()->Add_Object_Item(pObj, GAMEITEM::POTION);
			break;
		case GAMEITEM::SKATE:
			pObj = CAbstractFactory<CSkate>::Create(tItem.ItemPos.fX, tItem.ItemPos.fY);
			CObjManager::Get_Instance()->Add_Object_Item(pObj, GAMEITEM::SKATE);
			break;
		case GAMEITEM::MAXPOTION:
			pObj = CAbstractFactory<CMaxPotion>::Create(tItem.ItemPos.fX, tItem.ItemPos.fY);
			CObjManager::Get_Instance()->Add_Object_Item(pObj, GAMEITEM::MAXPOTION);
			break;
		}
	}
}

float CObjManager::Get_PlayerX()
{
	for (auto& iter = m_listObj[OBJID::PLAYER].begin(); iter != m_listObj[OBJID::PLAYER].end(); ++iter)
	{
		return dynamic_cast<CPlayer*>(*iter)->Get_PlayerX();
	}
}

float CObjManager::Get_PlayerY()
{
	for (auto& iter = m_listObj[OBJID::PLAYER].begin(); iter != m_listObj[OBJID::PLAYER].end(); ++iter)
	{
		return dynamic_cast<CPlayer*>(*iter)->Get_PlayerY();
	}
}

float CObjManager::Get_BombX()
{
	for (auto& iter = m_listObj[OBJID::BOMB].begin(); iter != m_listObj[OBJID::BOMB].end(); ++iter)
	{
		return dynamic_cast<CBomb*>(*iter)->Get_BombX();
	}
}

float CObjManager::Get_BombY()
{
	for (auto& iter = m_listObj[OBJID::BOMB].begin(); iter != m_listObj[OBJID::BOMB].end(); ++iter)
	{
		return dynamic_cast<CBomb*>(*iter)->Get_BombY();
	}
}

bool CObjManager::Get_isBombPos(float _x, float _y)
{
	for (auto& iter = m_listObj[OBJID::MULTIBOMB].begin(); iter != m_listObj[OBJID::MULTIBOMB].end(); ++iter)
	{
		if (dynamic_cast<CBomb*>(*iter)->Get_BombX() == _x &&
			dynamic_cast<CBomb*>(*iter)->Get_BombY() == _y)
			return true;
		return false;
	}
}

OBJDIR::DIR CObjManager::Get_PlayerDir()
{
	for (auto& iter = m_listObj[OBJID::PLAYER].begin(); iter != m_listObj[OBJID::PLAYER].end(); ++iter)
	{
		return dynamic_cast<CPlayer*>(*iter)->Get_PlayerDir();
	}
}

void CObjManager::Set_PlayerX(float fX)
{
	for (auto& iter = m_listObj[OBJID::PLAYER].begin(); iter != m_listObj[OBJID::PLAYER].end(); ++iter)
	{
		dynamic_cast<CPlayer*>(*iter)->Set_PlayerX(fX);
	}
}

void CObjManager::Set_PlayerY(float fY)
{
	for (auto& iter = m_listObj[OBJID::PLAYER].begin(); iter != m_listObj[OBJID::PLAYER].end(); ++iter)
	{
		dynamic_cast<CPlayer*>(*iter)->Set_PlayerY(fY);
	}
}

void CObjManager::Picking_Object(CObj* _pObj, MAPBLOCK::BLOCK _block)
{
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	int x = (pt.x - MAPSTARTX) / TILECX;
	int y = (pt.y - MAPSTARTY) / TILECY;

	switch (_block)
	{
	case MAPBLOCK::BASKET:
		_pObj = CAbstractFactory<CBasket>::Create(MAPSTARTX + (TILECX * x) + (TILECX >> 1), MAPSTARTY + (TILECY * y) + (TILECY >> 1));
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
	}
}

// ��Ʈ��ũ ������ ���� ����
void CObjManager::Save_Object()
{
	//HANDLE hFile = CreateFile(L"../Data/Object.dat", GENERIC_WRITE,
	//	NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	HANDLE hFile = CreateFile(L"../Data/NetworkObject.dat", GENERIC_WRITE,
		NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"������Ʈ ���� ����", L"����", MB_OK);
		return;
	}

	DWORD dwByte = 0;

	for (int i = 0; i < MAPBLOCK::END; ++i)
	{
		for (auto& pObj : m_listMapBLOCK[i])
		{
			// �ֵ� ������Ʈ�� Ÿ�� �Ӽ� ����
			WriteFile(hFile, &pObj->Get_Info(), sizeof(INFO), &dwByte, NULL);
			WriteFile(hFile, &pObj->Get_BlockType(), sizeof(MAPBLOCK::BLOCK), &dwByte, NULL);
			/*switch (i)
			{
			case MAPBLOCK::BASKET:
				WriteFile(hFile, &pObj->Get_BlockType(), sizeof(MAPBLOCK::BLOCK), &dwByte, NULL);
				break;
			}*/

		}
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"���� ����", L"����", MB_OK);
}

// ��Ʈ��ũ ������ ���� ����
void CObjManager::Load_Object()
{
	HANDLE hFile = CreateFile(L"../Data/NetworkObject.dat", GENERIC_READ,
		NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"������Ʈ �ҷ����� ����", L"����", MB_OK);
		return;
	}

	Release();
	CObj* _pObj = nullptr;
	DWORD dwByte = 0;
	INFO tTemp = {};
	MAPBLOCK::BLOCK		 BlockInfo;
	int iNum = 0;

	while (true)
	{
		ReadFile(hFile, &tTemp, sizeof(INFO), &dwByte, NULL);
		ReadFile(hFile, &BlockInfo, sizeof(MAPBLOCK::BLOCK), &dwByte, NULL);

		if (0 == dwByte)
			break;

		switch (BlockInfo)
		{
		case MAPBLOCK::BASKET:
			_pObj = CAbstractFactory<CBasket>::Create(tTemp.fX, tTemp.fY);
			//_pObj->Set_ObjNum(iNum);
			CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, BlockInfo);
			CTileManager::Get_Instance()->SetTileBlockType(tTemp.fX, tTemp.fY, BlockInfo);
			break;
		case MAPBLOCK::LEAF1:
			_pObj = CAbstractFactory<CLeaf1>::Create(tTemp.fX, tTemp.fY);
			//_pObj->Set_ObjNum(iNum);
			CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, BlockInfo);
			CTileManager::Get_Instance()->SetTileBlockType(tTemp.fX, tTemp.fY, BlockInfo);
			break;
		case MAPBLOCK::LEAF2:
			_pObj = CAbstractFactory<CLeaf2>::Create(tTemp.fX, tTemp.fY);
			//_pObj->Set_ObjNum(iNum);
			CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, BlockInfo);
			CTileManager::Get_Instance()->SetTileBlockType(tTemp.fX, tTemp.fY, BlockInfo);
			break;
		case MAPBLOCK::FLOWER:
			_pObj = CAbstractFactory<CFlower>::Create(tTemp.fX, tTemp.fY);
			//_pObj->Set_ObjNum(iNum);
			CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, BlockInfo);
			CTileManager::Get_Instance()->SetTileBlockType(tTemp.fX, tTemp.fY, BlockInfo);
			break;
		case MAPBLOCK::GRASS:
			_pObj = CAbstractFactory<CGrass>::Create(tTemp.fX, tTemp.fY);
			//_pObj->Set_ObjNum(iNum);
			CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, BlockInfo);
			CTileManager::Get_Instance()->SetTileBlockType(tTemp.fX, tTemp.fY, BlockInfo);
			break;
		case MAPBLOCK::ROCK1:
			_pObj = CAbstractFactory<Rock1>::Create(tTemp.fX, tTemp.fY);
			//_pObj->Set_ObjNum(iNum);
			CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, BlockInfo);
			CTileManager::Get_Instance()->SetTileBlockType(tTemp.fX, tTemp.fY, BlockInfo);
			break;
		case MAPBLOCK::ROCK2:
			_pObj = CAbstractFactory<Rock2>::Create(tTemp.fX, tTemp.fY);
			//_pObj->Set_ObjNum(iNum);
			CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, BlockInfo);
			CTileManager::Get_Instance()->SetTileBlockType(tTemp.fX, tTemp.fY, BlockInfo);
			break;
		case MAPBLOCK::ROCK3:
			_pObj = CAbstractFactory<Rock3>::Create(tTemp.fX, tTemp.fY);
			//_pObj->Set_ObjNum(iNum);
			CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, BlockInfo);
			CTileManager::Get_Instance()->SetTileBlockType(tTemp.fX, tTemp.fY, BlockInfo);
			break;
		}
		_pObj->Set_ObjNum(iNum);
		++iNum;
	}

	CloseHandle(hFile);
	//MessageBox(g_hWnd, L"������Ʈ �ҷ����� ����", L"����", MB_OK);

}

void CObjManager::Save_Object_Stage2()
{
	HANDLE hFile = CreateFile(L"../Data/Object_Stage2.dat", GENERIC_WRITE,
		NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"������Ʈ ���� ����", L"����", MB_OK);
		return;
	}

	DWORD dwByte = 0;

	for (int i = 0; i < MAPBLOCK::END; ++i)
	{
		for (auto& pObj : m_listMapBLOCK[i])
		{
			// �ֵ� ������Ʈ�� Ÿ�� �Ӽ� ����
			WriteFile(hFile, &pObj->Get_Info(), sizeof(INFO), &dwByte, NULL);
			WriteFile(hFile, &pObj->Get_BlockType(), sizeof(MAPBLOCK::BLOCK), &dwByte, NULL);

		}
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"���� ����", L"����", MB_OK);
}

void CObjManager::Load_Object_Stage2()
{
	HANDLE hFile = CreateFile(L"../Data/Object_Stage2.dat", GENERIC_READ,
		NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"������Ʈ �ҷ����� ����", L"����", MB_OK);
		return;
	}

	Release();
	CObj* _pObj = nullptr;
	DWORD dwByte = 0;
	INFO tTemp = {};
	MAPBLOCK::BLOCK		 BlockInfo;

	while (true)
	{
		ReadFile(hFile, &tTemp, sizeof(INFO), &dwByte, NULL);
		ReadFile(hFile, &BlockInfo, sizeof(MAPBLOCK::BLOCK), &dwByte, NULL);

		if (0 == dwByte)
			break;

		switch (BlockInfo)
		{
		case MAPBLOCK::BASKET:
			_pObj = CAbstractFactory<CBasket>::Create(tTemp.fX, tTemp.fY);
			CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, BlockInfo);
			CTileManager::Get_Instance()->SetTileBlockType(tTemp.fX, tTemp.fY, BlockInfo);
			break;
		case MAPBLOCK::LEAF1:
			_pObj = CAbstractFactory<CLeaf1>::Create(tTemp.fX, tTemp.fY);
			CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, BlockInfo);
			CTileManager::Get_Instance()->SetTileBlockType(tTemp.fX, tTemp.fY, BlockInfo);
			break;
		case MAPBLOCK::LEAF2:
			_pObj = CAbstractFactory<CLeaf2>::Create(tTemp.fX, tTemp.fY);
			CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, BlockInfo);
			CTileManager::Get_Instance()->SetTileBlockType(tTemp.fX, tTemp.fY, BlockInfo);
			break;
		case MAPBLOCK::FLOWER:
			_pObj = CAbstractFactory<CFlower>::Create(tTemp.fX, tTemp.fY);
			CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, BlockInfo);
			CTileManager::Get_Instance()->SetTileBlockType(tTemp.fX, tTemp.fY, BlockInfo);
			break;
		case MAPBLOCK::GRASS:
			_pObj = CAbstractFactory<CGrass>::Create(tTemp.fX, tTemp.fY);
			CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, BlockInfo);
			CTileManager::Get_Instance()->SetTileBlockType(tTemp.fX, tTemp.fY, BlockInfo);
			break;
		case MAPBLOCK::ROCK1:
			_pObj = CAbstractFactory<Rock1>::Create(tTemp.fX, tTemp.fY);
			CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, BlockInfo);
			CTileManager::Get_Instance()->SetTileBlockType(tTemp.fX, tTemp.fY, BlockInfo);
			break;
		case MAPBLOCK::ROCK2:
			_pObj = CAbstractFactory<Rock2>::Create(tTemp.fX, tTemp.fY);
			CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, BlockInfo);
			CTileManager::Get_Instance()->SetTileBlockType(tTemp.fX, tTemp.fY, BlockInfo);
			break;
		case MAPBLOCK::ROCK3:
			_pObj = CAbstractFactory<Rock3>::Create(tTemp.fX, tTemp.fY);
			CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, BlockInfo);
			CTileManager::Get_Instance()->SetTileBlockType(tTemp.fX, tTemp.fY, BlockInfo);
			break;
		}


	}

	CloseHandle(hFile);
	//MessageBox(g_hWnd, L"������Ʈ �ҷ����� ����", L"����", MB_OK);
}

void CObjManager::Save_Object_Boss()
{
	HANDLE hFile = CreateFile(L"../Data/Object_Boss.dat", GENERIC_WRITE,
		NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"������Ʈ ���� ����", L"����", MB_OK);
		return;
	}

	DWORD dwByte = 0;

	for (int i = 0; i < MAPBLOCK::END; ++i)
	{
		for (auto& pObj : m_listMapBLOCK[i])
		{
			// �ֵ� ������Ʈ�� Ÿ�� �Ӽ� ����
			WriteFile(hFile, &pObj->Get_Info(), sizeof(INFO), &dwByte, NULL);
			WriteFile(hFile, &pObj->Get_BlockType(), sizeof(MAPBLOCK::BLOCK), &dwByte, NULL);

		}
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"���� ����", L"����", MB_OK);
}

void CObjManager::Load_Object_Boss()
{
	HANDLE hFile = CreateFile(L"../Data/Object_Boss.dat", GENERIC_READ,
		NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"������Ʈ �ҷ����� ����", L"����", MB_OK);
		return;
	}

	Release();
	CObj* _pObj = nullptr;
	DWORD dwByte = 0;
	INFO tTemp = {};
	MAPBLOCK::BLOCK		 BlockInfo;


	while (true)
	{
		ReadFile(hFile, &tTemp, sizeof(INFO), &dwByte, NULL);
		ReadFile(hFile, &BlockInfo, sizeof(MAPBLOCK::BLOCK), &dwByte, NULL);

		if (0 == dwByte)
			break;

		switch (BlockInfo)
		{
		case MAPBLOCK::BASKET:
			_pObj = CAbstractFactory<CBasket>::Create(tTemp.fX, tTemp.fY);
			CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, BlockInfo);
			CTileManager::Get_Instance()->SetTileBlockType(tTemp.fX, tTemp.fY, BlockInfo);
			break;
		case MAPBLOCK::LEAF1:
			_pObj = CAbstractFactory<CLeaf1>::Create(tTemp.fX, tTemp.fY);
			CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, BlockInfo);
			CTileManager::Get_Instance()->SetTileBlockType(tTemp.fX, tTemp.fY, BlockInfo);
			break;
		case MAPBLOCK::LEAF2:
			_pObj = CAbstractFactory<CLeaf2>::Create(tTemp.fX, tTemp.fY);
			CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, BlockInfo);
			CTileManager::Get_Instance()->SetTileBlockType(tTemp.fX, tTemp.fY, BlockInfo);
			break;
		case MAPBLOCK::FLOWER:
			_pObj = CAbstractFactory<CFlower>::Create(tTemp.fX, tTemp.fY);
			CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, BlockInfo);
			CTileManager::Get_Instance()->SetTileBlockType(tTemp.fX, tTemp.fY, BlockInfo);
			break;
		case MAPBLOCK::GRASS:
			_pObj = CAbstractFactory<CGrass>::Create(tTemp.fX, tTemp.fY);
			CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, BlockInfo);
			CTileManager::Get_Instance()->SetTileBlockType(tTemp.fX, tTemp.fY, BlockInfo);
			break;
		case MAPBLOCK::ROCK1:
			_pObj = CAbstractFactory<Rock1>::Create(tTemp.fX, tTemp.fY);
			CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, BlockInfo);
			CTileManager::Get_Instance()->SetTileBlockType(tTemp.fX, tTemp.fY, BlockInfo);
			break;
		case MAPBLOCK::ROCK2:
			_pObj = CAbstractFactory<Rock2>::Create(tTemp.fX, tTemp.fY);
			CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, BlockInfo);
			CTileManager::Get_Instance()->SetTileBlockType(tTemp.fX, tTemp.fY, BlockInfo);
			break;
		case MAPBLOCK::ROCK3:
			_pObj = CAbstractFactory<Rock3>::Create(tTemp.fX, tTemp.fY);
			CObjManager::Get_Instance()->Add_Object_MapBlock(_pObj, BlockInfo);
			CTileManager::Get_Instance()->SetTileBlockType(tTemp.fX, tTemp.fY, BlockInfo);
			break;
		}


	}

	CloseHandle(hFile);
	//MessageBox(g_hWnd, L"������Ʈ �ҷ����� ����", L"����", MB_OK);
}

void CObjManager::Update_MonsterInfo(vector<MONSTERINFO> vInfo)
{
	int i = 0;
	for (auto iter = m_listObj[OBJID::MONSTER].begin(); iter != m_listObj[OBJID::MONSTER].end(); ++iter)
	{
		(*iter)->Change_PosX(vInfo[i].MonsterPos.fX);
		(*iter)->Change_PosY(vInfo[i].MonsterPos.fY);
		(*iter)->SetCurDIR(vInfo[i].MonsterDir);
		(*iter)->SetFrame(vInfo[i].Monsterframe);
		(*iter)->SetMonsterID(i);
		++i;
	}
}

void CObjManager::Add_Monster(MONSTERINFO info, int iNum)
{
	CObj* pObj = nullptr;
	if (info.MonsterName == MONSTERNAME::NAME::MESSI)
		pObj = CAbstractFactory<CMessi>::Create_Monster(info.MonsterPos.fX, info.MonsterPos.fY, info.MonsterDir);
	if (info.MonsterName == MONSTERNAME::NAME::MBAPE)
		pObj = CAbstractFactory<CMbape>::Create_Monster(info.MonsterPos.fX, info.MonsterPos.fY, info.MonsterDir);

	Add_Object(pObj, OBJID::MONSTER);
}

void CObjManager::Add_NetWorkPlayer(CLIENTINFO _playerinfo)
{
	CObj* pObj = nullptr;
	if (_playerinfo.PlayerInfo.PlayerName == CHARNAME::BAZZI) {
		pObj = CAbstractFactory<CBazzi>::Create();
		pObj->Set_ClientID(_playerinfo.ClientID);
	}
	if (_playerinfo.PlayerInfo.PlayerName == CHARNAME::DAO) {
		pObj = CAbstractFactory<CDao>::Create();
		pObj->Set_ClientID(_playerinfo.ClientID);
	}
	if (_playerinfo.PlayerInfo.PlayerName == CHARNAME::DIGENIE) {
		pObj = CAbstractFactory<CDigenie>::Create();
		pObj->Set_ClientID(_playerinfo.ClientID);
	}
	if (_playerinfo.PlayerInfo.PlayerName == CHARNAME::UNI) {
		pObj = CAbstractFactory<CUni>::Create();
		pObj->Set_ClientID(_playerinfo.ClientID);
	}

	m_listObj[OBJID::MULTIPLAYER].emplace_back(pObj);
	//Add_Object(pObj, OBJID::MULTIPLAYER);
}

void CObjManager::Update_NetWorkPlayer(CLIENTINFO& _playerinfo)
{
	for (auto& player : m_listObj[OBJID::MULTIPLAYER])
	{
		if (player->Get_ClientID() == _playerinfo.ClientID)
		{
			if (!isnan(_playerinfo.BombPos.fX) && !isnan(_playerinfo.BombPos.fY)) {
				if (!Get_isBombPos(_playerinfo.BombPos.fX, _playerinfo.BombPos.fY)) {
					Add_Bomb(_playerinfo.BombPos, player->Get_BombPower());
				}
			}
			player->Change_PosX(_playerinfo.PlayerInfo.PlayerPos.fX);
			player->Change_PosY(_playerinfo.PlayerInfo.PlayerPos.fY);
			if (player->Get_ClientID() == 0)
				dynamic_cast<CBazzi*>(player)->SetPlayerDIR(_playerinfo.PlayerInfo.PlayerDir);
			if (player->Get_ClientID() == 1)
				dynamic_cast<CDao*>(player)->SetPlayerDIR(_playerinfo.PlayerInfo.PlayerDir);
			if (player->Get_ClientID() == 2)
				dynamic_cast<CDigenie*>(player)->SetPlayerDIR(_playerinfo.PlayerInfo.PlayerDir);
		}
	}
}

void CObjManager::Add_Bomb(OBJPOS _pos, int _bombPower)
{
	CObj* pObj = nullptr;
	pObj = CAbstractFactory<CBomb>::Create(_pos.fX, _pos.fY, _bombPower, false);
	Add_Object(pObj, OBJID::MULTIBOMB);

}

void CObjManager::Set_BlockBubble(int iNum)
{
	for (int i = 0; i < MAPBLOCK::END; ++i)
	{
		for (auto& iter = m_listMapBLOCK[i].begin(); iter != m_listMapBLOCK[i].end();)
		{
			if ((*iter)->Get_ObjNum() == iNum)
			{
				(*iter)->SetState(OBJSTATE::STATE::BUBBLE);
				return;
			}
			else
				++iter;
		}
	}
}

bool CObjManager::Check_BlockState(int iNum)
{
	auto iter = find(m_vecNoItemBlock.begin(), m_vecNoItemBlock.end(), iNum);
	if (iter != m_vecNoItemBlock.end())
		return true;
	else
		return false;
}

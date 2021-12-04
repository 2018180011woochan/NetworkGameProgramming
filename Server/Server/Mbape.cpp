#include "stdafx.h"
#include "Mbape.h"
#include "Obj.h"
//#include "BmpManager.h"
#include "TileManager.h"
//#include "KeyManager.h"
#include "ObjManager.h"

CMbape::CMbape()
{
}


CMbape::~CMbape()
{
	Release();
}

void CMbape::Initialize()
{
	/*CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Obj/Monster/Mbape.bmp", L"Mbape");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Obj/Monster/Mbape_left.bmp", L"Mbape_left");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Obj/Monster/Small_Mbape.bmp", L"Small_Mbape");*/

	m_tInfo.iCX = 40;
	m_tInfo.iCY = 40;

	m_tInfo.fSpeed = 1.f;

	m_eState = OBJSTATE::IDLE;

	m_tInfo.iHP = 2;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 1;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.dwFrameSpeed = 200;

	
}

int CMbape::Update()
{

	if (OBJSTATE::IDLE == m_eState)
	{
		switch (m_eCurDir)
		{
		case OBJDIR::TOP:
			m_tInfo.fY -= m_tInfo.fSpeed;
			break;
		case OBJDIR::BOTTOM:
			m_tInfo.fY += m_tInfo.fSpeed;
			break;
		case OBJDIR::LEFT:
			m_tInfo.fX -= m_tInfo.fSpeed;
			break;
		case OBJDIR::RIGHT:
			m_tInfo.fX += m_tInfo.fSpeed;
			break;
		}
	}

	if (OBJSTATE::HIT == m_eState)
	{
		if (m_dwTime + 2000 < GetTickCount())
		{
			m_eState = OBJSTATE::DEAD;
			m_dwTime = GetTickCount();
		}

	}
	if (OBJSTATE::DEAD == m_eState)
	{
		if (m_dwTime + 2000 < GetTickCount())
		{
			CObjManager::Get_Instance()->Set_Stage1Clear(1);
			m_dwTime = GetTickCount();
			return OBJ_DEAD;
		}
	}

	Update_Rect();
	Frame_Move();
	return OBJ_NOEVENET;
}

void CMbape::Late_Update()
{
	if (m_eCurDir == OBJDIR::LEFT)
	{
		if (m_tInfo.fX + 1 - (TILECX >> 1) < 20 ||
			MAPBLOCK::BASKET == CTileManager::Get_Instance()->GetTileBlockType(m_tInfo.fX - (TILECX >> 1), m_tInfo.fY) ||
			MAPBLOCK::LEAF1 == CTileManager::Get_Instance()->GetTileBlockType(m_tInfo.fX - (TILECX >> 1), m_tInfo.fY) ||
			MAPBLOCK::LEAF2 == CTileManager::Get_Instance()->GetTileBlockType(m_tInfo.fX - (TILECX >> 1), m_tInfo.fY) ||
			MAPBLOCK::FLOWER == CTileManager::Get_Instance()->GetTileBlockType(m_tInfo.fX - (TILECX >> 1), m_tInfo.fY) ||
			MAPBLOCK::GRASS == CTileManager::Get_Instance()->GetTileBlockType(m_tInfo.fX - (TILECX >> 1), m_tInfo.fY) ||
			MAPBLOCK::BOMB == CTileManager::Get_Instance()->GetTileBlockType(m_tInfo.fX - (TILECX >> 1), m_tInfo.fY))
		{
			m_eCurDir = OBJDIR::BOTTOM;
		}
	}

	if (m_eCurDir == OBJDIR::RIGHT)
	{
		if (m_tInfo.fX + 1 + (TILECX >> 1) > 620 ||
			MAPBLOCK::BASKET == CTileManager::Get_Instance()->GetTileBlockType(m_tInfo.fX + (TILECX >> 1), m_tInfo.fY) ||
			MAPBLOCK::LEAF1 == CTileManager::Get_Instance()->GetTileBlockType(m_tInfo.fX + (TILECX >> 1), m_tInfo.fY) ||
			MAPBLOCK::LEAF2 == CTileManager::Get_Instance()->GetTileBlockType(m_tInfo.fX + (TILECX >> 1), m_tInfo.fY) ||
			MAPBLOCK::FLOWER == CTileManager::Get_Instance()->GetTileBlockType(m_tInfo.fX + (TILECX >> 1), m_tInfo.fY) ||
			MAPBLOCK::GRASS == CTileManager::Get_Instance()->GetTileBlockType(m_tInfo.fX + (TILECX >> 1), m_tInfo.fY) ||
			MAPBLOCK::BOMB == CTileManager::Get_Instance()->GetTileBlockType(m_tInfo.fX + (TILECX >> 1), m_tInfo.fY))
		{
			m_eCurDir = OBJDIR::TOP;
		}
	}

	if (m_eCurDir == OBJDIR::TOP)
	{
		if (m_tInfo.fY + 1 - (TILECY >> 1) < 40 ||
			MAPBLOCK::BASKET == CTileManager::Get_Instance()->GetTileBlockType(m_tInfo.fX, m_tInfo.fY - (TILECY >> 1)) ||
			MAPBLOCK::LEAF1 == CTileManager::Get_Instance()->GetTileBlockType(m_tInfo.fX, m_tInfo.fY - (TILECY >> 1)) ||
			MAPBLOCK::LEAF2 == CTileManager::Get_Instance()->GetTileBlockType(m_tInfo.fX, m_tInfo.fY - (TILECY >> 1)) ||
			MAPBLOCK::FLOWER == CTileManager::Get_Instance()->GetTileBlockType(m_tInfo.fX, m_tInfo.fY - (TILECY >> 1)) ||
			MAPBLOCK::GRASS == CTileManager::Get_Instance()->GetTileBlockType(m_tInfo.fX, m_tInfo.fY - (TILECY >> 1)) ||
			MAPBLOCK::BOMB == CTileManager::Get_Instance()->GetTileBlockType(m_tInfo.fX, m_tInfo.fY - (TILECY >> 1)))
		{
			m_eCurDir = OBJDIR::LEFT;
		}
	}

	if (m_eCurDir == OBJDIR::BOTTOM)
	{
		if (m_tInfo.fY + 1 + (TILECY >> 1) > 560 ||
			MAPBLOCK::BASKET == CTileManager::Get_Instance()->GetTileBlockType(m_tInfo.fX, m_tInfo.fY + (TILECY >> 1)) ||
			MAPBLOCK::LEAF1 == CTileManager::Get_Instance()->GetTileBlockType(m_tInfo.fX, m_tInfo.fY + (TILECY >> 1)) ||
			MAPBLOCK::LEAF2 == CTileManager::Get_Instance()->GetTileBlockType(m_tInfo.fX, m_tInfo.fY + (TILECY >> 1)) ||
			MAPBLOCK::FLOWER == CTileManager::Get_Instance()->GetTileBlockType(m_tInfo.fX, m_tInfo.fY + (TILECY >> 1)) ||
			MAPBLOCK::GRASS == CTileManager::Get_Instance()->GetTileBlockType(m_tInfo.fX, m_tInfo.fY + (TILECY >> 1)) ||
			MAPBLOCK::BOMB == CTileManager::Get_Instance()->GetTileBlockType(m_tInfo.fX, m_tInfo.fY + (TILECY >> 1)))
		{
			m_eCurDir = OBJDIR::RIGHT;
		}
	}
}

void CMbape::Render(HDC _DC)
{
	
}

void CMbape::Release()
{
}

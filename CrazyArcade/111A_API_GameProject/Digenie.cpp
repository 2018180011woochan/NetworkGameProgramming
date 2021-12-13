#include "stdafx.h"
#include "Digenie.h"
#include "Bomb.h"
#include "ObjManager.h"
#include "KeyManager.h"
#include "LineManager.h"
#include "BmpManager.h"
#include "SoundMgr.h"
#include "SceneManager.h"
#include "TileManager.h"
#include "ClientManager.h"

CDigenie::CDigenie()
{
}


CDigenie::~CDigenie()
{
}

void CDigenie::Initialize()
{
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Obj/Character/Digenie/digenie.bmp", L"Digenie");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Obj/Character/Digenie/digenie_bubble.bmp", L"DigenieBubble");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Obj/Character/Digenie/digenie_pop.bmp", L"DigeniePop");

	//ClientID = 0, 위치는 왼쪽 위
	if (CClientManager::Get_Instance()->GetClientID() == 0) {
		m_tInfo.fX = MAPSTARTX + (TILECX >> 1);
		m_tInfo.fY = MAPSTARTY + (TILECY >> 1);
	}
	// ClientID = 1,	위치는 오른쪽 위
	if (CClientManager::Get_Instance()->GetClientID() == 1) {
		m_tInfo.fX = MAPSTARTX + (TILECX * 14) + (TILECX >> 1);
		m_tInfo.fY = MAPSTARTY + (TILECY >> 1);
	}
	// ClientID = 2,	위치는 왼쪽 아래
	if (CClientManager::Get_Instance()->GetClientID() == 2) {
		m_tInfo.fX = MAPSTARTX + (TILECX >> 1);
		m_tInfo.fY = MAPSTARTY + (TILECY * 12) + (TILECY >> 1);
	}
	// ClientID = 3,	위치는 오른쪽 아래
	if (CClientManager::Get_Instance()->GetClientID() == 3) {
		m_tInfo.fX = MAPSTARTX + (TILECX * 14) + (TILECX >> 1);
		m_tInfo.fY = MAPSTARTY + (TILECY * 12) + (TILECY >> 1);
	}
	m_tInfo.iCX = 25;
	m_tInfo.iCY = 25;


	m_tInfo.iBombPower = 1;

	m_tInfo.iHP = 1;
	m_tInfo.iMaxBomb = 1;

	m_tInfo.fSpeed = 2.f;
	m_tInfo.bisDead = false;

	m_eState = OBJSTATE::IDLE;
	m_eCurDir = OBJDIR::IDLE;
}

int CDigenie::Update()
{
	if (OBJSTATE::BUBBLE == m_eState)
	{
		if (m_dwTime + 6000 < GetTickCount())
		{
			m_eState = OBJSTATE::DEAD;
			m_dwTime = GetTickCount();
		}

	}
	if (OBJSTATE::DEAD == m_eState)
	{
		if (m_dwTime + 2000 < GetTickCount())
		{
			m_dwTime = GetTickCount();
			/*CSoundMgr::Get_Instance()->StopAll();
			CSoundMgr::Get_Instance()->PlaySound(L"lose.mp3", CSoundMgr::PLAYER_LOSE);
			CSceneManager::Get_Instance()->Scene_Change(CSceneManager::SCENE_MENU);
			CTileManager::Get_Instance()->Release();*/
			return OBJ_DEAD;
		}

	}



	if (CClientManager::Get_Instance()->GetClientID() == 2) {
		if (OBJSTATE::IDLE == m_eState)
			Key_Check();
	}

	Update_Rect();
	Scene_Change();
	Frame_Move();

	return OBJ_NOEVENET;
}

void CDigenie::Late_Update()
{
	if (m_tInfo.fX < 32)
		m_tInfo.fX += m_tInfo.fSpeed;

	if (m_tInfo.fX > 605)
		m_tInfo.fX -= m_tInfo.fSpeed;

	if (m_tInfo.fY < 52)
		m_tInfo.fY += m_tInfo.fSpeed;

	if (m_tInfo.fY > 547)
		m_tInfo.fY -= m_tInfo.fSpeed;
}

void CDigenie::Render(HDC _DC)
{
	HDC hMemDC;
	switch (m_eState)
	{
	case OBJSTATE::IDLE:
		hMemDC = CBmpManager::Get_Instance()->Find_Image(L"Digenie");
		GdiTransparentBlt(_DC, m_tRect.left - 15, m_tRect.top - 30
			, 50, 60, hMemDC
			, m_tFrame.iFrameStart * 50, m_tFrame.iFrameScene * 60
			, 50, 60
			, RGB(100, 100, 100));
		break;
	case OBJSTATE::BUBBLE:
		m_tFrame.iFrameEnd = 6;
		m_tFrame.dwFrameSpeed = 1000;

		hMemDC = CBmpManager::Get_Instance()->Find_Image(L"DigenieBubble");
		GdiTransparentBlt(_DC, m_tRect.left - 5, m_tRect.top - 20
			, 50, 60, hMemDC
			, m_tFrame.iFrameStart * 60, 0
			, 60, 60
			, RGB(100, 100, 100));
		break;
	case OBJSTATE::DEAD:
		m_tFrame.iFrameEnd = 12;
		m_tFrame.dwFrameSpeed = 400;

		hMemDC = CBmpManager::Get_Instance()->Find_Image(L"DigeniePop");
		GdiTransparentBlt(_DC, m_tRect.left - 5, m_tRect.top - 20
			, 50, 60, hMemDC
			, 0, 0
			, 70, 70
			, RGB(100, 100, 100));
		break;
	}

	if (CKeyManager::Get_Instance()->Key_Pressing(VK_LCONTROL))
		Rectangle(_DC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CDigenie::Release()
{
}

void CDigenie::Key_Check()
{
	if (CKeyManager::Get_Instance()->Key_Pressing(VK_LEFT))
	{
		m_tInfo.fX -= m_tInfo.fSpeed;
		m_eCurDir = OBJDIR::LEFT;
		m_tInfo.PlayerDir = m_eCurDir;
	}
	else if (CKeyManager::Get_Instance()->Key_Pressing(VK_RIGHT))
	{
		m_tInfo.fX += m_tInfo.fSpeed;
		m_eCurDir = OBJDIR::RIGHT;
		m_tInfo.PlayerDir = m_eCurDir;
	}
	else if (CKeyManager::Get_Instance()->Key_Pressing(VK_UP))
	{
		m_tInfo.fY -= m_tInfo.fSpeed;
		m_eCurDir = OBJDIR::TOP;
		m_tInfo.PlayerDir = m_eCurDir;
	}
	else if (CKeyManager::Get_Instance()->Key_Pressing(VK_DOWN))
	{
		m_tInfo.fY += m_tInfo.fSpeed;
		m_eCurDir = OBJDIR::BOTTOM;
		m_tInfo.PlayerDir = m_eCurDir;
	}
	else
		m_eCurDir = OBJDIR::IDLE;

	if (CKeyManager::Get_Instance()->Key_Down(VK_SPACE))
	{
		CSoundMgr::Get_Instance()->PlaySound(L"bomb_set.mp3", CSoundMgr::BOMB);
		if (m_tInfo.iMaxBomb > CObjManager::Get_Instance()->Get_BombSize())
		{
			int iBombPosTemp;
			if (((int)m_tInfo.fX % TILECX) > 5)
			{
				iBombPosTemp = ((int)m_tInfo.fX / TILECX) + 1;
			}
			else
				iBombPosTemp = ((int)m_tInfo.fX / TILECX);

			int fBombX = (int)(TILECX * iBombPosTemp);
			int fBombY = (int)(TILECY * ((int)m_tInfo.fY / TILECY));

			CObj* pObj = CAbstractFactory<CBomb>::Create(fBombX, fBombY + 15, m_tInfo.iBombPower, false);
			CObjManager::Get_Instance()->Add_Object(pObj, OBJID::BOMB);
		}
	}
}

void CDigenie::Scene_Change()
{
	switch (m_tInfo.PlayerDir)
	{
	case OBJDIR::IDLE:
		m_eCurDir = m_tInfo.PlayerDir;
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 3;
		m_tFrame.iFrameScene = OBJDIR::IDLE;
		m_tFrame.dwFrameTime = GetTickCount();
		m_tFrame.dwFrameSpeed = 500;
		break;
	case OBJDIR::TOP:
		m_eCurDir = m_tInfo.PlayerDir;
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 3;
		m_tFrame.iFrameScene = OBJDIR::TOP;
		m_tFrame.dwFrameTime = GetTickCount();
		m_tFrame.dwFrameSpeed = 200;
		break;
	case OBJDIR::BOTTOM:
		m_eCurDir = m_tInfo.PlayerDir;
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 3;
		m_tFrame.iFrameScene = OBJDIR::BOTTOM;
		m_tFrame.dwFrameTime = GetTickCount();
		m_tFrame.dwFrameSpeed = 200;
		break;
	case OBJDIR::LEFT:
		m_eCurDir = m_tInfo.PlayerDir;
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 3;
		m_tFrame.iFrameScene = OBJDIR::LEFT;
		m_tFrame.dwFrameTime = GetTickCount();
		m_tFrame.dwFrameSpeed = 200;
		break;
	case OBJDIR::RIGHT:
		m_eCurDir = m_tInfo.PlayerDir;
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 3;
		m_tFrame.iFrameScene = OBJDIR::RIGHT;
		m_tFrame.dwFrameTime = GetTickCount();
		m_tFrame.dwFrameSpeed = 200;
		break;
	};
	m_ePreDir = m_eCurDir;

	/*if (m_ePreDir != m_eCurDir)
	{
		switch (m_eCurDir)
		{
		case OBJDIR::IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = OBJDIR::IDLE;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 500;
			break;
		case OBJDIR::TOP:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = OBJDIR::TOP;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 200;
			break;
		case OBJDIR::BOTTOM:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = OBJDIR::BOTTOM;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 200;
			break;
		case OBJDIR::LEFT:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = OBJDIR::LEFT;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 200;
			break;
		case OBJDIR::RIGHT:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = OBJDIR::RIGHT;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.dwFrameSpeed = 200;
			break;
		}
		m_ePreDir = m_eCurDir;
	}*/
}

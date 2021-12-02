#include "stdafx.h"
#include "Skate.h"
#include "BmpManager.h"

CSkate::CSkate()
{
}


CSkate::~CSkate()
{
	Release();
}

void CSkate::Initialize()
{
	m_tInfo.iCX = 40;
	m_tInfo.iCY = 40;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 1;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.dwFrameSpeed = 1000;
}

int CSkate::Update()
{
	if (m_tInfo.bisDead)
		return OBJ_DEAD;

	Update_Rect();
	Frame_Move();
	return OBJ_NOEVENET;
}

void CSkate::Late_Update()
{
}

void CSkate::Render(HDC _DC)
{
	HDC hMemDC = CBmpManager::Get_Instance()->Find_Image(L"Skate");
	GdiTransparentBlt(_DC, m_tRect.left, m_tRect.top,
		m_tInfo.iCX, m_tInfo.iCY, hMemDC,
		m_tFrame.iFrameStart * 50, 0,
		50, 62,
		RGB(68, 168, 27));
}

void CSkate::Release()
{
}

#pragma once

#ifndef __PLAYER_H__
#define __PLAYER_H__


#include "Obj.h"
class CPlayer : public CObj
{
public:
	CPlayer();
	virtual ~CPlayer();

public:
	virtual void Initialize();
	virtual int Update();
	virtual void Late_Update();
	virtual void Render(HDC _DC);
	virtual void Release();

public:
	float Get_PlayerX() { return m_tInfo.fX; }
	float Get_PlayerY() { return m_tInfo.fY; }
	OBJDIR::DIR Get_PlayerDir() { return m_tInfo.PlayerDir; }
	OBJSTATE::STATE Get_PlayerState() { return m_eState; }

	void Set_PlayerX(float fX) { m_tInfo.fX = fX; }
	void Set_PlayerY(float fY) { m_tInfo.fY = fY; }

public:
	void Set_PlayerBombCount(int _ballonitem) {
		if (m_tInfo.iMaxBomb == 7)
			return;

		m_tInfo.iMaxBomb += _ballonitem;
	}
	void Set_PlayerSpeed(int _skateitem) { m_tInfo.fSpeed += _skateitem; }
	void Set_PlayerSpeed(float _fSpeed) { m_tInfo.fSpeed = _fSpeed; }
	void Set_PlayerBombMax() { m_tInfo.iBombPower = 7; }
	void Set_PlayerBombPower(int _potionitem) { m_tInfo.iBombPower += _potionitem; }


private:
	void Key_Check();

public:
	void Scene_Change();

	map<int, CLIENTINFO> WorldInfo;

};


#endif // !__PLAYER_H__
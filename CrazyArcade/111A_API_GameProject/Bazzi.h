#pragma once

#ifndef __BAZZI_H__
#define __BAZZI_H__

#include "Player.h"
class CBazzi : public CPlayer
{
public:
	CBazzi();
	virtual ~CBazzi();

public:
	virtual void Initialize();
	virtual int Update();
	virtual void Late_Update();
	virtual void Render(HDC _DC);
	virtual void Release();

public:
	float Get_PlayerX() { return m_tInfo.fX; }
	float Get_PlayerY() { return m_tInfo.fY; }

public:
	void Set_PlayerBombCount(int _ballonitem) {
		if (m_tInfo.iMaxBomb == 7)
			return;

		m_tInfo.iMaxBomb += _ballonitem;
	}
	void Set_PlayerSpeed(int _skateitem) { m_tInfo.fSpeed += _skateitem; }
	void Set_PlayerBombMax() { m_tInfo.iBombPower = 7; }
	void Set_PlayerBombPower(int _potionitem) { m_tInfo.iBombPower += _potionitem; }

	void SetPlayerInfo(PLAYERINFO _PlayerInfo);

private:
	void Key_Check();

public:
	void Scene_Change();
};


#endif // !__BAZZI_H__

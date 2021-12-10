#pragma once

#ifndef __OBJMANAGER_H__
#define __OBJMANAGER_H__

class CObj;
class CObjManager
{
public:
	CObjManager();
	~CObjManager();

public:
	void Update();
	void Late_Update();
	void Render(HDC _DC);
	void Release();

public:
	void Add_Object(CObj* _pObj, OBJID::ID _eID) { m_listObj[_eID].emplace_back(_pObj); }
	void Add_Object_MapBlock(CObj* _pObj, MAPBLOCK::BLOCK _eBlock) { m_listMapBLOCK[_eBlock].emplace_back(_pObj); }
	void Add_Object_Item(CObj* _pObj, GAMEITEM::ITEM _eItem) { m_listItem[_eItem].emplace_back(_pObj); }

public:
	float Get_PlayerX();
	float Get_PlayerY();
	RECT Get_PlayerSize();
	float Get_BombX();
	float Get_BombY();
	bool Get_isBombPos(float _x, float _y);
	OBJDIR::DIR Get_PlayerDir();

	void Set_PlayerX(float fX);
	void Set_PlayerY(float fY);

public:
	int Get_BombSize() { return m_listObj[OBJID::BOMB].size(); }

public:
	void Picking_Object(CObj* _pObj, MAPBLOCK::BLOCK _block);
	void Save_Object();
	void Load_Object();
	void Save_Object_Stage2();
	void Load_Object_Stage2();
	void Save_Object_Boss();
	void Load_Object_Boss();

public:
	int Get_Stage1Clear() { return m_iStage1Clear; }
	int Get_BossHP() { return m_BossHP; }
	void Set_Stage1Clear(int _cnt) { m_iStage1Clear += _cnt; }
	void Set_BossHP(int _att) { m_BossHP += _att; }

	void Set_Cheat(bool _ischeat) { m_bisCheat = _ischeat; }
	bool Get_Cheat() { return m_bisCheat; }

	list<CObj*>& Get_List(OBJID::ID _eID) { return m_listObj[_eID]; }
	CObj* Get_Player() { return m_listObj[OBJID::PLAYER].front(); }
	
	void Update_MonsterInfo(vector<MONSTERINFO> vInfo);
	void Add_Monster(MONSTERINFO info, int iNum);
	list<CObj*> Get_MonsterList() { return m_listObj[OBJID::MONSTER]; }
	void Set_MonsterList(list<CObj*>& listM) { m_listObj[OBJID::MONSTER] = listM; }

	// 모든 클라이언트의 플레이어 동기화
	void Add_NetWorkPlayer(CLIENTINFO _playerinfo);
	void Update_NetWorkPlayer(CLIENTINFO _playerinfo);

	// 모든 클라이언트의 폭탄 동기화
	void Add_Bomb(OBJPOS _pos, int _bombPower);

public:
	static CObjManager* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CObjManager;
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}



private:
	list<CObj*>				m_listObj[OBJID::END];
	list<CObj*>				m_listMapBLOCK[MAPBLOCK::END];
	list<CObj*>				m_listItem[GAMEITEM::END];
	static CObjManager*		m_pInstance;

	bool					m_bisBombPlayerCollid;
	int						m_iStage1Clear;
	int						m_BossHP;

	bool					m_bisCheat;

};


#endif // !__OBJMANAGER_H__
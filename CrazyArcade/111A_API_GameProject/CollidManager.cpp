#include "stdafx.h"
#include "CollidManager.h"
#include "Obj.h"
#include "StageMap.h"
#include "Bomb.h"
#include "BombWave.h"
#include "Player.h"
#include "SoundMgr.h"
#include "Mbape.h"

CCollidManager::CCollidManager()
{
}


CCollidManager::~CCollidManager()
{
}

void CCollidManager::Collision_Rect(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	RECT rc = {};

	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (IntersectRect(&rc, &Dst->Get_Rect(), &Src->Get_Rect()))
			{
				//Dst->Set_Dead();
				Src->SetState(OBJSTATE::BUBBLE);
				//Src->Set_Dead();
			}
		}
	}
}

void CCollidManager::Collision_Rect_PlayerToBallon(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	RECT rc = {};

	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (IntersectRect(&rc, &Dst->Get_Rect(), &Src->Get_Rect()))
			{
				CSoundMgr::Get_Instance()->PlaySound(L"eat_item.mp3", CSoundMgr::ITEM);
				dynamic_cast<CPlayer*>(Dst)->Set_PlayerBombCount(1);
				Src->Set_Dead();
			}
		}
	}
}

void CCollidManager::Collision_Rect_PlayerToSkate(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	RECT rc = {};

	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (IntersectRect(&rc, &Dst->Get_Rect(), &Src->Get_Rect()))
			{
				CSoundMgr::Get_Instance()->PlaySound(L"eat_item.mp3", CSoundMgr::ITEM);
				dynamic_cast<CPlayer*>(Dst)->Set_PlayerSpeed(1.0f);
				Src->Set_Dead();
			}
		}
	}
}

void CCollidManager::Collision_Rect_PlayerToPotion(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	RECT rc = {};

	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (IntersectRect(&rc, &Dst->Get_Rect(), &Src->Get_Rect()))
			{
				CSoundMgr::Get_Instance()->PlaySound(L"eat_item.mp3", CSoundMgr::ITEM);
				dynamic_cast<CPlayer*>(Dst)->Set_PlayerBombPower(1);
				Src->Set_Dead();
			}
		}
	}
}

void CCollidManager::Collision_Rect_PlayerToMaxPotion(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	RECT rc = {};

	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (IntersectRect(&rc, &Dst->Get_Rect(), &Src->Get_Rect()))
			{
				CSoundMgr::Get_Instance()->PlaySound(L"eat_item.mp3", CSoundMgr::ITEM);
				dynamic_cast<CPlayer*>(Dst)->Set_PlayerBombMax();
				Src->Set_Dead();
			}
		}
	}
}

void CCollidManager::Collision_Rect_BombWaveToBlock(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	RECT rc = {};

	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			// Dst = 웨이브				Src = 블록
			if (IntersectRect(&rc, &Dst->Get_Rect(), &Src->Get_Rect()))
			{
				//Dst->Set_Dead();

				Src->SetState(OBJSTATE::BUBBLE);

				//dynamic_cast<CBombWave*>(Dst)->SetCollid();
			}

		}
	}
}

void CCollidManager::Collision_Rect_BombWaveToMonster(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	RECT rc = {};

	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			// Dst = 웨이브				Src = 블록
			if (IntersectRect(&rc, &Dst->Get_Rect(), &Src->Get_Rect()))
			{
				//Dst->Set_Dead();

				Src->SetState(OBJSTATE::HIT);
				//Src->SetState(OBJSTATE::DEAD);

				//dynamic_cast<CBombWave*>(Dst)->SetCollid();
			}

		}
	}
}

void CCollidManager::Collision_Rect_PlayerToBoss(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	RECT rc = {};

	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			// Dst = 플레이어				Src = 보스
			if (IntersectRect(&rc, &Dst->Get_Rect(), &Src->Get_Rect()))
			{
				if (Src->GetState() == OBJSTATE::BUBBLE)
					Src->SetState(OBJSTATE::DEAD);
				else
					Dst->SetState(OBJSTATE::DEAD);
			}

		}
	}
}

void CCollidManager::Collision_Rect_PlayerToMonster(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	RECT rc = {};

	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			// Dst = 플레이어				Src = 보스
			if (IntersectRect(&rc, &Dst->Get_Rect(), &Src->Get_Rect()))
			{
				Dst->SetState(OBJSTATE::DEAD);
			}

		}
	}
}



bool CCollidManager::isCollision_Rect_BombWaveToBlock(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	RECT rc = {};

	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (IntersectRect(&rc, &Dst->Get_Rect(), &Src->Get_Rect()))
			{
				return true;
			}
			else
				return false;
		}
	}
}

void CCollidManager::Collision_Rect_BombToBombWave(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	RECT rc = {};

	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (IntersectRect(&rc, &Dst->Get_Rect(), &Src->Get_Rect()))
			{
				Dst->Set_Dead();
				Src->Set_Dead();
			}
		}
	}
}

void CCollidManager::Collision_Rect_NoPushBombToPlayer(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	float fX = 0.f, fY = 0.f;
	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (Check_Rect(Dst, Src, &fX, &fY))
			{
				if (fX * fY < 160)
				{
					if (fX > fY)
					{
						if (Dst->Get_Info().fY < Src->Get_Info().fY)
							Dst->Set_PosY(-fY);
						else
							Dst->Set_PosY(fY);
					}
					else
					{
						if (Dst->Get_Info().fX < Src->Get_Info().fX)
							Dst->Set_PosX(-fX);
						else
							Dst->Set_PosX(fX);
					}
				}
			}
		}
	}
}

void CCollidManager::Collision_Rect_NoPush(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	float fX = 0.f, fY = 0.f;
	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (Check_Rect(Dst, Src, &fX, &fY))
			{
					if (fX > fY)
					{
						if (Dst->Get_Info().fY < Src->Get_Info().fY)
							Dst->Set_PosY(-fY);
						else
							Dst->Set_PosY(fY);
					}
					else
					{
						if (Dst->Get_Info().fX < Src->Get_Info().fX)
							Dst->Set_PosX(-fX);
						else
							Dst->Set_PosX(fX);
					}

			}
		}
	}
}

void CCollidManager::Collision_Rect_NoPush(list<CObj*>& _Dst, list<CStageMap*>& _Src)
{
	float fX = 0.f, fY = 0.f;
	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (Check_Rect(Dst, Src, &fX, &fY))
			{
				if (fX > fY)
				{
					if (Dst->Get_Info().fY < Src->Get_Info().fY)
						Dst->Set_PosY(-fY);
					else
						Dst->Set_PosY(fY);
				}
				else
				{
					if (Dst->Get_Info().fX < Src->Get_Info().fX)
						Dst->Set_PosX(-fX);
					else
						Dst->Set_PosX(fX);
				}
			}
		}
	}
}

void CCollidManager::Collision_Sphere(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (Check_Sphere(Dst, Src))
			{
				Dst->Set_Dead();
				Src->Set_Dead();
			}
		}
	}
}

bool CCollidManager::Check_Sphere(CObj * _Dst, CObj * _Src)
{
	float fX = abs(_Dst->Get_Info().fX - _Src->Get_Info().fX);
	float fY = _Dst->Get_Info().fY - _Src->Get_Info().fY;
	float fDia = sqrtf(fX * fX + fY * fY);

	float fRad = (float)((_Dst->Get_Info().iCX + _Src->Get_Info().iCX) >> 1);

	if (fRad > fDia)
		return true;
	return false;
}

bool CCollidManager::Check_Rect(CObj * _Dst, CStageMap * _Src, float * _x, float * _y)
{
	float fDisX = abs(_Dst->Get_Info().fX - _Src->Get_Info().fX);
	float fDisY = abs(_Dst->Get_Info().fY - _Src->Get_Info().fY);

	float fCX = (float)((_Dst->Get_Info().iCX + _Src->Get_Info().iCX) >> 1);
	float fCY = (float)((_Dst->Get_Info().iCY + _Src->Get_Info().iCY) >> 1);

	if (fDisX <= fCX && fDisY <= fCY)
	{
		*_x = fCX - fDisX;
		*_y = fCY - fDisY;
		return true;
	}
	return false;
}

bool CCollidManager::Check_Rect(CObj * _Dst, CObj * _Src, float * _x, float * _y)
{
	float fDisX = abs(_Dst->Get_Info().fX - _Src->Get_Info().fX);
	float fDisY = abs(_Dst->Get_Info().fY - _Src->Get_Info().fY);

	float fCX = (float)((_Dst->Get_Info().iCX + _Src->Get_Info().iCX) >> 1);
	float fCY = (float)((_Dst->Get_Info().iCY + _Src->Get_Info().iCY) >> 1);

	if (fDisX <= fCX && fDisY <= fCY)
	{
		*_x = fCX - fDisX;
		*_y = fCY - fDisY;
		return true;
	}
	return false;
}

bool CCollidManager::isCollid(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	RECT rc = {};

	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (IntersectRect(&rc, &Dst->Get_Rect(), &Src->Get_Rect()))
			{
				
				return true;
			}
		}
	}
	return false;
}

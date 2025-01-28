#include "CEntity.h"

bool CEntity::Update()
{
	m_vOldOrigin	  = m.Read<Vector3>(CCSPlayerPawn + offset::m_vOldOrigin);
	m_iHealth		  = m.Read<int>(CCSPlayerPawn + offset::m_iHealth);
	
	if (Vec3_Empty(m_vOldOrigin) || m_iHealth <= 0)
		return false;

	/*
	m_pBoneArray		  = m.Read<uintptr_t>(address + offset::m_nForceBone);
	m_vecAbsVelocity  = m.Read<Vector3>(address + offset::m_vecAbsVelocity);
	*/

	return true;
}

bool CEntity::UpdateStatic(const uintptr_t& entitylist)
{
	// pointer
	uint32_t pawn = m.Read<uint32_t>(address + offset::m_hPlayerPawn);
	uintptr_t list = m.Read<uintptr_t>(entitylist + 0x8 * ((pawn & 0x7FFF) >> 9) + 0x10);

	if (list == NULL)
		return false;
	
	// pointers
	CCSPlayerPawn = m.Read<uintptr_t>(list + 120 * (pawn & 0x1FF));
	m_pGameSceneNode = m.Read<uintptr_t>(CCSPlayerPawn + offset::m_pGameSceneNode);
	m_pCameraServices = m.Read<uintptr_t>(CCSPlayerPawn + offset::m_pCameraServices);
	m_pBoneArray = m.Read<uintptr_t>(m_pGameSceneNode + (offset::m_modelState + 0x80));

	// entity data
	m_iTeamNum = m.Read<int>(address + offset::m_iTeamNum);
	m_iMaxHealth = m.Read<int>(CCSPlayerPawn + offset::m_iMaxHealth);

	return true;
}

Vector3 CEntity::vecMin()
{
	uintptr_t temp = m.Read<uintptr_t>(CCSPlayerPawn + offset::m_pCollision);
	return m.Read<Vector3>(temp + 0x40) + m_vOldOrigin;
}

Vector3 CEntity::vecMax()
{
	uintptr_t temp = m.Read<uintptr_t>(CCSPlayerPawn + offset::m_pCollision);
	return m.Read<Vector3>(temp +  0x4C) + m_vOldOrigin;
}

uint32_t CEntity::GetFlag()
{
	return m.Read<int>(address + offset::m_fFlags);
}

bool CEntity::IsDead()
{
	return m.Read<int>(address + offset::m_lifeState) > 0;
}

std::vector<Vector3> CEntity::GetBoneList()
{
	AllBones ab = m.Read<AllBones>(m_pBoneArray), *all_bone = &ab;
	std::vector<Vector3> list(32);

	for (int i = 0; i < 32; i++)
	{
		if (Vec3_Empty(all_bone->bone[i].position)) {
			list.clear();
			return list;
		}
		
		list[i] = all_bone->bone[i].position;
	}

	return list;
}
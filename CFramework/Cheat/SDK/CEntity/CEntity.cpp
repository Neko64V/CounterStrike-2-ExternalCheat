#include "CEntity.h"

// 毎フレーム更新する必要のある情報
bool CEntity::Update()
{
	m_vOldOrigin	  = m.Read<Vector3>(m_pCSPlayerPawn + offset::m_vOldOrigin);
	m_iHealth		  = m.Read<int>(m_pCSPlayerPawn + offset::m_iHealth);
	
	if (Vec3_Empty(m_vOldOrigin) || m_iHealth <= 0)
		return false;

	m_ArmorValue = m.Read<int>(m_pCSPlayerPawn + offset::m_ArmorValue);
	m_iIDEntIndex = m.Read<uint32_t>(m_pCSPlayerPawn + offset::m_iIDEntIndex);

	return true;
}

bool CEntity::TriggerAllow(const uintptr_t& entitylist, CEntity* local)
{
	// pointer
	uintptr_t list = m.Read<uintptr_t>(entitylist + 0x8 * ((local->m_iIDEntIndex & 0x7FFF) >> 9) + 0x10);
	m_pCSPlayerPawn = m.Read<uintptr_t>(list + 120 * (local->m_iIDEntIndex & 0x1FF));

	if (!Update())
		return false;

	m_iTeamNum = m.Read<int>(address + offset::m_iTeamNum);

	return true;
}

// 毎フレーム更新する必要のない情報
bool CEntity::UpdateStatic(const uintptr_t& entitylist)
{
	// pointer
	uint32_t pawn = m.Read<uint32_t>(address + offset::m_hPlayerPawn);
	uintptr_t list = m.Read<uintptr_t>(entitylist + 0x8 * ((pawn & 0x7FFF) >> 9) + 0x10);

	if (list == NULL)
		return false;

	// pointers
	m_pCSPlayerPawn = m.Read<uintptr_t>(list + 120 * (pawn & 0x1FF));
	m_pCollision = m.Read<uintptr_t>(m_pCSPlayerPawn + offset::m_pCollision);
	m_pGameSceneNode = m.Read<uintptr_t>(m_pCSPlayerPawn + offset::m_pGameSceneNode);
	m_pBoneArray = m.Read<uintptr_t>(m_pGameSceneNode + (offset::m_modelState + 0x80));
	m_pCameraServices = m.Read<uintptr_t>(m_pCSPlayerPawn + offset::m_pCameraServices);
	m_pClippingWeapon = m.Read<uintptr_t>(m_pCSPlayerPawn + offset::m_pClippingWeapon);

	// entity data
	m_iTeamNum = m.Read<int>(address + offset::m_iTeamNum);
	m_iMaxHealth = m.Read<int>(m_pCSPlayerPawn + offset::m_iMaxHealth);
	
	// Name
	uintptr_t nameAddress = m.Read<uintptr_t>(address + offset::m_sSanitizedPlayerName);

	if (nameAddress != NULL) {
		m.ReadString(nameAddress, &m_namePlayer, sizeof(m_namePlayer));
	}

	// Weapon name
	uintptr_t pWeaponEntity = m.ReadChain(m_pClippingWeapon, { 0x10, 0x20 });

	if (pWeaponEntity != NULL) {
		m_nameWeapon = m.ReadStringA(pWeaponEntity);

		if (m_nameWeapon.find("weapon_") != std::string::npos)
			m_nameWeapon = m_nameWeapon.substr(7, m_nameWeapon.length());
	}

	return true;
}

Vector3 CEntity::vecMin()
{
	return m.Read<Vector3>(m_pCollision + 0x40) + m_vOldOrigin;
}

Vector3 CEntity::vecMax()
{
	return m.Read<Vector3>(m_pCollision +  0x4C) + m_vOldOrigin;
}

uint32_t CEntity::GetFlag()
{
	return m.Read<int>(address + offset::m_fFlags);
}

bool CEntity::IsDead()
{
	return m.Read<int>(address + offset::m_lifeState) > 0;
}

Vector3 CEntity::GetBoneByID(const int ID)
{
	return m.Read<Vector3>(m_pBoneArray + (ID * 0x20));
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
#pragma once
#include "../GameSDK/GameSDK.h"

struct Bone {
	Vector3 position;
	uint64_t address[2];
};

struct AllBones {
	Bone bone[128]{};
};

class CEntity
{
public:
    uintptr_t address;
	uintptr_t CCSPlayerPawn;
	uintptr_t m_pGameSceneNode;
	uintptr_t m_pCameraServices;

	// Player
	Vector3 m_vecAbsVelocity;
	Vector3 m_vOldOrigin;

	int m_iTeamNum;
	int m_lifeState;
	int m_iHealth;
	int m_iMaxHealth;
	uintptr_t m_pBoneArray;

    // Functions
	bool Update();
	bool UpdateStatic(const uintptr_t& entitylist);
	bool IsDead();

	Vector3 vecMin();
	Vector3 vecMax();

	uint32_t GetFlag();
	std::vector<Vector3> GetBoneList();
};
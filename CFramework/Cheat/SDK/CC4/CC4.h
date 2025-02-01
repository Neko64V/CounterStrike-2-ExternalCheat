#pragma once
#include "../GameSDK/GameSDK.h"

class CC4
{
public:
    uintptr_t address;

	uintptr_t GetGameSceneNode()
	{
		return m.Read<uintptr_t>(address + offset::m_pGameSceneNode);
	}
	float GetTimer()
	{
		return m.Read<float>(address + 0xFC0);
	}
	Vector3 GetAbsOrigin()
	{
		return m.Read<Vector3>(GetGameSceneNode() + offset::m_vecAbsOrigin);
	}

    // Functions
	bool Update(const uintptr_t& entitylist);
};
#pragma once
#include "../../../Framework/Utils/Utils.h"
#include "a2x/offsets.hpp"
#include "a2x/buttons.hpp"
#include "a2x/client_dll.hpp"

namespace offset
{
	// Address
	constexpr auto dwEntityList		= cs2_dumper::offsets::client_dll::dwEntityList;
	constexpr auto dwLocalPlayerController	= cs2_dumper::offsets::client_dll::dwLocalPlayerController;
	constexpr auto dwViewRender		= cs2_dumper::offsets::client_dll::dwViewRender;
	constexpr auto dwViewMatrix		= cs2_dumper::offsets::client_dll::dwViewMatrix;
	constexpr auto dwViewAngles		= cs2_dumper::offsets::client_dll::dwViewAngles;
	constexpr auto dwGlobalVars		= cs2_dumper::offsets::client_dll::dwGlobalVars;
	constexpr auto dwPlantedC4		= cs2_dumper::offsets::client_dll::dwPlantedC4;
	
	// Button
	constexpr auto attack = 0x1875710;
	constexpr auto attack2 = 0x18757A0;
	constexpr auto back = 0x18759E0;
	constexpr auto duck = 0x1875CB0;
	constexpr auto forward = 0x1875950;
	constexpr auto jump = 0x1875C20;
	constexpr auto left = 0x1875A70;
	constexpr auto lookatweapon = 0x1A9DF50;
	constexpr auto reload = 0x1875680;
	constexpr auto right = 0x1875B00;
	constexpr auto showscores = 0x1A9DE30;
	constexpr auto sprint = 0x18755F0;
	constexpr auto turnleft = 0x1875830;
	constexpr auto turnright = 0x18758C0;
	constexpr auto use = 0x1875B90;
	constexpr auto zoom = 0x1A9DEC0;

	// CCSPlayerControlle
	constexpr auto m_hPlayerPawn = cs2_dumper::schemas::client_dll::CCSPlayerController::m_hPlayerPawn;
	constexpr auto m_sSanitizedPlayerName = cs2_dumper::schemas::client_dll::CCSPlayerController::m_sSanitizedPlayerName;
	constexpr auto m_fFlags = cs2_dumper::schemas::client_dll::C_BaseEntity::m_fFlags;
	constexpr auto m_iHealth = cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth;
	constexpr auto m_iMaxHealth = cs2_dumper::schemas::client_dll::C_BaseEntity::m_iMaxHealth;
	constexpr auto m_iTeamNum = cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum;
	constexpr auto m_lifeState = cs2_dumper::schemas::client_dll::C_BaseEntity::m_lifeState;
	constexpr auto m_pCollision = cs2_dumper::schemas::client_dll::C_BaseEntity::m_pCollision;
	constexpr auto m_pGameSceneNode = cs2_dumper::schemas::client_dll::C_BaseEntity::m_pGameSceneNode;
	constexpr auto m_ArmorValue = cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_ArmorValue;
	constexpr auto m_vecLastClipCameraPos = cs2_dumper::schemas::client_dll::C_CSPlayerPawnBase::m_vecLastClipCameraPos;
	constexpr auto m_angEyeAngles = cs2_dumper::schemas::client_dll::C_CSPlayerPawnBase::m_angEyeAngles;
	constexpr auto m_iIDEntIndex = cs2_dumper::schemas::client_dll::C_CSPlayerPawnBase::m_iIDEntIndex;
	constexpr auto m_pClippingWeapon = cs2_dumper::schemas::client_dll::C_CSPlayerPawnBase::m_pClippingWeapon;
	constexpr auto m_vOldOrigin = cs2_dumper::schemas::client_dll::C_BasePlayerPawn::m_vOldOrigin;
	constexpr auto m_pCameraServices = cs2_dumper::schemas::client_dll::C_BasePlayerPawn::m_pCameraServices;
	constexpr auto m_modelState = cs2_dumper::schemas::client_dll::CSkeletonInstance::m_modelState;
	constexpr auto m_szName = cs2_dumper::schemas::client_dll::CCSWeaponBaseVData::m_szName;

	// C4
	constexpr auto m_flTimerLength = cs2_dumper::schemas::client_dll::C_PlantedC4::m_flTimerLength;
	constexpr auto m_vecAbsOrigin = cs2_dumper::schemas::client_dll::CGameSceneNode::m_vecAbsOrigin;
};

enum CBoneID
{
	BONE_HEAD = 6,
	BONE_NECK = 5,
	BONE_SPINE = 4,
	BONE_SPINE_1 = 2,
	BONE_HIP = 0,
	BONE_LEFT_SHOULDER = 8,
	BONE_LEFT_ARM = 9,
	BONE_LEFT_HAND = 10,
	BONE_RIGHT_SHOULDER = 13,
	BONE_RIGHT_ARM = 14,
	BONE_RIGHT_HAND = 15,
	BONE_LEFT_HIP = 22,
	BONE_LEFT_KNEE = 23,
	BONE_LEFT_FEET = 24,
	BONE_RIGHT_HIP = 25,
	BONE_RIGHT_KNEE = 26,
	BONE_RIGHT_FEET = 27,
};

extern bool Vec2_Empty(const Vector2& value);
extern bool Vec3_Empty(const Vector3& value);
extern void NormalizeAngles(Vector2& angle);
extern Vector2 CalcAngle(const Vector3& src, const Vector3& dst);
extern bool WorldToScreen(Matrix ViewMatrix, RECT Size, Vector3 vIn, Vector2& vOut);
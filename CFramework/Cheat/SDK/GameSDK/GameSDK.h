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

	// CCSPlayerControlle
	constexpr auto m_hPlayerPawn = cs2_dumper::schemas::client_dll::CCSPlayerController::m_hPlayerPawn;
	constexpr auto m_sSanitizedPlayerName = cs2_dumper::schemas::client_dll::CCSPlayerController::m_sSanitizedPlayerName;

	// C_BaseEntity
	constexpr auto m_fFlags = cs2_dumper::schemas::client_dll::C_BaseEntity::m_fFlags;
	constexpr auto m_iHealth = cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth;
	constexpr auto m_iMaxHealth = cs2_dumper::schemas::client_dll::C_BaseEntity::m_iMaxHealth;
	constexpr auto m_iTeamNum = cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum;
	constexpr auto m_lifeState = cs2_dumper::schemas::client_dll::C_BaseEntity::m_lifeState;
	constexpr auto m_pCollision = cs2_dumper::schemas::client_dll::C_BaseEntity::m_pCollision;
	constexpr auto m_pGameSceneNode = cs2_dumper::schemas::client_dll::C_BaseEntity::m_pGameSceneNode;

	// C_BasePlayerPawn
	constexpr auto m_vOldOrigin = cs2_dumper::schemas::client_dll::C_BasePlayerPawn::m_vOldOrigin;
	constexpr auto m_pCameraServices = cs2_dumper::schemas::client_dll::C_BasePlayerPawn::m_pCameraServices;

	// CSkeletonInstance
	constexpr auto m_modelState = cs2_dumper::schemas::client_dll::CSkeletonInstance::m_modelState;

	// dev
	constexpr auto m_ArmorValue = cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_ArmorValue;
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

extern bool Vec3_Empty(const Vector3& value);
extern bool WorldToScreen(Matrix ViewMatrix, RECT Size, Vector3 vIn, Vector2& vOut);
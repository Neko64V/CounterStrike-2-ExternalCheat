#include "GameSDK.h"

bool GameSDK::InitOffset()
{
    auto info = m.GetModuleInfo("client.dll");
    std::vector<uint8_t> bytes = m.ReadBytes((uintptr_t)info.lpBaseOfDll, info.SizeOfImage);

    dwEntityList = m.FindPattern(bytes, "48 89 35 ?? ?? ?? ?? 48 85 f6", 3, 7);
    printf("dwEntityList : 0x%I64x\n", dwEntityList);

    dwViewMatrix = m.FindPattern(bytes, "48 8d 0d ?? ?? ?? ?? 48 c1 e0 06", 3, 7);
    printf("dwViewMatrix : 0x%I64x\n", dwViewMatrix);

    dwPlantedC4 = m.FindPattern(bytes, "48 8b 15 ?? ?? ?? ?? 41 ff c0", 3, 7);
    printf("dwPlantedC4 : 0x%I64x\n", dwPlantedC4);

    dwLocalPlayerController = m.FindPattern(bytes, "48 89 05 ?? ?? ?? ?? 8b 9e", 3, 7);
    printf("dwLocalPlayerController : 0x%I64x\n", dwLocalPlayerController);

    bytes.clear();

    return dwEntityList != 0 && dwLocalPlayerController != 0 && dwViewMatrix != 0;
}

bool Vec2_Empty(const Vector2& value)
{
    return value == Vector2();
}

bool Vec3_Empty(const Vector3& value)
{
    return value == Vector3();
}

void NormalizeAngles(Vector2& angle)
{
    while (angle.x > 89.0f)
        angle.x -= 180.f;

    while (angle.x < -89.0f)
        angle.x += 180.f;

    while (angle.y > 180.f)
        angle.y -= 360.f;

    while (angle.y < -180.f)
        angle.y += 360.f;
}

Vector2 CalcAngle(const Vector3& src, const Vector3& dst)
{
    Vector2 angle{};
    Vector3 delta = Vector3((src.x - dst.x), (src.y - dst.y), (src.z - dst.z));

    double distance = sqrt(delta.x * delta.x + delta.y * delta.y);

    angle.x = atan(delta.z / distance) * (180.0f / 3.1415926535);
    angle.y = atan(delta.y / delta.x) * (180.0f / 3.1415926535);

    if (delta.x >= 0.0) angle.y += 180.0f;

    return angle;
}

bool WorldToScreen(Matrix ViewMatrix, RECT Size, Vector3 vIn, Vector2& vOut)
{
    float w = ViewMatrix.m[3][0] * vIn.x + ViewMatrix.m[3][1] * vIn.y + ViewMatrix.m[3][2] * vIn.z + ViewMatrix.m[3][3];

    if (w < 0.001)
        return false;

    vOut.x = ViewMatrix.m[0][0] * vIn.x + ViewMatrix.m[0][1] * vIn.y + ViewMatrix.m[0][2] * vIn.z + ViewMatrix.m[0][3];
    vOut.y = ViewMatrix.m[1][0] * vIn.x + ViewMatrix.m[1][1] * vIn.y + ViewMatrix.m[1][2] * vIn.z + ViewMatrix.m[1][3];

    float invw = 1.f / w;

    vOut.x *= invw;
    vOut.y *= invw;

    float x = Size.right / 2;
    float y = Size.bottom / 2;

    x += 0.5 * vOut.x * Size.right + 0.5;
    y -= 0.5 * vOut.y * Size.bottom + 0.5;

    vOut.x = x;
    vOut.y = y;

    return true;
}

GameSDK* Game = new GameSDK();
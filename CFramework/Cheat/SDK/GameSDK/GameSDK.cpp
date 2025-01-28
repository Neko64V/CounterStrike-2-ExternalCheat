#include "GameSDK.h"

bool Vec3_Empty(const Vector3& value)
{
    return value == Vector3();
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
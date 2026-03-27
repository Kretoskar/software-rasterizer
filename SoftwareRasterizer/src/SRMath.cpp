#include "SRMath.h"

#include <cmath>

using namespace SR;

Mat4 Mat4::Identity()
{
    Mat4 result = {};
    result.m[0][0] = 1.0f;
    result.m[1][1] = 1.0f;
    result.m[2][2] = 1.0f;
    result.m[3][3] = 1.0f;
    return result;
}

Mat4 Mat4::Translation(float x, float y, float z)
{
    Mat4 result = Identity();
    result.m[0][3] = x;
    result.m[1][3] = y;
    result.m[2][3] = z;
    return result;
}

Mat4 Mat4::Scale(float x, float y, float z)
{
    Mat4 result = Identity();
    result.m[0][0] = x;
    result.m[1][1] = y;
    result.m[2][2] = z;
    return result;
}

Mat4 Mat4::RotationX(float angleRadians)
{
    Mat4 result = Identity();

    const float c = std::cos(angleRadians);
    const float s = std::sin(angleRadians);

    result.m[1][1] =  c;
    result.m[1][2] = -s;
    result.m[2][1] =  s;
    result.m[2][2] =  c;

    return result;
}

Mat4 Mat4::RotationY(float angleRadians)
{
    Mat4 result = Identity();

    const float c = std::cos(angleRadians);
    const float s = std::sin(angleRadians);

    result.m[0][0] =  c;
    result.m[0][2] =  s;
    result.m[2][0] = -s;
    result.m[2][2] =  c;

    return result;
}

Mat4 Mat4::RotationZ(float angleRadians)
{
    Mat4 result = Identity();

    const float c = std::cos(angleRadians);
    const float s = std::sin(angleRadians);

    result.m[0][0] =  c;
    result.m[0][1] = -s;
    result.m[1][0] =  s;
    result.m[1][1] =  c;

    return result;
}

Mat4 Mat4::RotationXYZ(float xRadians, float yRadians, float zRadians)
{
    Mat4 rx = RotationX(xRadians);
    Mat4 ry = RotationY(yRadians);
    Mat4 rz = RotationZ(zRadians);
    
    return rz * ry * rx;
}

Mat4 Mat4::Perspective(float fovDegrees, float aspect, float nearPlane, float farPlane)
{
    Mat4 result = {};

    const float fovRadians = Deg2Rad(fovDegrees);
    const float f = 1.0f / std::tan(fovRadians * 0.5f);

    // Simple right-handed style projection where camera looks along +Z
    // and clip-space W becomes Z.
    result.m[0][0] = f / aspect;
    result.m[1][1] = f;
    result.m[2][2] = farPlane / (farPlane - nearPlane);
    result.m[2][3] = (-nearPlane * farPlane) / (farPlane - nearPlane);
    result.m[3][2] = 1.0f;

    return result;
}

#pragma once

namespace SR
{
    struct Vec3
    {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
    };

    struct Vec4
    {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
        float w = 0.0f;
    };

    /*  memory access as m[row][column]  
        
        vector convention is COLUMN
        | x |
        | y |
        | z |
        | w |
        
        | 1 0 0 tx |
        | 0 1 0 ty |
        | 0 0 1 tz |
        | 0 0 0  1 |
        
        therefore order of multiplication is RIGHT TO LEFT
     */
    struct Mat4
    {
        float m[4][4] = {};

        static Mat4 Identity();
        static Mat4 Translation(float x, float y, float z);
        static Mat4 Scale(float x, float y, float z);
        static Mat4 RotationY(float angleRadians);
        static Mat4 Perspective(float fovDegrees, float aspect, float nearPlane, float farPlane);
    };

    inline Vec4 operator*(const Mat4& mat, const Vec4& v)
    {
        Vec4 result;
        result.x =
            mat.m[0][0] * v.x +
            mat.m[0][1] * v.y +
            mat.m[0][2] * v.z +
            mat.m[0][3] * v.w;

        result.y =
            mat.m[1][0] * v.x +
            mat.m[1][1] * v.y +
            mat.m[1][2] * v.z +
            mat.m[1][3] * v.w;

        result.z =
            mat.m[2][0] * v.x +
            mat.m[2][1] * v.y +
            mat.m[2][2] * v.z +
            mat.m[2][3] * v.w;

        result.w =
            mat.m[3][0] * v.x +
            mat.m[3][1] * v.y +
            mat.m[3][2] * v.z +
            mat.m[3][3] * v.w;

        return result;
    }
    
    inline Mat4 operator*(const Mat4& a, const Mat4& b)
    {
        Mat4 result = {};

        for (int row = 0; row < 4; ++row)
        {
            for (int col = 0; col < 4; ++col)
            {
                result.m[row][col] =
                    a.m[row][0] * b.m[0][col] +
                    a.m[row][1] * b.m[1][col] +
                    a.m[row][2] * b.m[2][col] +
                    a.m[row][3] * b.m[3][col];
            }
        }

        return result;
    }
    
    inline Vec4 ToVec4(const Vec3& v, float w = 1.0f)
    {
        return { v.x, v.y, v.z, w };
    }
    
    inline float PI = 3.1415926535f;
    constexpr float Deg2Rad(float Deg)
    {
        return Deg * (PI / 180.0f);
    }
}
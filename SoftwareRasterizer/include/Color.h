#pragma once
#include <cstdint>

#include "Types.h"

namespace SR
{
    struct Color32
    {
        union
        {
            u32 value;

            struct
            {
                // matches BGRA byte layout that WIN32 uses
                u8 b;
                u8 g;
                u8 r;
                u8 a;
            };
        };

        constexpr Color32()
            : value(0) {}

        constexpr Color32(u8 red, u8 green, u8 blue, u8 alpha = 255)
            : b(blue), g(green), r(red), a(alpha) {}

        constexpr explicit Color32(u32 packed)
            : value(packed) {}

        constexpr u32 ToU32() const
        {
            return value;
        }

        static constexpr Color32 FromRGBA(u8 red, u8 green, u8 blue, u8 alpha = 255)
        {
            return {red, green, blue, alpha};
        }

        static constexpr Color32 FromPacked(u32 packed)
        {
            return Color32(packed);
        }
    };
}

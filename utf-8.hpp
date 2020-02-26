#pragma once

#include <string>
#include <cstdint>
#include <stdexcept>

std::string codepageToUtf8(uint32_t index)
{
    if (index > 0x1F'FFFF)
        throw std::invalid_argument("code page value is too large");

    if (index <= 0b0111'1111)
        return {static_cast <char> (index)};

    if (index <= 0x7FF)
        return {
            static_cast <char> (static_cast <char> (0b1100'0000) | static_cast <char> ((index >> 6) & 0x1F)),
            static_cast <char> (static_cast <char> (0b1000'0000) | static_cast <char> (index & 0x3F))
        };

    if (index <= 0xFFFF)
        return {
            static_cast <char> (static_cast <char> (0b1110'0000) | static_cast <char> ((index >> 12) & 0xF)),
            static_cast <char> (static_cast <char> (0b1000'0000) | static_cast <char> ((index >> 6) & 0x3F)),
            static_cast <char> (static_cast <char> (0b1000'0000) | static_cast <char> (index & 0x3F))
        };

    return {
        static_cast <char> (static_cast <char> (0b1111'0000) | static_cast <char> ((index >> 18) & 0x7)),
        static_cast <char> (static_cast <char> (0b1000'0000) | static_cast <char> ((index >> 12) & 0x3F)),
        static_cast <char> (static_cast <char> (0b1000'0000) | static_cast <char> ((index >> 6) & 0x3F)),
        static_cast <char> (static_cast <char> (0b1000'0000) | static_cast <char> (index & 0x3F))
    };
}

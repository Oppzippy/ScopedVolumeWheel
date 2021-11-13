#pragma once
#include <Windows.h>
#include <functional>

struct HotKey {
    UINT vk;
    UINT modifiers;

    HotKey()
    {
        this->vk = 0;
        this->modifiers = 0;
    }

    HotKey(UINT vk, UINT modifiers) noexcept
    {
        this->vk = vk;
        this->modifiers = modifiers;
    }

    bool operator==(const HotKey& hotKey) const noexcept
    {
        return this->vk == hotKey.vk && this->modifiers == hotKey.modifiers;
    }
};

namespace std {
template <>
struct hash<HotKey> {
    std::size_t operator()(const HotKey& hotKey) const noexcept
    {
        return std::hash<UINT>()(hotKey.vk) ^ std::hash<UINT>()(hotKey.modifiers);
    }
};
}

#pragma once
#include <cstdint>

class TypeHash
{
private:
    static constexpr uint32_t FNV_OFFSET_BASIS = 2166136261u;
    static constexpr uint32_t FNV_PRIME = 16777619u;

    template <size_t Length>
    static consteval uint32_t hash_impl(const char (&str)[Length])
    {
        uint32_t hash = FNV_OFFSET_BASIS;
        for (auto i = 0; i < Length - 1; ++i) // we skip '\n' by less than Length - 1
        {
            hash ^= static_cast<uint32_t>(str[i]);
            hash *= FNV_PRIME;
        }
        return hash;
    }

public:
    template <typename T>
    static consteval uint32_t index_of()
    {
#if defined(_MSC_VER)
        return hash_impl(__FUNCSIG__);
#elif defined(__GNUC__) || defined(__clang__)
        return hash_impl(__PRETTY_FUNCTION__);
#elif
        static_assert("Unsupported typename backend.");
#endif
    }
};

#pragma once

typedef unsigned int u_int;

typedef u_int Hash;
const Hash uHASH_UNSET = 0;
template<typename T>
static Hash GetHash(const T* pszString)
{
    Hash uHash = uHASH_UNSET;
    for (int i = 0; pszString[i]; ++i) {
        uHash = (uHash * 31) + pszString[i];
    }

    return uHash;
}

template<int iSize>
constexpr Hash __gethash(const char* pszString)
{
    return (__gethash<iSize - 1>(pszString) * 31) + pszString[iSize];
}
// This is the stop-recursion function
template<>
constexpr Hash __gethash<int(-1)>(const char* pszString) { return 0; }
#define CompileTimeGetHash(x) (__gethash<sizeof(x) - 2>(x))

struct Color {
    float fRed;
    float fGreen;
    float fBlue;
    float fAlpha;
};

#pragma once

typedef unsigned int u_int;

typedef u_int Hash;
const Hash uHASH_UNSET = 0;
template<typename T>
static Hash GetHash(const T* pszString)
{
    if (!pszString) { return uHASH_UNSET; }

    Hash uHash = uHASH_UNSET;
    for (u_int u = 0; pszString[u]; u++) {
        uHash *= 31;
        uHash += pszString[u];
    }
    return uHash;
}

struct Color {
    float fRed;
    float fGreen;
    float fBlue;
    float fAlpha;
};

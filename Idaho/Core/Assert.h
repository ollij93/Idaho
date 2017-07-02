#pragma once

#ifdef USE_ASSERT

// Includes...
#include "WindowManager.h"

#define ASSERT(__cond,__hint) \
    do { \
        if(!(__cond)) \
        { \
            const int iResult = MessageBox(WindowManager::GetHandle(), ##__hint, "Assert", MB_ABORTRETRYIGNORE); \
            if(iResult == IDABORT) \
            { \
                __debugbreak(); \
            } \
        } \
    } while (false)
#else
#define ASSERT(__cond,__hint) \
    do { (void)sizeof(__cond); } while(false)
#endif

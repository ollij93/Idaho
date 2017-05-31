#pragma once

#ifdef USE_ASSERT
#define ASSERT(__cond,__hint) \
    do { \
        if(!(__cond)) \
        { \
            const int iResult = MessageBox(gHWND, ##__hint, "Assert", MB_ABORTRETRYIGNORE); \
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

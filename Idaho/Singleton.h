#pragma once

template<typename T>
class Singleton {
public:
    static bool Create()
    {
        if (s_pxThis) { return false; }

        s_pxThis = new T;
        return s_pxThis->Init();
    }

    static void Destroy()
    {
        if (!s_pxThis) { return; }

        s_pxThis->Shutdown();
        delete s_pxThis;
        s_pxThis = nullptr;
    }

protected:
    virtual bool Init() = 0;
    virtual void Shutdown() = 0;

private:
    static T* s_pxThis;
    friend T;
};

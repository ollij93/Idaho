#pragma once

// Includes...
#include "Core/Types.h"
#include "Scene.h"

namespace cmd {
    template<typename T>
    class Register {
    public:
        typedef void(*FunctionType)(T);
        static std::map<Hash, FunctionType> Value;
    private:
        Register() {}
        ~Register() {}
    };

    template<typename T>
    typename Register<T>::FunctionType GetCommand(Hash uHash) { return Register<T>::Value[uHash]; }

    template<typename T>
    void RegisterCommand(Hash uHash, typename Register<T>::FunctionType pvfnCmd) {
        Register<T>::Value.insert(std::pair<Hash, Register<T>::FunctionType>(uHash, pvfnCmd));
    }
}

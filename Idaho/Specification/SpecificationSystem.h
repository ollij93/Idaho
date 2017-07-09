#pragma once

// External Includes...
#include <vector>

// Includes...
#include "Singleton.h"
#include "Core/Types.h"

// Forward Declarations...
class Specification;

class SpecificationSystem : public Singleton<SpecificationSystem> {
public:
    static const Specification* GetSpecificationByHash(Hash uHash);

protected:
    virtual bool Init() override;
    virtual void Shutdown() override;

private:
    static bool LoadSpecsFromFile(FILE* pxFile);
    std::vector<Specification*> m_vxSpecifications;

    friend Singleton<SpecificationSystem>;
};

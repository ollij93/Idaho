// External Includes...
#include "TinyXML/tinyxml2.h"

// Includes...
#include "SpecificationSystem.h"
#include "Specification.h"
#include "Core/Assert.h"
#include "LoadSystem.h"

#define MAX_SPEC_FILENAME_SIZE 256

// Statics...
SpecificationSystem* Singleton<SpecificationSystem>::s_pxThis = nullptr;

const Specification*
SpecificationSystem::GetSpecificationByHash(Hash uHash)
{
    ASSERT(s_pxThis, "Attempting to get a specifcation before the specification system is created.");
    if (!s_pxThis) { return nullptr; }

    for (u_int u = 0; u < s_pxThis->m_vxSpecifications.size(); u++) {
        if (s_pxThis->m_vxSpecifications[u]
            && s_pxThis->m_vxSpecifications[u]->m_uHash == uHash) {
            return s_pxThis->m_vxSpecifications[u];
        }
    }

    ASSERT(false, "Failed to get a specification by hash");
    return nullptr;
}

bool
SpecificationSystem::Init()
{
    return LoadSystem::ForFileInDir("Specs", ".spec", LoadSpecsFromFile);
}

void
SpecificationSystem::Shutdown()
{
    while (m_vxSpecifications.size() > 0) {
        Specification* pxSpec = m_vxSpecifications.back();
        pxSpec->Shutdown();
        delete pxSpec;
        pxSpec = nullptr;

        m_vxSpecifications.pop_back();
    }
}

bool
SpecificationSystem::LoadSpecsFromFile(FILE* pxFile)
{
    tinyxml2::XMLDocument xSpecDoc;
    tinyxml2::XMLElement* pxSpecElement;
    Specification* pxNewSpec;

    xSpecDoc.LoadFile(pxFile);

    pxSpecElement = xSpecDoc.FirstChildElement("Specification");
    while (pxSpecElement) {
        bool bLoadSuccess;
        pxNewSpec = new Specification;
        bLoadSuccess = pxNewSpec->LoadParamsFromElement(pxSpecElement);
        if (bLoadSuccess) {
            s_pxThis->m_vxSpecifications.push_back(pxNewSpec);
        } else {
            delete pxNewSpec;
            pxNewSpec = nullptr;
        }

        pxSpecElement = pxSpecElement->NextSiblingElement("Specification");
    }

    fclose(pxFile);

    return true;
}

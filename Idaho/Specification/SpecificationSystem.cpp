// External Includes...
#include "TinyXML/tinyxml2.h"

// Includes...
#include "SpecificationSystem.h"
#include "Specification.h"
#include "Core/Assert.h"

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
    char acFilename[MAX_SPEC_FILENAME_SIZE];
    errno_t iErrno;
    WIN32_FIND_DATA xFindData;
    HANDLE hFile = FindFirstFile("Specs/*.spec", &xFindData);
    ASSERT(hFile != INVALID_HANDLE_VALUE, "Failed to find any spec files to load");
    if (hFile == INVALID_HANDLE_VALUE) { return false; }

    do {
        if (!(xFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            iErrno = strcpy_s(acFilename, MAX_SPEC_FILENAME_SIZE, "Specs/");
            if (!iErrno) {
                iErrno = strcat_s(acFilename, MAX_SPEC_FILENAME_SIZE, xFindData.cFileName);
            }
            ASSERT(!iErrno, "Failed to copy specification filename to buffer. Filename too long?");
            if (iErrno) { return false; }

            LoadSpecsFromFile(acFilename);
        }
    } while (FindNextFile(hFile, &xFindData));
    FindClose(hFile);

    return true;
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

void
SpecificationSystem::LoadSpecsFromFile(const char* pszFilename)
{
    tinyxml2::XMLDocument xSpecDoc;
    tinyxml2::XMLElement* pxSpecElement;
    Specification* pxNewSpec;
    FILE* pxFile;

    fopen_s(&pxFile, pszFilename, "rb");
    ASSERT(pxFile, "Failed to open file for loading specifications.");
    if (!pxFile) { return; }

    xSpecDoc.LoadFile(pxFile);

    pxSpecElement = xSpecDoc.FirstChildElement("Specification");
    while (pxSpecElement) {
        bool bLoadSuccess;
        pxNewSpec = new Specification;
        bLoadSuccess = pxNewSpec->LoadParamsFromElement(pxSpecElement);
        if (bLoadSuccess) {
            m_vxSpecifications.push_back(pxNewSpec);
        } else {
            delete pxNewSpec;
            pxNewSpec = nullptr;
        }

        pxSpecElement = pxSpecElement->NextSiblingElement("Specification");
    }

    fclose(pxFile);
}

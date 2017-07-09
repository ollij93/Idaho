// External Includes...
#include <Windows.h>
#include <stdio.h>

// Includes...
#include "LoadSystem.h"
#include "Core/Assert.h"
#include "Scene.h"
#include "Objects/Camera.h"
#include "Objects/Light.h"
#include "Objects/StaticObject.h"
#include "UI/GUIElement.h"

// Statics...
LoadSystem* Singleton<LoadSystem>::s_pxThis = nullptr;

/*
 * ForFileInDir : Call a function on each file in a directory.
 *                Will also match on file extension if supplied
 */
bool
LoadSystem::ForFileInDir(const char* pszDir, const char* pszExt, bool(*pvfnAction)(FILE*))
{
    // Format the find string
    errno_t iErrno;
    char acFindString[MAX_FILENAME_SIZE];
    iErrno = strcpy_s(acFindString, MAX_FILENAME_SIZE, pszDir);
    if (!iErrno) {
        iErrno = strcat_s(acFindString, MAX_FILENAME_SIZE, "/*");
    }
    if (!iErrno && pszExt) {
        iErrno = strcat_s(acFindString, MAX_FILENAME_SIZE, pszExt);
    }
    ASSERT(!iErrno, "Failed to copy find string to buffer.");
    if (iErrno) { return false; }

    // Find all files in the directory and loop through them
    char acFilename[MAX_FILENAME_SIZE];
    WIN32_FIND_DATA xFindData;
    HANDLE hFile = FindFirstFile(acFindString, &xFindData);
    // If no files then do nothing
    if (hFile == INVALID_HANDLE_VALUE) {
        FindClose(hFile);
        return true;
    }

    do {
        if (!(xFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            iErrno = strcpy_s(acFilename, MAX_FILENAME_SIZE, pszDir);
            if (!iErrno) {
                iErrno = strcat_s(acFilename, MAX_FILENAME_SIZE, "/");
            }
            if (!iErrno) {
                iErrno = strcat_s(acFilename, MAX_FILENAME_SIZE, xFindData.cFileName);
            }
            ASSERT(!iErrno, "Failed to copy filename to buffer. Filename too long?");
            if (iErrno) {
                FindClose(hFile);
                return false;
            }

            FILE* pxFile = nullptr;
            fopen_s(&pxFile, acFilename, "rb");
            ASSERT(pxFile, "Failed to open file for loading.");
            if (!pxFile) {
                FindClose(hFile);
                return false;
            }

            // Perform the action on the file
            bool bRet = pvfnAction(pxFile);
            if (!bRet) {
                FindClose(hFile);
                return false;
            }
        }
    } while (FindNextFile(hFile, &xFindData));

    FindClose(hFile);
    return true;
}

/*
 * ForFileInDir : Call a function on each file in a directory.
 *                Will also match on file extension if supplied
 */
bool
LoadSystem::ForFileInDir(const char* pszDir, const char* pszExt, bool(*pvfnAction)(const char*))
{
    // Format the find string
    errno_t iErrno;
    char acFindString[MAX_FILENAME_SIZE];
    iErrno = strcpy_s(acFindString, MAX_FILENAME_SIZE, pszDir);
    if (!iErrno) {
        iErrno = strcat_s(acFindString, MAX_FILENAME_SIZE, "/*");
    }
    if (!iErrno && pszExt) {
        iErrno = strcat_s(acFindString, MAX_FILENAME_SIZE, pszExt);
    }
    ASSERT(!iErrno, "Failed to copy find string to buffer.");
    if (iErrno) { return false; }

    // Find all files in the directory and loop through them
    char acFilename[MAX_FILENAME_SIZE];
    WIN32_FIND_DATA xFindData;
    HANDLE hFile = FindFirstFile(acFindString, &xFindData);
    // If no files then do nothing
    if (hFile == INVALID_HANDLE_VALUE) {
        FindClose(hFile);
        return true;
    }

    do {
        if (!(xFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            iErrno = strcpy_s(acFilename, MAX_FILENAME_SIZE, pszDir);
            if (!iErrno) {
                iErrno = strcat_s(acFilename, MAX_FILENAME_SIZE, "/");
            }
            if (!iErrno) {
                iErrno = strcat_s(acFilename, MAX_FILENAME_SIZE, xFindData.cFileName);
            }
            ASSERT(!iErrno, "Failed to copy filename to buffer. Filename too long?");
            if (iErrno) {
                FindClose(hFile);
                return false;
            }

            // Perform the action on the filename
            bool bRet = pvfnAction(acFilename);
            if (!bRet) {
                FindClose(hFile);
                return false;
            }
        }
    } while (FindNextFile(hFile, &xFindData));

    FindClose(hFile);
    return true;
}

/*
 * GetFileInDir : Find a file in the supplied directory whose name (including dir) matches the hash
 */
FILE*
LoadSystem::GetFileInDir(const char* pszDir, Hash uFileHash)
{
    // Format the find string
    errno_t iErrno;
    char acFindString[MAX_FILENAME_SIZE];
    iErrno = strcpy_s(acFindString, MAX_FILENAME_SIZE, pszDir);
    if (!iErrno) {
        iErrno = strcat_s(acFindString, MAX_FILENAME_SIZE, "/*");
    }
    ASSERT(!iErrno, "Failed to copy find string to buffer.");
    if (iErrno) { return nullptr; }

    // Find all files in the directory and loop through them
    char acFilename[MAX_FILENAME_SIZE];
    WIN32_FIND_DATA xFindData;
    HANDLE hFile = FindFirstFile(acFindString, &xFindData);
    // If no files then do nothing
    if (hFile == INVALID_HANDLE_VALUE) {
        ASSERT(false, "Failed to copy find string to buffer.");
        FindClose(hFile);
        return nullptr;
    }

    do {
        if (!(xFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            iErrno = strcpy_s(acFilename, MAX_FILENAME_SIZE, pszDir);
            if (!iErrno) {
                iErrno = strcat_s(acFilename, MAX_FILENAME_SIZE, "/");
            }
            if (!iErrno) {
                iErrno = strcat_s(acFilename, MAX_FILENAME_SIZE, xFindData.cFileName);
            }
            ASSERT(!iErrno, "Failed to copy filename to buffer. Filename too long?");
            if (iErrno) {
                FindClose(hFile);
                return false;
            }

            if (GetHash(acFilename) == uFileHash) {
                FILE* pxFile = nullptr;
                fopen_s(&pxFile, acFilename, "rb");
                ASSERT(pxFile, "Failed to open file for loading.");
                if (!pxFile) {
                    FindClose(hFile);
                    return nullptr;
                } else {
                    return pxFile;
                }
            }
        }
    } while (FindNextFile(hFile, &xFindData));

    FindClose(hFile);
    return nullptr;
}

template<> Scene*
LoadSystem::CreateFromElement<Scene>(tinyxml2::XMLElement* pxElement)
{
    Scene* pxNewScene = new Scene(pxElement->UnsignedAttribute("id"));
    pxNewScene->SetTrapCursor(pxElement->BoolAttribute("trapMouse"));
    // Load Cameras
    for (tinyxml2::XMLElement* pxCameraElement = pxElement->FirstChildElement("Camera");
        pxCameraElement;
        pxCameraElement = pxCameraElement->NextSiblingElement("Camera")) {
        AddToSceneFromElement<Camera>(pxCameraElement, pxNewScene);
    }

    // Load Lights
    for (tinyxml2::XMLElement* pxLightElement = pxElement->FirstChildElement("Light");
        pxLightElement;
        pxLightElement = pxLightElement->NextSiblingElement("Light")) {
        AddToSceneFromElement<Light>(pxLightElement, pxNewScene);
    }

    // Load GUI Elements
    for (tinyxml2::XMLElement* pxGUIElement = pxElement->FirstChildElement("GUIElement");
        pxGUIElement;
        pxGUIElement = pxGUIElement->NextSiblingElement("GUIElement")) {
        AddToSceneFromElement<GUIElement>(pxGUIElement, pxNewScene);
    }

    // Load Static Objects
    for (tinyxml2::XMLElement* pxObjectElement = pxElement->FirstChildElement("StaticObject");
        pxObjectElement;
        pxObjectElement = pxObjectElement->NextSiblingElement("StaticObject")) {
        AddToSceneFromElement<StaticObject>(pxObjectElement, pxNewScene);
    }

    return pxNewScene;
}

template<> Vector2<int>
LoadSystem::GetFromElement<Vector2<int>>(tinyxml2::XMLElement* pxElement)
{
    Vector2<int> xVec;
    xVec.x = pxElement->IntAttribute("x");
    xVec.y = pxElement->IntAttribute("y");
    return xVec;
}

template<> Vector3<float>
LoadSystem::GetFromElement<Vector3<float>>(tinyxml2::XMLElement* pxElement)
{
    Vector3<float> xVec;
    xVec.x = pxElement->FloatAttribute("x");
    xVec.y = pxElement->FloatAttribute("y");
    xVec.z = pxElement->FloatAttribute("z");
    return xVec;
}

template<> Color
LoadSystem::GetFromElement<Color>(tinyxml2::XMLElement* pxElement)
{
    Color xColor;
    xColor.fRed = pxElement->FloatAttribute("r");
    xColor.fGreen = pxElement->FloatAttribute("g");
    xColor.fBlue = pxElement->FloatAttribute("b");
    xColor.fAlpha = pxElement->FloatAttribute("a");
    return xColor;
}

template<> Matrix3x3
LoadSystem::GetFromElement<Matrix3x3>(tinyxml2::XMLElement* pxElement)
{
    Matrix3x3 xMat;
    xMat.xx = pxElement->FloatAttribute("xx");
    xMat.xy = pxElement->FloatAttribute("xy");
    xMat.xz = pxElement->FloatAttribute("xz");
    xMat.yx = pxElement->FloatAttribute("yx");
    xMat.yy = pxElement->FloatAttribute("yy");
    xMat.yz = pxElement->FloatAttribute("yz");
    xMat.zx = pxElement->FloatAttribute("zx");
    xMat.zy = pxElement->FloatAttribute("zy");
    xMat.zz = pxElement->FloatAttribute("zz");
    return xMat;
}

template<> Camera*
LoadSystem::AddToSceneFromElement<Camera>(tinyxml2::XMLElement* pxElement, Scene* pxScene)
{
    tinyxml2::XMLElement* pxPosElement = pxElement->FirstChildElement("Position");
    tinyxml2::XMLElement* pxOriElement = pxElement->FirstChildElement("Orientation");
    // TODO : Scenes will need to keep lists of cameras and lights so they can delete them
    //        This will also allow them to verify that the camera being set as active is in
    //        the current scene
    // TODO : Need to figure out if this GUID thing is needed and if so how to generate it
    Camera* pxCamera = new Camera(*pxScene, 0);

    if (pxPosElement) {
        Vector3<float> xPos = GetFromElement<Vector3<float>>(pxPosElement);
        pxCamera->SetPosition(xPos);
    }
    if (pxOriElement) {
        Matrix3x3 xOri = GetFromElement<Matrix3x3>(pxOriElement);
        pxCamera->SetOrientation(xOri);
    }
    if (pxElement->BoolAttribute("active")) {
        pxScene->SetActiveCamera(pxCamera);
    }

    return pxCamera;
}

template<> Light*
LoadSystem::AddToSceneFromElement<Light>(tinyxml2::XMLElement* pxElement, Scene* pxScene)
{
    tinyxml2::XMLElement* pxPosElement = pxElement->FirstChildElement("Position");
    tinyxml2::XMLElement* pxOriElement = pxElement->FirstChildElement("Orientation");
    tinyxml2::XMLElement* pxAmbientElement = pxElement->FirstChildElement("Ambient");
    tinyxml2::XMLElement* pxDiffuseElement = pxElement->FirstChildElement("Diffuse");
    // TODO : Scenes will need to keep lists of cameras and lights so they can delete them
    //        This will also allow them to verify that the camera being set as active is in
    //        the current scene
    // TODO : Need to figure out if this GUID thing is needed and if so how to generate it
    Light* pxLight = new Light(*pxScene, 0);

    if (pxPosElement) {
        Vector3<float> xPos = GetFromElement<Vector3<float>>(pxPosElement);
        pxLight->SetPosition(xPos);
    }
    if (pxOriElement) {
        Matrix3x3 xOri = GetFromElement<Matrix3x3>(pxOriElement);
        pxLight->SetOrientation(xOri);
    }
    if (pxAmbientElement) {
        Color xColor = GetFromElement<Color>(pxAmbientElement);
        pxLight->SetAmbientColor(xColor);
    }
    if (pxDiffuseElement) {
        Color xColor = GetFromElement<Color>(pxDiffuseElement);
        pxLight->SetAmbientColor(xColor);
    }
    if (pxElement->BoolAttribute("active")) {
        pxScene->SetActiveLight(pxLight);
    }

    return pxLight;
}

template<> StaticObject*
LoadSystem::AddToSceneFromElement<StaticObject>(tinyxml2::XMLElement* pxElement, Scene* pxScene)
{
    tinyxml2::XMLElement* pxPosElement = pxElement->FirstChildElement("Position");
    StaticObject* pxNewObject = new StaticObject(*pxScene, 0);

    const Specification* pxSpec = SpecificationSystem::GetSpecificationByHash(pxElement->HashAttribute("spec"));
    pxNewObject->InitFromSpecification(pxSpec);

    if (pxPosElement) {
        Vector3<float> xPos = GetFromElement<Vector3<float>>(pxPosElement);
        pxNewObject->SetPosition(xPos);
    }

    return pxNewObject;
}

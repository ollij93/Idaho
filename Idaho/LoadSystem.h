#pragma once

// External Includes...
#include "TinyXML/tinyxml2.h"

// Includes...
#include "Singleton.h"
#include "Core/Math.h"
#include "Core/Types.h"

// Macro Defines...
#define MAX_FILENAME_SIZE 256

// Forward Declarations...
class Camera;
class GUIElement;
class Light;
class Renderable2D;
class Scene;
class SpinningObject;
class StaticObject;
class Text;

class LoadSystem : public Singleton<LoadSystem> {
public:
    static bool ForFileInDir(const char* pszDir, const char* pszExt, bool(*pvfnAction)(FILE *));
    static bool ForFileInDir(const char* pszDir, const char* pszExt, bool(*pvfnAction)(const char*));
    static FILE* GetFileInDir(const char* pszDir, Hash uFileHash);

public:
    template<class T> static T* AddToSceneFromElement(tinyxml2::XMLElement* pxElement, Scene* pxScene);
    template<class T> static T* CreateFromElement(tinyxml2::XMLElement* pxElement);
    template<class T> static T GetFromElement(tinyxml2::XMLElement* pxElement);

private:
    LoadSystem() {}

    // Singleton
    typedef Singleton<LoadSystem> PARENT;
    friend Singleton<LoadSystem>;
};

template<> static Vector2<int> LoadSystem::GetFromElement<Vector2<int>>(tinyxml2::XMLElement* pxElement);
template<> static Vector3<float> LoadSystem::GetFromElement<Vector3<float>>(tinyxml2::XMLElement* pxElement);
template<> static Scene* LoadSystem::CreateFromElement<Scene>(tinyxml2::XMLElement* pxElement);
template<> static Camera* LoadSystem::AddToSceneFromElement<Camera>(tinyxml2::XMLElement* pxElement, Scene* pxScene);
template<> static GUIElement* LoadSystem::AddToSceneFromElement<GUIElement>(tinyxml2::XMLElement* pxElement, Scene* pxScene);
template<> static Light* LoadSystem::AddToSceneFromElement<Light>(tinyxml2::XMLElement* pxElement, Scene* pxScene);
template<> static Renderable2D* LoadSystem::AddToSceneFromElement<Renderable2D>(tinyxml2::XMLElement* pxElement, Scene* pxScene);
template<> static SpinningObject* LoadSystem::AddToSceneFromElement<SpinningObject>(tinyxml2::XMLElement* pxElement, Scene* pxScene);
template<> static StaticObject* LoadSystem::AddToSceneFromElement<StaticObject>(tinyxml2::XMLElement* pxElement, Scene* pxScene);
template<> static Text* LoadSystem::AddToSceneFromElement<Text>(tinyxml2::XMLElement* pxElement, Scene* pxScene);

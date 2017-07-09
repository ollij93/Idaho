#pragma once

// External Includes...
#include <list>
#include "ReactPhysics3D/reactphysics3d.h"

// Includes...
#include "Core/Types.h"

// Forward Declarations...
class Camera;
class Entity;
class Light;
class Renderable;

class Scene {
public:
    Scene(u_int uID)
    : m_uID(uID)
    , m_xWorld()
    , m_lpxEntityList()
    , m_lpxRenderableList()
    , m_pxActiveCamera(nullptr)
    , m_pxActiveLight(nullptr)
    , m_bTrapCursor(false)
    {}
    ~Scene() {}

    void ProcessUpdates(float fTimestep);
    void AddToEntityList(Entity* pxEntity) { m_lpxEntityList.push_back(pxEntity); }
    void RemoveFromEntityList(Entity* pxEntity) { m_lpxEntityList.remove(pxEntity); }
    void AddToRenderableList(Renderable* pxRenderable) { m_lpxRenderableList.push_back(pxRenderable); }
    void RemoveFromRenderableList(Renderable* pxRenderable) { m_lpxRenderableList.remove(pxRenderable); }

    // Getters & Setters...
    rp3d::CollisionWorld* GetWorld() { return &m_xWorld; }
    Camera* GetActiveCamera() { return m_pxActiveCamera; }
    void SetActiveCamera(Camera* pxCamera) { m_pxActiveCamera = pxCamera; }
    Light* GetActiveLight() { return m_pxActiveLight; }
    void SetActiveLight(Light* pxLight) { m_pxActiveLight = pxLight; }
    void SetTrapCursor(bool bSet) { m_bTrapCursor = bSet; }

    // Statics...
    static bool LoadFromFile(FILE* pxFile);
    static void SwitchToScene(u_int uID) { SetActive(s_impxScenes[uID]); }

    // Static Getters & Setters...
    static Scene* GetActive() { return s_pxActive; }

protected:
    void Activate();
    void Deactivate();

    static void SetActive(Scene* pxScene);

private:
    u_int m_uID;
    rp3d::CollisionWorld m_xWorld;
    std::list<Entity*> m_lpxEntityList;
    std::list<Renderable*> m_lpxRenderableList;
    Camera* m_pxActiveCamera;
    Light* m_pxActiveLight;
    bool m_bTrapCursor;

    // Statics...
    static Scene* s_pxActive;
    static std::unordered_map<int, Scene*> s_impxScenes;
};


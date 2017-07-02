#pragma once

// External Includes...
#include <list>
#include "ReactPhysics3D/reactphysics3d.h"

// Forward Declarations...
class Camera;
class Entity;
class Light;
class Renderable;

class Scene {
public:
    Scene()
    : m_xWorld()
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
    static Scene* GetActive() { return s_pxActive; }
    static void SetActive(Scene* pxScene);

protected:
    void Activate();
    void Deactivate();

private:
    rp3d::CollisionWorld m_xWorld;
    std::list<Entity*> m_lpxEntityList;
    std::list<Renderable*> m_lpxRenderableList;
    Camera* m_pxActiveCamera;
    Light* m_pxActiveLight;
    bool m_bTrapCursor;

    // Statics...
    static Scene* s_pxActive;
};


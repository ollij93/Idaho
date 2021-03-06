// Includes...
#include "Scene.h"
#include "InputSystem.h"
#include "LoadSystem.h"
#include "Objects/Entity/Entity.h"

// Statics...
Scene* Scene::s_pxActive = nullptr;
std::unordered_map<int, Scene*> Scene::s_impxScenes;

/*
 * ProcessUpdates : Run the updates for the entities in the scene
 */
void
Scene::ProcessUpdates(float fTimestep)
{
    std::list<Entity*>::const_iterator xIter;
    for (xIter = m_lpxEntityList.begin(); xIter != m_lpxEntityList.end(); ++xIter) {
        Entity* pxEntity = *xIter;
        if (pxEntity) {
            pxEntity->Update(fTimestep);
        }
    }
}

bool
Scene::LoadFromFile(FILE* pxFile)
{
    tinyxml2::XMLDocument xDoc;
    xDoc.LoadFile(pxFile);

    for (tinyxml2::XMLElement* pxSceneElement = xDoc.FirstChildElement("Scene");
        pxSceneElement;
        pxSceneElement = pxSceneElement->NextSiblingElement("Scene")) {
        Scene* pxScene = LoadSystem::CreateFromElement<Scene>(pxSceneElement);
        if (pxScene) {
            s_impxScenes[pxScene->m_uID] = pxScene;
            pxScene->Deactivate();
        }
    }

    return true;
}

void
Scene::SetActive(Scene* pxScene)
{
    if (s_pxActive) {
        s_pxActive->Deactivate();
    }
    if (pxScene) {
        pxScene->Activate();
    }

    s_pxActive = pxScene;
}

void
Scene::Activate()
{
    // Loop through Renderables and add them all to the render lists
    std::list<Renderable*>::const_iterator xIterRenderable;
    for (xIterRenderable = m_lpxRenderableList.begin(); xIterRenderable != m_lpxRenderableList.end(); ++xIterRenderable) {
        Renderable* pxRenderable = *xIterRenderable;
        if (pxRenderable) {
            pxRenderable->AddToRenderList();
        }
    }

    // Loop through Entities and add all their input callbacks
    std::list<Entity*>::const_iterator xIterEntity;
    for (xIterEntity = m_lpxEntityList.begin(); xIterEntity != m_lpxEntityList.end(); ++xIterEntity) {
        Entity* pxEntity = *xIterEntity;
        if (pxEntity) {
            pxEntity->RegisterInputCallbacks();
        }
    }

    if (m_bTrapCursor) {
        while (ShowCursor(false) >= 0) {}
        InputSystem::SetLockMouse(true);
    }
}

void
Scene::Deactivate()
{
    // Ensure the cursor is shown
    while (ShowCursor(true) < 0) {}
    InputSystem::SetLockMouse(false);

    // Loop through Renderables and remove them from the render lists
    std::list<Renderable*>::const_iterator xIterRenderable;
    for (xIterRenderable = m_lpxRenderableList.begin(); xIterRenderable != m_lpxRenderableList.end(); ++xIterRenderable) {
        Renderable* pxRenderable = *xIterRenderable;
        if (pxRenderable) {
            pxRenderable->RemoveFromRenderList();
        }
    }

    // Loop through Entities and remove all their input callbacks
    std::list<Entity*>::const_iterator xIterEntity;
    for (xIterEntity = m_lpxEntityList.begin(); xIterEntity != m_lpxEntityList.end(); ++xIterEntity) {
        Entity* pxEntity = *xIterEntity;
        if (pxEntity) {
            pxEntity->UnRegisterInputCallbacks();
        }
    }
}

// Includes...
#include "SpinningObject.h"
#include "LoadSystem.h"

template<> SpinningObject*
LoadSystem::AddToSceneFromElement<SpinningObject>(tinyxml2::XMLElement* pxElement, Scene* pxScene)
{
    tinyxml2::XMLElement* pxPosElement = pxElement->FirstChildElement("Position");

    SpinningObject* pxObject = new SpinningObject(*pxScene, 0);

    const Specification* pxSpec = SpecificationSystem::GetSpecificationByHash(pxElement->HashAttribute("spec"));
    pxObject->InitFromSpecification(pxSpec);

    const float fRate = pxElement->FloatAttribute("rate");
    pxObject->SetRate(fRate);

    if (pxPosElement) {
        const Vector3<float> xPos = GetFromElement<Vector3<float>>(pxPosElement);
        pxObject->SetPosition(xPos);
    }

    return pxObject;
}

/*
 * Update : Update the orientation of the object each loop
 */
void
SpinningObject::Update(float fTimestep)
{
    PARENT::Update(fTimestep);

    Matrix3x3 xOri = GetOrientation();
    xOri.RotateWorldY(fTimestep * m_fRate);
    SetOrientation(xOri);
}

void
SpinningObject::InitFromSpecification(const Specification* pxSpecification)
{
    PARENT::InitFromSpecification(pxSpecification);
    Hash uModelHash;
    Hash uTextureHash;

    uModelHash = pxSpecification->GetValue<Hash>(CompileTimeGetHash("Model"), uHASH_UNSET);
    uTextureHash = pxSpecification->GetValue<Hash>(CompileTimeGetHash("Texture"), uHASH_UNSET);

    m_xModel.SetModelHash(uModelHash);
    m_xModel.SetTextureHash(uTextureHash);
    m_xModel.Init();
}

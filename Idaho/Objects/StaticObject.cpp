// Includes...
#include "StaticObject.h"

void
StaticObject::InitFromSpecification(const Specification* pxSpecification)
{
    PARENT::InitFromSpecification(pxSpecification);
    Hash uModelHash;
    Hash uTextureHash;

    uModelHash = pxSpecification->GetValue<Hash>(GetHash("Model"), uHASH_UNSET);
    uTextureHash = pxSpecification->GetValue<Hash>(GetHash("Texture"), uHASH_UNSET);

    m_xModel.SetModelHash(uModelHash);
    m_xModel.SetTextureHash(uTextureHash);
    m_xModel.Init();
}

void*
StaticObject::GetProxyShapeData()
{
    return this;
}

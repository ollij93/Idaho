// Includes...
#include "Renderable2D.h"

bool
Renderable2D::Init()
{
    m_uVertexCount = 6;
    m_pxVertices = new Vertex[m_uVertexCount];

    // Bottom left
    m_pxVertices[0].m_xPosition = Vector3<float>(0.f, 0.f, 0.f);
    m_pxVertices[0].m_xTexture = Vector2<float>(m_fTextureLeft, m_fTextureBottom);
    m_pxVertices[0].m_xNormal = Vector3<float>(0.f, 0.f, 1.f);

    // Top right
    m_pxVertices[1].m_xPosition = Vector3<float>((float)m_xDimensions.x, (float)m_xDimensions.y, 0.f);
    m_pxVertices[1].m_xTexture = Vector2<float>(m_fTextureRight, m_fTextureTop);
    m_pxVertices[1].m_xNormal = Vector3<float>(0.f, 0.f, 1.f);

    // Top left
    m_pxVertices[2].m_xPosition = Vector3<float>(0.f, (float)m_xDimensions.y, 0.f);
    m_pxVertices[2].m_xTexture = Vector2<float>(m_fTextureLeft, m_fTextureTop);
    m_pxVertices[2].m_xNormal = Vector3<float>(0.f, 0.f, 1.f);

    // Top right
    m_pxVertices[3].m_xPosition = Vector3<float>((float)m_xDimensions.x, (float)m_xDimensions.y, 0.f);
    m_pxVertices[3].m_xTexture = Vector2<float>(m_fTextureRight, m_fTextureTop);
    m_pxVertices[3].m_xNormal = Vector3<float>(0.f, 0.f, 1.f);

    // Bottom left
    m_pxVertices[4].m_xPosition = Vector3<float>(0.f, 0.f, 0.f);
    m_pxVertices[4].m_xTexture = Vector2<float>(m_fTextureLeft, m_fTextureBottom);
    m_pxVertices[4].m_xNormal = Vector3<float>(0.f, 0.f, 1.f);

    // Bottom right
    m_pxVertices[5].m_xPosition = Vector3<float>((float)m_xDimensions.x, 0.f, 0.f);
    m_pxVertices[5].m_xTexture = Vector2<float>(m_fTextureRight, m_fTextureBottom);
    m_pxVertices[5].m_xNormal = Vector3<float>(0.f, 0.f, 1.f);

    return PARENT::Init();
}

DirectX::XMMATRIX
Renderable2D::GetWorldMatrix() const
{
    return DirectX::XMMatrixTranslation((float)m_xPos.x, (float)m_xPos.y, 0.f);
}

void
Renderable2D::SetPosition(Vector2<int> xPos, bool bCentre /* = false */)
{
    if (bCentre) {
        xPos -= Vector2<int>(m_xDimensions.x/2, m_xDimensions.y/2);
    }
    m_xPos = xPos;
}

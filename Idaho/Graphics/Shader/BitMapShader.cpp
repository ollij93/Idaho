// Includes...
#include "BitMapShader.h"

// Global Variables...
extern u_int guSCREENWIDTH;
extern u_int guSCREENHEIGHT;
extern const float gfSCREENNEAR;
extern const float gfSCREENDEPTH;

BitMapShader* Singleton<BitMapShader>::s_pxThis = nullptr;
std::list<Renderable*> Shader<BitMapShader>::s_lpxRenderableslist;

void
BitMapShader::GetProjectionMatrix(DirectX::XMMATRIX& xProjectionMatrix)
{
    // Create the projection matrix
    xProjectionMatrix = DirectX::XMMatrixOrthographicRH((float)guSCREENWIDTH, (float)guSCREENHEIGHT, gfSCREENNEAR, gfSCREENDEPTH);
}

void
BitMapShader::GetViewMatrix(DirectX::XMMATRIX& xViewMatrix)
{
    DirectX::XMVECTOR xPos = DirectX::XMVectorSet(0.f, 0.f, -1.f, 0.f);
    DirectX::XMVECTOR xUpVec = DirectX::XMVectorSet(0.f, -1.f, 0.f, 0.f);
    DirectX::XMVECTOR xLookAt = DirectX::XMVectorSet(0.f, 0.f, 1.f, 0.f);

    // Create the view matrix from the three updated vectors.
    xViewMatrix = DirectX::XMMatrixLookAtRH(xPos, xLookAt, xUpVec);
}

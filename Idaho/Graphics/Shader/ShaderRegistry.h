#pragma once

// Includes...
#include "BitMapShader.h"
#include "ColorShader.h"
#include "LightShader.h"
#include "TextureShader.h"

// Macros...
#define REGISTERSHADER(__shadername) \
template bool Shader<__shadername>::Render(); \
template bool Shader<__shadername>::Init(); \
template void Shader<__shadername>::Shutdown(); \
template bool Shader<__shadername>::CreateMatrixBuffer(); \
template bool Shader<__shadername>::CreateVertexInputLayout(ID3D10Blob*); \
template bool Shader<__shadername>::CreateSamplerState(); \
template bool Shader<__shadername>::SetShaderParameters(ID3D11DeviceContext*, const DirectX::XMMATRIX&, const DirectX::XMMATRIX&, const DirectX::XMMATRIX&, ID3D11ShaderResourceView*); \
template void Shader<__shadername>::GetProjectionMatrix(DirectX::XMMATRIX&); \
template void Shader<__shadername>::GetViewMatrix(DirectX::XMMATRIX&) \

// Registry...
REGISTERSHADER(ColorShader);
REGISTERSHADER(TextureShader);
REGISTERSHADER(LightShader);
REGISTERSHADER(BitMapShader);

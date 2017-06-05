#pragma once

// Includes...
#include "ColorShader.h"
#include "TextureShader.h"

// Macros...
#define REGISTERSHADER(__shadername) \
template bool Shader<__shadername>::Render(); \
template bool Shader<__shadername>::Init(); \
template void Shader<__shadername>::Shutdown(); \
template bool Shader<__shadername>::CreateMatrixBuffer(); \
template bool Shader<__shadername>::CreateVertexInputLayout(ID3D10Blob*); \
template bool Shader<__shadername>::CreateSamplerState() \

// Registry...
REGISTERSHADER(ColorShader);
REGISTERSHADER(TextureShader);

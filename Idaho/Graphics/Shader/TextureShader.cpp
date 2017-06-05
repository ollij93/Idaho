// External Includes...
#include <list>

// Includes...
#include "TextureShader.h"
#include "Graphics/Renderable.h"

TextureShader* Singleton<TextureShader>::s_pxThis = nullptr;
std::list<Renderable*> Shader<TextureShader>::s_lpxRenderableslist;

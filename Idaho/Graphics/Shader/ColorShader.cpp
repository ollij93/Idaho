// External Includes...
#include <list>

// Includes...
#include "ColorShader.h"
#include "Graphics/Renderable.h"

ColorShader* Singleton<ColorShader>::s_pxThis = nullptr;
std::list<Renderable*> Shader<ColorShader>::s_lpxRenderableslist;

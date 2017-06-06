// Buffers...
Texture2D t2dTexture;
SamplerState xSamplerType;

cbuffer LightBuffer {
    float4 f4DiffuseColor;
    float4 f4AmbientColor;
    float3 f3LightDirection;
    float fPadding;
};

// Types...
struct LightPixelInputType
{
    float4 f4Position : SV_POSITION;
    float2 f2TexCoord : TEXCOORD0;
    float3 f3Normal : NORMAL;
};

/*
 * TexturePixelShader main
 */
float4 main(LightPixelInputType xInput) : SV_TARGET
{
    float4 f4TextureColor;
    float3 f3LightDir;
    float fLightIntensity;
    float4 f4Color;

    f4TextureColor = t2dTexture.Sample(xSamplerType, xInput.f2TexCoord);
    f4Color = f4AmbientColor;

    f3LightDir = -f3LightDirection;
    fLightIntensity = saturate(dot(xInput.f3Normal, f3LightDir));

    if (fLightIntensity > 0.f) {
        f4Color += f4DiffuseColor * fLightIntensity;
    }

    f4Color = saturate(f4Color);

    f4Color = f4Color * f4TextureColor;

    return f4Color;
}

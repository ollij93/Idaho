// Buffers...
Texture2D t2dTexture;
SamplerState xSamplerType;

cbuffer LightBuffer {
    float4 f4DiffuseColor;
    float4 f4AmbientColor;
    float4 f4LightDirection;
    float4 f4SpecularColor;
    float fSpecularPower;
};

// Types...
struct LightPixelInputType
{
    float4 f4Position : SV_POSITION;
    float2 f2TexCoord : TEXCOORD0;
    float3 f3Normal : NORMAL;
    float3 f3ViewDirection : TEXCOORD1;
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
    float3 f3Reflection;
    float4 f4Specular;

    f4TextureColor = t2dTexture.Sample(xSamplerType, xInput.f2TexCoord);
    f4Color = f4AmbientColor;
    f4Specular = float4(0.f, 0.f, 0.f, 0.f);

    f3LightDir = -f4LightDirection.xyz;
    fLightIntensity = saturate(dot(xInput.f3Normal, f3LightDir));

    if (fLightIntensity > 0.f) {
        f4Color += f4DiffuseColor * fLightIntensity;
        f4Color = saturate(f4Color);

        f3Reflection = normalize(2 * fLightIntensity * xInput.f3Normal - f3LightDir);
        f4Specular = f4SpecularColor * pow(saturate(dot(f3Reflection, xInput.f3ViewDirection)), fSpecularPower);
    }

    f4Color = f4Color * f4TextureColor;
    f4Color = saturate(f4Color + f4Specular);

    return f4Color;
}

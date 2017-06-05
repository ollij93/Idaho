// Buffers...
Texture2D t2dTexture;
SamplerState xSampleType;

// Types...
struct TexturePixelInputType
{
    float4 f4Position : SV_POSITION;
    float2 f2TexCoord : TEXCOORD0;
};

/*
 * TexturePixelShader main
 */
float4 main(TexturePixelInputType xInput) : SV_TARGET
{
    float4 f4TextureColor;

    f4TextureColor = t2dTexture.Sample(xSampleType, xInput.f2TexCoord);

    return f4TextureColor;
}

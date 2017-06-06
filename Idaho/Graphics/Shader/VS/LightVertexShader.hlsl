// Buffers...
cbuffer MatrixBuffer
{
    matrix mWorldMatrix;
    matrix mViewMatrix;
    matrix mProjectionMatrix;
};

// Types...
struct LightVertexInputType {
    float4 f4Position : POSITION;
    float2 f2TexCoord : TEXCOORD0;
    float3 f3Normal : NORMAL;
};
struct LightPixelInputType {
    float4 f4Position : SV_POSITION;
    float2 f2TexCoord : TEXCOORD0;
    float3 f3Normal : NORMAL;
};

/*
 * TextureVertexShader main
 */
LightPixelInputType main(LightVertexInputType xInput)
{
    LightVertexInputType xOutput;

    // Ensure the 4th component of position is 1
    xInput.f4Position.w = 1.0f;

    // Perform the matrix multiplication to convert world space to screen space
    xOutput.f4Position = mul(xInput.f4Position, mWorldMatrix);
    xOutput.f4Position = mul(xOutput.f4Position, mViewMatrix);
    xOutput.f4Position = mul(xOutput.f4Position, mProjectionMatrix);

    // Set the color in the output type
    xOutput.f2TexCoord = xInput.f2TexCoord;

    xOutput.f3Normal = mul(xInput.f3Normal, (float3x3)mWorldMatrix);
    
    // Set the normal in the output type
    xOutput.f3Normal = normalize(xOutput.f3Normal);

    return xOutput;
}

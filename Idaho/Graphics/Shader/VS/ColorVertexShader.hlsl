// Buffers...
cbuffer MatrixBuffer
{
    matrix mWorldMatrix;
    matrix mViewMatrix;
    matrix mProjectionMatrix;
};

// Types...
struct ColorVertexInputType {
    float4 f4Position : POSITION;
};
struct ColorPixelInputType {
    float4 f4Position : SV_POSITION;
    float4 f4Color : COLOR;
};

/*
 * ColorVertexShader main
 */
ColorPixelInputType main(ColorVertexInputType xInput)
{
    ColorPixelInputType xOutput;

    // Ensure the 4th component of position is 1
    xInput.f4Position.w = 1.0f;

    // Perform the matrix multiplication to convert world space to screen space
    xOutput.f4Position = mul(xInput.f4Position, mWorldMatrix);
    xOutput.f4Position = mul(xOutput.f4Position, mViewMatrix);
    xOutput.f4Position = mul(xOutput.f4Position, mProjectionMatrix);

    // Set the color in the output type
    xOutput.f4Color.r = 1.f;
    xOutput.f4Color.g = 0.f;
    xOutput.f4Color.b = 1.f;
    xOutput.f4Color.a = 1.f;

    return xOutput;
}

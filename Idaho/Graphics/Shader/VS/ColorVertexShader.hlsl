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
    float4 f4Color : COLOR;
};
struct ColorPixelInputType { // TODO-OJ : Check this duplication is necessary
    float4 f4Position : SV_POSITION;
    float4 f4Color : COLOR;
};

/*
 * ColorVertexShader
 */
ColorPixelInputType ColorVertexShader( ColorVertexInputType xInput )
{
    ColorPixelInputType xOutput;

    // Ensure the 4th component of position is 1
    xInput.position.w = 1.0f;

    // Perform the matrix multiplication to convert world space to screen space
    xOutput.f4Position = mul(xInput.f4Position, mWorldMatrix);
    xOutput.f4Position = mul(xOutput.f4Position, mViewMatrix);
    xOutput.f4Position = mul(xOutput.f4Position, mProjectionMatrix);

    // Set the color in the output type
    xOutput.f4Color = xInput.f4Color;

    return xOutput;
}
// Types...
struct ColorPixelInputType {
    float4 f4Position : SV_POSITION;
    float4 f4Color : COLOR;
};

/*
 * ColorPixelShader
 */
float4 main(ColorPixelInputType xInput) : SV_TARGET
{
    return xInput.f4Color;
}

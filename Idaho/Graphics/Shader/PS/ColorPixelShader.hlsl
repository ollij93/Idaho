// Types...
struct ColorPixelInputType {
    float4 f4Position : SV_POSITION;
    float4 f4Color : COLOR;
};

/*
 * ColorPixelShader
 */
float4 ColorPixelShader() : SV_TARGET
{
	return xInput.f4Color;
}
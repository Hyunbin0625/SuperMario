#include "CBuffers.hlsl"

Texture2D sourceTex : register(t0); // texture, 0번째
SamplerState samp : register(s0);   // sampler, 0번째

cbuffer OutlineBuffer : register(b0)
{
    bool _bOutline;
}

float4 PS(PixelInput input) : SV_Target0    // target0번에 출력
{
    // smpling
    float4 color = sourceTex.Sample(samp, input.uv);
    
//  if(color.a == 0.0f)
//      discard;    // 색을 버린다는 뜻, 처리하고 있는 예비 픽셀들을
    
    float border = 1.0f / 128;
    
    if (_bOutline && (input.uv.x < border || input.uv.x > 1.0f - border || input.uv.y < border || input.uv.y > 1.0f - border))
        color = float4(1, 1, 1, 1);
    
    return color;
};
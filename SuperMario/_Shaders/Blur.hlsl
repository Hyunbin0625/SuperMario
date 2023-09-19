struct VertexInput
{
    float4 position : POSITION0;
    float2 uv : TEXCOORD0;
};

struct PixelInput
{
    float4 position : SV_POSITION0; // 픽셀 위치를 설명하는 Semantics
    float2 uv : TEXCOORD0;
};

cbuffer World : register(b0) // buffer, 0번째
{
    matrix _world;
}

cbuffer ViewProjection : register(b1) // buffer, 1번째
{
    matrix _view;
    matrix _proj;
}

PixelInput VS(VertexInput input)
{
    PixelInput output;
    
//  output.position = input.position;
    // 월드 변환, 뷰 변환, 프로젝션 변환
    output.position = mul(input.position, _world);  // 들어온 input의 position에 들어온 world를 곱해, 월드 변환한 output의 position에 들어간다.
    output.position = mul(output.position, _view);  // 월드 변환된 output의 position에 들어온 view를 곱해, 뷰변환한 output의 position에 들어간다.
    output.position = mul(output.position, _proj);  // 뷰 변환된 output의 position에 들어온 projection을 곱해, 프로젝션 변환한 output의 position에 들어간다.
    
    output.uv = input.uv;
    
    return output;
}

Texture2D sourceTex : register(t0); // texture, 0번째
SamplerState samp : register(s0);   // sampler, 0번째

cbuffer SelectionBuffer : register(b0)
{
    uint _selection;
}

float4 PS(PixelInput input) : SV_Target0    // target0번에 출력
{
    // smpling
    float4 color = sourceTex.Sample(samp, input.uv);
    
    float2 arr[8] =
    {
        float2(-1, -1), float2(0, -1),  float2(1, -1),
        float2(-1, 0),                  float2(1, 0),
        float2(-1, 1),  float2(0, 1),   float2(1, 1)
    };
    
    uint blurCount = 0;
    
    blurCount = pow(2, _selection);
    
    float2 texturesSize = float2(1280, 720);    // 임시
    
    for (uint blur = 0; blur < blurCount; ++blur)
    {
        for (uint i = 0; i < 8; ++i)
        {
            float2 uv = arr[i] * (float) blur / texturesSize + input.uv;
            
            color += sourceTex.Sample(samp, uv);
        }
    }
    
    color /= blurCount * 8 + 1;

    return color;
};

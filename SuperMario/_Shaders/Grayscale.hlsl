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
    float3 grayscale = 0;
    float3 sepia = 0;
    
    if (color.a == 0)
        return color;
    
    if (_selection == 2)
    {
        grayscale = (color.r + color.g + color.b) / 3.0f;
        color = float4(grayscale, 1);
    }
    else if (_selection == 3)
    {
        grayscale = dot(color.rgb, float3(0.299f, 0.587f, 0.114f));
        color = float4(grayscale, 1);
    }
    else if (_selection == 4)
    {
        sepia.r = dot(color.rgb, float3(0.393f, 0.769f, 0.189f));
        sepia.g = dot(color.rgb, float3(0.349f, 0.686f, 0.168f));
        sepia.b = dot(color.rgb, float3(0.272f, 0.534f, 0.131f));
        color = float4(sepia.rgb, 1);
    }
    
    return color;
};

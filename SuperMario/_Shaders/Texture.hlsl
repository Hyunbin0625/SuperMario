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

cbuffer TransformBuffer : register(b0) // buffer, 0번째
{
    matrix _world;
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

float4 PS(PixelInput input) : SV_Target0    // target0번에 출력
{
    // smpling
    float4 color = sourceTex.Sample(samp, input.uv);
    
//  if(color.a == 0.0f)
//      discard;    // 색을 버린다는 뜻, 처리하고 있는 예비 픽셀들을
    
    return color;
};
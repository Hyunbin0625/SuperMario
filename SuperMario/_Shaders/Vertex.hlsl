struct VertexInput
{
    float4 position : POSITION0;
};

struct PixelInput
{
    float4 position : SV_POSITION0; // 픽셀 위치를 설명하는 Semantics
};

cbuffer World : register(b0) // buffer, 0번째
{
    matrix _world;
}

cbuffer ViewProjection : register(b1) // buffer, 0번째
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
    
    return output;
}

cbuffer ColorBuffer : register(b0)
{
    float4 _color;
}

float4 PS(PixelInput input) : SV_Target0    // target0번에 출력
{
    return _color;
};
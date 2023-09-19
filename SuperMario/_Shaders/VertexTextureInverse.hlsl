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
    float4 color;
    
    if(_selection == 1)
        color = sourceTex.Sample(samp, input.uv);
    else if (_selection == 2)
        color = sourceTex.Sample(samp, float2(1 - input.uv.x, input.uv.y));
    else if (_selection == 3)
    {
    //    float4 l = sourceTex.Sample(samp, input.uv);
    //    float4 r = sourceTex.Sample(samp, float2(input.uv.x - 0.5f, input.uv.y));
    //    
    //    if(input.uv.x < 0.5f)
    //        color = l;
    //    else if (input.uv.x > 0.5f)
    //        color = r;
        
        color = sourceTex.Sample(samp, float2(input.uv.x % 0.5f, input.uv.y));
        
        if(input.uv.x > 0.499f && input.uv.x < 0.501f)
            color = float4(1, 1, 1, 1);
    }
    else if (_selection == 4)
    {
        float4 lt = sourceTex.Sample(samp, 2 * input.uv);
        float4 rt = sourceTex.Sample(samp, 2 * float2(1 - input.uv.x, input.uv.y));
        float4 lb = sourceTex.Sample(samp, 2 * float2(input.uv.x, 1 - input.uv.y));
        float4 rb = sourceTex.Sample(samp, 2 * float2(1 - input.uv.x, 1 - input.uv.y));
        
        if (input.uv.x < 0.5f && input.uv.y < 0.5f)
            color = lt;
        else if (input.uv.x > 0.5f && input.uv.y < 0.5f)
            color = rt;
        else if (input.uv.x < 0.5f && input.uv.y > 0.5f)
            color = lb;
        else if (input.uv.x > 0.5f && input.uv.y > 0.5f)
            color = rb;
        
        if (input.uv.x > 0.499f && input.uv.x < 0.501f)
            color = float4(1, 1, 1, 1);
        else if (input.uv.y > 0.499f && input.uv.y < 0.501f)
            color = float4(1, 1, 1, 1);
    }
    
    return color;
};


Texture2D<float> heightTex : register(t0);
SamplerState smp : register(s0);

struct PSInput
{
    float4 svPos : SV_Position;
    float4 diff : COLOR0;
    float4 spc : COLOR1;
    float2 uv : TEXCOORD0;
    float2 suv : TEXCOORD1;
};
float4 main(PSInput input) : SV_TARGET
{
    int w, h;
    heightTex.GetDimensions(w, h);
    
    const float2 eps = float2(2, 2) / float2(w, h);
    const float2 xeps = float2(eps.x, 0);
    const float2 yeps = float2(0, eps.y);
    const float level = 8.0;
    
    
    float3 N = normalize(float3(
                    (heightTex.Sample(smp, input.uv - xeps) - heightTex.Sample(smp, input.uv + xeps)) / (2.0 * eps.x),
                    (heightTex.Sample(smp, input.uv + yeps) - heightTex.Sample(smp, input.uv - yeps)) / (2.0 * eps.y),
                    level));
    
    
    return float4((N + 1.0) / 2.0, 1.0f);
}
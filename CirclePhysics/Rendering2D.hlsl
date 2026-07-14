
Texture2D<float4> tex : register(t0);
Texture2D<float4> normTex: register(t1);
Texture2D<float4> backTex: register(t2);
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
    
    
    float3 eye = float3(0, 0, -10);
    float3 pos = float3(input.uv * float2(640.0, 480.0), 0) - float3(320.0, 240.0, 0);
    float3 ray = normalize(pos - eye);
    
    float3 light = normalize(float3(1, -1, 1));
    float3 N = normalize(normTex.Sample(smp, input.uv).xyz * 2 - 1);
    N.z = -N.z;
    
    float3 backCol = backTex.Sample(smp, input.uv+N.xy*0.1).rgb;
    
    float spec = pow(saturate(dot(reflect(light, N), -ray)), 10);
    
    float ambient = 0.2;
    float diffuse = max(dot(N, -light), ambient);
    float4 col = tex.Sample(smp, input.uv);
    return float4(lerp(backCol, col.rgb * diffuse + spec, 0.75), col.a);
	
}
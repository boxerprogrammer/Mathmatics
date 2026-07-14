Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);
struct PSInput
{
    float4 pos : SV_Position;
    float4 dif : COLOR0;
    float4 spc : COLOR1;
    float2 uv : TEXCOORD0;
    float2 suv : TEXCOORD1;
};

cbuffer PostEffectParam : register(b4)
{
    float4 param;
}

//UV値から乱数を生み出して返す
//@param uv値
//@return 0.0<=x<1.0の乱数値
float GetRandom(float2 uv)
{
    return frac(
                    sin(
                        dot(uv + floor(param[0] * 10.0) / 10.0, float2(12.9898, 78.233)
                    )
                ) * 43758.5453);
}

float2 GetRandom2(float2 uv)
{
    uv = float2(dot(uv, float2(129.9898, 311.233)),
                dot(uv, float2(256.5, 192.3)));
    return float2(frac(
                    sin(
                        uv + floor(param[0] * 10.0) / 10.0
                    )
                 * 43758.5453));
}


float Easing(float t)
{
    return 3.0f * t * t - 2.0f * t * t * t;
}

float4 main(PSInput input) : SV_TARGET
{
    uint w, h, mipLevels;
    float4 col = tex.Sample(smp, input.uv);
    tex.GetDimensions(0, w, h, mipLevels);
    float aspect = (float) w / (float) h;
    float2 gridNum = float2(10.0, 10.0 / aspect);
    
    float4 rnd = float4(GetRandom(floor(input.uv * gridNum)),
                        GetRandom(floor(input.uv * gridNum + float2(1, 0))),
                        GetRandom(floor(input.uv * gridNum + float2(0, 1))),
                        GetRandom(floor(input.uv * gridNum + float2(1, 1))));
    float2 fracUV = frac(input.uv * gridNum); //ブロックとブロックの間の値を取得
    
    float perlin = lerp(
                        lerp(rnd[0], rnd[1], Easing(fracUV.x)), //上の段の補間
                       lerp(rnd[2], rnd[3], Easing(fracUV.x)), //下の段の補間
                        Easing(fracUV.y));
    float wave = lerp(sin((input.uv.x + input.uv.y) * 10.0 + param[0] * 2.0) * 0.5 + 0.5, perlin, 0.2);
    return float4(lerp(col.rgb, col.rgb * wave, 0.5), col.a);
                        
    
}
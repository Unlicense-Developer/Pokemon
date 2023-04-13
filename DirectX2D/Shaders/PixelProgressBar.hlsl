struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
};

Texture2D map : register(t0);

SamplerState samp : register(s0);

cbuffer ColorBuffer : register(b0)
{
    float4 color;
}

cbuffer HpBuffer : register(b1)
{
    float fillAmount;
}

float4 PS(PixelInput input) : SV_TARGET
{
    float4 tex = map.Sample(samp, input.uv);
    
    if(input.uv.x < fillAmount)
        return tex * color;
    
    return 0.0f;
}




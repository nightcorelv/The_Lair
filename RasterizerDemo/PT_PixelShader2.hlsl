
struct PixelshaderInput
{
    float4 position : SV_POSITION;
    float4 worldPosition : WORLD_POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
};

float4 main(PixelshaderInput input) : SV_TARGET
{
    return float4(0.082f,0.453f,1,0);
}
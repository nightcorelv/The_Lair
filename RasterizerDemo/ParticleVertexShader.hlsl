struct VertexShaderInput
{
    float3 position : POSITION;
};

struct VertexShaderOutput
{
    float3 position : POSITION;
    //float4 position : SV_POSITION;
    //float4 worldPosition : WORLD_POSITION;
};


VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    //output.position = mul(mul(float4(input.position, 1.0f), transform), viewProj);
    //output.worldPosition = mul(float4(input.position, 1.0f), transform);
    output.position = input.position;
    return output;
}
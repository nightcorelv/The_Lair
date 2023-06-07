cbuffer SkinVtxBuffer : register(b2)
{
    int4 bones;
    float4 weights;
    int id; //if 1, it has a skeleton
}

cbuffer MxBuffer : register(b3)
{
    row_major float4x4 mx[50]; //set 50 in cpp
}

struct NewTf
{
    float4 position;
    float4 normal;
};

struct VertexShaderInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
};

struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
    float4 worldPosition : WORLD_POSITION;
    float4 localPosition : LOCAL_POSITION;
};

cbuffer RotMatrix : register(b0) //kolla på att ändra matriser till float4x4 och transponera innan skicka in
{
    row_major matrix transform; //world matrix  
};

cbuffer viewProjMatrix : register(b1)
{
    row_major matrix viewProj; //combined view + projection matrix
};


VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    output.localPosition = float4(input.position, 0.0f);
    if (id == 1)
    {
        NewTf vtx;

        float4 startPosition = float4(input.position, 1.0f);
        float3 sumPos = float3(0, 0, 0);

        sumPos += mul(startPosition, mx[bones[0]]) * weights[0];
        sumPos += mul(startPosition, mx[bones[1]]) * weights[1];
        sumPos += mul(startPosition, mx[bones[2]]) * weights[2];
        sumPos += mul(startPosition, mx[bones[3]]) * weights[3];

        float4 startNorm = float4(input.normal, 1.0f);
        float3 sumNorm = float3(0, 0, 0);

        sumNorm += mul(startNorm, mx[bones[0]]) * weights[0];
        sumNorm += mul(startNorm, mx[bones[1]]) * weights[1];
        sumNorm += mul(startNorm, mx[bones[2]]) * weights[2];
        sumNorm += mul(startNorm, mx[bones[3]]) * weights[3];

        vtx.position = float4(sumPos, 1.0f);
        vtx.normal = float4(sumNorm, 1.0f);

        output.position = mul(mul(vtx.position, transform), viewProj);
        output.normal = mul(vtx.normal, transform);
        output.uv = input.uv;
        output.worldPosition = mul(vtx.position, transform);
    }
    else
    {
        output.position = mul(mul(float4(input.position, 1.0f), transform), viewProj);
        output.normal = mul(float4(input.normal, 0.0f), transform);
        output.uv = input.uv;
        output.worldPosition = mul(float4(input.position, 1.0f), transform);
    }
    return output;
}
//Texture2D ambientTexture : register(t0);
Texture2D diffuseTexture : register(t0);
//Texture2D specularTexture : register(t2);
SamplerState testSampler : register(s0);

//cbuffer materialValue : register(b0)
//{
//    float4 matArray[3]; //[ka][kd][ks]//ks.w = exponent
//};


struct PixelshaderInput
{
    float4 position : SV_POSITION;
    float4 worldPosition : WORLD_POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
};

float4 main(PixelshaderInput input) : SV_TARGET
{
    //Variables
    //float exponent = matArray[2].w;
    //output.position =           float4(input.worldPosition);
    //output.normal =             float4(input.normal, 0);
    //output.color  = float4(diffuseTexture.Sample(testSampler, input.uv).xyz, 1.0f);;
    //output.ambientComponent =   matArray[0];
    //output.diffuseComponent =   matArray[1];
    //output.specularComponent =  matArray[2];
    //output.specularExponent =   float4(exponent, exponent, exponent, 0.0f);

    
    return diffuseTexture.Sample(testSampler, input.uv);
}
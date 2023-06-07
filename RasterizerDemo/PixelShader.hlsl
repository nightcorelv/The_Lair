//Texture2D ambientTexture : register(t0);
Texture2D diffuseTexture : register(t0);
//Texture2D specularTexture : register(t2);
TextureCube txCube : register(t1);
SamplerState testSampler : register(s0);

cbuffer materialValue : register(b0)
{
    float4 matArray[3]; //[ka][kd][ks]//ks.w = exponent
};

cbuffer actorTypeValue : register(b1) //determines sample method for Color: tx cube or 2d tx
{
    int actorType;
};

cbuffer cameraPosBuff : register(b2) //Only used if TxCube
{
    float4 cameraPos;
};

cbuffer viewProjMatrix : register(b3)
{
    row_major matrix viewProj; //combined view + projection matrix
};


struct PixelshaderInput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
    float4 worldPosition : WORLD_POSITION;
    float4 localPosition : LOCAL_POSITION;
};

struct PixelshaderOutput
{
    float4 position : SV_Target0;
    float4 normal : SV_Target1;
    float4 color : SV_Target2;
    float4 ambientComponent : SV_Target3;
    float4 diffuseComponent : SV_Target4;
    float4 specularComponent : SV_Target5;
    float4 specularExponent : SV_Target6;
    float2 uv : SV_Target7;
};

PixelshaderOutput main(PixelshaderInput input)
{
    //Variables
    float4 fragTxColor;
    float exponent = matArray[2].w;
    float4 outputWorldPos;
    
    //Determine Sampling method
    if (actorType == 1) //Is texture Cube
    {
        //normal
        float3 fragNormal = normalize(input.normal.xyz);
        
        //Vector from camera to framgent
        const float3 camToFrag = normalize(input.localPosition.xyz - cameraPos.xyz);
        
        //reflected vector
        float3 reflectedVector = reflect(camToFrag, fragNormal);
        
        //Sample from txCube
        fragTxColor = float4(txCube.Sample(testSampler, reflectedVector)); //input.worldPosition.xyz
        
        outputWorldPos = mul(viewProj, float4(input.localPosition.xyz + cameraPos.xyz, 1.0f)).xyww;

    }
    else //Is texture 2d
    {
        fragTxColor = float4(diffuseTexture.Sample(testSampler, input.uv).xyz, 0.0f);
        outputWorldPos = float4(input.worldPosition);
    }
    
    //input.position + cameraPosition,//mul(viewProjectionMatrix, float4(input.position + cameraPosition, 1.0f)).xyww;
   
   //Output definition
    PixelshaderOutput output;
    output.position = float4(input.worldPosition);
    output.normal = float4(input.normal, 0);
    output.color = fragTxColor;
    //output.ambientComponent = matArray[0];
    //output.diffuseComponent = matArray[1];
    //output.specularComponent = matArray[2];
    //output.specularExponent = float4(exponent, exponent, exponent, 0.0f);
    //output.uv = input.uv;
    return output;
}

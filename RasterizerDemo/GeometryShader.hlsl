cbuffer viewProjMatrix : register(b0)
{
    row_major matrix viewProj; //combined view + projection matrix
};

cbuffer cameraValue : register(b1)
{
    float4 cameraPosition; //CameraPos
};

struct VertexShaderOutput
{
    float3 position : POSITION;
};

struct GeometryShaderOutput
{
    float4 position : SV_POSITION;
    float4 worldPosition : WORLD_POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
};


[maxvertexcount(4)]
void main(
	point VertexShaderOutput input[1],
	inout TriangleStream<GeometryShaderOutput> outputStream)
{
    //variables
    float halfWidth = 0.3f;
    float4 upVector = float4(0.0f, 1.0f, 0.0f, 0);

    //create vector towards camera (normal for particle)
    float3 planeNormal = normalize(input[0].position - cameraPosition.xyz);
  
    //Calculate vector for plane
    float4 rightVector = -float4(normalize(cross(planeNormal, upVector.xyz)), 0);
    
    //Change vector length
    rightVector = rightVector * halfWidth;
    upVector = upVector * (2 * halfWidth);
    
    //Get clipspace position
    float4 clipPos = mul(float4(input[0].position, 1.0f), viewProj);
    
    
    //create Uv & texture coordinates
    float2 texCoord[4]; //Uv pos
    texCoord[0] = float2(1, 1);
    texCoord[1] = float2(0, 1);
    texCoord[2] = float2(1, 0);
    texCoord[3] = float2(0, 0);
    float4 clipVertPos[4]; //Clip space pos
    clipVertPos[0] = clipPos + rightVector; //BR
    clipVertPos[1] = clipPos - rightVector; //BL
    clipVertPos[2] = clipPos + rightVector + upVector; //TR
    clipVertPos[3] = clipPos - rightVector + upVector; //TL
    float4 vertexPos[4]; //World space pos
    vertexPos[0] = float4(input[0].position, 0) + rightVector; //BR
    vertexPos[1] = float4(input[0].position, 0) - rightVector; //BL
    vertexPos[2] = float4(input[0].position, 0) + rightVector + upVector; //TR
    vertexPos[3] = float4(input[0].position, 0) - rightVector + upVector; //TL

    
    //Assemble vertData & append
    for (uint i = 0; i < 4; i++)
    {
        GeometryShaderOutput element;
        element.position = clipVertPos[i];
        element.worldPosition = vertexPos[i];
        element.uv = texCoord[i];
        element.normal = planeNormal;
        outputStream.Append(element);
    }
}
    
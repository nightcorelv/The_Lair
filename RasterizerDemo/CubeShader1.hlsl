struct lightStruct
{
    float4 lightPos;
    float4 lightColor;
    float4 ambientComponent;
    float4 diffuseComponent;
    float4 specularComponent;
    float4x4 ltViewMatix; //Light View Matrix
};

RWTexture2DArray<unorm float4> backBuffer : register(u0);
SamplerState testSampler : register(s0); //Sampler
Texture2D posTex : register(t0); //pos
Texture2D normTex : register(t1); //normal
Texture2D colTex : register(t2); //color
Texture2D ambTex : register(t3); //Ambient component
Texture2D difTex : register(t4); //Diffuse component
Texture2D speTex : register(t5); //Specular component
Texture2D exsTex : register(t6); //Specular Exponent
Texture2D uvTex : register(t7); //UV
Texture2DArray texArray : register(t8); //Shadow maps
StructuredBuffer<lightStruct> lightStructBuff : register(t9); //Structured buffer with Light data


cbuffer cameraValue : register(b0)
{
    float4 cameraPosition;
};
cbuffer numLightBuffer : register(b1)
{
    int lightNr;
};


[numthreads(32, 18, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    //variables
    const int nrOfLights = lightNr; //Nr of lights
    const float4 ambientColor = ambTex.Load(DTid, 0); //Ab col
    const float4 diffuseColor = difTex.Load(DTid, 0); //Df col
    const float4 specularColor = speTex.Load(DTid, 0); //Sp col
    float specularExponent = max(((exsTex.Load(DTid).y)), 10.0f); //Spc Exp, currently defaults to 10 if les than
    const float4 cameraPos = cameraPosition; //Camera Position    (worldspace)
    const float4 fragmentPosition = posTex.Load(DTid, 0); //Fragment Position  (worldspace)
    const float3 fragementNormal = normalize(normTex.Load(DTid, 0)); //normal for fragment
    const float4 texColor = colTex.Load(DTid, 0); //Texture Color
    const float2 UVpos = uvTex.Load(DTid, 0).xy; //UV !!UNUSED!!
    
    //Placeholder/Hardcoded?//hämta in från ljuset via struct
    float ambientStrenght = 0.1f;
    float diffuseStrenght = 0.0f;
    float specularStrenght = 10.5f;
    
    //Changes per light
    float4 lightColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 lightPos = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    //Cumulative
    float3 diffuse = float3(0.0f, 0.0f, 0.0f);
    float3 specular = float3(0.0f, 0.0f, 0.0f);
    //lägg till range baserad fall of på ljus
 
    //For each light check depth
    for (int i = 0; i < nrOfLights; i++)
    {
        lightColor = lightStructBuff[i].lightColor;
        lightPos = lightStructBuff[i].lightPos;
        //float4 ambientStrenght = lightStructBuff[i].ambientComponent;
        //float4 diffuseStrenght = lightStructBuff[i].diffuseComponent;
        //float4 specularStrenght =lightStructBuff[i].specularComponent;
        
        // -------------Shading--------------- //
        
        //fragments position transformed into lights view->clip space, then convert to ndc by dividing by w. Z channel represents depth from light
        float4 fragPosInLightView = mul(fragmentPosition, lightStructBuff[i].ltViewMatix);
        fragPosInLightView.xy /= fragPosInLightView.w;
        float depth = fragPosInLightView.z / fragPosInLightView.w;
        
        //modify xy value range from (-1 to 1) to (0 to 1)
        float2 smTex = float2(0.5f * fragPosInLightView.x + 0.5f, -0.5f * fragPosInLightView.y + 0.5f);
    
        //get depth at pos in shadow map and compare to fragPos depth. Offset added to remove shadow acne.
        if (texArray.SampleLevel(testSampler, float3(smTex.xy, 0), i).r > depth - 0.005)
        {
            // ------------- Diffuse --------------  //

            //normalized vector from fragment to light
            const float3 fragToLight = normalize(lightPos.xyz - fragmentPosition.xyz);

	        //calculate diffuse light
            float diff = max(dot((fragToLight), fragementNormal), 0.0);
            
            //multiply with light color
            diffuse += diff * lightColor; //diffuseStrenght * (lightColor*?) (diff * diffuseColor)
    
            
            //	------------- Specular -------------- //

            //vector from fragment to camera
            const float3 fragToCam = normalize(cameraPos.xyz - fragmentPosition.xyz);
    
            //reflected vector
            float3 reflectedLight = normalize(reflect(-fragToLight, fragementNormal));

            //phong type
            float spec = pow(max(dot(fragToCam, reflectedLight), 0.0f), specularExponent);
    
            
            specular += specularStrenght * spec * lightColor; //specularStrenght * (lightColor*?) (spec * specularColor)?
        }
    } //problem: kommer använda endast sista ljusets data. lägg += på specular och diffuse
    
  
    //	------------- Ambient ---------------//
    
    const float3 ambient = ambientStrenght * lightColor; //ambientStrenght * (lightColor*) ambientColor (ta bort strenght?)
   
    //Combine Light for fragment
    const float4 resultColor = float4((ambient + diffuse + specular), 0);
    
    //Write to cubeMapArray Texture
    uint3 writeToLocation = uint3(DTid.xy, 0);
    backBuffer[writeToLocation] = float4(saturate(ambient + diffuse) + specular, 0) * texColor;
}


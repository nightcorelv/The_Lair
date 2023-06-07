RWBuffer<float> particlePositions : register(u0);

cbuffer timeValue : register(b0)
{
    float time;
};

cbuffer PlayerPos : register(b2)
{
    float3 playerPosition;
    float radius;
};

[numthreads(1, 1, 1)]
void main(int3 DTid : SV_DispatchThreadID)
{
    //particlePositions[DTid.x * 3] = particlePositions[DTid.x * 3] ; //+cos(time * +DTid.x);
    
    //if (particlePositions[DTid.x * 3 + 1] > 15)
    //{
    //    particlePositions[DTid.x * 3 + 1] = particlePositions[DTid.x * 3 + 1] - 25; //sin(time * 1.1f + DTid.x);
    //}
    //else
    //{
    //    particlePositions[DTid.x * 3 + 1] = particlePositions[DTid.x * 3 + 1] + (time * 0.02); //sin(time * 1.1f + DTid.x);
    //}
    //particlePositions[DTid.x * 3 + 2] = particlePositions[DTid.x * 3 + 2] ; //+sin(time * +DTid.x); //sin(time * 1.1f + DTid.x);

    particlePositions[DTid.x * 3] = cos(time * 0.1 * DTid.x) * radius + playerPosition.x;
    if (particlePositions[DTid.x * 3 + 1] + playerPosition.y > playerPosition.y + 5.0f)
    {
        particlePositions[DTid.x * 3 + 1] = playerPosition.y;
    }
    else
    {
        particlePositions[DTid.x * 3 + 1] += (1 - (cos(time * 3.1415926 * DTid.x) * 0.5f + 0.5f)) * 0.3f;
    }
    //particlePositions[DTid.x * 3 + 1] = (DTid.x * 0.3f + playerPosition.y > playerPosition.y + 1) ? playerPosition.y : particlePositions[DTid.x * 3 + 1] + 0.1;
    particlePositions[DTid.x * 3 + 2] = sin(time * 0.1 * DTid.x) * radius + playerPosition.z;
    
}
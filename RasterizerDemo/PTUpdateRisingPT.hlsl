RWBuffer<float> particlePositions : register(u0);

cbuffer timeValue : register(b0)
{
    float time;
};

cbuffer PlayerPos : register(b1)
{
    float4 particlePos;//w element determines on or off
};

[numthreads(1, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    
    if (particlePos.w > 0)
    {
        particlePositions[DTid.x * 3] = particlePositions[DTid.x * 3];
    
        if (particlePositions[DTid.x * 3 + 1] > 15)
        {
            particlePositions[DTid.x * 3 + 1] = particlePositions[DTid.x * 3 + 1] - 25;
        }
        else
        {
            particlePositions[DTid.x * 3 + 1] = particlePositions[DTid.x * 3 + 1] + (time * 0.02);
        }
        particlePositions[DTid.x * 3 + 2] = particlePositions[DTid.x * 3 + 2];
    }
    else
    {
        particlePositions[DTid.x * 3] = 0;
        particlePositions[DTid.x * 3 + 1] = 0;
        particlePositions[DTid.x * 3 + 2] = 0;
    }
    
        //particlePositions[DTid.x * 3] = cos(time * DTid.x) * 1 + particlePos.x;
    //particlePositions[DTid.x * 3 + 1] = DTid.x * 0.3f + particlePos.y;
    //particlePositions[DTid.x * 3 + 2] = sin(time * DTid.x) * 1 + particlePos.z;
        
   // float x = particlePositions[DTid.x * 3];
   // float y = particlePositions[DTid.x * 3 + 1];
   // float z = particlePositions[DTid.x * 3 + 2];
    
   // float time2 = time;
    
   // while (time2 >= 10.0f)
   // {
   //     time2 -= 10.0f;
   // }
    
   // //if (y > 15)
   // //{
   // //    y = DTid.x - 25; //sin(time * 1.1f + DTid.x);
   // //}
   // //else
   // //{
   // //    y = DTid.x * 0.3f + particlePos.y + time;
   // //}
   // y = DTid.x * 0.3f + particlePos.y + time2;
   // //if (particlePos.w > 0)
   // //{
        
   // //   // x = DTid + particlePos.x; //+cos(time * +DTid.x);
   // //    //sin(time * 1.1f + DTid.x);
        
        
   // //    if (y > 15)
   // //    {
   // //        y = DTid.x - 25; //sin(time * 1.1f + DTid.x);
   // //    }
   // //    else
   // //    {
   // //        y = DTid.x * 0.3f + particlePos.y + time;
   // //    }
        
        
   // //   // z = DTid.x * 0.3f + particlePos.z; //+sin(time * +DTid.x); //sin(time * 1.1f + DTid.x);

   // //}
   // particlePositions[DTid.x * 3] = x;
   // particlePositions[DTid.x * 3 + 1] = y;
   //particlePositions[DTid.x * 3 + 2] = z;
    
    //+ particlePos.x
    //+ particlePos.y
    //+ particlePos.z
   
    

    
    
    //particlePositions[DTid.x * 3] = cos(time * DTid.x) * 1 + particlePos.x;
    //particlePositions[DTid.x * 3 + 1] = DTid.x * 0.3f + particlePos.y;
    //particlePositions[DTid.x * 3 + 2] = sin(time * DTid.x) * 1 + particlePos.z;
}
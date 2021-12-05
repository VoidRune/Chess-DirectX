/* vertex attributes go here to input to the vertex shader */
struct vs_in {
    float3 position_local : Position;
    float3 textureCoord : TextureCoord;
};

/* outputs from vertex shader go here. can be interpolated to pixel shader */
struct vs_out {
    float4 position_clip : SV_POSITION; // required output of VS
    float3 pos : Position;
    float2 textureCoord : TextureCoord;
};

Texture2D tex : register(t0);
SamplerState splr : register(s0);

vs_out vs_main(vs_in input)
{
    vs_out output = (vs_out)0; // zero the memory first
    output.position_clip = float4(input.position_local, 1.0);
    output.textureCoord = float2(input.textureCoord.xy);
    output.pos = float3(input.position_local);
    return output;
}

#define INTENSITY 0.8f

float getDist(float2 a, float2 b)
{
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

float4 ps_main(vs_out input) : SV_Target0
{


    uint width, height;
    tex.GetDimensions(width, height);
    /*
    float3 spherespos[5] = 
    {
      0.4f, 0.2f, 1.0f,   
      0.3f, 0.3f, 2.0f,   
      0.2f, 0.4f, 3.0f,   
      0.4f, 0.5f, 4.0f,   
      0.5f, 0.6f, 5.0f   
    };

    float3 spherescol[5] =
    {
      0.1f, 0.6f, 0.1f,   
      0.2f, 0.2f, 0.7f,   
      0.6f, 0.2f, 0.1f,   
      0.2f, 0.8f, 0.5f,   
      0.7f, 0.1f, 0.4f 
    };
    float2 sun = { 0, 0 };
    float sunDistance = getDist(float2(sun.x * width, sun.y * height), float2(input.textureCoord.x * width, input.textureCoord.y * height));
    float3 color = {0, 0, 0};
    int depth = 0;
    for (int i = 0; i < 5; i++)
    {
        int distance = getDist(float2(spherespos[i].x * width, spherespos[i].y * height), float2(input.textureCoord.x * width, input.textureCoord.y * height));
        if (distance < 100)
        {
          if (depth < spherespos[i].z)
          {
            color = spherescol[i];
            depth = spherespos[i].z;
          }
        }
    }
    

    return float4(color, 1.0f);
    */
    //if (getDist(float2(spheres[i].x * width, spheres[i].y * height), float2(input.textureCoord.x * width, input.textureCoord.y * height)) < 100)
    //    return float4(0.0f, 0.0f, 1.0f, 1.0f);
    //return float4(0.0f, 1.0f, 0.0f, 1.0f);


    //--------HORIZONTAL GAUSSIAN BLUR
    //float weight[5] = { 0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216 };
    //
    //float2 tex_offset = 1.0 / width;
    //float3 result = tex.Sample(splr, input.textureCoord).rgb * weight[0];
    //
    //for (int i = 1; i < 5; ++i)
    //{
    //    result += tex.Sample(splr, input.textureCoord + float2(tex_offset.x * i, 0.0)).rgb * weight[i];
    //    result += tex.Sample(splr, input.textureCoord + float2(tex_offset.x * i, 0.0)).rgb * weight[i];
    //}
    //
    //return float4(result.rgb, 1.0f);

    //-----------HDR => High Dynamic Range
    //const float gamma = 2.2;
    //const float exposure = 1.2;
    //float3 hdrColor = tex.Sample(splr, input.textureCoord);
    //
    //// exposure tone mapping
    //float3 mapped = float3(1, 1, 1) -exp(-hdrColor * exposure);
    //// gamma correction 
    //mapped = pow(mapped, float3(1.0 / gamma, 1.0 / gamma, 1.0 / gamma));
    //
    //return float4(mapped, 1.0);

    //Pixels towards the edge get darker
    float dist = 1.0f - distance(float2((input.position_clip.x - width / 2) / width, (input.position_clip.y - height / 2) / height) * INTENSITY, float2(0.0f, 0.0f));

    float3 texColor = tex.Sample(splr, input.textureCoord);
    return float4(texColor * sqrt(dist), 1);

    //----------INVERTED
    //return float4(1 - texColor * dist, 1);

    //----------GREYSCALE IMAGE
    //float avg = (texColor.r + texColor.g + texColor.b) / 3;
    //return float4(float3(avg, avg, avg) * dist, 1);
}
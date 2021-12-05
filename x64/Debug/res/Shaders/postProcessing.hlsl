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

#define INTENSITY 0.6f

float4 ps_main(vs_out input) : SV_Target0
{
    uint width, height;
    tex.GetDimensions(width, height);

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
    return float4(texColor * dist, 1);

    //----------INVERTED
    //return float4(1 - texColor * dist, 1);

    //----------GREYSCALE IMAGE
    //float avg = (texColor.r + texColor.g + texColor.b) / 3;
    //return float4(float3(avg, avg, avg) * dist, 1);
}
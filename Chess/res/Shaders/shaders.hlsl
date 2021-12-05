/* vertex attributes go here to input to the vertex shader */
struct vs_in {
    float3 position_local : Position;
    float3 textureCoord : TextureCoord;
};

/* outputs from vertex shader go here. can be interpolated to pixel shader */
struct vs_out {
    float4 position_clip : SV_POSITION; // required output of VS
    float3 pos : Position;
    float3 textureCoord : TextureCoord;
};

cbuffer Cbuff : register(b0)
{
    matrix transform;
};

Texture2DArray tex : register(t0);
SamplerState splr : register(s0);

vs_out vs_main(vs_in input) {
    vs_out output = (vs_out)0; // zero the memory first
    output.position_clip = mul(float4(input.position_local, 1.0), transform);
    output.textureCoord = float3(input.textureCoord);
    output.pos = float3(input.position_local);
    return output;
}

float4 ps_main(vs_out input) : SV_Target{

    float4 texColor = tex.Sample(splr, float3(input.textureCoord.xyz));
    if (texColor.w < 0.1)
        discard;
    return texColor;
}
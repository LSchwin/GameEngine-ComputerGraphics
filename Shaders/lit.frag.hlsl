cbuffer Uniforms : register(b0, space3)
{
    float4 color;
};

struct Input
{
	float2 texcoord : TEXTCOORD0;
	float4 color : TEXCOORD1;
};

struct Output
{
	float4 Color : SV_Target0;
};

Output main(Input input)
{
	Output output;
	output.Color = color; //u_texture.Sample(u_sampler, input.texcoord) * input.color

	return output;
}

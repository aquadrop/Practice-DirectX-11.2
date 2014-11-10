// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved
//----------------------------------------------------------------------

Texture2D Texture : register(t0);
SamplerState Sampler : register(s0);

struct PixelShaderInput
{
    float4 pos : SV_POSITION;
    float3 norm : NORMAL;
    float2 tex : TEXCOORD0;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
	float3 lightDirection1 = normalize(float3(1, -1, 0));
	float3 lightDirection2 = normalize(float3(-1, 1, 0));
	return Texture.Sample(Sampler, input.tex) * (0.8f * saturate(dot(normalize(input.norm), -lightDirection1)) + 1.5f * saturate(dot(normalize(input.norm), -lightDirection2)) + 0.2f);
}

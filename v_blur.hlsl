/********************************************************************************
 Copyright (C) 2013 Ginshali.info

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.
********************************************************************************/

uniform Texture2D diffuseTexture;
uniform float height;

SamplerState textureSampler
{
    AddressU  = Clamp;
    AddressV  = Clamp;
    Filter    = Linear;
};

struct VS_OUT {
    float4 pos : POSITION;
    float2 uv : TEXCOORD0;
};

float4 main(VS_OUT input) : SV_TARGET
{
    const float weights[11] = { 0.01, 0.05, 0.09, 0.12, 0.15, 0.16, 0.15, 0.12, 0.09, 0.05, 0.01 };
    const float delta_v[11] = { -5.0, -4.0, -3.0, -2.0, -1.0,  0.0,  1.0,  2.0,  3.0,  4.0,  5.0 };
    
    float size = 1.0 / height;
    float4 color = float4(0.0, 0.0, 0.0, 0.0);

    for (uint i = 0; i < 11; i++)
    {
        color += diffuseTexture.Sample(textureSampler, float2(input.uv.x, input.uv.y + delta_v[i] * size)) * weights[i];
    }

    return float4(color.r, color.g, color.b, 1.0);
}
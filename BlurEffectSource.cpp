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

#include "stdafx.h"
#include "BlurEffectSource.h"


bool BlurEffectSource::Init(XElement* data)
{
	baseSize = OBSGetBaseSize();
	texture = CreateTexture((UINT)baseSize.x, (UINT)baseSize.y, GS_BGRA, NULL, FALSE, TRUE);
	
	horizontalBlurShader = CreatePixelShaderFromFile(TEXT("plugins/BlurPlugin/h_blur.hlsl"));
	verticalBlurShader = CreatePixelShaderFromFile(TEXT("plugins/BlurPlugin/v_blur.hlsl"));
	horizontalBlurShader->SetFloat(horizontalBlurShader->GetParameterByName(TEXT("width")), baseSize.x);
	verticalBlurShader->SetFloat(verticalBlurShader->GetParameterByName(TEXT("height")), baseSize.y);

    Log(TEXT("Using blur effect plugin"));

	return true;
}

BlurEffectSource::~BlurEffectSource()
{
	delete verticalBlurShader;
	delete horizontalBlurShader;
	delete texture;
}


void BlurEffectSource::Render(const Vect2 &pos, const Vect2 &size)
{
	Shader* oldShader = GetCurrentPixelShader();
	SamplerState* sampler = NULL;

	if (verticalBlurShader && horizontalBlurShader)
	{
		float x = pos.x;
		float y = pos.y;
		float x2 = (x + size.x);
		float y2 = (y + size.y);
		float u = x / baseSize.x;
		float v = y / baseSize.y;
		float u2 = x2 / baseSize.x;
		float v2 = y2 / baseSize.y;
		
		// get render target
		Texture* renderTarget = GetRenderTarget();
		
		// apply vertex blur effect
		GS->CopyTexture(texture, renderTarget);
		LoadPixelShader(verticalBlurShader);
		DrawSpriteEx(texture, 0xFFFFFFFF, x, y, x2, y2, u, v, u2, v2);

		// apply vertex blur effect
		GS->CopyTexture(texture, renderTarget);
		LoadPixelShader(horizontalBlurShader);
		DrawSpriteEx(texture, 0xFFFFFFFF, x, y, x2, y2, u, v, u2, v2);

		// restore shader
		LoadPixelShader(oldShader);
	}
}

Vect2 BlurEffectSource::GetSize() const
{
	return API->GetBaseSize();
}
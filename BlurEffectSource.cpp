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
#include <string>

#define VERT_EFFECT_NAME "VerticalEffect"
#define HORIZ_EFFECT_NAME "HorizontalEffect"

bool BlurEffectSource::Init(XElement* data)
{
    this->data = data;

    horizValue = data->GetFloat(TEXT("horizValue"), 1.0f);
    vertValue = data->GetFloat(TEXT("vertValue"), 1.0f);

    baseSize = OBSGetBaseSize();
    
    Shader* horizontalBlurShader = CreatePixelShaderFromFile(TEXT("plugins/BlurPlugin/h_blur.hlsl"));
    Shader* verticalBlurShader = CreatePixelShaderFromFile(TEXT("plugins/BlurPlugin/v_blur.hlsl"));

    horizontalBlurEffect = new Effect(horizontalBlurShader, 0.0f, 0.0f, baseSize.x, baseSize.y);
    verticalBlurEffect = new Effect(verticalBlurShader, 0.0f, 0.0f, baseSize.x, baseSize.y);
    
    AddEffect(HORIZ_EFFECT_NAME, horizontalBlurEffect);
    AddEffect(VERT_EFFECT_NAME, verticalBlurEffect);

    Log(TEXT("Using blur effect plugin"));

    return true;
}

BlurEffectSource::~BlurEffectSource()
{
}


void BlurEffectSource::BeforeRender(const Vect2 &pos, const Vect2 &size)
{
    if (horizValue > 0.0f)
    {
        horizontalBlurEffect->enabled = true;
        horizontalBlurEffect->shader->SetFloat(
            horizontalBlurEffect->shader->GetParameterByName(TEXT("width")),
            baseSize.x / horizValue);
        horizontalBlurEffect->pos = pos;
        horizontalBlurEffect->size = size;
    }
    else
    {
        horizontalBlurEffect->enabled = false;
    }

    if (vertValue > 0.0f)
    {
        verticalBlurEffect->enabled = true;
        verticalBlurEffect->shader->SetFloat(
            verticalBlurEffect->shader->GetParameterByName(TEXT("height")),
            baseSize.y / vertValue);
        verticalBlurEffect->pos = pos;
        verticalBlurEffect->size = size;
    }
    else
    {
        verticalBlurEffect->enabled = false;
    }
}

Vect2 BlurEffectSource::GetSize() const
{
    return API->GetBaseSize();
}

void BlurEffectSource::SetFloat(CTSTR lpName, float fVal)
{
    if (scmpi(lpName, TEXT("vertValue")) == 0)
    {
        vertValue = fVal;
    }
    else if (scmpi(lpName, TEXT("horizValue")) == 0)
    {
        horizValue = fVal;
    }
}
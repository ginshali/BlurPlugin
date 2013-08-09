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

#pragma once

class BlurEffectSource :
    public EffectSource
{
    Vect2 baseSize;
    XElement* data;

    Effect* horizontalBlurEffect;
    Effect* verticalBlurEffect;

    float vertValue;
    float horizValue;

public:
    bool Init(XElement* data);
    ~BlurEffectSource();

    void BeforeRender(const Vect2 &pos, const Vect2 &size);
    Vect2 GetSize() const;

    void SetFloat(CTSTR lpName, float fVal);
};


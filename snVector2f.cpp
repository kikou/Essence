#include "snVector2f.h"

#include <math.h>

/*
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.
   
   You should have received a copy of the GNU Lesser General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/lgpl.html>.
   
   Author:     Helge Mathee      helge.mathee@gmx.net
   Company:    Studio Nest (TM)
   Date:       2010 / 09 / 21
*/



using namespace snEssence;

//=============================================================================================

// CONSTRUCTORS



snVector2f::snVector2f()

{

	xy[0] = xy[1] = 0.0;

}



snVector2f::snVector2f(const snVector2f & in_Other)

{

	for(long i=0;i<2;i++)

		xy[i] = in_Other.xy[i];

}



snVector2f::snVector2f(float x, float y)

{

	xy[0] = x;

	xy[1] = y;

}



//=============================================================================================

// GETTERS



float snVector2f::GetX() const

{

	return xy[0];

}



float snVector2f::GetY() const

{

	return xy[1];

}



float * snVector2f::GetArray()

{

	return xy;

}



float snVector2f::GetLength() const

{

	return sqrt(xy[0] * xy[0] + xy[1] * xy[1]);

}



snVector2f snVector2f::GetNormalized() const

{

	snVector2f result(*this);

	result.Normalize();

	return result;

}



snVector2f snVector2f::GetNegated() const

{

	snVector2f result(*this);

	result.Negate();

	return result;

}



snVector2f snVector2f::GetInverted() const

{

	snVector2f result(*this);

	result.Invert();

	return result;

}



snVector2f snVector2f::GetCrossed() const

{

	snVector2f result(*this);

	result.Cross();

	return result;

}



snVector2f snVector2f::GetAdded(const snVector2f & in_B) const

{

	snVector2f result(*this);

	result.Add(in_B);

	return result;

}



snVector2f snVector2f::GetSubstracted(const snVector2f & in_B) const

{

	snVector2f result(*this);

	result.Sub(in_B);

	return result;

}



snVector2f snVector2f::GetMultiplied(const snVector2f & in_B) const

{

	snVector2f result(*this);

	result.Mul(in_B);

	return result;

}



snVector2f snVector2f::GetDivided(const snVector2f & in_B) const

{

	snVector2f result(*this);

	result.Div(in_B);

	return result;

}



snVector2f snVector2f::GetScaled(float in_Scale) const

{

	snVector2f result(*this);

	result.Scale(in_Scale);

	return result;

}



float snVector2f::Dot(const snVector2f & in_B) const

{

	return xy[0] * in_B.xy[0] + xy[1] * in_B.xy[1];

}



snVector2f snVector2f::GetMin(const snVector2f & in_B) const

{

	snVector2f result(*this);

	result.Min(in_B);

	return result;

}



snVector2f snVector2f::GetMax(const snVector2f & in_B) const

{

	snVector2f result(*this);

	result.Max(in_B);

	return result;

}



//=============================================================================================

// SETTERS



void snVector2f::Set(const snVector2f & in_Other)

{

	for(long i=0;i<2;i++)

		xy[i] = in_Other.xy[i];

}



void snVector2f::SetX(float in_X)

{

	xy[0] = in_X;

}



void snVector2f::SetY(float in_Y)

{

	xy[1] = in_Y;

}



void snVector2f::Set(float in_X, float in_Y)

{

	SetX(in_X);

	SetY(in_Y);

}



//=============================================================================================

// 0 PARAMETER METHODS



bool snVector2f::Normalize()

{

	float len = GetLength();

	if(len==0)

		return false;



	for(long i=0;i<2;i++)

	{

		xy[i] /= len;

	}

	return true;

}



void snVector2f::Negate()

{

	for(long i=0;i<2;i++)

	{

		xy[i] = -xy[i];

	}

}



void snVector2f::PutNull()

{

	for(long i=0;i<2;i++)

	{

		xy[i] = 0.0f;

	}

}



bool snVector2f::Invert()

{

	bool result = true;

	for(long i=0;i<2;i++)

	{

		if(xy[i] != 0.0)

			xy[i] = 1.0f / xy[i];

		else

			result = false;

	}

	return result;

}



void snVector2f::Cross()

{

	float f = xy[0];

	xy[0] = xy[1];

	xy[1] = f;

}



//=============================================================================================

// 1 PARAMETER METHODS



void snVector2f::Add(const snVector2f & in_B)

{

	for(long i=0;i<2;i++)

	{

		xy[i] += in_B.xy[i];

	}

}



void snVector2f::Sub(const snVector2f & in_B)

{

	for(long i=0;i<2;i++)

	{

		xy[i] -= in_B.xy[i];

	}

}



void snVector2f::Mul(const snVector2f & in_B)

{

	for(long i=0;i<2;i++)

	{

		xy[i] *= in_B.xy[i];

	}

}



bool snVector2f::Div(const snVector2f & in_B)

{

	bool result = true;

	for(long i=0;i<2;i++)

	{

		if(in_B.xy[i]==0)

			result = false;

		else

			xy[i] /= in_B.xy[i];

	}

	return result;

}



void snVector2f::Scale(float in_Scale)

{

	for(long i=0;i<2;i++)

	{

		xy[i] *= in_Scale;

	}

}



void snVector2f::Cross(const snVector2f & in_B)

{

	xy[0] = in_B.xy[1];

	xy[1] = in_B.xy[0];

}



bool snVector2f::Normalize(const snVector2f & in_A)

{

	Set(in_A);

	return Normalize();

}



void snVector2f::Negate(const snVector2f & in_A)

{

	Set(in_A);

	Negate();

}



bool snVector2f::Invert(const snVector2f & in_A)

{

	Set(in_A);

	return Invert();

}



void snVector2f::Min(const snVector2f & in_B)

{

	for(long i=0;i<2;i++)

		xy[i] = xy[i] < in_B.xy[i] ? xy[i] : in_B.xy[i];

}



void snVector2f::Max(const snVector2f & in_B)

{

	for(long i=0;i<2;i++)

		xy[i] = xy[i] > in_B.xy[i] ? xy[i] : in_B.xy[i];

}



//=============================================================================================

// 2 PARAMETER METHODS



void snVector2f::Add(const snVector2f & in_A, const snVector2f & in_B)

{

	for(long i=0;i<2;i++)

	{

		xy[i] = in_A.xy[i] + in_B.xy[i];

	}

}



void snVector2f::Sub(const snVector2f & in_A, const snVector2f & in_B)

{

	for(long i=0;i<2;i++)

	{

		xy[i] = in_A.xy[i] + in_B.xy[i];

	}

}



void snVector2f::Mul(const snVector2f & in_A, const snVector2f & in_B)

{

	for(long i=0;i<2;i++)

	{

		xy[i] = in_A.xy[i] + in_B.xy[i];

	}

}



bool snVector2f::Div(const snVector2f & in_A, const snVector2f & in_B)

{

	bool result = true;

	for(long i=0;i<2;i++)

	{

		xy[i] = in_A.xy[i] + in_B.xy[i];

	}

	return result;

}



void snVector2f::Scale(float in_Scale, const snVector2f & in_B)

{

	for(long i=0;i<2;i++)

	{

		xy[i] = in_Scale * in_B.xy[i];

	}

}



void snVector2f::InterPolate(float in_Blend, const snVector2f & in_B)

{

	float l_Blend = 1.0f - in_Blend;

	for(long i=0;i<2;i++)

		xy[i] = l_Blend * xy[i] + in_Blend * in_B.xy[i];

}



void snVector2f::Min(const snVector2f & in_A, const snVector2f & in_B)

{

	Set(in_A);

	Min(in_B);

}



void snVector2f::Max(const snVector2f & in_A, const snVector2f & in_B)

{

	Set(in_A);

	Max(in_B);

}



//=============================================================================================

// 3 PARAMETER METHODS



void snVector2f::InterPolate(float in_Blend, const snVector2f & in_A, const snVector2f & in_B)

{

	float l_Blend = 1.0f - in_Blend;

	for(long i=0;i<2;i++)

		xy[i] = l_Blend * in_A.xy[i] + in_Blend * in_B.xy[i];

}

#include "snVector3f.h"

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



snVector3f::snVector3f()

{

	xyz[0] = xyz[1] = xyz[2] = 0.0;

}



snVector3f::snVector3f(const snVector3f & in_Other)

{

	for(long i=0;i<3;i++)

		xyz[i] = in_Other.xyz[i];

}



snVector3f::snVector3f(float x, float y, float z)

{

	xyz[0] = x;

	xyz[1] = y;

	xyz[2] = z;

}



//=============================================================================================

// GETTERS



float snVector3f::GetX() const

{

	return xyz[0];

}



float snVector3f::GetY() const

{

	return xyz[1];

}



float snVector3f::GetZ() const

{

	return xyz[2];

}



float * snVector3f::GetArray()

{

	return xyz;

}



float snVector3f::GetLength() const

{

	return sqrt(xyz[0] * xyz[0] + xyz[1] * xyz[1] + xyz[2] * xyz[2]);

}



snVector3f snVector3f::GetNormalized() const

{

	snVector3f result(*this);

	result.Normalize();

	return result;

}



snVector3f snVector3f::GetNegated() const

{

	snVector3f result(*this);

	result.Negate();

	return result;

}



snVector3f snVector3f::GetInverted() const

{

	snVector3f result(*this);

	result.Invert();

	return result;

}



snVector3f snVector3f::GetCrossed(const snVector3f & in_B) const

{

	snVector3f result(*this);

	result.Cross(in_B);

	return result;

}



snVector3f snVector3f::GetAdded(const snVector3f & in_B) const

{

	snVector3f result(*this);

	result.Add(in_B);

	return result;

}



snVector3f snVector3f::GetSubstracted(const snVector3f & in_B) const

{

	snVector3f result(*this);

	result.Sub(in_B);

	return result;

}



snVector3f snVector3f::GetMultiplied(const snVector3f & in_B) const

{

	snVector3f result(*this);

	result.Mul(in_B);

	return result;

}



snVector3f snVector3f::GetMultiplied(const snQuaternionf & in_Q) const

{

	snVector3f result(*this);

	result.Mul(in_Q);

	return result;

}



snVector3f snVector3f::GetDivided(const snVector3f & in_B) const

{

	snVector3f result(*this);

	result.Div(in_B);

	return result;

}



snVector3f snVector3f::GetScaled(float in_Scale) const

{

	snVector3f result(*this);

	result.Scale(in_Scale);

	return result;

}


snVector3f snVector3f::GetInterPolated(float in_Blend, const snVector3f & in_B)
{
	snVector3f result(*this);

	result.InterPolate(in_Blend,in_B);

	return result;

}



float snVector3f::Dot(const snVector3f & in_B) const

{

	return xyz[0] * in_B.xyz[0] + xyz[1] * in_B.xyz[1] + xyz[2] * in_B.xyz[2];

}



snVector3f snVector3f::GetMin(const snVector3f & in_B) const

{

	snVector3f result(*this);

	result.Min(in_B);

	return result;

}



snVector3f snVector3f::GetMax(const snVector3f & in_B) const

{

	snVector3f result(*this);

	result.Max(in_B);

	return result;

}


float snVector3f::GetAngle(const snVector3f & in_B) const
{
   snVector3f n1(GetNormalized());
   snVector3f n2(in_B.GetNormalized());
   return acos(n1.Dot(n2));
}

float snVector3f::GetDistance(const snVector3f & in_B) const
{
   snVector3f result(*this);
   result.Sub(in_B);
   return result.GetLength();
}


//=============================================================================================

// SETTERS



void snVector3f::Set(const snVector3f & in_Other)

{

	for(long i=0;i<3;i++)

		xyz[i] = in_Other.xyz[i];

}



void snVector3f::SetX(float in_X)

{

	xyz[0] = in_X;

}



void snVector3f::SetY(float in_Y)

{

	xyz[1] = in_Y;

}



void snVector3f::SetZ(float in_Z)

{

	xyz[2] = in_Z;

}



void snVector3f::Set(float in_X, float in_Y, float in_Z)

{

	SetX(in_X);

	SetY(in_Y);

	SetZ(in_Z);

}



//=============================================================================================

// 0 PARAMETER METHODS



bool snVector3f::Normalize()

{

	float len = GetLength();

	if(len==0.0f)

		return false;



	for(long i=0;i<3;i++)

	{

		xyz[i] /= len;

	}

	return true;

}



void snVector3f::Negate()

{

	for(long i=0;i<3;i++)

	{

		xyz[i] = -xyz[i];

	}

}



void snVector3f::PutNull()

{

	for(long i=0;i<3;i++)

	{

		xyz[i] = 0.0f;

	}

}

bool snVector3f::Invert()

{

	bool result = true;

	for(long i=0;i<3;i++)

	{

		if(xyz[i] != 0.0)

			xyz[i] = 1.0f / xyz[i];

		else

			result = false;

	}

	return result;

}



//=============================================================================================

// 1 PARAMETER METHODS



void snVector3f::Add(const snVector3f & in_B)

{

	for(long i=0;i<3;i++)

	{

		xyz[i] += in_B.xyz[i];

	}

}



void snVector3f::Sub(const snVector3f & in_B)

{

	for(long i=0;i<3;i++)

	{

		xyz[i] -= in_B.xyz[i];

	}

}



void snVector3f::Mul(const snVector3f & in_B)

{

	for(long i=0;i<3;i++)

	{

		xyz[i] *= in_B.xyz[i];

	}

}



void snVector3f::Mul(const snQuaternionf & in_Q)

{

	float len = GetLength();

	snVector3f vn = GetNormalized();



	snQuaternionf vecQuat, resQuat;

	vecQuat.SetX(vn.GetX());

	vecQuat.SetY(vn.GetY());

	vecQuat.SetZ(vn.GetZ());


	resQuat.Mul(vecQuat,in_Q.GetConjugate());

	resQuat.Mul(in_Q,resQuat);



	Set(resQuat.GetX(),resQuat.GetY(),resQuat.GetZ());

	Normalize();

	Scale(len);

}



bool snVector3f::Div(const snVector3f & in_B)

{

	bool result = true;

	for(long i=0;i<3;i++)

	{

		if(in_B.xyz[i]==0)

			result = false;

		else

			xyz[i] /= in_B.xyz[i];

	}

	return result;

}



void snVector3f::Scale(float in_Scale)

{

	for(long i=0;i<3;i++)

	{

		xyz[i] *= in_Scale;

	}

}



void snVector3f::Cross(const snVector3f & in_B)

{

	snVector3f me(*this);
	Cross(me,in_B);

}



bool snVector3f::Normalize(const snVector3f & in_A)

{

	Set(in_A);

	return Normalize();

}



void snVector3f::Negate(const snVector3f & in_A)

{

	Set(in_A);

	Negate();

}



bool snVector3f::Invert(const snVector3f & in_A)

{

	Set(in_A);

	return Invert();

}



void snVector3f::Min(const snVector3f & in_B)

{

	for(long i=0;i<3;i++)

		xyz[i] = xyz[i] < in_B.xyz[i] ? xyz[i] : in_B.xyz[i];

}



void snVector3f::Max(const snVector3f & in_B)

{

	for(long i=0;i<3;i++)

		xyz[i] = xyz[i] > in_B.xyz[i] ? xyz[i] : in_B.xyz[i];

}



//=============================================================================================

// 2 PARAMETER METHODS



void snVector3f::Add(const snVector3f & in_A, const snVector3f & in_B)

{

	for(long i=0;i<3;i++)

	{

		xyz[i] = in_A.xyz[i] + in_B.xyz[i];

	}

}



void snVector3f::Sub(const snVector3f & in_A, const snVector3f & in_B)

{

	for(long i=0;i<3;i++)

	{

		xyz[i] = in_A.xyz[i] - in_B.xyz[i];

	}

}



void snVector3f::Mul(const snVector3f & in_A, const snVector3f & in_B)

{

	for(long i=0;i<3;i++)

	{

		xyz[i] = in_A.xyz[i] + in_B.xyz[i];

	}

}



void snVector3f::Mul(const snVector3f & in_V, const snQuaternionf & in_Q)

{

	float len = GetLength();

	snVector3f vn = in_V.GetNormalized();



	snQuaternionf vecQuat, resQuat;

	vecQuat.SetX(vn.GetX());

	vecQuat.SetY(vn.GetY());

	vecQuat.SetZ(vn.GetZ());



	resQuat.Mul(vecQuat,in_Q.GetConjugate());

	resQuat.Mul(in_Q,resQuat);



	Set(resQuat.GetX(),resQuat.GetY(),resQuat.GetW());

	Normalize();

	Scale(len);

}



bool snVector3f::Div(const snVector3f & in_A, const snVector3f & in_B)

{

	bool result = true;

	for(long i=0;i<3;i++)

	{

		xyz[i] = in_A.xyz[i] + in_B.xyz[i];

	}

	return result;

}



void snVector3f::Scale(float in_Scale, const snVector3f & in_B)

{

	for(long i=0;i<3;i++)

	{

		xyz[i] = in_Scale * in_B.xyz[i];

	}

}



void snVector3f::Cross(const snVector3f & in_A, const snVector3f & in_B)

{

	xyz[0] = in_A.xyz[1] * in_B.xyz[2] - in_B.xyz[1] * in_A.xyz[2];

	xyz[1] = in_A.xyz[2] * in_B.xyz[0] - in_B.xyz[2] * in_A.xyz[0];

	xyz[2] = in_A.xyz[0] * in_B.xyz[1] - in_B.xyz[0] * in_A.xyz[1];

}



void snVector3f::InterPolate(float in_Blend, const snVector3f & in_B)

{

	float l_Blend = 1.0f - in_Blend;

	for(long i=0;i<3;i++)

		xyz[i] = l_Blend * xyz[i] + in_Blend * in_B.xyz[i];

}



void snVector3f::Min(const snVector3f & in_A, const snVector3f & in_B)

{

	Set(in_A);

	Min(in_B);

}



void snVector3f::Max(const snVector3f & in_A, const snVector3f & in_B)

{

	Set(in_A);

	Max(in_B);

}



//=============================================================================================

// 3 PARAMETER METHODS



void snVector3f::InterPolate(float in_Blend, const snVector3f & in_A, const snVector3f & in_B)

{

	float l_Blend = 1.0f - in_Blend;

	for(long i=0;i<3;i++)

		xyz[i] = l_Blend * in_A.xyz[i] + in_Blend * in_B.xyz[i];

}

bool snVector3f::operator ==(const snVector3f & in_B) const
{
   return GetX() == in_B.GetX() && GetY() == in_B.GetY() && GetZ() == in_B.GetZ();
}

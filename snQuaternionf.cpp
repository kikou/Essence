#include "snQuaternionf.h"

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



snQuaternionf::snQuaternionf()

{

	xyzw[0] = xyzw[1] = xyzw[2] = xyzw[3] = 0.0;

}



snQuaternionf::snQuaternionf(const snQuaternionf & in_Other)

{

	for(long i=0;i<4;i++)

		xyzw[i] = in_Other.xyzw[i];

}



snQuaternionf::snQuaternionf(float x, float y, float z, float w)

{

	xyzw[0] = x;

	xyzw[1] = y;

	xyzw[2] = z;

	xyzw[3] = w;

}



//=============================================================================================

// GETTERS



float snQuaternionf::GetX() const

{

	return xyzw[0];

}



float snQuaternionf::GetY() const

{

	return xyzw[1];

}



float snQuaternionf::GetZ() const

{

	return xyzw[2];

}



float snQuaternionf::GetW() const

{

	return xyzw[3];

}



float * snQuaternionf::GetArray()

{

	return xyzw;

}



snVector3f snQuaternionf::GetAxis()

{

	snVector3f result(xyzw[0],xyzw[1],xyzw[2]);;

	result.Normalize();

	return result;

}



float snQuaternionf::GetAngle()

{

	return acos(xyzw[3]) * 2.0f;

}



snQuaternionf snQuaternionf::GetNormalized() const

{

	snQuaternionf result(*this);

	result.Normalize();

	return result;

}



snQuaternionf snQuaternionf::GetConjugate() const

{

	snQuaternionf result(*this);

	result.Conjugate();

	return result;

}



snQuaternionf snQuaternionf::GetMultiplied(const snQuaternionf & in_B) const

{

	snQuaternionf result(*this);

	result.Mul(in_B);

	return result;

}



//=============================================================================================

// SETTERS



void snQuaternionf::Set(const snQuaternionf & in_Other)

{

	for(long i=0;i<4;i++)

		xyzw[i] = in_Other.xyzw[i];

}



void snQuaternionf::SetX(float in_X)

{

	xyzw[0] = in_X;

}



void snQuaternionf::SetY(float in_Y)

{

	xyzw[1] = in_Y;

}



void snQuaternionf::SetZ(float in_Z)

{

	xyzw[2] = in_Z;

}



void snQuaternionf::SetW(float in_W)

{

	xyzw[3] = in_W;

}



void snQuaternionf::Set(float in_X, float in_Y, float in_Z, float in_W)

{

	SetX(in_X);

	SetY(in_Y);

	SetZ(in_Z);

	SetW(in_W);

}



//=============================================================================================

// CONVERSION



void snQuaternionf::SetAxisAngle(const snVector3f & in_Axis, float in_Angle)

{

	snVector3f axisNorm(in_Axis);

	axisNorm.Normalize();



	float halfAngle = in_Angle * 0.5f;

	float sinAngle = sin(halfAngle);



	xyzw[0] = (axisNorm.GetX() * sinAngle);

	xyzw[1] = (axisNorm.GetY() * sinAngle);

	xyzw[2] = (axisNorm.GetZ() * sinAngle);

	xyzw[3] = cos(halfAngle);

}



void snQuaternionf::SetEuler(float pitch, float yaw, float roll)

{

	float p = pitch * PIOVER180 * 0.5f;

	float y = yaw * PIOVER180 * 0.5f;

	float r = roll * PIOVER180 * 0.5f;



	float sinp = sin(p);

	float siny = sin(y);

	float sinr = sin(r);

	float cosp = cos(p);

	float cosy = cos(y);

	float cosr = cos(r);



	xyzw[0] = sinr * cosp * cosy - cosr * sinp * siny;

	xyzw[0] = cosr * sinp * cosy + sinr * cosp * siny;

	xyzw[0] = cosr * cosp * siny - sinr * sinp * cosy;

	xyzw[0] = cosr * cosp * cosy + sinr * sinp * siny;



	Normalize();

}





//=============================================================================================

// 0 PARAMETER METHODS



bool snQuaternionf::Normalize()

{

	// Don't normalize if we don't have to

	float mag2 = xyzw[0] * xyzw[0] + xyzw[1] * xyzw[1] + xyzw[2] * xyzw[2]+ xyzw[3] * xyzw[3];

	if(mag2==0.0f)

		return false;

	if (fabs(mag2 - 1.0f) > 0.0001) {

		float mag = sqrt(mag2);

		xyzw[0] /= mag;

		xyzw[1] /= mag;

		xyzw[2] /= mag;

		xyzw[3] /= mag;

	}

	return true;

}



void snQuaternionf::Conjugate()

{

   xyzw[0] = -xyzw[0];

   xyzw[1] = -xyzw[1];

   xyzw[2] = -xyzw[2];

   //xyzw[3] = -xyzw[3];

}



//=============================================================================================

// 1 PARAMETER METHODS



void snQuaternionf::Mul(const snQuaternionf & in_B)

{

	snQuaternionf me(*this);

	xyzw[0] = me.xyzw[3] * in_B.xyzw[0] + me.xyzw[0] * in_B.xyzw[3] + me.xyzw[1] * in_B.xyzw[2] - me.xyzw[2] * in_B.xyzw[1];

	xyzw[1] = me.xyzw[3] * in_B.xyzw[1] + me.xyzw[1] * in_B.xyzw[3] + me.xyzw[2] * in_B.xyzw[0] - me.xyzw[0] * in_B.xyzw[2];

	xyzw[2] = me.xyzw[3] * in_B.xyzw[2] + me.xyzw[2] * in_B.xyzw[3] + me.xyzw[0] * in_B.xyzw[1] - me.xyzw[1] * in_B.xyzw[0];

	xyzw[3] = me.xyzw[3] * in_B.xyzw[3] - me.xyzw[0] * in_B.xyzw[0] - me.xyzw[1] * in_B.xyzw[1] - me.xyzw[2] * in_B.xyzw[2];

}



bool snQuaternionf::Normalize(const snQuaternionf & in_A)

{

	Set(in_A);

	return Normalize();

}



void snQuaternionf::Conjugate(const snQuaternionf & in_A)

{

	Set(in_A);

	return Conjugate();

}



//=============================================================================================

// 2 PARAMETER METHODS



void snQuaternionf::Mul(const snQuaternionf & in_A, const snQuaternionf & in_B)

{

	xyzw[0] = in_A.xyzw[3] * in_B.xyzw[0] + in_A.xyzw[0] * in_B.xyzw[3] + in_A.xyzw[1] * in_B.xyzw[2] - in_A.xyzw[2] * in_B.xyzw[1];

	xyzw[1] = in_A.xyzw[3] * in_B.xyzw[1] + in_A.xyzw[1] * in_B.xyzw[3] + in_A.xyzw[2] * in_B.xyzw[0] - in_A.xyzw[0] * in_B.xyzw[2];

	xyzw[2] = in_A.xyzw[3] * in_B.xyzw[2] + in_A.xyzw[2] * in_B.xyzw[3] + in_A.xyzw[0] * in_B.xyzw[1] - in_A.xyzw[1] * in_B.xyzw[0];

	xyzw[3] = in_A.xyzw[3] * in_B.xyzw[3] - in_A.xyzw[0] * in_B.xyzw[0] - in_A.xyzw[1] * in_B.xyzw[1] - in_A.xyzw[2] * in_B.xyzw[2];

}

#include "snTransformf.h"

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

using namespace std;

using namespace snEssence;

//=============================================================================================

// CONSTRUCTORS



snTransformf::snTransformf()

{
   m_Array = NULL;

}


snTransformf::~snTransformf()
{
   DeallocateArray();
}


snTransformf::snTransformf(const snTransformf & in_Other)

{

	m_vPos = in_Other.m_vPos;

	m_qRot = in_Other.m_qRot;

	m_vScl = in_Other.m_vScl;

	// if the array is allocated, copy it also!
	/*
	if(in_Other.m_Array != NULL)
	{
      m_Array = (float*)malloc(sizeof(float)*16);
      memcpy(m_Array,in_Other.m_Array,sizeof(float)*16);
   }
   else

      m_Array = NULL;

   DeallocateArray();
   */
   m_Array = NULL;

}



snTransformf::snTransformf(const snVector3f & in_vPos, const snQuaternionf & in_qRot, const snVector3f & in_vScl)

{

	m_vPos = in_vPos;

	m_qRot = in_qRot;

	m_vScl = in_vScl;

   m_Array = NULL;

}



//=============================================================================================

// GETTERS



snTransformf snTransformf::GetMultiplied( const snTransformf & in_B ) const

{

	snTransformf result(*this);

	result.Mul(in_B);

	return result;

}



//=============================================================================================

// GETTERS



void snTransformf::Set(const snTransformf & in_Other)

{

   DeallocateArray();
	m_vPos = in_Other.m_vPos;

	m_qRot = in_Other.m_qRot;

	m_vScl = in_Other.m_vScl;

}



//=============================================================================================

// 0 PARAMETER METHODS



bool snTransformf::Invert()

{

   DeallocateArray();
	m_vPos.Negate();

	m_qRot.Conjugate();

	return m_vScl.Invert();

}



//=============================================================================================

// 1 PARAMETER METHODS



void snTransformf::Mul( const snTransformf & in_B )

{

   DeallocateArray();
	m_vPos += m_vScl * in_B.m_vPos * m_qRot;

	m_qRot *= in_B.m_qRot;

	m_vScl *= in_B.m_vScl;

}



//=============================================================================================

// 2 PARAMETER METHODS



void snTransformf::Mul( const snTransformf & in_A, const snTransformf & in_B )

{

   DeallocateArray();
	Set(in_A);

	Mul(in_B);

}

//==============================================================================================
// Matrix conversion!
float * snTransformf::GetArray()
{
   if(m_Array == NULL)
   {
      // alloc the memory for the matrix!
      m_Array = (float*)malloc(sizeof(float)*16);
      snVector3f x,y,z;
      x.SetX(m_vScl.GetX());
      y.SetY(m_vScl.GetY());
      z.SetZ(m_vScl.GetZ());
      x.Mul(m_qRot);
      y.Mul(m_qRot);
      z.Mul(m_qRot);

      m_Array[0]  = x.GetX();
      m_Array[1]  = x.GetY();
      m_Array[2]  = x.GetZ();
      m_Array[3]  = 0.0f;
      m_Array[4]  = y.GetX();
      m_Array[5]  = y.GetY();
      m_Array[6]  = y.GetZ();
      m_Array[7]  = 0.0f;
      m_Array[8]  = z.GetX();
      m_Array[9]  = z.GetY();
      m_Array[10] = z.GetZ();
      m_Array[11] = 0.0f;
      m_Array[12] = m_vPos.GetX();
      m_Array[13] = m_vPos.GetY();
      m_Array[14] = m_vPos.GetZ();
      m_Array[15] = 1.0f;
   }
   return m_Array;
}

void snTransformf::SetArray(float * in_M)
{
   // first, let's copy the array!
   if(m_Array == NULL)
      m_Array = (float*)malloc(sizeof(float)*16);
   memcpy(m_Array,in_M,sizeof(float)*16);

   // use m as a shortcut only
   float * m = m_Array;
   {
      // extract the x,y,z axes
      snVector3f x,y,z;
      x.Set(m[0],m[1],m[2]);
      y.Set(m[4],m[5],m[6]);
      z.Set(m[8],m[9],m[10]);
      m_vScl.Set(x.GetLength(),y.GetLength(),z.GetLength());
   }
   {
      float qx,qy,qz,qw,qw4;
      float tr = m[0] + m[5] + m[10];

      if (tr > 0) {
         float S = sqrt(tr+1.0) * 2; // S=4*qw
         qw = 0.25 * S;
         qx = (m[9] - m[6]) / S;
         qy = (m[2] - m[8]) / S;
         qz = (m[4] - m[1]) / S;
      } else if ((m[0] > m[5])&(m[0] > m[10])) {
         float S = sqrt(1.0 + m[0] - m[5] - m[10]) * 2; // S=4*qx
         qw = (m[9] - m[6]) / S;
         qx = 0.25 * S;
         qy = (m[1] + m[4]) / S;
         qz = (m[2] + m[8]) / S;
      } else if (m[5] > m[10]) {
         float S = sqrt(1.0 + m[5] - m[0] - m[10]) * 2; // S=4*qy
         qw = (m[2] - m[8]) / S;
         qx = (m[1] + m[4]) / S;
         qy = 0.25 * S;
         qz = (m[6] + m[9]) / S;
      } else {
         float S = sqrt(1.0 + m[10] - m[0] - m[5]) * 2; // S=4*qz
         qw = (m[4] - m[1]) / S;
         qx = (m[2] + m[8]) / S;
         qy = (m[6] + m[9]) / S;
         qz = 0.25 * S;
      }

      // set the rotation!
      //*
      qw = sqrtf(1.0f + m[0] + m[5] + m[10]) * 0.5f;
      qw4 = 4.0f * qw;
      qx = (m[9] - m[6]) / qw4;
      qy = (m[2] - m[8]) / qw4;
      qz = (m[4] - m[1]) / qw4;
      //*/
      m_qRot.Set(qx,qy,qz,qw);
   }
   {
      // finally set the position!
      m_vPos.Set(m[12],m[13],m[14]);
   }
}



void snTransformf::DeallocateArray()
{
   if(m_Array == NULL)
   {
      free(m_Array);
      m_Array = NULL;
   }
}


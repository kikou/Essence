#ifndef __SNVECTOR3F__
#define __SNVECTOR3F__

#include "snQuaternionf.h"
#include <cstring>
#include <cstdlib>
#include <vector>

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

/**
 * snEssence is the shared namespace between all classes within the snEssence library.
 */
namespace snEssence
{
   // forward definitions
   class snQuaternionf;

   // constants
   const float PI = 3.14159265f;
   const float PIOVER180 = 3.14159265f / 180.0f;

   /**
    * A simple vector 3 (x,z,z) implementation using floats.
    */
   class snVector3f
   {
   private:
      float xyz[3];
   public:

      // constructors
      snVector3f();
      snVector3f(const snVector3f & in_Other);
      snVector3f(float x, float y, float z);

      // getters
      float GetX() const;
      float GetY() const;
      float GetZ() const ;
      float * GetArray();
      float GetLength() const;
      snVector3f GetNormalized() const;
      snVector3f GetNegated() const;
      snVector3f GetCrossed (const snVector3f & in_B) const;
      snVector3f GetAdded(const snVector3f & in_B) const;
      snVector3f GetSubstracted(const snVector3f & in_B) const;
      snVector3f GetMultiplied(const snVector3f & in_B) const;
      snVector3f GetMultiplied(const snQuaternionf & in_Q) const;
      snVector3f GetDivided(const snVector3f & in_B) const;
      snVector3f GetScaled(float in_Scale) const;
      snVector3f GetInterPolated(float in_Blend, const snVector3f & in_B);
      snVector3f GetInverted() const;
      float Dot(const snVector3f & in_B) const;
      snVector3f GetMin(const snVector3f & in_B) const;
      snVector3f GetMax(const snVector3f & in_B) const;
      float GetAngle(const snVector3f & in_B) const;
      float GetDistance(const snVector3f & in_B) const;

      // setters
      void Set(const snVector3f & in_Other);
      void SetX(float in_X);
      void SetY(float in_Y);
      void SetZ(float in_Z);
      void Set(float in_X, float in_Y, float in_Z);

      // 0 parameter
      bool Normalize();
      void Negate();
      void PutNull();
      bool Invert();

      // 1 parameter
      void Add(const snVector3f & in_B);
      void Sub(const snVector3f & in_B);
      void Mul(const snVector3f & in_B);
      void Mul(const snQuaternionf & in_Q);
      bool Div(const snVector3f & in_B);
      void Scale(float in_Scale);
      void Cross(const snVector3f & in_B);
      bool Normalize(const snVector3f & in_A);
      void Negate(const snVector3f & in_A);
      bool Invert(const snVector3f & in_A);
      void Min(const snVector3f & in_B);
      void Max(const snVector3f & in_B);

      // 2 parameter
      void Add(const snVector3f & in_A, const snVector3f & in_B);
      void Sub(const snVector3f & in_A, const snVector3f & in_B);
      void Mul(const snVector3f & in_A, const snVector3f & in_B);
      bool Div(const snVector3f & in_A, const snVector3f & in_B);
      void Scale(float in_Scale, const snVector3f & in_B);
      void Cross(const snVector3f & in_A, const snVector3f & in_B);
      void InterPolate(float in_Blend, const snVector3f & in_B);
      void Mul(const snVector3f & in_V, const snQuaternionf & in_Q);
      void Min(const snVector3f & in_A, const snVector3f & in_B);
      void Max(const snVector3f & in_A, const snVector3f & in_B);

      // 3 parameter
      void InterPolate(float in_Blend, const snVector3f & in_A, const snVector3f & in_B);

      // operators
      snVector3f operator +(const snVector3f & in_B) const { return GetAdded(in_B); }
      snVector3f operator -(const snVector3f & in_B) const { return GetSubstracted(in_B); }
      snVector3f operator *(const snVector3f & in_B) const { return GetMultiplied(in_B); }
      snVector3f operator *(const snQuaternionf & in_Q) const { return GetMultiplied(in_Q); }
      snVector3f operator /(const snVector3f & in_B) const { return GetDivided(in_B); }
      void operator +=(const snVector3f & in_B) { return Add(in_B); }
      void operator -=(const snVector3f & in_B) { return Sub(in_B); }
      void operator *=(const snVector3f & in_B) { return Mul(in_B); }
      void operator *=(const snQuaternionf & in_Q) { return Mul(in_Q); }
      bool operator /=(const snVector3f & in_B) { return Div(in_B); }
      bool operator ==(const snVector3f & in_B) const;
   };

   typedef snVector3f * snpVector3f;
   typedef std::vector<snVector3f> snVector3fVec;
   typedef std::vector<snVector3fVec> snVector3fVecVec;
   typedef std::vector<snpVector3f> snpVector3fVec;
}
#endif

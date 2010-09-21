#ifndef __SNVECTOR2F__
#define __SNVECTOR2F__

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
   /**
    * A simple vector 2 (x,z) implementation using floats.
    */
   class snVector2f
   {
   private:
      float xy[2];
   public:

      // constructors
      snVector2f();
      snVector2f(const snVector2f & in_Other);
      snVector2f(float x, float y);

      // getters
      float GetX() const;
      float GetY() const;
      float * GetArray();
      float GetLength() const;
      snVector2f GetNormalized() const;
      snVector2f GetNegated() const;
      snVector2f GetCrossed() const;
      snVector2f GetAdded(const snVector2f & in_B) const;
      snVector2f GetSubstracted(const snVector2f & in_B) const;
      snVector2f GetMultiplied(const snVector2f & in_B) const;
      snVector2f GetDivided(const snVector2f & in_B) const;
      snVector2f GetScaled(float in_Scale) const;
      snVector2f GetInverted() const;
      float Dot(const snVector2f & in_B) const;
      snVector2f GetMin(const snVector2f & in_B) const;
      snVector2f GetMax(const snVector2f & in_B) const;

      // setters
      void Set(const snVector2f & in_Other);
      void SetX(float in_X);
      void SetY(float in_Y);
      void Set(float in_X, float in_Y);

      // 0 parameter
      bool Normalize();
      void Negate();
      void PutNull();
      bool Invert();
      void Cross();

      // 1 parameter
      void Add(const snVector2f & in_B);
      void Sub(const snVector2f & in_B);
      void Mul(const snVector2f & in_B);
      bool Div(const snVector2f & in_B);
      void Scale(float in_Scale);
      void Cross(const snVector2f & in_B);
      bool Normalize(const snVector2f & in_A);
      void Negate(const snVector2f & in_A);
      bool Invert(const snVector2f & in_A);
      void Min(const snVector2f & in_B);
      void Max(const snVector2f & in_B);

      // 2 parameter
      void Add(const snVector2f & in_A, const snVector2f & in_B);
      void Sub(const snVector2f & in_A, const snVector2f & in_B);
      void Mul(const snVector2f & in_A, const snVector2f & in_B);
      bool Div(const snVector2f & in_A, const snVector2f & in_B);
      void Scale(float in_Scale, const snVector2f & in_B);
      void InterPolate(float in_Blend, const snVector2f & in_B);
      void Min(const snVector2f & in_A, const snVector2f & in_B);
      void Max(const snVector2f & in_A, const snVector2f & in_B);

      // 3 parameter
      void InterPolate(float in_Blend, const snVector2f & in_A, const snVector2f & in_B);

      // operators
      snVector2f operator +(const snVector2f & in_B) const { return GetAdded(in_B); }
      snVector2f operator -(const snVector2f & in_B) const { return GetSubstracted(in_B); }
      snVector2f operator *(const snVector2f & in_B) const { return GetMultiplied(in_B); }
      snVector2f operator /(const snVector2f & in_B) const { return GetDivided(in_B); }
      void operator +=(const snVector2f & in_B) { return Add(in_B); }
      void operator -=(const snVector2f & in_B) { return Sub(in_B); }
      void operator *=(const snVector2f & in_B) { return Mul(in_B); }
      bool operator /=(const snVector2f & in_B) { return Div(in_B); }
   };

   typedef snVector2f * snpVector2f;
   typedef std::vector<snVector2f> snVector2fVec;
   typedef std::vector<snpVector2f> snpVector2fVec;
}
#endif

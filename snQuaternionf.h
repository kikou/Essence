#ifndef __SNQUATERNIONF__
#define __SNQUATERNIONF__

#include "snVector3f.h"
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
   class snVector3f;

   /**
    * A class representing xyz-w Quaternion using floats.
    */
   class snQuaternionf
   {
   private:
      float xyzw[4];

   public:

      // constructors
      snQuaternionf();
      snQuaternionf(const snQuaternionf & in_Other);
      snQuaternionf(float x, float y, float z, float w);

      // getters
      float GetX() const;
      float GetY() const;
      float GetZ() const ;
      float GetW() const ;
      float * GetArray();
      snVector3f GetAxis();
      float GetAngle();
      snQuaternionf GetNormalized() const;
      snQuaternionf GetConjugate() const;
      snQuaternionf GetMultiplied(const snQuaternionf & in_B) const;

      // setters
      void Set(const snQuaternionf & in_Other);
      void SetX(float in_X);
      void SetY(float in_Y);
      void SetZ(float in_Z);
      void SetW(float in_W);
      void Set(float in_X, float in_Y, float in_Z, float in_W);

      // conversion
      void SetAxisAngle(const snVector3f & in_Axis, float in_Angle);
      void SetEuler(float pitch, float yaw, float roll);

      // 0 parameter
      bool Normalize();
      void Conjugate();

      // 1 parameter
      void Mul(const snQuaternionf & in_B);
      bool Normalize(const snQuaternionf & in_A);
      void Conjugate(const snQuaternionf & in_A);

      // 2 parameter
      void Mul(const snQuaternionf & in_A, const snQuaternionf & in_B);

      // operators
      snQuaternionf operator *(const snQuaternionf & in_B) const { return GetMultiplied(in_B); }
      void operator *=(const snQuaternionf & in_B) { return Mul(in_B); }
   };

   typedef snQuaternionf * snpQuaternionf;
   typedef std::vector<snQuaternionf> snQuaternionfVec;
   typedef std::vector<snpQuaternionf> snpQuaternionfVec;
}
#endif

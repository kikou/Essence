#ifndef __SNTRANSFORMF__
#define __SNTRANSFORMF__

#include "snVector3f.h"
#include "snQuaternionf.h"
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
    * A 4x4 matrix represented as a scale, orientation, position ordered transform.
    */
   class snTransformf
   {
   private:
      snVector3f m_vPos;
      snQuaternionf m_qRot;
      snVector3f m_vScl;
      float * m_Array;
      void DeallocateArray();

   public:

      // constructors
      snTransformf();
      snTransformf(const snTransformf & in_Other);
      snTransformf(const snVector3f & in_vPos, const snQuaternionf & in_qRot, const snVector3f & in_vScl);
      ~snTransformf();

      // getters
      snVector3f GetPosition() const { return m_vPos; }
      snQuaternionf GetRotation() const { return m_qRot; }
      snVector3f GetScaling() const { return m_vScl; }
      float * GetArray();
      snVector3f * GetPositionPtr() { return &m_vPos; }
      snQuaternionf * GetRotationPtr() { return &m_qRot; }
      snVector3f * GetScalingPtr() { return &m_vScl; }
      snTransformf GetMultiplied( const snTransformf & in_B ) const;

      // setters
      void SetPosition( const snVector3f & in_vPos ) {    DeallocateArray(); m_vPos = in_vPos; }
      void SetRotation( const snQuaternionf& in_qRot ) { DeallocateArray();  m_qRot = in_qRot; }
      void SetScaling( const snVector3f & in_vScl ) { DeallocateArray(); m_vScl = in_vScl; }
      void Set(const snTransformf & in_Other);
      void SetArray(float * in_M);

      // 0 parameter methods
      bool Invert();

      // 1 parameter methods
      void Mul( const snTransformf & in_B );

      // 2 parameter methods
      void Mul( const snTransformf & in_A, const snTransformf & in_B );
   };

   typedef snTransformf snMatrix44f;
   typedef snTransformf * snpMatrix44fpTransformf;
   typedef std::vector<snTransformf> snTransformfVec;
   typedef std::vector<snpMatrix44fpTransformf> snpMatrix44fpTransformfVec;
   typedef snMatrix44f * snpMatrix44f;
   typedef std::vector<snMatrix44f> snMatrix44ffVec;
   typedef std::vector<snpMatrix44f> snpMatrix44ffVec;
}
#endif

#ifndef __SNPLANE__
#define __SNPLANE__

#include "snVector3f.h"
#include "snLine.h"
#include "snSegment.h"
#include "snRay.h"
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
   // forwards
   class snLine;

   /**
    * A 3d plane represented by origin and normal
    */
   class snPlane
   {
   private:
      snVector3f mOrigin;
      snVector3f mNormal;
      size_t mPointID1;
      size_t mPointID2;

   public:

      // constructors
      snPlane();
      snPlane(const snVector3f & in_Origin, const snVector3f & in_Normal, bool in_GivenTwoPoints = false);

      // getters
      snVector3f GetOrigin() const { return mOrigin; }
      snVector3f GetNormal() const { return mNormal; }
      size_t GetPointID1() const { return mPointID1; }
      size_t GetPointID2() const { return mPointID2; }

      // setters
      void Set(const snVector3f & in_Origin, const snVector3f & in_Normal);
      bool SetFromTwoPoints(const snVector3f & in_PointA, const snVector3f & in_PointB);
      void SetPointID1(size_t in_ID) { mPointID1 = in_ID; return ;}
      void SetPointID2(size_t in_ID) { mPointID2 = in_ID; return ;}

      // intersections
      float GetDistance(const snVector3f & in_Point) const;
      bool IsAbove(const snVector3f & in_Point) const { return GetDistance(in_Point) >= 0; }
      bool IsBelow(const snVector3f & in_Point) const { return !IsAbove(in_Point); }
      bool GetIntersectionWithSegment(const snSegment & in_Segment, snVector3f * out_pResult = NULL) const;
      bool GetIntersectionWithLine(const snLine & in_Line, snVector3f * out_pResult = NULL) const;
      bool GetIntersectionWithRay(const snRay & in_Ray, snVector3f * out_pResult = NULL) const;
      bool GetIntersectionWithPlane(const snPlane & in_Plane, snLine * out_pResult = NULL) const;
   };

   typedef snPlane * snpPlane;
   typedef std::vector<snPlane> snPlaneVec;
   typedef std::vector<snpPlane> snpPlaneVec;
}
#endif

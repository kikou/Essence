#ifndef __SNTRIANGLE_
#define __SNTRIANGLE_

#include "snPolygon.h"

#define UNUSED_ARGUMENT(x) (void)x

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
   class snTriangleMesh;

   /**
    * A 3d triangle
    */
   class snTriangle : public snPolygon
   {
   public:

      /// A struct representing a 2-dimensional uv coordinate
      struct uvCoord
      {
         float u;
         float v;

         uvCoord(float in_U = 0,float in_V = 0)
         {
            u = in_U;
            v = in_V;
         }

         bool IsOnTriangle()
         {
            return (u >= 0.0f) && (v >= 0.0f) && (u + v <= 1.0f);
         }
         bool IsInsideTriangle()
         {
            return (u > 0.0f) && (v > 0.0f) && (u + v < 1.0f);
         }
      };

      /// A struct storing all of the information for a ray-triangle intersection.
      struct tri2ray
      {
         snVector3f pnt;
         uvCoord uv;
      };
      /// A struct storing all of the information for a triangle-triangle intersection.
      struct tri2tri
      {
         snIndex tri0Index;
         snIndex tri1Index;

         snIndexVec edge0;
         snIndexVec edge1;
         snVector3fVec edgePos0;
         snVector3fVec edgePos1;
      };
      // constructors
      snTriangle(snIndex index1, snIndex index2, snIndex index3, snTriangleMesh * in_pTriangleMesh);
      snTriangle(snTriangleMesh * in_pTriangleMesh);
      virtual ~snTriangle();
      virtual void Copy(snTriangle * in_pTriangle);

      // getters
      virtual snIndex GetNbPoints() const { return 3; }
      virtual snIndex GetNbSegments() const { return 3; }
      virtual bool IsValid() const { return true; }
      virtual bool IsTriangle() const { return true; }
      virtual bool IsQuadrangle() const { return false; }
      virtual snVector3f GetCenter() const;
      virtual snIndex GetPointIndex0() const { return mIndices[0]; }
      virtual snIndex GetPointIndex1() const { return mIndices[1]; }
      virtual snIndex GetPointIndex2() const { return mIndices[2]; }
      virtual snVector3f GetPoint0() const { return GetPoint(GetPointIndex0()); }
      virtual snVector3f GetPoint1() const { return GetPoint(GetPointIndex1()); }
      virtual snVector3f GetPoint2() const { return GetPoint(GetPointIndex2()); }
      virtual snIndexTuple GetSegmentTuple(snIndex in_Index);

      // overdrive the addpoint methods, we don't want to allow to add points!
      virtual void AddPoint(const snVector3f & in_Point) { UNUSED_ARGUMENT(in_Point); return; }
      virtual void AddPointByIndex(snIndex in_Index) { UNUSED_ARGUMENT(in_Index); return; }

      // order operations
      virtual void Invert();

      // intersections
      virtual bool GetEdgePlane(snIndex in_Index, snPlane * out_pResult);
      virtual uvCoord GetUVFromPoint(const snVector3f & in_Point);
      virtual snVector3f GetPointFromUV(const uvCoord & in_UV);
      virtual bool IsPointOnTriangle(const snVector3f & in_Point);
      virtual bool IsPointInsideTriangle(const snVector3f & in_Point);
      virtual bool GetIntersectionWithBBox(const snBboxf & in_Bbox);
      virtual bool GetIntersectionWithLine(const snLine & in_Line, tri2ray * out_pResult = NULL);
      virtual bool GetIntersectionWithRay(const snRay & in_Ray, tri2ray * out_pResult = NULL);
      virtual bool GetIntersectionWithSegment(const snSegment & in_Segment, tri2ray * out_pResult = NULL);
      virtual bool GetIntersectionWithPlane(const snPlane & in_Plane, snSegment * out_pResult = NULL);
      virtual bool GetIntersectionWithTriangle(snTriangle * in_pTriangle, tri2tri * out_pResult = NULL, bool in_bSkipSecondaryCases = false);
      static int GetIntersectionWithUVPair(const uvCoord & in_UVInside,const uvCoord & in_UVOutside);

   protected:
      virtual void UpdatePlane();
   };

   typedef snTriangle * snpTriangle;
   typedef std::vector<snTriangle> snTriangleVec;
   typedef std::vector<snpTriangle> snpTriangleVec;
}

#include "snTriangleMesh.h"

#endif

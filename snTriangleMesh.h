#ifndef __SNTRIANGLEMESH__
#define __SNTRIANGLEMESH__

#include "snMesh.h"
#include "snTriangle.h"
#include "snString.h"

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
    * A 3d polygon-mesh consisting only of triangles
    */
   class snTriangleMesh : public snMesh
   {
   protected:
      snpTriangleVec mTriangles;

      // boolean lookup data
      bool mBoolUpToDate;
      snBboxf mBBox;

   public:
      /// A struct storing all of the information for a triangleMesh-ray intersection.
      struct mesh2ray
      {
         snIndex index;
         snTriangle::tri2ray intersection;
      };

      /// A struct storing all of the information for a triangleMesh-line intersection.
      struct mesh2line
      {
         snIndexVec index;
         std::vector<snTriangle::tri2ray> intersection;
      };

      /// A struct storing all of the information for a triangleMesh-triangleMesh intersection.
      struct mesh2mesh
      {
         std::vector<snTriangle::tri2tri> intersection;
      };

      /// A struct storing a single contour cut
      struct meshContourPnt
      {
         bool edgeOnFirstMesh;
         int edge;
         int triangle;
         int pnt;

         meshContourPnt(bool in_EdgeOnFirstMesh, int in_Edge, int in_Triangle, int in_Pnt)
         {
            edgeOnFirstMesh = in_EdgeOnFirstMesh;
            edge = in_Edge;
            triangle = in_Triangle;
            pnt = in_Pnt;
         }
      };
      typedef std::vector<meshContourPnt> meshContour;
      typedef std::vector<meshContour> meshContourVec;
      // constructors
      snTriangleMesh();
      virtual ~snTriangleMesh();
      virtual void Clear();
      virtual void Copy(snTriangleMesh * in_pTriangleMesh);
      // getters
      virtual snIndex GetNbTriangles() const { return mTriangles.size(); }
      virtual snIndex GetNbPolygons() const { return GetNbTriangles(); }
      virtual snTriangle * GetTriangle(snIndex in_Index);
      virtual snPolygon * GetPolygon(snIndex in_Index) { return (snPolygon*)GetTriangle(in_Index); }
      virtual void DeleteTriangle(snIndex in_Index);
      virtual void DeletePolygon(snIndex in_Index) { return DeleteTriangle(in_Index); }
      virtual void Merge(snTriangleMesh * in_Mesh, bool in_InvertTriangles = false);
      virtual snIndex RemoveUnusedPoints();
      virtual void RefreshTopoLookupsPerPolygon(snIndex in_PolygonIndex);

      // topology
      virtual snPolygon *  AddPolygon(snIndexVec in_Indices = snIndexVec(),bool in_UpdateLookups = true) { UNUSED_ARGUMENT(in_Indices);UNUSED_ARGUMENT(in_UpdateLookups);return NULL; }
      virtual snTriangle *  AddTriangle(snIndex index1, snIndex index2, snIndex index3 ,bool in_UpdateLookups = true);
      virtual bool GetOutlines(snIndexVecVec * out_pResult);

      // boolean lookups
      virtual snBboxf GetBbox();
      virtual void RefreshBoolLookups();
      virtual void InvalidateBool() { mBoolUpToDate = false; }

      // primitive intersection
      virtual bool IsPointInsideClosed(const snVector3f & in_Point);
      virtual bool IsPointInsideClosedConvex(const snVector3f & in_Point, mesh2ray * out_pResult = NULL);
      virtual bool GetIntersectionWithRay(const snRay & in_Ray, mesh2ray * out_pResult = NULL);
      virtual bool GetClosestIntersectionWithRay(const snRay & in_Ray, mesh2ray * out_pResult);
      virtual bool GetIntersectionsWithRay(const snRay & in_Ray, mesh2line * out_pResult);
      virtual bool GetIntersectionsWithLine(const snLine & in_Line, mesh2line * out_pResult = NULL);
      virtual bool GetIntersectionsWithSegment(const snSegment & in_Segment,  mesh2line * out_pResult = NULL);

      // mesh intersection
      virtual bool GetMeshBelowPlane(const snPlane & in_Plane, snTriangleMesh * out_pResult);
   };

   typedef snTriangleMesh * snpTriangleMesh;
   typedef std::vector<snTriangleMesh> snTriangleMeshVec;
   typedef std::vector<snpTriangleMesh> snpTriangleMeshVec;
}

#endif

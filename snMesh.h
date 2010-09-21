#ifndef __SNMESH__
#define __SNMESH__

#include "snSegment.h"
#include "snPolygon.h"
#include <vector>
#include <map>

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
    * A 3d polygon-mesh
    */
   class snMesh
   {
   protected:
      snVector3fVec mPoints;
      snpPolygonVec mPolygons;

      // topo lookup data
      bool mTopoUpToDate;
      snIndexTupleMap mLU_PointIndices2SegmentIndex;
      snIndexVecVec mLU_PointIndexToPointIndices;
      snIndexVecVec mLU_PointIndexToSegmentIndices;
      snIndexVecVec mLU_PointIndexToPolygonIndices;
      snIndexVecVec mLU_SegmentIndexToPolygonIndices;
      snIndexVecVec mLU_SegmentIndexToPointIndices;
      snIndexVec mLU_SegmentIndexToInverted;
      snIndexVecVec mLU_PolygonIndexToPointIndices;
      snIndexVecVec mLU_PolygonIndexToSegmentIndices;
      snIndex mLU_NbSegments;
      snIndex mLU_NbSamples;

   public:

      // constructors
      snMesh();
      virtual ~snMesh();
      virtual void Clear();

      // getters
      virtual snIndex GetNbPoints() const { return mPoints.size(); }
      virtual snIndex GetNbPolygons() const { return mPolygons.size(); }
      virtual snVector3fVec GetPoints() const { return mPoints; }
      virtual snIndexVec GetNbPointsPerPolygon();
      virtual snIndexVec GetPointIndices();
      virtual snIndexVec GetPointIndicesCombined();
      virtual snVector3f GetPoint(snIndex in_Index) const;
      virtual snVector3f * GetPointPtr(snIndex in_Index);
      virtual snVector3fVec * GetPointsPtr() { return &mPoints; }
      virtual snPolygon * GetPolygon(snIndex in_Index);
      virtual snBboxf GetBbox();

      // setters
      virtual void SetPoint(const snVector3f & in_Point, snIndex in_Index);
      virtual void AddPoint(const snVector3f & in_Point);
      virtual snPolygon *  AddPolygon(snIndexVec in_Indices = snIndexVec(),bool in_UpdateLookups = true);
      virtual snVector3fVec * ResizePoints(snIndex in_Size);

      // lookups
      virtual void RefreshTopoLookups();
      virtual void RefreshTopoLookupsPerPolygon(snIndex in_PolygonIndex);
      virtual void InvalidateTopo() { mTopoUpToDate = false; }
      virtual snIndex GetNbSegments() { RefreshTopoLookups(); return mLU_NbSegments; }
      virtual snIndex GetNbSamples() { RefreshTopoLookups(); return mLU_NbSamples; }
      virtual snIndex GetSegmentIndexFromTuple(snIndexTuple in_Tuple);
      virtual snIndexTuple GetSegmentTupleFromIndex(snIndex in_SegmentIndex);
      virtual snIndexTupleMap * GetSegmentMap();
      virtual snSegment GetSegment(snIndex in_Index);
      virtual snSegment GetSegmentFromTuple(snIndexTuple in_Tuple);
      virtual snIndexVec * GetPointsFromPoint(snIndex in_PointIndex);
      virtual snIndexVec * GetPointsFromSegment(snIndex in_SegmentIndex);
      virtual snIndexVec * GetPointsFromPolygon(snIndex in_PolygonIndex);
      virtual snIndexVec * GetSegmentsFromPoint(snIndex in_PointIndex);
      virtual snIndexVec * GetSegmentsFromPolygon(snIndex in_PolygonIndex);
      virtual bool         GetSegmentIsInverted(snIndex in_SegmentIndex);
      virtual snIndexVec * GetPolygonsFromPoint(snIndex in_PointIndex);
      virtual snIndexVec * GetPolygonsFromSegment(snIndex in_SegmentIndex);

      // boundary calcs
      virtual bool IsPointBoundary(snIndex in_PointIndex);
      virtual bool IsSegmentBoundary(snIndex in_SegmentIndex);
      virtual bool IsPolygonBoundary(snIndex in_PolygonIndex);

      // polygon operators
      virtual void Merge(snMesh * in_Mesh);
      virtual snIndex RemoveUnusedPoints();
      virtual void DeletePolygon(snIndex in_Index);
   };

   typedef snMesh * snpMesh;
   typedef std::vector<snMesh> snMeshVec;
   typedef std::vector<snpMesh> snpMeshVec;
}

#endif

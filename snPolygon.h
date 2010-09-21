#ifndef __SNPOLYGON_
#define __SNPOLYGON_

#include "snVector3f.h"
#include "snBboxf.h"
#include "snSegment.h"
#include <vector>
#include "snPlane.h"

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
   class snSegment;
   class snPlane;
   class snMesh;
   struct snIndexTuple;

   /**
    * A 3d polygon represented by at least three points
    */
   class snPolygon
   {
   protected:
      snVector3fVec * mpPoints;
      snIndexVec mIndices;
      snPlane mPlane;
      snMesh * mMesh;
      bool mPlaneUptoDate;
      int mIndex;

   public:

      // constructors
      snPolygon(snMesh * in_pMesh);
      virtual ~snPolygon();

      // getters
      virtual snIndex GetNbPoints() const;
      virtual snIndex GetNbSegments() const { return mIndices.size(); }
      virtual bool IsValid() const { return GetNbPoints() >= 3; }
      virtual bool IsTriangle() const { return GetNbPoints() == 3; }
      virtual bool IsQuadrangle() const { return GetNbPoints() == 3; }
      virtual snVector3f GetPoint(snIndex in_Index) const;
      virtual snpVector3f GetPointPtr(snIndex in_Index);
      virtual snIndexVec GetPointIndices() const { return mIndices; }
      virtual snIndexVec * GetPointIndicesPtr() { return &mIndices; }
      virtual snIndex GetPointIndex(snIndex in_Index) const;
      virtual snBboxf GetBbox() const;
      virtual snVector3f GetCenter() const;
      virtual snPlane GetPlane() { UpdatePlane(); return mPlane; }
      virtual snPlane * GetPlanePtr() { UpdatePlane(); return &mPlane; }
      virtual void InvalidatePlane();
      virtual snSegment GetSegment(snIndex in_Index);
      virtual snIndexTuple GetSegmentTuple(snIndex in_Index);
      virtual snIndex GetSegmentIndex(snIndex in_Index);
      virtual snMesh * GetMesh() { return mMesh; }

      // setters
      virtual void SetPoint(const snVector3f & in_Point, snIndex in_Index);
      virtual void AddPoint(const snVector3f & in_Point);
      virtual void AddPointByIndex(snIndex in_Index);

      // lookups
      virtual snIndex GetIndex() const { return (snIndex)mIndex; }

   protected:
      virtual void UpdatePlane();
   };

   typedef snPolygon * snpPolygon;
   typedef std::vector<snPolygon> snPolygonVec;
   typedef std::vector<snpPolygon> snpPolygonVec;
}

#include "snMesh.h"
#include "snSegment.h"

#endif

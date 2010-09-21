#ifndef __SNSEGMENT_
#define __SNSEGMENT_

#include "snVector3f.h"
#include "snBboxf.h"
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
   // forwards
   class snPlane;
   class snMesh;

   typedef size_t snIndex;
   typedef std::vector<snIndex> snIndexVec;
   typedef std::map<snIndex,snIndex> snIndexMap;
   typedef std::pair<snIndex,snIndex> snIndexPair;
   typedef std::vector<snIndexVec> snIndexVecVec;

   /**
    * A struct representing two indices
    */
   struct snIndexTuple
   {
      snIndex a;
      snIndex b;

      snIndexTuple()
      {
         a = b = 0;
      }

      snIndexTuple(snIndex in_a, snIndex in_b, bool in_sort = false)
      {
         if(in_sort)
            SetUnsorted(in_a,in_b);
         else
            SetSorted(in_a,in_b);
      }

      snIndexTuple(const snIndexTuple & in_t)
      {
         a = in_t.a;
         b = in_t.b;
      }

      void SetSorted(snIndex in_a, snIndex in_b)
      {
         a = in_a;
         b = in_b;
      }

      void SetUnsorted(snIndex in_a, snIndex in_b)
      {
         if(in_a > in_b)
         {
            b = in_a;
            a = in_b;
         }
         else
         {
            a = in_a;
            b = in_b;
         }
      }

      snIndexTuple GetInverted()
      {
         return snIndexTuple(b,a);
      }

      bool operator < (const snIndexTuple & in_t) const
      {
         if(a < in_t.a)
            return true;
         else if(a > in_t.a)
            return false;
         return b < in_t.b;
      }
   };

   typedef std::map<snIndexTuple,snIndex> snIndexTupleMap;
   typedef std::pair<snIndexTuple,snIndex> snIndexTuplePair;

   /**
    * A 3d segment represented by start and end point
    */
   class snSegment
   {
   protected:
      snVector3f mStart;
      snVector3f mEnd;
      snIndex mPointID1;
      snIndex mPointID2;
      int mIndex;
      snMesh * mMesh;
      bool mInverse;

   public:

      // constructors
      snSegment();
      snSegment(const snVector3f & in_Start, const snVector3f & in_End);
      virtual ~snSegment() {}

      // getters
      snVector3f GetStart() const { return mStart; }
      snVector3f GetEnd() const { return mEnd; }
      snIndex GetPointID1() const { return mPointID1; }
      snIndex GetPointID2() const { return mPointID2; }
      snIndexTuple GetIndexTuple() const;
      virtual snVector3f GetConnection() const;
      virtual snVector3f GetCenter() const;
      virtual float GetLength() const { return GetConnection().GetLength(); }
      snBboxf GetBbox() const;
      bool GetIsInverse() const { return mInverse; }

      // setters
      void SetMesh(snMesh * in_pMesh) { mMesh = in_pMesh; }
      virtual void Set(const snVector3f & in_Start, const snVector3f & in_End);
      void SetPointID1(snIndex in_ID) { mPointID1 = in_ID; return ;}
      void SetPointID2(snIndex in_ID) { mPointID2 = in_ID; return ;}
      void SetIsInverse(bool in_Inverse) { mInverse = in_Inverse; }
      void SetIndex(snIndex in_Index) { mIndex = (int)in_Index; }

      // lookups
      snIndex GetIndex();
      snIndexVec * GetPoints();
      snIndexVec * GetPolygons();

      // intersection
      virtual bool GetIntersectionWithBBox(const snBboxf & in_Bbox, snVector3fVec * out_pResult = NULL) const;
   };

   typedef snSegment * snpSegment;
   typedef std::vector<snSegment> snSegmentVec;
   typedef std::vector<snpSegment> snpSegmentVec;
}

//#include "snPlane.h"

#endif

#include "snSegment.h"

#include "snMesh.h"

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


using namespace snEssence;

snSegment::snSegment()
{
   mStart.Set(0,0,0);
   mEnd.Set(0,0,0);
   mPointID1 = mPointID2 = 0;
   mInverse = false;
   mIndex = -1;
}


snSegment::snSegment(const snVector3f & in_Start, const snVector3f & in_End)
{
   Set(in_Start,in_End);
   mPointID1 = mPointID2 = 0;
   mIndex = -1;
}


void snSegment::Set(const snVector3f & in_Start, const snVector3f & in_End)
{
   mStart = in_Start;
   mEnd = in_End;
}

snBboxf snSegment::GetBbox() const
{
   snBboxf box;
   box.Merge(mStart);
   box.Merge(mEnd);
   return box;
}



snVector3f snSegment::GetConnection() const
{
   snVector3f con;
   con.Sub(mEnd,mStart);
   return con;
}


snVector3f snSegment::GetCenter() const
{
   snVector3f center;
   center.InterPolate(.5f,mEnd,mStart);
   return center;
}


snIndexTuple snSegment::GetIndexTuple() const
{
   return mPointID1 > mPointID2 ? snIndexTuple(mPointID2,mPointID1) : snIndexTuple(mPointID1,mPointID2);
}


snIndex snSegment::GetIndex()
{
   if(mIndex == -1)
   {
      if(mMesh==NULL)
         return 0;
      mIndex = (int)mMesh->GetSegmentIndexFromTuple(GetIndexTuple());
   }
   return (snIndex)mIndex;
}

snIndexVec * snSegment::GetPoints()
{
   if(mMesh==NULL)
      return NULL;

   return mMesh->GetPointsFromSegment(GetIndex());
}

snIndexVec * snSegment::GetPolygons()
{
   if(mMesh==NULL)
      return NULL;

   return mMesh->GetPolygonsFromSegment(GetIndex());
}

bool snSegment::GetIntersectionWithBBox(const snBboxf & in_Bbox, snVector3fVec * out_pResult) const
{
   // if both points are inside, the segment is intersecting
   if(in_Bbox.IsPointInside(mStart) && in_Bbox.IsPointInside(mEnd))
      return true;

   // first check if both points are on one side of the bbox
   snVector3f pMin = in_Bbox.GetMin();
   snVector3f pMax = in_Bbox.GetMax();
   if(
         ((mStart.GetX() < pMin.GetX()) && (mEnd.GetX() < pMin.GetX())) ||
         ((mStart.GetY() < pMin.GetY()) && (mEnd.GetY() < pMin.GetY())) ||
         ((mStart.GetZ() < pMin.GetZ()) && (mEnd.GetZ() < pMin.GetZ())) ||
         ((mStart.GetX() > pMax.GetX()) && (mEnd.GetX() > pMax.GetX())) ||
         ((mStart.GetY() > pMax.GetY()) && (mEnd.GetY() > pMax.GetY())) ||
         ((mStart.GetZ() > pMax.GetZ()) && (mEnd.GetZ() > pMax.GetZ())))
      return false;

   // localize the vectors
   snVector3f width,localStart,localEnd;
   width.Sub(pMax,pMin);
   width.Invert();
   localStart.Sub(mStart,pMin);
   localStart.Mul(width);
   localEnd.Sub(mEnd,pMin);
   localEnd.Mul(width);
   snSegment local(localStart,localEnd);

   // create six planes
   snPlane plane[6];
   plane[0].Set(snVector3f(0,0,0),snVector3f(-1,0,0));
   plane[1].Set(snVector3f(0,0,0),snVector3f(0,-1,0));
   plane[2].Set(snVector3f(0,0,0),snVector3f(0,0,-1));
   plane[3].Set(snVector3f(1,1,1),snVector3f(1,0,0));
   plane[4].Set(snVector3f(1,1,1),snVector3f(0,1,0));
   plane[5].Set(snVector3f(1,1,1),snVector3f(0,0,1));

   // clear the output vector
   if(out_pResult != NULL)
      out_pResult->clear();

   // collect all valid intersections
   snVector3f intersection;
   bool result = false;
   for(snIndex i=0;i<6;i++)
   {
      if(plane[i].GetIntersectionWithSegment(local,&intersection))
      {
         if(intersection.GetX() >= 0.0f &&
            intersection.GetX() <= 1.0f &&
            intersection.GetY() >= 0.0f &&
            intersection.GetY() <= 1.0f &&
            intersection.GetZ() >= 0.0f &&
            intersection.GetZ() <= 1.0f)
         {
            result = true;
            if(out_pResult != NULL)
                  out_pResult->push_back(intersection);
         }
      }
   }

   return result;
}


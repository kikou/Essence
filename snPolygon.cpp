#include "snPolygon.h"

#include "snSegment.h"

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

snPolygon::snPolygon(snMesh * in_pMesh)
{
   // empty the sets
   mIndices.clear();

   // assign the mesh!
   mMesh = in_pMesh;
   mpPoints = mMesh->GetPointsPtr();
   mIndex = (int)mMesh->GetNbPolygons();

   mPlaneUptoDate = false;
}

snPolygon::~snPolygon()
{
}


snIndex snPolygon::GetNbPoints() const
{
   return mIndices.size();
}


snVector3f snPolygon::GetPoint(snIndex in_Index) const
{
   if(in_Index<mpPoints->size())
      return mpPoints->at(in_Index);
   return snVector3f();
}

snpVector3f snPolygon::GetPointPtr(snIndex in_Index)
{
   if(in_Index<mpPoints->size())
      return &mpPoints->at(in_Index);
   return NULL;
}

snIndex snPolygon::GetPointIndex(snIndex in_Index) const
{
   if(in_Index<mIndices.size())
      return mIndices[in_Index];
   return 0;
}

snBboxf snPolygon::GetBbox() const

{
   snBboxf box;
   for(snIndex i=0;i<mIndices.size();i++)
      box.Merge(mpPoints->at(mIndices[i]));
   return box;
}

snVector3f snPolygon::GetCenter() const
{
   snVector3f center;
   for(snIndex i=0;i<mIndices.size();i++)
      center.Add(mpPoints->at(mIndices[i]));
   center.Scale(1.0f / float(mIndices.size()));
   return center;
}

void snPolygon::SetPoint(const snVector3f & in_Point, snIndex in_Index)
{
   if(in_Index<mpPoints->size())
      (*mpPoints)[in_Index] = in_Point;
}

void snPolygon::AddPoint(const snVector3f & in_Point)
{
   mIndices.push_back(mpPoints->size());
   mpPoints->push_back(in_Point);
   if(mMesh!=NULL)
      mMesh->InvalidateTopo();
   mPlaneUptoDate = false;
}


void snPolygon::AddPointByIndex(snIndex in_Index)
{
   if(in_Index < mpPoints->size())
      mIndices.push_back(in_Index);
   if(mMesh!=NULL)
      mMesh->InvalidateTopo();
   mPlaneUptoDate = false;
}


void snPolygon::UpdatePlane()
{
   if(mPlaneUptoDate)
      return;

   if(GetNbPoints()<3)
      return;

   // get normal of the triangle
   snVector3f edge1,edge2,normal;
   snIndex k=0;
   while(k<GetNbPoints() && normal.GetX() == 0 && normal.GetY() == 0 && normal.GetZ() == 0)
   {
      edge1.Sub(GetPoint(GetPointIndex((k+1)%GetNbPoints())),GetPoint(GetPointIndex(k)));
      edge2.Sub(GetPoint(GetPointIndex((k+2)%GetNbPoints())),GetPoint(GetPointIndex(k)));
      normal.Cross(edge1,edge2);
      normal.Normalize();
      k++;
   }

   mPlane.Set(GetPoint(GetPointIndex(0)),normal);
   mPlaneUptoDate = true;
}

snSegment snPolygon::GetSegment(snIndex in_Index)
{
   snSegment seg;
   if(in_Index >= GetNbSegments())
      return seg;

   seg.SetMesh(mMesh);
   snIndexTuple tuple = GetSegmentTuple(in_Index);

   seg.Set(mpPoints->at(tuple.a),mpPoints->at(tuple.b));
   seg.SetPointID1(tuple.a);
   seg.SetPointID2(tuple.b);
   if(mMesh != NULL)
      seg.SetIndex(mMesh->GetSegmentIndexFromTuple(tuple));

   // for this polygon let me know if the segment is in sorted order
   seg.SetIsInverse(GetPointIndex(in_Index) != tuple.a);
   return seg;
}

snIndexTuple snPolygon::GetSegmentTuple(snIndex in_Index)
{
   snIndexTuple tuple;
   tuple.SetUnsorted(GetPointIndex(in_Index),GetPointIndex((in_Index+1) % mIndices.size()));
   return tuple;
}

snIndex snPolygon::GetSegmentIndex(snIndex in_Index)
{
   if(mMesh==NULL)
      return in_Index;
   return mMesh->GetSegmentIndexFromTuple(GetSegmentTuple(in_Index));
}

void snPolygon::InvalidatePlane()
{
   mPlaneUptoDate = false;
}

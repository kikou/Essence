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

snMesh::snMesh()
{
   mTopoUpToDate = false;
}

snMesh::~snMesh()
{
   Clear();
}

void snMesh::Clear()
{
   mPoints.clear();
   for(snIndex i=0;i<mPolygons.size();i++)
      delete(mPolygons[i]);
   mPolygons.clear();

   // topo lookups
   mTopoUpToDate = false;
   mLU_PointIndices2SegmentIndex.clear();
   mLU_PointIndexToPointIndices.clear();
   mLU_PointIndexToSegmentIndices.clear();
   mLU_PointIndexToPolygonIndices.clear();
   mLU_SegmentIndexToPolygonIndices.clear();
   mLU_SegmentIndexToPointIndices.clear();
   mLU_SegmentIndexToInverted.clear();
   mLU_PolygonIndexToPointIndices.clear();
   mLU_PolygonIndexToSegmentIndices.clear();
   mLU_NbSegments = 0;
   mLU_NbSamples = 0;
}


snIndexVec snMesh::GetNbPointsPerPolygon()
{
   snIndexVec indices;
   indices.resize(GetNbPolygons());
   for(snIndex i=0;i<GetNbPolygons();i++)
      indices[i] = GetPolygon(i)->GetNbPoints();
   return indices;
}

snIndexVec snMesh::GetPointIndices()
{
   snIndexVec indices;
   for(snIndex i=0;i<GetNbPolygons();i++)
   {
      // skip invalid polygons
      if(GetPolygon(i)->GetNbPoints()<3)
         continue;

      for(snIndex j=0;j<GetPolygon(i)->GetNbPoints();j++)
      {
         indices.push_back(GetPolygon(i)->GetPointIndex(j));
      }
   }
   return indices;
}

snIndexVec snMesh::GetPointIndicesCombined()
{
   snIndexVec indices;
   for(snIndex i=0;i<GetNbPolygons();i++)
   {
      // skip invalid polygons
      if(GetPolygon(i)->GetNbPoints()<3)
         continue;

      indices.push_back(GetPolygon(i)->GetNbPoints());
      for(snIndex j=0;j<GetPolygon(i)->GetNbPoints();j++)
      {
         indices.push_back(GetPolygon(i)->GetPointIndex(j));
      }
   }
   return indices;
}

void snMesh::SetPoint(const snVector3f & in_Point, snIndex in_Index)
{
   if(in_Index<mPoints.size())
      mPoints[in_Index] = in_Point;
}

void snMesh::AddPoint(const snVector3f & in_Point)
{
   mPoints.push_back(in_Point);

   // add just a single point lookup
   if(mTopoUpToDate)
   {
      mLU_PointIndexToSegmentIndices.push_back(snIndexVec());
      mLU_PointIndexToPolygonIndices.push_back(snIndexVec());
   }
}


snPolygon * snMesh::AddPolygon(snIndexVec in_Indices,bool in_UpdateLookups)

{
   mPolygons.push_back(new snPolygon(this));
   snPolygon * p = mPolygons[mPolygons.size()-1];
   for(snIndex i=0;i<in_Indices.size();i++)
      p->AddPointByIndex(in_Indices[i]);

   // if we are already up to date, update myself!
   if(mTopoUpToDate && in_UpdateLookups)
      RefreshTopoLookupsPerPolygon(p->GetIndex());
   return p;
}

snVector3f snMesh::GetPoint(snIndex in_Index) const
{
   if(in_Index<mPoints.size())
      return mPoints[in_Index];
   return snVector3f();
}

snVector3f * snMesh::GetPointPtr(snIndex in_Index)
{
   if(in_Index<mPoints.size())
      return &mPoints[in_Index];
   return NULL;
}

snVector3fVec * snMesh::ResizePoints(snIndex in_Size)
{
   if(in_Size != mPoints.size())
   {
      mPoints.resize(in_Size);
      InvalidateTopo();
   }
   return &mPoints;
}

snPolygon * snMesh::GetPolygon(snIndex in_Index)
{
   if(in_Index<GetNbPolygons())
      return mPolygons.at(in_Index);
   return NULL;
}

snBboxf snMesh::GetBbox()
{
   snBboxf box;
   for(snIndex i=0;i<mPoints.size();i++)
      box.Merge(mPoints[i]);
   return box;
}

void snMesh::Merge(snMesh * in_Mesh)
{
   snIndex offset = GetNbPoints();

   // copy all points
   for(snIndex i=0;i<in_Mesh->GetNbPoints();i++)
      mPoints.push_back(in_Mesh->GetPoint(i));

   // copy all polygons
   for(snIndex i=0;i<in_Mesh->GetNbPolygons();i++)
   {
      snIndexVec index = in_Mesh->GetPolygon(i)->GetPointIndices();
      for(snIndex j=0;j<index.size();j++)
         index[j] += offset;
      AddPolygon(index,false);
   }

   InvalidateTopo();
}

snIndex snMesh::RemoveUnusedPoints()
{
   snIndex removed = 0;

   // first count the usage
   snIndexVec count(GetNbPoints(),0);
   snIndexVec * pnts;
   for(snIndex i=0;i<GetNbPolygons();i++)
   {
      pnts = GetPolygon(i)->GetPointIndicesPtr();
      for(snIndex j=0;j<pnts->size();j++)
         count[pnts->at(j)]++;
   }

   // now create an offset map
   snIndexVec offset(GetNbPoints(),0);
   snVector3fVec newPoints;
   for(snIndex i=1,o=0;i<GetNbPoints();i++,o++)
   {
      // copy the offset from the previous point
      offset[i] = offset[o];
      if(count[o]==0)
      {
         offset[i]++;
         removed++;
      }
      else
         newPoints.push_back(GetPoint(o));
   }
   // add the last point
   if(count[count.size()-1]>0)
      newPoints.push_back(GetPoint(count.size()-1));

   // update the points!
   mPoints = newPoints;

   // now update the polygons
   for(snIndex i=0;i<GetNbPolygons();i++)
   {
      pnts = GetPolygon(i)->GetPointIndicesPtr();
      for(snIndex j=0;j<pnts->size();j++)
         pnts->at(j) -= offset[pnts->at(j)];
   }

   // if we removed some points, we need to refresh the lookups
   if(removed > 0)
      InvalidateTopo();
   return removed;
}

void snMesh::DeletePolygon(snIndex in_Index)
{
   if(in_Index >= GetNbPolygons())
      return;

   // first delete the memory of the polygon
   delete(mPolygons[in_Index]);
   snpPolygonVec::iterator it = mPolygons.begin();
   it+=in_Index;
   mPolygons.erase(it);

   InvalidateTopo();
}

void snMesh::RefreshTopoLookups()
{
   if(mTopoUpToDate)
      return;

   // reset the counters!
   mLU_NbSegments = 0;
   mLU_NbSamples = 0;

   // clear the dynamic fields
   mLU_PointIndices2SegmentIndex.clear();
   mLU_SegmentIndexToPolygonIndices.clear();
   mLU_SegmentIndexToPointIndices.clear();
   mLU_SegmentIndexToInverted.clear();

   // resize the static fields
   mLU_PointIndexToPointIndices.clear();
   mLU_PointIndexToSegmentIndices.clear();
   mLU_PointIndexToPolygonIndices.clear();
   mLU_PolygonIndexToPointIndices.clear();
   mLU_PolygonIndexToSegmentIndices.clear();
   mLU_PointIndexToPointIndices.resize(GetNbPoints(),snIndexVec());
   mLU_PointIndexToSegmentIndices.resize(GetNbPoints(),snIndexVec());
   mLU_PointIndexToPolygonIndices.resize(GetNbPoints(),snIndexVec());
   mLU_PolygonIndexToPointIndices.resize(GetNbPolygons(),snIndexVec());
   mLU_PolygonIndexToSegmentIndices.resize(GetNbPolygons(),snIndexVec());

   // now let's walk all polygons!
   for(snIndex i=0;i<GetNbPolygons();i++)
      RefreshTopoLookupsPerPolygon(i);

   mTopoUpToDate = true;
}

void snMesh::RefreshTopoLookupsPerPolygon(snIndex in_PolygonIndex)
{
   snPolygon * poly = GetPolygon(in_PolygonIndex);
   if(poly==NULL)
      return;

   snIndexVec * index = poly->GetPointIndicesPtr();
   snIndex i = in_PolygonIndex;

   // add a vector of indices for the polygon if necessary
   while(mLU_PolygonIndexToPointIndices.size() < in_PolygonIndex)
   {
      mLU_PolygonIndexToPointIndices.push_back(snIndexVec());
      mLU_PolygonIndexToSegmentIndices.push_back(snIndexVec());
   }

   for(snIndex j=0;j<index->size();j++)
   {
      // add the point to the polygon!
      mLU_PolygonIndexToPointIndices[i].push_back(index->at(j));
      // add the polygon to the point
      mLU_PointIndexToPolygonIndices[index->at(j)].push_back(i);

      // get the index tuple for the segment
      snIndexTuple tuple = poly->GetSegmentTuple(j);
      snIndexTupleMap::iterator it = mLU_PointIndices2SegmentIndex.find(tuple);
      if(it == mLU_PointIndices2SegmentIndex.end())
      {
         // this is a new edge, hurray! (the index of the edge is the size of edges mapped)
         mLU_PointIndexToSegmentIndices[index->at(j)].push_back(mLU_NbSegments);
         mLU_PolygonIndexToSegmentIndices[i].push_back(mLU_NbSegments);
         mLU_SegmentIndexToPolygonIndices.push_back(snIndexVec());
         mLU_SegmentIndexToPolygonIndices[mLU_NbSegments].push_back(i);
         mLU_SegmentIndexToPointIndices.push_back(snIndexVec());
         mLU_SegmentIndexToPointIndices[mLU_NbSegments].push_back(tuple.a);
         mLU_SegmentIndexToPointIndices[mLU_NbSegments].push_back(tuple.b);
         mLU_SegmentIndexToInverted.push_back((tuple.a != poly->GetPointIndex(j)) ? 1 : 0);

         // also add the points to points
         mLU_PointIndexToPointIndices[tuple.a].push_back(tuple.b);
         mLU_PointIndexToPointIndices[tuple.b].push_back(tuple.a);

         // insert it to the lookup!
         mLU_PointIndices2SegmentIndex.insert(snIndexTuplePair(tuple,mLU_NbSegments));
         mLU_NbSegments++;
      }
      else
      {
         // we already have the edge!
         mLU_PointIndexToSegmentIndices[index->at(j)].push_back(it->second);
         mLU_PolygonIndexToSegmentIndices[i].push_back(it->second);
         mLU_SegmentIndexToPolygonIndices[it->second].push_back(i);
      }

      mLU_NbSamples++;
   }
}

snIndex snMesh::GetSegmentIndexFromTuple(snIndexTuple in_Tuple)
{
   RefreshTopoLookups();
   std::map<snIndexTuple,snIndex>::iterator it = mLU_PointIndices2SegmentIndex.find(in_Tuple);
   if(it != mLU_PointIndices2SegmentIndex.end())
      return it->second;
   return 0;
}

bool snMesh::GetSegmentIsInverted(snIndex in_SegmentIndex)
{
   RefreshTopoLookups();
   if(in_SegmentIndex < mLU_SegmentIndexToInverted.size())
      return mLU_SegmentIndexToInverted[in_SegmentIndex] == 1;
   return 0;
}

snIndexTuple snMesh::GetSegmentTupleFromIndex(snIndex in_SegmentIndex)
{
   snIndexTuple result;
   RefreshTopoLookups();
   if(in_SegmentIndex < mLU_SegmentIndexToPointIndices.size())
      result.SetUnsorted(mLU_SegmentIndexToPointIndices[in_SegmentIndex][0],mLU_SegmentIndexToPointIndices[in_SegmentIndex][1]);
   return result;
}

snSegment snMesh::GetSegment(snIndex in_Index)
{
   RefreshTopoLookups();

   snSegment seg;
   if(in_Index >= mLU_NbSegments)
      return seg;

   seg.SetMesh(this);
   seg.SetPointID1(mLU_SegmentIndexToPointIndices[in_Index][0]);
   seg.SetPointID2(mLU_SegmentIndexToPointIndices[in_Index][1]);
   seg.Set(GetPoint(seg.GetPointID1()),GetPoint(seg.GetPointID2()));
   seg.SetIsInverse(GetSegmentIsInverted(in_Index));
   seg.SetIndex(in_Index);
   return seg;
}

snSegment snMesh::GetSegmentFromTuple(snIndexTuple in_Tuple)
{
   return GetSegment(GetSegmentIndexFromTuple(in_Tuple));
}

snIndexVec * snMesh::GetPointsFromPoint(snIndex in_PointIndex)
{
   RefreshTopoLookups();
   if(in_PointIndex >= mLU_PointIndexToPointIndices.size())
      return NULL;

   return &mLU_PointIndexToPointIndices[in_PointIndex];
}

snIndexVec * snMesh::GetPointsFromSegment(snIndex in_SegmentIndex)
{
   RefreshTopoLookups();
   if(in_SegmentIndex >= mLU_SegmentIndexToPointIndices.size())
      return NULL;

   return &mLU_SegmentIndexToPointIndices[in_SegmentIndex];
}

snIndexVec * snMesh::GetPointsFromPolygon(snIndex in_PolygonIndex)
{
   RefreshTopoLookups();
   if(in_PolygonIndex >= mLU_PolygonIndexToPointIndices.size())
      return NULL;

   return &mLU_PolygonIndexToPointIndices[in_PolygonIndex];
}

snIndexVec * snMesh::GetSegmentsFromPoint(snIndex in_PointIndex)
{
   RefreshTopoLookups();
   if(in_PointIndex >= mLU_PointIndexToSegmentIndices.size())
      return NULL;

   return &mLU_PointIndexToSegmentIndices[in_PointIndex];
}

snIndexVec * snMesh::GetSegmentsFromPolygon(snIndex in_PolygonIndex)
{
   RefreshTopoLookups();
   if(in_PolygonIndex >= mLU_PolygonIndexToSegmentIndices.size())
      return NULL;

   return &mLU_PolygonIndexToSegmentIndices[in_PolygonIndex];
}

snIndexVec * snMesh::GetPolygonsFromPoint(snIndex in_PointIndex)
{
   RefreshTopoLookups();
   if(in_PointIndex >= mLU_PointIndexToPolygonIndices.size())
      return NULL;

   return &mLU_PointIndexToPolygonIndices[in_PointIndex];
}

snIndexVec * snMesh::GetPolygonsFromSegment(snIndex in_SegmentIndex)
{
   RefreshTopoLookups();
   if(in_SegmentIndex >= mLU_SegmentIndexToPolygonIndices.size())
      return NULL;

   return &mLU_SegmentIndexToPolygonIndices[in_SegmentIndex];
}

snIndexTupleMap * snMesh::GetSegmentMap()
{
    RefreshTopoLookups();
    return &mLU_PointIndices2SegmentIndex;
}

bool snMesh::IsPointBoundary(snIndex in_PointIndex)
{
   RefreshTopoLookups();
   if(in_PointIndex >= GetNbPoints())
      return false;

   return GetSegmentsFromPoint(in_PointIndex)->size() > GetPolygonsFromPoint(in_PointIndex)->size();
}

bool snMesh::IsSegmentBoundary(snIndex in_SegmentIndex)
{
   RefreshTopoLookups();

   if(in_SegmentIndex >= GetNbSegments())
      return false;

   return GetPolygonsFromSegment(in_SegmentIndex)->size()==1;
}

bool snMesh::IsPolygonBoundary(snIndex in_PolygonIndex)
{
   RefreshTopoLookups();

   if(in_PolygonIndex >= GetNbPolygons())
      return false;

   snIndexVec * pnts = GetPolygon(in_PolygonIndex)->GetPointIndicesPtr();
   for(snIndex i=0;i<pnts->size();i++)
   {
      if(IsPointBoundary(pnts->at(i)))
         return true;
   }
   return false;
}

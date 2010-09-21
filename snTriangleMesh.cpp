#include "snTriangleMesh.h"

#include <math.h>

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

snTriangleMesh::snTriangleMesh()
: snMesh()
{
   mBoolUpToDate = false;
   mTriangles.clear();
}

snTriangleMesh::~snTriangleMesh()
{
   Clear();
}

void snTriangleMesh::Clear()
{
   snMesh::Clear();
   for(snIndex i=0;i<mTriangles.size();i++)
      delete(mTriangles[i]);
   mTriangles.clear();
}

void snTriangleMesh::Copy(snTriangleMesh * in_pTriangleMesh)
{
   Clear();
   mPoints = in_pTriangleMesh->mPoints;
   // copy all triangles
   mTriangles.resize(in_pTriangleMesh->mTriangles.size(),NULL);
   for(snIndex i=0;i<mTriangles.size();i++)
   {
      mTriangles[i] = new snTriangle(this);
      mTriangles[i]->Copy(in_pTriangleMesh->mTriangles[i]);
   }

   // copy the lookups
   mTopoUpToDate = in_pTriangleMesh->mTopoUpToDate;
   mLU_PointIndices2SegmentIndex = in_pTriangleMesh->mLU_PointIndices2SegmentIndex;
   mLU_PointIndexToPointIndices = in_pTriangleMesh->mLU_PointIndexToPointIndices;
   mLU_PointIndexToSegmentIndices = in_pTriangleMesh->mLU_PointIndexToSegmentIndices;
   mLU_PointIndexToPolygonIndices = in_pTriangleMesh->mLU_PointIndexToPolygonIndices;
   mLU_SegmentIndexToPolygonIndices = in_pTriangleMesh->mLU_SegmentIndexToPolygonIndices;
   mLU_SegmentIndexToPointIndices = in_pTriangleMesh->mLU_SegmentIndexToPointIndices;
   mLU_SegmentIndexToInverted = in_pTriangleMesh->mLU_SegmentIndexToInverted;
   mLU_PolygonIndexToPointIndices = in_pTriangleMesh->mLU_PolygonIndexToPointIndices;
   mLU_PolygonIndexToSegmentIndices = in_pTriangleMesh->mLU_PolygonIndexToSegmentIndices;
   mLU_NbSegments = in_pTriangleMesh->mLU_NbSegments;
   mLU_NbSamples = in_pTriangleMesh->mLU_NbSamples;
}

snTriangle * snTriangleMesh::AddTriangle(snIndex index1, snIndex index2, snIndex index3 ,bool in_UpdateLookups)
{
   mTriangles.push_back(new snTriangle(index1,index2,index3,this));
   snTriangle * triangle = mTriangles[mTriangles.size()-1];

   // if we are already up to date, update myself!
   if(mTopoUpToDate && in_UpdateLookups)
      RefreshTopoLookupsPerPolygon(triangle->GetIndex());
   return triangle;
}

snBboxf snTriangleMesh::GetBbox()
{
   RefreshBoolLookups();
   return mBBox;
}

snTriangle * snTriangleMesh::GetTriangle(snIndex in_Index)
{
   if(in_Index<GetNbTriangles())
      return mTriangles[in_Index];
   return NULL;
}

void snTriangleMesh::DeleteTriangle(snIndex in_Index)
{
   if(in_Index >= GetNbTriangles())
      return;

   // first delete the memory of the triangle
   delete(mTriangles[in_Index]);
   snpTriangleVec::iterator it = mTriangles.begin();
   it+=in_Index;
   mTriangles.erase(it);

   InvalidateTopo();
}

void snTriangleMesh::Merge(snTriangleMesh * in_Mesh, bool in_InvertTriangles)
{
   snIndex offset = GetNbPoints();

   // copy all points
   for(snIndex i=0;i<in_Mesh->GetNbPoints();i++)
      mPoints.push_back(in_Mesh->GetPoint(i));

   // copy all polygons
   if(in_InvertTriangles)
   {
      for(snIndex i=0;i<in_Mesh->GetNbTriangles();i++)
      {
         snTriangle * triangle = in_Mesh->GetTriangle(i);
         AddTriangle(
            triangle->GetPointIndex2()+offset,
            triangle->GetPointIndex1()+offset,
            triangle->GetPointIndex0()+offset,false);
      }
   }
   else
   {
      for(snIndex i=0;i<in_Mesh->GetNbTriangles();i++)
      {
         snTriangle * triangle = in_Mesh->GetTriangle(i);
         AddTriangle(
            triangle->GetPointIndex0()+offset,
            triangle->GetPointIndex1()+offset,
            triangle->GetPointIndex2()+offset,false);
      }
   }
   InvalidateTopo();

}

snIndex snTriangleMesh::RemoveUnusedPoints()
{
   snIndex removed = 0;

   // first count the usage
   snIndexVec count(GetNbPoints(),0);
   snIndexVec * pnts;
   snTriangle * triangle;
   for(snIndex i=0;i<GetNbTriangles();i++)
   {
      triangle = GetTriangle(i);
      for(snIndex j=0;j<3;j++)
         count[triangle->GetPointIndex(j)]++;
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
   for(snIndex i=0;i<GetNbTriangles();i++)
   {
      pnts = GetTriangle(i)->GetPointIndicesPtr();
      for(snIndex j=0;j<pnts->size();j++)
         pnts->at(j) -= offset[pnts->at(j)];
   }

   // if we removed some points, we need to refresh the lookups
   if(removed > 0)
      InvalidateTopo();
   return removed;
}

void snTriangleMesh::RefreshTopoLookupsPerPolygon(snIndex in_PolygonIndex)
{
   snTriangle * triangle = GetTriangle(in_PolygonIndex);
   if(triangle==NULL)
      return;

   snIndexVec * index = triangle->GetPointIndicesPtr();
   snIndex i = in_PolygonIndex;

   snString log;

   // add a vector of indices for the polygon if necessary
   while(mLU_PolygonIndexToPointIndices.size() < in_PolygonIndex)
   {
      mLU_PolygonIndexToPointIndices.push_back(snIndexVec());
      mLU_PolygonIndexToSegmentIndices.push_back(snIndexVec());
   }

   for(snIndex j=0;j<3;j++)
   {
      log = "refresh 1\n";
      log.Save("/tmp/test.log","ab");

      // add the point to the polygon!
      mLU_PolygonIndexToPointIndices[i].push_back(index->at(j));
      // add the polygon to the point
      mLU_PointIndexToPolygonIndices[index->at(j)].push_back(i);

      log = "refresh 2\n";
      log.Save("/tmp/test.log","ab");

      // get the index tuple for the segment
      snIndexTuple tuple = triangle->GetSegmentTuple(j);
      snIndexTupleMap::iterator it = mLU_PointIndices2SegmentIndex.find(tuple);
      if(it == mLU_PointIndices2SegmentIndex.end())
      {
         log = "refresh 3\n";
         log.Save("/tmp/test.log","ab");

         // this is a new edge, hurray! (the index of the edge is the size of edges mapped)
         mLU_PointIndexToSegmentIndices[index->at(j)].push_back(mLU_NbSegments);
         mLU_PolygonIndexToSegmentIndices[i].push_back(mLU_NbSegments);
         mLU_SegmentIndexToPolygonIndices.push_back(snIndexVec());
         mLU_SegmentIndexToPolygonIndices[mLU_NbSegments].push_back(i);
         mLU_SegmentIndexToPointIndices.push_back(snIndexVec());
         mLU_SegmentIndexToPointIndices[mLU_NbSegments].push_back(tuple.a);
         mLU_SegmentIndexToPointIndices[mLU_NbSegments].push_back(tuple.b);
         mLU_SegmentIndexToInverted.push_back((tuple.a != triangle->GetPointIndex(j)) ? 1 : 0);

         log = "refresh 4\n";
         log.Save("/tmp/test.log","ab");

         // also add the points to points
         mLU_PointIndexToPointIndices[tuple.a].push_back(tuple.b);
         mLU_PointIndexToPointIndices[tuple.b].push_back(tuple.a);

         log = "refresh 5\n";
         log.Save("/tmp/test.log","ab");

         // insert it to the lookup!
         mLU_PointIndices2SegmentIndex.insert(snIndexTuplePair(tuple,mLU_NbSegments));
         mLU_NbSegments++;
      }
      else
      {
         log = "refresh 6\n";
         log.Save("/tmp/test.log","ab");

         // we already have the edge!
         mLU_PointIndexToSegmentIndices[index->at(j)].push_back(it->second);
         mLU_PolygonIndexToSegmentIndices[i].push_back(it->second);
         mLU_SegmentIndexToPolygonIndices[it->second].push_back(i);

         log = "refresh 7\n";
         log.Save("/tmp/test.log","ab");

      }
   }
   mLU_NbSamples+=3;
}

void snTriangleMesh::RefreshBoolLookups()
{
   if(mBoolUpToDate)
      return;

   mBBox.Clear();

   // calculate the bbox
   for(snIndex i=0;i<mPoints.size();i++)
      mBBox.Merge(mPoints[i]);

   mBoolUpToDate = true;
}

bool snTriangleMesh::GetOutlines(snIndexVecVec * out_pResult)
{
   RefreshTopoLookups();

   // map to mark points as done!
   snIndex nbOutline = 0;
   snIndexVec pointOutline;
   snIndexVec pointDone(GetNbPoints(),0);
   for(snIndex i=0;i<pointDone.size();i++)
   {
      if(IsPointBoundary(i))
      {
         pointOutline.push_back(i);
         nbOutline++;
         pointDone[i] = 1;
      }
   }

   // let's early out here, if the output is not wanted
   if(out_pResult == NULL)
      return nbOutline > 0;

   // now as we know how many points we have as outline,
   // lets create the vectors for each one
   snIndexVec outline;
   snIndexVec * pointEdges;
   snIndexVec * edgePoints;
   bool found = true;
   snIndex nextIndex;
   while(nbOutline>0)
   {
      // if this is the first time we hit this
      if(outline.size()==0)
      {
         // find the first outline point
         for(snIndex i=0;i<pointOutline.size();i++)
         {
            if(pointDone[pointOutline[i]]==1)
            {
               // we found our start!
               pointDone[pointOutline[i]] = 0;
               outline.push_back(pointOutline[i]);
            }
         }
      }
      else
      {
         // find the next point in the outline
         found = false;
         pointEdges = GetSegmentsFromPoint(outline.back());

         // now, the first time we need to really watch for the direction!
         for(snIndex i=0;i<pointEdges->size();i++)
         {
            // skip non-boundary points
            if(!IsSegmentBoundary(pointEdges->at(i)))
               continue;

            // get the other point on that edge
            edgePoints = GetPointsFromSegment(pointEdges->at(i));
            nextIndex = (edgePoints->at(0) == outline.back()) ? edgePoints->at(1) : edgePoints->at(0);

            // skip done points!
            if(nextIndex == outline.front())
            {
               // this is closed!
               out_pResult->push_back(outline);
               outline.clear();
               found = true;
               break;
            }

            // if the point was touched before
            if(pointDone[nextIndex]==0)
               continue;

            pointDone[nextIndex] = 1;
            nbOutline--;
            found = true;
            break;
         }

         // this should not happen, but anyway
         // let's try to avoid an infinite loop
         if(!found)
            break;
      }
   }

   return nbOutline > 0;
}

bool snTriangleMesh::IsPointInsideClosed(const snVector3f & in_Point)
{
   snVector3f direction(0,1,0);
   snRay ray(in_Point,direction);

   mesh2line hits;
   if(GetIntersectionsWithRay(ray,&hits))
   {
      // check if the count is odd
      float countf = float(hits.index.size()) * .5f;
      return countf != floor(countf);
   }
   return false;
}

bool snTriangleMesh::IsPointInsideClosedConvex(const snVector3f & in_Point, mesh2ray * out_pResult)
{
   RefreshBoolLookups();

   // check if the point is inside the bounding box, if not
   // it cannot be inside the mesh
   if(!mBBox.IsPointInside(in_Point))
      return false;

   // now, create a ray and check the normal that you are hitting!
   snVector3f direction(0,1,0);
   snRay ray(in_Point,direction);
   mesh2ray intersection;
   if(GetIntersectionWithRay(ray,&intersection))
   {
      snTriangle * triangle = GetTriangle(intersection.index);
      bool result = triangle->GetPlanePtr()->GetNormal().Dot(direction)>0.0f;
      if(result && out_pResult != NULL)
      {
         out_pResult->index = intersection.index;
         out_pResult->intersection = intersection.intersection;
      }
      return result;
   }

   return false;
}

bool snTriangleMesh::GetIntersectionWithRay(const snRay & in_Ray, mesh2ray * out_pResult)
{
   // find out if the ray will ever hit the bbox!
   if(!in_Ray.GetIntersectionWithBBox(mBBox))
      return false;

   // find the triangle that we hit!
   snTriangle * triangle;
   snTriangle::tri2ray intersection;
   for(snIndex i=0;i<GetNbTriangles();i++)
   {
      triangle = GetTriangle(i);

      // if we hit a triangle, let's check if the normal is the same way
      if(triangle->GetIntersectionWithRay(in_Ray,&intersection))
      {
         if(out_pResult != NULL)
         {
            out_pResult->index = i;
            out_pResult->intersection = intersection;
         }
         return true;
      }
   }
   return false;
}

bool snTriangleMesh::GetClosestIntersectionWithRay(const snRay & in_Ray, mesh2ray * out_pResult)
{
   // if we want to find the closest one, let's use the GetALl method
   if(out_pResult == NULL)
      return false;

   mesh2line hits;
   if(GetIntersectionsWithRay(in_Ray,&hits))
   {
      out_pResult->index = hits.index.front();
      out_pResult->intersection = hits.intersection.front();
      return true;
   }
   return false;
}

bool snTriangleMesh::GetIntersectionsWithRay(const snRay & in_Ray, mesh2line * out_pResult)
{
   // in this case we really want to collect intersections,
   // so if there is no output pointer, bail out
   if(out_pResult == NULL)
      return false;

   // find out if the ray will ever hit the bbox!
   if(!in_Ray.GetIntersectionWithBBox(mBBox))
      return false;

   // find the triangle that we hit!
   snTriangle * triangle;
   snTriangle::tri2ray triangleHit;
   mesh2ray hit;
   std::map<float,mesh2ray> hits;
   float distance;
   snVector3f hitPoint;
   for(snIndex i=0;i<GetNbTriangles();i++)
   {
      triangle = GetTriangle(i);

      // see if the triangle is hit by the ray
      if(triangle->GetIntersectionWithRay(in_Ray,&triangleHit))
      {
         // construct the mesh hit
         hit.index = i;
         hit.intersection = triangleHit;

         // calculate a pseudo distance
         // (fast using dot product)
         hitPoint.Sub(triangleHit.pnt,in_Ray.GetOrigin());
         distance = in_Ray.GetDirection().Dot(hitPoint);
         hits.insert(std::pair<float,mesh2ray>(distance,hit));
      }
   }

   // if we don't have any hits
   if(hits.size()==0)
      return false;

   // loop over all results and output them!
   out_pResult->index.resize(hits.size());
   out_pResult->intersection.resize(hits.size());
   snIndex i=0;
   for(std::map<float,mesh2ray>::iterator it=hits.begin();it!=hits.end();it++)
   {
      out_pResult->index[i] = it->second.index;
      out_pResult->intersection[i++] = it->second.intersection;
   }
   return true;
}

bool snTriangleMesh::GetIntersectionsWithLine(const snLine & in_Line, mesh2line * out_pResult)
{
   // find out if the line will ever hit the bbox!
   if(!in_Line.GetIntersectionWithBBox(mBBox))
      return false;

   // find the triangle that we hit!
   snTriangle * triangle;
   snTriangle::tri2ray intersection;
   bool result = false;
   for(snIndex i=0;i<GetNbTriangles();i++)
   {
      triangle = GetTriangle(i);

      // if we hit a triangle, let's check if the normal is the same way
      if(triangle->GetIntersectionWithLine(in_Line,&intersection))
      {
         if(out_pResult != NULL)
         {
            out_pResult->index.push_back(i);
            out_pResult->intersection.push_back(intersection);
            result = true;
         }
         else
            // early jump out if we don't have to collect intersections
            return true;
      }
   }
   return result;
}

bool snTriangleMesh::GetIntersectionsWithSegment(const snSegment & in_Segment, mesh2line * out_pResult)
{
   // find out if the segment will ever hit the bbox!
   if(!in_Segment.GetIntersectionWithBBox(mBBox))
      return false;

   // find the triangle that we hit!
   snTriangle * triangle;
   snTriangle::tri2ray intersection;
   bool result = false;
   for(snIndex i=0;i<GetNbTriangles();i++)
   {
      triangle = GetTriangle(i);

      // if we hit a triangle, let's check if the normal is the same way
      if(triangle->GetIntersectionWithSegment(in_Segment,&intersection))
      {
         if(out_pResult != NULL)
         {
            out_pResult->index.push_back(i);
            out_pResult->intersection.push_back(intersection);
            result = true;
         }
         else
            // early jump out if we don't have to collect intersections
            return true;
      }
   }
   return result;
}

bool snTriangleMesh::GetMeshBelowPlane(const snPlane & in_Plane, snTriangleMesh * out_pResult)
{
   // early jumpout
   if(out_pResult == NULL)
      return false;

   // first, let's find all points that are below the plane
   snIndex nbBelow = 0;
   std::vector<float> pointDistance(GetNbPoints(),0.0f);
   snIndexVec pointPointIndex(GetNbPoints());
   snIndexVec pointsBelow(GetNbPoints());
   for(snIndex i=0;i<GetNbPoints();i++)
   {
      pointDistance[i] = in_Plane.GetDistance(GetPoint(i));
      if(pointDistance[i]<0)
      {
         pointPointIndex[i] = nbBelow;
         pointsBelow[nbBelow] = i;
         nbBelow++;
      }
   }

   // empty the output mesh
   out_pResult->Clear();

   // if there is nothing above, we can't cut it
   if(nbBelow == pointDistance.size())
   {
      out_pResult->Merge(this);
      return false;
   }

   // if all are above
   if(nbBelow == 0)
      return false;

   // great, first, let's add all of the old points
   for(snIndex i=0;i<nbBelow;i++)
      out_pResult->AddPoint(GetPoint(pointsBelow[i]));

   // we have to cut something!
   // let's cut all edges and remember the newly created points!
   std::vector<bool> edgeIsCut(GetNbSegments(),false);
   std::vector<snIndex> edgePointIndex(GetNbSegments());
   snIndexVec * edgePoints;
   snVector3f edgePos;
   for(snIndex i=0;i<GetNbSegments();i++)
   {
      edgePoints = GetPointsFromSegment(i);
      // if the points are on the same side, continue
      if((pointDistance[edgePoints->at(0)] < 0) == (pointDistance[edgePoints->at(1)] < 0))
         continue;

      // create the new point
      edgePos.InterPolate(
         fabs(pointDistance[edgePoints->at(0)]) / (fabs(pointDistance[edgePoints->at(0)]) + fabs(pointDistance[edgePoints->at(1)])),
              GetPoint(edgePoints->at(0)),GetPoint(edgePoints->at(1)));

      // remember the new point's index
      edgePointIndex[i] = out_pResult->GetNbPoints();
      out_pResult->AddPoint(edgePos);
   }

   // cool, now let's add all polygons...!
   snTriangle * triangle;
   snIndex index0,index1,index2;
   bool above0,above1,above2;
   for(snIndex i=0;i<GetNbTriangles();i++)
   {
      triangle = GetTriangle(i);
      for(snIndex k=0;k<3;k++)
      {
         // we loop here for shifting!
         index0 = triangle->GetPointIndex((k+0)%3);
         index1 = triangle->GetPointIndex((k+1)%3);
         index2 = triangle->GetPointIndex((k+2)%3);
         above0 = pointDistance[index0] > 0.0f;
         above1 = pointDistance[index1] > 0.0f;
         above2 = pointDistance[index2] > 0.0f;

         // first check if the triangle is completely above
         if(above0 && above1 && above2)
            break;

         // now check if the triangle is completely below
         if(!above0 && !above1 && !above2)
         {
            out_pResult->AddTriangle(
               pointPointIndex[index0],
               pointPointIndex[index1],
               pointPointIndex[index2]);
            break;
         }

         // well well, this polygon has to be cut, let's find out
         // which edge has to be cut
         if(above0 && above1 && !above2)
         {
            // we need to create a new triangle
            out_pResult->AddTriangle(
               edgePointIndex[GetSegmentIndexFromTuple(snIndexTuple(index1,index2,true))],
               pointPointIndex[index2],
               edgePointIndex[GetSegmentIndexFromTuple(snIndexTuple(index0,index2,true))]);
            break;;
         }

         // we need to create two triangles!
         if(above0 && !above1 && !above2)
         {
            out_pResult->AddTriangle(
               edgePointIndex[GetSegmentIndexFromTuple(snIndexTuple(index0,index2,true))],
               edgePointIndex[GetSegmentIndexFromTuple(snIndexTuple(index0,index1,true))],
               pointPointIndex[index1]);
            out_pResult->AddTriangle(
               edgePointIndex[GetSegmentIndexFromTuple(snIndexTuple(index0,index2,true))],
               pointPointIndex[index1],
               pointPointIndex[index2]);
            break;
         }
      }
   }

   return false;
}

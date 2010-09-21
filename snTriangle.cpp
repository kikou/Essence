#include "snTriangle.h"

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

snTriangle::snTriangle(snIndex index1, snIndex index2, snIndex index3, snTriangleMesh * in_pTriangleMesh)
: snPolygon((snMesh*)in_pTriangleMesh)
{
   // add the indices!
   mIndices.resize(3);
   mIndices[0] = index1;
   mIndices[1] = index2;
   mIndices[2] = index3;
   mPlaneUptoDate = false;
}

snTriangle::snTriangle(snTriangleMesh * in_pTriangleMesh)
: snPolygon((snMesh*)in_pTriangleMesh)
{
   mPlaneUptoDate = false;
}

snTriangle::~snTriangle()
{
}

void snTriangle::Copy(snTriangle * in_pTriangle)
{
   mIndices = in_pTriangle->mIndices;
   mPlaneUptoDate = false;
}

snVector3f snTriangle::GetCenter() const
{
   snVector3f center;
   center.InterPolate(0.5f,GetPoint(0),GetPoint(1));
   center.InterPolate(0.5f,GetPoint(2));
   return center;
}

snIndexTuple snTriangle::GetSegmentTuple(snIndex in_Index)
{
   snIndexTuple result;
   in_Index = in_Index % 3;
   switch(in_Index)
   {
      case 0:
         result.SetUnsorted(GetPointIndex0(),GetPointIndex1());
         break;
      case 1:
         result.SetUnsorted(GetPointIndex1(),GetPointIndex2());
         break;
      case 2:
         result.SetUnsorted(GetPointIndex0(),GetPointIndex2());
         break;
   }
   return result;
}

void snTriangle::UpdatePlane()
{
   if(mPlaneUptoDate)
      return;

   // get normal of the triangle
   snVector3f edge1,edge2,normal;
   edge1.Sub(GetPoint1(),GetPoint0());
   edge2.Sub(GetPoint2(),GetPoint0());

   // shit.. we don't have a normal
   normal.Cross(edge1,edge2);
   normal.Normalize();

   mPlane.Set(GetPoint0(),normal);
   mPlaneUptoDate = true;
}

bool snTriangle::GetEdgePlane(snIndex in_Index, snPlane * out_pResult)
{
   if(out_pResult==NULL)
      return false;
   snVector3f edge,normal;
   edge.Sub(GetPoint(GetPointIndex((in_Index+1) % 3)),GetPoint(GetPointIndex(in_Index % 3)));
   normal.Cross(GetPlanePtr()->GetNormal(),edge);
   out_pResult->Set(GetPoint(GetPointIndex(in_Index % 3)),normal);
   return true;
}

snTriangle::uvCoord snTriangle::GetUVFromPoint(const snVector3f & in_Point)
{
   // Compute vectors
   snVector3f v0,v1,v2;
   v0.Sub(GetPoint2(),GetPoint0());
   v1.Sub(GetPoint1(),GetPoint0());
   v2.Sub(in_Point,GetPoint0());

   // Compute dot products
   float dot00 = v0.Dot(v0);
   float dot01 = v0.Dot(v1);
   float dot02 = v0.Dot(v2);
   float dot11 = v1.Dot(v1);
   float dot12 = v1.Dot(v2);

   // Compute barycentric coordinates
   float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);

   // pack into struct
   uvCoord uv;
   uv.u = (dot11 * dot02 - dot01 * dot12) * invDenom;
   uv.v = (dot00 * dot12 - dot01 * dot02) * invDenom;
   return uv;
}

snVector3f snTriangle::GetPointFromUV(const snTriangle::uvCoord & in_UV)
{
   snVector3f v0,v1,result;
   v0.Sub(GetPoint2(),GetPoint0());
   v1.Sub(GetPoint1(),GetPoint0());
   v0.Scale(in_UV.u);
   v1.Scale(in_UV.v);

   result.Add(v0,v1);
   result.Add(GetPoint(0));
   return result;
}

int snTriangle::GetIntersectionWithUVPair(const uvCoord & in_UVInside,const uvCoord & in_UVOutside)
{
   if(in_UVOutside.u + in_UVOutside.v >= 1.0f)
   {
      return 1; // edge between points 1 & 2
   }

   if(in_UVOutside.u >= 0.0f && in_UVOutside.v >= 0.0f)
   {
      return -1; // no intersection
   }

   if(in_UVOutside.u <= 0)
   {
      if(fabs(in_UVInside.u / in_UVInside.v) > fabs(in_UVInside.u - in_UVOutside.u) / fabs(in_UVInside.v - in_UVOutside.v))
         return 2;
      return 0;
   }

   return 2; // edge between points 0 & 2
}


bool snTriangle::IsPointOnTriangle(const snVector3f & in_Point)
{
   // check if the UVs are inside
   uvCoord uv = GetUVFromPoint(in_Point);
   if(!uv.IsOnTriangle())
      return false;
   return true;
}

bool snTriangle::IsPointInsideTriangle(const snVector3f & in_Point)
{
   // check if the UVs are inside
   uvCoord uv = GetUVFromPoint(in_Point);
   if(!uv.IsInsideTriangle())
      return false;
   return true;
}

bool snTriangle::GetIntersectionWithBBox(const snBboxf & in_Bbox)
{
   snIndex index[3];
   index[0] = GetPointIndex0();
   index[1] = GetPointIndex1();
   index[2] = GetPointIndex2();
   snSegment segment;

   for(snIndex i=0;i<3;i++)
   {
      if(in_Bbox.IsPointInside(GetPoint(index[i])))
         return true;
      segment.Set(GetPoint(index[i]),GetPoint(index[(i+1)%3]));
      if(segment.GetIntersectionWithBBox(in_Bbox))
         return true;
   }

   // additionally we need to compare all edges of the cube
   // to intersect with the triangle, otherwise it might be cutting
   return false;
}

bool snTriangle::GetIntersectionWithLine(const snLine & in_Line, snTriangle::tri2ray * out_pResult)
{
   snVector3f intersection;
   if(!GetPlanePtr()->GetIntersectionWithLine(in_Line,&intersection))
      return false;
   uvCoord uv = GetUVFromPoint(intersection);
   if(!uv.IsOnTriangle())
      return false;
   if(out_pResult!=NULL)
   {
      out_pResult->pnt = intersection;
      out_pResult->uv = uv;
   }
   return true;
}

bool snTriangle::GetIntersectionWithRay(const snRay & in_Ray, snTriangle::tri2ray * out_pResult)
{
   snVector3f intersection;
   if(!GetPlanePtr()->GetIntersectionWithRay(in_Ray,&intersection))
      return false;

   uvCoord uv = GetUVFromPoint(intersection);
   if(!uv.IsOnTriangle())
      return false;

   if(out_pResult!=NULL)
   {
      out_pResult->pnt = intersection;
      out_pResult->uv = uv;
   }
   return true;
}

bool snTriangle::GetIntersectionWithSegment(const snSegment & in_Segment, snTriangle::tri2ray * out_pResult)
{
   snVector3f intersection;
   if(!GetPlanePtr()->GetIntersectionWithSegment(in_Segment,&intersection))
      return false;
   uvCoord uv = GetUVFromPoint(intersection);
   if(!uv.IsOnTriangle())
      return false;
   if(out_pResult!=NULL)
   {
      out_pResult->pnt = intersection;
      out_pResult->uv = uv;
   }
   return true;
}

bool snTriangle::GetIntersectionWithPlane(const snPlane & in_Plane, snSegment * out_pResult)
{
   // get all distances to the plane
   float distance0 = in_Plane.GetDistance(GetPoint0());
   float distance1 = in_Plane.GetDistance(GetPoint1());
   float distance2 = in_Plane.GetDistance(GetPoint2());

   // if all are above or all are below, we don't intersect
   if(((distance0>0.0f)&&(distance1>0.0f)&&(distance2>0.0f)) ||
      ((distance0<0.0f)&&(distance1<0.0f)&&(distance2<0.0f)))
      return false;

   // now let's find out which edges cut
   bool edge0Cuts = ((distance0>0.0f)&&(distance1<0.0f)) || ((distance0<0.0f)&&(distance1>0.0f));
   bool edge1Cuts = ((distance1>0.0f)&&(distance2<0.0f)) || ((distance1<0.0f)&&(distance2>0.0f));

   // decide how to set the result
   snVector3f p1,p2;
   if(edge0Cuts && edge1Cuts)
   {
      if(out_pResult!=NULL)
      {
         p1.InterPolate(fabs(distance0) / (fabs(distance0) + fabs(distance1)),GetPoint0(),GetPoint1());
         p2.InterPolate(fabs(distance1) / (fabs(distance1) + fabs(distance2)),GetPoint1(),GetPoint2());
         out_pResult->Set(p1,p2);
         out_pResult->SetPointID1(GetSegmentIndex(0));
         out_pResult->SetPointID2(GetSegmentIndex(1));
      }
      return true;
   }

   // delaying this to here since we didn't need before
   bool edge2Cuts = ((distance0>0.0f)&&(distance2<0.0f)) || ((distance0<0.0f)&&(distance2>0.0f));
   if(edge1Cuts && edge2Cuts)
   {
      if(out_pResult!=NULL)
      {
         p1.InterPolate(fabs(distance1) / (fabs(distance1) + fabs(distance2)),GetPoint1(),GetPoint2());
         p2.InterPolate(fabs(distance0) / (fabs(distance0) + fabs(distance2)),GetPoint0(),GetPoint2());
         out_pResult->Set(p1,p2);
         out_pResult->SetPointID1(GetSegmentIndex(1));
         out_pResult->SetPointID2(GetSegmentIndex(2));
      }
      return true;
   }
   if(edge0Cuts && edge2Cuts)
   {
      if(out_pResult!=NULL)
      {
         p1.InterPolate(fabs(distance0) / (fabs(distance0) + fabs(distance1)),GetPoint0(),GetPoint1());
         p2.InterPolate(fabs(distance0) / (fabs(distance0) + fabs(distance2)),GetPoint0(),GetPoint2());
         out_pResult->Set(p1,p2);
         out_pResult->SetPointID1(GetSegmentIndex(0));
         out_pResult->SetPointID2(GetSegmentIndex(2));
      }
      return true;
   }
   return false;
}

bool snTriangle::GetIntersectionWithTriangle(snTriangle * in_pTriangle, snTriangle::tri2tri * out_pResult, bool in_bSkipSecondaryCases)
{
   // ok, first let's intersect with the plane
   snSegment cut;
   if(!in_pTriangle->GetIntersectionWithPlane(GetPlane(),&cut))
      return false;

   // since we have an intersection, let's find out about the uv coords
   uvCoord uv11 = GetUVFromPoint(cut.GetStart());
   uvCoord uv12 = GetUVFromPoint(cut.GetEnd());
   bool uv11Inside = uv11.IsInsideTriangle();
   bool uv12Inside = uv12.IsInsideTriangle();

   // if both are inside, just return the segment!
   if(uv11Inside && uv12Inside)
   {
      if(out_pResult != NULL)
      {
         out_pResult->tri0Index = GetIndex();
         out_pResult->tri1Index = in_pTriangle->GetIndex();

         out_pResult->edge1.push_back(cut.GetPointID1());
         out_pResult->edge1.push_back(cut.GetPointID2());
         out_pResult->edgePos1.push_back(cut.GetStart());
         out_pResult->edgePos1.push_back(cut.GetEnd());
      }
      return true;
   }

   // abort the recursion!
   if(in_bSkipSecondaryCases)
      return false;

   // if one side is inside and the other one outside
   if(uv11Inside != uv12Inside)
   {
      if(out_pResult != NULL)
      {
         snSegment cut2;
         if(!GetIntersectionWithPlane(in_pTriangle->GetPlane(),&cut2))
            return false;

         // since we have an intersection, let's find out about the uv coords
         uvCoord uv21 = in_pTriangle->GetUVFromPoint(cut2.GetStart());
         uvCoord uv22 = in_pTriangle->GetUVFromPoint(cut2.GetEnd());
         bool uv21Inside = uv21.IsInsideTriangle();
         bool uv22Inside = uv22.IsInsideTriangle();

         snVector3f pnt;
         snPlane plane;
         int edgeIndex;

         if(uv11Inside && !uv12Inside)
         {
            edgeIndex = GetIntersectionWithUVPair(uv11,uv12);
            if(edgeIndex != -1)
            {
               GetEdgePlane(edgeIndex,&plane);
               if(plane.GetIntersectionWithSegment(cut,&pnt))
               {
                  out_pResult->edge0.push_back(GetSegmentIndex(edgeIndex));
                  out_pResult->edgePos0.push_back(pnt);
               }
            }

            out_pResult->edge1.push_back(cut.GetPointID1());
            out_pResult->edgePos1.push_back(cut.GetStart());
         }
         else if(!uv11Inside && uv12Inside)
         {
            edgeIndex = GetIntersectionWithUVPair(uv12,uv11);
            if(edgeIndex != -1)
            {
               GetEdgePlane(edgeIndex,&plane);
               if(plane.GetIntersectionWithSegment(cut,&pnt))
               {
                  out_pResult->edge0.push_back(GetSegmentIndex(edgeIndex));
                  out_pResult->edgePos0.push_back(pnt);
               }
            }

            out_pResult->edge1.push_back(cut.GetPointID2());
            out_pResult->edgePos1.push_back(cut.GetEnd());
         }

         if(uv21Inside && !uv22Inside)
         {
            out_pResult->edge0.push_back(cut2.GetPointID1());
            out_pResult->edgePos0.push_back(cut2.GetStart());
         }
         else if(!uv21Inside && uv22Inside)
         {
            out_pResult->edge0.push_back(cut2.GetPointID2());
            out_pResult->edgePos0.push_back(cut2.GetEnd());
         }
      }
      return true;
   }

   // if we reach here, it means that both points are outside of the
   // triangle. let's do the test the opposite way, and see what happens
   // if we don't hit, fair enough! (the 'true' in the end ensure the
   // recursion will stop!)
   tri2tri intersection;
   if(in_pTriangle->GetIntersectionWithTriangle(this,&intersection,true))
   {
      if(out_pResult != NULL)
      {
         out_pResult->tri0Index = intersection.tri1Index;
         out_pResult->tri1Index = intersection.tri0Index;

         out_pResult->edge0 = intersection.edge1;
         out_pResult->edge1 = intersection.edge0;
         out_pResult->edgePos0 = intersection.edgePos1;
         out_pResult->edgePos1 = intersection.edgePos0;
      }
      return true;
   }
   return false;
}

void snTriangle::Invert()
{
   snIndex tmp = mIndices[2];
   mIndices[2] = mIndices[0];
   mIndices[0] = tmp;
   mMesh->InvalidateTopo();
}

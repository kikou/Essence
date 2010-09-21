#include "snPlane.h"

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

snPlane::snPlane()
{
   mOrigin.Set(0,0,0);
   mNormal.Set(0,1,0);
   mPointID1 = 0;
   mPointID2 = 0;
}


snPlane::snPlane(const snVector3f & in_Origin, const snVector3f & in_Normal, bool in_GivenTwoPoints)
{
   if(in_GivenTwoPoints)
      SetFromTwoPoints(in_Origin,in_Normal);
   else
      Set(in_Origin,in_Normal);

   mPointID1 = 0;
   mPointID2 = 0;
}


void snPlane::Set(const snVector3f & in_Origin, const snVector3f & in_Normal)
{
   mOrigin = in_Origin;
   mNormal = in_Normal;
}


bool snPlane::SetFromTwoPoints(const snVector3f & in_PointA, const snVector3f & in_PointB)
{
   mOrigin.InterPolate(0.5f,in_PointA,in_PointB);
   mNormal.Sub(in_PointB,in_PointA);
   return mNormal.Normalize();
}


float snPlane::GetDistance(const snVector3f & in_Point) const
{
   snVector3f p;
   p.Sub(in_Point,mOrigin);
   return p.Dot(mNormal);
}


bool snPlane::GetIntersectionWithSegment(const snSegment & in_Segment, snVector3f * out_pResult) const
{
   float distance1 = GetDistance(in_Segment.GetStart());
   float distance2 = GetDistance(in_Segment.GetEnd());
   if((distance1 > 0 && distance2 < 0) || (distance1 < 0 && distance2 > 0))
   {
      if(out_pResult != NULL)
         out_pResult->InterPolate(fabs(distance1) / (fabs(distance1) + fabs(distance2)),in_Segment.GetStart(), in_Segment.GetEnd());
      return true;
   }
   return false;
}

bool snPlane::GetIntersectionWithLine(const snLine & in_Line, snVector3f * out_pResult) const
{
   float dNd = in_Line.GetDirection().Dot(mNormal);
   if(dNd != 0.0f)
   {
      if(out_pResult!=NULL)
      {
         float distance = GetDistance(in_Line.GetOrigin());
         out_pResult->Scale(-distance / dNd, in_Line.GetDirection());
         out_pResult->Add(in_Line.GetOrigin());
      }
      return true;
   }
   return false;
}

bool snPlane::GetIntersectionWithRay(const snRay & in_Ray, snVector3f * out_pResult) const
{
   snVector3f p;
   float dNd = in_Ray.GetDirection().Dot(mNormal);
   if(dNd == 0.0f)
      return false;
   float distance = GetDistance(in_Ray.GetOrigin());
   if(dNd > 0.0f && distance > 0.0f)
      return false;
   else if(dNd < 0.0f && distance < 0.0f)
      return false;
   if(out_pResult!=NULL)
   {
      out_pResult->Scale(-distance / dNd, in_Ray.GetDirection());
      out_pResult->Add(in_Ray.GetOrigin());
   }
   return true;
}

bool snPlane::GetIntersectionWithPlane(const snPlane & in_Plane, snLine * out_pResult) const
{
   if(mNormal == in_Plane.GetNormal())
      return false;
   if(mNormal.GetNegated() == in_Plane.GetNormal())
      return false;

   if(out_pResult!=NULL)
   {
      snVector3f normal,direction,origin;
      normal.Cross(GetNormal(),in_Plane.GetNormal());
      direction.Cross(normal,in_Plane.GetNormal());
      snLine line(in_Plane.GetOrigin(),direction);
      GetIntersectionWithLine(line,&origin);
      out_pResult->Set(origin,normal);
   }
   return true;
}

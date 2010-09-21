#include "snRay.h"

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


using namespace snEssence;

snRay::snRay()
: snLine()
{
}


snRay::snRay(const snVector3f & in_Origin, const snVector3f & in_Direction)
: snLine(in_Origin,in_Direction)
{
}



bool snRay::GetIntersectionWithBBox(const snBboxf & in_Bbox, snVector3fVec * out_pResult) const
{
   // first check if the ray fires "away" from the box
   snVector3f pMin = in_Bbox.GetMin();
   snVector3f pMax = in_Bbox.GetMax();
   if(
         ((mStart.GetX() < pMin.GetX()) && (mDirection.GetX() <= 0)) ||
         ((mStart.GetY() < pMin.GetY()) && (mDirection.GetY() <= 0)) ||
         ((mStart.GetZ() < pMin.GetZ()) && (mDirection.GetZ() <= 0)) ||
         ((mStart.GetX() > pMax.GetX()) && (mDirection.GetX() >= 0)) ||
         ((mStart.GetY() > pMax.GetY()) && (mDirection.GetY() >= 0)) ||
         ((mStart.GetZ() > pMax.GetZ()) && (mDirection.GetZ() >= 0)))
      return false;

   // localize the vectors
   snVector3f width,localStart,localDirection;
   width.Sub(pMax,pMin);
   width.Invert();
   localStart.Sub(mStart,pMin);
   localStart.Mul(width);
   localDirection.Mul(mDirection,width);
   snRay local(localStart,localDirection);

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
      if(plane[i].GetIntersectionWithRay(local,&intersection))
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


#include "snBboxf.h"

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

snBboxf::snBboxf()

{
   Clear();
}


snVector3f snBboxf::GetCenter() const
{
   snVector3f result;
   result.InterPolate(0.5f,GetMin(),GetMax());
   return result;
}

void snBboxf::MergeMin( const snVector3f & in_Min )
{
   if(mIsValidMin)
      mMin.Min(in_Min);
   else
      mMin =  in_Min;
   mIsValidMin = true;
}


void snBboxf::MergeMax( const snVector3f & in_Max )
{
   if(mIsValidMax)
      mMax.Max(in_Max);
   else
      mMax = in_Max;
   mIsValidMax = true;
}

void snBboxf::Clear()
{
   mMin.Set(0,0,0);

   mMax.Set(0,0,0);
   mIsValidMin = false;

   mIsValidMax = false;

}

void snBboxf::Extend(float in_Extension)
{
   if(mIsValidMin)
      mMin.Sub(snVector3f(in_Extension,in_Extension,in_Extension));
   if(mIsValidMax)
      mMax.Add(snVector3f(in_Extension,in_Extension,in_Extension));
}

void snBboxf::Merge(const snBboxf & in_Other)
{
   if(in_Other.IsValidMin())
      MergeMin(in_Other.mMin);
   if(in_Other.IsValidMax())
      MergeMax(in_Other.mMax);
}


void snBboxf::Scale(const snVector3f & in_Scaling)

{
   if(mIsValidMin)
      mMin.Mul(in_Scaling);
   if(mIsValidMax)
      mMax.Mul(in_Scaling);
}


void snBboxf::Rotate(const snQuaternionf & in_Rotation)

{
   if(mIsValidMin)
      mMin.Mul(in_Rotation);
   if(mIsValidMax)
      mMax.Mul(in_Rotation);
}


void snBboxf::Move(const snVector3f & in_Movement)
{
   if(mIsValidMin)
      mMin.Add(in_Movement);
   if(mIsValidMax)
      mMax.Add(in_Movement);
}


bool snBboxf::IsPointInside(const snVector3f & in_Pos) const
{
   return in_Pos.GetX() >= mMin.GetX() && in_Pos.GetX() <= mMax.GetX() &&
          in_Pos.GetY() >= mMin.GetY() && in_Pos.GetY() <= mMax.GetY() &&
          in_Pos.GetZ() >= mMin.GetZ() && in_Pos.GetZ() <= mMax.GetZ();
}

bool snBboxf::GetIntersectionWithBbox(const snBboxf & in_BBox, snBboxf * out_pResult)
{
   // check if the one box is inside the other one
   snVector3f min2,max2;
   min2 = in_BBox.GetMin();
   max2 = in_BBox.GetMax();

   if((min2.GetX() < mMin.GetX() && max2.GetX() < mMin.GetX()) ||
      (min2.GetX() > mMax.GetX() && max2.GetX() > mMax.GetX()) ||
      (min2.GetY() < mMin.GetY() && max2.GetY() < mMin.GetY()) ||
      (min2.GetY() > mMax.GetY() && max2.GetY() > mMax.GetY()) ||
      (min2.GetZ() < mMin.GetZ() && max2.GetZ() < mMin.GetZ()) ||
      (min2.GetZ() > mMax.GetZ() && max2.GetZ() > mMax.GetZ()))
      return false;

   // compose the resulting bbox
   if(out_pResult != NULL)
   {
      out_pResult->SetMin(min2.GetMax(mMin));
      out_pResult->SetMax(max2.GetMin(mMax));
   }
   return true;
}

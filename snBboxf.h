#ifndef __SNBBOXF__
#define __SNBBOXF__

#include "snVector3f.h"
#include <vector>

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
   /**
    * A float based bounding box.
    */
   class snBboxf
   {
   private:
      snVector3f mMin;
      snVector3f mMax;
      bool mIsValidMin;
      bool mIsValidMax;
   public:

      // constructors
      snBboxf();
      snBboxf(const snBboxf & in_Other)
      {
         mMin = in_Other.mMin;
         mMax = in_Other.mMax;
         mIsValidMin = in_Other.mIsValidMin;
         mIsValidMax = in_Other.mIsValidMax;
      }
      snBboxf(const snVector3f & in_Min,const snVector3f & in_Max)
      {
         mMin = in_Min;
         mMax = in_Max;
         mIsValidMin = true;
         mIsValidMax = true;
      }
      void Clear();

      // getters
      snVector3f GetMin() const { return mMin; }
      snVector3f GetMax() const { return mMax; }
      snVector3f * GetMinPtr() { return &mMin; }
      snVector3f * GetMaxPtr() { return &mMax; }
      bool IsValidMin() const { return mIsValidMin; }
      bool IsValidMax() const { return mIsValidMax; }
      bool IsValid() const { return mIsValidMin && mIsValidMax; }
      snVector3f GetCenter() const;

      // setters
      void Set(const snBboxf & in_Other)
      {
         mMin = in_Other.mMin;
         mMax = in_Other.mMax;
         mIsValidMin = in_Other.mIsValidMin;
         mIsValidMax = in_Other.mIsValidMax;
      }
      void SetMin(const snVector3f & in_Min) { mMin = in_Min; mIsValidMin = true; }
      void SetMax(const snVector3f & in_Max) { mMax = in_Max; mIsValidMax = true; }
      void Extend(float in_Extension);

      // mergers
      void MergeMin( const snVector3f & in_Min );
      void MergeMax( const snVector3f & in_Max );
      void Merge(const snVector3f & in_V) { MergeMin(in_V); MergeMax(in_V); }
      void Merge(const snBboxf & in_Other);

      // manipulation
      void Scale(const snVector3f & in_Scaling);
      void Rotate(const snQuaternionf & in_Rotation);
      void Move(const snVector3f & in_Movement);

      // intersection
      bool IsPointInside(const snVector3f & in_Pos) const;
      bool GetIntersectionWithBbox(const snBboxf & in_BBox, snBboxf * out_pResult = NULL);
   };

   typedef snBboxf * snpBboxf;
   typedef std::vector<snBboxf> snBboxfVec;
   typedef std::vector<snpBboxf> snpBboxfVec;
}
#endif

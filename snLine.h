#ifndef __SNLINE__
#define __SNLINE__

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

/**
 * snEssence is the shared namespace between all classes within the snEssence library.
 */
namespace snEssence
{
   /**
    * A 3d line represented by origin and direction
    */
   class snLine : public snSegment
   {
   protected:
      snVector3f mDirection;

   public:

      // constructors
      snLine();
      snLine(const snVector3f & in_Origin, const snVector3f & in_Direction);

      // getters
      virtual snVector3f GetOrigin() const { return GetStart(); }
      virtual snVector3f GetDirection() const { return mDirection; }
      virtual snVector3f GetConnection() const { return mDirection; }
      virtual float GetLength() const { return 1.0; }

      // setters
      void Set(const snVector3f & in_Origin, const snVector3f & in_Direction);

      // intersection
      virtual bool GetIntersectionWithBBox(const snBboxf & in_Bbox, snVector3fVec * out_pResult = NULL) const;
   };

   typedef snLine * snpLine;
   typedef std::vector<snLine> snLineVec;
   typedef std::vector<snpLine> snpLineVec;
}

#endif

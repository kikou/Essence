#ifndef __SNRAY__
#define __SNRAY__

#include "snLine.h"

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
    * A 3d ray represented by origin and direction
    */
   class snRay : public snLine
   {
   public:

      // constructors
      snRay();
      snRay(const snVector3f & in_Origin, const snVector3f & in_Direction);

      // intersection
      virtual bool GetIntersectionWithBBox(const snBboxf & in_Bbox, snVector3fVec * out_pResult = NULL) const;
   };

   typedef snRay * snpRay;
   typedef std::vector<snRay> snRayVec;
   typedef std::vector<snpRay> snpRayVec;
}

#endif

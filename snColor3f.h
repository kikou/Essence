#ifndef __SNCOLOR3F__
#define __SNCOLOR3F__

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
    * A class implementing a basic 3 channel color using floats.
    */
   class snColor3f
   {
   private:
      float rgb[3]; /// Float array holding red, green, blue and alpha.
   public:

      snColor3f();
      snColor3f(const snColor3f & in_Other);
      snColor3f(float r, float g, float b);
      snColor3f(int r, int g, int b);
      snColor3f(float * in_pFloat);

      // getters
      float GetR() const;
      float GetG() const;
      float GetB() const ;
      /// Get the average of r,g and b.
      float GetBrightness() const;
      float * GetArray();

      // setters
      void Set(const snColor3f & in_Other);
      void SetR(float in_R);
      void SetG(float in_G);
      void SetB(float in_B);
      void Set(float r, float g, float b);

      /// Sets all red, green and blue to the value of f.
      void SetAsGrey( float f);
   };

   typedef snColor3f * snpColor3f;
   typedef std::vector<snColor3f> snColor3fVec;
   typedef std::vector<snpColor3f> snpColor3fVec;
}
#endif

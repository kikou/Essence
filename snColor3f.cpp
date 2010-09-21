#include "snColor3f.h"

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


//=============================================================================================

// CONSTRUCTORS



snColor3f::snColor3f()

{

	rgb[0] = rgb[1] = rgb[2] = 0.0;

}



snColor3f::snColor3f(const snColor3f & in_Other)

{

	for(long i=0;i<3;i++)

		rgb[i] = in_Other.rgb[i];

}



snColor3f::snColor3f(float r, float g, float b)

{

	rgb[0] = r;

	rgb[1] = g;

	rgb[2] = b;

}



snColor3f::snColor3f(int r, int g, int b)

{

	rgb[0] = float(r) / 255.0f;

	rgb[1] = float(g) / 255.0f;

	rgb[2] = float(b) / 255.0f;

}



snColor3f::snColor3f(float * in_pFloat)

{

	rgb[0] = in_pFloat[0];

	rgb[1] = in_pFloat[1];

	rgb[2] = in_pFloat[2];

}



//=============================================================================================

// GETTERS



float snColor3f::GetR() const

{

	return rgb[0];

}



float snColor3f::GetG() const

{

	return rgb[1];

}



float snColor3f::GetB() const

{

	return rgb[2];

}



float * snColor3f::GetArray()

{

	return rgb;

}



float snColor3f::GetBrightness() const

{

	return ( rgb[0] + rgb[1] + rgb[2] ) / 3.0f;

}



//=============================================================================================

// SETTERS



void snColor3f::Set(const snColor3f & in_Other)

{

	for(long i=0;i<3;i++)

		rgb[i] = in_Other.rgb[i];

}



void snColor3f::SetR(float in_R)

{

	rgb[0] = in_R;

}



void snColor3f::SetG(float in_G)

{

	rgb[1] = in_G;

}



void snColor3f::SetB(float in_B)

{

	rgb[2] = in_B;

}



void snColor3f::Set(float r, float g, float b)

{

	rgb[0] = r;

	rgb[1] = g;

	rgb[2] = b;

}



void snColor3f::SetAsGrey( float f )

{

	rgb[0] = rgb[1] = rgb[2] = f;

}

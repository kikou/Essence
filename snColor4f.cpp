#include "snColor4f.h"

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



snColor4f::snColor4f()

{

	rgba[0] = rgba[1] = rgba[2] = rgba[3] = 0.0;

}



snColor4f::snColor4f(const snColor4f & in_Other)

{

	for(long i=0;i<4;i++)

		rgba[i] = in_Other.rgba[i];

}



snColor4f::snColor4f(float r, float g, float b, float a)

{

	rgba[0] = r;

	rgba[1] = g;

	rgba[2] = b;

	rgba[3] = a;

}



snColor4f::snColor4f(int r, int g, int b)

{

	rgba[0] = float(r) / 255.0f;

	rgba[1] = float(g) / 255.0f;

	rgba[2] = float(b) / 255.0f;

	rgba[3] = 1.0f;

}



snColor4f::snColor4f(float * in_pFloat)

{

	rgba[0] = in_pFloat[0];

	rgba[1] = in_pFloat[1];

	rgba[2] = in_pFloat[2];

	rgba[3] = in_pFloat[3];

}



//=============================================================================================

// GETTERS



float snColor4f::GetR() const

{

	return rgba[0];

}



float snColor4f::GetG() const

{

	return rgba[1];

}



float snColor4f::GetB() const

{

	return rgba[2];

}



float snColor4f::GetA() const

{

	return rgba[3];

}



float * snColor4f::GetArray()

{

	return rgba;

}



float snColor4f::GetBrightness() const

{

	return ( rgba[0] + rgba[1] + rgba[2] ) / 3.0f;

}



//=============================================================================================

// SETTERS



void snColor4f::Set(const snColor4f & in_Other)

{

	for(long i=0;i<4;i++)

		rgba[i] = in_Other.rgba[i];

}



void snColor4f::SetR(float in_R)

{

	rgba[0] = in_R;

}



void snColor4f::SetG(float in_G)

{

	rgba[1] = in_G;

}



void snColor4f::SetB(float in_B)

{

	rgba[2] = in_B;

}



void snColor4f::SetA(float in_A)

{

	rgba[3] = in_A;

}



void snColor4f::Set(float r, float g, float b, float a)

{

	rgba[0] = r;

	rgba[1] = g;

	rgba[2] = b;

	rgba[3] = a;

}



void snColor4f::SetAsGrey( float f, float a )

{

	rgba[0] = rgba[1] = rgba[2] = f;

	rgba[3] = a;

}

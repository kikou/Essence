#include "snError.h"

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

snError::snError(eError_Status in_eStatus)

{

	mStatus = in_eStatus;

}



snError::snError(const snString & in_strMessage, eError_Status in_eStatus)

{

	mMessage = in_strMessage;

	mStatus = in_eStatus;
}



snError::~snError()

{

}


snError::snError(const snError & in_Other)
{
   mMessage = in_Other.mMessage;
   mStatus = in_Other.mStatus;
}

snString snError::GetStatusMessage() const
{
   switch(mStatus)
   {
      case eError_None:
      case eError_Success:
      {
         return "SUCCESS";
      }
      case eError_UserAbort:
      {
         return "ABORTED";
      }
      case eError_NullPtr:
      {
         return "NULLPTR";
      }
      case eError_Fail:
      {
         return "FAILED";
      }
      case eError_InvalidArg:
      {
         return "INVARG";
      }
      case eError_FileIO:
      {
         return "FILEIO";
      }
      case eError_Unexpected:
      {
         return "UNEXP";
      }
      case eError_NotImpl:
      {
         return "NOTIMPL";
      }
      case eError_NbElements:
      {
         return "UNDEF";
      }
   }
   return snString();
}


snString snError::GetComposedMessage(const snString & in_StrCaller) const
{
   snString result;
   if(!in_StrCaller.IsEmpty())
   {
      result.Compose("[%s]%s: %s",in_StrCaller.GetAsChar(),GetStatusMessage().GetAsChar(),GetMessage().GetAsChar());
      return result;
   }
   result.Compose("%s: %s",GetStatusMessage().GetAsChar(),GetMessage().GetAsChar());
   return result;
}

void snError::Print(const snString & in_StrCaller) const
{
   printf(GetComposedMessage(in_StrCaller).GetAsChar());
}

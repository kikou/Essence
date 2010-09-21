#ifndef __SNERROR__
#define __SNERROR__

#include "snString.h"

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
   /// The status of the error
   enum eError_Status
   {
      eError_None,         /**< No error at all, invalid enum value. */
      eError_Success,      /**< Succeeded the execution. */
      eError_UserAbort,    /**< Aborted by the user. */
      eError_NullPtr,      /**< Discovered a NULL ptr. */
      eError_Fail,         /**< Failed during the execution. */
      eError_InvalidArg,   /**< Invalid argument specified. */
      eError_Unexpected,   /**< Unexpected code behaviour encountered. */
      eError_FileIO,       /**< Failure during File Input / Output. */
      eError_NotImpl,      /**< Method not yet implemented. */
      eError_NbElements    /**< Max Value for the Enum */
   };

   /**
    * A class representing an error description.
    */
   class snError
   {
   private:

      snString mMessage;      /// The error message.
      eError_Status mStatus;  /// The status of the error.

   public:

      /// Default constructor
      snError(eError_Status in_eStatus = eError_None);
      /// Constructor taking a message as well as a default error status.
      snError(const snString & in_strMessage, eError_Status in_eStatus = eError_Fail);
      ~snError();

      /// Copy constructor
      snError(const snError & in_Other);

      // getters
      eError_Status GetStatus() const { return mStatus; }
      snString GetStatusMessage() const;
      snString GetMessage() const { return mMessage; }
      snString GetComposedMessage(const snString & in_StrCaller) const;

      /// Do a STD::OUT Print of the error.
      void Print(const snString & in_StrCaller) const;

      /// Compare an error instance to a status directly.
      bool operator ! () const { return GetStatus() != eError_None && GetStatus() != eError_Success; }
   };
}
#endif

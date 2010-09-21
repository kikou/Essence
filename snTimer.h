#ifndef __SNTIMER__
#define __SNTIMER__

#include "snString.h"

#ifdef WIN32
   #include <windows.h>
   #define longinttype __int64
#else
   #include <sys/time.h>
   #define longinttype timeval
#endif

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
    * A cross-OS class for measuring times in miliseconds for code complexity improvements.
    */
   class snTimer
   {
   private:

      longinttype m_iStartTime;
      longinttype m_iEndTime;
      double m_dFrequeny;
      bool m_bRunning;
      snString m_strMessage;

   public:

      /// Default constructor, starts the timer also.
      snTimer();
      /// Default constructor, sets the message and starts the timer also.
      snTimer(const snString & in_strMessage);
      /// Default destructor. If the timer is still running it will print the
      /// timer message to the STD::OUT.
      ~snTimer();

      /// Get the time spent in miliseconds.
      long GetMiliSeconds() const;
      /// Get a string message including the miliseconds spent.
      snString GetTimerMessage() const;

      // setters
      void SetTimerMessage(const snString & in_strMessage);

      /// Start the timer another time.
      void Start();
      /// Stop the timer (should not be necessary).
      void Stop();
   };
}
#endif

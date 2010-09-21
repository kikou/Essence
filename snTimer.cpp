#include "snTimer.h"

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

snTimer::snTimer()

{

	m_bRunning = false;

	Start();
}



snTimer::snTimer(const snString & in_strMessage)

{

	m_strMessage = in_strMessage;

	m_bRunning = false;

	Start();

}



snTimer::~snTimer()

{

	if(m_bRunning && m_strMessage.GetLength()>0)

		printf("%s\n",GetTimerMessage().GetAsChar());

}



void snTimer::Start()

{

	if(!m_bRunning)

	{

#ifdef WIN32

		QueryPerformanceCounter((LARGE_INTEGER*)&m_iStartTime);

      longinttype l_iFrequency;

		QueryPerformanceFrequency((LARGE_INTEGER*)&l_iFrequency);

      m_dFrequeny = (double)l_iFrequency;

#else

      gettimeofday(&m_iStartTime,NULL);

      m_dFrequeny = 1000000.0;

#endif

		m_bRunning = true;

	}

}



void snTimer::Stop()

{

#ifdef WIN32

	QueryPerformanceCounter((LARGE_INTEGER*)&m_iEndTime);

#else

      gettimeofday(&m_iEndTime,NULL);

#endif

	m_bRunning = false;

}



long snTimer::GetMiliSeconds() const

{

	double l_dDiff;

	if(m_bRunning)

	{

		longinttype l_iCurrent;

#ifdef WIN32

   	QueryPerformanceCounter((LARGE_INTEGER*)&l_iCurrent);

		l_dDiff = (double)(l_iCurrent - m_iStartTime);

#else

      gettimeofday(&l_iCurrent,NULL);

      l_dDiff = 1000000 * (l_iCurrent.tv_sec - m_iStartTime.tv_sec) + (l_iCurrent.tv_usec - m_iStartTime.tv_usec);

#endif

	}

	else

	{

#ifdef WIN32

		l_dDiff = (double)(m_iEndTime - m_iStartTime);

#else

      l_dDiff = 1000000 * (m_iEndTime.tv_sec - m_iStartTime.tv_sec) + (m_iEndTime.tv_usec - m_iStartTime.tv_usec);
#endif

	}

	l_dDiff = floor(1000.0 * l_dDiff / m_dFrequeny);



	return (long)l_dDiff;

}



snString snTimer::GetTimerMessage() const

{

	snString result;

	if(m_strMessage.GetLength() > 0)

	{

		result.Compose("snTimer: %s took %d ms.",m_strMessage.GetAsChar(),GetMiliSeconds());

	}

	else

	{

		result.Compose("%d ms",GetMiliSeconds());

	}

	return result;

}



void snTimer::SetTimerMessage(const snString & in_strMessage)

{

	m_strMessage.Set(in_strMessage);

}




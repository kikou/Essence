#include "snString.h"
#include <stdarg.h>
#include <ctype.h>
#include <memory.h>
#include <cstring>

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

char * snString::sBuffer = NULL;
long snString::sInstanceCount = 0l;

snString::snString()
{
	sInstanceCount++;
	m_pBuffer = NULL;
	m_pBufferW = NULL;
	m_iLength = 0;
	m_iSize = 0;
   Allocate();
}

snString::~snString()
{
	sInstanceCount--;
	m_iLength = 0;
	m_iSize = 0;

	Deallocate();
}

void snString::Deallocate()
{
	if(m_pBuffer != NULL)
		free(m_pBuffer);
	m_pBuffer = NULL;
	if(m_pBufferW != NULL)
		free(m_pBufferW);
	m_pBufferW = NULL;

	// delete the static buffer
	if(sInstanceCount==0l)
	{
      free(sBuffer);
      sBuffer = NULL;
	}
}

void snString::Allocate(bool in_bCopy)
{
   // allocate the static buffer
   if(sBuffer==NULL && sInstanceCount > 0)
      sBuffer = (char*)malloc(g_lBuffersize);

	if(m_iLength > 0)
	{
		if(m_pBuffer == NULL)
		{
			m_iSize = (m_iLength % g_lChunkSize) ? (m_iLength - (m_iLength % g_lChunkSize) + 1) * g_lChunkSize : m_iLength;
			m_pBuffer = (char*)malloc(sizeof(char) * (m_iSize + 1));
		}
		else
		{
			long l_iNewSize = (m_iLength % g_lChunkSize) ? m_iLength - (m_iLength % g_lChunkSize) + g_lChunkSize : m_iLength;
			if(l_iNewSize != m_iSize)
			{
				char * l_pBuffer = (char*)malloc(sizeof(char) * (l_iNewSize + 1));
				if(in_bCopy)
               memcpy(l_pBuffer,m_pBuffer,m_iSize > l_iNewSize ? l_iNewSize : m_iSize);
				free(m_pBuffer);
				m_pBuffer = l_pBuffer;
				m_iSize = l_iNewSize;
			}
		}
		m_pBuffer[m_iLength] = (char)0;
	}
}

snString::snString(const snString & in_strOther)
{
	sInstanceCount++;
	m_iLength = in_strOther.m_iLength;
	m_pBuffer = NULL;
	m_pBufferW = NULL;
   Allocate();
	if(m_iLength > 0)
	{
		memcpy(m_pBuffer,in_strOther.m_pBuffer,sizeof(char) * (m_iLength+1));
	}
	else
		m_pBuffer = NULL;

}

snString::snString(const char & in_Char)
{
	sInstanceCount++;
   m_iLength = 1;
	m_pBuffer = NULL;
	m_pBufferW = NULL;
   Allocate();
   m_pBuffer[0] = in_Char;
}


snString::snString(const char * in_pChar)
{
	sInstanceCount++;
	m_pBufferW = NULL;
	if(in_pChar!=NULL)
	{
		m_iLength = (long)strlen(in_pChar);
		m_pBuffer = NULL;
		if(m_iLength > 0)
		{
			Allocate();
			memcpy(m_pBuffer,in_pChar,sizeof(char) * (m_iLength+1));
		}
		else
			m_pBuffer = NULL;
	}
	else
	{
		m_iLength = 0;
		m_pBuffer = NULL;
	}
}

snString::snString(const wchar_t * in_pWChar)
{
	sInstanceCount++;
	m_pBufferW = NULL;
	if(in_pWChar!=NULL)
	{
		m_iLength = (long)wcslen(in_pWChar);
		m_pBuffer = NULL;
		if(m_iLength > 0)
		{
			Allocate();
			for(long i=0;i<m_iLength;i++)
				m_pBuffer[i] = (char)in_pWChar[i];
		}
		else
			m_pBuffer = NULL;
	}
	else
	{
		m_iLength = 0;
		m_pBuffer = NULL;
	}
}

char * snString::GetAsChar() const
{
	return m_pBuffer;
}

std::string snString::GetAsStdString() const
{
	return std::string(m_pBuffer);
}

wchar_t * snString::GetAsWChar()
{
	if(m_pBufferW != NULL)
		free(m_pBufferW);
	m_pBufferW = (wchar_t*)malloc(sizeof(wchar_t) * (m_iLength + 1));
	for(long i=0;i<m_iLength+1;i++) // +1 because there is still the "/0" at the end of the string
		m_pBufferW[i] = m_pBuffer[i];

	return m_pBufferW;
}

char snString::GetCharAt(long in_iIndex) const
{
	if(in_iIndex < 0 || in_iIndex > m_iLength-1)
		return (char)0;
	return m_pBuffer[in_iIndex];
}

char snString::GetReverseCharAt(long in_iIndex) const
{
	if(in_iIndex < 0 || in_iIndex > m_iLength-1)
		return (char)0;
	return m_pBuffer[m_iLength - in_iIndex - 1];
}

void snString::Compose(snString in_strTemplate, ...)
{
	va_list ap;
	va_start(ap, in_strTemplate);
	vsprintf(sBuffer,in_strTemplate.GetAsChar(),ap);
	m_iLength = (long)strlen(sBuffer);
	if(m_iLength > 0)
	{
		Allocate();
		memcpy(m_pBuffer,sBuffer,sizeof(char) * (m_iLength+1));
	}
	else
	{
		if(m_pBuffer != NULL)
			free(m_pBuffer);
		m_pBuffer = NULL;
	}
}

void snString::Compose(const char * in_cTemplate, ...)
{
	if(in_cTemplate != NULL)
	{
		va_list ap;
		va_start(ap, in_cTemplate);
		vsprintf(sBuffer,in_cTemplate,ap);
		m_iLength = (long)strlen(sBuffer);
		if(m_iLength > 0)
		{
			Allocate();
			memcpy(m_pBuffer,sBuffer,sizeof(char) * (m_iLength+1));
		}
		else
		{
			if(m_pBuffer != NULL)
				free(m_pBuffer);
			m_pBuffer = NULL;
		}
	}
	else
	{
		m_iLength = 0;
	}
}

long snString::GetAsLong() const
{
	return atol(GetAsChar());
}

float snString::GetAsFloat() const
{
	return (float)atof(GetAsChar());
}

bool snString::operator ==(const snString & in_strOther) const
{
	return *this == in_strOther.GetAsChar();
}

bool snString::operator !=(const snString & in_strOther) const
{
	return !(*this == in_strOther);
}

bool snString::operator ==(const char * in_cOther) const
{
	if(in_cOther == NULL)
	{
		return (m_iLength == 0);
	}
	else
	{
		if(m_iLength != (signed int)strlen(in_cOther))
			return false;

		for(long i=0;i<m_iLength;i++)
		{
			if(in_cOther[i] != m_pBuffer[i])
				return false;
		}
	}
	return true;
}

bool snString::operator !=(const char * in_cOther) const
{
	return !(*this == in_cOther);
}

snString & snString::operator = (const snString & in_strOther)
{
   Set(in_strOther);
	return *this;
}

snString & snString::operator = (const char * in_cOther)
{
	if(in_cOther == NULL)
	{
		Empty();
	}
	else
	{
		m_iLength = (long)strlen(in_cOther);
		if(m_iLength > 0)
		{
			Allocate();
			memcpy(m_pBuffer,in_cOther,sizeof(char) * (m_iLength+1));
		}
	}
	return *this;
}

snString snString::operator + (const snString & in_strOther) const
{
	snString l_strResult(*this);
	l_strResult.Append(in_strOther);
	return l_strResult;
}

snString snString::operator + (const char * in_cOther) const
{
	if(in_cOther == NULL)
		return *this;
	snString l_strResult(*this);
	l_strResult.Append(in_cOther);
	return l_strResult;
}

void snString::operator += (const snString & in_strOther)
{
	Append(in_strOther);
}

void snString::operator += (const char * in_cOther)
{
	Append(in_cOther);
}

bool snString::operator < (const snString & in_strOther) const
{
	if(GetLength() < in_strOther.GetLength())
		return true;
	else if(GetLength() > in_strOther.GetLength())
		return false;

	// here we know the string are of the same length!
	for(long i=0;i<GetLength();i++)
	{
		if(GetCharAt(i) != in_strOther.GetCharAt(i))
			return GetCharAt(i) < in_strOther.GetCharAt(i);
	}
	return false;
}

bool snString::operator > (const snString & in_strOther) const
{
	return !(*this < in_strOther);
}

snString snString::GetLeft(long in_iCount) const
{
	if(in_iCount <= 0)
		return snString();
	if(in_iCount > m_iLength)
		return *this;

	snString l_strResult;
	l_strResult.m_iLength = in_iCount;
	l_strResult.Allocate();
	memcpy(l_strResult.m_pBuffer,m_pBuffer,sizeof(char) * in_iCount);
	return l_strResult;
}

snString snString::GetMiddle(long in_iStart, long in_iCount) const
{
	if(in_iStart < 0 || in_iStart >= m_iLength || in_iCount <= 0)
		return snString();
	if(in_iCount > m_iLength)
		return *this;

	if(in_iStart+in_iCount >= m_iLength)
		return GetRight(m_iLength - in_iStart);

	snString l_strResult;
	l_strResult.m_iLength = in_iCount;
	l_strResult.Allocate();
	memcpy(l_strResult.m_pBuffer,&(m_pBuffer[in_iStart]),sizeof(char) * in_iCount);
	return l_strResult;
}

snString snString::GetRight(long in_iCount) const
{
	if(in_iCount <= 0)
		return snString();
	if(in_iCount > m_iLength)
		return *this;

	snString l_strResult;
	l_strResult.m_iLength = in_iCount;
	l_strResult.Allocate();
	memcpy(l_strResult.m_pBuffer,&(m_pBuffer[m_iLength-in_iCount]),sizeof(char) * in_iCount);
	return l_strResult;
}

snString snString::GetReverse() const
{
	snString l_strResult;
	l_strResult.m_iLength = m_iLength;
	l_strResult.Allocate();
	for(long i=0;i<m_iLength;i++)
		l_strResult.m_pBuffer[i] = m_pBuffer[m_iLength-i-1];
	return l_strResult;
}

long snString::GetFirstIndexOf(char in_cToFind,long in_iStartIndex) const
{
	if(in_iStartIndex < m_iLength)
	{
		for(long i = in_iStartIndex; i < m_iLength; i++)
		{
			if(GetCharAt(i) == in_cToFind)
				return i;
		}
	}
	return -1;
}

long snString::GetLastIndexOf(char in_cToFind) const
{
	for(long i = m_iLength-1; i >= 0; i--)
	{
		if(GetCharAt(i) == in_cToFind)
			return i;
	}
	return -1;
}

long snString::GetFirstIndexOfNot(char in_cNotToFind,long in_iStartIndex) const
{
	if(in_iStartIndex < m_iLength)
	{
		for(long i = in_iStartIndex; i < m_iLength; i++)
		{
			if(GetCharAt(i) != in_cNotToFind)
				return i;
		}
	}
	return -1;
}

long snString::GetLastIndexOfNot(char in_cNotToFind) const
{
	for(long i = m_iLength-1; i >= 0; i--)
	{
		if(GetCharAt(i) != in_cNotToFind)
			return i;
	}
	return -1;
}

void snString::Set(const snString & in_strOther)
{
	m_iLength = in_strOther.m_iLength;
	if(m_pBuffer != NULL)
		free(m_pBuffer);
	m_pBuffer = NULL;

	if(m_iLength > 0)
	{
		Allocate();
		memcpy(m_pBuffer,in_strOther.m_pBuffer,m_iLength);
	}
}

void snString::Set(const char * in_cOther)
{
	if(in_cOther == NULL)
		m_iLength = 0;
	else
	{
		m_iLength = (long)strlen(in_cOther);
		if(m_iLength > 0)
		{
			Allocate();
			memcpy(m_pBuffer,in_cOther,m_iLength);
		}
	}
}

void snString::TruncateLeft(long in_iCount)
{
	Set(GetRight(m_iLength-in_iCount));
}

void snString::TruncateMiddle(long in_iStart, long in_iCount)
{
	Set(GetLeft(in_iStart) + GetRight(m_iLength - in_iStart - in_iCount));
}

void snString::TruncateLeftRight(long in_iLeft, long in_iRight)
{
   Set(GetMiddle(in_iLeft,GetLength()-in_iRight-in_iLeft));
}


void snString::TruncateRight(long in_iCount)
{
	Set(GetLeft(m_iLength-in_iCount));
}

void snString::Reverse()
{
	Set(GetReverse());
}

void snString::Prepend(const snString & in_strPrefix)
{
	snString myself(*this);
	m_iLength = in_strPrefix.m_iLength + myself.m_iLength;
	Allocate();
	memcpy(m_pBuffer,in_strPrefix.m_pBuffer,sizeof(char) * in_strPrefix.m_iLength);
	memcpy(&(m_pBuffer[in_strPrefix.m_iLength]),myself.m_pBuffer,sizeof(char) * myself.m_iLength);
}

void snString::Append(const snString & in_strSuffix)
{
	Append(in_strSuffix.GetAsChar(),in_strSuffix.GetLength());
}

void snString::Append(const char * in_cSuffix, size_t in_Size)
{
	if(in_cSuffix==NULL)
		return;
	long l_iPreviousLength = m_iLength;
	long l_iSuffixLength = (long)in_Size;
	if(l_iSuffixLength == 0l)
      l_iSuffixLength = (long)strlen(in_cSuffix);
	m_iLength += l_iSuffixLength;
	Allocate();
	memcpy(&(m_pBuffer[l_iPreviousLength]),in_cSuffix,l_iSuffixLength);
}

void snString::Empty()
{
	m_iLength = 0;
	Deallocate();
}

void snString::Fill(const snString & in_strElement, const snString & in_strDelim, long in_iCount)
{
	m_iLength = in_strElement.m_iLength * in_iCount + in_strDelim.m_iLength * (in_iCount - 1);
	Allocate();
	long l_iOffset = 0;
	for(long i=0;i<in_iCount;i++)
	{
		memcpy(&(m_pBuffer[l_iOffset]),in_strElement.m_pBuffer,in_strElement.m_iLength);
		l_iOffset += in_strElement.m_iLength;
		if(i!=in_iCount-1)
		{
         memcpy(&(m_pBuffer[l_iOffset]),in_strDelim.m_pBuffer,in_strDelim.m_iLength);
         l_iOffset += in_strDelim.m_iLength;
      }
	}
}

void snString::Fill(const snString & in_strElement, long in_iCount)
{
	m_iLength = in_strElement.m_iLength * in_iCount;
	Allocate();
	long l_iOffset = 0;
	for(long i=0;i<in_iCount;i++)
	{
		memcpy(&(m_pBuffer[l_iOffset]),in_strElement.m_pBuffer,in_strElement.m_iLength);
		l_iOffset += in_strElement.m_iLength;
	}
}

bool snString::Replace(char in_cToFind, char in_cReplacement, long in_iOccurences)
{
	bool l_bResult = false;
	long iOcc = in_iOccurences;
	for(long i=0;i<m_iLength;i++)
	{
		if(m_pBuffer[i] == in_cToFind)
		{
			l_bResult = true;
			m_pBuffer[i] = in_cReplacement;
			iOcc--;
			if(iOcc==0)
				break;
		}
	}
	return l_bResult;
}

bool snString::Replace(char in_cToFind, snString in_strReplacement, long in_iOccurences)
{
	bool l_bResult = false;
	long iOcc = in_iOccurences;

	if(in_strReplacement.GetLength()==1)
	{
		for(long i=0;i<m_iLength;i++)
		{
			if(m_pBuffer[i] == in_cToFind)
			{
				m_pBuffer[i] = in_strReplacement.GetCharAt();
				iOcc--;
				l_bResult = true;
				if(iOcc==0)
					break;
			}
		}
	}
	else
	{
		long iCount = CountChar(in_cToFind);
		if(iCount==0)
			return false;
		l_bResult = true;

		// we can only replace what's there
		if(iOcc == -1)
			iOcc = iCount;
		else if(iOcc > iCount)
			iOcc = iCount;
		else if(iOcc == 0)
			return false;

		snString l_strMySelf(*this);

		m_iLength += iOcc * (in_strReplacement.GetLength()-1);
		Allocate();

		long l_iOffset=0;
		for(long i=0;i<l_strMySelf.GetLength();i++)
		{
			if(l_strMySelf.GetCharAt(i) == in_cToFind && iOcc>0)
			{
				for(long j=0;j<in_strReplacement.GetLength();j++)
					m_pBuffer[l_iOffset++] = in_strReplacement.GetCharAt(j);
				iOcc--;
			}
			else
				m_pBuffer[l_iOffset++] = l_strMySelf.GetCharAt(i);
		}
	}
	return l_bResult;
}

bool snString::Replace(snString in_strToFind, snString in_strReplacement, long in_iOccurences)
{
	bool l_bResult = false;
	long iOcc = in_iOccurences;

	// we can only replace what's there
	if(iOcc == 0)
		return false;

	long l_lOffset = 0;
	long l_lFindPos = Find(in_strToFind,l_lOffset);
	while(l_lFindPos != -1 && iOcc != 0)
	{
		Set(GetLeft(l_lFindPos) +  in_strReplacement + GetRight(GetLength() - l_lFindPos - in_strToFind.GetLength()));

		l_lOffset = l_lFindPos + in_strReplacement.GetLength();
		l_bResult = true;
		l_lFindPos = Find(in_strToFind,l_lOffset);
		iOcc--;

	}
	return l_bResult;
}

snString snString::GetLowerCase() const
{
	snString l_strResult(*this);
	l_strResult.LowerCase();
	return l_strResult;
}

snString snString::GetUpperCase() const
{
	snString l_strResult(*this);
	l_strResult.UpperCase();
	return l_strResult;
}

long snString::CountChar(char in_cToCount) const
{
	long l_iResult = 0;
	for(long i=0;i<m_iLength;i++)
	{
		if(m_pBuffer[i] == in_cToCount)
			l_iResult++;
	}
	return l_iResult;
}

long snString::Count(const snString & in_strToCount) const
{
	if(in_strToCount.IsEmpty() || m_iLength == 0)
		return 0;
	else if(in_strToCount.GetLength()==1)
		return CountChar(in_strToCount.GetCharAt());

	long l_lCount = 0;
	for(long i=0;i<m_iLength-in_strToCount.GetLength();i++)
	{
		if(GetMiddle(i,in_strToCount.GetLength()) == in_strToCount)
		{
			l_lCount++;
			i+=in_strToCount.GetLength()-1;
		}
	}
	return l_lCount;
}

void snString::LowerCase()
{
	for(long i=0;i<m_iLength;i++)
		m_pBuffer[i] = tolower(m_pBuffer[i]);
}

void snString::UpperCase()
{
	for(long i=0;i<m_iLength;i++)
		m_pBuffer[i] = toupper(m_pBuffer[i]);
}

void snString::Resize(long in_iSize)
{
	m_iLength = in_iSize;
	Allocate();
}


snStringArray snString::Split(char in_cDelimiter,bool in_bSkipQuotes) const
{
	snStringArray l_strResult;

	// check if the delimiter is a end string,
	// then we tackle a special case
	if(in_cDelimiter == (char)0)
	{
		l_strResult.resize((size_t)m_iLength);
		for(long i=0;i<m_iLength;i++)
			l_strResult[i] = GetMiddle(i,1);
	}
	else
	{
		long l_iOffset = 0;
		long l_iDelimPos = -1;
      while(true)
      {
         if(in_bSkipQuotes)
         {
            char quote = 0;
            bool backSlash = false;

            l_iDelimPos = -1;
            for(long i = l_iOffset; i < m_iLength; i++)
            {
               if(!backSlash)
               {
                  if(quote == 0)
                  {
                     if(GetCharAt(i)=='\"' || GetCharAt(i)=='\'')
                        quote = GetCharAt(i);
                  }
                  else if(GetCharAt(i)==quote)
                     quote = 0;
               }
               if(quote == 0)
               {
                  if(GetCharAt(i) == in_cDelimiter)
                  {
                     l_iDelimPos = i;
                     break;
                  }
               }

               backSlash = GetCharAt(i) == '\\';
            }
         }
         else
            l_iDelimPos = GetFirstIndexOf(in_cDelimiter,l_iOffset);
         if(l_iDelimPos == -1)
            break;
         l_strResult.push_back(GetMiddle(l_iOffset,l_iDelimPos - l_iOffset));
         l_iOffset = l_iDelimPos+1;
      }
		if(l_iOffset < m_iLength)
			l_strResult.push_back(GetRight(m_iLength - l_iOffset));
	}

	return l_strResult;
}

snStringArray snString::Partition(char in_cDelimiter, bool in_bSkipQuotes) const
{
	snStringArray l_strResult;
   long l_iDelimPos = -1;
	if(in_bSkipQuotes)
	{
      char quote = 0;
      for(long offset = 0;offset < m_iLength;offset++)
      {
         if(quote && m_pBuffer[offset]==quote)
         {
            quote = 0;
            continue;
         }

         if(quote == 0 && m_pBuffer[offset]==in_cDelimiter)
         {
            l_iDelimPos = offset;
            break;
         }

         if(m_pBuffer[offset]=='\'' || m_pBuffer[offset]=='\"')
            quote = m_pBuffer[offset];
      }
	}
   else
   {
      // just do a simple GetFirstIndexOf
      l_iDelimPos = GetFirstIndexOf(in_cDelimiter);
   }

   if(-1 != l_iDelimPos)
   {
      l_strResult.push_back(GetLeft(l_iDelimPos));
      l_strResult.push_back(GetRight(GetLength() - l_iDelimPos -1));
   }
   else
   {
      l_strResult.push_back(*this);
   }
	return l_strResult;
}

snStringArray snString::RPartition(char in_cDelimiter, bool in_bSkipQuotes) const
{
	snStringArray l_strResult;
   long l_iDelimPos = -1;
   if(in_bSkipQuotes)
   {
      char quote = 0;
      for(int offset = m_iLength-1;offset >= 0;offset--)
      {
         if(quote && m_pBuffer[offset]==quote)
         {
            quote = 0;
            continue;
         }

         if(quote == 0 && m_pBuffer[offset]==in_cDelimiter)
         {
            l_iDelimPos = offset;
            break;
         }

         if(m_pBuffer[offset]=='\'' || m_pBuffer[offset]=='\"')
            quote = m_pBuffer[offset];
      }
   }
   else
   {
      // just do a simple GetLastIndexOf
      l_iDelimPos = GetLastIndexOf(in_cDelimiter);
   }

   if(-1 != l_iDelimPos)
   {
      l_strResult.push_back(GetLeft(l_iDelimPos));
      l_strResult.push_back(GetRight(GetLength() - l_iDelimPos -1));
   }
   else
   {
      l_strResult.push_back(*this);
   }

	return l_strResult;
}

void snString::Join(const snStringArray & in_pStringArray, char in_cDelimiter)
{
	// check if the delimiter is a string end,
	// which means we are looking at a special case
	bool l_bHasDelim = in_cDelimiter != char(0);
	long l_iSize = l_bHasDelim ? (long)(in_pStringArray.size() - 1) : 0l;
	for(size_t i=0;i<in_pStringArray.size();i++)
		l_iSize += in_pStringArray[i].GetLength();

	m_iLength = l_iSize;
	Allocate();
	long l_iOffset = 0;
	for(size_t i=0;i<in_pStringArray.size();i++)
	{
		memcpy(&(m_pBuffer[l_iOffset]),in_pStringArray[i].m_pBuffer,in_pStringArray[i].GetLength());
		l_iOffset += in_pStringArray[i].GetLength();
		if(l_bHasDelim && i < (in_pStringArray.size() - 1))
		{
			m_pBuffer[l_iOffset] = in_cDelimiter;
			l_iOffset++;
		}
	}
}

void snString::Remove(char in_cToRemove)
{
	long l_iLength = m_iLength;
	for(long i=0,j=0;i<l_iLength;i++,j++)
	{
		if(m_pBuffer[i]==in_cToRemove)
		{
			m_iLength--;
			j--;
		}
		else if(i!=j)
		{
			m_pBuffer[j] = m_pBuffer[i];
		}
	}
	m_pBuffer[m_iLength] = (char)0;
}

bool snString::Save(const snString & in_strFileName,const snString & in_strMode) const
{
	FILE * l_pFile;
	l_pFile = fopen(in_strFileName.GetAsChar(),in_strMode.GetAsChar());
	if(l_pFile==NULL)
		return false;
	fwrite(m_pBuffer,1,m_iLength,l_pFile);
	fclose(l_pFile);
	return true;
}

bool snString::Load(const snString & in_strFileName)
{
	Empty();
	FILE * l_pFile;
	l_pFile = fopen(in_strFileName.GetAsChar(),"rb");
	if(l_pFile==NULL)
		return false;
	fseek( l_pFile, 0, SEEK_END );
	m_iLength = ftell( l_pFile );
	Allocate();
	fseek( l_pFile, 0, SEEK_SET );
	fread( m_pBuffer,1,m_iLength,l_pFile);
	fclose(l_pFile);

	// ANSI compatibility
	Remove((char)13);

	return true;
}


// returns the index position of the string found, returns -1 if not found
long snString::Find(const snString & in_strToFind, long in_iStartIndex) const
{
	// walk the string and try to find the smaller piece!
	snString l_strWork;
	for(long i=in_iStartIndex;i<GetLength() - in_strToFind.GetLength()+1;i++)
	{
		l_strWork = GetMiddle(i,in_strToFind.GetLength());
		if(l_strWork == in_strToFind)
			return i;
	}
	return -1;
}

// checks if open and close brace match
bool snString::ContainsTokens(char in_cOpen, char in_cClose) const
{
   long openBrace = CountChar(in_cOpen);
   long closeBrace = CountChar(in_cClose);
   if(openBrace > 0 && closeBrace > 0)
      return true;
   return false;
}

bool snString::AddToken(const snString & in_TokenName, const snString & in_TokenValue)
{
   snString tokenName(in_TokenName.GetLowerCase());
   if(m_sTokens.find(tokenName) == m_sTokens.end())
   {
      m_sTokens.insert(snStringPair(tokenName,in_TokenValue));
      return true;
   }
   return false;
}


// searches for tokens inside of the string and returns a cleaned version
snString snString::GetTokenized(snStringMap * in_pTokens, char in_cOpen, char in_cClose)
{
   snStringMap * tokens;
   if(in_pTokens == NULL)
      tokens = &m_sTokens;
   else
      tokens = in_pTokens;

   // loop over all pieces of the string,
   // try to find tokens and replace them
   // accordingly
   snString result;
   bool inToken = false;
   snString token;
   for(long i=0;i<GetLength();i++)
   {
      if(inToken)
      {
         if(GetCharAt(i)==in_cClose)
         {
            snStringMapIt it = tokens->find(token.GetLowerCase());
            if(it == tokens->end())
            {
               token.Prepend(in_cOpen);
               token.Append(in_cClose);
               result.Append(token);
            }
            else
            {
               result.Append(it->second);
            }
            token.Empty();
            inToken = false;
         }
         else
            token.Append(GetSubString(i,1));
      }
      else
      {
         if(GetCharAt(i)==in_cOpen)
            inToken = true;
         else
            result.Append(GetSubString(i,1));
      }
   }

   return result;
}

bool snString::StartsWith(const snString & in_Prefix) const
{
   return GetLeft(in_Prefix.GetLength()) == in_Prefix;
}

bool snString::EndsWith(const snString & in_Suffix) const
{
   return GetRight(in_Suffix.GetLength()) == in_Suffix;
}

void snString::Capitalize()
{
   if(IsEmpty())
      return;

   m_pBuffer[0] = toupper(m_pBuffer[0]);
}

snString snString::GetCapitalize() const
{
	snString l_strResult(*this);
	l_strResult.Capitalize();
	return l_strResult;
}

void snString::LStrip(char in_Char)
{
   if(m_iLength == 0)
      return;

   long offset = 0;
   while(GetCharAt(offset)==in_Char && offset < m_iLength)
      offset++;

   TruncateLeft(offset);
}

void snString::RStrip(char in_Char)
{
   if(m_iLength == 0)
      return;

   int offset = m_iLength-1;
   while(GetCharAt(offset)==in_Char && offset > -1)
      offset--;

   TruncateRight(offset);
}

void snString::Strip(char in_Char)
{
   if(m_iLength == 0)
      return;

   long loffset = 0;
   while(GetCharAt(loffset)==in_Char && loffset < m_iLength)
      loffset++;

   int roffset = m_iLength-1;
   while(GetCharAt(roffset)==in_Char && roffset > loffset)
      roffset--;

   Set(GetMiddle(loffset,roffset-loffset+1));
}

void snString::StripWhiteSpaces()
{
   if(m_iLength == 0)
      return;

   long loffset = 0;
   while((GetCharAt(loffset)==' ' || GetCharAt(loffset)=='\t') && loffset < m_iLength)
      loffset++;

   int roffset = m_iLength-1;
   while((GetCharAt(roffset)==' ' || GetCharAt(roffset)=='\t') && roffset > loffset)
      roffset--;

   Set(GetMiddle(loffset,roffset-loffset+1));
}

void snString::InjectDelimiters(char in_cDelim, const snString & in_Chars, bool in_bSkipQuotes)
{

   UNUSED_ARGUMENT(in_bSkipQuotes);
   // create a boolean map for fast lookup
   bool charMap[255];
   for(size_t i=0;i<255;i++)
      charMap[i] = false;
   for(long i=0;i<in_Chars.GetLength();i++)
      charMap[int(in_Chars.GetCharAt(i))] = true;

   snString result;
   result.Resize(GetLength()*3);
   size_t offset=0;
   char quote = 0;
   for(long i=0;i<GetLength();i++)
   {
      if(quote == 0)
      {
         if(GetCharAt(i) == '\'' || GetCharAt(i) == '\"')
         {
            quote = GetCharAt(i);
         }
      }
      if(quote == 0 && charMap[int(GetCharAt(i))])
      {
         if(i>0)
         {
            if(GetCharAt(i-1)!=in_cDelim)
               result.GetAsChar()[offset++] = in_cDelim;
         }
         result.GetAsChar()[offset++] = GetCharAt(i);
         if(i<GetLength()-1)
         {
            if(GetCharAt(i+1)!=in_cDelim)
               result.GetAsChar()[offset++] = in_cDelim;
         }
      }
      else
      {
         result.GetAsChar()[offset++] = GetCharAt(i);
      }

      if(quote == GetCharAt(i))
         quote = 0;
   }
   Set(result.GetLeft(offset));
}

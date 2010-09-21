#ifndef __SNSTRING__
#define __SNSTRING__

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <map>
#define UNUSED_ARGUMENT(x) (void)x

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
   // forward declarations
   class snString;
   typedef std::vector<snString> snStringArray;
   typedef std::map<snString, snString> snStringMap;
   typedef std::map<snString, snString>::iterator snStringMapIt;
   typedef std::pair<snString, snString> snStringPair;

   // constants
   const long g_lChunkSize = 255l;
   const long g_lBuffersize = 16000000l;

   /**
    * A complex string implementation supporting tokens.
    */
   class snString
   {
   private:

      /// The internal char buffer.
      char * m_pBuffer;
      /// The internal wchar buffer.
      wchar_t * m_pBufferW;
      /// The length of the string.
      long m_iLength;
      /// The size of the buffer.
      long m_iSize;
      /// The token list.
      snStringMap m_sTokens;

      /// Deallocate all memory used for the string.
      void Deallocate();
      /// Allocate all memory necessary for the string.
      void Allocate(bool in_bCopy = true);

      static char * sBuffer;
      static long sInstanceCount;

   public:

      // constructors
      snString();
      snString(const char & in_Char);
      snString(const char * in_pChar);
      snString(const wchar_t * in_pChar);
      snString(const snString & in_strOther);
      ~snString();

      // operators
      bool operator != (const snString & in_strOther) const;
      bool operator == (const snString & in_strOther) const;
      bool operator != (const char * in_cOther) const;
      bool operator == (const char * in_cOther) const;
      snString & operator = (const snString & in_strOther);
      snString & operator = (const char * in_cOther);
      snString operator + (const snString & in_strOther) const;
      snString operator + (const char * in_cOther) const;
      void operator += (const snString & in_strOther);
      void operator += (const char * in_cOther);
      bool operator < (const snString & in_strOther) const;
      bool operator > (const snString & in_strOther) const;

      // getters
      long GetLength() const {return m_iLength;}
      char * GetAsChar() const;
      std::string GetAsStdString() const;
      wchar_t * GetAsWChar();
      /// Return the char at a given position in the string.
      char GetCharAt(long in_iIndex = 0) const;
      /// Get the char at a given position from the back of the string.
      char GetReverseCharAt(long in_iIndex = 0) const;
      /// Get a left-aligned version of the string with the given length.
      snString GetLeft(long in_iCount) const;
      /// Return a substring portion of the string.
      snString GetMiddle(long in_iStart, long in_iCount) const;
      /// Get a right-aligned version of the string with the given length.
      snString GetRight(long in_iCount) const;
      /// Get a reversed version of the string.
      snString GetReverse() const;
      /// Get the first index matching the given char (using the offset), or -1 if not found.
      long GetFirstIndexOf(char in_cToFind,long in_iStartIndex = 0) const;
      /// Get the last index matching the given char (using the offset), or -1 if not found.
      long GetLastIndexOf(char in_cToFind) const;
      /// Get the first index NOT matching the given char (using the offset), or if all are the same.
      long GetFirstIndexOfNot(char in_cNotToFind,long in_iStartIndex = 0) const;
      /// Get the last index NOT matching the given char (using the offset), or if all are the same.
      long GetLastIndexOfNot(char in_cNotToFind) const;
      /// Return true if the string is empty or of zero length.
      bool IsEmpty() const { return m_iLength == 0; }
      /// Get a lower case version of the string.
      snString GetLowerCase() const;
      /// Get a upper case version of the string.
      snString GetUpperCase() const;
      /// Count the appearances of a given char.
      long CountChar(char in_cToCount) const;
      /// Count the appearances of a given substring.
      long Count(const snString & in_strToCount) const;
      /// Find the location of a given substring, or -1 if not found.
      long Find(const snString & in_strToFind, long in_iStartIndex = 0) const;
      /// Cast the string representing a number to a long.
      long GetAsLong() const;
      /// Cast the string representing a decimal number to a float.
      float GetAsFloat() const;
      /// Return true if the string starts with a given substring.
      bool StartsWith(const snString & in_Prefix) const;
      /// Return true if the string ends with a given substring.
      bool EndsWith(const snString & in_Suffix) const;
      /// Get the string with the first character as upper case.
      snString GetCapitalize() const;

      /// An alias with GetMiddle.
      snString GetSubString(long in_iStart, long in_iCount) const {return GetMiddle(in_iStart,in_iCount);}

      /// Clear the string.
      void Empty();
      /// Copy from another snString.
      void Set(const snString & in_strOther);
      /// Copy from a given char array.
      void Set(const char * in_cOther);
      /// Use the printf composition syntax using a snString format.
      void Compose(snString in_strTemplate, ...);
      /// Use the printf composition syntax using a char array format.
      void Compose(const char * in_cTemplate, ...);
      /// Cut off n chars from the left of the string.
      void TruncateLeft(long in_iCount);
      /// Cut off n chars in the middle of the string.
      void TruncateMiddle(long in_iStart, long in_iCount);
      /// Cut off n chars from the right of the string.
      void TruncateRight(long in_iCount);
      /// Cut off n chars from the left and right
      void TruncateLeftRight(long in_iLeft, long in_iRight);
      /// Set this string to the reverse of itself.
      void Reverse();
      /// Put a given string infront of the current string as a prefix.
      void Prepend(const snString & in_strPrefix);
      /// Put a given string behind of the current string as a suffix.
      void Append(const snString & in_strSuffix);
      /// Put a given char array behind of the current string as a suffix.
      void Append(const char * in_cSuffix, size_t in_Size = 0);
      /// Fill the string with a given substring n times.
      void Fill(const snString & in_strElement, long in_iCount);
      /// Fill the string with a given substring n times, using a delimiter
      void Fill(const snString & in_strElement, const snString & in_strDelim, long in_iCount);
      /// Replace all occurences of a given char with a given replacement char.
      bool Replace(char in_cToFind, char in_cReplacement, long in_iOccurences = -1);
      /// Replace all occurences of a given char with a given replacement string.
      bool Replace(char in_cToFind, snString in_strReplacement, long in_iOccurences = -1);
      /// Replace all occurences of a given stirng with a given replacement string.
      bool Replace(snString in_strToFind, snString in_strReplacement, long in_iOccurences = -1);
      /// Make this string the lowercase version of itself.
      void LowerCase();
      /// Make this string the uppercase version of itself.
      void UpperCase();
      /// Resize this string to a given length.
      void Resize(long in_iSize);
      /// Return a stringarray by splitting the string with a given delimiter (and optionally, skip delims within quotes).
      snStringArray Split(char in_cDelimiter = 0, bool in_bSkipQuotes = false) const;
      /// Return a stringarray by splitting the string with a given delimiter (for the left most instance of the delimiter).
      snStringArray Partition(char in_cDelimiter = '_', bool in_bSkipQuotes = false) const;
      /// Return a stringarray by splitting the string with a given delimiter (for the right most instance of the delimiter).
      snStringArray RPartition(char in_cDelimiter = '_', bool in_bSkipQuotes = false) const;
      /// Join a given array of string with a given delimiter.
      void Join(const snStringArray & in_pStringArray, char in_cDelimiter = 0);
      /// Remove all occurences of a given char.
      void Remove(char in_cToRemove);
      /// Make the first char of this string uppercase.
      void Capitalize();
      /// Strip the given char from the left side
      void LStrip(char in_Char = ' ');
      /// Strip the given char from the right side
      void RStrip(char in_Char = ' ');
      /// Strip the given char from both sides
      void Strip(char in_Char = ' ');
      /// Strip spaces and tab characters from both sides
      void StripWhiteSpaces();
      /// Inject left and right delimiters for given chars
      void InjectDelimiters(char in_cDelim, const snString & in_Chars, bool in_bSkipQuotes = false);

      /// Add a token for the token table.
      bool AddToken(const snString & in_TokenName, const snString & in_TokenValue);
      /// Set all tokens by a prebuild token map.
      void SetTokens(const snStringMap & in_sTokens) { m_sTokens = in_sTokens; }
      /// Remove all tokens.
      void ClearTokens() { m_sTokens.clear(); }
      /// Resolve all tokens with either the internal or a given token map.
      snString GetTokenized(snStringMap * in_pTokens = NULL, char in_cOpen = '[', char in_cClose = ']');
      /// Check if this string contains tokens.
      bool ContainsTokens( char in_cOpen = '[', char in_cClose = ']' ) const;

      /// Load the contents of a file into this string.
      bool Load(const snString & in_strFileName);
      /// Save the content of this string into a file.
      bool Save(const snString & in_strFileName, const snString & in_strMode = "wb") const;
   };
}
#endif

#ifndef __SNPATH__
#define __SNPATH__

#include "snString.h"
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

/**
 * snEssence is the shared namespace between all classes within the snEssence library.
 */
namespace snEssence
{
   /**
    * @brief A filepath class.
    *
    * A class wrapping the snString to deal with filepath operations
    * as well as token resolval. The class represents a filepath as a
    * relation between a so called base path and a relative path to that.
    * So for example the base path could be /mnt/cosmo2 while the input
    * path is /mnt/cosmo2/assets/monsterAvA.png. The input path could
    * also just be asset/monsterAvA.png, and the snPath will deal with
    * the absolute / relative file path conversions.
    *
    * Both the base and the input path can contain tokens, using the
    * brackets, so for example /mnt/cosmo2/[myvariable]/monsterAvA.[extension]
    * The resolved versions of these paths are then stored in other members,
    * called the TokenBasePath and the TokenInputPath.
    */
   class snPath
   {
   protected:

      /// The base path the input path is based on.
      snString mBasePath;
      /// The path input by the user (relative or absolute)
      snString mInputPath;
      /// The resolved (without tokens) version of the basepath.
      snString mTokenBasePath;
      /// The resolved (without tokens) version of the inputpath.
      snString mTokenInputPath;
      /// The resolved relative path (relative to the basepath).
      snString mRelativePath;
      /// The resolved absolute path.
      snString mAbsolutePath;

      /// Holds true if we resolved all tokens.
      bool mResolvedTokens;
      /// Holds true if we already resolved the relative path.
      bool mResolvedRelative;
      /// Holds true if we already resolved the absolute path.
      bool mResolvedAbsolute;
      /// Holds true if the input path is absolute.
      bool mIsAbsolute;

      /// A map of the tokens of the path.
      snStringMap mTokens;

      /// The error member.
      snError mError;

   public:

      snPath();
      /// Construct a path from two strings
      /// @param in_Path The input path.
      /// @param in_BasePath The base path.
      snPath(const snString & in_Path, const snString & in_BasePath = "");
      ~snPath();

      /// Compare two path for inequality.
      bool operator != (snPath in_Other);
      /// Compare two path for equality.
      bool operator == (snPath in_Other);
      /// Copy a path.
      snPath & operator = (const snPath & in_Other);

      /// Returns the relative path (based on mBasePath)
      snString GetRelativePath() { Resolve(); return mRelativePath; }
      /// Returns the absolute path (based on mBasePath)
      snString GetAbsolutePath() { Resolve(); return mAbsolutePath; }
      /// Returns the used inputpath
      snString GetInputPath() const { return mInputPath; }
      /// Returns the used basepath
      snString GetBasePath() const { return mBasePath; }
      /// Returns the resolved version of the inputpath.
      snString GetTokenInputPath() { Resolve(); return mTokenInputPath; }
      /// Returns the resolved version of the basepath.
      snString GetTokenBasePath() { Resolve(); return mTokenBasePath; }
      /// Returns the OS-specific slash as a char (/ or \)
      static char GetSlash();
      /// Returns the OS-specific slash as a snString ("/" or "\")
      static snString GetSlashStr();
      /// Returns true if the given path is a file (if it contains '.').
      bool IsFile() const { return mInputPath.GetFirstIndexOf('.')!=-1l;}
      /// Returns true if the given path is a folder path.
      bool IsFolder() const { return !IsFile(); }
      /// Returns true if the input path is not absolute!
      bool IsAbsolute() { Resolve(); return mIsAbsolute;}

      /// Set the input path.
      snString SetPath( const snString & in_Path);
      /// Set the base path.
      snString SetBasePath( const snString & in_Path, bool in_bAppend = false);

      /// Add a token to the token map.
      bool AddToken(const snString & in_TokenName, const snString & in_TokenValue);
      /// Set all tokens using a prebuild stringmap.
      void SetTokens(const snStringMap & in_sTokens) { mTokens = in_sTokens; }
      /// Remove all tokens.
      void ClearTokens() { mTokens.clear(); }
      /// Get the number of tokens.
      size_t GetNbTokens() const { return mTokens.size(); }
      /// Get all tokens
      const snStringMap & GetTokens() const { return mTokens; }

      /// Remove the last segment from the path (folder or file)
      snString PopSegment();
      /// Add a segment to the path (folder or file)
      void PushSegment(const snString & in_strSegment);
      /// Create the directory represented by the path.
      bool CreateDirectory();
      /// Get all files within a directory
      snStringArray GetFilesInFolder();

   private:
      /// Resolve all contained tokens.
      snError Resolve();
      /// Unset all resolved flags.
      void Unresolve();
      /// Return true if we are compiled for Windows.
      static bool IsWindows();
   };
}
#endif

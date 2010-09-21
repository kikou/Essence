#include "snPath.h"

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

// this is for the mkdir command
#ifdef WIN32
   #include <direct.h>
#else
   #include <sys/types.h>
   #include <sys/stat.h>
   #include <dirent.h>
#endif

using namespace snEssence;

snPath::snPath()
{
   mResolvedTokens = false;
   mResolvedRelative = false;
   mResolvedAbsolute = false;
}

snPath::~snPath()
{
}

snPath::snPath(const snString & in_Path, const snString & in_BasePath)
{
   mInputPath = in_Path;
   mBasePath = in_BasePath;
   Unresolve();
}

bool snPath::operator ==(snPath in_Other)
{
   if(!Resolve())
      return false;

   if(mInputPath == in_Other.GetInputPath())
      return true;
   if(mRelativePath == in_Other.GetRelativePath())
      return true;
   if(mAbsolutePath == in_Other.GetAbsolutePath())
      return true;
   return false;
}

bool snPath::operator !=(snPath in_Other)
{
	return !(*this == in_Other);
}

snPath & snPath::operator = (const snPath & in_Other)
{
	mInputPath = in_Other.GetInputPath();
	mBasePath = in_Other.GetBasePath();

   mTokens = in_Other.mTokens;

   Unresolve();
   return *this;
}

snString snPath::SetPath( const snString & in_Path)
{
   Unresolve();
   mInputPath = in_Path;
   return in_Path;
}

snString snPath::SetBasePath( const snString & in_Path, bool in_bAppend )
{
   Unresolve();
   if(in_bAppend)
   {
      if(in_Path.StartsWith(GetSlashStr()))
      {
         // remove trailing slashes
         while(mBasePath.EndsWith(GetSlashStr()))
            mBasePath.TruncateRight(1);
      }
      else if(!mBasePath.EndsWith(GetSlashStr()))
      {
         mBasePath.Append(GetSlashStr());
      }
      mBasePath.Append(in_Path);
   }
   else
      mBasePath = in_Path;
   return in_Path;
}

char snPath::GetSlash()
{
#ifdef WIN32
   return '\\';
#else
   return '/';
#endif
}

snString snPath::GetSlashStr()
{
#ifdef WIN32
   return "\\";
#else
   return "/";
#endif
}

bool snPath::IsWindows()
{
#ifdef WIN32
   return true;
#else
   return false;
#endif
}

snError snPath::Resolve()
{
   // check if we have tokens inside of the path
   // if so resolve both input path and basepath
   if(!mResolvedTokens)
   {
      if(mInputPath.ContainsTokens())
         mTokenInputPath = mInputPath.GetTokenized(&mTokens);
      else
         mTokenInputPath = mInputPath;

      if(mBasePath.ContainsTokens())
         mTokenBasePath = mBasePath.GetTokenized(&mTokens);
      else
         mTokenBasePath = mBasePath;

      mResolvedTokens = true;

      // windows is not case sensitive, so let's just deal with lowercase
      if(IsWindows())
      {
         mTokenInputPath = mTokenInputPath.GetLowerCase();
         mTokenBasePath = mTokenBasePath.GetLowerCase();
         mTokenInputPath.Replace("/","\\");
         mTokenBasePath.Replace("/","\\");
      }
      else
      {
         mTokenInputPath.Replace("\\","/");
         mTokenBasePath.Replace("\\","/");
      }

      // if the basepath is a file, remove the trailing file name
      if(mTokenBasePath.GetFirstIndexOf('.')!=-1)
         mTokenBasePath = mTokenBasePath.GetLeft(mTokenBasePath.GetLastIndexOf(GetSlash()));

      // if we have '..' in front of the input path
      if((mTokenInputPath.GetCharAt(0)=='.') && (mTokenInputPath.GetCharAt(0)=='.'))
      {
         snStringArray mBaseParts = mTokenBasePath.Split(GetSlash());
         snStringArray mInputParts = mTokenInputPath.Split(GetSlash());

         // if the base path ends in a slash, remove the empty part
         if(mBaseParts[mBaseParts.size()-1].IsEmpty())
            mBaseParts.pop_back();

         // now remove one for each
         while(mInputParts[0] == "..")
         {
            mBaseParts.pop_back();
            mInputParts.erase(mInputParts.begin());
         }

         // great, now put it back together
         mTokenBasePath.Join(mBaseParts,GetSlash());
         mTokenInputPath.Join(mInputParts,GetSlash());
      }
   }

   // check if we have the absolute path
   // if not let's see what we have
   if(!mResolvedAbsolute)
   {
      // do two different things for windows and linux
      if(IsWindows())
      {
         // let's see if the path is absolute on windows,
         // there are two options:
         if((mTokenInputPath.GetLeft(2) == "\\\\") || (mTokenInputPath.GetCharAt(1)==':'))
         {
            mAbsolutePath = mTokenInputPath;
            mIsAbsolute = true;
         }
         else
         {
            // this is a relative path, let's add the basepath
            mAbsolutePath = mTokenBasePath;
            if(!(mAbsolutePath.GetRight(1)=="\\") && !(mTokenInputPath.GetLeft(1)=="\\"))
               mAbsolutePath.Append("\\");
            mAbsolutePath.Append(mTokenInputPath);
            mIsAbsolute = false;
         }
      }
      else
      {
         if(mTokenInputPath.GetCharAt(0)=='/')
         {
            mAbsolutePath = mTokenInputPath;
            mIsAbsolute = true;
         }
         else
         {
            // this is a relative path, let's add the basepath
            mAbsolutePath = mTokenBasePath;
            if(!(mAbsolutePath.GetRight(1)=="/") && !(mTokenInputPath.GetCharAt(0)=='/'))
               mAbsolutePath.Append("/");
            mAbsolutePath.Append(mTokenInputPath);
            mIsAbsolute = false;
         }
      }
      mResolvedAbsolute = true;
   }

   // check if we have the relative path
   // if not let's resolve to the relative path
   if(!mResolvedRelative)
   {
      mRelativePath.Empty();

      if(!mIsAbsolute)
         mRelativePath = mTokenInputPath;
      else
      {
         // this means the inputpath is absolute
         // first, on windows, let's check if we
         // have UNC or drive paths
         if(IsWindows())
         {
            // if the first two chars don't match, we cannot make this relative
            if(mTokenInputPath.GetLeft(2) != mTokenBasePath.GetLeft(2))
            {
               mRelativePath = mTokenInputPath;
               mResolvedRelative = true;
            }
            else
            {
               // if we have UNC, the server and share have to match
               long firstSlash = mTokenInputPath.GetFirstIndexOf('\\',2);
               long secondSlash = mTokenInputPath.GetFirstIndexOf('\\',firstSlash+1);
               if(secondSlash == -1)
               {
                  // if we didn't find the second slash, the path is too short for UNC
                  mRelativePath = mTokenInputPath;
                  mResolvedRelative = true;
               }
               else
               {
                  if(mTokenInputPath.GetLeft(secondSlash) != mTokenBasePath.GetLeft(secondSlash))
                  {
                     // if the server and share don't match, we cannot make the path relative
                     // ex: \\SERVER01\DATA and \\SERVER02\HOME
                     mRelativePath = mTokenInputPath;
                     mResolvedRelative = true;
                  }
               }
            }
         }

         if(!mResolvedRelative)
         {
            snStringArray mBaseParts = mTokenBasePath.Split(GetSlash());
            snStringArray mInputParts = mTokenInputPath.Split(GetSlash());

            snStringArray resultParts;
            size_t minCount = mBaseParts.size() < mInputParts.size() ? mBaseParts.size() : mInputParts.size();
            bool match = true;
            for(size_t i=0;i<minCount;i++)
            {
               // if the folder is the same, we skip that part
               if(match && mBaseParts[i] == mInputParts[0])
               {
                  mInputParts.erase(mInputParts.begin());
                  continue;
               }

               match = false;
               resultParts.push_back("..");
            }
            for(size_t i=0;i<mInputParts.size();i++)
               resultParts.push_back(mInputParts[i]);
            mRelativePath.Join(resultParts,GetSlash());
         }
      }
      mResolvedRelative = true;
   }
   return snError();
}

void snPath::Unresolve()
{
   mResolvedTokens = false;
   mResolvedRelative = false;
   mResolvedAbsolute = false;
}

bool snPath::AddToken(const snString & in_TokenName, const snString & in_TokenValue)
{
   snString tokenName(in_TokenName.GetLowerCase());
   if(mTokens.find(tokenName) == mTokens.end())
   {
      mTokens.insert(snStringPair(tokenName,in_TokenValue));
      return true;
   }
   return false;
}

snString snPath::PopSegment()
{
   snStringArray parts = mInputPath.Split(GetSlash());
   snString result = parts[parts.size()-1];
   parts.pop_back();
   mInputPath.Join(parts,GetSlash());
   Unresolve();
   return result;
}

void snPath::PushSegment(const snString & in_strSegment)
{
   snString segment(in_strSegment);
   segment.Remove('/');
   segment.Remove('\\');

   snStringArray parts = mInputPath.Split(GetSlash());
   parts.push_back(segment);
   mInputPath.Join(parts,GetSlash());

   Unresolve();
}

bool snPath::CreateDirectory()
{
   snPath folder(GetAbsolutePath());
   if(folder.IsFile())
      folder.PopSegment();

   snString path;
   snStringArray parts = folder.GetAbsolutePath().Split(GetSlash());
   path = parts[0];
   for(size_t i = 1; i<parts.size();i++)
   {
      path += GetSlashStr() + parts[i];
#ifdef WIN32
      _mkdir(path.GetAsChar());
#else
      mkdir(path.GetAsChar(), 0777);
#endif
   }
   return true;
}

snStringArray snPath::GetFilesInFolder()
{
   snStringMap resultMap;
#ifdef WIN32
   printf("NOT IMPLEMENTED !!!\n");
#else

   struct dirent *de=NULL;
   DIR *d=opendir(GetAbsolutePath().GetAsChar());
   if(d == NULL)
      return snStringArray();

   // de turns NULL if last file
   while((de = readdir(d)))
      resultMap.insert(snStringPair(de->d_name,de->d_name));
   closedir(d);
#endif

   snStringArray results;
   for(snStringMapIt it=resultMap.begin();it!=resultMap.end();it++)
      results.push_back(it->first);
   return results;
}

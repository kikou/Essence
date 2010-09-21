#ifndef __SNVARIANT__
#define __SNVARIANT__

#include <vector>
#include "snString.h"
#include "snVector2f.h"
#include "snVector3f.h"
#include "snBboxf.h"
#include "snColor3f.h"
#include "snColor4f.h"
#include "snQuaternionf.h"
#include "snTransformf.h"

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
   class snVariant;
   typedef std::vector<snVariant> snVariantVec;
   typedef snVariant * snpVariant;
   typedef std::vector<snpVariant> snpVariantVec;
   typedef snpVariantVec * snpVariantpVec;

   /**
    * A variant representing a variety of datatypes.
    */
   class snVariant
   {
   public:
      /// The enum representing the current datatype of the variant.
      enum eType{
         eType_Bool,          /**< Boolean. */
         eType_Float,         /**< Float. */
         eType_Long,          /**< long. */
         eType_String,        /**< snString. */
         eType_TimeStamp,     /**< snString as TimeStamp. */
         eType_VariantVec,    /**< A std::vector of snVariant. */
         eType_Vector2f,      /**< A vector2 float. */
         eType_Vector3f,      /**< A vector3 float. */
         eType_BBoxf,         /**< A bounding box float. */
         eType_Color3f,       /**< A color3 float. */
         eType_Color4f,       /**< A color4 float. */
         eType_Quaternionf,   /**< A quaternion float. */
         eType_Matrix44f,     /**< A 4x4 float matrix. */
         eType_FloatArray,    /**< A float array. */
         eType_LongArray,     /**< A long array. */
         eType_VoidPtr,       /**< A void ptr. */
         eType_NbElements     /**< Max Value for the enum. */
         // if you are adding new types makes sure to add them in the contructors implementation!
      };

      static snStringArray eTypeNames;
      static snStringArray float1Formats;
      static snStringArray float2Formats;
      static snStringArray float3Formats;
      static snStringArray float4Formats;
      static snStringArray float6Formats;
      static snStringArray float16Formats;
      static void FillStaticMembers();

   private:
      eType m_eType;

      union uData{
         bool b;
         long l;
         float f;
         snString * s;
         snpVariantpVec pv;
         snVector2f * v2;
         snVector3f * v3;
         snBboxf * bb;
         snColor3f * c3;
         snColor4f * c4;
         snQuaternionf * q;
         snMatrix44f * m;
         float * fp;
         long * lp;
         void * ptr;
      };

      uData m_uData;
      size_t m_iSize;

   public:

      /// default constructor, uses bool.
      snVariant();
      /// constructor by given datatype.
      snVariant(eType in_eType);
      /// constructor casting from bool
      snVariant(bool in_B);
      /// constructor casting from float
      snVariant(float in_F);
      /// constructor casting from long
      snVariant(long in_L);
      /// constructor casting from snString
      snVariant(const snString & in_S);
      /// copy constructor
      snVariant(const snVariant & in_V);
      /// constructor casting from a std::vector of snVariant
      snVariant(snpVariantpVec in_pV);
      /// constructor casting from snVector2f
      snVariant(const snVector2f & in_V);
      /// constructor casting from snVector3f
      snVariant(const snVector3f & in_V);
      /// constructor casting from snBboxf
      snVariant(const snBboxf & in_BB);
      /// constructor casting from snColor3f
      snVariant(const snColor3f & in_C);
      /// constructor casting from snColor4f
      snVariant(const snColor4f & in_C);
      /// constructor casting from snQuaternionf
      snVariant(const snQuaternionf & in_Q);
      /// constructor casting from snMatrix44f
      snVariant(const snMatrix44f & in_M);
      /// constructor casting from a float array
      snVariant(float * in_pFloat, size_t in_iCount);
      /// constructor casting from a long array
      snVariant(long * in_pLong, size_t in_iCount);
      /// constructor casting from a void pointer
      snVariant(void * in_Ptr, size_t in_iSize);
      ~snVariant();

      /// copy constructor helper
      void Set(const snVariant & in_V);
      /// destructor helper
      void Clear();

      /// Get the size of the vector, if datatype = eType_VariantVec, otherwise 1
      long GetSize() const;
      /// Get the datatype of the variant
      eType GetType() const { return m_eType; }
      /// Get the datatype as a string
      static snString GetTypeName( const eType & in_eType ) { return eTypeNames[(size_t)in_eType] ; }
      /// Get the value as a bool (does automatic casting).
      bool GetBool() const;
      /// Get the value as a float (does automatic casting).
      float GetFloat() const;
      /// Get the value as a long (does automatic casting).
      long GetLong() const;
      /// Get the value as a snString (does automatic casting).
      snString GetString(bool in_bPadded = false, size_t in_lPrecision = 5, char in_cDelimiter = ' ') const;
      /// Get the pointer to the internal vector.
      snpVariantpVec GetVariants();
      /// Get the value as a snVector2f
      snVector2f GetVector2f( ) const;
      /// Get the value as a snVector3f
      snVector3f GetVector3f( ) const;
      /// Get the value as a snBboxf
      snBboxf GetBboxf( ) const;
      /// Get the value as a snColor3f
      snColor3f GetColor3f( ) const;
      /// Get the value as a snColor4f
      snColor4f GetColor4f( ) const;
      /// Get the value as a snQuaternionf
      snQuaternionf GetQuaternionf( ) const;
      /// Get the value as a snMatrix44f
      snMatrix44f GetMatrix44f( ) const;
      /// Get the value as a float pointer / array.
      float * GetFloatArray( );
      /// Get the value as a long pointer / array.
      long * GetLongArray( );
      /// Get the value as a void pointer.
      void * GetVoidPtr( );
      /// Get the number of floats in the float array.
      size_t GetFloatArrayCount( ) const { return m_iSize / sizeof(float); }
      /// Get the number of longs in the long array.
      size_t GetLongArrayCount(  ) const { return m_iSize / sizeof(long); }
      /// Get the size of the float pointer.
      size_t GetFloatArraySize(  ) const { return m_iSize; }
      /// Get the size of the long pointer.
      size_t GetLongArraySize(  ) const { return GetFloatArraySize(); }
      /// Get the size of the void pointer.
      size_t GetVoidPtrSize(  ) const { return GetFloatArraySize(); }

      /// Set this variant from a bool
      void SetBool(bool in_B);
      /// Set this variant from a float
      void SetFloat(float in_F);
      /// Set this variant from a long
      void SetLong(long in_L);
      /// Set this variant from a snString
      void SetString(const snString & in_S, char in_cDelimiter = ' ');
      /// Set this variant from a snVariant (copy constructor)
      void SetVariant(const snVariant & in_V);
      /// Set this variant from a snVector2f
      void SetVector2f(const snVector2f & in_V);
      /// Set this variant from a snVector3f
      void SetVector3f(const snVector3f & in_V);
      /// Set this variant from a snBboxf
      void SetBboxf(const snBboxf & in_BB);
      /// Set this variant from a snColor3f
      void SetColor3f(const snColor3f & in_C);
      /// Set this variant from a snColor4f
      void SetColor4f(const snColor4f & in_C);
      /// Set this variant from a snQuaternionf
      void SetQuaternionf(const snQuaternionf & in_Q);
      /// Set this variant from a snQuaternionf
      void SetMatrix44f(const snTransformf & in_M);
      /// Set this variant from a float array
      void SetFloatArray(float * in_pFloat, size_t in_iCount);
      /// Set this variant from a long array
      void SetLongArray(long * in_pLong, size_t in_iCount);
      /// Set this variant from a void ptr
      void SetVoidPtr(void * in_Ptr, size_t in_iSize);

      /// Allocate memory for the float array
      float * AllocateFloatArray(size_t in_iCount);
      /// Allocate memory for the long array
      long * AllocateLongArray(size_t in_iCount);
      /// Allocate memory for the long array
      void * AllocateVoidPtr(size_t in_iSize);

      // operators
      snVariant & operator = (const snVariant & in_V);
      operator bool() const { return GetBool(); }
      operator float() const { return GetFloat(); }
      operator long() const { return GetLong(); }
      operator snString() const { return GetString(); }
   };

   typedef std::vector<snVariant::eType> snVariantTypeVec;
}
#endif

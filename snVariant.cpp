#include "snVariant.h"

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

snStringArray snVariant::eTypeNames;

snStringArray snVariant::float1Formats;

snStringArray snVariant::float2Formats;

snStringArray snVariant::float3Formats;

snStringArray snVariant::float4Formats;

snStringArray snVariant::float6Formats;

snStringArray snVariant::float16Formats;


void snVariant::FillStaticMembers()
{
   // if we didn't add the names yet!

   if(eTypeNames.size() == 0)

   {
      eTypeNames.resize(eType_NbElements);
      eTypeNames[eType_Bool]            = "Bool";

      eTypeNames[eType_Float]           = "Float";

      eTypeNames[eType_Long]            = "Long";

      eTypeNames[eType_String]          = "String";

      eTypeNames[eType_TimeStamp]       = "TimeStamp";

      eTypeNames[eType_VariantVec]      = "Variant Array";

      eTypeNames[eType_Vector2f]        = "Vector2";

      eTypeNames[eType_Vector3f]        = "Vector3";

      eTypeNames[eType_Color3f]         = "Color3";

      eTypeNames[eType_Color4f]         = "Color4";

      eTypeNames[eType_Quaternionf]     = "Quaternion";

      eTypeNames[eType_Matrix44f]       = "Matrix44";

      eTypeNames[eType_BBoxf]           = "Bbox";

      eTypeNames[eType_FloatArray]      = "Float Array";

      eTypeNames[eType_LongArray]       = "Long Array";

      eTypeNames[eType_VoidPtr]         = "Void Pointer";


      float1Formats.push_back("%ld");
      float1Formats.push_back("%.1f");
      float1Formats.push_back("%.2f");
      float1Formats.push_back("%.3f");
      float1Formats.push_back("%.4f");
      float1Formats.push_back("%.5f");
      float1Formats.push_back("%.6f");
      float1Formats.push_back("%.7f");
      float1Formats.push_back("%.8f");
      float1Formats.push_back("%.9f");
      float1Formats.push_back("%.10f");

      for(size_t i=0;i<float1Formats.size();i++)
      {
         float2Formats.push_back(float1Formats[i] + "%c" + float1Formats[i]);
         float3Formats.push_back(float2Formats[i] + "%c" + float1Formats[i]);
         float4Formats.push_back(float3Formats[i] + "%c" + float1Formats[i]);
         float6Formats.push_back(float3Formats[i] + "%c" + float3Formats[i]);
         float16Formats.push_back(float4Formats[i] + "%c" + float4Formats[i] + "%c" + float4Formats[i] + "%c" + float4Formats[i]);
      }
   }

}



snVariant::snVariant()

{

   FillStaticMembers();

	m_eType = eType_NbElements;
	m_uData.ptr = NULL;

}



snVariant::snVariant(eType in_eType)

{

   FillStaticMembers();

	m_eType = in_eType;
	m_iSize = 0;



	switch(m_eType)

	{

		case eType_Bool:

		{

			m_uData.b = false;

			break;

		}

		case eType_Long:

		{

			m_uData.l = 0l;

			break;

		}

		case eType_Float:

		{

			m_uData.f = 0.0f;

			break;

		}

		case eType_VariantVec:

		{

			m_uData.pv = new snpVariantVec();

			break;

		}

		case eType_Vector2f:

		{

			m_uData.v2 = new snVector2f();

			break;

		}

		case eType_Vector3f:

		{

			m_uData.v3 = new snVector3f();

			break;

		}

		case eType_BBoxf:

		{

			m_uData.bb = new snBboxf();

			break;

		}

		case eType_Color3f:

		{

			m_uData.c3 = new snColor3f();

			break;

		}

		case eType_Color4f:

		{

			m_uData.c4 = new snColor4f();

			break;

		}

		case eType_Quaternionf:

		{

			m_uData.q = new snQuaternionf();

			break;

		}

		case eType_Matrix44f:

		{

			m_uData.m = new snMatrix44f();

			break;

		}

		case eType_FloatArray:

		{

			m_uData.fp = NULL;
			break;

		}

		case eType_LongArray:

		{

			m_uData.lp = NULL;
			break;

		}

		case eType_VoidPtr:

		{

			m_uData.ptr = NULL;
			break;

		}

		case eType_String:

		case eType_TimeStamp:
		{
			m_uData.s = new snString();
			break;

		}
		case eType_NbElements:

		default:

		{

			break;

		}

	}

}



snVariant::snVariant(bool in_B)

{

   FillStaticMembers();

	m_iSize = 0;

	m_eType = eType_Bool;

	m_uData.b = in_B;

}



snVariant::snVariant(float in_F)

{

   FillStaticMembers();

	m_iSize = 0;

	m_eType = eType_Float;

	m_uData.f = in_F;

}



snVariant::snVariant(long in_L)

{

   FillStaticMembers();

	m_iSize = 0;

	m_eType = eType_Long;

	m_uData.l = in_L;

}



snVariant::snVariant(const snString & in_S)

{

   FillStaticMembers();

	m_iSize = 0;

	m_eType = eType_String;

	m_uData.s = new snString(in_S);

}



snVariant::snVariant(const snVariant & in_V)

{

   FillStaticMembers();
   m_uData.ptr = NULL;

	m_eType = in_V.m_eType;

	Set(in_V);

}

snVariant::snVariant(const snVector2f & in_V)
{
   FillStaticMembers();

	m_iSize = 0;

   m_eType = eType_Vector2f;
   m_uData.v2 = new snVector2f(in_V);
}


snVariant::snVariant(const snVector3f & in_V)

{
   FillStaticMembers();

	m_iSize = 0;

   m_eType = eType_Vector3f;
   m_uData.v3 = new snVector3f(in_V);
}


snVariant::snVariant(const snBboxf & in_BB)

{
   FillStaticMembers();

	m_iSize = 0;

   m_eType = eType_BBoxf;
   m_uData.bb = new snBboxf(in_BB);
}


snVariant::snVariant(const snColor3f & in_C)

{
   FillStaticMembers();

	m_iSize = 0;

   m_eType = eType_Color3f;
   m_uData.c3 = new snColor3f(in_C);
}


snVariant::snVariant(const snColor4f & in_C)

{
   FillStaticMembers();

	m_iSize = 0;

   m_eType = eType_Color4f;
   m_uData.c4 = new snColor4f(in_C);
}


snVariant::snVariant(const snQuaternionf & in_Q)

{
   FillStaticMembers();

	m_iSize = 0;

   m_eType = eType_Quaternionf;
   m_uData.q = new snQuaternionf(in_Q);
}


snVariant::snVariant(const snMatrix44f & in_M)
{
   FillStaticMembers();

	m_iSize = 0;

   m_eType = eType_Matrix44f;
   m_uData.m = new snMatrix44f(in_M);
}


snVariant::snVariant(float * in_pFloat, size_t in_iCount)

{
   FillStaticMembers();

	m_iSize = in_iCount * sizeof(float);

   m_eType = eType_FloatArray;
   m_uData.fp = (float*)malloc(m_iSize);
   if(in_pFloat != NULL)
      memcpy(m_uData.fp,in_pFloat,m_iSize);
}


snVariant::snVariant(long * in_pLong, size_t in_iCount)

{
   FillStaticMembers();

	m_iSize = in_iCount * sizeof(long);

   m_eType = eType_LongArray;
   m_uData.lp = (long*)malloc(m_iSize);
   if(in_pLong != NULL)
      memcpy(m_uData.lp,in_pLong,m_iSize);
}


snVariant::snVariant(void * in_Ptr, size_t in_iSize)

{
   FillStaticMembers();

	m_iSize = in_iSize;

   m_eType = eType_VoidPtr;
   m_uData.ptr = malloc(m_iSize);
   memcpy(m_uData.ptr,in_Ptr,m_iSize);
}



void snVariant::Set(const snVariant & in_V)

{
   FillStaticMembers();

   Clear();

	m_eType = in_V.m_eType;

	switch(m_eType)

	{

		case eType_Bool:

		case eType_Float:

		case eType_Long:

		{

			m_uData = in_V.m_uData;

			break;

		}

		case eType_String:

		case eType_TimeStamp:

		{

			if(in_V.m_uData.s != NULL)

				m_uData.s = new snString(*in_V.m_uData.s);

			else

				m_uData.s = new snString();

			break;

		}

		case eType_VariantVec:

		{

			m_uData.pv = new snpVariantVec();

			if(in_V.m_uData.pv != NULL)

			{

				for(size_t i=0;i<in_V.m_uData.pv->size();i++)

				{

					m_uData.pv->push_back((*in_V.m_uData.pv)[i]);

				}

			}

			break;

		}
		case eType_Vector2f:
		{
		   m_uData.v2 = new snVector2f(*in_V.m_uData.v2);
		   break;
		}
		case eType_Vector3f:
		{
		   m_uData.v3 = new snVector3f(*in_V.m_uData.v3);
		   break;
		}
		case eType_BBoxf:
		{
		   m_uData.bb = new snBboxf(*in_V.m_uData.bb);
		   break;
		}
		case eType_Color3f:
		{
		   m_uData.c3 = new snColor3f(*in_V.m_uData.c3);
		   break;
		}
		case eType_Color4f:
		{
		   m_uData.c4 = new snColor4f(*in_V.m_uData.c4);
		   break;
		}
		case eType_Quaternionf:
		{
		   m_uData.q = new snQuaternionf(*in_V.m_uData.q);
		   break;
		}
		case eType_Matrix44f:
		{
		   m_uData.m = new snMatrix44f(*in_V.m_uData.m);
		   break;
		}
		case eType_FloatArray:
		{
		   SetFloatArray(in_V.m_uData.fp,in_V.GetFloatArrayCount());
		   break;
		}
		case eType_LongArray:
		{
		   SetLongArray(in_V.m_uData.lp,in_V.GetLongArrayCount());
		   break;
		}
		case eType_VoidPtr:
		{
		   SetVoidPtr(in_V.m_uData.ptr,in_V.m_iSize);
		   break;
		}
		case eType_NbElements:
		default:
		{
		   // if we don't know the type, reset the var
		   Clear();
         break;
		}

	}

}

snVariant::snVariant(snpVariantpVec in_pV)

{

   FillStaticMembers();

	m_eType = eType_VariantVec;

	m_uData.pv = new snpVariantVec();

	if(in_pV != NULL)

	{

		for(size_t i=0;i<in_pV->size();i++)

		{

			m_uData.pv->push_back((*in_pV)[i]);

		}

	}

}



snVariant::~snVariant()

{

	Clear();

}


void snVariant::Clear()
{
	switch(m_eType)

	{

		case eType_String:

		case eType_TimeStamp:

		{

			if(m_uData.s != NULL)

			{

				delete(m_uData.s);

				m_uData.s = NULL;

			}

			break;

		}

		case eType_VariantVec:

		{

			if(m_uData.pv != NULL)

			{

				for(size_t i=0;i<m_uData.pv->size();i++)

				{

					if((*m_uData.pv)[i] != NULL)

						delete((*m_uData.pv)[i]);

				}

			}

			delete(m_uData.pv);

			m_uData.pv = NULL;

			break;

		}

		case eType_Vector2f:

		{

			if(m_uData.v2 != NULL)

			{

            delete(m_uData.v2);

            m_uData.v2 = NULL;

			}

			break;

		}

		case eType_Vector3f:

		{

			if(m_uData.v3 != NULL)

			{

            delete(m_uData.v3);

            m_uData.v3 = NULL;

			}

			break;

		}

		case eType_BBoxf:

		{

			if(m_uData.bb != NULL)

			{

            delete(m_uData.bb);

            m_uData.bb = NULL;

			}

			break;

		}

		case eType_Color3f:

		{

			if(m_uData.c3 != NULL)

			{

            delete(m_uData.c3);

            m_uData.c3 = NULL;

			}

			break;

		}

		case eType_Color4f:

		{

			if(m_uData.c4 != NULL)

			{

            delete(m_uData.c4);

            m_uData.c4 = NULL;

			}

			break;

		}

		case eType_Quaternionf:

		{

			if(m_uData.q != NULL)

			{

            delete(m_uData.q);

            m_uData.q = NULL;

			}

			break;

		}

		case eType_Matrix44f:

		{

			if(m_uData.m != NULL)

			{

            delete(m_uData.m);

            m_uData.m = NULL;

			}

			break;

		}
		case eType_FloatArray:

		{

			if(m_uData.fp != NULL)

			{
			   free(m_uData.fp);

            m_uData.fp = NULL;

			}

			break;

		}
		case eType_LongArray:

		{

			if(m_uData.lp != NULL)

			{
			   free(m_uData.lp);

            m_uData.lp = NULL;

			}

			break;

		}
		case eType_VoidPtr:

		{

			if(m_uData.ptr != NULL)

			{
			   free(m_uData.ptr);

            m_uData.ptr = NULL;

			}

			break;

		}
		case eType_Bool:

		case eType_Float:

		case eType_Long:

		case eType_NbElements:

		default:

		{

			break;

		}

	}

	// reset to bool
	m_eType = eType_Bool;
	m_uData.b = false;
	m_iSize = 0;
}



long snVariant::GetSize() const

{

	if(m_eType != eType_VariantVec)

		return 1l;

	if(m_uData.pv == NULL)

		return 0l;

	return (long)m_uData.pv->size();

}



bool snVariant::GetBool() const

{

	switch(m_eType)

	{

		case eType_Bool:

		{

			return m_uData.b;

		}

		case eType_Float:

		{

			return m_uData.f > 0.0f;

		}

		case eType_Long:

		{

			return m_uData.l > 0l;

		}
		case eType_String:
		case eType_TimeStamp:
		case eType_VariantVec:
		case eType_Vector2f:

		case eType_Vector3f:

		case eType_BBoxf:

		case eType_Color3f:

		case eType_Color4f:

		case eType_Quaternionf:

		case eType_Matrix44f:

		case eType_FloatArray:

		case eType_LongArray:

		case eType_VoidPtr:
		case eType_NbElements:

		default:
		{
		   return false;
		}

	}

	return false;

}



float snVariant::GetFloat() const

{

	switch(m_eType)

	{

		case eType_Bool:

		{

			return m_uData.b ? 1.0f : 0.0f;

		}

		case eType_Float:

		{

			return m_uData.f;

		}

		case eType_Long:

		{

			return (float)m_uData.l;

		}

		case eType_String:

		case eType_TimeStamp:

		{

			return (float)atof(m_uData.s->GetAsChar());

		}

		case eType_VariantVec:
		case eType_Vector2f:

		case eType_Vector3f:

		case eType_BBoxf:

		case eType_Color3f:

		case eType_Color4f:

		case eType_Quaternionf:

		case eType_Matrix44f:

		case eType_FloatArray:

		case eType_LongArray:

		case eType_VoidPtr:
		case eType_NbElements:

		default:
		{
		   return 0.0f;
		}

	}

	return 0.0f;

}



long snVariant::GetLong() const

{

	switch(m_eType)

	{

		case eType_Bool:

		{

			return m_uData.b ? 1l : 0l;

		}

		case eType_Float:

		{

			return (long)m_uData.f;

		}

		case eType_Long:

		{

			return m_uData.l;

		}

		case eType_String:

		case eType_TimeStamp:

		{

			return (long)atoi(m_uData.s->GetAsChar());

		}

		case eType_VariantVec:
		case eType_Vector2f:

		case eType_Vector3f:

		case eType_BBoxf:

		case eType_Color3f:

		case eType_Color4f:

		case eType_Quaternionf:

		case eType_Matrix44f:

		case eType_FloatArray:

		case eType_LongArray:

		case eType_VoidPtr:
		case eType_NbElements:

		default:
         return 0;

	}

	return 0l;

}

snString snVariant::GetString(bool in_bPadded, size_t in_lPrecision, char in_cDelimiter) const

{
   // clamp precision

   if(in_lPrecision >= float1Formats.size())
      in_lPrecision = float1Formats.size()-1;

   // for each supported type:
	switch(m_eType)

	{

		case eType_Bool:

		{

			return m_uData.b ? "true" : "false";

		}

		case eType_Float:

		{
		   // we hard code the format strings for performance reasons
		   snString result;
         result.Compose(float1Formats[in_lPrecision],m_uData.f);
			return result;

		}

		case eType_Long:

		{

			snString result;

			if(in_bPadded)

				result.Compose("%04ld",m_uData.l);

			else

				result.Compose("%ld",m_uData.l);

			return result;

		}

		case eType_String:

		case eType_TimeStamp:

		{

			if(m_uData.s != NULL)

				return *m_uData.s;
		}

		case eType_VariantVec:
		{
         if(m_uData.pv->size() == 0)
            return "[]";

         snStringArray resultArray;
		   for(size_t i=0;i<m_uData.pv->size();i++)
		   {
		      if(m_uData.pv->at(i)->GetType()==eType_String)
               resultArray.push_back(snString("\"")+m_uData.pv->at(i)->GetString(in_bPadded, in_lPrecision, in_cDelimiter)+snString("\""));
            else
               resultArray.push_back(m_uData.pv->at(i)->GetString(in_bPadded, in_lPrecision, in_cDelimiter));
		   }

		   snString joined, result;
		   joined.Join(resultArray,in_cDelimiter);
		   result.Compose("[%s]",joined.GetAsChar());

		   return result;
		}
		case eType_Vector2f:
		{
		   snString result;
         result.Compose(
            float2Formats[in_lPrecision],
            m_uData.v2->GetX(),
            in_cDelimiter,
            m_uData.v2->GetY());
         return result;
		}

		case eType_Vector3f:

		{
		   snString result;
         result.Compose(
            float3Formats[in_lPrecision],
            m_uData.v3->GetX(),
            in_cDelimiter,
            m_uData.v3->GetY(),
            in_cDelimiter,
            m_uData.v3->GetZ());
         return result;
		}

		case eType_BBoxf:

		{
		   snString result;
         result.Compose(
            float6Formats[in_lPrecision],
            m_uData.bb->GetMinPtr()->GetX(),
            in_cDelimiter,
            m_uData.bb->GetMinPtr()->GetY(),
            in_cDelimiter,
            m_uData.bb->GetMinPtr()->GetZ(),
            in_cDelimiter,
            m_uData.bb->GetMaxPtr()->GetX(),
            in_cDelimiter,
            m_uData.bb->GetMaxPtr()->GetY(),
            in_cDelimiter,
            m_uData.bb->GetMaxPtr()->GetZ());
         return result;
		}

		case eType_Color3f:

		{
		   snString result;
         result.Compose(
            float3Formats[in_lPrecision],
            m_uData.c3->GetR(),
            in_cDelimiter,
            m_uData.c3->GetG(),
            in_cDelimiter,
            m_uData.c3->GetB());
         return result;
		}

		case eType_Color4f:

		{
		   snString result;
         result.Compose(
            float4Formats[in_lPrecision],
            m_uData.c4->GetR(),
            in_cDelimiter,
            m_uData.c4->GetG(),
            in_cDelimiter,
            m_uData.c4->GetB(),
            in_cDelimiter,
            m_uData.c4->GetA());
         return result;
		}

		case eType_Quaternionf:

		{
		   snString result;
         result.Compose(
            float4Formats[in_lPrecision],
            m_uData.q->GetX(),
            in_cDelimiter,
            m_uData.q->GetY(),
            in_cDelimiter,
            m_uData.q->GetZ(),
            in_cDelimiter,
            m_uData.q->GetW());
         return result;
		}

		case eType_Matrix44f:
		{
		   snString result;
		   float * m = m_uData.m->GetArray();
         result.Compose(
            float16Formats[in_lPrecision],
            m[0],in_cDelimiter,
            m[1],in_cDelimiter,
            m[2],in_cDelimiter,
            m[3],in_cDelimiter,
            m[4],in_cDelimiter,
            m[5],in_cDelimiter,
            m[6],in_cDelimiter,
            m[7],in_cDelimiter,
            m[8],in_cDelimiter,
            m[9],in_cDelimiter,
            m[10],in_cDelimiter,
            m[11],in_cDelimiter,
            m[12],in_cDelimiter,
            m[13],in_cDelimiter,
            m[14],in_cDelimiter,
            m[15]);
         return result;
		}

		case eType_FloatArray:

		{
		   snString result;
		   snString singleFloat;
		   for(size_t i=0;i<GetFloatArrayCount();i++)
		   {
		      singleFloat.Compose(float1Formats[in_lPrecision],m_uData.fp[i]);
		      if(i>0)
               result.Append(snString(in_cDelimiter)+singleFloat);
            else
               result.Append(singleFloat);
		   }
         return result;
		}

		case eType_LongArray:

		{
		   snString result;
		   snString singleLong;
		   for(size_t i=0;i<GetLongArrayCount();i++)
		   {
            if(in_bPadded)

               singleLong.Compose("%04ld",m_uData.lp[i]);

            else

               singleLong.Compose("%ld",m_uData.lp[i]);

		      if(i>0)
               result.Append(snString(in_cDelimiter)+singleLong);
            else
               result.Append(singleLong);
		   }
         return result;
		}

		case eType_VoidPtr:
		case eType_NbElements:

		default:
		{
		   return "";
		}

	}

	return "";

}



snpVariantpVec snVariant::GetVariants()

{

   if(m_eType == eType_VariantVec)

      return m_uData.pv;

	return NULL;

}

snVector2f snVariant::GetVector2f() const
{
   if(m_eType == eType_Vector2f)
      return snVector2f(*m_uData.v2);
   return snVector2f();
}

snVector3f snVariant::GetVector3f() const
{
   if(m_eType == eType_Vector3f)
      return snVector3f(*m_uData.v3);
   return snVector3f();
}

snBboxf snVariant::GetBboxf() const
{
   if(m_eType == eType_BBoxf)
      return snBboxf(*m_uData.bb);
   return snBboxf();
}

snColor3f snVariant::GetColor3f() const
{
   if(m_eType == eType_Color3f)
      return snColor3f(*m_uData.c3);
   return snColor3f();
}

snColor4f snVariant::GetColor4f() const
{
   if(m_eType == eType_Color4f)
      return snColor4f(*m_uData.c4);
   return snColor4f();
}

snQuaternionf snVariant::GetQuaternionf() const
{
   if(m_eType == eType_Quaternionf)
      return snQuaternionf(*m_uData.q);
   return snQuaternionf();
}

snTransformf snVariant::GetMatrix44f() const
{
   if(m_eType == eType_Matrix44f)
      return snTransformf(*m_uData.m);
   return snTransformf();
}

float * snVariant::GetFloatArray( )
{
   if(m_eType == eType_FloatArray)
      return m_uData.fp;
   return NULL;
}

long * snVariant::GetLongArray( )
{
   if(m_eType == eType_LongArray)
      return m_uData.lp;
   return NULL;
}

void * snVariant::GetVoidPtr( )
{
   if(m_eType == eType_VoidPtr)
      return m_uData.ptr;
   return NULL;
}


void snVariant::SetBool(bool in_B)

{

	switch(m_eType)

	{

		case eType_Bool:

		{

			m_uData.b = in_B;

			break;

		}

		case eType_Float:

		{

			m_uData.f = in_B ? 1.0f : 0.0f;

			break;

		}

		case eType_Long:

		{

			m_uData.l = in_B ? 1l : 0l;

			break;

		}

		case eType_String:

		case eType_TimeStamp:

		{

			if(m_uData.s != NULL)

				m_uData.s->Set(snString(in_B ? "true" : "false"));

			break;

		}
		case eType_VariantVec:
		case eType_Vector2f:

		case eType_Vector3f:

		case eType_BBoxf:

		case eType_Color3f:

		case eType_Color4f:

		case eType_Quaternionf:

		case eType_Matrix44f:

		case eType_FloatArray:

		case eType_LongArray:

		case eType_VoidPtr:

		case eType_NbElements:

		default:
		{
		   break;
		}

	}

}



void snVariant::SetFloat(float in_F)

{

	switch(m_eType)

	{

		case eType_Bool:

		{

			m_uData.b = in_F > 0.0f;

			break;

		}

		case eType_Float:

		{

			m_uData.f = in_F;

			break;

		}

		case eType_Long:

		{

			m_uData.l = long(in_F);

			break;

		}

		case eType_String:

		case eType_TimeStamp:

		{

			if(m_uData.s != NULL)

				m_uData.s->Compose("%f",in_F);

			break;

		}

		case eType_VariantVec:
		case eType_Vector2f:

		case eType_Vector3f:

		case eType_BBoxf:

		case eType_Color3f:

		case eType_Color4f:

		case eType_Quaternionf:

		case eType_Matrix44f:

		case eType_FloatArray:

		case eType_LongArray:

		case eType_VoidPtr:

		case eType_NbElements:

		default:
		{
		   break;
		}

	}

}



void snVariant::SetLong(long in_L)

{

	switch(m_eType)

	{

		case eType_Bool:

		{

			m_uData.b = in_L > 0l;

			break;

		}

		case eType_Float:

		{

			m_uData.f = (float)in_L;

			break;

		}

		case eType_Long:

		{

			m_uData.l = in_L;

			break;

		}

		case eType_String:

		case eType_TimeStamp:

		{

			if(m_uData.s != NULL)

				m_uData.s->Compose("%d",in_L);

			break;

		}

		case eType_VariantVec:
		case eType_Vector2f:

		case eType_Vector3f:

		case eType_BBoxf:

		case eType_Color3f:

		case eType_Color4f:

		case eType_Quaternionf:

		case eType_Matrix44f:

		case eType_FloatArray:

		case eType_LongArray:

		case eType_VoidPtr:

		case eType_NbElements:

		default:
		{
		   break;
		}
	}

}



void snVariant::SetString(const snString & in_S, char in_cDelimiter)

{

	switch(m_eType)

	{

		case eType_Bool:

		{

			m_uData.b = (in_S == "true") || (in_S == "yes") || (in_S == "on");

			break;

		}

		case eType_Float:

		{

			snString l_strValue(in_S);

			while(l_strValue.GetCharAt() == '0')

				l_strValue.TruncateLeft(1);

			m_uData.f = (float)atof(l_strValue.GetAsChar());

			break;

		}

		case eType_Long:

		{

			snString l_strValue(in_S);

			while(l_strValue.GetCharAt() == '0')

				l_strValue.TruncateLeft(1);

			m_uData.l = atoi(l_strValue.GetAsChar());

			break;

		}

		case eType_String:

		case eType_TimeStamp:

		{

			if(m_uData.s != NULL)

				m_uData.s->Set(in_S);

			break;

		}
		case eType_Vector2f:
		{
		   float x,y;
		   char c;
		   sscanf(in_S.GetAsChar(),"%f%c%f",&x,&c,&y);
		   m_uData.v2->Set(x,y);
		   break;
		}

		case eType_Vector3f:
		{
		   float x,y,z;
		   char c;
		   sscanf(in_S.GetAsChar(),"%f%c%f%c%f",&x,&c,&y,&c,&z);
		   m_uData.v3->Set(x,y,z);
		   break;
		}

		case eType_BBoxf:
		{
		   float x,y,z,x2,y2,z2;
		   char c;
		   sscanf(in_S.GetAsChar(),"%f%c%f%c%f%c%f%c%f%c%f",&x,&c,&y,&c,&z,&c,&x2,&c,&y2,&c,&z2);
		   m_uData.bb->GetMinPtr()->Set(x,y,z);
		   m_uData.bb->GetMaxPtr()->Set(x2,y2,z2);
		   break;
		}

		case eType_Color3f:
		{
		   float r,g,b;
		   char c;
		   sscanf(in_S.GetAsChar(),"%f%c%f%c%f",&r,&c,&g,&c,&b);
		   m_uData.c3->Set(r,g,b);
		   break;
		}

		case eType_Color4f:
		{
		   float r,g,b,a;
		   char c;
		   sscanf(in_S.GetAsChar(),"%f%c%f%c%f%c%f",&r,&c,&g,&c,&b,&c,&a);
		   m_uData.c4->Set(r,g,b,a);
		   break;
		}

		case eType_Quaternionf:
		{
		   float x,y,z,w;
		   char c;
		   sscanf(in_S.GetAsChar(),"%f%c%f%c%f%c%f",&x,&c,&y,&c,&z,&c,&w);
		   m_uData.q->Set(x,y,z,w);
		   break;
		}

		case eType_Matrix44f:
		{
		   float m[16];
		   char c;
		   sscanf(in_S.GetAsChar(),"%f%c%f%c%f%c%f%c%f%c%f%c%f%c%f%c%f%c%f%c%f%c%f%c%f%c%f%c%f%c%f",
            &m[0],&c,&m[1],&c,&m[2],&c,&m[3],&c,&m[4],&c,&m[5],&c,&m[6],&c,&m[7],&c,&m[8],&c,&m[9],&c,&m[10],&c,&m[11],&c,&m[12],&c,&m[13],&c,&m[14],&c,&m[15]);
         m_uData.m->SetArray(m);
		   break;
		}
		case eType_FloatArray:
		{
		   size_t count = in_S.Count(in_cDelimiter)+1;
		   if(count != GetFloatArrayCount())
		   {
            if(m_uData.fp != NULL)
               free(m_uData.fp);
            m_iSize = count * sizeof(float);
            m_uData.fp = (float*)malloc(m_iSize);
		   }
		   snStringArray parts = in_S.Split(in_cDelimiter);
		   if(parts.size() != count)
            return;
         for(size_t i=0;i<parts.size();i++)
            sscanf(parts[i].GetAsChar(),"%f",&m_uData.fp[i]);
		   break;
		}
		case eType_LongArray:
		{
		   size_t count = in_S.Count(in_cDelimiter)+1;
		   if(count != GetLongArrayCount())
		   {
            if(m_uData.lp != NULL)
               free(m_uData.lp);
            m_iSize = count * sizeof(long);
            m_uData.lp = (long*)malloc(m_iSize);
		   }
		   snStringArray parts = in_S.Split(in_cDelimiter);
		   if(parts.size() != count)
            return;
         for(size_t i=0;i<parts.size();i++)
            sscanf(parts[i].GetAsChar(),"%ld",&m_uData.lp[i]);
		   break;
		}
		case eType_VoidPtr:

		case eType_NbElements:

		case eType_VariantVec:

		default:
		   break;
	}

}



void snVariant::SetVariant(const snVariant & in_V)

{

   Set(in_V);
}



void snVariant::SetVector2f(const snVector2f & in_V)
{
   if(!m_eType == eType_Vector2f)
      return;
   m_uData.v2->Set(in_V);
}


void snVariant::SetVector3f(const snVector3f & in_V)

{
   if(!m_eType == eType_Vector3f)
      return;
   m_uData.v3->Set(in_V);
}


void snVariant::SetBboxf(const snBboxf & in_BB)

{
   if(!m_eType == eType_BBoxf)
      return;
   m_uData.bb->Set(in_BB);
}


void snVariant::SetColor3f(const snColor3f & in_C)

{
   if(!m_eType == eType_Color3f)
      return;
   m_uData.c3->Set(in_C);
}


void snVariant::SetColor4f(const snColor4f & in_C)

{
   if(!m_eType == eType_Color4f)
      return;
   m_uData.c4->Set(in_C);
}


void snVariant::SetQuaternionf(const snQuaternionf & in_Q)

{
   if(!m_eType == eType_Quaternionf)
      return;
   m_uData.q->Set(in_Q);
}


void snVariant::SetMatrix44f(const snTransformf & in_M)

{
   if(!m_eType == eType_Matrix44f)
      return;
   m_uData.m->Set(in_M);
}


void snVariant::SetFloatArray(float * in_pFloat, size_t in_iCount)

{
   if(!m_eType == eType_FloatArray)
      return;
   if(m_uData.fp != NULL)
      free(m_uData.fp);
   m_iSize = in_iCount * sizeof(float);
   m_uData.fp = (float*)malloc(m_iSize);

   memcpy(m_uData.fp,in_pFloat,m_iSize);
}


void snVariant::SetLongArray(long * in_pLong, size_t in_iCount)

{
   if(!m_eType == eType_LongArray)
      return;
   if(m_uData.lp != NULL)
      free(m_uData.lp);
   m_iSize = in_iCount * sizeof(long);
   m_uData.lp = (long*)malloc(m_iSize);
   memcpy(m_uData.lp,in_pLong,m_iSize);
}


void snVariant::SetVoidPtr(void * in_Ptr, size_t in_iSize)

{
   if(!m_eType == eType_VoidPtr)
      return;
   if(m_uData.ptr != NULL)
      free(m_uData.ptr);
   m_iSize = in_iSize;
   m_uData.ptr = malloc(m_iSize);
   memcpy(m_uData.ptr,in_Ptr,m_iSize);
}

float * snVariant::AllocateFloatArray(size_t in_iCount)
{
   Clear();
   m_eType = eType_FloatArray;
   m_iSize = sizeof(float) * in_iCount;
   m_uData.fp = (float*)malloc(m_iSize);
   return m_uData.fp;
}

long * snVariant::AllocateLongArray(size_t in_iCount)
{
   Clear();
   m_eType = eType_LongArray;
   m_iSize = sizeof(long) * in_iCount;
   m_uData.lp = (long*)malloc(m_iSize);
   return m_uData.lp;
}

void * snVariant::AllocateVoidPtr(size_t in_iSize)
{
   Clear();
   m_eType = eType_VoidPtr;
   m_iSize = in_iSize;
   m_uData.ptr = malloc(m_iSize);
   return m_uData.ptr;
}

snVariant & snVariant::operator = (const snVariant & in_V)
{
   Set(in_V);
   return *this;
}


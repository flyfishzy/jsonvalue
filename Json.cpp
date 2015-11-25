/*
 * IntegraXor Web SCADA - JsonValue
 * http://www.integraxor.com/
 * author: KHW
 * (c)2010~2011 ecava
 * Dual licensed under the MIT or GPL Version 2 licenses.
 */

#include "StdAfx.h"
#include "Json.h"
#include <float.h>

#pragma warning(disable:4389)


// constructor
JSONVALUE::JSONVALUE() 
{
	vt = VT_NULL;
	jt = JT_NULL;
	val.numVal.llVal = 0i64;
}

JSONVALUE::JSONVALUE(const JSONTYPE nType)
{
	order.clear();
	jt = nType;
	switch (jt) {
		case JT_STRING:
			vt = VT_BSTR;
			val.strVal = new wstring;
			*val.strVal = L"";
			break;
		case JT_NUMBER:
			vt = VT_I4;
			val.numVal.llVal = 0i64;
			break;
		case JT_BOOL:
			vt = VT_BOOL;
			val.boolVal = false;
			break;
		case JT_OBJECT:
			vt = VT_DISPATCH;
			val.objVal = new JSONOBJECT;
			break;
		case JT_ARRAY:
			vt = VT_ARRAY;
			val.arVal = new JSONARRAY;
			break;
		case JT_NULL:
			vt = VT_NULL;
			val.numVal.llVal = 0i64;
			break;
		case JT_UNDEFINED:
		default:
			vt = VT_EMPTY;
			val.numVal.llVal = 0i64;
			break;
	};
}

JSONVALUE::JSONVALUE(const JSONVALUE& src) 
{
	order.clear();
	vt = src.vt;
	jt = src.jt;
	switch (jt) {
		case JT_STRING:
			val.strVal = new wstring;
			*val.strVal = src.val.strVal ? *src.val.strVal : L"";
			break;
		case JT_NUMBER:
			memcpy(&val.numVal, &src.val.numVal, sizeof(val.numVal));
			break;
		case JT_BOOL:
			val.boolVal = src.val.boolVal;
			break;
		case JT_OBJECT:
			val.objVal = new JSONOBJECT;
			if (src.val.objVal && !src.val.objVal->empty()) {
				val.objVal->insert(src.val.objVal->begin(), src.val.objVal->end());
				order.insert(order.end(), src.order.begin(), src.order.end());
			}
			break;
		case JT_ARRAY:
			val.arVal = new JSONARRAY;
			if (src.val.arVal && !src.val.arVal->empty())
				val.arVal->insert(val.arVal->end(), src.val.arVal->begin(), src.val.arVal->end());
			break;
		default:
			val.numVal.llVal = 0i64;
			break;
	};
}

JSONVALUE::JSONVALUE(const wstring& src) 
{
	order.clear();
	vt = VT_BSTR;
	jt = JT_STRING;
	val.strVal = new wstring;
	*val.strVal = src;
}

JSONVALUE::JSONVALUE(const std::string& src)
{
	order.clear();
	vt = VT_BSTR;
	jt = JT_STRING;
	val.strVal = new wstring;
	ConvertAW(*val.strVal, src.c_str());
}

JSONVALUE::JSONVALUE(const wchar_t* src) 
{
	order.clear();
	vt = VT_BSTR;
	jt = JT_STRING;
	val.strVal = new wstring;
	if (src)
		*val.strVal = src;
}

JSONVALUE::JSONVALUE(const char* src)
{
	order.clear();
	vt = VT_BSTR;
	jt = JT_STRING;
	val.strVal = new wstring;
	if (src)
		ConvertAW(*val.strVal, src);
}

JSONVALUE::JSONVALUE(const DECIMAL& src) 
{
	order.clear();
	vt = VT_DECIMAL;
	jt = JT_NUMBER;
	VarR8FromDec((DECIMAL *)&src, &val.numVal.dblVal);
}

JSONVALUE::JSONVALUE(const double src)
{
	order.clear();
	vt = VT_R8;
	jt = JT_NUMBER;
	val.numVal.dblVal = src;
}

JSONVALUE::JSONVALUE(const float src)
{
	order.clear();
	vt = VT_R4;
	jt = JT_NUMBER;
	val.numVal.dblVal = src;
}

JSONVALUE::JSONVALUE(const __int64 src) 
{
	order.clear();
	vt = VT_I8;
	jt = JT_NUMBER;
	val.numVal.llVal = src;
}

JSONVALUE::JSONVALUE(const long src) 
{
	order.clear();
	vt = VT_I4;
	jt = JT_NUMBER;
	val.numVal.llVal = src;
}

JSONVALUE::JSONVALUE(const int src) 
{
	order.clear();
	vt = VT_I4;
	jt = JT_NUMBER;
	val.numVal.llVal = src;
}

JSONVALUE::JSONVALUE(const short src) 
{
	order.clear();
	vt = VT_I2;
	jt = JT_NUMBER;
	val.numVal.llVal = src;
}

JSONVALUE::JSONVALUE(const char src) 
{
	order.clear();
	vt = VT_I1;
	jt = JT_NUMBER;
	val.numVal.llVal = src;
}

JSONVALUE::JSONVALUE(const unsigned __int64 src) 
{
	order.clear();
	vt = VT_UI8;
	jt = JT_NUMBER;
	val.numVal.ullVal = src;
}

JSONVALUE::JSONVALUE(const unsigned long src) 
{
	order.clear();
	vt = VT_UI4;
	jt = JT_NUMBER;
	val.numVal.ullVal = src;
}

JSONVALUE::JSONVALUE(const unsigned int src) 
{
	order.clear();
	vt = VT_UI4;
	jt = JT_NUMBER;
	val.numVal.ullVal = src;
}

JSONVALUE::JSONVALUE(const unsigned short src) 
{
	order.clear();
	vt = VT_UI2;
	jt = JT_NUMBER;
	val.numVal.ullVal = src;
}

JSONVALUE::JSONVALUE(const unsigned char src) 
{
	order.clear();
	vt = VT_UI1;
	jt = JT_NUMBER;
	val.numVal.ullVal = src;
}

JSONVALUE::JSONVALUE(const bool src) 
{
	order.clear();
	vt = VT_BOOL;
	jt = JT_BOOL;
	val.boolVal = src;
}

JSONVALUE::JSONVALUE(const JSONOBJECT& src) 
{
	order.clear();		// NOTE: in this case, order is not preserved
	vt = VT_DISPATCH;
	jt = JT_OBJECT;
	val.objVal = new JSONOBJECT;
	if (!src.empty())
		val.objVal->insert(src.begin(), src.end());
}

JSONVALUE::JSONVALUE(const JSONARRAY& src) 
{
	order.clear();
	vt = VT_ARRAY;
	jt = JT_ARRAY;
	val.arVal = new JSONARRAY;
	if (!src.empty())
		val.arVal->insert(val.arVal->end(), src.begin(), src.end());
}

JSONVALUE::JSONVALUE(const _variant_t& src) 
{
	order.clear();
	vt = src.vt;
	switch (src.vt) {
		// special
		case VT_DATE:
			// save as number
			jt = JT_NUMBER;
			val.numVal.dblVal = src.date;
			break;
		case VT_NULL:
		case VT_EMPTY:
			vt = VT_NULL;
			jt = JT_NULL;
			val.numVal.llVal = 0i64;
			break;

		// normal
		case VT_BSTR:
			jt = JT_STRING;
			val.strVal = new wstring;
			*val.strVal = (LPCTSTR)src.bstrVal;
			break;
		case VT_DECIMAL:
			jt = JT_NUMBER;
			VarR8FromDec((DECIMAL *)&src.decVal, &val.numVal.dblVal);
			break;
		case VT_R4:
			jt = JT_NUMBER;
			val.numVal.dblVal = src.fltVal;
			break;
		case VT_R8:
			jt = JT_NUMBER;
			val.numVal.dblVal = src.dblVal;
			break;
		case VT_I1:
			jt = JT_NUMBER;
			val.numVal.llVal = src.cVal;
			break;
		case VT_I2:
			jt = JT_NUMBER;
			val.numVal.llVal = src.iVal;
			break;
		case VT_I4:
			jt = JT_NUMBER;
			val.numVal.llVal = src.lVal;
			break;
		case VT_I8:
			jt = JT_NUMBER;
			val.numVal.llVal = src.llVal;
			break;
		case VT_UI1:
			jt = JT_NUMBER;
			val.numVal.ullVal = src.bVal;
			break;
		case VT_UI2:
			jt = JT_NUMBER;
			val.numVal.ullVal = src.uiVal;
			break;
		case VT_UI4:
			jt = JT_NUMBER;
			val.numVal.ullVal = src.ulVal;
			break;
		case VT_UI8:
			jt = JT_NUMBER;
			val.numVal.ullVal = src.ullVal;
			break;
		case VT_BOOL:
			jt = JT_BOOL;
			val.boolVal = (VARIANT_FALSE != src.boolVal);
			break;
		default:
			vt = VT_NULL;
			jt = JT_NULL;
			val.numVal.llVal = 0i64;
			break;
	}
}

// destructor
JSONVALUE::~JSONVALUE() 
{
	Clear();
}

void JSONVALUE::Clear() 
{
	order.clear();
	switch (jt) {
		case JT_STRING:
			if (NULL != val.strVal) {
				delete val.strVal;
				val.strVal = NULL;
			}
			break;
		case JT_OBJECT:
			if (NULL != val.objVal) {
				if (!val.objVal->empty())
					val.objVal->clear();
				delete val.objVal;
				val.objVal = NULL;
			}
			break;
		case JT_ARRAY:
			if (NULL != val.arVal) {
				if (!val.arVal->empty())
					val.arVal->clear();
				delete val.arVal;
				val.arVal = NULL;
			}
			break;
		case JT_NUMBER:
			val.numVal.llVal = 0i64;
			break;
		case JT_BOOL:
			val.boolVal = false;
			break;
		default:
			break;
	};
	vt = VT_NULL;
	jt = JT_NULL;
}

// assignment
JSONVALUE& JSONVALUE::operator =(const JSONVALUE& src) 
{
	Clear();
	vt = src.vt;
	jt = src.jt;
	switch (jt) {
		case JT_STRING:
			val.strVal = new wstring;
			*val.strVal = src.val.strVal ? *src.val.strVal : L"";
			break;
		case JT_NUMBER:
			memcpy(&val.numVal, &src.val.numVal, sizeof(val.numVal));
			break;
		case JT_BOOL:
			val.boolVal = src.val.boolVal;
			break;
		case JT_OBJECT:
			val.objVal = new JSONOBJECT;
			if (src.val.objVal && !src.val.objVal->empty()) {
				val.objVal->insert(src.val.objVal->begin(), src.val.objVal->end());
				order.insert(order.end(), src.order.begin(), src.order.end());
			}
			break;
		case JT_ARRAY:
			val.arVal = new JSONARRAY;
			if (src.val.arVal && !src.val.arVal->empty())
				val.arVal->insert(val.arVal->end(), src.val.arVal->begin(), src.val.arVal->end());
			break;
		default:
			break;
	};
	return *this;
}

JSONVALUE& JSONVALUE::operator =(const wstring& src) 
{
	Clear();
	vt = VT_BSTR;
	jt = JT_STRING;
	val.strVal = new wstring;
	*val.strVal = src;
	return *this;
}

JSONVALUE& JSONVALUE::operator =(const std::string& src) 
{
	Clear();
	vt = VT_BSTR;
	jt = JT_STRING;
	val.strVal = new wstring;
	ConvertAW(*val.strVal, src.c_str());
	return *this;
}

JSONVALUE& JSONVALUE::operator =(const wchar_t* src) 
{
	Clear();
	vt = VT_BSTR;
	jt = JT_STRING;
	val.strVal = new wstring;
	if (src)
		*val.strVal = src;
	return *this;
}

JSONVALUE& JSONVALUE::operator =(const char* src) 
{
	Clear();
	vt = VT_BSTR;
	jt = JT_STRING;
	val.strVal = new wstring;
	if (src)
		ConvertAW(*val.strVal, src);
	return *this;
}

JSONVALUE& JSONVALUE::operator =(const DECIMAL& src) 
{
	Clear();
	vt = VT_DECIMAL;
	jt = JT_NUMBER;
	VarR8FromDec((DECIMAL *)&src, &val.numVal.dblVal);
	return *this;
}

JSONVALUE& JSONVALUE::operator =(const double src) 
{
	Clear();
	vt = VT_R8;
	jt = JT_NUMBER;
	val.numVal.dblVal = src;
	return *this;
}

JSONVALUE& JSONVALUE::operator =(const float src) 
{
	Clear();
	vt = VT_R4;
	jt = JT_NUMBER;
	val.numVal.dblVal = src;
	return *this;
}

JSONVALUE& JSONVALUE::operator =(const __int64 src) 
{
	Clear();
	vt = VT_I8;
	jt = JT_NUMBER;
	val.numVal.llVal = src;
	return *this;
}

JSONVALUE& JSONVALUE::operator =(const long src) 
{
	Clear();
	vt = VT_I4;
	jt = JT_NUMBER;
	val.numVal.llVal = src;
	return *this;
}

JSONVALUE& JSONVALUE::operator =(const int src) 
{
	Clear();
	vt = VT_I4;
	jt = JT_NUMBER;
	val.numVal.llVal = src;
	return *this;
}

JSONVALUE& JSONVALUE::operator =(const short src) 
{
	Clear();
	vt = VT_I2;
	jt = JT_NUMBER;
	val.numVal.llVal = src;
	return *this;
}

JSONVALUE& JSONVALUE::operator =(const char src) 
{
	Clear();
	vt = VT_I1;
	jt = JT_NUMBER;
	val.numVal.llVal = src;
	return *this;
}

JSONVALUE& JSONVALUE::operator =(const unsigned __int64 src) 
{
	Clear();
	vt = VT_UI8;
	jt = JT_NUMBER;
	val.numVal.ullVal = src;
	return *this;
}

JSONVALUE& JSONVALUE::operator =(const unsigned long src) 
{
	Clear();
	vt = VT_UI4;
	jt = JT_NUMBER;
	val.numVal.ullVal = src;
	return *this;
}

JSONVALUE& JSONVALUE::operator =(const unsigned int src) 
{
	Clear();
	vt = VT_UI4;
	jt = JT_NUMBER;
	val.numVal.ullVal = src;
	return *this;
}

JSONVALUE& JSONVALUE::operator =(const unsigned short src) 
{
	Clear();
	vt = VT_UI2;
	jt = JT_NUMBER;
	val.numVal.ullVal = src;
	return *this;
}

JSONVALUE& JSONVALUE::operator =(const unsigned char src) 
{
	Clear();
	vt = VT_UI1;
	jt = JT_NUMBER;
	val.numVal.ullVal = src;
	return *this;
}

JSONVALUE& JSONVALUE::operator =(const bool src) 
{
	Clear();
	vt = VT_BOOL;
	jt = JT_BOOL;
	val.boolVal = src;
	return *this;
}

JSONVALUE& JSONVALUE::operator =(const JSONOBJECT& src) 
{
	// NOTE: in this case, order is not preserved
	Clear();
	vt = VT_DISPATCH;
	jt = JT_OBJECT;
	val.objVal = new JSONOBJECT;
	if (!src.empty())
		val.objVal->insert(src.begin(), src.end());
	return *this;
}

JSONVALUE& JSONVALUE::operator =(const JSONARRAY& src) 
{
	Clear();
	vt = VT_ARRAY;
	jt = JT_ARRAY;
	val.arVal = new JSONARRAY;
	if (!src.empty())
		val.arVal->insert(val.arVal->end(), src.begin(), src.end());
	return *this;
}

JSONVALUE& JSONVALUE::operator =(const _variant_t& src) 
{
	Clear();
	vt = src.vt;
	switch (src.vt) {
		// special
		case VT_DATE:
			// save as number
			jt = JT_NUMBER;
			val.numVal.dblVal = src.date;
			break;
		case VT_NULL:
		case VT_EMPTY:
			val.numVal.llVal = 0;
			vt = VT_NULL;
			jt = JT_NULL;
			break;

		// normal
		case VT_BSTR:
			jt = JT_STRING;
			val.strVal = new wstring;
			*val.strVal = (LPCTSTR)src.bstrVal;
			break;
		case VT_DECIMAL:
			jt = JT_NUMBER;
			VarR8FromDec((DECIMAL *)&src.decVal, &val.numVal.dblVal);
			break;
		case VT_R4:
			jt = JT_NUMBER;
			val.numVal.dblVal = src.fltVal;
			break;
		case VT_R8:
			jt = JT_NUMBER;
			val.numVal.dblVal = src.dblVal;
			break;
		case VT_I1:
			jt = JT_NUMBER;
			val.numVal.llVal = src.cVal;
			break;
		case VT_I2:
			jt = JT_NUMBER;
			val.numVal.llVal = src.iVal;
			break;
		case VT_I4:
			jt = JT_NUMBER;
			val.numVal.llVal = src.lVal;
			break;
		case VT_I8:
			jt = JT_NUMBER;
			val.numVal.llVal = src.llVal;
			break;
		case VT_UI1:
			jt = JT_NUMBER;
			val.numVal.ullVal = src.bVal;
			break;
		case VT_UI2:
			jt = JT_NUMBER;
			val.numVal.ullVal = src.uiVal;
			break;
		case VT_UI4:
			jt = JT_NUMBER;
			val.numVal.ullVal = src.ulVal;
			break;
		case VT_UI8:
			jt = JT_NUMBER;
			val.numVal.ullVal = src.ullVal;
			break;
		case VT_BOOL:
			jt = JT_BOOL;
			val.boolVal = (VARIANT_FALSE != src.boolVal);
			break;
		default:
			val.numVal.llVal = 0;
			vt = VT_NULL;
			jt = JT_NULL;
			break;
	}
	return *this;
}

JSONVALUE& JSONVALUE::operator [](const wstring &src)
{
	// if data type is not object, convert
	if (JT_OBJECT != jt) {
		Clear();
		SetObject(this);
	}

	order.push_back(src);
	JSONOBJECT::iterator it = val.objVal->find(src);
	if (val.objVal->end() == it) {
		(*val.objVal)[src] = JSONVALUE(JT_UNDEFINED);
		return (*val.objVal)[src];
	} else {
		return it->second;
	}
}

JSONVALUE& JSONVALUE::operator [](const wchar_t *src)
{
	wstring w(src);
	return operator [](w);
}

JSONVALUE& JSONVALUE::operator [](const std::string &src)
{
	wstring w;
	ConvertAW(w, src.c_str());
	return operator [](w);
}

JSONVALUE& JSONVALUE::operator [](const char *src)
{
	wstring w;
	ConvertAW(w, src);
	return operator [](w);
}

void JSONVALUE::Push(const JSONVALUE &src)
{
	// if data type is not array, convert
	if (JT_ARRAY != jt) {
		Clear();
		SetArray(this);
	}

	val.arVal->push_back(src);
}

bool JSONVALUE::SetAt(const size_t pos,const JSONVALUE &src)
{
	// if array
	if ((JT_ARRAY == jt) && val.arVal && (0 <= pos) && (pos < val.arVal->size())) {
		(*val.arVal)[pos]=src;
		return true;
	}
	// other
	return false;
	//// return false if not array or position invalid
}
size_t JSONVALUE::Size()
{
	switch (jt) {
		case JT_OBJECT:
			return val.objVal ? val.objVal->size() : 0;
		case JT_ARRAY:
			return val.arVal ? val.arVal->size() : 0;
		default:
			return 0;
	}
}

bool JSONVALUE::isNull()
{
	return (JT_UNDEFINED == jt);
}
bool JSONVALUE::isMember(const wchar_t* pos)
{
	JSONVALUE j(JT_UNDEFINED);
	return At(pos,j);
}
LPCTSTR JSONVALUE::asCString()
{
	if(jt==JT_STRING)
		return val.strVal->c_str();
	else
		return L"";
}
wstring JSONVALUE::asString()
{
	if(jt==JT_STRING)
		return *val.strVal;
	else
		return L"";
}
int JSONVALUE::asInt()
{
	switch ( jt )
   {
   case JT_UNDEFINED:
      return 0;
   case JT_NUMBER:
      return val.numVal.llVal;
   case JT_BOOL:
	   return val.boolVal ? 1 : 0;
	}
	return 0;
}
double JSONVALUE::asDouble()
{
	switch ( jt )
	{
	case JT_UNDEFINED:
		return 0;
	case JT_NUMBER:
		{
			if(vt==VT_R4||vt==VT_R8)
				return val.numVal.dblVal;
			else
				return val.numVal.llVal;
		}
	case JT_BOOL:
		return val.boolVal ? 1 : 0;
	}
	return 0;
}
int ParseJsonFile(LPCTSTR fileName,JSONVALUE& jVal)
{
	FILE* fin=NULL;
	//¶ÁÈ¡²¢ÅÐ¶ÏBOM
	_wfopen_s(&fin,fileName,_T("rb"));
	if (fin==NULL)
		return -1;
	BYTE* buf[2];
	BYTE utf16_le[]={0xFF,0xFE};
	BYTE utf16_be[]={0xFE,0xFF};
	size_t nRead=fread((void*)buf,sizeof(BYTE),2,fin);
	fseek(fin,0,SEEK_END);
	int nFileLen=ftell(fin);
	fclose(fin);
	int iUnicode=2;
	if(memcmp(buf,&utf16_be,2)==0||memcmp(buf,&utf16_le,2)==0)
	{//unicode
		iUnicode=2;
	}
	else
		iUnicode=0;
	_wfopen_s(&fin,fileName,_T("r,ccs=UNICODE"));
	fseek(fin,iUnicode,SEEK_SET);
	char* szBuf=new char[nFileLen+1];
	memset(szBuf,0,nFileLen+1);
	fread(szBuf, sizeof(char),nFileLen, fin);
	fclose(fin);
	JSONERROR err;
	if(iUnicode)
	{
		if (!jVal.Parse((wchar_t*)szBuf,JSON_FLAG_LOOSE ,&err))
		{
			delete szBuf;
			return -2;
		}
	}
	else
	{
		if (!jVal.Parse(szBuf,JSON_FLAG_LOOSE ,&err))
		{
			delete szBuf;
			return -2;
		}
	}
	delete szBuf;
	return 0;
}
JSONVALUE& JSONVALUE::At(const size_t pos)
{
	static JSONVALUE j(JT_UNDEFINED);
	At(pos, j);
	return j;
}

JSONVALUE& JSONVALUE::At(const wchar_t* pos)
{
	static JSONVALUE j(JT_UNDEFINED);
	At(pos, j);
	return j;
}

JSONVALUE& JSONVALUE::At(const char* pos)
{
	static JSONVALUE j(JT_UNDEFINED);
	At(pos, j);
	return j;
}

bool JSONVALUE::At(const size_t pos, wstring& _key, JSONVALUE& _val)
{
	// if object
	if ((JT_OBJECT == jt) && val.objVal && (0 <= pos) && (pos < val.objVal->size())) {
		JSONOBJECT::iterator it = val.objVal->find(order.at(pos));
		if (val.objVal->end() != it) {
			_key = it->first;
			_val = it->second;
			return true;
		}
	}
	// other
	return false;
}

bool JSONVALUE::At(const size_t pos, JSONVALUE& _val)
{
	// if array
	if ((JT_ARRAY == jt) && val.arVal && (0 <= pos) && (pos < val.arVal->size())) {
		_val = val.arVal->at(pos);
		return true;
	}
	// other
	return false;
	//// return false if not array or position invalid
	//if ((JT_ARRAY != jt) || (NULL == val.arVal) || (val.arVal->size() <= pos))
	//	return false;
	//_val = val.arVal->at(pos);
	//return true;
}

bool JSONVALUE::At(const wchar_t* pos, JSONVALUE& _val)
{
	// return false if not object or position invalid
	if ((JT_OBJECT != jt) || (NULL == val.objVal) || (NULL == pos))
		return false;
	// return false if key not found
	JSONOBJECT::iterator it = val.objVal->find(pos);
	if (val.objVal->end() == it)
		return false;
	_val = it->second;
	return true;
}

bool JSONVALUE::At(const char* pos, JSONVALUE& val)
{
	wstring w;
	ConvertAW(w, pos);
	return At(w.c_str(), val);
}

// conversion
const char* JSONVALUE::ToString(std::string& sz, const DWORD nFlag, const JSONFORMAT* pFormat)
{
	wstring szw;
	ToString(szw, nFlag, pFormat);
	return ConvertWA(sz, szw.c_str());
}

bool JSONVALUE::Parse(const std::string& sz, const DWORD nFlag, JSONERROR *pError)
{
	wstring szw;
	if (NULL == ConvertAW(szw, sz.c_str()))
		return false;
	return Parse(szw, nFlag, pError);
}

const wchar_t* JSONVALUE::ToString(wstring& sz, const DWORD nFlag, const JSONFORMAT* pFormat) 
{
	// NOTE: when formatting value, remove JSON_FLAG_ALLOWUNQUOTENAME flag

	sz = L"";
	bool bPrettyPrint = HasFlag(JSON_FLAG_PRETTYPRINTTAB, nFlag) ||
		HasFlag(JSON_FLAG_PRETTYPRINTSPACE, nFlag);

	// format
	JSONFORMAT format;
	if (NULL != pFormat) {
		format = *pFormat;
//		memcpy(&format, pFormat, sizeof(JSONFORMAT));
	//} else {
	//	format.nIndentLevel = 0;
	//	format.nIndentSpace = 2;
	//	format.nDecimalPoint = 6;
	//	format.szNumberFormat = L"%.*f";
	}
	wstring szNumberFormat;
	if (NULL == format.szNumberFormat)
		szNumberFormat = (format.nDecimalPoint >= 0) ? L"%.*f" : L"%f";
	else
		szNumberFormat = format.szNumberFormat;
	if (NULL != pFormat) {
		// verify number format
		wstring::size_type pos = szNumberFormat.find(L".*");
		if (format.nDecimalPoint < 0) {
			// if auto decimal point, replace '.*' to '.' in format
			if (wstring::npos != pos) {
				wstring temp = szNumberFormat.substr(0, pos +1) + szNumberFormat.substr(pos +2);
				szNumberFormat = temp;
			}
		} else {
			// if specific decimal point, ensure there is '.*'
			if (wstring::npos == pos) {
				pos = szNumberFormat.find(L".");
				if (wstring::npos != pos) {
					wstring temp = szNumberFormat.substr(0, pos +1) + L"*";
					temp += szNumberFormat.substr(pos +1);
					szNumberFormat = temp;
				} else {
					// ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.en/dv_vccrt/html/dc59ea4e-d23a-4f1f-9881-2c919ebefb82.htm
					pos = szNumberFormat.find_first_of(L"aAdiuoxXeEfgGsS");
					if (wstring::npos != pos) {
						wstring temp = szNumberFormat.substr(0, pos) + L".*";
						temp += szNumberFormat.substr(pos);
						szNumberFormat = temp;
					}
				}
			}
		}
	}

	// stringify
	switch (jt) {
		case JT_NULL:
			sz = JSON_LITERAL_NULL;
			break;
		case JT_STRING:
			FormatString(sz, val.strVal ? *val.strVal : L"", nFlag, false);
			break;
		case JT_NUMBER:
			switch (vt) {
				case VT_I1:
				case VT_I2:
				case VT_I4:
				case VT_I8:
					{
						// _I64_MIN = -9,223,372,036,854,775,808 = max 20 char
						wchar_t* sVal = new wchar_t[21];
						memset(sVal, 0, 21);
						#if defined(_MSC_VER) && (_MSC_VER >= 1400)
						_i64tow_s(val.numVal.llVal, sVal, 21, 10);
						#else
						_i64tow(val.numVal.llVal, sVal, 10);
						#endif
						sz = sVal;
						delete[] sVal;
					}
					break;
				case VT_UI1:
				case VT_UI2:
				case VT_UI4:
				case VT_UI8:
					{
						// _UI64_MAX = 18,446,744,073,709,551,615 = max 20 char
						wchar_t* sVal = new wchar_t[21];
						memset(sVal, 0, 21);
						#if defined(_MSC_VER) && (_MSC_VER >= 1400)
						_ui64tow_s(val.numVal.ullVal, sVal, 21, 10);
						#else
						_ui64tow(val.numVal.ullVal, sVal, 10);
						#endif
						sz = sVal;
						delete[] sVal;
					}
					break;
				case VT_R4:
				case VT_R8:
				case VT_DECIMAL:
					{
						// DBL_MAX = 1.7976931348623158e+308 = max 308 char before decimal point
						int MaxChar = 308 +1 +1;
						if (format.nDecimalPoint > 0)
							MaxChar += format.nDecimalPoint;
						wchar_t* sVal = new wchar_t[MaxChar];
						memset(sVal, 0, MaxChar);
						#if defined(_MSC_VER) && (_MSC_VER >= 1400)
						if (format.nDecimalPoint >= 0)
							swprintf_s(sVal, MaxChar, szNumberFormat.c_str(), format.nDecimalPoint, val.numVal.dblVal);
						else
							swprintf_s(sVal, MaxChar, szNumberFormat.c_str(), val.numVal.dblVal);
						#else
						if (format.nDecimalPoint >= 0)
							swprintf(sVal, MaxChar, szNumberFormat.c_str(), format.nDecimalPoint, val.numVal.dblVal);
						else
							swprintf(sVal, MaxChar, szNumberFormat.c_str(), val.numVal.dblVal);
						#endif
						sz = sVal;
						delete[] sVal;
					}
					break;
				case VT_DATE:
					if (HasFlag(JSON_FLAG_LOCALEDATE, nFlag)) {
						BSTR bstr = NULL;
						VarBstrFromDate(val.numVal.dblVal, LOCALE_SYSTEM_DEFAULT,
							LOCALE_NOUSEROVERRIDE, &bstr);
						FormatString(sz, (LPCWSTR)bstr, nFlag, false);
						SysFreeString(bstr);
					} else {
						wstring stime;
						ToIsoTime(stime, (DATE)val.numVal.dblVal, format.nDateFormat);
						FormatString(sz, stime, nFlag, false);
						// default JSON_FLAG_ISODATE
					//	SYSTEMTIME* sdt = new SYSTEMTIME;
					//	memset(sdt, 0, sizeof(SYSTEMTIME));
					//	VariantTimeToSystemTime(val.numVal.dblVal, sdt);
					//	// yyyy-mm-dd hh:mm:ss = max 19 char
					//	wchar_t* sVal = new wchar_t[20];
					//	memset(sVal, 0, 20);
					//	#if defined(_MSC_VER) && (_MSC_VER >= 1400)
					//	swprintf_s(sVal, 20, L"%04d-%02d-%02d %02d:%02d:%02d",
					//		sdt->wYear, sdt->wMonth, sdt->wDay,
					//		sdt->wHour, sdt->wMinute, sdt->wSecond);
					//	#else
					//	swprintf(sVal, 20, L"%04d-%02d-%02d %02d:%02d:%02d",
					//		sdt->wYear, sdt->wMonth, sdt->wDay,
					//		sdt->wHour, sdt->wMinute, sdt->wSecond);
					//	#endif
					//	FormatString(sz, sVal, nFlag, false);
					//	delete[] sVal;
					//	delete sdt;
					}
					break;
				default:
					break;
			}
			break;
		case JT_BOOL:
			sz = val.boolVal ? JSON_LITERAL_TRUE : JSON_LITERAL_FALSE;
			break;
		case JT_OBJECT:
			sz = JSON_OBJECT_START;
			if (val.objVal && !val.objVal->empty()) {
				wstring szIndent = L"", cIndent = L"";
				JSONFORMAT formatNew;
				memcpy(&formatNew, &format, sizeof(JSONFORMAT));
				lstrcpy(formatNew.szNumberFormat, szNumberFormat.c_str());
				if (bPrettyPrint)
					bPrettyPrint = (val.objVal->size() > 1);
				if (bPrettyPrint) {
					// newline and increase indent
					if (HasFlag(JSON_FLAG_PRETTYPRINTSPACE, nFlag)) {
						if (format.nIndentSpace > 0) {
							for (DWORD i = 0; i < format.nIndentSpace; i++)
								cIndent += JSON_SPACE;
						}
					} else {
						cIndent = JSON_INDENTTAB;
					}
					sz += JSON_NEWLINE;
					for (DWORD i = 0; i < format.nIndentLevel; i++)
						szIndent += cIndent;
					sz += szIndent + cIndent;
					formatNew.nIndentLevel++;
				}
				wstring szString;
				bool bFirstItem = true;
				if (HasFlag(JSON_FLAG_ORDEREDOBJECT, nFlag) || order.empty()) {
					// output follow key name order
					for (JSONOBJECT::const_iterator it = val.objVal->begin(); it != val.objVal->end(); ++it)
						ObjectToString(sz, it->first, it->second, nFlag, formatNew, bFirstItem, bPrettyPrint, szIndent, cIndent);
				} else {
					JSONOBJECT obj;
					obj.insert(val.objVal->begin(), val.objVal->end());
					JSONOBJECT::iterator it;
					// output follow insertion sequence saved into 'order' vector
					for (JSONOBJECTORDER::iterator itorder = order.begin(); itorder != order.end(); ++itorder) {
						it = obj.find(*itorder);
						if (obj.end() != it) {
							ObjectToString(sz, it->first, it->second, nFlag, formatNew, bFirstItem, bPrettyPrint, szIndent, cIndent);
							obj.erase(it);
						}
					}
					if (!obj.empty()) {
						for (it = obj.begin(); it != obj.end(); ++it)
							ObjectToString(sz, it->first, it->second, nFlag, formatNew, bFirstItem, bPrettyPrint, szIndent, cIndent);
					}
				}
				if (bPrettyPrint) {
					// newline and decrease indent
					sz += JSON_NEWLINE;
					sz += szIndent;
				}
			}
			sz += JSON_OBJECT_END;
			break;
		case JT_ARRAY:
			sz = JSON_ARRAY_START;
			if (val.arVal && !val.arVal->empty()) {
				wstring szIndent = L"", cIndent = L"";
				JSONFORMAT formatNew;
				memcpy(&formatNew, &format, sizeof(JSONFORMAT));
				lstrcpy(formatNew.szNumberFormat, szNumberFormat.c_str());
				if (bPrettyPrint)
					bPrettyPrint = (val.arVal->size() > 1);
				if (bPrettyPrint) {
					// newline and increase indent
					if (HasFlag(JSON_FLAG_PRETTYPRINTSPACE, nFlag)) {
						if (format.nIndentSpace > 0) {
							for (DWORD i = 0; i < format.nIndentSpace; i++)
								cIndent += JSON_SPACE;
						}
					} else {
						cIndent = JSON_INDENTTAB;
					}
					sz += JSON_NEWLINE;
					for (DWORD i = 0; i < format.nIndentLevel; i++)
						szIndent += cIndent;
					sz += szIndent + cIndent;
					formatNew.nIndentLevel++;
				}
				wstring szString;
				bool bFIrstItem = true;
				for (JSONARRAY::iterator it = val.arVal->begin(); it != val.arVal->end(); ++it) {
					if (false == bFIrstItem) {
						sz += JSON_ARRAY_SEPARATOR;
						if (true == bPrettyPrint) {
							sz += JSON_NEWLINE;
							sz += szIndent + cIndent;
						}
					}
					it->ToString(szString, nFlag, &formatNew);
					sz += szString;
					bFIrstItem = false;
				}
				if (bPrettyPrint) {
					// newline and decrease indent
					sz += JSON_NEWLINE;
					sz += szIndent;
				}
			}
			sz += JSON_ARRAY_END;
			break;
		default:
			break;
	};

	return sz.c_str();
}

void JSONVALUE::ObjectToString(wstring& sz, const wstring& szKey, const JSONVALUE& jVal, const DWORD nFlag, const JSONFORMAT& formatNew,
	bool& bFirstItem, const bool bPrettyPrint, const wstring& szIndent, const wstring& cIndent)
{
	// if value is not undefined
	if (false == ((JSONVALUE)jVal).isNull()) {
		wstring szString;

		if (false == bFirstItem) {
			sz += JSON_ARRAY_SEPARATOR;
			if (true == bPrettyPrint) {
				sz += JSON_NEWLINE;
				sz += szIndent + cIndent;
			}
		}
		FormatString(szString, szKey, nFlag, true);
		sz += szString;
		sz += JSON_VALUE_SEPARATOR;
		if (bPrettyPrint)
			sz += JSON_SPACE;
		((JSONVALUE)jVal).ToString(szString, nFlag, &formatNew);
		sz += szString;
		bFirstItem = false;
	}
}

bool JSONVALUE::Parse(const wstring& sz, const DWORD nFlag, JSONERROR *pError)
{
	Clear();
	m_stage.clear();
	SetError(pError, sz, 0, NULL);

	if (sz.empty()) {
		SetError(pError, sz, 0, L"Empty string.");
		return false;
	}

	bool bAllowUnquoteName = HasFlag(JSON_FLAG_ALLOWUNQUOTENAME, nFlag),
		bAllowWhitespaceControlChar = HasFlag(JSON_FLAG_ALLOWWSCNTRLCHAR, nFlag),
		bAllowSingleQuote = HasFlag(JSON_FLAG_ALLOWSINGLEQUOTE, nFlag);

	wstring sName,				// object name
		sVal;					// array value
	wchar_t c = 0,				// current character
		lastc = 0,				// last character
		namequote = 0;
	JSONVALUE *pCur = this;		// current json value
	bool bError = false,		// error?
		bStart = false;			// first json construct found
	wstring::size_type pos = 0;	// current position
	int nIndex = 0;				// current scope index

	// check every character
	for (wstring::const_iterator it = sz.begin(); (it != sz.end()) && !bError; ++it) {

		// get current character
		pos = it - sz.begin();
		c = *it;
		if (iswcntrl(c)) {
			if (bAllowWhitespaceControlChar && iswspace(c)) {
				// continue checking if allow white-space control character
				// and this character is white-space character
				bError = false;
			} else {
				bError = true;
					SetError(pError, sz, pos, bAllowWhitespaceControlChar ?
						L"Invalid control character." : L"Invalid whitespace character.");
				break;
			}
		}

		// get current scope
		JSONSCOPES cur;
		nIndex = GetCurrentScope(&cur);
		if (bStart && (0 == nIndex) && !iswspace(c)) {
			// if parsing has already started, and a scope has already closed, and we found a non-whitespace
			// the json text is likely to start another scope, which is invalid.
			bError = true;
			SetError(pError, sz, pos, L"Invalid character after a matching close.");
			break;
		}

		pCur = cur.pValue;
		switch (cur.nScope) {

			// start -----------------------------------------------------------
			case JS_NONE:
				switch (c) {
					case wchar_t('/'):
						// / signal starts of comment
						SetNextScope(JS_COMMENT, NULL);
						break;
					case wchar_t('{'):
						// { signals start of object
						SetObject(this);
						SetNextScope(JS_OBJECT, this);
						bStart = true;
						break;
					case wchar_t('['):
						// [ signals start of array
						SetArray(this);
						SetNextScope(JS_ARRAY, this);
						bStart = true;
						break;
					default:
						// ignore if whitespace, otherwise error
						if (!iswspace(c)) {
							bError = true;
							SetError(pError, sz, pos, L"Invalid first character, expected array, object or comment.");
						}
						break;
				}
				break;
			// comment ---------------------------------------------------------
			case JS_COMMENT:
				switch (cur.nSubScope) {
					case JS_NONE:
						// if possible new comment
						switch (c) {
							case wchar_t('*'):
								// /* signals start of comment block
								SetCurrentSubScope(JS_COMMENTBLOCK);
								break;
							case wchar_t('/'):
								// // signals start of comment line
								SetCurrentSubScope(JS_COMMENTLINE);
								break;
							default:
								// any other than /* and // is error
								bError = true;
								SetError(pError, sz, pos, L"Invalid comment block.");
								break;
						}
						break;
					case JS_COMMENTBLOCK:
						// if /* comment block
						switch (c) {
							case wchar_t('/'):
								// if */ found, comment block ends
								if (wchar_t('*') == lastc)
									RevertScope(1);
								break;
							default:
								// continue scanning
								break;
						}
						break;
					case JS_COMMENTLINE:
						// if // comment line
						switch (c) {
							case wchar_t('\r'):
							case wchar_t('\n'):
								// if \r or \n found, comment line ends
								RevertScope(1);
								break;
							default:
								// continue scanning
								break;
						}
						break;
					default:
						// unreacheable place
						break;
				}
				break;
			// object ----------------------------------------------------------
			case JS_OBJECT:
				switch (cur.nSubScope) {
					case JS_NONE:
					case JS_NAME:
						// start of object
						switch (c) {
							case wchar_t('/'):
								// comment starts
								SetNextScope(JS_COMMENT, NULL);
								break;
							case wchar_t('"'):
								// in either strict or loose mode, name can start with "
								SetCurrentSubScope(JS_NAMESTRICT);
								namequote = c;
								break;
							case wchar_t(','):
								// if value separator
								if (pCur->val.objVal->empty()) {
									// only valid if already contain at least 1 item
									bError = true;
									SetError(pError, sz, pos, L"Invalid separator at start of object.");
								} else if (JS_NAME == cur.nSubScope) {
									// , followed by , empty value
									bError = true;
									SetError(pError, sz, pos, L"Invalid extra separator, expected object name.");
								} else {
									// if more than 1 item, comma (,) signals start of another name
									SetCurrentSubScope(JS_NAME);
									sName.clear();
								}
								break;
							case wchar_t('}'):
								if (JS_NAME == cur.nSubScope) {
									// , followed by }, extra comma
									bError = true;
									SetError(pError, sz, pos, L"Invalid end of object, expected another object name after separator.");
								} else {
									// end of object block, this is an empty object
									RevertScope(1);
									SetCurrentSubScope(JS_NONE);
								}
								break;
							case wchar_t('\''):
								if (bAllowSingleQuote) {
									// if allow single quote, start name
									SetCurrentSubScope(JS_NAMESTRICT);
									namequote = c;
								} else {
									// invalid quote
									bError = true;
									SetError(pError, sz, pos, L"Invalid single quote.");
								}
								break;
							default:
								if (!iswspace(c)) {
									if (bAllowUnquoteName) {
										// if allow unquote name, name can start with non-whitespace
										SetCurrentSubScope(JS_NAMELOOSE);
										namequote = 0;
										sName.push_back(c);
									} else {
										// if strict json, no other format allowed
										bError = true;
										SetError(pError, sz, pos, L"Invalid unquoted object name.");
									}
								}
								break;
						}
						break;
					case JS_NAMESTRICT:
						switch (c) {
							case wchar_t('"'):
							case wchar_t('\''):
								if (c == namequote) {
									// possible end of strict name
									wstring temp = sName;
									if (JSON_VALID == UnescapeString(temp)) {
										sName = temp;
										SetCurrentSubScope(JS_NAME_END);
										namequote = 0;
									} else
										sName.push_back(c);
								} else
									sName.push_back(c);
								break;
							default:
								sName.push_back(c);
								break;
						}
						break;
					case JS_NAMELOOSE:
						switch (c) {
							case wchar_t('*'):
							case wchar_t('/'):
								if (wchar_t('/') == lastc) {
									// /* or // signals start of comment, thus ends the name
								//	(*pCur->val.objVal)[sName.substr(0, sName.length() -1)] = JSONVALUE();
									(*pCur)[sName.substr(0, sName.length() -1)] = JSONVALUE();
									SetNextScope(JS_COMMENT, NULL);
								} else
									sName.push_back(c);
								break;
							case wchar_t(':'):
								// : signals start of value, thus ends the name
								(*pCur)[sName] = JSONVALUE(); //(*pCur->val.objVal)[sName] = JSONVALUE();
								SetCurrentSubScope(JS_VALUE);
								break;
							default:
								if (iswspace(c)) {
									// any whitespace signals end of loose name
									(*pCur)[sName] = JSONVALUE(); //(*pCur->val.objVal)[sName] = JSONVALUE();
									SetCurrentSubScope(JS_NAME_END);
								} else
									sName.push_back(c);
								break;
						}
						break;
					case JS_NAME_END:
						switch (c) {
							case wchar_t('/'):
									SetNextScope(JS_COMMENT, NULL);
								break;
							case wchar_t(':'):
								(*pCur)[sName] = JSONVALUE(); //(*pCur->val.objVal)[sName] = JSONVALUE();
								SetCurrentSubScope(JS_VALUE);
								break;
							default:
								if (!iswspace(c)) {
									// if not whitespace, error
									bError = true;
									SetError(pError, sz, pos, L"Invalid end of object name, expected colon not found.");
								}
								break;
						}
						break;
					case JS_VALUE:
						switch (c) {
							case wchar_t('/'):
								SetNextScope(JS_COMMENT, NULL);
								break;
							case wchar_t('{'):
								// object value
								pCur = &(*cur.pValue->val.objVal)[sName];
								sName.clear();
								SetObject(pCur);
								SetNextScope(JS_OBJECT, pCur);
								break;
							case wchar_t('['):
								// array value
								pCur = &(*cur.pValue->val.objVal)[sName];
								sName.clear();
								SetArray(pCur);
								SetNextScope(JS_ARRAY, pCur);
								break;
							default:
								if (!iswspace(c)) {
									pCur = &(*cur.pValue->val.objVal)[sName];
									SetString(pCur);
									pCur->val.strVal->push_back(c);
									SetCurrentSubScope(JS_VALUEOTHER);
								}
								break;
						}
						break;
					case JS_VALUEOTHER:
						// for string, number, boolean and null, comma (,) signals end
						{
							pCur = &(*cur.pValue->val.objVal)[sName];
							pCur->val.strVal->push_back(c);
							wstring strVal = *pCur->val.strVal;
							TrimString(strVal, 0);
							if (!strVal.empty()) {

								// if this is possible a string, we always check value
								// otherwise, we check value only if comma signal
								bool bCheckValue = false,
									bNonStringEnd = false;
								if (wchar_t('"') == strVal.at(0)) {
									bCheckValue = true;
								} else if ((wchar_t('\'') == strVal.at(0)) && bAllowSingleQuote) {
									bCheckValue = true;
								} else {
									bNonStringEnd = (wchar_t(',') == c) || (wchar_t('}') == c);
									if (bNonStringEnd) {
										bCheckValue = true;
										// remove the trailing , or }
										wstring temp = strVal.substr(0, strVal.length() -1);
										strVal = temp;
									}
								}

								// check value
								if (true == bCheckValue) {
									JSONVALUE jval;
									int isstr = JsonFromString(&jval, strVal, nFlag);
									switch (isstr) {
										case JSON_VALID:
											*pCur = jval;
											sName.clear();
											if (wchar_t(',') == c) {
												SetCurrentSubScope(JS_NAME);
											} else {
												SetCurrentSubScope(JS_NONE);
												if (wchar_t('}') == c) {
													RevertScope(1);
													SetCurrentSubScope(JS_NONE);
												}
											}
											break;
										case JSON_INVALID:
											bError = true;
											SetError(pError, sz, pos, L"Invalid string format.");
											break;
										default:
											if (bNonStringEnd) {
												bError = true;
												SetError(pError, sz, pos, L"Invalid end of string.");
											}
											break;
									}
								}
							}
						}
						break;
					default:
						break;
				}
				break;

			// array -----------------------------------------------------------
			case JS_ARRAY:
				switch (cur.nSubScope) {
					case JS_NONE:	// start of array
					case JS_VALUE:	// start of value
				switch (c) {
							case wchar_t('/'):
								// comment starts
								SetNextScope(JS_COMMENT, NULL);
						break;
							case wchar_t('{'):
						// object value
								cur.pValue->val.arVal->push_back(JSONVALUE());
								pCur = &*cur.pValue->val.arVal->rbegin();
								SetObject(pCur);
								SetNextScope(JS_OBJECT, pCur);
						break;
							case wchar_t('['):
						// array value
								cur.pValue->val.arVal->push_back(JSONVALUE());
								pCur = &*cur.pValue->val.arVal->rbegin();
								SetArray(pCur);
								SetNextScope(JS_ARRAY, pCur);
								break;
							case wchar_t(','):
								// if value separator
								if (pCur->val.arVal->empty()) {
									// only valid if already contain at least 1 item
									bError = true;
									SetError(pError, sz, pos, L"Invalid separator at start of array.");
								} else if (JS_VALUE == cur.nSubScope) {
									// , followsed by , empty value
									bError = true;
									SetError(pError, sz, pos, L"Invalid extra separator, expected value.");
								} else {
									// if more than 1 item, comma (,) signals start of another value
									SetCurrentSubScope(JS_VALUE);
								}
								break;
							case wchar_t(']'):
								if (JS_VALUE == cur.nSubScope) {
									// , followed by ], extra comma
									bError = true;
									SetError(pError, sz, pos, L"Invalid end of array, expected another value after separator.");
								} else {
									// [ followed by ], end of array block, this is an empty array
									RevertScope(1);
									SetCurrentSubScope(JS_NONE);
								}
						break;
					default:
								if (!iswspace(c)) {
									SetCurrentSubScope(JS_VALUEOTHER);
									sVal = c;
								}
								break;
						}
						break;
					case JS_VALUEOTHER:
						// for string, number, boolean and null, comma (,) signals end
						{
							sVal.push_back(c);
							wstring strVal = sVal;
							TrimString(strVal, 0);
							if (!strVal.empty()) {

								// if this is possible a string, we always check value
								// otherwise, we check value only if comma signal
								bool bCheckValue = false,
									bNonStringEnd = false;
								if (wchar_t('"') == strVal.at(0)) {
									bCheckValue = true;
								} else {
									bNonStringEnd = (wchar_t(',') == c) || (wchar_t(']') == c);
									if (bNonStringEnd) {
										bCheckValue = true;
										// remove the trailing , or ]
										wstring temp = strVal.substr(0, strVal.length() -1);
										strVal = temp;
									}
								}

								// check value
								if (true == bCheckValue) {
									JSONVALUE jval;
									int isstr = JsonFromString(&jval, strVal, nFlag);
									switch (isstr) {
										case JSON_VALID:
											pCur->val.arVal->push_back(jval);
											sVal.clear();
											SetCurrentSubScope(JS_NONE);
											if (wchar_t(']') == c) {
												RevertScope(1);
												SetCurrentSubScope(JS_NONE);
											}
											break;
										case JSON_INVALID:
											bError = true;
											SetError(pError, sz, pos, L"Invalid string format.");
											break;
										default:
											if (bNonStringEnd) {
												bError = true;
												SetError(pError, sz, pos, L"Invalid end of string.");
						}
						break;
				}
								}
							}
						}
				break;
					default:
						break;
				}
				break;
			default:
				// unreacheable place
				break;
		}

		// remember last char
		lastc = c;
	}

	// check whether the json string is closed correctly
	if (!bError && !m_stage.empty()) {
		Clear();
		m_stage.clear();
		bError = true;
		SetError(pError, sz, pos +1, L"Invalid format, matching closing character not found.");
	}

	return !bError;
}

void JSONVALUE::SetString(JSONVALUE* p)
{
	if (NULL == p)
		return;
	p->order.clear();
	p->jt = JT_STRING;
	p->vt = VT_BSTR;
	p->val.strVal = new wstring;
}

void JSONVALUE::SetObject(JSONVALUE* p)
{
	if (NULL == p)
		return;
	p->order.clear();
	p->jt = JT_OBJECT;
	p->vt = VT_DISPATCH;
	p->val.objVal = new JSONOBJECT;
}

void JSONVALUE::SetArray(JSONVALUE* p)
{
	if (NULL == p)
		return;
	p->order.clear();
	p->jt = JT_ARRAY;
	p->vt = VT_ARRAY;
	p->val.arVal = new JSONARRAY;
}


int JSONVALUE::JsonFromString(JSONVALUE* dst, const wstring& sz, const DWORD nFlag)
{
	if (NULL == dst)
		return JSON_CHECKAGAIN;

	int bOk = JSON_CHECKAGAIN;

	wstring strVal = TrimString((wstring&)sz, 0);
	if (true == strVal.empty()) {
		// if empty, error
		bOk = JSON_CHECKAGAIN;
	} else if (0 == strVal.compare(JSON_LITERAL_TRUE)) {
		// true
		*dst = true;
		bOk = JSON_VALID;
	} else if (0 == strVal.compare(JSON_LITERAL_FALSE)) {
		// false
		*dst = false;
		bOk = JSON_VALID;
	} else if (0 == strVal.compare(JSON_LITERAL_NULL)) {
		// null
		dst->Clear();
		bOk = JSON_VALID;
	} else {
		// string or number
		bool bAllowSingleQuote = HasFlag(JSON_FLAG_ALLOWSINGLEQUOTE, nFlag);

		wstring::size_type strl = strVal.length();
		wchar_t cStart = strVal.at(0),
			cEnd = 0;
		if (strl > 1)
			cEnd = strVal.at(strl -1);
		bool bString = ((wchar_t('"') == cStart) && (wchar_t('"') == cEnd)) ||
			((wchar_t('\'') == cStart) && (wchar_t('\'') == cEnd) && bAllowSingleQuote);
		if (bString) {
			// if starts and ends with ", possible string
			wstring content = strVal.substr(1, strl -2);		// check content
			bOk = UnescapeString(content);
			if (JSON_VALID == bOk)
				*dst = content;
		} else if ((wchar_t('"') != cStart) && (wchar_t('"') != cEnd)) {
			// if not starts and ends with ", this is number
			wchar_t* wEnd = NULL;
			if (wstring::npos == strVal.find_first_of(L".eE")) {
				// no decimal point, integer
				bool bAllowHexNumber = HasFlag(JSON_FLAG_ALLOWHEXNUMBER, nFlag),
					bAllowOctNumber = HasFlag(JSON_FLAG_ALLOWOCTNUMBER, nFlag),
					bCheck = false;
				if (1 == strl) {
					// string length = 1, process
					bCheck = true;
				} else if (wchar_t('0') != strVal.at(0)) {
					// string length at least 2, and not starts with 0, process
					bCheck = true;
				} else if (!bAllowHexNumber && !bAllowOctNumber) {
					// string length at least 2, starts with 0, but oct and hex not allowed, invalid
					bOk = JSON_INVALID;
				} else {
					// string length at least 2, starts with 0, either oct or hex allowed
					wchar_t c2 = strVal.at(1);
					switch (c2) {
						case wchar_t('x'):
						case wchar_t('X'):
							// if starts with 0x or 0X, and hex allowed, process, otherwise invalid
							if (bAllowHexNumber)
								bCheck = true;
							else
								bOk = JSON_INVALID;
							break;
						default:
							// if starts with 0n, and oct allowed, process, otherwise invalid
							if (bAllowOctNumber && iswdigit(c2))
								bCheck = true;
							else
								bOk = JSON_INVALID;
							break;
					}
				} 
				if (bCheck) {
					// check for signed whole number
					__int64 llVal = _wcstoi64(strVal.c_str(), &wEnd, 10);
					if ((NULL == wEnd) || (wEnd - strVal.c_str() == strl)) {
						// scale down to lowest data type
						if ((llVal >= _I8_MIN) && (llVal <= _I8_MAX))
							*dst = (char)llVal;		// 8 bit signed
						else if ((llVal >= _I16_MIN) && (llVal <= _I16_MAX))
							*dst = (short)llVal;	// 16 bit signed
						else if ((llVal >= _I32_MIN) && (llVal <= _I32_MAX))
							*dst = (long)llVal;		// 32 bit signed
						else
							*dst = llVal;			// 64 bit signed
						bOk = JSON_VALID;
					} else {
						// check for unsigned whole number
						wEnd = NULL;
						unsigned __int64 ullVal = _wcstoui64(strVal.c_str(), &wEnd, 10);
						if ((NULL == wEnd) || (wEnd - strVal.c_str() == strl)) {
							// scale down to lowest data type
							if (ullVal <= _UI8_MAX)
								*dst = (unsigned char)ullVal;	// 8 bit unsigned
							else if (ullVal <= _UI16_MAX)
								*dst = (unsigned short)ullVal;	// 16 bit unsigned
							else if (ullVal <= _UI32_MAX)
								*dst = (unsigned long)ullVal;	// 32 bit unsigned
							else
								*dst = ullVal;					// 64 bit unsigned
							bOk = JSON_VALID;
						}
					}
				}
			} else {
				// with decimal point, check for double
				double dblVal = wcstod(strVal.c_str(), &wEnd);
				if ((NULL == wEnd) || (wEnd - strVal.c_str() == strl)) {
					// scale down to lowest data type
					if ((dblVal >= FLT_MIN) && (dblVal <= FLT_MAX))
						*dst = (float)dblVal;
					else
						*dst = dblVal;
					bOk = JSON_VALID;
				}
			}
		}
	}

	return bOk;
}

// helper
bool JSONVALUE::HasFlag(const DWORD nFlag, const DWORD nSrc) 
{
	return ((0 == nFlag) ? true : (nFlag == (nFlag & nSrc)));
}

bool JSONVALUE::IsHexChar(const wchar_t c)
{
	switch (c) {
		case wchar_t('0'):
		case wchar_t('1'):
		case wchar_t('2'):
		case wchar_t('3'):
		case wchar_t('4'):
		case wchar_t('5'):
		case wchar_t('6'):
		case wchar_t('7'):
		case wchar_t('8'):
		case wchar_t('9'):
		case wchar_t('a'):
		case wchar_t('b'):
		case wchar_t('c'):
		case wchar_t('d'):
		case wchar_t('e'):
		case wchar_t('f'):
		case wchar_t('A'):
		case wchar_t('B'):
		case wchar_t('C'):
		case wchar_t('D'):
		case wchar_t('E'):
		case wchar_t('F'):
			return true;
		default:
			return false;
	}
}

const wchar_t* JSONVALUE::FormatString(wstring &sz, const wstring &szSrc, const DWORD nFlag, const bool bName) 
{
	bool bAllowUnquoteName = bName && HasFlag(JSON_FLAG_ALLOWUNQUOTENAME, nFlag),
		bAllowSingleQuote = HasFlag(JSON_FLAG_ALLOWSINGLEQUOTE, nFlag),
		bAllowWhitespaceControlChar = HasFlag(JSON_FLAG_ALLOWWSCNTRLCHAR, nFlag),
		bHasSingleQuote = (wstring::npos != szSrc.find(wchar_t('\''))),
		bHasDoubleQuote = (wstring::npos != szSrc.find(wchar_t('"'))),
		bHasWhiteSpace = (wstring::npos != szSrc.find_first_of(L"\b\t\n\v\f\r "));	// 0x08-0x0d, 0x20
	// KHW, 2012-09-03, JSON key must be quoted is contain any one of the non-alpha values below
	if (bName)
		bHasWhiteSpace |= (wstring::npos != szSrc.find_first_of(L"~`!@#$%^&*()-_+={}[]|\\:;<>,.?\/"));

	wchar_t cQuote = wchar_t('"');			// default to strict mode output
	if (szSrc.empty()) {
		// empty string always need to be surrounded with quote
		if (bAllowSingleQuote)
			cQuote = wchar_t('\'');
	} else if (bAllowUnquoteName) {
		cQuote = 0;							// allow unquote name, default to no quote
		if (bHasSingleQuote)				// but if there is single quote in string, surround with double quote
			cQuote = wchar_t('"');
		else if (bHasDoubleQuote || bHasWhiteSpace ||
			(0 == szSrc.compare(JSON_LITERAL_TRUE)) || (0 == szSrc.compare(JSON_LITERAL_FALSE)) || 
			(0 == szSrc.compare(JSON_LITERAL_NULL)))
			// but if there is double quote or whitespace or is json literal, surround with single or double quote
			cQuote = bAllowSingleQuote ? wchar_t('\'') : wchar_t('"');
	} else if (bAllowSingleQuote) {
		cQuote = wchar_t('\'');				// allow single quote, default to single quote
		if (bHasSingleQuote)				// but if there is single quote in string, surround with double quote
			cQuote = wchar_t('"');
	}

	sz = L"";
	if (0 != cQuote) sz += cQuote;

	if (szSrc.length() > 0) {
		wstring::size_type pos = 0,
			escpos = wstring::npos;
		wstring escchar = bAllowWhitespaceControlChar ? L"\"\\" : L"\"\\\b\f\n\r\t";
		do {
			escpos = szSrc.find_first_of(escchar.c_str(), pos);
			if (wstring::npos == escpos) {
				// copy balance to destination
				sz += szSrc.substr(pos);
				break;
			} else {
				sz += szSrc.substr(pos, escpos - pos);
				sz += EscapeChar(szSrc.at(escpos), cQuote);
				pos = escpos +1;
			}
		} while (wstring::npos != escpos);
	}

	if (0 != cQuote) sz += cQuote;
	return (wchar_t*)sz.c_str();
}

const wchar_t* JSONVALUE::EscapeChar(const wchar_t c, const wchar_t cQuote)
{
	switch (c) {
		case wchar_t('"'):	return (wchar_t('\'') == cQuote) ? L"\"" : L"\\\"";
		case wchar_t('\\'):	return L"\\\\";
		case wchar_t('\b'):	return L"\\b";
		case wchar_t('\f'):	return L"\\f";
		case wchar_t('\n'):	return L"\\n";
		case wchar_t('\r'):	return L"\\r";
		case wchar_t('\t'):	return L"\\t";
		// default return the character
	//	case wchar_t('/'):	return L"/";
		default:			return (wchar_t*)&c;
	}
}

wchar_t JSONVALUE::UnescapeChar(const wstring& sz)
{
	if (0 == sz.compare(L"\\\""))		return wchar_t('"');
	else if (0 == sz.compare(L"\\\\"))	return wchar_t('\\');
	else if (0 == sz.compare(L"/"))		return wchar_t('/');
	else if (0 == sz.compare(L"\\b"))	return wchar_t('\b');
	else if (0 == sz.compare(L"\\f"))	return wchar_t('\f');
	else if (0 == sz.compare(L"\\n"))	return wchar_t('\n');
	else if (0 == sz.compare(L"\\r"))	return wchar_t('\r');
	else if (0 == sz.compare(L"\\t"))	return wchar_t('\t');
	// default return 0
	else return 0;
}

int JSONVALUE::UnescapeString(wstring& sz)
{
		// empty string
	if (sz.empty())
		return JSON_VALID;

	// find invalid escape sequence -> '\' not followed by "\/bfnrtu
	wstring temp = sz;
	wstring::size_type escpos = 0,
		escfound = wstring::npos;
	wchar_t escchar = 0;
	int bOk = JSON_VALID;
	bool bExit = false;
	do {
		escfound = temp.find(wchar_t('\\'), escpos);
		if (wstring::npos == escfound) {
			// no escape sequence, done
			bExit = true;
			sz = temp;
			bOk = JSON_VALID;
			break;
		} else if (escfound +1 < temp.size()) {
			// check next character
			switch (temp.at(escfound +1)) {
				case wchar_t('"'):
				case wchar_t('\\'):
				case wchar_t('b'):
				case wchar_t('f'):
				case wchar_t('n'):
				case wchar_t('r'):
				case wchar_t('t'):
					// valid escape sequence, continue checking
					escchar = UnescapeChar(temp.substr(escfound, 2));
					if (0 != escchar) {
						wstring temp2 = temp.substr(0, escfound);
						temp2 += escchar;
						temp2 += temp.substr(escfound +2);
						temp = temp2;
						escpos = escfound +1;
					} else {
						escpos = escfound +2;
					}
					break;
				case wchar_t('/'):
					// special handle, if \/, just store as it is
					escpos = escfound +2;
					break;
				case wchar_t('u'):
					// the following 4 characters must be hex: 0-9, a-f, A-F
					if (escfound +5 < temp.size()) {
						for (wstring::size_type i = escfound +2; i < escfound +6; i++) {
							if (false == IsHexChar(temp.at(i))) {
								// invalid string, \u not followed by 4 characters
								bExit = true;
								bOk = JSON_INVALID;
								break;
							}
						}
						if (JSON_VALID == bOk)
							escpos = escfound +6;	// continue checking
					} else {
						bExit = true;
						bOk = JSON_CHECKAGAIN;	// check again, not enough character after \u
						break;
					}
					break;
				default:
					// invalid string, \ not followed by valid character
					bExit = true;
					bOk = JSON_INVALID;
					break;
			}
		} else {
			// invalid string, \ (escape sequence) is the last character
			bExit = true;
			bOk = JSON_CHECKAGAIN;	// check again, because now maybe "\" (invalid), next maybe "\\" (valid)
			break;
		}
	} while (!bExit);

	return bOk;
}

const wchar_t* JSONVALUE::TrimString(wstring& sz, const int nLR)
{
	// trim left
	if ((-1 == nLR) || (0 == nLR)) {
		wstring::iterator it;
		for (it = sz.begin(); it != sz.end(); it++) {
			if (!iswspace(*it))
				break;
			}
		sz.erase(sz.begin(), it);
		}

	// trim right
	if ((1 == nLR) || (0 == nLR)) {
		wstring::difference_type dt;
		wstring::reverse_iterator it;

		for (it = sz.rbegin(); it != sz.rend(); it++) {
			if (!iswspace(*it))
				break;
		}
		dt = sz.rend() - it;
		sz.erase(sz.begin() + dt, sz.end());
	}

	return sz.c_str();
}

const wchar_t* JSONVALUE::ConvertAW(wstring& dest, const char* src, const UINT srccp, const bool bRemoveBom)
{
	dest = L"";
	bool bOk = false;

	if (NULL != src) {
		int nsrclen = (int)strlen(src);
		if (nsrclen > 0) {
			DWORD dwFlags = (CP_UTF8 == srccp) ? 0 : MB_PRECOMPOSED;
			LPCSTR srcNoBom = src;
			if (true == bRemoveBom) {
				// UTF-8
				if ((nsrclen > 2) && (0xef == (BYTE)src[0]) && (0xbb == (BYTE)src[1]) && (0xbf == (BYTE)src[2]))
					srcNoBom = src +3;
				nsrclen = (int)strlen(srcNoBom);
			}

			// get output buffer size
			int ndestlen = MultiByteToWideChar(srccp, dwFlags, srcNoBom, nsrclen, NULL, 0);
			if (ndestlen > 0) {
				ndestlen++;
				LPWSTR p = new wchar_t[ndestlen];
				memset(p, 0, ndestlen * sizeof(wchar_t));
				// convert
				MultiByteToWideChar(srccp, dwFlags, srcNoBom, nsrclen, p, ndestlen);
				dest = p;
				delete[] p;
				bOk = true;
			}
		}
}

	return bOk ? dest.c_str() : NULL;
}

const char* JSONVALUE::ConvertWA(std::string& dest, const wchar_t* src, const UINT destcp, const bool bRemoveBom)
{
	dest = "";
	bool bOk = false;

	if (NULL != src) {
		int nsrclen = (int)wcslen(src);
		if (nsrclen > 0) {
			DWORD dwFlags = ((destcp < 50000) && (42 != destcp)) ? (WC_COMPOSITECHECK | WC_SEPCHARS) : 0;
			LPCWSTR srcNoBom = src;
			if (true == bRemoveBom) {
				if (0xfeff == (WORD)src[0]) srcNoBom = src +1;		// UNICODE
				else if (0xfffe == (WORD)src[0]) srcNoBom = src +1;	// UNICODE big-endian
				nsrclen = (int)wcslen(srcNoBom);
			}

			// get output buffer size
			int ndestlen = WideCharToMultiByte(destcp, dwFlags, srcNoBom, nsrclen, NULL, 0, NULL, NULL);
			if (ndestlen > 0) {
				ndestlen++;
				LPSTR p = new char[ndestlen];
				memset(p, 0, ndestlen);
				// convert
				WideCharToMultiByte(destcp, dwFlags, srcNoBom, nsrclen, p, ndestlen, NULL, NULL);
				dest = p;
				delete[] p;
				bOk = true;
			}
		}
	}

	return bOk ? dest.c_str() : NULL;
}

void JSONVALUE::SetError(JSONERROR *pError, const wstring& sz, const wstring::size_type pos, LPCWSTR pszDesc)
{
	if (NULL == pError)
		return;

	pError->szDesc = pszDesc ? pszDesc : L"";
	pError->nPosition = pos;

	// calculate line and column from position
	DWORD nLine = 1,
		nColumn = 0;
	wstring sNewline = JSON_NEWLINE;
	wstring::size_type nlpos = 0,
		nllen = sNewline.length();
	do {
		nlpos = sz.find(sNewline, nlpos);
		if (wstring::npos == nlpos) {
			nColumn = pos - nColumn +1;
			break;
		} else if (nlpos < pos) {
			nLine++;			// if newline position is earlier than pos, increase line count
			nlpos += nllen;		// continue searching after newline characters
			nColumn = nlpos;	// next line start position
		} else {
			// line number confirmed, calculate column number
			nColumn = pos - nColumn +1;
			break;
		}
	} while (nlpos < pos);

	pError->nLine = nLine;
	pError->nColumn = nColumn;
}

// stage
int JSONVALUE::GetCurrentScope(JSONSCOPES *pScope)
{
	if (NULL == pScope)
		return -1;

	if (m_stage.empty()) {
		pScope->nScope = JS_NONE;
		pScope->nSubScope = JS_NONE;
		pScope->pValue = NULL;
		return 0;
	}

	JSONSTAGE::reverse_iterator rit = m_stage.rbegin();
	pScope->nScope = rit->nScope;
	pScope->nSubScope = rit->nSubScope;
	pScope->pValue = rit->pValue;
	return m_stage.size();
}

void JSONVALUE::SetNextScope(const JSONSCOPE nScope, const JSONVALUE *pValue)
{
	JSONSCOPES s;
	s.nScope = nScope;
	s.nSubScope = JS_NONE;
	s.pValue = (JSONVALUE*)pValue;
	m_stage.push_back(s);
}

bool JSONVALUE::SetCurrentSubScope(const JSONSCOPE nSubScope)
{
	if (m_stage.empty())
		return false;
	JSONSTAGE::reverse_iterator rit = m_stage.rbegin();
	rit->nSubScope = nSubScope;
	return true;
}

DWORD JSONVALUE::RevertScope(const DWORD nLevel)
{
	if (m_stage.empty())
		return (DWORD)-1;
	
	DWORD nLevelReverted = 0;
	while (!m_stage.empty()) {
		m_stage.pop_back();
		nLevelReverted++;
		if (nLevelReverted >= nLevel)
			break;
	}

	return nLevelReverted;
}

// copy from igtype
bool JSONVALUE::ToIsoTime(wstring& dest, const DATE src, const DWORD dwOption)
{
	bool bHasMs = HasFlag(JSON_DATEFORMAT_MILLISECOND, dwOption);

	dest = L"";
	SYSTEMTIME sdt = {0};
	bool bOk = bHasMs ? VariantTimeToSystemTimeWithMs(src, &sdt) :
		(TRUE == VariantTimeToSystemTime(src, &sdt));
	if (false == bOk)
		return false;

	// yyyy-mm-dd hh:mm:ss.xxx = max 23 char
	wchar_t *sVal = new wchar_t[24],
		*sTemp = new wchar_t[24];
	memset(sVal, 0, 24);
	memset(sTemp, 0, 24);

	if (HasFlag(JSON_DATEFORMAT_DATE, dwOption)) {
		#if defined(_MSC_VER) && (_MSC_VER >= 1400)
		swprintf_s(sTemp, 24, L"%04d-%02d-%02d ",
			sdt.wYear, sdt.wMonth, sdt.wDay,
			sdt.wHour, sdt.wMinute, sdt.wSecond);
		#else
		swprintf(sTemp, 24, L"%04d-%02d-%02d ",
			sdt.wYear, sdt.wMonth, sdt.wDay,
			sdt.wHour, sdt.wMinute, sdt.wSecond);
		#endif
		dest += sTemp;
	}
	if (HasFlag(JSON_DATEFORMAT_ZEROTIME, dwOption)) {
		dest += L"00:00:00";
		if (bHasMs)
			dest += L".000";
	} else if (HasFlag(JSON_DATEFORMAT_TIME, dwOption)) {
		if (bHasMs) {
			#if defined(_MSC_VER) && (_MSC_VER >= 1400)
			swprintf_s(sTemp, 24, L"%02d:%02d:%02d.%03d",
				sdt.wHour, sdt.wMinute, sdt.wSecond, sdt.wMilliseconds);
			#else
			swprintf(sTemp, 24, L"%02d:%02d:%02d.%03d",
				sdt.wHour, sdt.wMinute, sdt.wSecond, sdt.wMilliseconds);
			#endif
		} else {
			#if defined(_MSC_VER) && (_MSC_VER >= 1400)
			swprintf_s(sTemp, 24, L"%02d:%02d:%02d",
				sdt.wHour, sdt.wMinute, sdt.wSecond);
			#else
			swprintf(sTemp, 24, L"%02d:%02d:%02d",
				sdt.wHour, sdt.wMinute, sdt.wSecond);
			#endif
		}
		dest += sTemp;
	}
	delete[] sVal;
	delete[] sTemp;

	TrimString(dest, 0);
	return true;
}

bool JSONVALUE::VariantTimeToSystemTimeWithMs(const DATE vtime, LPSYSTEMTIME lpSystemTime)
{
	if (NULL == lpSystemTime)
		return false;

	// variant half second
	double v500Ms = (1.0/(24*60*60)) /2.0;//IGRC_VARIANTTIME_ONESECOND /2.0;
	if (FALSE == VariantTimeToSystemTime(vtime - v500Ms, lpSystemTime))
		return false;

	// get time or fraction portion from vtime
    double time = vtime - (int)vtime; 
    double hours = (time - (int)time) *24;
    double minutes = (hours - (int)hours) *60;
    double seconds = (minutes - (int)minutes) *60;
    double milliseconds = (seconds - (int)seconds) *1000;

	// rounding off ms to the nearest ms
    milliseconds = milliseconds + 0.5;
	if ((milliseconds < 1.0) || (milliseconds > 999.0)) {
		// calculations may yield in results like 0.00001 or 999.9999 which should  
		// actually be zero (slightly above or below limits are actually zero)
		milliseconds = 0.0;
		return (TRUE == VariantTimeToSystemTime(vtime, lpSystemTime));
	}

	lpSystemTime->wMilliseconds = (WORD)milliseconds;
    return true;
}

//bool JSONVALUE::FromIsoTime(DATE& dest, LPCTSTR src, const DWORD dwOption)
//{
//	bool bHasMs = HASFLAG(IGRC_ISOTIME_MILLISECOND, dwOption);
//
//	if (NULL == src)
//		return false;
//	int nLen = _tcsclen(src);
//	if (nLen < 19)
//		return false;
//
//	SYSTEMTIME sdt = {0};
//	int nCount = 0;
//	if ((19 == nLen) || ((23 == nLen) && !bHasMs)) {
//		nCount =
//#if defined(_MSC_VER) && (_MSC_VER >= 1400)
//			_stscanf_s
//#else
//			_stscanf
//#endif
//			(src, _T("%u-%u-%u %u:%u:%u"),
//			&sdt.wYear, &sdt.wMonth, &sdt.wDay,
//			&sdt.wHour, &sdt.wMinute, &sdt.wSecond);
//		if (6 != nCount)
//			return false;
//	} else if (23 == nLen) {
//		// NOTE: use a variable bigger than WORD to avoid runtime check failure #2
//		DWORD dwMs = 0;
//		nCount =
//#if defined(_MSC_VER) && (_MSC_VER >= 1400)
//			_stscanf_s
//#else
//			_stscanf
//#endif
//			(src, _T("%u-%u-%u %u:%u:%u.%u"),
//			&sdt.wYear, &sdt.wMonth, &sdt.wDay,
//			&sdt.wHour, &sdt.wMinute, &sdt.wSecond, &dwMs);
//		if (7 != nCount)
//			return false;
//		else
//			sdt.wMilliseconds = (WORD)dwMs;
//	} else
//		return false;
//
//	return SystemTimeToVariantTimeWithMs(&sdt, &dest);
//}

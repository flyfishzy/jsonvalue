/*
 * IntegraXor Web SCADA - JsonValue
 * http://www.integraxor.com/
 * author: KHW
 * (c)2010~2011 ecava
 * Dual licensed under the MIT or GPL Version 2 licenses.
 */
#pragma once

#include <string>
#include <map>
#include <vector>
#include <comdef.h>
using namespace std;
// forward declaration of json value
class JSONVALUE;


// json object is a collection of name-value pair
typedef std::map<wstring, JSONVALUE> JSONOBJECT, *LPJSONOBJECT;
typedef std::vector<wstring> JSONOBJECTORDER, *LPJSONOBJECTORDER;

// json array is a collection of json value
typedef std::vector<JSONVALUE> JSONARRAY, *LPJSONARRAY;

// json type can be
enum JSONTYPE {
	JT_UNDEFINED = -1,	// undefined
	JT_NULL = 0,		// null
	JT_STRING,			// string
	JT_NUMBER,			// number
	JT_BOOL,			// true/false
	JT_OBJECT,			// json object
	JT_ARRAY			// json array
};

// defaults
#define JSON_FLAG_STRICT			0x00000000	// default
#define JSON_FLAG_ISODATE			0x00000000	// default

// stringify flags
#define JSON_FLAG_ALLOWUNQUOTENAME	0x00000001
#define JSON_FLAG_LOCALEDATE		0x00000002
#define JSON_FLAG_PRETTYPRINTTAB	0x00000004
#define JSON_FLAG_PRETTYPRINTSPACE	0x00000008
#define JSON_FLAG_ORDEREDOBJECT		0x00000100	// output object follow key name sequence instead of insertion sequence	

// parse flags
#define JSON_FLAG_ALLOWHEXNUMBER	0x00000010	// allow 0x and 0X
#define JSON_FLAG_ALLOWOCTNUMBER	0x00000020	// allow 0n
#define JSON_FLAG_ALLOWWSCNTRLCHAR	0x00000040	// allow white space control character: 0x09-0x0d
#define JSON_FLAG_ALLOWSINGLEQUOTE	0x00000080	// allow single quote for string

#define JSON_FLAG_LOOSE				(JSON_FLAG_ALLOWUNQUOTENAME | JSON_FLAG_ALLOWWSCNTRLCHAR | JSON_FLAG_ALLOWSINGLEQUOTE)

// json string return
#define JSON_VALID				1
#define JSON_INVALID			(-1)
#define JSON_CHECKAGAIN			0

#define JSON_OBJECT_START		L"{"
#define JSON_OBJECT_END			L"}"
#define JSON_ARRAY_START		L"["
#define JSON_ARRAY_END			L"]"
#define JSON_ARRAY_SEPARATOR	L","
#define JSON_VALUE_SEPARATOR	L":"
#define JSON_QUOTE				L"\""
#define JSON_SINGLEQUOTE		L"'"
#define JSON_LITERAL_TRUE		L"true"
#define JSON_LITERAL_FALSE		L"false"
#define JSON_LITERAL_NULL		L"null"
#define JSON_NEWLINE			L"\r\n"
#define JSON_SPACE				L" "
#define JSON_INDENTTAB			L"\t"

// json parser error
typedef struct JSONERROR {
	DWORD nLine;
	DWORD nColumn;
	DWORD nPosition;
	wstring szDesc;

	JSONERROR() {
		nLine = 0;
		nColumn = 0;
		nPosition = 0;
	};

	JSONERROR(const JSONERROR& src) {
		nLine = src.nLine;
		nColumn = src.nColumn;
		nPosition = src.nPosition;
		szDesc = src.szDesc;
	};

	JSONERROR& operator =(const JSONERROR& src) {
		nLine = src.nLine;
		nColumn = src.nColumn;
		nPosition = src.nPosition;
		szDesc = src.szDesc;
		return *this;
	};

} JSONERROR, *LPJSONERROR;

// json stringify format
#define JSON_DATEFORMAT_DATE			0x00000001
#define JSON_DATEFORMAT_TIME			0x00000002
#define JSON_DATEFORMAT_ZEROTIME		0x00000004
#define JSON_DATEFORMAT_MILLISECOND		0x00000008

typedef struct JSONFORMAT {
	DWORD nIndentLevel;				// formatting
	DWORD nIndentSpace;				// number of space character to use as indent if JSON_FLAG_PRETTYPRINTSPACE
	int nDecimalPoint;				// number of decimal point for floating point
	wchar_t* szNumberFormat;		// floting point formatting
	DWORD nDateFormat;				// date formatting

	JSONFORMAT() {
		nIndentLevel = 0;
		nIndentSpace = 2;
		nDecimalPoint = 2;
		szNumberFormat = L"%.*f";
		nDateFormat = JSON_DATEFORMAT_DATE | JSON_DATEFORMAT_TIME;
	};

	JSONFORMAT(const JSONFORMAT& src) {
		nIndentLevel = src.nIndentLevel;
		nIndentSpace = src.nIndentSpace;
		nDecimalPoint = src.nDecimalPoint;
		lstrcpy(szNumberFormat, src.szNumberFormat);
		nDateFormat = src.nDateFormat;
	};

	JSONFORMAT& operator =(const JSONFORMAT& src) {
		nIndentLevel = src.nIndentLevel;
		nIndentSpace = src.nIndentSpace;
		nDecimalPoint = src.nDecimalPoint;
		lstrcpy(szNumberFormat, src.szNumberFormat);
		nDateFormat = src.nDateFormat;
		return *this;
	};

} JSONFORMAT, *LPJSONFORMAT;

enum JSONSCOPE {
	JS_NONE,
	JS_COMMENT,
		JS_COMMENTBLOCK,
		JS_COMMENTLINE,
	JS_OBJECT,
		JS_NAME,
		JS_NAMESTRICT,
		JS_NAMELOOSE,
		JS_NAME_END,
	JS_ARRAY,
	JS_VALUE,
		JS_VALUEOTHER
};

typedef struct JSONSCOPES {
	JSONSCOPE nScope;
	JSONSCOPE nSubScope;
	JSONVALUE* pValue;
} JSONSCOPES, *LPJSONSCOPES;

typedef std::vector<JSONSCOPES> JSONSTAGE, *LPJSONSTAGE;

// json value
class JSONVALUE
{
public:
	// member
#pragma region member
	VARTYPE vt;
	JSONTYPE jt;
	union {
		wstring*	strVal;
		union {
			__int64				llVal;	// signed whole number
			unsigned __int64	ullVal;	// unsigned whole number
			double				dblVal;	// floating point number
		} numVal;
		bool		boolVal;
		JSONOBJECT*	objVal;
		JSONARRAY*	arVal;
	} val;
	JSONOBJECTORDER order;
#pragma endregion

	// constructor
#pragma region constructor
	JSONVALUE();
	JSONVALUE(const JSONTYPE nType);
	JSONVALUE(const JSONVALUE& src);
	JSONVALUE(const wstring& src);
	JSONVALUE(const std::string& src);
	JSONVALUE(const wchar_t* src);
	JSONVALUE(const char* src);
	JSONVALUE(const DECIMAL& src);
	JSONVALUE(const double src);
	JSONVALUE(const float src);
	JSONVALUE(const __int64 src);
	JSONVALUE(const long src);
	JSONVALUE(const int src);
	JSONVALUE(const short src);
	JSONVALUE(const char src);
	JSONVALUE(const unsigned __int64 src);
	JSONVALUE(const unsigned long src);
	JSONVALUE(const unsigned short src);
	JSONVALUE(const unsigned int src);
	JSONVALUE(const unsigned char src);
	JSONVALUE(const bool src);
	JSONVALUE(const JSONOBJECT& src);
	JSONVALUE(const JSONARRAY& src);
	JSONVALUE(const _variant_t& src);
#pragma endregion

	// destructor
#pragma region destructor
	~JSONVALUE();
	void Clear();
#pragma endregion

	// assignment
#pragma region assignment
	JSONVALUE& operator =(const JSONVALUE& src);
	JSONVALUE& operator =(const wstring& src);
	JSONVALUE& operator =(const std::string& src);
	JSONVALUE& operator =(const wchar_t* src);
	JSONVALUE& operator =(const char* src);
	JSONVALUE& operator =(const DECIMAL& src);
	JSONVALUE& operator =(const double src);
	JSONVALUE& operator =(const float src);
	JSONVALUE& operator =(const __int64 src);
	JSONVALUE& operator =(const long src);
	JSONVALUE& operator =(const int src);
	JSONVALUE& operator =(const short src);
	JSONVALUE& operator =(const char src);
	JSONVALUE& operator =(const unsigned __int64 src);
	JSONVALUE& operator =(const unsigned long src);
	JSONVALUE& operator =(const unsigned int src);
	JSONVALUE& operator =(const unsigned short src);
	JSONVALUE& operator =(const unsigned char src);
	JSONVALUE& operator =(const bool src);
	JSONVALUE& operator =(const JSONOBJECT& src);
	JSONVALUE& operator =(const JSONARRAY& src);
	JSONVALUE& operator =(const _variant_t& src);

	// object assignment
	JSONVALUE& operator [](const wstring& src);
	JSONVALUE& operator [](const wchar_t* src);
	JSONVALUE& operator [](const std::string& src);
	JSONVALUE& operator [](const char* src);

	// array
	void Push(const JSONVALUE& src);
#pragma endregion

	// general
	size_t Size();
	bool isNull();
	bool isMember(const wchar_t* pos);
	LPCTSTR asCString();
	static void Unescape(const wchar_t *src,CString& dest);
	wstring asString();
	int asInt();
	double asDouble();
	bool SetAt(const size_t pos,const JSONVALUE &src);
	bool At(const size_t pos, JSONVALUE& val);
	bool At(const wchar_t* pos, JSONVALUE& val);
	bool At(const char* pos, JSONVALUE& val);
	bool At(const size_t pos, wstring& _key, JSONVALUE& _val);

	// conversion
#pragma region conversion
	// conversion, wide string version
	const wchar_t* ToString(wstring& sz, const DWORD nFlag = 0, const JSONFORMAT* pFormat = NULL);
	bool Parse(const wstring& sz, const DWORD nFlag = 0, JSONERROR *pError = NULL);

	// conversion, ansi string version
	const char* ToString(std::string& sz, const DWORD nFlag = 0, const JSONFORMAT* pFormat = NULL);
	bool Parse(const std::string& sz, const DWORD nFlag = 0, JSONERROR *pError = NULL);

#pragma endregion

protected:
	// string
	const wchar_t* FormatString(wstring &sz, const wstring &szSrc, const DWORD nFlag, const bool bName);
	const wchar_t* TrimString(wstring& sz, const int nLR);
	bool IsHexChar(const wchar_t c);
	const wchar_t* EscapeChar(const wchar_t c, const wchar_t cQuote);
	wchar_t UnescapeChar(const wstring& sz);
	int UnescapeString(wstring& sz);
	const wchar_t* ConvertAW(wstring& dest, const char* src, const UINT srccp = CP_ACP, const bool bRemoveBom = true);
	const char* ConvertWA(std::string& dest, const wchar_t* src, const UINT destcp = CP_ACP, const bool bRemoveBom = true);
	int JsonFromString(JSONVALUE* dst, const wstring& sz, const DWORD nFlag);
	
	bool HasFlag(const DWORD nFlag, const DWORD nSrc);
	void SetError(JSONERROR *pError, const wstring& sz, const wstring::size_type pos, LPCWSTR pszDesc);

	// value
	void SetString(JSONVALUE* p);
	void SetObject(JSONVALUE* p);
	void SetArray(JSONVALUE* p);
	void ObjectToString(wstring& sz, const wstring& szKey, const JSONVALUE& jVal, const DWORD nFlag, const JSONFORMAT& formatNew,
		bool& bFirstItem, const bool bPrettyPrint, const wstring& szIndent, const wstring& cIndent);
	void JsonFromIDispatch(const _variant_t& v, JSONVALUE& j);

	// date
	bool ToIsoTime(wstring& dest, const DATE src, const DWORD dwOption);
	bool VariantTimeToSystemTimeWithMs(const DATE vtime, LPSYSTEMTIME lpSystemTime);

	// stage
	JSONSTAGE m_stage;
	int GetCurrentScope(JSONSCOPES *pScope);
	void SetNextScope(const JSONSCOPE nScope, const JSONVALUE *pValue);
	bool SetCurrentSubScope(const JSONSCOPE nSubScope);
	DWORD RevertScope(const DWORD nLevel);
};
int ParseJsonFile(LPCTSTR path,JSONVALUE& jVal);

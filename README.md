# jsonvalue

This is a clone of [jsonvalue](https://code.google.com/p/jsonvalue/)  from google code，
and I made some changes.

## Introduction

C++ JSON class with support for:

*strict and loose mode
*variant data type
*simple API
*depends on STL only.
JSONVALUE is a simple C++ class for parsing JSON string to C++ object, and creating a C++ representation of JSON string.

Both ANSI and UNICODE strings are supported for parsing and stringification.

JSON C++ value, object and array can be created and added and retrieved using simple assignment operator (=) and object assignment operator ([]).

JSON string parsing support strict parsing as well as loose parsing via additional flags passed to Parse() function.

In loose parsing, user can opt to:

*allow unquote name
*allow white space control character
*allow single quote
In loose stringification, user can opt to:

*create unquote name
*create name with single quote
*pretty print output with TAB character or SPACE character
*output date as locale date
In strict mode:

*date is created in ISO date format: YYYY-MM-DD HH:MM:SS
*all names and strings are double quoted

## Useage
Sample code to parse a JSON string:

Strict mode:

  Parse(json_string);
Strict mode, with error messages: err is meaningful if Parse() returns false.

  JSONERROR err;
  Parse(json_string, 0, &err);
Loose mode: by default JSON_FLAG_LOOSE allow unquote name, white space control characters and single quote name. User can further restrict the level of looseness by passing different JSON_FLAG_xx values into Parse() function.

  Parse(json_string, JSON_FLAG_LOOSE);
Loose mode, with error messages: err is meaningful if Parse() returns false.

  JSONERROR err;
  Parse(json_string, JSON_FLAG_LOOSE, &err);
Sample code to create JSON string:

  JSONVALUE j;
  j[_T("double")].Push(0.32);
  j[_T("test")][_T("boolean")] = true;
  string a;
  j.ToString(a);
The default strict mode will generate the following JSON string:

  {"double":[0.32],"test":{"boolean":true}}
User can further customize the output string by passing JSONFORMAT structure into ToString() function, which he can control the indent space (if JSON_FLAG_PRETTYPRINTSPACE is specified), floating point format and date format.
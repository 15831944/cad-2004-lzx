#pragma once

#ifdef ZHFDLL_EXPORT
#define ZHFDLL __declspec(dllexport)
#else
#define ZHFDLL __declspec(dllimport)
#endif

#include <string>
#include "../mygreta/greta/regexpr2.h"
#include <comdef.h>
#include <afxtempl.h>
using namespace std;
using namespace regex;

typedef CArray<long, long> CLongArray;

#ifndef _tstring
#	ifndef UNICODE
typedef string _tstring;
#	else
typedef wstring _tstring;
#	endif
#endif

#ifndef _tcout
#	ifndef UNICODE
#		define _tcout cout
#	else
#		define _tcout wcout
#	endif
#endif

class ZHFDLL CBlkUtility
{
public:
	CBlkUtility(void);
	virtual ~CBlkUtility(void);

	static CString GetErrorDescription(_com_error& e) ;

	//正则匹配和替换
	static BOOL IsStrMatch(const CString strReg, const CString strToCheck) ;
	static int RegReplace(const CString strReg, const CString strSub, const CString strSrc, CString &strResult) ;

	//罗马字符转成数字，这里罗马字符只是使用普通字符拼凑的形式：I IV
	static BOOL RomanToDig(const CString strRoman, CString &strDig) ;
	//数字转成罗马字符，默认itype=0，转换成非百分号形式，itype=1，转换成%%161形式
	static BOOL DigToRoman(int iDig, CString &strRoman, int iType=0) ;

	//将类似DIK110+111形式的字符串转换为类似110.111的数值
	static BOOL ConvertStrToDigital(CString strToConv, double &dDigital);
	static bool Compute_Pre_Bh(CString strBh, CString & strPre, int & iBh);
	//将数字转换为excel的列名，比如第1列为“A”，第2列为“B”，
	static CString GetExcelFieldName( int iNum );	
	//GetExcelFieldName的逆过程
	static int GetNumFromExcelColName(TCHAR ch[]);
	//字符串替换，大小写区分/不区分
	static int ReplaceTxt(CString &strSource, CString strOld, CString strNew, BOOL bCaseSensitive=TRUE);
	//查找指定目录下的、指定后缀名的文件,iMode_SearchDepth为1时,只查找当前目录,iMode_SearchFileOrFolder为0时返回文件,为1时返回目录
	static void FindFile(CString strDirName, CString strFileExten, CStringArray &strArrayFilePath, int iMode_SearchDepth=0, int iMode_SearchFileOrFolder=0);
	// 从文件全路径获取文件名称，默认不带后缀，iOption=1则带后缀
	static BOOL GetFileName(const CString& strFilePath, CString& strFileName, int iOption=0 );
	// 从文件全路径名获取文件路径
	static BOOL GetFilePath(const CString& strFileFullPath, CString& strFilePath) ;
	//数字转换为大写（100以内）
	static CString NumToCapsString(int iNum) ;	
	//1~9转换为大写
	static CString DigitToCapsString(int iNum);
	static CString GetDateStr();
	//删除字符窜中以strStrart为头至strEnd的部分,例如;
	//CutString("111ss333ed44rrrss000ed3ed33","ss","ed", strResult);
	//strResult = "11144rrr3ed33" ;
	static void CutString(CString strSrc, CString strStart,CString strEnd, CString &strResult);
	//分解含有特定分割符号的字符串,比如"aaa\pbbb\pccc",以"\p"为分隔符,分解为"aaa" "bbb" "ccc" ;
	static void DivideString(CString strSrc, CString strMark, CStringArray & strArrayResult);
	// 带百分号的罗马字符形式，转换成字母I、V组合的形式
	static BOOL ConvertRomanToChar(const CString & strRoman, CString & strRet);
	// 将strArray数组内容用strMark链接成字符串
	static BOOL ConvertArrayToString(const CStringArray & strArray, const CString & strMark, CString & strRet,  int iAddQuota=0);
	static BOOL ConvertArrayToString(const CLongArray& nArray, const CString & strMark, CString & strRet,  int iAddQuota=0);
	// 查找指定字符串在字符串数组中的位置，索引以0开始，未找到返回-1
	static int FindStrInArray(const CStringArray & strArraySrc, LPCTSTR lpszToFind);
	//查找指定long在long数组中的位置，索引以0开始，未找到返回-1
	static int FindLongInArray(const CLongArray& nArraySrc, long nToFind);
	// 查找字符串
	static int FindString(const CString & strSrc, const CString & strPreMark, const CString & strBackMark, CStringArray & strArrayFind);

	// 从注册表中HKEY_LOCAL_MACHINE\\SOFTWARE\\ZHFCAD\\[strToOpenSub]读取strItem的值
	static BOOL QueryValueReg(const CString & strToOpenSub,const CString & strItem, CString & strResult);
	static BOOL QueryValueReg(const CString & strToOpenSub,const CString & strItem, DWORD & dwResult);
	static BOOL QueryValueReg(const CString & strToOpenSub,const CString & strItem, int & iResult);
	static BOOL QueryValueReg(const CString & strToOpenSub,const CString & strItem, double & dResult);
	// 向注册表中HKEY_LOCAL_MACHINE\\SOFTWARE\\ZHFCAD\\[strToOpenSub]的strItem写入值strValue
	static BOOL SetValueReg(const CString & strToOpenSub, const CString & strItem, const CString & strValue);
	static BOOL SetValueReg(const CString & strToOpenSub, const CString & strItem, DWORD dwValue);
	static BOOL SetValueReg(const CString & strToOpenSub,const CString & strItem, int iValue);
	static BOOL SetValueReg(const CString & strToOpenSub,const CString & strItem, double dValue);
	//取得程序安装目录
	static BOOL GetBasePath(CString &strBasePath, LPCTSTR lpstrProject=_T("ZHFCAD")) ;

	//读注册表，通用
	static BOOL ReadReg(HKEY hKeyParent, const CString& strKeyName, const CString& strItem, CString & strResult);
	static BOOL ReadReg(HKEY hKeyParent, const CString& strKeyName, const CString& strItem, DWORD & dwResult);
	static BOOL ReadReg(HKEY hKeyParent, const CString& strKeyName, const CString& strItem, int & iResult);
	static BOOL ReadReg(HKEY hKeyParent, const CString& strKeyName, const CString& strItem, double & dResult);

	static BOOL WriteReg(HKEY hKeyParent, const CString& strKeyName, const CString& strItem, const CString & strValue);
	static BOOL WriteReg(HKEY hKeyParent, const CString& strKeyName, const CString& strItem, DWORD dwValue);
	static BOOL WriteReg(HKEY hKeyParent, const CString& strKeyName, const CString& strItem, int iValue);
	static BOOL WriteReg(HKEY hKeyParent, const CString& strKeyName, const CString& strItem, double dValue);

	// 多字节转宽字节(默认是ascii转换为unicode)
	static BOOL ConvMultiToWide(const char* szMultiByte, CString& strWideChar, int nSrcCodePage=CP_ACP);
	// 宽字节转多字节(默认是unicode转换为ascii)
	static BOOL ConvWideToMulti( LPCWSTR strWideChar, CStringA& strMultiByte, int nSrcCodePage=CP_ACP) ;

	static void SortStringArray(CStringArray& strArray, int iOrder=0);
	static void SortStringArray( CStringArray &strArray,TCHAR del_char);
	static void SortandDistinguishStringArray( CStringArray &strArray,CStringArray &strArrayOdd ,CStringArray &strArrayEven,TCHAR del_char);
	// 根据vc版本适应参数的copy函数
	static void tcscpy(TCHAR * dest, SIZE_T length, const TCHAR* src);
	static int FindString(const CString& strSrc, const CString& strToFind, CUIntArray& nArrayLoc);
	static void SortStringArray_CZXHJ( CStringArray &strArray,TCHAR del_char);
	static void SortStringArray_CZXHJHandle( CStringArray &strArray,TCHAR del_char,CLongArray &nxhj_Array);
	static BOOL IsFolderExist(const CString& strFolder);
	// 存储icon为文件
	static void SaveIcon(HICON hIcon, const CString& strFilePath);
	static int FindStrInVector(const vector<CString> & strVecSrc, LPCTSTR lpszToFind);
	static BOOL CStringToDouble( CString str_a,double &d_a);
	static BOOL DoubleToCString( double d_a,CString &str_a );
	static int GetCharCountFromCString(const CString &strSrc,TCHAR cDest);
	static void SortStringArray_CZXHJHandle( CStringArray &strArray,TCHAR del_char,CStringArray &sGudaoname,CLongArray &sGudaohandle,CStringArray &xGudaoname,CLongArray &xGuidaohandle);
	static	void SortStringArray_CZXHJHandle( CStringArray &strArray,CLongArray &nxhj_Array);

	static void SortStringArray_CZXHJHandle( CStringArray &strArray,CStringArray &sGudaoname,CLongArray &sGudaohandle,CStringArray &xGudaoname,CLongArray &xGuidaohandle);
//	static int FindStrInVector(const vector<CString> & strVecSrc, LPCTSTR lpszToFind);
	static void SortStringArray_DCZhongduanxhj( CStringArray &strArray,CLongArray &nxhj_Array);
    static void CopyDataVectorFromLongArray(vector<long> &l_vec,CLongArray &nArray);
	static void CopyDataLongArrayFromVector(CLongArray &nArray,vector<long> &l_vec);
};

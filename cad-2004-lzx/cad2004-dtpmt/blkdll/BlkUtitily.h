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

	//����ƥ����滻
	static BOOL IsStrMatch(const CString strReg, const CString strToCheck) ;
	static int RegReplace(const CString strReg, const CString strSub, const CString strSrc, CString &strResult) ;

	//�����ַ�ת�����֣����������ַ�ֻ��ʹ����ͨ�ַ�ƴ�յ���ʽ��I IV
	static BOOL RomanToDig(const CString strRoman, CString &strDig) ;
	//����ת�������ַ���Ĭ��itype=0��ת���ɷǰٷֺ���ʽ��itype=1��ת����%%161��ʽ
	static BOOL DigToRoman(int iDig, CString &strRoman, int iType=0) ;

	//������DIK110+111��ʽ���ַ���ת��Ϊ����110.111����ֵ
	static BOOL ConvertStrToDigital(CString strToConv, double &dDigital);
	static bool Compute_Pre_Bh(CString strBh, CString & strPre, int & iBh);
	//������ת��Ϊexcel�������������1��Ϊ��A������2��Ϊ��B����
	static CString GetExcelFieldName( int iNum );	
	//GetExcelFieldName�������
	static int GetNumFromExcelColName(TCHAR ch[]);
	//�ַ����滻����Сд����/������
	static int ReplaceTxt(CString &strSource, CString strOld, CString strNew, BOOL bCaseSensitive=TRUE);
	//����ָ��Ŀ¼�µġ�ָ����׺�����ļ�,iMode_SearchDepthΪ1ʱ,ֻ���ҵ�ǰĿ¼,iMode_SearchFileOrFolderΪ0ʱ�����ļ�,Ϊ1ʱ����Ŀ¼
	static void FindFile(CString strDirName, CString strFileExten, CStringArray &strArrayFilePath, int iMode_SearchDepth=0, int iMode_SearchFileOrFolder=0);
	// ���ļ�ȫ·����ȡ�ļ����ƣ�Ĭ�ϲ�����׺��iOption=1�����׺
	static BOOL GetFileName(const CString& strFilePath, CString& strFileName, int iOption=0 );
	// ���ļ�ȫ·������ȡ�ļ�·��
	static BOOL GetFilePath(const CString& strFileFullPath, CString& strFilePath) ;
	//����ת��Ϊ��д��100���ڣ�
	static CString NumToCapsString(int iNum) ;	
	//1~9ת��Ϊ��д
	static CString DigitToCapsString(int iNum);
	static CString GetDateStr();
	//ɾ���ַ�������strStrartΪͷ��strEnd�Ĳ���,����;
	//CutString("111ss333ed44rrrss000ed3ed33","ss","ed", strResult);
	//strResult = "11144rrr3ed33" ;
	static void CutString(CString strSrc, CString strStart,CString strEnd, CString &strResult);
	//�ֽ⺬���ض��ָ���ŵ��ַ���,����"aaa\pbbb\pccc",��"\p"Ϊ�ָ���,�ֽ�Ϊ"aaa" "bbb" "ccc" ;
	static void DivideString(CString strSrc, CString strMark, CStringArray & strArrayResult);
	// ���ٷֺŵ������ַ���ʽ��ת������ĸI��V��ϵ���ʽ
	static BOOL ConvertRomanToChar(const CString & strRoman, CString & strRet);
	// ��strArray����������strMark���ӳ��ַ���
	static BOOL ConvertArrayToString(const CStringArray & strArray, const CString & strMark, CString & strRet,  int iAddQuota=0);
	static BOOL ConvertArrayToString(const CLongArray& nArray, const CString & strMark, CString & strRet,  int iAddQuota=0);
	// ����ָ���ַ������ַ��������е�λ�ã�������0��ʼ��δ�ҵ�����-1
	static int FindStrInArray(const CStringArray & strArraySrc, LPCTSTR lpszToFind);
	//����ָ��long��long�����е�λ�ã�������0��ʼ��δ�ҵ�����-1
	static int FindLongInArray(const CLongArray& nArraySrc, long nToFind);
	// �����ַ���
	static int FindString(const CString & strSrc, const CString & strPreMark, const CString & strBackMark, CStringArray & strArrayFind);

	// ��ע�����HKEY_LOCAL_MACHINE\\SOFTWARE\\ZHFCAD\\[strToOpenSub]��ȡstrItem��ֵ
	static BOOL QueryValueReg(const CString & strToOpenSub,const CString & strItem, CString & strResult);
	static BOOL QueryValueReg(const CString & strToOpenSub,const CString & strItem, DWORD & dwResult);
	static BOOL QueryValueReg(const CString & strToOpenSub,const CString & strItem, int & iResult);
	static BOOL QueryValueReg(const CString & strToOpenSub,const CString & strItem, double & dResult);
	// ��ע�����HKEY_LOCAL_MACHINE\\SOFTWARE\\ZHFCAD\\[strToOpenSub]��strItemд��ֵstrValue
	static BOOL SetValueReg(const CString & strToOpenSub, const CString & strItem, const CString & strValue);
	static BOOL SetValueReg(const CString & strToOpenSub, const CString & strItem, DWORD dwValue);
	static BOOL SetValueReg(const CString & strToOpenSub,const CString & strItem, int iValue);
	static BOOL SetValueReg(const CString & strToOpenSub,const CString & strItem, double dValue);
	//ȡ�ó���װĿ¼
	static BOOL GetBasePath(CString &strBasePath, LPCTSTR lpstrProject=_T("ZHFCAD")) ;

	//��ע�����ͨ��
	static BOOL ReadReg(HKEY hKeyParent, const CString& strKeyName, const CString& strItem, CString & strResult);
	static BOOL ReadReg(HKEY hKeyParent, const CString& strKeyName, const CString& strItem, DWORD & dwResult);
	static BOOL ReadReg(HKEY hKeyParent, const CString& strKeyName, const CString& strItem, int & iResult);
	static BOOL ReadReg(HKEY hKeyParent, const CString& strKeyName, const CString& strItem, double & dResult);

	static BOOL WriteReg(HKEY hKeyParent, const CString& strKeyName, const CString& strItem, const CString & strValue);
	static BOOL WriteReg(HKEY hKeyParent, const CString& strKeyName, const CString& strItem, DWORD dwValue);
	static BOOL WriteReg(HKEY hKeyParent, const CString& strKeyName, const CString& strItem, int iValue);
	static BOOL WriteReg(HKEY hKeyParent, const CString& strKeyName, const CString& strItem, double dValue);

	// ���ֽ�ת���ֽ�(Ĭ����asciiת��Ϊunicode)
	static BOOL ConvMultiToWide(const char* szMultiByte, CString& strWideChar, int nSrcCodePage=CP_ACP);
	// ���ֽ�ת���ֽ�(Ĭ����unicodeת��Ϊascii)
	static BOOL ConvWideToMulti( LPCWSTR strWideChar, CStringA& strMultiByte, int nSrcCodePage=CP_ACP) ;

	static void SortStringArray(CStringArray& strArray, int iOrder=0);
	static void SortStringArray( CStringArray &strArray,TCHAR del_char);
	static void SortandDistinguishStringArray( CStringArray &strArray,CStringArray &strArrayOdd ,CStringArray &strArrayEven,TCHAR del_char);
	// ����vc�汾��Ӧ������copy����
	static void tcscpy(TCHAR * dest, SIZE_T length, const TCHAR* src);
	static int FindString(const CString& strSrc, const CString& strToFind, CUIntArray& nArrayLoc);
	static void SortStringArray_CZXHJ( CStringArray &strArray,TCHAR del_char);
	static void SortStringArray_CZXHJHandle( CStringArray &strArray,TCHAR del_char,CLongArray &nxhj_Array);
	static BOOL IsFolderExist(const CString& strFolder);
	// �洢iconΪ�ļ�
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
#include "dtdcdata.h"
#include "StdAfx.h"
const CString CDtDcDATA::strDcItem[19] = 
{
	_T("����"),
		_T("�����"),
		_T("���ǣ������"),
		_T("���ǣ����ʽ"),
		_T("���ǣ����λ�ڵ���"),
		_T("�����Ƿ�������"),
		_T("�Ƿ������"),
		_T("�����������"),
		_T("���λ�ڵ���"),
		_T("β���ַ���")
		_T("�������"),
		_T("�Ƿ񵥶�"),
		_T("�Ƿ�ʽ����"),
		_T("�ද��������"),
		_T("�ڼ���"),
		_T("�Ƿ��е绰"),
		_T("�Ƿ��е���ȱ�ڱ���"),
		_T("����ֵ"),
		_T("��Ե��ֱ����и�")
} ;

CDtDcDATA::CDtDcDATA(void)
: m_nLoc(0)
, m_nJG_QyNum(1)
, m_strJG_QyType(_T("ZD6-D"))
, m_strJG_QyPos(_T("��"))
, m_bDC_HasBox(TRUE)
, m_bDC_ShareBox(TRUE)
, m_strDC_BoxType(_T("FXX"))
, m_strDC_BoxPos(_T("��"))
, m_strCableString(_T("��� 10-12(3)"))
, m_strBoxType(_T("HZ24"))
, m_bIsSingleAct(TRUE)
, m_bIsComplex(FALSE)
, m_strRelationDc(_T(""))
, m_nActIndex(0 )
, m_bHasPhone(TRUE )
, m_bHasDCQKBJ(TRUE)
, m_nSpeedLimit (0 )
, m_iCutJYJPos(0 )
{
	// 	DWORD iJgNum = 1, iXgNum = 0 ;
	// 	CBlkUtility::QueryValueReg(_T("ZnOption"), _T("JGNUM"), iJgNum) ;
	// 	CBlkUtility::QueryValueReg(_T("ZnOption"), _T("XGNUM"), iXgNum) ;
	// 	m_nJG_QyNum = iJgNum ;
	// 	m_nXG_QyNum = iXgNum ;
}

CDtDcDATA::~CDtDcDATA(void)
{
}

// ����ǣ����ʽ�ó�ÿһ��ǣ��
// int CDtDcDATA::GetDrag(CStringArray& strArrayDrag, int iJgOrXg)
// {
// 	int iRet = 0 ;
// 	CString& strQyType = (iJgOrXg==0?m_strJG_QyType:m_strXG_QyType) ;
// 	int& iQyNum = (iJgOrXg==0?m_nJG_QyNum:m_nXG_QyNum) ;
// 	if (strQyType==_T("��"))
// 	{
// 		iRet = 0 ;
// 	}
// 	else if (strQyType==_T("ZD6-D"))
// 	{
// 		iRet = 1 ;
// 		strArrayDrag.Add(_T("ZD6-D")) ;
// 	}
// 	else if (strQyType==_T("ZD6-EJ"))
// 	{
// 		iRet = 2 ;
// 		strArrayDrag.Add(_T("ZD6-E")) ;
// 		strArrayDrag.Add(_T("ZD6-J")) ;
// 	}
// 	else if (strQyType==_T("ZD6-EJJ"))
// 	{
// 		iRet = 3 ;
// 		strArrayDrag.Add(_T("ZD6-E")) ;
// 		strArrayDrag.Add(_T("ZD6-J")) ;
// 		strArrayDrag.Add(_T("ZD6-J")) ;
// 	}
// 	else if (strQyType==_T("ZYJ7+SH6"))
// 	{
// 		iRet = 2 ;
// 		strArrayDrag.Add(_T("ZYJ7")) ;
// 		strArrayDrag.Add(_T("SH6")) ;
// 	}
// 	else if (strQyType==_T("ZYJ7+SH6+SH6"))
// 	{
// 		iRet = 3 ;
// 		strArrayDrag.Add(_T("ZYJ7")) ;
// 		strArrayDrag.Add(_T("SH6")) ;
// 		strArrayDrag.Add(_T("SH6")) ;
// 	}
// 	else	if (strQyType==_T("ZYJ7")||strQyType==_T("S700K")||strQyType==_T("ZDJ9"))
// 	{
// 		iRet = iQyNum ;
// 		for (int i=0; i<iRet; i++)
// 		{
// 			strArrayDrag.Add(strQyType) ;
// 		}
// 	}
// 
// 	return iRet;
// }

int CDtDcDATA::GetDrag(const CString& strQyType)
{
	int iRet = 0 ;

	if (strQyType==_T("��"))
	{
		iRet = 0 ;
	}
	else if (strQyType==_T("ZD6-D"))
	{
		iRet = 1 ;
	}
	else if (strQyType==_T("ZD6-EJ"))
	{
		iRet = 2 ;
	}
	else if (strQyType==_T("ZD6-EJJ"))
	{
		iRet = 3 ;
	}
	else if (strQyType==_T("ZYJ7+SH6"))
	{
		iRet = 2 ;
	}
	else if (strQyType==_T("ZYJ7+SH6+SH6"))
	{
		iRet = 3 ;
	}
	else 	if (strQyType==_T("ZYJ7")||strQyType==_T("S700K")||strQyType==_T("ZDJ9"))
	{
		iRet = -1 ;
	}

	return iRet;
}

// ������
int CDtDcDATA::GetActionNum(void)
{
	CStringArray strArrayConDc ;
	CBlkUtility::DivideString(m_strRelationDc, _T(","), strArrayConDc) ;	
	return strArrayConDc.GetCount()+1 ;
}

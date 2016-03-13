#include "StdAfx.h"
#include "cableimportdata.h"
#include "ZnSxt.h"

CCableImportDATA::~CCableImportDATA(void)
{
}
const CString CCableImportDATA::strCableImportItem[5] = {_T("站名"), _T("是否集中站"),_T("公里标"), _T("上下行"), _T("距信号设备室距离")} ;

CCableImportDATA::CCableImportDATA(void)
: m_strBlkName(_T(""))
,m_strStation(_T(""))
, d_nLoc(0) 
, m_bIsJZ(FALSE)
, m_IsUP(0) 
, m_dDistToDeviceRoom(0) 
{
}
CCableImportDATA::CCableImportDATA( const CString& strBlkName )
: m_strBlkName(strBlkName)
,m_strStation(_T(""))
, d_nLoc(0) 
, m_bIsJZ(FALSE)
, m_IsUP(FALSE) 
, m_dDistToDeviceRoom(0) 
{
	Init() ;

}

BOOL CCableImportDATA::Init(void)
{
// 	CZnSxt DTsxt;
//     DTsxt.GetBoxLocation()
// 	if (!m_strBlkName.IsEmpty())
// 	{
// 		CADODatabase adoDb;
// 		CADORecordset rs;
// 		adoDb.Open(g_strAdoCon) ;
// 		rs.SetConnection(&adoDb) ;
// 
// 		CString strSql ;
// 		strSql.Format(_T("select * from define_default_xhj where block_name='%s'"), m_strBlkName) ;
// 		if (rs.Open(strSql))
// 		{
// 			while(!rs.IsEOF())
// 			{
// 				long nTmp = 0 ;
// 				CString strTmp ;
// 				rs.GetFieldValue(_T("is_train"), nTmp) ;
// 				m_bIsTrain = (nTmp==0?FALSE:TRUE) ;
// 				rs.GetFieldValue(_T("box_type"), m_strBoxTypeOption) ;
// 				int iLoc = m_strBoxTypeOption.Find(_T(";")) ;
// 				m_strBoxType = (iLoc==-1?m_strBoxTypeOption:m_strBoxTypeOption.Left(iLoc)) ;
// 				rs.GetFieldValue(_T("core_num1"), m_nCoreNumI) ;
// 				rs.GetFieldValue(_T("core_num2"), m_nCoreNumII) ;
// 				rs.GetFieldValue(_T("has_ddunit"), nTmp) ;
// 				m_bHasDDUnit = (nTmp==0?FALSE:TRUE) ;
// 				rs.GetFieldValue(_T("is_alu"), nTmp) ;
// 				m_bIsAlu = (nTmp==0?FALSE:TRUE) ;
// 				rs.GetFieldValue(_T("has_phone"), nTmp) ;
// 				m_bHasPhone = (nTmp==0?FALSE:TRUE) ;
// 				rs.GetFieldValue(_T("ds_num"), m_nDSNum) ;
// 				rs.GetFieldValue(_T("speed_limit"), m_nSpeedLimit) ;
// 
// 				rs.MoveNext() ;
// 			}
// 			rs.Close() ;
// 		}
// 
// 
// 		rs.Close() ;
// 		adoDb.Close() ;
//	}

	return 0;
}

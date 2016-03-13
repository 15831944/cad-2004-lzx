#include "StdAfx.h"
#include "XhDevice.h"
const CString CXhDevice::strXhDeviceItem[14]={_T("设备名"),_T("块名"),_T("公里标"),_T("是否采用箱盒"),_T("箱盒类型"),_T("箱盒位于设备"),_T("尾缆根数"),_T("进箱盒芯数1"),_T("进箱盒芯数2"),_T("尾缆字符串1"),_T("尾缆字符串2"),_T("是否有电话线"),_T("是否有贯通线"),_T("是否接地")};

CXhDevice::CXhDevice(void)
: m_strBlkName(_T(""))
, m_strName(_T(""))
,m_dLoc(0)
, m_bHasBox(FALSE)
, m_strBoxType(_T("上") ) 
, m_strBoxPos(_T("")) 
, m_nLenCable(0)  
, m_nCoreNum1(0)
,m_nCoreNum2(0)
, m_nBackNum1(0)
,m_nBackNum2(0)
,m_nCable(0)
,m_strCable1(_T(""))
,m_strCable2(_T(""))
, m_bHasPhone(FALSE)
, m_bHasGT(FALSE) 
, m_bHasGround(FALSE)
{
}

CXhDevice::CXhDevice( const CString& strBlkName )
: m_strBlkName(strBlkName)
, m_strName(_T(""))
,m_dLoc(0)
, m_bHasBox(FALSE)
, m_strBoxType(_T("上") ) 
, m_strBoxPos(_T("")) 
, m_nLenCable(0)  
, m_nCoreNum1(0)
,m_nCoreNum2(0)
, m_nBackNum1(0)
,m_nBackNum2(0)
,m_nCable(0)
,m_strCable1(_T(""))
,m_strCable2(_T(""))
, m_bHasPhone(FALSE)
, m_bHasGT(FALSE) 
, m_bHasGround(FALSE)
{
	Init();
}

CXhDevice::~CXhDevice(void)
{
}

BOOL CXhDevice::Init( void )
{
	if (!m_strBlkName.IsEmpty())
	{
		CADODatabase adoDb;
		CADORecordset rs;
		adoDb.Open(g_strAdoCon) ;
		rs.SetConnection(&adoDb) ;

		CString strSql ;
		strSql.Format(_T("select * from define_default_device where block_name='%s'"), m_strBlkName) ;
		if (rs.Open(strSql))
		{
			while(!rs.IsEOF())
			{
				long nTmp = 0 ;
				CString strTmp ;
				rs.GetFieldValue(_T("has_box"), nTmp) ;
				m_bHasBox = (nTmp==0?FALSE:TRUE) ;
				rs.GetFieldValue(_T("box_type"), m_strBoxType) ;
				rs.GetFieldValue(_T("cable_num"),m_nCable);
				rs.GetFieldValue(_T("core_num1"), m_nCoreNum1) ;
				rs.GetFieldValue(_T("core_num2"), m_nCoreNum2) ;
				rs.GetFieldValue(_T("cable_string1"), m_strCable1) ;
				rs.GetFieldValue(_T("cable_string2"), m_strCable2) ;
				rs.GetFieldValue(_T("has_phone"), nTmp) ;
				m_bHasPhone = (nTmp==0?FALSE:TRUE) ;
				rs.GetFieldValue(_T("has_gt"), nTmp) ;
				m_bHasGT = (nTmp==0?FALSE:TRUE) ;
				rs.GetFieldValue(_T("has_ground"), nTmp) ;
				m_bHasGround = (nTmp==0?FALSE:TRUE) ;
				rs.MoveNext() ;
			}
			rs.Close() ;
		}


		rs.Close() ;
		adoDb.Close() ;
	}

	return 0;

}

#include "guidaonode.h"
#include "../blkdll/BlkUtitily.h"
// CGuiDaoNode::CGuiDaoNode(void)
// : m_nHandleLine(0)
// , m_nLineType(0)
// , m_strGuiDaoName(_T(""))
// , m_nZhixhjLeft(0)
// , m_nZhixhjRight(0)
// , m_nXiangxhjLeft(0)
// , m_nXiangxhjRight(0)
// , m_nHandleNextLineLeft(0)
// , m_nHandleNextLineRight(0)
// {
// 	//m_nArrayHandleDC_Asc
// }
// CGuiDaoNode::CGuiDaoNode( long nHandleLine )
// : m_nHandleLine(nHandleLine)
// , m_nLineType(0)
// , m_strGuiDaoName(_T(""))
// , m_nZhixhjLeft(0)
// , m_nZhixhjRight(0)
// , m_nXiangxhjLeft(0)
// , m_nXiangxhjRight(0)
// , m_nHandleNextLineLeft(0)
// , m_nHandleNextLineRight(0)
// {
// 	
// }

CGuiDaoNode::CGuiDaoNode( long nHandleLine )
: m_nHandleLine(nHandleLine)
, m_iLineType(0)
, m_strGuiDaoName(_T(""))
, m_nZhixhjLeft(0)
, m_nZhixhjRight(0)
, m_nXiangxhjLeft(0)
, m_nXiangxhjRight(0)
, m_nHandleNextLineLeft(0)
, m_nHandleNextLineRight(0)
{
	m_AdoDb.Open(g_strAdoCon) ;
	m_RecordSet.SetConnection(&m_AdoDb) ;
// 	m_nHandleLine=0;
// 	m_nLineType=0;
// 	m_strGuiDaoName=_T("");
// 	m_nZhixhjLeft=0;
// 	m_nZhixhjRight=0;
// 	m_nXiangxhjLeft=0;
// 	m_nXiangxhjRight=0;
// 	m_nHandleNextLineLeft=0;
// 	m_nHandleNextLineRight=0;
// 	m_nHandleLine=nHandleLine;
	//m_nArrayHandleDC_Asc.SetSize(10,10);
	//m_nArrayHandleDC_Desc.SetSize(10,10);
	//m_nArrayHandleDC_Desc =CLongArray a;
	CLongArray m_nArrayHandleDC_Asc;
	//往右走相应道岔
	CLongArray m_nArrayHandleDC_Desc ;
	//轨道左侧信号机
	CLongArray m_nArrayHandleXHJLeft;
	//轨道右侧信号机
	CLongArray m_nArrayHandleXHJRight;
	m_sys.GetXdata(m_nHandleLine,_T("GUIDAONAME"),m_strGuiDaoName);
	//这些是否可以通过一次数据库查询获得结果，从而更加优化
	this->GetGuiDaoDuanXHJ(m_nHandleLine,true,m_nArrayHandleXHJRight);
	this->GetGuiDaoDuanXHJ(m_nHandleLine,false,m_nArrayHandleXHJLeft);
	this->GetGuiDaoDuanXHJ(true,m_nHandleLine,m_nZhixhjRight,m_nXiangxhjRight);
	this->GetGuiDaoDuanXHJ(false,m_nHandleLine,m_nZhixhjLeft,m_nXiangxhjLeft);
	this->GetOnLineDaoChaOrderByXAsc(m_nHandleLine,m_nArrayHandleDC_Asc);
	this->GetOnLineChaOrderByXDesc(m_nHandleLine,m_nArrayHandleDC_Desc);
	this->FindNextLineByJYJ(m_nHandleLine,m_nHandleNextLineLeft,false);
	this->FindNextLineByJYJ(m_nHandleLine,m_nHandleNextLineRight,true);
	this->FindLineType(m_nHandleLine,m_iLineType);
	CBlkUtility::CopyDataVectorFromLongArray(m_vecHandleXHJLeft,m_nArrayHandleXHJLeft);
	CBlkUtility::CopyDataVectorFromLongArray(m_vecHandleXHJRight,m_nArrayHandleXHJRight);
	CBlkUtility::CopyDataVectorFromLongArray(m_vecThisDaocha_Asc,m_nArrayHandleDC_Asc);
	CBlkUtility::CopyDataVectorFromLongArray(m_vecThisDaocha_Desc,m_nArrayHandleDC_Desc);
}

CGuiDaoNode::CGuiDaoNode( void )
{
	m_nHandleLine=0;
	m_iLineType=0;
	m_strGuiDaoName=_T("");
	m_nZhixhjLeft=0;
	m_nZhixhjRight=0;
	m_nXiangxhjLeft=0;
	m_nXiangxhjRight=0;
	m_nHandleNextLineLeft=0;
	m_nHandleNextLineRight=0;
}

// 查询轨道区段右侧的信号机，并按照块的基点x坐标升序排列
//conline_left=true时，轨道区段的右侧的信号机
void CGuiDaoNode::GetGuiDaoDuanXHJ(bool bforward,UINT nLineHandle, long& ZhiXHJ,long& XiangXHJ)
{
	//nArrayLineDoneCur.Add(nLineHandle) ;
	try
	{
		CString strSql ;
		CLongArray nArrayHandleXHJTmp;
		//注意：nLineHandle一定不能取0，否则可能查到错误的结果
		if(nLineHandle==0)
		{
			ZhiXHJ=0;
			XiangXHJ=0;
			return;
		}
		
		if(bforward==false)
		{
			//轨道右端信号机
			strSql.Format(_T("select * from sxt_ent where sxt_ent_conline_left=%d and sxt_ent_type>99 and sxt_ent_type<200 order by sxt_ent_x asc"), nLineHandle,nLineHandle ) ;

		}
		else
		{
			strSql.Format(_T("select * from sxt_ent where sxt_ent_conline_right=%d and sxt_ent_type>99 and sxt_ent_type<200 order by sxt_ent_x desc"),nLineHandle,nLineHandle ) ;
		}
		/*		strSql.Format(_T("select * from sxt_ent where (sxt_ent_conline_left=%d or sxt_ent_online_handle=%d) and sxt_ent_type>99 and sxt_ent_type<200 order by sxt_ent_x asc"), nLineHandle, nLineHandle ) ;*/
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nArrayHandleXHJTmp) ;
		for(int i=0;i<nArrayHandleXHJTmp.GetCount();i++)
		{
			if(bforward==true)
			{
				int toward=-1;
				if(GetBlockToward(nArrayHandleXHJTmp[i],toward)==TRUE)
				{
					if(toward==1)
					{
						ZhiXHJ=nArrayHandleXHJTmp[i];
						//找到同向信号机即可结束
						//break;
					}
					else if(toward==2||toward==0||toward==3)
					{
						XiangXHJ=nArrayHandleXHJTmp[i];
					}
				}

			}
			else
			{
				//轨道左端信号机
				int toward=-1;
				if(GetBlockToward(nArrayHandleXHJTmp[i],toward)==TRUE)
				{
					if(toward==2)
					{
						ZhiXHJ=nArrayHandleXHJTmp[i]; 
						//break;
					}
					else if(toward==1||toward==0||toward==3)
					{
						XiangXHJ=nArrayHandleXHJTmp[i];
					}
				}
			}
		}
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误!in GetGuiDaoInfoOrderByX"));
	}
}
//向右推进时，获取轨道左侧信号机，向左推进时，获取轨道右侧信号机
void CGuiDaoNode::GetGuiDaoDuanXHJ(UINT nLineHandle, bool bToward,CLongArray& nArrayXHJ)
{
	//nArrayLineDoneCur.Add(nLineHandle) ;
	try
	{
		CString strSql ;
		CLongArray nArrayHandleXHJTmp;
		if(nLineHandle==0)
		{
			return;
		}
		if(bToward==false)
		{
			//获取轨道右端信号机
			strSql.Format(_T("select * from sxt_ent where (sxt_ent_conline_left=%d ) and sxt_ent_type>99 and sxt_ent_type<200 order by sxt_ent_x asc"), nLineHandle) ;
		}
		else
		{
			//获取轨道左端信号机
			strSql.Format(_T("select * from sxt_ent where (sxt_ent_conline_right=%d ) and sxt_ent_type>99 and sxt_ent_type<200 order by sxt_ent_x desc"), nLineHandle) ;
		}
		//strSql.Format(_T("select * from sxt_ent where (sxt_ent_conline_left=%d or sxt_ent_conline_right=%d or sxt_ent_online_handle=%d) and sxt_ent_type>99 and sxt_ent_type<200 order by sxt_ent_x asc"), nLineHandle, nLineHandle,nLineHandle ) ;
		/*		strSql.Format(_T("select * from sxt_ent where (sxt_ent_conline_left=%d or sxt_ent_online_handle=%d) and sxt_ent_type>99 and sxt_ent_type<200 order by sxt_ent_x asc"), nLineHandle, nLineHandle ) ;*/
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nArrayHandleXHJTmp) ;
		nArrayXHJ.Append(nArrayHandleXHJTmp);
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误!in GetGuiDaoInfoOrderByX"));
	}
}
BOOL CGuiDaoNode::GetBlockToward( long nHandleXHJ ,int &iToward)
{
	if(nHandleXHJ==0)
	{
		return FALSE;
	}
	long t1=GetTickCount();
	BOOL bRet = FALSE ;
	AcDbEntity* pEnt = NULL ;
	iToward =-1;
	if (m_sys.OpenAcDbEntity(nHandleXHJ, pEnt, AcDb::kForRead))
	{
		if (pEnt->isKindOf(AcDbBlockReference::desc()))
		{
			AcDbBlockReference* pRef = AcDbBlockReference::cast(pEnt) ;
			//bRet= GetBlockName(pRef, strBlockName) ;
			double rota=pRef->rotation();
			//acutPrintf(_T("\n %6f"),rota);
			int cnt=0;
			while((rota-PI)>1.0E-6)
			{
				rota-=PI;
				cnt++;
			}
			while((rota+PI)<1.0E-6)
			{
				rota+=PI;
				cnt++;
			}
			float res=rota-PI/2;
			//acutPrintf("rota=%6f",rota);
			if(fabs(res)<=1.0E-6&&cnt%2==0)
			{
				//向上
				iToward=0;
			}
			else if(fabs(res)<=1.0E-6&&cnt%2==1)
			{
				//向下；
				iToward=3;
			}
			else if(res<1.0E-6&&cnt%2==0)
			{
				//朝右
				iToward=1;
			}
			else if(res<1.0E-6&&cnt%2==1)
			{
				iToward=2;
			}
			else if(res>1.0E-6&&cnt%2==0)
			{
				//朝左
				iToward=2;
			}
			else if(res>1.0E-6&&cnt%2==1)
			{
				iToward=1;
			}
			else
			{
				iToward=-1;

			}

		}
		pEnt->close() ;
		bRet =TRUE;
	}
	long t2=GetTickCount();
	CString str;
	str.Format(_T("GetBlockToward:%d ms"),t2-t1);
	//acutPrintf(_T("\n%s"),str);
	return bRet ;


}
// 循环查询轨道线连接的信号机及道岔，并按照块的基点x坐标升序排列
void CGuiDaoNode::GetOnLineDaoChaOrderByXAsc(UINT nLineHandle, CLongArray& nArrayHandleDC)
{
	try
	{
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		CLongArray nArrayHandleDCTmp ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_online_handle=%d and sxt_ent_type>-1 and sxt_ent_type<100 order by sxt_ent_x asc"), nLineHandle ) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nArrayHandleDCTmp) ;
		nArrayHandleDC.Append(nArrayHandleDCTmp) ;
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误!in GetGuiDaoInfoOrderByX"));
	}
}
// 循环查询轨道线连接的信号机及道岔，并按照块的基点x坐标升序排列
void CGuiDaoNode::GetOnLineChaOrderByXDesc(UINT nLineHandle, CLongArray& nArrayHandleDC)
{
	//nArrayLineDoneCur.Add(nLineHandle) ;
	try
	{
		CString strSql ;
		CLongArray nArrayHandleDCTmp ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_online_handle=%d and sxt_ent_type>-1 and sxt_ent_type<100 order by sxt_ent_x desc"), nLineHandle ) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nArrayHandleDCTmp) ;
		nArrayHandleDC.Append(nArrayHandleDCTmp) ;
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误!inGetGuiDaoChaOrderByXDesc"));
	}
}
BOOL CGuiDaoNode::FindNextLineByJYJ( long nHandleLineStart,long &nHandleLineNext,bool bForward )
{
	try
	{
		if(nHandleLineStart==0)
		{
			return FALSE;
		}
		CString strSql ;
		long nHandleLeftJyj = 0, nHandleRightJyj = 0 ;
		if(bForward==false)
		{
			strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandleLineStart) ;
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj) ;
		}
		else
		{
			strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandleLineStart) ;
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj) ;     
		}
		//如果是向右寻找，则找右端绝缘节。
		int nHandleEndConJyj = (bForward?nHandleRightJyj:nHandleLeftJyj) ; //线末端连接的绝缘节handle
		//long nHandleNextLine = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
		CString strFieldName = (bForward?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
		//读取结果集
		m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleLineNext);

		return TRUE;

	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);   
		return FALSE;
		//return -2;
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误!in FindNextLineByJYJ"));
		//return -2;
		return FALSE;
	}		
	return FALSE;
}
BOOL CGuiDaoNode::FindLineType(long nHandleLine,int &iLineType)
{
	try
	{
		CString strSql ;
		strSql.Format(_T("select  *  from sxt_line where sxt_line_handle=%d and sxt_line_vertex<>-1"),nHandleLine) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), iLineType);
		return TRUE;

	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);   
		return FALSE;
		//return -2;
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误!in FindNextLineByJYJ"));
		//return -2;
		return FALSE;
	}		
	return FALSE;

}

CGuiDaoNode::~CGuiDaoNode(void)
{
}

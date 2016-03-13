#include "xhjdatanode.h"

CXhjDataNode::CXhjDataNode(void)
{
}

CXhjDataNode::CXhjDataNode( long nHandleXhj )
:m_nHandleXhj(nHandleXhj)
,m_nHandleXhjLeftLine(0)
,m_nHandleXhjRightLine(0)
,m_strXhjName(_T(""))
,m_strBlockName(_T(""))
,m_dXhj_x(0)
,m_iToward(-1)
{
	m_AdoDb.Open(g_strAdoCon) ;
	m_RecordSet.SetConnection(&m_AdoDb) ;
	this->GetXhjConLineLeft(nHandleXhj,m_nHandleXhjLeftLine);
	this->GetXhjConLineRight(nHandleXhj,m_nHandleXhjRightLine);
	m_sys.GetAttValue(nHandleXhj,_T("XHJNAME"),m_strXhjName);
	this->GetEntBlkNameByHandle(nHandleXhj,m_strBlockName);
	if(this->GetBlockPosition_X(nHandleXhj,m_dXhj_x)==FALSE)
	{
		m_dXhj_x=0;
	}
	if(this->GetBlockToward(nHandleXhj,m_iToward)==FALSE)
	{
		m_iToward=-1;
	}


}
int CXhjDataNode::GetXhjConLineLeft(  long nHandleXhj,long &nHandleLine)
{
	try
	{
		CString strSql ;
		if(nHandleXhj==0)
		{
			nHandleLine=0;
			return -1;
		}
		strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleXhj) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_conline_left"),nHandleLine) ;
		return 0;
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);   
		return -2;
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误!in GetXhjConLineLeft"));
		return -2;
	}		
}
int CXhjDataNode::GetXhjConLineRight(  long nHandleXhj,long &nHandleLine)
{
	try
	{
		CString strSql ;
		if(nHandleXhj==0)
		{
			nHandleLine=0;
			return -1;
		}
		strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleXhj) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_conline_right"),nHandleLine) ;
		return 0;
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);   
		return -2;
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误!in GetXhjConLineRight"));
		return -2;
	}		
}
BOOL CXhjDataNode::GetBlockPosition_X( long nHandle,double &point_x )
{
	try
	{
		CString strSql ;
		point_x=0;
		if(nHandle!=0)
		{
			strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle) ;
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_x"),point_x) ;
			return TRUE;
		}
		return FALSE;
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);       
		return FALSE;
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误!in CXhjDataNode::GetBlockPosition_X"));
		return FALSE;
	}

}
void CXhjDataNode::GetEntBlkNameByHandle( const long nHandle,CString &str_ent_name)
{
	try
	{
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		str_ent_name=_T("");
		if(nHandle!=0)
		{
			strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle) ;
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_block"),str_ent_name);
		}

	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);   
		//return -2;
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误!in GetEntBlkNameByHandle"));
		//return -2;
	}		

}
BOOL CXhjDataNode::GetBlockToward( long nHandleXHJ ,int &iToward)
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
CXhjDataNode::~CXhjDataNode(void)
{
}

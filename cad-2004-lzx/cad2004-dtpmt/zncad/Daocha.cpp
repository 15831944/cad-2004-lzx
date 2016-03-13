#include "daocha.h"

CDaocha::CDaocha(void)
: m_nHandleDC(0)
, m_strDcName(_T(""))
, m_nHandleLine_fw(0)
, m_nHandleLine_dw(0)
, m_dDcEnt_x(0)
, m_nHandleLine_fw_vertex(-1)
,m_nHandle_Another_DC(0)
,m_nHandle_Jiaocha_DCMin(0)
,m_nHandle_Jiaocha_DCMax(0)
,m_nHandleDC_Min(0)
,m_nHandleDC_Max(0)
{

}
CDaocha::CDaocha( long nHandleDC )
: m_nHandleDC(nHandleDC)
, m_strDcName(_T(""))
, m_nHandleLine_fw(0)
, m_nHandleLine_dw(0)
, m_dDcEnt_x(0)
, m_nHandleLine_fw_vertex(-1)
,m_nHandle_Another_DC(0)
,m_nHandle_Jiaocha_DCMin(0)
,m_nHandle_Jiaocha_DCMax(0)
,m_nHandleDC_Min(0)
,m_nHandleDC_Max(0)
{
	m_AdoDb.Open(g_strAdoCon) ;
	m_RecordSet.SetConnection(&m_AdoDb) ;
	//查询道岔所在的线，该线为后继
	CString strSql;
	if(nHandleDC==0)
	{
		return;
	}
	strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), m_nHandleDC) ;
	//这里可以优化
	CADORecordset cSet (&m_AdoDb) ;
	if(cSet.Open(strSql))
	{
		if (cSet.GetRecordCount()>0)
		{
			cSet.GetFieldValue(_T("sxt_ent_fwline_handle"),m_nHandleLine_fw) ;
			cSet.GetFieldValue(_T("sxt_ent_online_handle"), m_nHandleLine_dw) ;
			cSet.GetFieldValue(_T("sxt_ent_fwline_vertex"),m_nHandleLine_fw_vertex) ;
			cSet.GetFieldValue(_T("sxt_ent_name"),m_strDcName) ;
			cSet.GetFieldValue(_T("sxt_ent_x"),m_dDcEnt_x);
		}
		cSet.Close() ;
	}
	this->GetAnotherDuXianDaoCha(m_nHandleDC,m_nHandle_Another_DC);
	m_nHandleDC_Min=this->GetDuXianMinDaoCha(nHandleDC,m_nHandle_Another_DC);
	m_nHandleDC_Max=this->GetDuXianMaxDaoCha(nHandleDC,m_nHandle_Another_DC);
	if(m_nHandle_Another_DC!=0)
		this->GetJiaoChaDuXianAnotherDaoChas(m_nHandleDC,m_nHandle_Another_DC,m_nHandle_Jiaocha_DCMin,m_nHandle_Jiaocha_DCMax);
	GetDaidongDaochas1(nHandleDC,m_strDaidong);
	GetEntName(m_nHandleDC_Min,m_strDC_Min);
	GetEntName(m_nHandleDC_Max,m_strDC_Max);
	GetEntName(m_nHandle_Jiaocha_DCMin,m_str_Jiaocha_DCMin);
	GetEntName(m_nHandle_Jiaocha_DCMax,m_str_Jiaocha_DCMax);
	GetEntName(m_nHandle_Another_DC,m_str_Another_DC);
}
long CDaocha::GetDuXianMinDaoCha( long nHandleDc_One,long  nHandleDc_Another )
{
	try
	{
		//if(nHandleDc_Another!=GetAnotherDuXianDaoCha())
		if(nHandleDc_One==0||nHandleDc_Another==0)
		{
			return 0;
		}
		long nHandle=0;
		if(GetAnotherDuXianDaoCha(nHandleDc_One,nHandle)==TRUE)
		{
			//是渡线道岔
			//CADORecordset cSet(&m_AdoDb) ;
			CString strSql ;
			strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDc_One) ;
			//long nHandle_fw=0;
			CString name_one;
			//取道岔反位线
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"),name_one);
			strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDc_Another) ;
			//long nHandle_fw=0;
			CString name_another;
			//取道岔反位线
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"),name_another);
			int iDCNum1 = _ttoi(name_one.GetString()) ;
			int iDcNum2=_ttoi(name_another.GetString());
			return iDCNum1<iDcNum2?nHandleDc_One:nHandleDc_Another;
		}
		else
		{
			return 0;
		}
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误!in FindNextLine"));
	}
	return 0;

}
long CDaocha::GetDuXianMaxDaoCha( long nHandleDc_One,long  nHandleDc_Another )
{
	try
	{
		//if(nHandleDc_Another!=GetAnotherDuXianDaoCha())
		if(nHandleDc_One==0||nHandleDc_Another==0)
		{
			return 0;
		}
		long nHandle=0;
		if(GetAnotherDuXianDaoCha(nHandleDc_One,nHandle)==TRUE)
		{
			//是渡线道岔
			//CADORecordset cSet(&m_AdoDb) ;
			CString strSql ;
			strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDc_One) ;
			//long nHandle_fw=0;
			CString name_one;
			//取道岔反位线
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"),name_one);
			strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDc_Another) ;
			//long nHandle_fw=0;
			CString name_another;
			//取道岔反位线
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"),name_another);
			int iDCNum1 = _ttoi(name_one.GetString()) ;
			int iDcNum2=_ttoi(name_another.GetString());
			return iDCNum1>iDcNum2?nHandleDc_One:nHandleDc_Another;
		}
		else
		{
			return 0;
		}
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误!in FindNextLine"));
	}
	return 0;

}
BOOL CDaocha::GetAnotherDuXianDaoCha( long nHandleDc_One,long & nHandleDc_Another )
{
	try
	{

		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDc_One) ;
		long nHandle_fw=0;
		//取道岔反位线
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw);
		if(nHandle_fw==0)
		{
			nHandleDc_Another =0;
			return FALSE;
		}
		//向左或者向右找一后继
		long nHandleLeftJyj = 0, nHandleRightJyj = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj) ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj) ;      
		//如果是向右寻找，则找右端绝缘节。
		//int nHandleEndConJyj = (bForword?nHandleRightJyj:nHandleLeftJyj) ; //线末端连接的绝缘节handle
		long nHandleNextLine_left =0;
		long nHandleNextLine_right=0;
		long linetype1=-1;
		long linetype2=-1;
		if(nHandleLeftJyj!=0)
		{
			strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleLeftJyj) ;
			//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_conline_left"), nHandleNextLine_left);
			strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandleNextLine_left) ;
			m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), linetype1) ;
		}
		if(nHandleRightJyj!=0)
		{
			strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleRightJyj) ;
			//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_conline_right"), nHandleNextLine_right);
			strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandleNextLine_right) ;
			m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), linetype2) ;
		}
		//判断后继是否有一条为渡线
		if(nHandleNextLine_left==0&&nHandleNextLine_right==0)
		{
			nHandleDc_Another =0;
			return FALSE;
		}
		if(linetype1==1)
		{
			strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine_left) ;
			//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
			long nHandle_DC1=0;
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandle_DC1);
			if(nHandle_DC1!=0)
			{
				nHandleDc_Another =nHandle_DC1;
				return TRUE;	
			}
		}
		if(linetype2==1)
		{
			strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine_right) ;
			//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
			long nHandle_DC2=0;
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandle_DC2);	
			if(nHandle_DC2!=0)
			{
				nHandleDc_Another =nHandle_DC2;
				return TRUE;	
			}
		}
		nHandleDc_Another=0;
		return FALSE;
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误!in FindNextLine"));
	}
	return FALSE;
}

// 循环查询轨道线连接的信号机及道岔，并按照块的基点x坐标升序排列
void CDaocha::GetGuiDaoDaoChaOrderByXAsc(UINT nLineHandle, CLongArray& nArrayHandleDC, CLongArray& nArrayLineDoneCur, CLongArray& nArrayLineCurType)
{
	//long t1=GetTickCount();
	nArrayLineDoneCur.Add(nLineHandle) ;
	try
	{
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nLineHandle) ;
		long nLineType = 0 ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
		nArrayLineCurType.Add(nLineType) ;
		CLongArray nArrayHandleDCTmp ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_online_handle=%d and sxt_ent_type>-1 and sxt_ent_type<100 order by sxt_ent_x asc"), nLineHandle ) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nArrayHandleDCTmp) ;
		nArrayHandleDC.Append(nArrayHandleDCTmp) ;
		for (int i=0; i<nArrayHandleDCTmp.GetCount(); i++)
		{
			UINT nHandleDc = nArrayHandleDCTmp.GetAt(i) ;
			strSql.Format(_T("select sxt_ent_fwline_handle from sxt_ent where sxt_ent_handle=%d"), nHandleDc) ;
			long nHandleFwLine = 0 ;
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"), nHandleFwLine) ;
			if (nHandleFwLine>0)
			{
				//this->GetGuiDaoInfo(nHandleFwLine, nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType) ;
				this->GetGuiDaoDaoChaOrderByXAsc(nHandleFwLine,nArrayHandleDC,nArrayLineDoneCur,nArrayLineCurType);
			}
		}
		// 		long t2=GetTickCount();
		// 		CString str;
		// 		str.Format("GetGuiDaoDaoChaOrderByXAsc time:%d ms",t2-t1);
		//acutPrintf(_T("\n %s"),str);
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
void CDaocha::GetOnLineDaoChaOrderByXAsc(UINT nLineHandle, CLongArray& nArrayHandleDC)
{
	long t1=GetTickCount();
	//nArrayLineDoneCur.Add(nLineHandle) ;
	try
	{
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		// 		strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nLineHandle) ;
		// 		long nLineType = 0 ;
		//m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
		//nArrayLineCurType.Add(nLineType) ;
		CLongArray nArrayHandleDCTmp ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_online_handle=%d and sxt_ent_type>-1 and sxt_ent_type<100 order by sxt_ent_x asc"), nLineHandle ) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nArrayHandleDCTmp) ;
		nArrayHandleDC.Append(nArrayHandleDCTmp) ;
		long t2=GetTickCount();
		CString str;
		str.Format(_T("GetOnLineDaoChaOrderByXAsc time:%d ms"),t2-t1);
		//acutPrintf(_T("\n %s"),str);
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
//由一条渡线的两个道岔，如果交叉渡线，则返回另一条渡线的另外两个道岔
BOOL CDaocha::GetJiaoChaDuXianAnotherDaoChas( long nHandleDc_One,long nHandleDc_Another,long &nHandleDc_Min,long &nHandleDc_Max )
{
	try
	{
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		//输入的两个道岔必须是渡线
		long nHandleDc_One2=0;
		if(GetAnotherDuXianDaoCha(nHandleDc_One,nHandleDc_One2)==TRUE)
		{
			if(nHandleDc_Another==nHandleDc_One2)
			{//输入为渡线
				long nHandleline1=0;
				long nHandleline2=0;
				strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDc_One) ;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"),nHandleline1) ;
				strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDc_Another) ;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"),nHandleline2) ;
				//找online1上的其他道岔是否为渡线道岔
				CLongArray nArrayHandleDC;
				CLongArray nArrayHandleXHJ;
				CLongArray nArrayHandleLineCur;
				CLongArray nArrayHandleLineType;
				int  iNumDC=0;
				this->GetGuiDaoDaoChaOrderByXAsc(nHandleline1,nArrayHandleDC,nArrayHandleLineCur,nArrayHandleLineType);
				//this->GetGuiDaoDaoChaOrderByXAsc()
				iNumDC =nArrayHandleDC.GetCount();
				CLongArray nArrayHandleDC2;
				CLongArray nArrayHandleXHJ2;
				CLongArray nArrayHandleLineCur2;
				CLongArray nArrayHandleLineType2;
				int iNumDC2=0;
				this->GetGuiDaoDaoChaOrderByXAsc(nHandleline2, nArrayHandleDC2,nArrayHandleLineCur2,nArrayHandleLineType2);
				iNumDC2 =nArrayHandleDC2.GetCount();
				if(iNumDC2<2||iNumDC<2)
				{
					return FALSE;
				}
				for(int i=0;i<iNumDC;i++)
				{
					long nHandleDC =nArrayHandleDC[i];
					if(nHandleDC!=nHandleDc_One)
					{
						//是否是渡线
						long nHandleDC_A=0;
						if(this->GetAnotherDuXianDaoCha(nHandleDC,nHandleDC_A)==TRUE)
						{
							//是渡线道岔，并且另一道岔位于line2。此时的两个轨道区段共同组成两条渡线，则很可能为交叉渡线
							if(CBlkUtility::FindLongInArray(nArrayHandleDC2,nHandleDC_A)!=-1)
							{
								//是否还需要加额外条件来区分交叉渡线
								nHandleDc_Min =this->GetDuXianMinDaoCha(nHandleDC,nHandleDC_A);
								nHandleDc_Max =this->GetDuXianMaxDaoCha(nHandleDC,nHandleDC_A);
								return TRUE;
							}
						}
					}
				}

			}
			else
			{
				return FALSE;
			}

		}
		else
		{
			return FALSE;
		}
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误!in FindNextLine"));
	}
	return FALSE;
}
BOOL CDaocha::GetDaidongDaochas1( long nHandleDC, CString &str_daidongdaochas)
{

	CString str_daidong_temp;
	if(GetDaidongDaochas(nHandleDC,str_daidong_temp)==TRUE)
	{
		str_daidongdaochas=str_daidong_temp;
		return TRUE;
	}
	long nHandleDC_Ano=0;
	if(GetAnotherDuXianDaoCha(nHandleDC,nHandleDC_Ano)==TRUE)
	{
		if(GetDaidongDaochas(nHandleDC_Ano,str_daidong_temp)==TRUE)
		{
			str_daidongdaochas=str_daidong_temp;
			return TRUE;
		}
	}
	return FALSE;
}
BOOL CDaocha::FindNextLineByJYJ( long nHandleLineStart,long &nHandleLineNext,bool bForward )
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
BOOL CDaocha::IsOntheHorizonline( long nHandleLineStart,long nHandleLineEnd,bool bForward )
{
	long nHandLineTemp=nHandleLineStart;
	// 	if(nHandleLineStart==273234||nHandleLineEnd==273230)
	// 	{
	// 		nHandleLineEnd=nHandleLineEnd;
	// 	}
	if(nHandleLineEnd==0)
		return FALSE;
	while(nHandLineTemp!=0)
	{
		long nHandleNextLine=0;
		FindNextLineByJYJ(nHandLineTemp,nHandleNextLine,bForward);
		if (nHandleNextLine==0)
		{
			return FALSE;
		}
		else if(nHandleNextLine==nHandleLineEnd)
		{
			return TRUE;
		}
		else
		{
			nHandLineTemp=nHandleNextLine;
		}
	}
	return FALSE;
}
// 循环查询轨道线连接的信号机及道岔，并按照块的基点x坐标升序排列
void CDaocha::GetOnLineChaOrderByXDesc(UINT nLineHandle, CLongArray& nArrayHandleDC)
{
	//nArrayLineDoneCur.Add(nLineHandle) ;
	try
	{
		long t1=GetTickCount();
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		CLongArray nArrayHandleDCTmp ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_online_handle=%d and sxt_ent_type>-1 and sxt_ent_type<100 order by sxt_ent_x desc"), nLineHandle ) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nArrayHandleDCTmp) ;
		nArrayHandleDC.Append(nArrayHandleDCTmp) ;
		long t2=GetTickCount();
		CString str;
		str.Format(_T("GetGuiDaoChaOrderByXDesc time:%d ms"),t2-t1);
		//acutPrintf(_T("\n %s"),str);
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
//记录访问过的道岔，避免走回路，顺便记录走过道岔的定反位，用于联锁表,只用于寻找后继，无其他功能，开销较小
void CDaocha::FindNextLine( long nHandleLineStart,long nHandleLineEnd,CLongArray&nArrayHandleLine,bool bForword/*=true*/ )
{
	try
	{
		// static int stop =0;
		if(nHandleLineStart==nHandleLineEnd||nHandleLineStart==0)
		{//找到，结束
			return;
		}
		// 		CString GuidaoName;
		// 		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),GuidaoName);
		CADORecordset cSet(&m_AdoDb);
		CString strSql ;
		strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandleLineStart) ;
		long nLineType = 0 ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
		//表示为尽头线
		if (nLineType==2) 
		{
			CString name;
			m_sys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),name);
			//acutPrintf(_T("\n 尽头线：%s"),name.GetString());
			return ;
		}
		//判断轨道上DC个数
		CLongArray nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType ;
		//获取轨道信息，这个函数需要仔细看看
		//this->GetGuiDaoInfo(nHandleLineStart, nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType) ;	
		if(bForword==true)
		{
			//this->GetGuiDaoDaoChaOrderByXAsc(nHandleLineStart, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType) ;
			this->GetOnLineDaoChaOrderByXAsc(nHandleLineStart,nArrayHandleDC);
		}
		else
		{
			//this->GetGuiDaoChaOrderByXDesc(nHandleLineStart, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType) ;
			this->GetOnLineChaOrderByXDesc(nHandleLineStart,nArrayHandleDC);
		}
		//for test print DC
		int iNumDC = nArrayHandleDC.GetCount() ;//当前轨道电路包含几个道岔
		//int iNumXHJ = nArrayHandleXHJ.GetCount() ; //当前轨道电路连接有几架信号机
		//int iNumLine = nArrayLineDoneCur.GetCount() ; //当前轨道电路由几段组成
		// 		CString guiname;
		// 		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),guiname);
		//acutPrintf(_T("\n 道岔个数=%d,信号机个数=%d,轨道名称=%d"),iNumDC,iNumXHJ,guiname.GetString());
		long nHandleLeftJyj = 0, nHandleRightJyj = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj) ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj) ;      
		//如果是向右寻找，则找右端绝缘节。
		int nHandleEndConJyj = (bForword?nHandleRightJyj:nHandleLeftJyj) ; //线末端连接的绝缘节handle
		long nHandleNextLine = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
		CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
		//读取结果集
		m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine);
		if(iNumDC<1)
		{
			//轨道不包含道岔时，后继只有一个
			if(nLineType==0)
			{//不为渡线时
				if(nHandleNextLine!=0)
				{
					nArrayHandleLine.Add(nHandleNextLine);
					// 					CString Daocha=_T("");
					// 					GuoDaoCha.Add(Daocha);
				}
				else
				{
					//还有可能当前线为某一个道岔的反位线
					strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
					//反位线对应的道岔
					long nHandleDC=0;
					m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
					if(nHandleDC!=0)
					{
						//查询道岔所在的线，该线为后继
						strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
						//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
						//反位线对应的道岔
						long nHandle_dw=0;
						// long nHandle_fw=0;						
						int IsStart=-1;
						CString DCname;
						//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
						//这里可以优化
						// 						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw) ;
						// 						m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
						// 						m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname) ;
						CADORecordset cSet (&m_AdoDb) ;
						if(cSet.Open(strSql))
						{
							if (cSet.GetRecordCount()>0)
							{
								//cSet.GetFieldValue(_T("sxt_ent_fwline_handle"),nHandle_fw) ;
								cSet.GetFieldValue(_T("sxt_ent_online_handle"), nHandle_dw) ;
								cSet.GetFieldValue(_T("sxt_ent_fwline_vertex"),IsStart) ;
								cSet.GetFieldValue(_T("sxt_ent_name"),DCname) ;
							}
							cSet.Close() ;
						}
						if(bForword==true&&IsStart>0)
						{
							//定位所在线为后继
							nArrayHandleLine.Add(nHandle_dw);
						}
						else if(bForword==false&&IsStart==0)
						{
							nArrayHandleLine.Add(nHandle_dw);
						}
					}
					else
					{
						//acutPrintf(_T("\n nHandNextLine为0"));
					}
				}
			}
			else
			{
			}
			//return;
		}//endif NumDC
		else
		{
			//含有道岔时，可能存在多条后继
			//道岔为定位时后继
			//只要有一个道岔为反位，就是一条新的后继，这个结论通常应该是没问题的
			CString DaochaD;
			for(int k=0;k<iNumDC;k++)
			{//道岔反位时后继,需要前面访问的道岔
				//取道岔handle
				long nHandleDC=nArrayHandleDC[k];
				//查询反位所在线
				strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
				long nHandle_fw=0;
				long nHandle_dw=0;
				int IsStart=-1;
				CString DCname1;
				//这里可以优化
				// 				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
				// 				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw) ;
				// 				m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
				// 				m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname1) ;
				CADORecordset cSet (&m_AdoDb) ;
				if(cSet.Open(strSql))
				{
					if (cSet.GetRecordCount()>0)
					{
						cSet.GetFieldValue(_T("sxt_ent_fwline_handle"),nHandle_fw) ;
						cSet.GetFieldValue(_T("sxt_ent_online_handle"), nHandle_dw) ;
						cSet.GetFieldValue(_T("sxt_ent_fwline_vertex"),IsStart) ;
						cSet.GetFieldValue(_T("sxt_ent_name"),DCname1) ;
					}
					cSet.Close() ;
				}
				long nHanderDC_another=0;
				//向右推进有后继时
				if(IsStart==0&&bForword==true)
				{
					//向右推进时起点
					if(nHandle_fw!=0)
					{
						//继续往下求一次后继，判断是否为渡线
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//为渡线时，求反位线的后继
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//如果是向右寻找，则找右端绝缘节。
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //线末端连接的绝缘节handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//读取结果集
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//再找该反位线所在道岔的定位线
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//反位线对应的道岔
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//查询道岔所在的线，该线为后继
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//反位线对应的道岔
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2即为渡线的后继
								if(nHandle_dw2!=0)
									nArrayHandleLine.Add(nHandle_dw2);
							}
						}
						else
						{//反位线不为渡线时
							nArrayHandleLine.Add(nHandle_fw);
						}
					}
				}
				else if(IsStart>0&&bForword==false)
				{
					//向左推进时起点
					if(nHandle_fw!=0)
					{
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//为渡线时，求反位线的后继
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//如果是向右寻找，则找右端绝缘节。
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //线末端连接的绝缘节handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//读取结果集
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//再找该反位线所在道岔的定位线
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//反位线对应的道岔
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//查询道岔所在的线，该线为后继
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//反位线对应的道岔
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2即为渡线的后继
								if(nHandle_dw2!=0)
									nArrayHandleLine.Add(nHandle_dw2);
							}
						}
						//	acutPrintf(_T("Isstart=%d"),IsStart);
						else
						{//反位线不为渡线时
							nArrayHandleLine.Add(nHandle_fw);
						}
					}
				}
				else
				{

				}
				/*GuoDaoCha.Add(DaochaD);*/
			}
			if(nHandleNextLine!=0)
			{
				nArrayHandleLine.Add(nHandleNextLine);
			}
			else
			{

				//可能为某一个道岔的反位线
				//还有可能当前线为某一个道岔的反位线
				strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
				//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
				//反位线对应的道岔
				long nHandleDC=0;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
				if(nHandleDC!=0)
				{
					//查询道岔所在的线，该线为后继
					strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
					//反位线对应的道岔
					long nHandle_dw=0;
					// long nHandle_fw=0;						
					int IsStart=-1;
					CString DCname;
					//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
					//这里可以优化
					// 					m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw) ;
					// 					m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;	
					// 					m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_name"),DCname) ;
					CADORecordset cSet (&m_AdoDb) ;
					if(cSet.Open(strSql))
					{
						if (cSet.GetRecordCount()>0)
						{
							//cSet.GetFieldValue(_T("sxt_ent_fwline_handle"),nHandle_fw) ;
							cSet.GetFieldValue(_T("sxt_ent_online_handle"), nHandle_dw) ;
							cSet.GetFieldValue(_T("sxt_ent_fwline_vertex"),IsStart) ;
							cSet.GetFieldValue(_T("sxt_ent_name"),DCname) ;
						}
						cSet.Close() ;
					}
					if(bForword==true&&IsStart>0)
					{
						//定位所在线为后继
						nArrayHandleLine.Add(nHandle_dw);
					}
					else if(bForword==false&&IsStart==0)
					{
						nArrayHandleLine.Add(nHandle_dw);
					}
					else
					{
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
		AfxMessageBox(_T("未知的错误!in FindNextLine"));
	}	
}
BOOL CDaocha::GetDaidongDaochas( long nHandleDC,CString &str_daidongdaochas)
{
	try
	{
		//获取道岔所在定位线
		//CADORecordset cSet(&m_AdoDb) ;
		if(nHandleDC==271611)
		{
			nHandleDC=nHandleDC;
		}
		if(nHandleDC==271605||nHandleDC==271599)
		{
			nHandleDC=nHandleDC;
		}
		CString strSql ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
		//long nHandle_fw=0;
		long nHandle_dw_0=0;
		long nHandle_fw_0=0;
		long iStart1=-1;
		CString str_daochas_temp;
		long nHandleDc_DdMin=0;
		long nHandleDc_DdMax=0;
		//取道岔定位线
		//这里可以优化
		// 		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"),nHandle_dw_0);
		// 		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw_0);
		// 		m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),iStart1) ;
		CADORecordset cSet (&m_AdoDb) ;
		if(cSet.Open(strSql))
		{
			if (cSet.GetRecordCount()>0)
			{
				cSet.GetFieldValue(_T("sxt_ent_fwline_handle"),nHandle_fw_0) ;
				cSet.GetFieldValue(_T("sxt_ent_online_handle"), nHandle_dw_0) ;
				cSet.GetFieldValue(_T("sxt_ent_fwline_vertex"),iStart1) ;
				//cSet.GetFieldValue(_T("sxt_ent_name"),DCname1) ;
			}
			cSet.Close() ;
		}
		strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw_0) ;
		long nLineType = 0 ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
		long nHandleLineTfw=0;
		long nHandleLineTdw=0;
		if(nLineType==1)
		{
			//为渡线，则往后求
			if(iStart1==0)
			{
				FindNextLineByJYJ(nHandle_fw_0,nHandleLineTfw,true);
				if(nHandleLineTfw!=0)
				{
					//求道岔定位所在线
					strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineTfw) ;
					m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandleLineTdw) ;
				}
				else
					return FALSE;
			}
			else
			{
				FindNextLineByJYJ(nHandle_fw_0,nHandleLineTfw,false);
				if(nHandleLineTfw!=0)
				{
					//求道岔定位所在线
					strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineTfw) ;
					m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandleLineTdw) ;
				}
				else
				{
					return FALSE;
				}

			}

		}
		else
		{
			nHandleLineTdw=nHandle_fw_0;
		}

		if(nHandle_dw_0==0||nHandle_fw_0==0)
		{
			return FALSE;
		}
		//获取定位线道岔个数，并以X坐标升序排列
		//CLongArray nArrayHandleXHJ;
		CLongArray nArrayHandleDC;
		CLongArray nArrayHandleLineType;
		CLongArray nArrayHandleLineCur;
		//this->GetGuiDaoDaoChaOrderByXAsc(nHandle_dw,nArrayHandleDC,nArrayHandleLineCur,nArrayHandleLineType);
		this->GetOnLineDaoChaOrderByXAsc(nHandle_dw_0,nArrayHandleDC);
		int iNum_DC=0;
		iNum_DC =nArrayHandleDC.GetCount();
		//acutPrintf(_T("\n 轨道上包含 %d个道岔"),iNum_DC);
		if(iNum_DC<2)
		{
			return FALSE;
		}
		//判断道岔是否位于轨道最顶端
		int index=0;
		index=CBlkUtility::FindLongInArray(nArrayHandleDC,nHandleDC);
		//acutPrintf(_T("\n 道岔位置；%d"),index);
		if(index==-1)
		{
			return FALSE;
		}
		//向右推进时,有多条后继
		CLongArray nArrayHandleNext;
		long nHandleLineEnd=0;
		CStringArray sGuodaocha;
		CStringArray sGudao;
		//bool foword=true;
		if(iStart1==0)
		{
			//向右推进
			this->FindNextLine(nHandle_dw_0,nHandleLineEnd,nArrayHandleNext,true);
			int iNum_Next=nArrayHandleNext.GetCount();
			//acutPrintf(_T("\n 往右轨道上包含 %d个后继"),iNum_Next);
			if(iNum_Next>=3)
			{
				for(int i=index+1;i<iNum_DC;i++)
				{
					long nHandle_DXDC=nArrayHandleDC[i];
					//带动道岔一定是渡线道岔
					long nHandle_DXAno=0;
					if(this->GetAnotherDuXianDaoCha(nHandle_DXDC,nHandle_DXAno)==TRUE)
					{
						strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_DXDC) ;
						long nHandle_fw=0;
						int IsStart=-1;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
						m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
						//求另一个道岔所在定位线；
						long nHandle_dwAno;
						strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_DXAno) ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"),nHandle_dwAno);
						if(IsStart==0&&nHandle_fw!=0&&(IsOntheHorizonline(nHandleLineTdw,nHandle_dwAno,true)==FALSE))
						{
							CString str_dc1;
							CString str_dc2;
							nHandleDc_DdMin=this->GetDuXianMinDaoCha(nHandle_DXDC,nHandle_DXAno);
							nHandleDc_DdMax=this->GetDuXianMaxDaoCha(nHandle_DXDC,nHandle_DXAno);
							GetEntName(nHandleDc_DdMin,str_dc1);
							GetEntName(nHandleDc_DdMax,str_dc2);
							str_daochas_temp+=__T("{")+str_dc1+_T("/")+str_dc2+_T("}")+_T(",");
							//return TRUE;
						}	
					}
				}
				str_daidongdaochas=str_daochas_temp;
				return TRUE;
			}	
		}
		else
		{
			//向左推进
			CLongArray nArrayHandleNext2;
			long nHandleLineEnd2=0;
			//bool foword2=false;
			this->FindNextLine(nHandle_dw_0,nHandleLineEnd2,nArrayHandleNext2,false);
			int iNum_Next2=nArrayHandleNext2.GetCount();
			//acutPrintf(_T("\n 往左轨道上包含 %d个后继"),iNum_Next2);
			if(iNum_Next2>=3)
			{
				for(int i=index-1;i>=0;i--)
				{
					long nHandle_DXDC=nArrayHandleDC[i];
					//long nHandle_DXMin=0;
					long nHandle_DXAno=0;
					//未考虑交叉渡线存在的情况
					if(this->GetAnotherDuXianDaoCha(nHandle_DXDC,nHandle_DXAno)==TRUE)
					{

						strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_DXDC) ;
						long nHandle_fw=0;
						int IsStart=-1;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
						m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
						//求另一个道岔所在定位线；
						long nHandle_dwAno;
						strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_DXAno) ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"),nHandle_dwAno);
						if(IsStart>0&&nHandle_fw!=0&&(IsOntheHorizonline(nHandleLineTdw,nHandle_dwAno,false)==FALSE))
						{
							CString str_dc1;
							CString str_dc2;
							nHandleDc_DdMin=this->GetDuXianMinDaoCha(nHandle_DXDC,nHandle_DXAno);
							nHandleDc_DdMax=this->GetDuXianMaxDaoCha(nHandle_DXDC,nHandle_DXAno);
							GetEntName(nHandleDc_DdMin,str_dc1);
							GetEntName(nHandleDc_DdMax,str_dc2);
							str_daochas_temp+=__T("{")+str_dc1+_T("/")+str_dc2+_T("}")+_T(",");
						}		
					}
				}
				str_daidongdaochas =str_daochas_temp;
				return TRUE;
			}

		}	
		return FALSE;
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误!in GetdaidongDaoChas"));
	}
	return FALSE;
}
void CDaocha::GetEntName( long nHandle,CString &Entname )
{
	try
	{
		if(nHandle==0)
		{
			return;
		}
		CString name;
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		if(nHandle!=0)
		{
			strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle) ;
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"),name) ;
		}
		Entname =name;
		return ;
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误!in GetEntName"));
	}	
}

CDaocha::~CDaocha(void)
{
}

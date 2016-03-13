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
	//��ѯ�������ڵ��ߣ�����Ϊ���
	CString strSql;
	if(nHandleDC==0)
	{
		return;
	}
	strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), m_nHandleDC) ;
	//��������Ż�
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
			//�Ƕ��ߵ���
			//CADORecordset cSet(&m_AdoDb) ;
			CString strSql ;
			strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDc_One) ;
			//long nHandle_fw=0;
			CString name_one;
			//ȡ����λ��
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"),name_one);
			strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDc_Another) ;
			//long nHandle_fw=0;
			CString name_another;
			//ȡ����λ��
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
		AfxMessageBox(_T("δ֪�Ĵ���!in FindNextLine"));
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
			//�Ƕ��ߵ���
			//CADORecordset cSet(&m_AdoDb) ;
			CString strSql ;
			strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDc_One) ;
			//long nHandle_fw=0;
			CString name_one;
			//ȡ����λ��
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"),name_one);
			strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDc_Another) ;
			//long nHandle_fw=0;
			CString name_another;
			//ȡ����λ��
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
		AfxMessageBox(_T("δ֪�Ĵ���!in FindNextLine"));
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
		//ȡ����λ��
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw);
		if(nHandle_fw==0)
		{
			nHandleDc_Another =0;
			return FALSE;
		}
		//�������������һ���
		long nHandleLeftJyj = 0, nHandleRightJyj = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj) ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj) ;      
		//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
		//int nHandleEndConJyj = (bForword?nHandleRightJyj:nHandleLeftJyj) ; //��ĩ�����ӵľ�Ե��handle
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
		//�жϺ���Ƿ���һ��Ϊ����
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
		AfxMessageBox(_T("δ֪�Ĵ���!in FindNextLine"));
	}
	return FALSE;
}

// ѭ����ѯ��������ӵ��źŻ������������տ�Ļ���x������������
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
		AfxMessageBox(_T("δ֪�Ĵ���!in GetGuiDaoInfoOrderByX"));
	}
}
// ѭ����ѯ��������ӵ��źŻ������������տ�Ļ���x������������
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
		AfxMessageBox(_T("δ֪�Ĵ���!in GetGuiDaoInfoOrderByX"));
	}
}
//��һ�����ߵ������������������ߣ��򷵻���һ�����ߵ�������������
BOOL CDaocha::GetJiaoChaDuXianAnotherDaoChas( long nHandleDc_One,long nHandleDc_Another,long &nHandleDc_Min,long &nHandleDc_Max )
{
	try
	{
		//CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		//�����������������Ƕ���
		long nHandleDc_One2=0;
		if(GetAnotherDuXianDaoCha(nHandleDc_One,nHandleDc_One2)==TRUE)
		{
			if(nHandleDc_Another==nHandleDc_One2)
			{//����Ϊ����
				long nHandleline1=0;
				long nHandleline2=0;
				strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDc_One) ;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"),nHandleline1) ;
				strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDc_Another) ;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"),nHandleline2) ;
				//��online1�ϵ����������Ƿ�Ϊ���ߵ���
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
						//�Ƿ��Ƕ���
						long nHandleDC_A=0;
						if(this->GetAnotherDuXianDaoCha(nHandleDC,nHandleDC_A)==TRUE)
						{
							//�Ƕ��ߵ���������һ����λ��line2����ʱ������������ι�ͬ����������ߣ���ܿ���Ϊ�������
							if(CBlkUtility::FindLongInArray(nArrayHandleDC2,nHandleDC_A)!=-1)
							{
								//�Ƿ���Ҫ�Ӷ������������ֽ������
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
		AfxMessageBox(_T("δ֪�Ĵ���!in FindNextLine"));
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
		//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
		int nHandleEndConJyj = (bForward?nHandleRightJyj:nHandleLeftJyj) ; //��ĩ�����ӵľ�Ե��handle
		//long nHandleNextLine = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
		CString strFieldName = (bForward?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
		//��ȡ�����
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
		AfxMessageBox(_T("δ֪�Ĵ���!in FindNextLineByJYJ"));
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
// ѭ����ѯ��������ӵ��źŻ������������տ�Ļ���x������������
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
		AfxMessageBox(_T("δ֪�Ĵ���!inGetGuiDaoChaOrderByXDesc"));
	}
}
//��¼���ʹ��ĵ��������߻�·��˳���¼�߹�����Ķ���λ������������,ֻ����Ѱ�Һ�̣����������ܣ�������С
void CDaocha::FindNextLine( long nHandleLineStart,long nHandleLineEnd,CLongArray&nArrayHandleLine,bool bForword/*=true*/ )
{
	try
	{
		// static int stop =0;
		if(nHandleLineStart==nHandleLineEnd||nHandleLineStart==0)
		{//�ҵ�������
			return;
		}
		// 		CString GuidaoName;
		// 		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),GuidaoName);
		CADORecordset cSet(&m_AdoDb);
		CString strSql ;
		strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandleLineStart) ;
		long nLineType = 0 ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
		//��ʾΪ��ͷ��
		if (nLineType==2) 
		{
			CString name;
			m_sys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),name);
			//acutPrintf(_T("\n ��ͷ�ߣ�%s"),name.GetString());
			return ;
		}
		//�жϹ����DC����
		CLongArray nArrayHandleXHJ, nArrayHandleDC, nArrayLineDoneCur, nArrayLineCurType ;
		//��ȡ�����Ϣ�����������Ҫ��ϸ����
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
		int iNumDC = nArrayHandleDC.GetCount() ;//��ǰ�����·������������
		//int iNumXHJ = nArrayHandleXHJ.GetCount() ; //��ǰ�����·�����м����źŻ�
		//int iNumLine = nArrayLineDoneCur.GetCount() ; //��ǰ�����·�ɼ������
		// 		CString guiname;
		// 		m_mysys.GetXdata(nHandleLineStart,_T("GUIDAONAME"),guiname);
		//acutPrintf(_T("\n �������=%d,�źŻ�����=%d,�������=%d"),iNumDC,iNumXHJ,guiname.GetString());
		long nHandleLeftJyj = 0, nHandleRightJyj = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj) ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandleLineStart) ;
		m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj) ;      
		//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
		int nHandleEndConJyj = (bForword?nHandleRightJyj:nHandleLeftJyj) ; //��ĩ�����ӵľ�Ե��handle
		long nHandleNextLine = 0 ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
		CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
		//��ȡ�����
		m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine);
		if(iNumDC<1)
		{
			//�������������ʱ�����ֻ��һ��
			if(nLineType==0)
			{//��Ϊ����ʱ
				if(nHandleNextLine!=0)
				{
					nArrayHandleLine.Add(nHandleNextLine);
					// 					CString Daocha=_T("");
					// 					GuoDaoCha.Add(Daocha);
				}
				else
				{
					//���п��ܵ�ǰ��Ϊĳһ������ķ�λ��
					strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
					//��λ�߶�Ӧ�ĵ���
					long nHandleDC=0;
					m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
					if(nHandleDC!=0)
					{
						//��ѯ�������ڵ��ߣ�����Ϊ���
						strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
						//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
						//��λ�߶�Ӧ�ĵ���
						long nHandle_dw=0;
						// long nHandle_fw=0;						
						int IsStart=-1;
						CString DCname;
						//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
						//��������Ż�
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
							//��λ������Ϊ���
							nArrayHandleLine.Add(nHandle_dw);
						}
						else if(bForword==false&&IsStart==0)
						{
							nArrayHandleLine.Add(nHandle_dw);
						}
					}
					else
					{
						//acutPrintf(_T("\n nHandNextLineΪ0"));
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
			//���е���ʱ�����ܴ��ڶ������
			//����Ϊ��λʱ���
			//ֻҪ��һ������Ϊ��λ������һ���µĺ�̣��������ͨ��Ӧ����û�����
			CString DaochaD;
			for(int k=0;k<iNumDC;k++)
			{//����λʱ���,��Ҫǰ����ʵĵ���
				//ȡ����handle
				long nHandleDC=nArrayHandleDC[k];
				//��ѯ��λ������
				strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
				long nHandle_fw=0;
				long nHandle_dw=0;
				int IsStart=-1;
				CString DCname1;
				//��������Ż�
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
				//�����ƽ��к��ʱ
				if(IsStart==0&&bForword==true)
				{
					//�����ƽ�ʱ���
					if(nHandle_fw!=0)
					{
						//����������һ�κ�̣��ж��Ƿ�Ϊ����
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//Ϊ����ʱ����λ�ߵĺ��
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //��ĩ�����ӵľ�Ե��handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��ȡ�����
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//���Ҹ÷�λ�����ڵ���Ķ�λ��
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��λ�߶�Ӧ�ĵ���
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//��ѯ�������ڵ��ߣ�����Ϊ���
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//��λ�߶�Ӧ�ĵ���
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2��Ϊ���ߵĺ��
								if(nHandle_dw2!=0)
									nArrayHandleLine.Add(nHandle_dw2);
							}
						}
						else
						{//��λ�߲�Ϊ����ʱ
							nArrayHandleLine.Add(nHandle_fw);
						}
					}
				}
				else if(IsStart>0&&bForword==false)
				{
					//�����ƽ�ʱ���
					if(nHandle_fw!=0)
					{
						//nArrayHandleLine.Add(nHandle_fw);
						strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d"), nHandle_fw) ;
						long nLineType = 0 ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_line_type"), nLineType) ;
						if(nLineType==1)
						{
							//Ϊ����ʱ����λ�ߵĺ��
							long nHandleLeftJyj2 = 0, nHandleRightJyj2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_right=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleLeftJyj2) ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_conline_left=%d"), nHandle_fw) ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleRightJyj2) ;      
							//���������Ѱ�ң������Ҷ˾�Ե�ڡ�
							int nHandleEndConJyj = (bForword?nHandleRightJyj2:nHandleLeftJyj2) ; //��ĩ�����ӵľ�Ե��handle
							long nHandleNextLine2 = 0 ;
							strSql.Format(_T("select * from sxt_ent where sxt_ent_type>=200 and sxt_ent_type<220 and sxt_ent_handle=%d"), nHandleEndConJyj) ;
							CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��ȡ�����
							m_RecordSet.GetRecordValue(strSql, strFieldName, nHandleNextLine2);
							//���Ҹ÷�λ�����ڵ���Ķ�λ��
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleNextLine2) ;
							//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
							//��λ�߶�Ӧ�ĵ���
							long nHandleDC2=0;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC2);
							if(nHandleDC2!=0)
							{
								long nMinDc=GetDuXianMinDaoCha(nHandleDC,nHandleDC2);
								long nMaxDc=GetDuXianMaxDaoCha(nHandleDC,nHandleDC2);	
								//��ѯ�������ڵ��ߣ�����Ϊ���
								strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC2) ;
								//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
								//��λ�߶�Ӧ�ĵ���
								long nHandle_dw2=0;				
								//int IsStart=-1;
								//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), nHandle_dw2) ;
								//m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
								//nHandle_dw2��Ϊ���ߵĺ��
								if(nHandle_dw2!=0)
									nArrayHandleLine.Add(nHandle_dw2);
							}
						}
						//	acutPrintf(_T("Isstart=%d"),IsStart);
						else
						{//��λ�߲�Ϊ����ʱ
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

				//����Ϊĳһ������ķ�λ��
				//���п��ܵ�ǰ��Ϊĳһ������ķ�λ��
				strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d"), nHandleLineStart) ;
				//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
				//��λ�߶�Ӧ�ĵ���
				long nHandleDC=0;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDC);
				if(nHandleDC!=0)
				{
					//��ѯ�������ڵ��ߣ�����Ϊ���
					strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDC) ;
					//CString strFieldName = (bForword?_T("sxt_ent_conline_right"):_T("sxt_ent_conline_left")) ;
					//��λ�߶�Ӧ�ĵ���
					long nHandle_dw=0;
					// long nHandle_fw=0;						
					int IsStart=-1;
					CString DCname;
					//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
					//��������Ż�
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
						//��λ������Ϊ���
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
		AfxMessageBox(_T("δ֪�Ĵ���!in FindNextLine"));
	}	
}
BOOL CDaocha::GetDaidongDaochas( long nHandleDC,CString &str_daidongdaochas)
{
	try
	{
		//��ȡ�������ڶ�λ��
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
		//ȡ����λ��
		//��������Ż�
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
			//Ϊ���ߣ���������
			if(iStart1==0)
			{
				FindNextLineByJYJ(nHandle_fw_0,nHandleLineTfw,true);
				if(nHandleLineTfw!=0)
				{
					//�����λ������
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
					//�����λ������
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
		//��ȡ��λ�ߵ������������X������������
		//CLongArray nArrayHandleXHJ;
		CLongArray nArrayHandleDC;
		CLongArray nArrayHandleLineType;
		CLongArray nArrayHandleLineCur;
		//this->GetGuiDaoDaoChaOrderByXAsc(nHandle_dw,nArrayHandleDC,nArrayHandleLineCur,nArrayHandleLineType);
		this->GetOnLineDaoChaOrderByXAsc(nHandle_dw_0,nArrayHandleDC);
		int iNum_DC=0;
		iNum_DC =nArrayHandleDC.GetCount();
		//acutPrintf(_T("\n ����ϰ��� %d������"),iNum_DC);
		if(iNum_DC<2)
		{
			return FALSE;
		}
		//�жϵ����Ƿ�λ�ڹ�����
		int index=0;
		index=CBlkUtility::FindLongInArray(nArrayHandleDC,nHandleDC);
		//acutPrintf(_T("\n ����λ�ã�%d"),index);
		if(index==-1)
		{
			return FALSE;
		}
		//�����ƽ�ʱ,�ж������
		CLongArray nArrayHandleNext;
		long nHandleLineEnd=0;
		CStringArray sGuodaocha;
		CStringArray sGudao;
		//bool foword=true;
		if(iStart1==0)
		{
			//�����ƽ�
			this->FindNextLine(nHandle_dw_0,nHandleLineEnd,nArrayHandleNext,true);
			int iNum_Next=nArrayHandleNext.GetCount();
			//acutPrintf(_T("\n ���ҹ���ϰ��� %d�����"),iNum_Next);
			if(iNum_Next>=3)
			{
				for(int i=index+1;i<iNum_DC;i++)
				{
					long nHandle_DXDC=nArrayHandleDC[i];
					//��������һ���Ƕ��ߵ���
					long nHandle_DXAno=0;
					if(this->GetAnotherDuXianDaoCha(nHandle_DXDC,nHandle_DXAno)==TRUE)
					{
						strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_DXDC) ;
						long nHandle_fw=0;
						int IsStart=-1;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
						m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
						//����һ���������ڶ�λ�ߣ�
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
			//�����ƽ�
			CLongArray nArrayHandleNext2;
			long nHandleLineEnd2=0;
			//bool foword2=false;
			this->FindNextLine(nHandle_dw_0,nHandleLineEnd2,nArrayHandleNext2,false);
			int iNum_Next2=nArrayHandleNext2.GetCount();
			//acutPrintf(_T("\n �������ϰ��� %d�����"),iNum_Next2);
			if(iNum_Next2>=3)
			{
				for(int i=index-1;i>=0;i--)
				{
					long nHandle_DXDC=nArrayHandleDC[i];
					//long nHandle_DXMin=0;
					long nHandle_DXAno=0;
					//δ���ǽ�����ߴ��ڵ����
					if(this->GetAnotherDuXianDaoCha(nHandle_DXDC,nHandle_DXAno)==TRUE)
					{

						strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandle_DXDC) ;
						long nHandle_fw=0;
						int IsStart=-1;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_fwline_handle"),nHandle_fw) ;
						m_RecordSet.GetRecordValue(strSql,  _T("sxt_ent_fwline_vertex"),IsStart) ;
						//����һ���������ڶ�λ�ߣ�
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
		AfxMessageBox(_T("δ֪�Ĵ���!in GetdaidongDaoChas"));
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
		AfxMessageBox(_T("δ֪�Ĵ���!in GetEntName"));
	}	
}

CDaocha::~CDaocha(void)
{
}

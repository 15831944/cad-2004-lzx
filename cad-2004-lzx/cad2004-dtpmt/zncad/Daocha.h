#pragma once
#include "StdAfx.h"
#include "../blkuserdll/UserSystem.h"
#include "../blkdll/MyAdo.h"
class CDaocha
{
public:
	CDaocha(void);
	CDaocha(long nHandleDC);
	//CZnSxt m_Sxt ;
	CUserSystem m_sys ;
	CADODatabase m_AdoDb ;
	CADORecordset m_RecordSet ;
	virtual ~CDaocha(void);
	BOOL GetAnotherDuXianDaoCha( long nHandleDc_One,long & nHandleDc_Another );
//	BOOL GetAnotherDuXianDaoCha( long nHandleDc_One,long & nHandleDc_Another );
	BOOL GetJiaoChaDuXianAnotherDaoChas( long nHandleDc_One,long nHandleDc_Another,long &nHandleDc_Min,long &nHandleDc_Max );
	//BOOL CDaocha::GetJiaoChaDuXianAnotherDaoChas( long nHandleDc_One,long nHandleDc_Another,long &nHandleDc_Min,long &nHandleDc_Max )
	void GetOnLineDaoChaOrderByXAsc(UINT nLineHandle, CLongArray& nArrayHandleDC);
	long GetDuXianMaxDaoCha( long nHandleDc_One,long nHandleDc_Another );
	long GetDuXianMinDaoCha( long nHandleDc_One,long nHandleDc_Another );
	void GetGuiDaoDaoChaOrderByXAsc(UINT nLineHandle, CLongArray& nArrayHandleDC, CLongArray& nArrayLineDoneCur, CLongArray& nArrayLineCurType);
	BOOL GetDaidongDaochas1( long nHandleDC, CString &str_daidongdaochas);
	BOOL GetDaidongDaochas( long nHandleDC,CString &str_daidongdaochas);
	BOOL FindNextLineByJYJ( long nHandleLineStart,long &nHandleLineNext,bool bForward );
	BOOL IsOntheHorizonline( long nHandleLineStart,long nHandleLineEnd,bool bForward );
	void FindNextLine( long nHandleLineStart,long nHandleLineEnd,CLongArray&nArrayHandleLine,bool bForword/*=true*/ );
	void GetOnLineChaOrderByXDesc(UINT nLineHandle, CLongArray& nArrayHandleDC);
	void GetEntName( long nHandle,CString &Entname );
	long m_nHandleDC;//����
	CString m_strDcName;//��������
	long m_nHandleLine_fw;//����λ��
	long m_nHandleLine_dw;//����λ��
	double m_dDcEnt_x;//����x����
	int m_nHandleLine_fw_vertex;
	long m_nHandle_Another_DC;//˫�����������һ������
	long m_nHandle_Jiaocha_DCMin;//������ߣ�����һ��˫������ı�Ž�С�ĵ���
	long m_nHandle_Jiaocha_DCMax;//������ߣ�����һ��˫������ı�Žϴ�ĵ���
	long m_nHandleDC_Min;
	long m_nHandleDC_Max;
	CString m_str_Another_DC;
	CString m_str_Jiaocha_DCMin;
	CString m_str_Jiaocha_DCMax;
	CString m_strDC_Max;
	CString m_strDC_Min;
	CString m_strDaidong;
};

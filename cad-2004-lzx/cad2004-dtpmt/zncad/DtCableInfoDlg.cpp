// (C) Copyright 1990-2002 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- DtCableInfoDlg.cpp : Implementation of CDtCableInfoDlg
//-----------------------------------------------------------------------------
#include "StdAfx.h"
//#include "resource.h"
#include "DtCableInfoDlg.h"
#include "ZnUtility.h"

//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CDtCableInfoDlg, CAcUiDialog)

BEGIN_MESSAGE_MAP(CDtCableInfoDlg, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_MESSAGE(ZHFMSG_ZHFEDIT_TEXTCHANGED, OnTextChanged)
//	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CDtCableInfoDlg::CDtCableInfoDlg (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAcUiDialog (CDtCableInfoDlg::IDD, pParent, hInstance) , m_dWanquRatio(0)
, m_dLength2(0)
, m_iLength3(0)
, m_dDisttoCable(0)
, m_iZuotou(0)
, m_danzhuang(0)
, m_dGuogui(0)
, m_dJiexuPer(0)
, m_dRemFmPer(0)
, m_dSXjiaPer(0)
{
	//CZnUtility::GetCableParam(m_dDisttoCable, m_dZuotou, m_fanzhuang, m_fGuogui, m_fJiexuPer, m_fRemFmPer, m_fSXjiaPer) ;
	//初始化参数
// 	m_dWanquRatio=1.1;
// 	m_dDisttoCable=80;
// 	m_iZuotou=4;
// 	m_danzhuang=5;
// 	m_dGuogui=6;
// 	m_dJiexuPer=8;
// 	m_dRemFmPer=10;
// 	m_dSXjiaPer=3.5;
	CZnUtility::DT_GetCableParam(m_dWanquRatio,m_iZuotou,m_dGuogui,m_dRemFmPer,m_danzhuang,m_dJiexuPer,m_dSXjiaPer,m_iDistJX);
}
////0：设备坐标1：1：设备坐标2；2：做头； 3设备室之电缆孔；4 过道；5长短链；6 上下架；7 人防门 ；8 安装余量 9 修正量 
//-----------------------------------------------------------------------------
void CDtCableInfoDlg::DoDataExchange (CDataExchange *pDX) {
	CAcUiDialog::DoDataExchange (pDX) ;
	DDX_Control(pDX, IDC_EDIT_DTCABLEINFO_ALL, m_editCableString);
	DDX_Control(pDX, IDC_EDIT_DTCABLEINFO_LENFORPLUS, m_editIsBack);
	DDX_Control(pDX, IDC_EDIT_DTCABLEINFO_DIST1, m_edit_deviceLocate);
	DDX_Control(pDX, IDC_EDIT_DTCABLEINFO_DIST2, m_edit_CableLocate);
	DDX_Control(pDX, IDC_EDIT_DTCABLEINFO_ZT, m_edit_Zuotou);
	DDX_Control(pDX, IDC_EDIT_DTCABLEINFO_TOCABLE, m_edit_distToCable);
	DDX_Control(pDX, IDC_EDIT_DTCABLEINFO_GD, m_edit_Guodao);
	DDX_Control(pDX, IDC_EDIT_DTCABLEINFO_CDLIAN, m_edit_cdlian);
	DDX_Control(pDX, IDC_EDIT_DTCABLEINFO_SX, m_edit_sxjia);
	DDX_Control(pDX, IDC_EDIT_DTCABLEINFO_RFM, m_edit_RenFm);
	DDX_Control(pDX, IDC_EDIT_DTCABLEINFO_ANZHU, m_edit_anzhuang);
	DDX_Control(pDX, IDC_EDIT_DTCABLEINFO_XIUZ, m_edit_xiuzheng);
	DDX_Control(pDX, IDC_EDIT_DTCABLEINFO_LEN1, m_edit_Length1);
	DDX_Control(pDX, IDC_EDIT_DTCABLEINFO_CABLENAME, m_edit_CableName);
	DDX_Control(pDX, IDC_EDIT_DTCABLEINFO_BACKXINSHU, m_edit_backxinshu);
	DDX_Control(pDX, IDC_EDIT_DTCABLEINFO_CABLETYPE, m_edit_CableType);
	DDX_Control(pDX, IDC_EDIT_DTCABLEINFO_CABLENUM, m_edit_CableNum);
	DDX_Control(pDX, IDC_EDIT_DTCABLEINFO_OLD, m_editCableInfoOld);
	DDX_Control(pDX, IDC_EDIT_DTCABLEINFO_LEN, m_editLength);
	DDX_Control(pDX, IDC_EDIT_DTCABLEINFO_WANQU1, m_editWanquRatio);
    DDX_Control(pDX, IDC_EDIT_DTCABLEINFO_LEN2, m_edit_Length2);
	DDX_Control(pDX, IDC_EDIT_DTCABLEINFO_LEN3, m_edit_Length3);
	//DDX_Text(pDX, IDC_EDIT_DTCABLEINFO_LEN2, m_dLength2);
	//DDX_Text(pDX, IDC_EDIT_DTCABLEINFO_LEN3, m_iLength3);
	DDX_Text(pDX, IDC_EDIT_DTCABLEINFO_WANQU2, m_dWanquRatio);
	DDX_Text(pDX, IDC_EDIT_DTCABLEINFO_DISTJIEXU, m_iDistJX);
	DDX_Text(pDX, IDC_EDIT_DTCABLEINFO_ZUOTOU, m_iZuotou);
	DDX_Text(pDX, IDC_EDIT_DTCABLEINFO_ANZHUANG, m_danzhuang);
	DDX_Text(pDX, IDC_EDIT_DTCABLEINFO_GUOGUI, m_dGuogui);
	DDX_Text(pDX, IDC_EDIT_DTCABLEINFO_PERJIEXU, m_dJiexuPer);
	DDX_Text(pDX, IDC_EDIT_DTCABLEINFO_PERRENFM, m_dRemFmPer);
	DDX_Text(pDX, IDC_EDIT_DTCABLEINFO_PERSXJ, m_dSXjiaPer);
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CDtCableInfoDlg::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}

int CDtCableInfoDlg::SetEditText()
{	
	int iCount = m_dArrayData.GetCount() ;
	if (iCount<7)
	{
		acutPrintf(_T("\n电缆信息对话框设置文本失败，因为存储数据数组小于预定个数6")) ;
		return 0 ;
	}
	//0：设备坐标1：1：设备坐标2；2：做头； 3设备室之电缆孔；4 过道；5长短链；6 上下架；7 人防门 ；8 安装余量 9 修正量 
	double dDist1 = m_dArrayData.GetAt(0) ;
	double dDist2 = m_dArrayData.GetAt(1) ;
	double dDist = fabs(dDist1-dDist2);
	double d2 = m_dArrayData.GetAt(2) ;
	double d3 = m_dArrayData.GetAt(3) ;
	double d4 = m_dArrayData.GetAt(4) ;
	double d5 = m_dArrayData.GetAt(5) ;
	double d6 = m_dArrayData.GetAt(6) ;
	double d7=	 m_dArrayData.GetAt(7);
	double d8 = m_dArrayData.GetAt(8);
	double d9 = m_dArrayData.GetAt(9);
	double dLen1=(dDist+d2*m_iZuotou+d3+d4*m_dGuogui+d5+d6*m_dSXjiaPer+d7*m_dRemFmPer+d8*m_danzhuang+d9);
	//接续后长度
	double dLen2= (((int)dLen1)/m_iDistJX)*m_dJiexuPer+dLen1;
	//弯曲系数
	double dLen3=dLen2*m_dWanquRatio;
	//对五取整后长度，这里会发生向下取整
	int iLen3=(int) dLen3;
	if((dLen3-iLen3)>0.001)
	{
		//向上取整
		iLen3++;
	}
	double dLen= ((iLen3)%5)==0?(iLen3):(5+(iLen3/5)*5);
	m_dLength2=dLen2;
	m_iLength3=dLen3;
	//设置Edit
	for (int i=IDC_EDIT_DTCABLEINFO_ALL; i<=IDC_EDIT_DTCABLEINFO_LEN3; i++)
	{
		CZhfAcUiEdit* pEdit = (CZhfAcUiEdit*)this->GetDlgItem(i) ;
		if (i==IDC_EDIT_DTCABLEINFO_ALL)
		{
			CString strCableAllTmp ;
			CString strCableName;
			m_edit_CableName.GetWindowText(strCableName);
			CString strCableNum;
			m_edit_CableNum.GetWindowText(strCableNum);
// 			CString strCableText;
// 			m_edit_CableType.GetWindowText(strCableText);
			CString strBackNum;
 			//m_edit_backxinshu.GetWindowText(m_strCableBackxinshu);
			//m_edit_backxinshu.GetInt(m_iCableBackxinshu);
			//m_iCableBackxinshu=StrToIntA(strBackNum);
			if(strCableNum!=_T(""))
			{
				strCableAllTmp.Format(_T("%s %sx%d-%s(%s)"), strCableName,strCableNum,(int)dLen, m_strCableType, m_strCableBackxinshu) ;
			}
			else 
			{
				strCableAllTmp.Format(_T("%s %d-%s(%s)"), strCableName,(int)dLen, m_strCableType, m_strCableBackxinshu) ;
			}
			pEdit->SetWindowText(strCableAllTmp) ;
		}
		else if(i==IDC_EDIT_DTCABLEINFO_OLD)
		{
			pEdit->SetWindowText(m_strCableAllOld);
		}
		else if(i==IDC_EDIT_DTCABLEINFO_LENFORPLUS)
		{
			pEdit->SetInt(m_bIsback);
		}
		else if (i==IDC_EDIT_DTCABLEINFO_LENFORPLUS)
		{
			//pEdit->SetDouble(dLen) ;
			pEdit->SetInt(0);
		}
		else if(i<=IDC_EDIT_DTCABLEINFO_XIUZ)
		{
			pEdit->SetDouble(m_dArrayData.GetAt(i-IDC_EDIT_DTCABLEINFO_DIST1)) ;
		}
		else if(i==IDC_EDIT_DTCABLEINFO_LEN)
		{
			pEdit->SetDouble(dLen);
		}
		else if(i==IDC_EDIT_DTCABLEINFO_WANQU1)
		{
			pEdit->SetDouble(m_dWanquRatio);
		}
		else if(i==IDC_EDIT_DTCABLEINFO_LEN1)
		{
			pEdit->SetDouble(dLen1);
		}
		else if(i==IDC_EDIT_DTCABLEINFO_LEN2)
		{
			pEdit->SetDouble(dLen2);

		}
		else if(i==IDC_EDIT_DTCABLEINFO_LEN3)
		{
			pEdit->SetDouble(dLen3);
		}
		else if(i==IDC_EDIT_DTCABLEINFO_BACKXINSHU)
		{
			pEdit->SetWindowText(m_strCableBackxinshu);
		}
		else if(i==IDC_EDIT_DTCABLEINFO_CABLETYPE)
		{
			pEdit->SetWindowText(m_strCableType);
		}
	}
	//m_editType.SetWindowText(m_strCableTypeName) ;
	//m_editWanquRatio.SetWindowText(m_dWanquRatio);
	//m_editBackup.SetInt(m_nBackup) ;

	return 0;
}
void CDtCableInfoDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_editCableString.GetWindowText(m_strCableAll) ;
	m_dArrayData.RemoveAll() ;
	for (int i=IDC_EDIT_DTCABLEINFO_DIST1; i<=IDC_EDIT_DTCABLEINFO_XIUZ; i++)
	{
		CZhfAcUiEdit* pEdit = (CZhfAcUiEdit*)this->GetDlgItem(i) ;
		double iTmp = 0 ;
		pEdit->GetDouble(iTmp) ;
		m_dArrayData.Add(iTmp) ;
	}
	int iIsBack=0;
	CZhfAcUiEdit* pEdit = (CZhfAcUiEdit*)this->GetDlgItem(IDC_EDIT_DTCABLEINFO_LENFORPLUS) ;
	pEdit->GetInt(iIsBack);
	m_bIsback=iIsBack;
	 pEdit = (CZhfAcUiEdit*)this->GetDlgItem(IDC_EDIT_DTCABLEINFO_BACKXINSHU) ;
	pEdit->GetWindowText(m_strCableBackxinshu);
	pEdit = (CZhfAcUiEdit*)this->GetDlgItem(IDC_EDIT_DTCABLEINFO_CABLETYPE) ;
	pEdit->GetWindowText(m_strCableType);
	CAcUiDialog::OnOK();
}

BOOL CDtCableInfoDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化
	CString strTipTitle = _T("提示：") ;
	CString strRegInt = _T("^-?[0-9]+[0-9]*$") ;
	CString strTipInt = _T("请输入整数!") ;
	CString strRegDouble = _T("^-?(0|[1-9]+[0-9]*)(\\.\\d{0,3})?$") ; //小数部分不超过3位
	CString strTipDouble = _T("请输入小数位数最多3位的小数!") ;
// 	for (int i=IDC_EDIT_DTCABLEINFO_ALL; i<=IDC_EDIT_DTCABLEINFO_WANQU1; i++)
// 	{
// 		CZhfAcUiEdit* pEdit = (CZhfAcUiEdit*)this->GetDlgItem(i) ;
// 		if (i==IDC_EDIT_CABLEINFO_ALL)
// 		{			
// 		}
// 		else if (i==IDC_EDIT_CABLEINFO_TYPE)
// 		{
// 			//pEdit->SetValidate(_T("^\\d+[A-Z]*$"), strTipTitle, _T("输入示例:4、16A")) ;
// 		}
// 		else 
// 		{
// 			//pEdit->SetValidate(strRegInt, strTipTitle, strTipInt) ;
// 		}
// // 				else
// // 				// 		{
// // 				// 			pEdit->SetValidate(strRegDouble, strTipTitle, strTipDouble) ;
// // 				// 			pEdit->SetWindowText(_T("0")) ;
// // 				// 		}
//  	}
	this->SetEditText() ;
	//将对话框挪到指定位置显示
	CRect rectClient ;
	this->GetWindowRect(rectClient) ;		
	//rectClient.DeflateRect(5,0,5,0) ;
	rectClient.left+=100;
	rectClient.right+=100;
	rectClient.top+=100;
	rectClient.bottom+=100;
	//rectClient.bottom-=68 ;
	this->SetWindowPos(&wndTop,rectClient.left,rectClient.top,rectClient.Width(),rectClient.Height(),SWP_SHOWWINDOW);
	//this->CreateDlg()
 	//m_grid.CreateGrid( WS_CHILD|WS_VISIBLE,rectClient, this, IDC_LIST_GRID );
 	return TRUE;  // return TRUE unless you set the focus to a control
// 	// 异常：OCX 属性页应返回 FALSE

}

BOOL CDtCableInfoDlg::PreTranslateMessage( MSG* pMsg )
{
	return CAcUiDialog::PreTranslateMessage(pMsg);

}

LRESULT CDtCableInfoDlg::OnTextChanged( WPARAM wparam,LPARAM lparam )
{
	HWND hWnd = (HWND)wparam ;

	CString strText ;	
	if (hWnd==m_edit_deviceLocate.m_hWnd||hWnd==m_edit_distToCable.m_hWnd||hWnd==m_edit_CableLocate.m_hWnd||hWnd==m_edit_Zuotou.m_hWnd||hWnd==m_edit_Guodao.m_hWnd||hWnd==m_edit_sxjia.m_hWnd||hWnd==m_edit_cdlian.m_hWnd||hWnd==m_edit_RenFm.m_hWnd||hWnd==m_edit_anzhuang.m_hWnd||hWnd==m_edit_xiuzheng.m_hWnd)
	{
		//int iNumBoxF = ComputeNumBoxF() ;
		//m_editBoxFNum.SetInt(iNumBoxF) ;		
        double dDist1=0;
		double dDist2=0;
		double dZ=0;
		double dS=0;
		double dG=0;
		double dC=0;
		double dH=0;
		double dR=0;
		double dA=0;
		double dx=0;
		double dLen1=0;
		m_edit_deviceLocate.GetDouble(dDist1);
		m_edit_CableLocate.GetDouble(dDist2);
		m_edit_Zuotou.GetDouble(dZ);
		m_edit_distToCable.GetDouble(dS);
		m_edit_sxjia.GetDouble(dH);
		m_edit_Guodao.GetDouble(dG);
		m_edit_cdlian.GetDouble(dC);
		m_edit_RenFm.GetDouble(dR);
		m_edit_anzhuang.GetDouble(dA);
		m_edit_xiuzheng.GetDouble(dx);
		//dL1=(fabs(dDist1-dDist2)+dS+)
	     dLen1=(fabs(dDist1-dDist2)+dS+dZ*m_iZuotou+dC+dG*m_dGuogui+dH*m_dSXjiaPer+dR*m_dRemFmPer+dA*m_danzhuang+dx);
		//接续后长度
		double dLen2= (((int)dLen1)/m_iDistJX)*m_dJiexuPer+dLen1;
		//弯曲系数
		double dLen3=dLen2*m_dWanquRatio;
		//对五取整后长度
		int iLen3=(int)dLen3;
		if((dLen3-iLen3)>0.001)
		{
			iLen3++;
		}
		double dLen= ((iLen3)%5)==0?((iLen3)):(5+(iLen3/5)*5);
		m_edit_Length1.SetDouble(dLen1);
		m_edit_Length2.SetDouble(dLen2);
		m_edit_Length3.SetDouble(dLen3);
		m_editLength.SetDouble(dLen);
	}
	else if (hWnd==m_edit_CableType.m_hWnd||hWnd==m_edit_backxinshu.m_hWnd||hWnd==m_edit_CableName.m_hWnd||hWnd==m_edit_CableNum.m_hWnd||hWnd==m_editLength.m_hWnd)
	{
// 		CString strType ;
// 		//int i1 = 0 ;
// 		CString strCableNum;
// 		m_edit_CableType.GetWindowText(strType) ;
// 		m_edit_backxinshu.GetWindowText(strCableNum) ;		
		double dLen1 = 0 ;
		m_editLength.GetDouble(dLen1) ;
		//int iLen = (iLen1/10)*10+((iLen1%10)<5?5:10) ;
// 		strText.Format(_T("%d-%s(%s)"), (int)dLen1, strType, strCableNum) ;
// 		m_editCableString.SetWindowText(strText) ;			
		CString strCableAllTmp ;
		CString strCableName;
		m_edit_CableName.GetWindowText(strCableName);
		CString strCableNum;
		m_edit_CableNum.GetWindowText(strCableNum);
		//CString strCableText;
		m_edit_CableType.GetWindowText(m_strCableType);
		//CString strBackNum;
		//m_edit_backxinshu.GetWindowText(strBackNum);
		//m_iCableBackxinshu=StrToIntA(strBackNum);
	    m_edit_backxinshu.GetWindowText(m_strCableBackxinshu);
		if(strCableNum!=_T(""))
		{
			strCableAllTmp.Format(_T("%s %sx%d-%s(%s)"), strCableName,strCableNum,(int)dLen1, m_strCableType, m_strCableBackxinshu) ;
		}
		else 
		{
			strCableAllTmp.Format(_T("%s %d-%s(%s)"), strCableName,(int)dLen1, m_strCableType, m_strCableBackxinshu) ;
		}
		m_editCableString.SetWindowText(strCableAllTmp) ;
	}
// 	else if (hWnd==m_editLength.m_hWnd)
// 	{
// 		double dLen1= 0 ;
// 		m_editLength.GetDouble(dLen1) ;
// 		//int iLen = (iLen1/10)*10+((iLen1%10)<5?5:10) ;
// 		CString strType ;		
// 		m_edit_CableType.GetWindowText(strType) ;
// 		//int iBackup = 0 ;
// 		CString strbackup;
// 		m_edit_backxinshu.GetWindowText(strbackup) ;		
// 		strText.Format(_T("%d-%s(%s)"),(int)dLen1, strType, strbackup) ;
// 		m_editCableString.SetWindowText(strText) ;				
// 	}

	return 0 ;

}



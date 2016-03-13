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
//----- DtCableInfoDlg.h : Declaration of the CDtCableInfoDlg
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "acui.h"

#include "afxwin.h"
#include "ZhfAcUiEdit.h"
#include "CableCoreListUGCtrl.h"

//-----------------------------------------------------------------------------
class CDtCableInfoDlg : public CAcUiDialog {
	DECLARE_DYNAMIC (CDtCableInfoDlg)

public:
	CDtCableInfoDlg (CWnd *pParent =NULL, HINSTANCE hInstance =NULL) ;

	enum { IDD = IDD_DTCABLEINFO} ;

protected:
	virtual void DoDataExchange (CDataExchange *pDX) ;
	afx_msg LRESULT OnAcadKeepFocus (WPARAM, LPARAM) ;
	afx_msg LRESULT OnTextChanged(WPARAM wparam,LPARAM lparam);
	//afx_msg LRESULT OnAcadKeepFocus (WPARAM, LPARAM) ;
	void OnOK();
	DECLARE_MESSAGE_MAP()	
//	void OnOK();

public:
	//电缆字符串
	CZhfAcUiEdit m_editCableString;
	//是否加备用
	CZhfAcUiEdit m_editIsBack;
	//设备坐标
	CZhfAcUiEdit m_edit_deviceLocate;
	//电缆孔或者设备2坐标
	CZhfAcUiEdit m_edit_CableLocate;
	//做头及环储
	CZhfAcUiEdit m_edit_Zuotou;
	//设备室至电缆孔
	CZhfAcUiEdit m_edit_distToCable;
	//过道
	CZhfAcUiEdit m_edit_Guodao;
	//长短链
	CZhfAcUiEdit m_edit_cdlian;
	//上下托架
	CZhfAcUiEdit m_edit_sxjia;
	//人防门
	CZhfAcUiEdit m_edit_RenFm;
	//安装余量
	CZhfAcUiEdit m_edit_anzhuang;
	//修正量
	CZhfAcUiEdit m_edit_xiuzheng;
	//长度1
	CZhfAcUiEdit m_edit_Length1;
	//电缆名称
	CZhfAcUiEdit m_edit_CableName;
	//备用芯数
	CZhfAcUiEdit m_edit_backxinshu;
	//电缆型号
	CZhfAcUiEdit m_edit_CableType;
	//电缆根数
	CZhfAcUiEdit m_edit_CableNum;
	//原有电缆信息
	CZhfAcUiEdit m_editCableInfoOld;
	//电缆长度
	CZhfAcUiEdit m_editLength;
	//弯曲系数
	CZhfAcUiEdit m_editWanquRatio;
	
	CZhfAcUiEdit m_edit_Length2;
	CZhfAcUiEdit m_edit_Length3;
	//长度L2
	double m_dLength2;
	//长度L3
	int  m_iLength3;
	//参数
	double m_dWanquRatio;//弯曲系数
	double m_dDisttoCable;//设备室至孔
	int  m_iZuotou;//做头
	double m_danzhuang;//安装余量
	double m_dGuogui;//每过轨增缆
	double m_dJiexuPer;//每接续增缆
	double  m_dRemFmPer;//每人防门增缆
	double m_dSXjiaPer;//每上下架增缆
	int m_iDistJX;//接续间距
    CDoubleArray m_dArrayData ;
	//完整电缆字符串
	CString m_strCableAll;
	CString m_strCableAllOld;
	int m_bIsback;
	CString m_strCableType;
	CString m_strCableBackxinshu;
	CCableCoreListUGCtrl m_grid;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	int SetEditText();
	//CEdit m_editLength2;
	//CEdit m_editLength3;
	//afx_msg void OnBnClickedOk();
} ;

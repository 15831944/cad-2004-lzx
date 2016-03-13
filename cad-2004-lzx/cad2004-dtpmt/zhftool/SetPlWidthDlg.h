#pragma once

// #include "acui.h"
// #include "acuibutton.h"
// #include "acuiedit.h"

// CSetPlWidthDlg 对话框

class CSetPlWidthDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(CSetPlWidthDlg)

public:
	//CSetPlWidthDlg(CWnd* pParent = NULL);   // 标准构造函数
	CSetPlWidthDlg (CWnd *pParent =NULL, HINSTANCE hInstance =NULL) ;

	virtual ~CSetPlWidthDlg();

// 对话框数据
	enum { IDD = IDD_SETPLWIDTH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg LRESULT OnAcadKeepFocus (WPARAM, LPARAM) ;

	DECLARE_MESSAGE_MAP()
public:
	double m_dFrom;
	double m_dTo;
protected:
	virtual void OnOK();
public:
	BOOL m_bIncludeBlk;

	CAcUiSelectButton m_btnSel;
	afx_msg void OnBnClickedSel();
 	virtual BOOL OnInitDialog();

	ads_name m_ss;
	void Clear(void);
	int m_iComoBoxIndex ;
	void UpdateComoBox(void);
};

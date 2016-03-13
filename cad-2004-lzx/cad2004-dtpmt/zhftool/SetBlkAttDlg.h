#pragma once
// #include "afxwin.h"
#include "AttUGCtrl.h"


// CSetBlkAttDlg 对话框

class CSetBlkAttDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(CSetBlkAttDlg)

public:
	CSetBlkAttDlg(CWnd* pParent = NULL, HINSTANCE hInstance =NULL);   // 标准构造函数
	virtual ~CSetBlkAttDlg();

// 对话框数据
	enum { IDD = IDD_SETBLKATT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg LRESULT OnAcadKeepFocus (WPARAM, LPARAM) ;


	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_strBlkName;
	CComboBox m_comboxBlkName;
protected:
	virtual void OnOK();
public:
	afx_msg void OnCbnSelchangeComboSetblkattBlkname();
	
	CAcUiSelectButton m_btnSel;

	ads_name m_ss;
	void Clear(void);

	afx_msg void OnBnClickedButtonSetblkattSel();
	int m_iRange;
	int m_iColorIndex;

	CAttUGCtrl m_grid ;
	afx_msg void OnBnClickedRadioSetblkattAll(UINT id);

	void RefreshGrid(void);
	int UpdateComboBox();
	int GetSelected(void);
};

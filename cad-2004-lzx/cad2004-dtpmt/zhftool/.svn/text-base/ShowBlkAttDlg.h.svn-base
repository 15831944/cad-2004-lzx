#pragma once
//#include "afxwin.h"


// CShowBlkAttDlg 对话框

class CShowBlkAttDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(CShowBlkAttDlg)

public:
	CShowBlkAttDlg(CWnd* pParent = NULL, HINSTANCE hInstance =NULL);   // 标准构造函数
	virtual ~CShowBlkAttDlg();

// 对话框数据
	enum { IDD = IDD_SHOWBLKATT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg LRESULT OnAcadKeepFocus (WPARAM, LPARAM) ;

	DECLARE_MESSAGE_MAP()
public:
	CString m_strBlkName;
	CString m_strAtt;
	int m_iShowOrHide;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CComboBox m_comboBoxBlkName;
	CComboBox m_comboBoxAtt;
	CAcUiSelectButton m_btnSel;
	ads_name m_ss;
	void Clear(void);

	afx_msg void OnBnClickedButtonShowblkattSel();
	afx_msg void OnCbnSelchangeComboShowblkattBlkname();
	int UpdateComoBlkName(void);
	int UpdateComboAtt(void);
	int m_iRange;
};

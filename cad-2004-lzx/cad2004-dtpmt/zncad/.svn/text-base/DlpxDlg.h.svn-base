#pragma once


// CDlpxDlg 对话框

class CDlpxDlg : public CDialog
{
	DECLARE_DYNAMIC(CDlpxDlg)

public:
	CDlpxDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlpxDlg();

// 对话框数据
	enum { IDD = IDD_DLPX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strSavePath;
	CString m_strMode;
	afx_msg void OnBnClickedButtonDlpxSelpath();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};

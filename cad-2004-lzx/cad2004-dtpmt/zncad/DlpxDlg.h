#pragma once


// CDlpxDlg �Ի���

class CDlpxDlg : public CDialog
{
	DECLARE_DYNAMIC(CDlpxDlg)

public:
	CDlpxDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlpxDlg();

// �Ի�������
	enum { IDD = IDD_DLPX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strSavePath;
	CString m_strMode;
	afx_msg void OnBnClickedButtonDlpxSelpath();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};

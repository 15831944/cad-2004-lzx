#pragma once
#include "resource.h"


// CDtAddBoxInfo �Ի���

class CDtAddBoxInfo : public CDialog
{
	DECLARE_DYNAMIC(CDtAddBoxInfo)

public:
	CDtAddBoxInfo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDtAddBoxInfo();

// �Ի�������
	enum { IDD = IDD_ADDBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg LRESULT OnTextChanged(WPARAM wparam,LPARAM lparam);
	DECLARE_MESSAGE_MAP()
public:
// 	int m_iSelShape;
// 	bool m_iSelName;
	static const CString strboxName[10];
	int m_iSelShape;
	int m_iSelName;
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
	virtual void OnCancel();
public:
	CString m_strboxName;
/*	afx_msg void OnBnHotItemChangeRadio3(NMHDR *pNMHDR, LRESULT *pResult);*/
	afx_msg void OnBnClickedRadio3(UINT id);
	afx_msg void OnBnClickedRadio1(UINT id);
/*	afx_msg void OnBnSetfocusRadio3();*/
/*	afx_msg void OnEnChangeEdit1();*/
};

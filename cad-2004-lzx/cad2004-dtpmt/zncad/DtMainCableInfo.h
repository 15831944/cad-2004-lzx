#pragma once
#include "Resource.h"


// CDtMainCableInfo 对话框

class CDtMainCableInfo : public CDialog
{
	DECLARE_DYNAMIC(CDtMainCableInfo)

public:
	CDtMainCableInfo(CWnd* pParent = NULL);   // 标准构造函数
	CDtMainCableInfo(AcDbEntity *pEnt);   // 标准构造函数
//	CDtMainCableInfo(CWnd* pParent /*=NULL*/);
	virtual ~CDtMainCableInfo();
	virtual BOOL OnInitDialog();

// 对话框数据
	enum { IDD = IDD_DTMAINCABLEINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_iSelDevice;
	int m_iSelColor;
	static const CString strDeviceTypeItem[8];
	AcDbEntity* m_pEnt;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};

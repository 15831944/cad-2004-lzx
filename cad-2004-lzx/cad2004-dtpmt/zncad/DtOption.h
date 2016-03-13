#pragma once


// CDtOption 对话框

class CDtOption : public CDialog
{
	DECLARE_DYNAMIC(CDtOption)

public:
	CDtOption(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDtOption();
	virtual BOOL OnInitDialog();

// 对话框数据
	enum { IDD = IDD_DTOPTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
		virtual void OnOK();

	DECLARE_MESSAGE_MAP()
public:
	double m_dWanquRatio;
	int m_iZuoTou;
	double m_dGuoGuiPer;
	int m_iDistJieX;
	double m_dRFMPer;
	double m_dAZPer;
	double m_dJXPer;
	double m_dSXJiaPer;
};

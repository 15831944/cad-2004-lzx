#pragma once


// CDtOption �Ի���

class CDtOption : public CDialog
{
	DECLARE_DYNAMIC(CDtOption)

public:
	CDtOption(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDtOption();
	virtual BOOL OnInitDialog();

// �Ի�������
	enum { IDD = IDD_DTOPTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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

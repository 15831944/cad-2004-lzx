// DtOption.cpp : 实现文件
//

#include "stdafx.h"
//#include "zncad.h"
#include "DtOption.h"
#include "ZnUtility.h"

// CDtOption 对话框

IMPLEMENT_DYNAMIC(CDtOption, CDialog)
CDtOption::CDtOption(CWnd* pParent /*=NULL*/)
	: CDialog(CDtOption::IDD, pParent)
	, m_dWanquRatio(1.1)
	, m_iZuoTou(4)
	, m_dGuoGuiPer(6)
	, m_iDistJieX(500)
	, m_dRFMPer(10)
	, m_dAZPer(5)
	, m_dJXPer(8)
	, m_dSXJiaPer(3.5)
{
}

CDtOption::~CDtOption()
{
}

void CDtOption::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_DTOPTION_WQRATIO, m_dWanquRatio);
	DDX_Text(pDX, IDC_EDIT_DTOPTION_ZT, m_iZuoTou);
	DDX_Text(pDX, IDC_EDIT_DTOPTION_GUOGUIPER, m_dGuoGuiPer);
	DDX_Text(pDX, IDC_EDIT_DTOPTION_DISTJIEXU, m_iDistJieX);
	DDX_Text(pDX, IDC_EDIT_DTOPTION_RFMPER, m_dRFMPer);
	DDX_Text(pDX, IDC_EDIT_DTOPTION_AZYULIANG, m_dAZPer);
	DDX_Text(pDX, IDC_EDIT_ZNOPTION_CABLE_PERBOXF, m_dJXPer);
	DDX_Text(pDX, IDC_EDIT_DTOPTION_SXJPER, m_dSXJiaPer);
}


BEGIN_MESSAGE_MAP(CDtOption, CDialog)
END_MESSAGE_MAP()

BOOL CDtOption::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CZnUtility::DT_GetCableParam(m_dWanquRatio, m_iZuoTou, m_dGuoGuiPer, m_dRFMPer, m_dAZPer, m_dJXPer, m_dSXJiaPer,m_iDistJieX) ;
// 	CBlkUtility::QueryValueReg(_T("ZnOption"), _T("TolCheckGd"), m_dTolCheckGd) ;
// 	CBlkUtility::QueryValueReg(_T("ZnOption"), _T("TolJyjXhjToGd"), m_dTolJyjXhjToGd) ;
// 	CBlkUtility::QueryValueReg(_T("ZnOption"), _T("TolJyjXhjOverlap"), m_dTolJyjXhjOverlap) ;
// 	CBlkUtility::QueryValueReg(_T("ZnOption"), _T("TwoOrFourLine"), m_i4Or2Line) ;
// 	CBlkUtility::QueryValueReg(_T("ZnOption"), _T("XhjDsNum"), m_iDsNumXhj) ;
// 	CBlkUtility::QueryValueReg(_T("ZnOption"), _T("DcQkNum"), m_iQkNum) ;
// 	CBlkUtility::QueryValueReg(_T("ZnOption"), _T("DcDsNum"), m_iDcDsNum) ;
// 	CBlkUtility::QueryValueReg(_T("ZnOption"), _T("TukuXhjWithJyj"), m_iTukuXhjWithJyj) ;

	this->UpdateData(FALSE) ;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE

}

void CDtOption::OnOK()
{
	this->UpdateData(TRUE) ;

	CBlkUtility::SetValueReg(_T("DtOption"), _T("WanquRatio"), m_dWanquRatio) ;
	CBlkUtility::SetValueReg(_T("DtOption"), _T("ZuoTou"), m_iZuoTou) ;
	CBlkUtility::SetValueReg(_T("DtOption"), _T("GuoGuiPer"), m_dGuoGuiPer) ;
	CBlkUtility::SetValueReg(_T("DtOption"), _T("RFMPer"), m_dRFMPer) ;
	CBlkUtility::SetValueReg(_T("DtOption"), _T("AZPer"), m_dAZPer) ;
	CBlkUtility::SetValueReg(_T("DtOption"), _T("JXPer"), m_dJXPer) ;
	CBlkUtility::SetValueReg(_T("DtOption"), _T("SXJiaPer"), m_dSXJiaPer) ;
	CBlkUtility::SetValueReg(_T("DtOption"), _T("DistJieX"), m_iDistJieX) ;

	CDialog::OnOK();

}


// CDtOption 消息处理程序

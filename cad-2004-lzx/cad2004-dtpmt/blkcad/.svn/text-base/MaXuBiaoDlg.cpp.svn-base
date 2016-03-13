// MaXuBiaoDlg.cpp : implementation file
//

#include "StdAfx.h"
#include "resource.h"
#include "MaXuBiaoDlg.h"
#include "SBDestination.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMaXuBiaoDlg dialog


CMaXuBiaoDlg::CMaXuBiaoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMaXuBiaoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMaXuBiaoDlg)
	m_strStationName = _T("");
	m_dSYT_Height = 0.0;
	m_dFrameHeight = 0.0;
	m_dBetweenTable = 0.0;
	m_strSavePath = _T("");
	m_iVersionFor = 1;
	m_dWidthGuiDao = 0.7;
	m_dWidthBianMa = 0.7;
	m_dHeightGuiDao = 4.0;
	m_dHeightBianMa = 4.0;
	m_strProjName = _T("");
	m_strDrawNum = _T("");
	//}}AFX_DATA_INIT
}


void CMaXuBiaoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMaXuBiaoDlg)
	DDX_Text(pDX, IDC_EDIT_StationName, m_strStationName);
	DDX_Text(pDX, IDC_EDIT_SYT_HEIGHT, m_dSYT_Height);
	DDX_Text(pDX, IDC_EDIT_FRAMEHEIGHT, m_dFrameHeight);
	DDX_Text(pDX, IDC_EDIT_BETWEENTABLE, m_dBetweenTable);
	DDX_Text(pDX, IDC_EDIT_SAVEPATH, m_strSavePath);
	DDX_CBIndex(pDX, IDC_COMBO_MAXUBIAO_TYPE, m_iVersionFor);
	DDX_Text(pDX, IDC_EDIT_MXB_FONTWIDTH_GUIDAO, m_dWidthGuiDao);
	DDV_MinMaxDouble(pDX, m_dWidthGuiDao, 0.1, 1.);
	DDX_Text(pDX, IDC_EDIT_MXB_FONTWIDTH_BIANMA, m_dWidthBianMa);
	DDV_MinMaxDouble(pDX, m_dWidthBianMa, 0.1, 1.);
	DDX_Text(pDX, IDC_EDIT_MXB_FONTHEIGHT_GUIDAO, m_dHeightGuiDao);
	DDV_MinMaxDouble(pDX, m_dHeightGuiDao, 1., 10.);
	DDX_Text(pDX, IDC_EDIT_MXB_FONTHEIGHT_BIANMA, m_dHeightBianMa);
	DDV_MinMaxDouble(pDX, m_dHeightBianMa, 1., 10.);
	DDX_Text(pDX, IDC_EDIT_MXB_PROJNAME, m_strProjName);
	DDX_Text(pDX, IDC_EDIT_MXB_DRAWNUM, m_strDrawNum);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMaXuBiaoDlg, CDialog)
	//{{AFX_MSG_MAP(CMaXuBiaoDlg)
	ON_BN_CLICKED(IDC_BUTTON_SelectDir, OnBUTTONSelectDir)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMaXuBiaoDlg message handlers

BOOL CMaXuBiaoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->SetWindowText(m_strTitle) ;

	CBlkUtility::QueryValueReg(_T("MaXuBiao"), _T("InitDir"), m_strSavePath) ;
	CBlkUtility::QueryValueReg(_T("MaXuBiao"), _T("VersionFor"), m_iVersionFor) ;
	CBlkUtility::QueryValueReg(_T("MaXuBiao"), _T("StationName"), m_strStationName) ;
	CBlkUtility::QueryValueReg(_T("MaXuBiao"), _T("ProjectName"), m_strProjName) ;
	CBlkUtility::QueryValueReg(_T("MaXuBiao"), _T("DwgNum"), m_strDrawNum) ;
	
	this->UpdateData(FALSE) ;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CMaXuBiaoDlg::OnOK() 
{
	// TODO: Add extra validation here
	this->UpdateData() ;

	CBlkUtility::SetValueReg(_T("MaXuBiao"), _T("InitDir"), m_strSavePath) ;
	CBlkUtility::SetValueReg(_T("MaXuBiao"), _T("VersionFor"), m_iVersionFor) ;
	CBlkUtility::SetValueReg(_T("MaXuBiao"), _T("StationName"), m_strStationName) ;
	CBlkUtility::SetValueReg(_T("MaXuBiao"), _T("ProjectName"), m_strProjName) ;
	CBlkUtility::SetValueReg(_T("MaXuBiao"), _T("DwgNum"), m_strDrawNum) ;
	
	CDialog::OnOK();
}

void CMaXuBiaoDlg::OnBUTTONSelectDir() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData() ;
	
	CSBDestination sb( NULL , IDS_BFF_TITLE_MXB ) ;
	sb.SetInitialSelection(m_strSavePath) ;
	sb.SetFlags(BIF_RETURNONLYFSDIRS|BIF_STATUSTEXT) ;
	if(!sb.SelectFolder())
		return ;
	this->m_strSavePath = sb.GetSelectedFolder() ;
	
	
	this->UpdateData(FALSE) ;
	
	
}

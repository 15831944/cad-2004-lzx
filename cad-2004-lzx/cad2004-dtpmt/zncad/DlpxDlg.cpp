// DlpxDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DlpxDlg.h"
#include "SBDestination.h"

// CDlpxDlg 对话框

IMPLEMENT_DYNAMIC(CDlpxDlg, CDialog)
CDlpxDlg::CDlpxDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDlpxDlg::IDD, pParent)
	, m_strSavePath(_T(""))
	,m_strMode(_T(""))
{
}

CDlpxDlg::~CDlpxDlg()
{
}

void CDlpxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DLPX_SAVEPATH, m_strSavePath);
	DDX_CBString(pDX, IDC_COMBO_MODE, m_strMode);
}


BEGIN_MESSAGE_MAP(CDlpxDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_DLPX_SELPATH, OnBnClickedButtonDlpxSelpath)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CDlpxDlg 消息处理程序

void CDlpxDlg::OnBnClickedButtonDlpxSelpath()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData() ;

	CSBDestination sb( NULL , IDS_BFF_TITLE ) ;
	sb.SetInitialSelection(m_strSavePath) ;
	sb.SetFlags(BIF_RETURNONLYFSDIRS|BIF_STATUSTEXT) ;
	if(!sb.SelectFolder())
		return ;
	this->m_strSavePath = sb.GetSelectedFolder() ;	

	this->UpdateData(FALSE) ;	
}

BOOL CDlpxDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CBlkUtility::QueryValueReg(_T("DLPX"),  _T("SavePath"), m_strSavePath) ;
	CBlkUtility::QueryValueReg(_T("DLPX"),  _T("Mode"), m_strMode) ;

	this->UpdateData(FALSE) ;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CDlpxDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData() ;
	//检查存储目录是否存在
	if (CBlkUtility::IsFolderExist(m_strSavePath)==FALSE)
	{
		AfxMessageBox(_T("选定目录不存在,请重新选择！") ) ;
		return ;
	}

	CBlkUtility::SetValueReg(_T("DLPX"),  _T("SavePath"), m_strSavePath) ;
	CBlkUtility::SetValueReg(_T("DLPX"),  _T("Mode"), m_strMode) ;


	CButton *pButton = (CButton *)this->GetDlgItem(IDOK) ;
	pButton->EnableWindow(FALSE) ;
	CButton *pButton2 = (CButton *)this->GetDlgItem(IDCANCEL) ;
	pButton2->EnableWindow(FALSE) ;

	OnOK();
}

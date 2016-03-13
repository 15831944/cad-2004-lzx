// DtAddBoxInfo.cpp : 实现文件
//

#include "stdafx.h"
//#include "zncad.h"
#include "DtAddBoxInfo.h"

const  CString CDtAddBoxInfo::strboxName[10] = {_T("XB1"), _T("XB2"), _T("KAD"), _T("TCB"), _T("FXX"), _T("TWC"), _T("4"), _T("7"),_T("12"),_T("24")} ;
// CDtAddBoxInfo 对话框

IMPLEMENT_DYNAMIC(CDtAddBoxInfo, CDialog)
CDtAddBoxInfo::CDtAddBoxInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDtAddBoxInfo::IDD, pParent)
	,m_iSelShape(0)
	,m_iSelName(0)
	, m_strboxName(_T(""))
{
	m_strboxName=this->strboxName[m_iSelName];

}

CDtAddBoxInfo::~CDtAddBoxInfo()
{
}

void CDtAddBoxInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_iSelShape);
	DDX_Radio(pDX, IDC_RADIO3, m_iSelName);
	DDX_Text(pDX, IDC_EDIT1, m_strboxName);
}


BEGIN_MESSAGE_MAP(CDtAddBoxInfo, CDialog)
//	ON_NOTIFY(BCN_HOTITEMCHANGE, IDC_RADIO3, OnBnHotItemChangeRadio3)
	ON_COMMAND_RANGE(IDC_RADIO3, IDC_RADIO12, OnBnClickedRadio3)
	ON_COMMAND_RANGE(IDC_RADIO1,IDC_RADIO2,OnBnClickedRadio1)
/*	ON_BN_SETFOCUS(IDC_RADIO3, OnBnSetfocusRadio3)*/
//	ON_EN_CHANGE(IDC_EDIT1, OnEnChangeEdit1)
END_MESSAGE_MAP()


// CDtAddBoxInfo 消息处理程序

BOOL CDtAddBoxInfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CDtAddBoxInfo::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	GetDlgItemText(IDC_EDIT1,m_strboxName);
	  this->UpdateData(FALSE);
	CDialog::OnOK();
}

void CDtAddBoxInfo::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	CDialog::OnCancel();
}

LRESULT CDtAddBoxInfo::OnTextChanged( WPARAM wparam,LPARAM lparam )
{
	HWND hWnd = (HWND)wparam ;
	HWND *phWnd =NULL;
	GetDlgItem(IDC_RADIO3,phWnd) ;
	if(hWnd==*phWnd)
	{
		m_strboxName=this->strboxName[m_iSelName] ;
		this->UpdateData(TRUE);
	}
   return 0;

}

// void CDtAddBoxInfo::OnBnHotItemChangeRadio3(NMHDR *pNMHDR, LRESULT *pResult)
// {
// 	// 此功能要求 Internet Explorer 6 或更高版本。
// 	// 符号 _WIN32_IE 必须是 >= 0x0600。
// 	LPNMBCHOTITEM pHotItem = reinterpret_cast<LPNMBCHOTITEM>(pNMHDR);
// 	m_strboxName =this->strboxName[m_iSelName];
// 	// TODO: 在此添加控件通知处理程序代码
// 	*pResult = 0;
// }

void CDtAddBoxInfo::OnBnClickedRadio3(UINT id)
{
	// TODO: 在此添加控件通知处理程序代码
	//m_strboxName=this->strboxName[m_iSelName];
	//UpdateData(TRUE);
// 	CString strMsg ;
// 	strMsg.Format(_T("%d"), id) ;
// 	AfxMessageBox(strMsg) ;
	m_iSelName=(id-IDC_RADIO3);
	m_strboxName =this->strboxName[m_iSelName];
	if(id>=IDC_RADIO9)
	{
		m_iSelShape=1;
	}
	else
	{
		m_iSelShape=0;
	}
	UpdateData(FALSE);
}

// void CDtAddBoxInfo::OnBnSetfocusRadio3()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 	m_strboxName=this->strboxName[m_iSelName];
// 	UpdateData(TRUE);
// }

void CDtAddBoxInfo::OnBnClickedRadio1( UINT id )
{
// 	CString strMsg ;
// 	strMsg.Format(_T("%d"), id) ;
// 	AfxMessageBox(strMsg) ;
	if(id==IDC_RADIO1)
	{
		m_iSelName=0;
		m_iSelShape=0;
	}
	else
	{
		m_iSelName=6;
		m_iSelShape=1;
	}
	UpdateData(FALSE);
}

// void CDtAddBoxInfo::OnEnChangeEdit1()
// {
// 	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
// 	// 发送该通知，除非重写 CDialog::OnInitDialog()
// 	// 函数并调用 CRichEditCtrl().SetEventMask()，
// 	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
//     UpdateData(FALSE);
// 	CString strMsg ;
// 	strMsg.Format(_T("%s"), m_strboxName) ;
// 	AfxMessageBox(strMsg) ;
// 	
// 	// TODO:  在此添加控件通知处理程序代码
// }

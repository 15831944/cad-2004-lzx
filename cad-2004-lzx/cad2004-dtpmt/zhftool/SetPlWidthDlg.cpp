// SetPlWidthDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SetPlWidthDlg.h"

// CSetPlWidthDlg �Ի���

IMPLEMENT_DYNAMIC(CSetPlWidthDlg, CAcUiDialog)
CSetPlWidthDlg::CSetPlWidthDlg (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) 
	 : CAcUiDialog (CSetPlWidthDlg::IDD, pParent, hInstance) 
	 , m_dFrom(0)
	 , m_dTo(0)
	 , m_bIncludeBlk(TRUE)
	 ,m_iComoBoxIndex(0)
 {
 }

CSetPlWidthDlg::~CSetPlWidthDlg()
{
	this->Clear() ;
}

void CSetPlWidthDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SETPLWIDTH_FROM, m_dFrom);
	DDV_MinMaxDouble(pDX, m_dFrom, 0, 10);
	DDX_Text(pDX, IDC_EDIT_SETPLWIDTH_TO, m_dTo);
	DDV_MinMaxDouble(pDX, m_dTo, 0, 1);
	DDX_Check(pDX, IDC_CHECK_SETPLWIDTH_INCLUDEBLK, m_bIncludeBlk);
	DDX_Control(pDX, IDC_BUTTON_SETPLWIDTH_SEL, m_btnSel);
	DDX_CBIndex(pDX, IDC_COMBO_SETPLWIDTH_RANGE, m_iComoBoxIndex);
}


BEGIN_MESSAGE_MAP(CSetPlWidthDlg, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_BN_CLICKED(IDC_BUTTON_SETPLWIDTH_SEL, OnBnClickedSel)
END_MESSAGE_MAP()


//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CSetPlWidthDlg::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}

// CSetPlWidthDlg ��Ϣ�������

void CSetPlWidthDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	this->UpdateData() ;

	CDialog::OnOK();
}


void CSetPlWidthDlg::OnBnClickedSel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
    BeginEditorCommand(); 
	this->Clear() ;
	if (acedSSGet(NULL, NULL, NULL, NULL, m_ss) == RTNORM)
	{
		CUserSystem mysys ;
		long nLen = 0 ;
		acedSSLength(m_ss, &nLen) ;
		if (nLen>0)
		{
			mysys.Highlight(m_ss) ;
		}
	}
// 	else 
// 	{ 
// 		// ����ȡ����������Ի���
// 		CancelEditorCommand(); 
// 	} 
	CompleteEditorCommand(); 

	this->UpdateComoBox();
}

BOOL CSetPlWidthDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_btnSel.AutoLoad() ;

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}

void CSetPlWidthDlg::Clear(void)
{
	if (!((m_ss[0] == 0L) && (m_ss[1] == 0L)))
	{
		CUserSystem mysys ;
		mysys.UnHighlight(m_ss) ;
		acedSSFree(m_ss);
		m_ss[0] = m_ss[1] = 0L;
	}
}

void CSetPlWidthDlg::UpdateComoBox(void)
{
	if (!((m_ss[0] == 0L) && (m_ss[1] == 0L)))
	{
		m_iComoBoxIndex = 1 ;
	}
	else
	{
		m_iComoBoxIndex = 0 ;
	}
	this->UpdateData(FALSE) ;
}

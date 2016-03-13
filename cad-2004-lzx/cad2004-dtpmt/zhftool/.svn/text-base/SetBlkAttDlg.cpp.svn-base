// SetBlkAttDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SetBlkAttDlg.h"


// CSetBlkAttDlg 对话框

IMPLEMENT_DYNAMIC(CSetBlkAttDlg, CAcUiDialog)
CSetBlkAttDlg::CSetBlkAttDlg (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) 
	: CAcUiDialog (CSetBlkAttDlg::IDD, pParent, hInstance) 
	, m_strBlkName(_T(""))
	, m_iRange(0)
	, m_iColorIndex(7)
{
}

CSetBlkAttDlg::~CSetBlkAttDlg()
{
	this->Clear() ;
}

void CSetBlkAttDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SETBLKATT_BLKNAME, m_comboxBlkName);
	DDX_Control(pDX, IDC_BUTTON_SETBLKATT_SEL, m_btnSel);

	DDX_Radio(pDX, IDC_RADIO_SETBLKATT_ALL, m_iRange);
	DDX_Radio(pDX, IDC_RADIO_SETBLKATT_RED, m_iColorIndex);
}


BEGIN_MESSAGE_MAP(CSetBlkAttDlg, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_CBN_SELCHANGE(IDC_COMBO_SETBLKATT_BLKNAME, OnCbnSelchangeComboSetblkattBlkname)
	ON_BN_CLICKED(IDC_BUTTON_SETBLKATT_SEL, OnBnClickedButtonSetblkattSel)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_SETBLKATT_ALL, IDC_RADIO_SETBLKATT_SELECT,OnBnClickedRadioSetblkattAll)
END_MESSAGE_MAP()


// CSetBlkAttDlg 消息处理程序

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CSetBlkAttDlg::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}

BOOL CSetBlkAttDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_btnSel.AutoLoad() ;

 	CBlkUtility::QueryValueReg(_T("SetBlkAtt"), _T("Range"), m_iRange) ;
	CBlkUtility::QueryValueReg(_T("SetBlkAtt"), _T("ColorIndex"), m_iColorIndex) ;
	this->UpdateData(FALSE) ;

	CUserSystem mysys ;
	CStringArray strArrayBlkName ;
	mysys.GetAllBlockTable(strArrayBlkName, 1) ;
	for (int i=0; i<strArrayBlkName.GetCount(); i++)
	{
		CString strBlkName = strArrayBlkName.GetAt(i) ;
		m_comboxBlkName.AddString(strBlkName) ;
	}

	CString strHeads[2] = {_T("项目"), _T("值")} ;
	for (int i=0; i<2 ;i++)
	{
		m_grid.m_vecStringHead.push_back(strHeads[i]) ;
	}
  this->GetSelected() ;
// 	this->RefreshGrid() ;

	CRect rectClient ;
	GetClientRect(rectClient) ;	

	CButton* pButton = (CButton*)GetDlgItem(IDC_BUTTON_SETBLKATT_SEL) ;
	CRect rectButton ;
	pButton->GetWindowRect(&rectButton) ;
	ScreenToClient(&rectButton) ;

	rectClient.left = rectButton.right+15 ;
	rectClient.bottom = rectClient.bottom-20 ;
	rectClient.right = rectClient.right-5 ;
	m_grid.CreateGrid( WS_CHILD|WS_VISIBLE, rectClient, this, IDC_LIST_GRID );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CSetBlkAttDlg::Clear(void)
{
	if (!((m_ss[0] == 0L) && (m_ss[1] == 0L)))
	{
		CUserSystem mysys ;
		mysys.UnHighlight(m_ss) ;
		acedSSFree(m_ss);
		m_ss[0] = m_ss[1] = 0L;
	}
}

void CSetBlkAttDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	this->UpdateData(TRUE) ;
	m_comboxBlkName.GetLBText(m_comboxBlkName.GetCurSel(), m_strBlkName) ;
	if (m_strBlkName==_T(""))
	{
		AfxMessageBox(_T("未指定任何要设置属性的块！")) ;
		return ;
	}
	CBlkUtility::SetValueReg(_T("SetBlkAtt"), _T("Range"), m_iRange) ;
	CBlkUtility::SetValueReg(_T("SetBlkAtt"), _T("ColorIndex"), m_iColorIndex) ;

	CDialog::OnOK();
}

void CSetBlkAttDlg::OnCbnSelchangeComboSetblkattBlkname()
{
	// TODO: 在此添加控件通知处理程序代码
	this->RefreshGrid() ;
	//m_comboxBlkName.GetLBText(m_comboxBlkName.GetCurSel(), m_strBlkName) ;
	//acutPrintf(_T("\nyou selected %s"), m_strBlkName) ;
}

void CSetBlkAttDlg::OnBnClickedButtonSetblkattSel()
{
	// TODO: 在此添加控件通知处理程序代码
	CUserSystem mysys ;

	BeginEditorCommand(); 
	this->Clear() ;
	struct resbuf *pRb ;
	pRb = acutBuildList(RTDXF0, _T("INSERT"), 0) ;
	ads_name ss_temp ;
	if (acedSSGet(NULL, NULL, NULL, pRb, ss_temp) == RTNORM)
	{
		long nLen = 0 ;
		acedSSLength(ss_temp, &nLen) ;
		if (nLen>0)
		{
			ads_name temp_name ;
			acedSSName(ss_temp, 0, temp_name) ;
			mysys.GetBlockName(temp_name, m_strBlkName) ;
		}
		acedSSFree(ss_temp) ;
	}
	acutRelRb(pRb) ;
	if (!m_strBlkName.IsEmpty())
	{
		pRb = acutBuildList(RTDXF0, _T("INSERT"), 2, m_strBlkName, 0) ;
		if (acedSSGet(_T("P"), NULL, NULL, pRb, m_ss) == RTNORM)
		{
			this->UpdateComboBox() ;
		}
		acutRelRb(pRb) ;
	}

	// 	else 
	// 	{ 
	// 		// 否则取消命令（包括对话框）
	// 		CancelEditorCommand(); 
	// 	} 
	CompleteEditorCommand(); 
	mysys.Highlight(m_ss) ;
}

void CSetBlkAttDlg::OnBnClickedRadioSetblkattAll(UINT id)
{
	// TODO: 在此添加控件通知处理程序代码
// 	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_SETBLKATT_BLKNAME) ;
// 	if (id==IDC_RADIO_SETBLKATT_ALL)
// 	{
// 		pComboBox->EnableWindow(TRUE) ;
// 	}
// 	else if (id==IDC_RADIO_SETBLKATT_SELECT)
// 	{
// 		pComboBox->EnableWindow(FALSE) ;
// 	}
}

void CSetBlkAttDlg::RefreshGrid(void)
{
	m_grid.m_vecStringCell.clear() ;

	long nLength = 0 ;
	acedSSLength(m_ss, &nLength) ;
	if (nLength>0)
	{
		CUserSystem mysys ;

		ads_name temp_name ;
		ads_ssname(m_ss, 0, temp_name) ;
		AcDbEntity * pEnt = NULL ;
		if (mysys.OpenAcDbEntity(temp_name, pEnt))
		{
			if (pEnt->isKindOf(AcDbBlockReference::desc()))
			{
				AcDbBlockReference * pRef = (AcDbBlockReference *)pEnt ;
				mysys.GetAttValue(pRef, m_grid.m_vecStringCell) ;									
			}
			pEnt->close() ;				
		}
	}

	m_grid.Refresh() ;
}
int CSetBlkAttDlg::UpdateComboBox()
{
// 	long nLen = 0 ;
// 	acedSSLength(sum_name, &nLen) ;
// 	if (nLen>0)
// 	{
// 		CUserSystem mysys ;
// 		ads_name temp_name ;
// 		ads_ssname(sum_name, 0, temp_name) ;
// 		mysys.GetBlockName(temp_name, m_strBlkName) ;							
// 	}

	if (m_comboxBlkName.FindString(0, m_strBlkName)!=CB_ERR)
	{
		m_comboxBlkName.SelectString(0, m_strBlkName) ;
	}	
	this->RefreshGrid() ;

// 	this->OnCbnSelchangeComboSetblkattBlkname() ;
// 	m_comboxBlkName.SendMessage(CBN_SELCHANGE) ; //不起作用，回头再查
	
	return 0;
}

int CSetBlkAttDlg::GetSelected(void)
{
	m_grid.m_vecStringCell.clear() ;

	CUserSystem mysys ;

	struct resbuf *pRb ;
	pRb = acutBuildList(RTDXF0, _T("INSERT"), 0) ;
	ads_name ss_temp ;
	if (acedSSGet(_T("_I"), NULL, NULL, NULL, ss_temp) == RTNORM)
	{
		long nLen = 0 ;
		acedSSLength(ss_temp, &nLen) ;
		for (int i=0; i<nLen; i++)
		{
			ads_name temp_name ;
			acedSSName(ss_temp, i, temp_name) ;
			
			AcDbEntity * pEnt = NULL ;
			if (mysys.OpenAcDbEntity(temp_name, pEnt))
			{
				if (pEnt->isKindOf(AcDbBlockReference::desc()))
				{
					AcDbBlockReference * pRef = (AcDbBlockReference *)pEnt ;
					mysys.GetBlockName(pRef, m_strBlkName) ;
					mysys.GetAttValue(pRef, m_grid.m_vecStringCell) ;
					pEnt->close() ;
					break;
				}
				pEnt->close() ;				
			}
		}
		mysys.UnHighlight(ss_temp) ;
		acedSSFree(ss_temp) ;
	}
	acutRelRb(pRb) ;
	if (!m_strBlkName.IsEmpty())
	{
		pRb = acutBuildList(RTDXF0, _T("INSERT"), 2, m_strBlkName, 0) ;
		if (acedSSGet(_T("P"), NULL, NULL, pRb, m_ss) == RTNORM)
		{
			//this->UpdateComboBox() ;
			if (m_comboxBlkName.FindString(0, m_strBlkName)!=CB_ERR)
			{
				m_comboxBlkName.SelectString(0, m_strBlkName) ;
			}	
		}

		mysys.Highlight(m_ss) ;

		acutRelRb(pRb) ;
	}
	return 0;
}

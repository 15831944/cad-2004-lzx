// ShowBlkAttDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ShowBlkAttDlg.h"

// CShowBlkAttDlg 对话框

IMPLEMENT_DYNAMIC(CShowBlkAttDlg, CAcUiDialog)
CShowBlkAttDlg::CShowBlkAttDlg(CWnd* pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/)
	: CAcUiDialog(CShowBlkAttDlg::IDD, pParent, hInstance)
	, m_strBlkName(_T(""))
	, m_strAtt(_T(""))
	, m_iShowOrHide(0)
	, m_iRange(0)
{
}

CShowBlkAttDlg::~CShowBlkAttDlg()
{
	this->Clear() ;
}

void CShowBlkAttDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	// 	DDX_Text(pDX, IDC_EDIT_SHOWBLKATT_BLKNAME, m_strBlkName);
	// 	DDX_Text(pDX, IDC_EDIT_SHOWBLKATT_ATTS, m_strAtts);
	DDX_Radio(pDX, IDC_RADIO_SHOWBLKATT_SHOW, m_iShowOrHide);
	DDX_Control(pDX, IDC_COMBO_SHOWBLKATT_BLKNAME, m_comboBoxBlkName);
	DDX_Control(pDX, IDC_COMBO_SHOWBLKATT_ATTS, m_comboBoxAtt);
	DDX_Control(pDX, IDC_BUTTON_SHOWBLKATT_SEL, m_btnSel);
	DDX_Check(pDX, IDC_CHECK_SHOWBLKATT_ALL, m_iRange);
}


BEGIN_MESSAGE_MAP(CShowBlkAttDlg, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_SHOWBLKATT_SEL, OnBnClickedButtonShowblkattSel)
	ON_CBN_SELCHANGE(IDC_COMBO_SHOWBLKATT_BLKNAME, OnCbnSelchangeComboShowblkattBlkname)
END_MESSAGE_MAP()


// CShowBlkAttDlg 消息处理程序

LRESULT CShowBlkAttDlg::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}

void CShowBlkAttDlg::Clear(void)
{
	if (!((m_ss[0] == 0L) && (m_ss[1] == 0L)))
	{
		CUserSystem mysys ;
		mysys.UnHighlight(m_ss) ;
		acedSSFree(m_ss);
		m_ss[0] = m_ss[1] = 0L;
	}
}

void CShowBlkAttDlg::OnBnClickedButtonShowblkattSel()
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
			this->UpdateComoBlkName() ;
// 			if (m_comboBoxBlkName.FindString(0, m_strBlkName)!=CB_ERR)
// 			{
// 				m_comboBoxBlkName.SelectString(0, m_strBlkName) ;
// 			}	
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
	//this->OnCbnSelchangeComboShowblkattBlkname() ;
}

void CShowBlkAttDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData() ;
	m_comboBoxBlkName.GetLBText(m_comboBoxBlkName.GetCurSel(), m_strBlkName) ;
	m_comboBoxAtt.GetLBText(m_comboBoxAtt.GetCurSel(), m_strAtt) ;
	m_strBlkName.Trim() ;
	m_strAtt.Trim() ;

	if (m_strBlkName.IsEmpty()||m_strAtt.IsEmpty())
	{
		AfxMessageBox(_T("块名或者属性为空，请输入或者取消本次操作！")) ;
		return ;
	}

	CBlkUtility::SetValueReg(_T("SHOWBLKATT"), _T("BlockName"), m_strBlkName) ;
	CBlkUtility::SetValueReg(_T("SHOWBLKATT"), _T("BlockAtts"), m_strAtt) ;
	CBlkUtility::SetValueReg(_T("SHOWBLKATT"), _T("ShowOrHide"), m_iShowOrHide) ;
	CBlkUtility::SetValueReg(_T("SHOWBLKATT"), _T("Range"), m_iRange) ;

	OnOK();
}

BOOL CShowBlkAttDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_btnSel.AutoLoad() ;
	CBlkUtility::QueryValueReg(_T("SHOWBLKATT"), _T("BlockName"), m_strBlkName) ;
	CBlkUtility::QueryValueReg(_T("SHOWBLKATT"), _T("BlockAtts"), m_strAtt) ;
	CBlkUtility::QueryValueReg(_T("SHOWBLKATT"), _T("ShowOrHide"), m_iShowOrHide) ;
	CBlkUtility::QueryValueReg(_T("SHOWBLKATT"), _T("Range"), m_iRange) ;

	CUserSystem mysys ;
	CStringArray strArrayBlkName ;
	mysys.GetAllBlockTable(strArrayBlkName, 1) ;
	for (int i=0; i<strArrayBlkName.GetCount(); i++)
	{
		CString strBlkName = strArrayBlkName.GetAt(i) ;
		m_comboBoxBlkName.AddString(strBlkName) ;
		if (i==0)
		{
			m_comboBoxBlkName.SetCurSel(i) ;
		}
		if (m_strBlkName==strBlkName)
		{
			m_comboBoxBlkName.SetCurSel(i) ;
			CLongArray nArrayHandle ;
			mysys.FindBlk(strBlkName, nArrayHandle) ;
			vector<vector<CString> > vecAtt ;
			mysys.GetAttValue(nArrayHandle.GetAt(0), vecAtt) ;
			for (int j=0; j<vecAtt.size(); j++)
			{
				CString strTag = vecAtt[j][0] ;
				m_comboBoxAtt.AddString(strTag) ;
				if (j==0)
				{
					m_comboBoxAtt.SetCurSel(j) ;
				}
				if (m_strAtt==strTag)
				{
					m_comboBoxAtt.SetCurSel(j) ;
				}
			}
		}

	}

	this->UpdateData(FALSE) ;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CShowBlkAttDlg::OnCbnSelchangeComboShowblkattBlkname()
{
	// TODO: 在此添加控件通知处理程序代码
	CUserSystem mysys ;
	m_comboBoxBlkName.GetLBText(m_comboBoxBlkName.GetCurSel(), m_strBlkName) ;

	this->UpdateComboAtt() ;
}

int CShowBlkAttDlg::UpdateComoBlkName(void)
{
	CUserSystem mysys ;
	long nLen = 0 ;
	acedSSLength(m_ss, &nLen) ;
	if (nLen>0)
	{
		ads_name temp_name ;
		acedSSName(m_ss, 0, temp_name) ;
		mysys.GetBlockName(temp_name, m_strBlkName) ;
		int iLoc = m_comboBoxBlkName.FindString(0, m_strBlkName) ;
		if (iLoc!=CB_ERR)
		{
			m_comboBoxBlkName.SelectString(0, m_strBlkName) ;
		}
	}

	this->UpdateComboAtt() ;

	return 0;
}

int CShowBlkAttDlg::UpdateComboAtt(void)
{
	CUserSystem mysys ;

	CLongArray nArrayHandle ;
	mysys.FindBlk(m_strBlkName, nArrayHandle) ;

	vector<vector<CString> > vecAtt ;
	mysys.GetAttValue(nArrayHandle.GetAt(0), vecAtt) ;
	m_comboBoxAtt.ResetContent() ;
	for (int j=0; j<vecAtt.size(); j++)
	{
		CString strTag = vecAtt[j][0] ;
		m_comboBoxAtt.AddString(strTag) ;
		if (j==0)
		{
			m_comboBoxAtt.SetCurSel(j) ;
		}
		if (m_strAtt==strTag)
		{
			m_comboBoxAtt.SetCurSel(j) ;
		}
	}

	return 0;
}

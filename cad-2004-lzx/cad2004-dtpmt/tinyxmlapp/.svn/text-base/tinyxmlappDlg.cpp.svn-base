// tinyxmlappDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "tinyxmlapp.h"
#include "tinyxmlappDlg.h"

#include "../tinyxml/tinyxml.h"
#include "../blkdll/BlkUtitily.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CtinyxmlappDlg 对话框



CtinyxmlappDlg::CtinyxmlappDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CtinyxmlappDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CtinyxmlappDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CtinyxmlappDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
END_MESSAGE_MAP()


// CtinyxmlappDlg 消息处理程序

BOOL CtinyxmlappDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将\“关于...\”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO：在此添加额外的初始化代码
	
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CtinyxmlappDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CtinyxmlappDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CtinyxmlappDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CtinyxmlappDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (CreateXml()==true)
	{
		AfxMessageBox(_T("create ok")) ;
	}
	else 
	{
		AfxMessageBox(_T("create fail")) ;

	}

}

bool CtinyxmlappDlg::CreateXml(void)
{
	CString strXhPaths[] = {_T("xhj\\接车"), _T("xhj\\发车"), _T("xhj\\发车(带表示器)"),  _T("xhj\\调车"), _T("xhj\\预告"), _T("xhj\\通过"), _T("xhj\\复示"), _T("xhj\\地铁"), _T("dc"), _T("jyj"), _T("box"), _T("dltb"), _T("jdq"), _T("sign"), _T("bg"), _T("dz"), _T("conti"), _T("pmt")} ;
	CString strXhPaletteNames[] = {_T("接车"),  _T("发车"), _T("发车(带表示器)"), _T("调车"), _T("预告"), _T("通过"), _T("复示"), _T("地铁"), _T("道岔"), _T("绝缘节"), _T("箱盒"), _T("电路"),  _T("继电器"), _T("签字栏"), _T("表格"), _T("端子"), _T("连接符"), _T("其他")} ;

	CString strDtPaths[] = {_T("xhj\\地铁"),  _T("dt\\道岔转辙机"), _T("dt\\占用检测"), _T("dt\\校准及通信"), _T("dt\\车站设备"), _T("dt\\箱盒"), _T("dt\\车载设备"), _T("dt\\网络设备"), _T("dt\\其他")} ;
	CString strDtPaletteNames[] = {_T("信号机"),  _T("道岔转辙机"), _T("占用检测"),  _T("校准及通信"),  _T("车站设备"),  _T("箱盒"),  _T("车载设备"),  _T("网络设备"),  _T("其他")} ;

	CStringA strSetNamesA[] = {"大铁图库", "地铁图库"} ;

	TiXmlDocument doc ;
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "gb2312", "") ;
	doc.LinkEndChild(decl) ;

	//节点：根目录
	TiXmlElement* root = new TiXmlElement("ZhfCAD") ;
	doc.LinkEndChild(root) ;
	TiXmlComment* comment = new TiXmlComment("Settings for ZhfCAD") ;
	root->LinkEndChild(comment) ;

	for (int n=0; n<2; n++) 
	{
		//节点：信号图库，地铁图库
		TiXmlElement* paletteset = new TiXmlElement("PaletteSet") ; ;
		root->LinkEndChild(paletteset) ;
		paletteset->SetAttribute("Name", strSetNamesA[n] ) ; //显示名称
// 		CStringA strSetForComment ;
// 		strSetForComment.Format("Settings for %s", strSetNamesA[n]) ;
// 		comment = new TiXmlComment(strSetForComment) ;
// 		paletteset->LinkEndChild(comment) ;

		int iNumPalette = (n==0?18:9) ;
		for (int i=0; i<iNumPalette; i++)
		{
			//节点：各palette（一般对应一个目录，也有例外）
			CStringA strPaletteNameTagA("Palette") ;
			TiXmlElement* palette = new TiXmlElement(strPaletteNameTagA) ;
			paletteset->LinkEndChild(palette) ;
			CString strPath = (n==0?strXhPaths[i]:strDtPaths[i]) ;
			CString strPaletteName = (n==0?strXhPaletteNames[i]:strDtPaletteNames[i]) ;
			CStringA strPathA, strPaletteNameA ;
#ifdef UNICODE
			CBlkUtility::ConvWideToMulti(strPath, strPathA) ;
			CBlkUtility::ConvWideToMulti(strPaletteName, strPaletteNameA) ;
#else
			strPathA = strPath ;
			strPaletteNameA = strPaletteName ;
#endif
			palette->SetAttribute("Name", strPaletteNameA ) ; //显示名称
			palette->SetAttribute("DefaultPath", strPathA) ; //对应默认目录
			palette->SetAttribute("DefaultShowAttribute", 0) ; //该palette下图块默认显示属性与否

			//各palette下对应文件
			CString strFullPath ;
			strFullPath.Format(_T("C:\\block_zhf\\support\\%s"), strPath) ;
			CStringArray strArrayFilePath ;
			CBlkUtility::FindFile(strFullPath, _T("dwg"), strArrayFilePath, 1) ;
			int iNumFile = strArrayFilePath.GetCount() ;
			for (int j=0; j<iNumFile; j++)
			{
				//节点：文件
				CStringA strFileTagA("File") ;
				//strFileTagA.Format("File%d", j) ;
				TiXmlElement* filetag = new TiXmlElement(strFileTagA) ;
				palette->LinkEndChild(filetag) ;

				CString strFilePath, strFileName ;
				strFilePath = strArrayFilePath.GetAt(j) ;
				CBlkUtility::GetFileName(strFilePath, strFileName) ;
				CStringA strFilePathA, strFileNameA ;
#ifdef UNICODE
				CBlkUtility::ConvWideToMulti(strFilePath, strFilePathA) ;
				CBlkUtility::ConvWideToMulti(strFileName, strFileNameA) ;
#else
				strFilePathA = strFilePath ;
				strFileNameA = strFileName ;
#endif
				filetag->SetAttribute("FullPath", strFilePathA) ; //文件全路径
				filetag->SetAttribute("ShowName", strFileNameA) ;//图块显示名称，默认用文件名（不带扩展），需要改为中文名
				filetag->SetAttribute("ShowAttribute", 0) ; //该图块显示属性与否
			}
		}
	}

	return doc.SaveFile("zhfcad.xml") ;


}

void CtinyxmlappDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strMsg ;

	TiXmlDocument doc1( "D:\\cadapp\\zhfcad2004\\debug\\appsettings.xml" );
	bool b1 = doc1.LoadFile();
	strMsg.Format(_T("%d"), b1) ;
	AfxMessageBox(strMsg) ;

	TiXmlDocument doc("D:\\cadapp\\zhfcad2004\\debug\\zhfcad.xml") ;
	bool bLoadOk = doc.LoadFile() ;
	strMsg.Format(_T("%d"), bLoadOk) ;
	AfxMessageBox(strMsg) ;
	if (bLoadOk)
	{
		TiXmlHandle docHandle(&doc) ;
		TiXmlElement* paletteset = docHandle.FirstChild("ZhfCAD").FirstChild("PaletteSet").ToElement() ;
		strMsg.Empty() ;
		for (; paletteset!=NULL; paletteset=paletteset->NextSiblingElement())
		{
			strMsg.AppendFormat(_T("\n%s"), paletteset->Attribute("Name")) ;
			TiXmlElement* palette = paletteset->FirstChildElement() ;
			for (;palette!=NULL;palette=palette->NextSiblingElement())
			{
				CString strAttA = palette->Attribute("Name") ;
				strMsg.AppendFormat(_T("\n%s"), strAttA) ;
			}
		}

		AfxMessageBox(strMsg) ;



	}
}

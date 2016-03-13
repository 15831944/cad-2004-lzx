// DtMainCableInfo.cpp : 实现文件
//

#include "stdafx.h"
//#include "zncad.h"
#include "DtMainCableInfo.h"


const  CString CDtMainCableInfo::strDeviceTypeItem[8] = {_T("信号机"), _T("计轴"), _T("应答器"), _T("AP"), _T("转辙机"), _T("信号设备室"), _T("EMP"), _T("其他")} ;
// CDtMainCableInfo 对话框

IMPLEMENT_DYNAMIC(CDtMainCableInfo, CDialog)
CDtMainCableInfo::CDtMainCableInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDtMainCableInfo::IDD, pParent)
	, m_iSelDevice(0)
	, m_iSelColor(0)
	,m_pEnt(NULL)
{
}

CDtMainCableInfo::CDtMainCableInfo( AcDbEntity *pEnt )
{
	//CDtMainCableInfo(CWnd* pParent = NULL);   // 标准构造函数
	if(pEnt!=NULL)
	{
		CUserSystem mysys ;
		//CString  strDev[]={};
		//CString strDev=CDtMainCableInfo::strDeviceTypeItem[this->m_iSelDevice];
		this->m_pEnt=pEnt;
		CString strDev;
		mysys.GetXdata(m_pEnt,_T("DEVTYPE"),strDev);
		int index=0;
		for(int i=0;i<8;i++)
		{
			CString strTemp=CDtMainCableInfo::strDeviceTypeItem[i];
			if(strDev==strTemp)
			{
				this->m_iSelDevice=i;
				break;
			}
		}
		//AcCmColor color ;
		//m_pEnt->upgradeOpen();
		//m_pEnt->setColorIndex(this->m_iSelColor+1);
		this->m_iSelColor=m_pEnt->colorIndex()-1;
		//m_pEnt->downgradeOpen();

	}

}

CDtMainCableInfo::~CDtMainCableInfo()
{
}

void CDtMainCableInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_DEVICE_RADIO1, m_iSelDevice);
	DDX_Radio(pDX, IDC_COLOR_RADIO9, m_iSelColor);
}


BEGIN_MESSAGE_MAP(CDtMainCableInfo, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
END_MESSAGE_MAP()


// CDtMainCableInfo 消息处理程序

void CDtMainCableInfo::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData() ;
	CUserSystem mysys ;
	//CString  strDev[]={};
	CString strDev=CDtMainCableInfo::strDeviceTypeItem[this->m_iSelDevice];
	mysys.SetXdata(m_pEnt, _T("DEVTYPE"), strDev) ; //进楼50米
	//m_pEnt->setColor(this->m_iSelColor);
	//mysys.SetXdata(m_pEnt, _T("CROSSGD_CABLE"), m_iCrossGd) ; 
	//double dSum =  m_iCableLenIntoXhl+m_iCrossGd*m_iPerGd+(m_iCrossGd-1)*m_iPerBwtweenGd ;
	AcCmColor color ;
// 	color.setColorIndex(this->m_iSelColor+1) ;
// 	m_pEnt->setColor(color) ;
	m_pEnt->upgradeOpen();
	m_pEnt->setColorIndex(this->m_iSelColor+1);
	m_pEnt->downgradeOpen();
	CDialog::OnOK() ;
}

void CDtMainCableInfo::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
		CDialog::OnCancel();
}

BOOL CDtMainCableInfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CUserSystem mysys ;
	//CString  strDev[]={};
	//CString strDev=CDtMainCableInfo::strDeviceTypeItem[this->m_iSelDevice];
	//this->m_pEnt=pEnt;
	CString strDev;
	mysys.GetXdata(m_pEnt,_T("DEVTYPE"),strDev);
	int index=0;
	for(int i=0;i<8;i++)
	{
		CString strTemp=CDtMainCableInfo::strDeviceTypeItem[i];
		if(strDev==strTemp)
		{
			this->m_iSelDevice=i;
			break;
		}
	}
	//AcCmColor color ;
	//m_pEnt->upgradeOpen();
	//m_pEnt->setColorIndex(this->m_iSelColor+1);
	this->m_iSelColor=m_pEnt->colorIndex()-1;

	this->UpdateData(FALSE) ;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE

}

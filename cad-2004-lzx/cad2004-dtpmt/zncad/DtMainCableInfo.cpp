// DtMainCableInfo.cpp : ʵ���ļ�
//

#include "stdafx.h"
//#include "zncad.h"
#include "DtMainCableInfo.h"


const  CString CDtMainCableInfo::strDeviceTypeItem[8] = {_T("�źŻ�"), _T("����"), _T("Ӧ����"), _T("AP"), _T("ת�޻�"), _T("�ź��豸��"), _T("EMP"), _T("����")} ;
// CDtMainCableInfo �Ի���

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
	//CDtMainCableInfo(CWnd* pParent = NULL);   // ��׼���캯��
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


// CDtMainCableInfo ��Ϣ�������

void CDtMainCableInfo::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData() ;
	CUserSystem mysys ;
	//CString  strDev[]={};
	CString strDev=CDtMainCableInfo::strDeviceTypeItem[this->m_iSelDevice];
	mysys.SetXdata(m_pEnt, _T("DEVTYPE"), strDev) ; //��¥50��
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
		CDialog::OnCancel();
}

BOOL CDtMainCableInfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
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
	// �쳣��OCX ����ҳӦ���� FALSE

}

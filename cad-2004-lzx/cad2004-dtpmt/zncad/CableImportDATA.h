#pragma once
#include "entdata.h"
class CCableImportDATA :public CEntDATA
{
public:
	CCableImportDATA(void);
	CCableImportDATA(const CString& strBlkName);
	virtual ~CCableImportDATA(void);
	//���¿׿���
	CString m_strBlkName ;
	//վ��
	CString m_strStation ;
	// ����꣬ȱʡΪ��
	double d_nLoc ;
	// �Ƿ�Ϊ����վ
	BOOL m_bIsJZ  ;
	//���л�������
	bool m_IsUP;
	//���ź��豸�Ҿ���
	double m_dDistToDeviceRoom ;
	static const CString strCableImportItem[5] ;
	//BOOL Init(void);
	BOOL Init(void) ;
};

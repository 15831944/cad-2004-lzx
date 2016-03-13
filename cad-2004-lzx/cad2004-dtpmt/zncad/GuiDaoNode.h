#pragma once
#include "StdAfx.h"
#include "../blkuserdll/UserSystem.h"
#include "../blkdll/MyAdo.h"
//#include "Daocha.h"
class CGuiDaoNode
{
public:
	CGuiDaoNode(void) ;
	CGuiDaoNode(long nHandleLine);
	//bForward==true Ĭ�����ң�bForward ==false Ĭ��Ϊ����
	
	CUserSystem m_sys;
	CADODatabase m_AdoDb;
	CADORecordset m_RecordSet;
	virtual ~CGuiDaoNode(void) ;

	// CZnSxt m_Sxt;
	//������
	long m_nHandleLine ;
	int m_iLineType;
	//�������
	CString m_strGuiDaoName ;
	// 	//��ȡ����ϵĵ���
	vector<long> m_vecThisDaocha_Asc;
	vector<long> m_vecThisDaocha_Desc;
	//�������źŻ�
	vector<long> m_vecHandleXHJLeft;
	//����Ҳ��źŻ�
	vector<long> m_vecHandleXHJRight;
	//���������Ҳ�Ѱ��ʱ
	long m_nZhixhjLeft;
	long m_nXiangxhjLeft;
	long m_nZhixhjRight;
	long m_nXiangxhjRight;
	//���������
	long m_nHandleNextLineLeft;
	//�Ҳ�������
	long m_nHandleNextLineRight;
	BOOL FindNextLineByJYJ( long nHandleLineStart,long &nHandleLineNext,bool bForward );
	void GetOnLineDaoChaOrderByXAsc(UINT nLineHandle, CLongArray& nArrayHandleDC);
	void GetOnLineChaOrderByXDesc(UINT nLineHandle, CLongArray& nArrayHandleDC);
	void GetGuiDaoDuanXHJ(bool bforward,UINT nLineHandle, long& ZhiXHJ,long& XiangXHJ);
	void GetGuiDaoDuanXHJ(UINT nLineHandle, bool bToward,CLongArray& nArrayXHJ);
	BOOL GetBlockToward( long nHandleXHJ ,int &iToward);
	BOOL FindLineType(long nHandleLine,int &iLineType);
    
};

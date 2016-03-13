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
	//bForward==true 默认向右；bForward ==false 默认为向左；
	
	CUserSystem m_sys;
	CADODatabase m_AdoDb;
	CADORecordset m_RecordSet;
	virtual ~CGuiDaoNode(void) ;

	// CZnSxt m_Sxt;
	//轨道句柄
	long m_nHandleLine ;
	int m_iLineType;
	//轨道名称
	CString m_strGuiDaoName ;
	// 	//获取轨道上的道岔
	vector<long> m_vecThisDaocha_Asc;
	vector<long> m_vecThisDaocha_Desc;
	//轨道左侧信号机
	vector<long> m_vecHandleXHJLeft;
	//轨道右侧信号机
	vector<long> m_vecHandleXHJRight;
	//轨道往左或右侧寻找时
	long m_nZhixhjLeft;
	long m_nXiangxhjLeft;
	long m_nZhixhjRight;
	long m_nXiangxhjRight;
	//左侧连接线
	long m_nHandleNextLineLeft;
	//右侧连接线
	long m_nHandleNextLineRight;
	BOOL FindNextLineByJYJ( long nHandleLineStart,long &nHandleLineNext,bool bForward );
	void GetOnLineDaoChaOrderByXAsc(UINT nLineHandle, CLongArray& nArrayHandleDC);
	void GetOnLineChaOrderByXDesc(UINT nLineHandle, CLongArray& nArrayHandleDC);
	void GetGuiDaoDuanXHJ(bool bforward,UINT nLineHandle, long& ZhiXHJ,long& XiangXHJ);
	void GetGuiDaoDuanXHJ(UINT nLineHandle, bool bToward,CLongArray& nArrayXHJ);
	BOOL GetBlockToward( long nHandleXHJ ,int &iToward);
	BOOL FindLineType(long nHandleLine,int &iLineType);
    
};

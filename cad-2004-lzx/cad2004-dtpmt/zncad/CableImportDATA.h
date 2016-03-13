#pragma once
#include "entdata.h"
class CCableImportDATA :public CEntDATA
{
public:
	CCableImportDATA(void);
	CCableImportDATA(const CString& strBlkName);
	virtual ~CCableImportDATA(void);
	//电缆孔块名
	CString m_strBlkName ;
	//站名
	CString m_strStation ;
	// 公里标，缺省为空
	double d_nLoc ;
	// 是否为集中站
	BOOL m_bIsJZ  ;
	//下行还是上行
	bool m_IsUP;
	//距信号设备室距离
	double m_dDistToDeviceRoom ;
	static const CString strCableImportItem[5] ;
	//BOOL Init(void);
	BOOL Init(void) ;
};

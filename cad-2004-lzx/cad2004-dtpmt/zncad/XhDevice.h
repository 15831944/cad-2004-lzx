#pragma once
#include "entdata.h"

class CXhDevice :
	public CEntDATA
{
public:
	CXhDevice(void);
	CXhDevice(const CString& strBlkName);
	virtual~CXhDevice(void);
	//设备名
	CString m_strName;
	//块名
	CString m_strBlkName;
	// 公里标，缺省为空
	double  m_dLoc;
	// 是否采用盒子
	BOOL m_bHasBox;
	//采用箱盒类型
	CString m_strBoxType; 
	//箱盒位于设备上0/下1/左2/右3（缺省值为信号机下）
	CString m_strBoxPos ; 
	//尾缆长度
	int  m_nLenCable;
	//进入箱盒实际芯数
	int m_nCoreNum1 ; 
	int m_nCoreNum2;
	//尾缆备用芯数
	int m_nBackNum1 ;
	int m_nBackNum2;
	//尾缆根数
	int m_nCable;
	CString m_strCable1;
	CString m_strCable2;
	//是否有电话（缺省值为是）
	BOOL m_bHasPhone ;
	//是否有贯通
	BOOL m_bHasGT;
	//是否接地
	BOOL m_bHasGround;
	static const CString strXhDeviceItem[14];
	BOOL Init(void);
	CString m_strBoxTypeOption;
};

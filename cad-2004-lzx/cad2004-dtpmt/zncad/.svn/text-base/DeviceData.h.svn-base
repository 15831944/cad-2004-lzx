#pragma once
#include "StdAfx.h"
class CDeviceData
{
public:
	CDeviceData(void);
	//CDeviceData(double dLibiao1,CString strDevname,double dLibiao2,CString strDevType,int iLine,int iupordown,CString m_strRemarks=_T(""));
	CDeviceData(double dLibiao1,CString strDevname,CString strDevType,int iLine,int iupordown,double dLibiao2=0,CString strRemarks=_T(""));
	bool operator >(const CDeviceData& devData) const ;
	bool operator <(const CDeviceData& devData) const ;
	virtual ~CDeviceData(void);
	double m_dLibiao1;
	CString m_strDevname;
	double m_dLibiao2;
	CString m_strDevType;
	int m_iLine;
	int m_iupordown;
	CString m_strRemarks;
};

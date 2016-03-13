#include "devicedata.h"

CDeviceData::CDeviceData(void)
{
}

CDeviceData::CDeviceData( double dLibiao1,CString strDevname,CString strDevType,int iLine,int iupordown,double dLibiao2/*=0*/,CString strRemarks/*=_T("")*/ )
{
	m_dLibiao1 = dLibiao1;
	m_strDevname = strDevname;
	m_strDevType = strDevType;
	m_iLine = iLine;
	m_iupordown = iupordown;
	m_dLibiao2 = dLibiao2;
	m_strRemarks = strRemarks;

}
CDeviceData::~CDeviceData(void)
{
}
bool CDeviceData::operator>( const CDeviceData& devData ) const
{
	return m_dLibiao1 >devData.m_dLibiao1;
}

bool CDeviceData::operator<( const CDeviceData& devData ) const
{
	return m_dLibiao1 <devData.m_dLibiao1;

}

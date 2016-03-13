#pragma once
#include "entdata.h"

class CDtDcDATA :
	public CEntDATA
{
public:
	CDtDcDATA(void);
	~CDtDcDATA(void);

	//公里标，输入方式：K000+000
	int m_nLoc;
	//尖轨牵引点数（缺省值为1）
	int m_nJG_QyNum ;
	//尖轨牵引方式，ZD6-D、ZD6-EJ、ZD6-EJJ、ZYJ7、ZYJ7-SH6、S700K（无缺省值）
	CString m_strJG_QyType ;
	//尖轨牵引点位于道岔（缺省值为下）
	CString m_strJG_QyPos ;
	//道岔是否采用箱盒
	BOOL m_bDC_HasBox;
	//尖轨牵引点是否共用箱盒
	BOOL m_bDC_ShareBox;
	//道岔箱盒类型
	CString m_strDC_BoxType;
	//箱盒位于道岔(缺省值为下)
	CString m_strDC_BoxPos;
	//箱盒尾缆
	CString m_strCableString;
	//箱盒类型XB1/XB2/HZ12/HZ24（缺省值为HZ24）
	CString m_strBoxType ;
	//是否单动（缺省值为是）
	BOOL m_bIsSingleAct ;
	//是否复式交分（缺省值为否，当选择非单动时有此选项）
	BOOL m_bIsComplex ;
	//多动关联道岔
	CString m_strRelationDc ;
	//第几动 （无缺省值，当非单动、非复式交分时有此选项）
	int m_nActIndex ;
	//是否有电话（缺省值为是）
	BOOL m_bHasPhone ;
	//是否有道岔缺口报警（缺省值为是）
	BOOL m_bHasDCQKBJ ;
	//限速值
	int m_nSpeedLimit ;
	//绝缘节直弯股切割，0：弯股（缺省），1：直股
	int m_iCutJYJPos ;

	static const CString strDcItem[19] ;

	// 根据牵引方式得出每一机牵引
	//int GetDrag(CStringArray& strArrayDrag,  int iJgOrXg=0);
	static int GetDrag(const CString& strQyType);
	// 共几动
	int GetActionNum(void);
};

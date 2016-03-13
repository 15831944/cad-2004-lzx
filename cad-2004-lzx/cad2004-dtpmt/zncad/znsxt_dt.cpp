#pragma once
#include "znsxt.h"
#include "Dtpmt.h"
#include "CableInfoDlg.h"
#include "XhlCableInfoDlg.h"
#include "znutility.h"
#include "PolylineUtil.h"
#include "ConvertUtil.h"
#include "EntPropertyDlg.h"
#include "BlockUtil.h"
#include "DtCableInfoDlg.h"
#include "..\blkuserdll\ViewExtents.h"
#include "BoxNodeEnt.h"
#include "..\blkdll\BlkUtitily.h"
#include "DtMainCableInfo.h"
#include "XhDevice.h"
#include "DtDcDATA.h"
#include "DtAddBoxInfo.h"
#include "DeviceData.h"
//#include ""

//取得地铁各设备的公里标
int CZnSxt::GetDistToXhl_DT(const AcGePoint3d& pt_3d_toxhlheader1, const AcGePoint3d& pt_3d_toxhlheader2, vector<vector<vector<CString> > >& vecSection)
{
	struct resbuf* pRb ;

	//将距离栏的所有文本都设为中间对齐（不移动其位置）
	for (int n=0; n<2; n++)
	{
		ads_point pt1 = {n==0?pt_3d_toxhlheader1.x:pt_3d_toxhlheader2.x, n==0?pt_3d_toxhlheader1.y:pt_3d_toxhlheader2.y-35, 0} ;
		ads_point pt2 = {n==0?pt_3d_toxhlheader1.x+m_dMaxLenPmt:pt_3d_toxhlheader2.x+m_dMaxLenPmt, n==0?pt_3d_toxhlheader1.y+35:pt_3d_toxhlheader2.y, 0} ;
		pRb = acutBuildList(RTDXF0, _T("TEXT"), -4,  _T(">,>,*"), 10,  pt1, -4,  _T("<,<,*"), 10,  pt2, 0) ;
		CLongArray nArrayHandleDist;
		int nLen = m_mysys.GetSSEntHandle(_T("X"), NULL, NULL, pRb, nArrayHandleDist) ;
		acutRelRb(pRb) ;
		for (int i=0; i<nLen; i++)
		{
			AcDbEntity* pEnt = NULL ;
			if (m_mysys.OpenAcDbEntity(nArrayHandleDist.GetAt(i), pEnt))
			{
				AcDbText* pText = AcDbText::cast(pEnt) ;
				m_mysys.ConvertTextMode(pText, AcDb::kTextMid) ;
				pEnt->close() ;
			}
		}
	}

	CMapStringToString mapBlkNameToType ;
	CString strBlkNames[11] = {_T("DT_AP_000"), _T("DT_AP_100"), _T("DT_AP_200"), _T("DT_DHX_000"), _T("DT_JZ_000"), _T("DT_JZ_100"), _T("DT_JZ_200"), _T("DT_RFM_000"), _T("DT_YDQ_000"), _T("DT_YDQ_100"), _T("DT_YDQ_200")} ;
	CString strTypes[11] = {_T("无线-波导管"),  _T("无线-天线"),  _T("无线-漏缆"),  _T("短环线"),  _T("计轴"),  _T("计轴-分界处"),  _T("计轴-超限"),  _T("人防门"),  _T("应答器-可变"),  _T("应答器-固定"),  _T("应答器-预告")} ;
	for (int i=0; i<11; i++)
	{
		mapBlkNameToType.SetAt(strBlkNames[i], strTypes[i]) ;
	}
	//搜索出所有车站标，以此将整条线分成多段，每区段的设备（信号机）不会重名？
	long nNumStation = 0 ;
	CLongArray nArrayHandleStation;
	ads_point pt1 = {pt_3d_toxhlheader2.x, pt_3d_toxhlheader2.y, 0} ;
	ads_point pt2 = {pt_3d_toxhlheader1.x+m_dMaxLenPmt, pt_3d_toxhlheader1.y, 0} ; 
	pRb = acutBuildList(RTDXF0, _T("INSERT"), 2, _T("PMT_STATION"), -4,  _T(">,>,*"), 10,  pt1, -4,  _T("<,<,*"), 10,  pt2, 0) ;
	nNumStation = m_mysys.GetSSEntHandle(_T("X"), NULL, NULL, pRb, nArrayHandleStation) ;
	acutRelRb(pRb) ;

	//begin 各区段x
	m_mysys.SortEnt(nArrayHandleStation,1, 0) ;
	CDoubleArray dArrayX ;
	dArrayX.Add(pt_3d_toxhlheader1.x) ;
	for(int iIndexStation=0; iIndexStation<nNumStation; iIndexStation++)
	{
		AcGePoint3d pt_3d_insert ;
		m_mysys.GetEntPosition(nArrayHandleStation.GetAt(iIndexStation), pt_3d_insert) ;
		dArrayX.Add(pt_3d_insert.x) ;
	}
	dArrayX.Add(pt_3d_toxhlheader1.x+m_dMaxLenPmt) ;
	//end 各区段x

	for(int iIndexSection=0; iIndexSection<(dArrayX.GetCount()-1); iIndexSection++)
	{
		//先直接在距离栏搜索一遍信号机/道岔名称并搜索其匹配的距离
		double dWidthOfTextExt = 3 ; //文本区域所占的宽（此时文本旋转了90度，故此值应等于文本高度）
		long nLen = 0 ;
		CLongArray nArrayHandle;
		CMapStringToString mapToXhl1 ;//距离信号楼距离
		for (int n=0; n<2; n++) //上下2个栏
		{
			//所有在距离栏的信号机/道岔名称
			nLen = 0 ;
			nArrayHandle.RemoveAll() ;

			//n==0 上栏
			pt1[0] = dArrayX.GetAt(iIndexSection) ;
			pt1[1] = (n==0?(pt_3d_toxhlheader1.y):(pt_3d_toxhlheader2.y-13));
			pt2[0] = dArrayX.GetAt(iIndexSection+1) ;
			pt2[1] = (n==0?(pt_3d_toxhlheader1.y+13):(pt_3d_toxhlheader2.y)) ;

			pRb = acutBuildList(RTDXF0, _T("TEXT"), -4,  _T(">,>,*"), 10,  pt1, -4,  _T("<,<,*"), 10,  pt2, 0) ;
			nLen = m_mysys.GetSSEntHandle(_T("X"), NULL, NULL, pRb, nArrayHandle) ;
			acutRelRb(pRb) ;
			acutPrintf(_T("\nall name in header:%d"), nLen) ;

			for (int j=0; j<nLen; j++)
			{
				AcDbEntity * pEnt = NULL ;
				if(m_mysys.OpenAcDbEntity(nArrayHandle.GetAt(j), pEnt, AcDb::kForRead))
				{
					if (pEnt->isKindOf(AcDbText::desc()))
					{
						AcDbText* pText = AcDbText::cast(pEnt) ;
						CString strName = pText->textString() ;
						strName.Trim() ;
						acutPrintf(_T("\n第1次搜索:%s："), strName) ;

						//找出位置和对起点均在指定范围的文本（距离）
						AcDbExtents ext ;
						pText->getGeomExtents(ext) ;
						AcGePoint3d pt_3d_min = ext.minPoint() ;
						AcGePoint3d pt_3d_max = ext.maxPoint() ;
						AcGePoint3d pt_3d_mid = m_mysys.GetMidPoint(pt_3d_min, pt_3d_max) ;
						dWidthOfTextExt = abs(pt_3d_max.x-pt_3d_min.x) ;

						pt1[0] = pt_3d_min.x-dWidthOfTextExt/8 ;
						pt1[1] =(n==0?(pt_3d_toxhlheader1.y+13):(pt_3d_toxhlheader2.y-35));
						pt2[0] = pt_3d_max.x+dWidthOfTextExt/8 ;
						pt2[1] = (n==0?(pt_3d_toxhlheader1.y+35):(pt_3d_toxhlheader2.y-13)) ;
						pRb = acutBuildList(RTDXF0, _T("TEXT"), -4,  _T(">,>,*"), 10,  pt1, -4,  _T("<,<,*"), 10,  pt2, -4,  _T(">,>,*"), 11,  pt1, -4,  _T("<,<,*"), 11,  pt2, 0) ;
						CLongArray nArrayHandleVal ;
						long nLenVal = m_mysys.GetSSEntHandle(_T("X"), NULL, NULL, pRb, nArrayHandleVal) ;
						if (nLenVal==1)
						{
							CString strVal ;
							m_mysys.GetTextString(nArrayHandleVal.GetAt(0), strVal) ;
							strVal.Trim() ;
							if (!strVal.IsEmpty())
							{
								strVal.AppendFormat(_T("__%s"), n==0?_T("上"):_T("下")) ; //附带记录该公里标写在上面还是下面
								acutPrintf(_T("%s："), strVal) ;
								mapToXhl1.SetAt(strName, strVal) ;
							}
						}
						acutRelRb(pRb) ;
					}
					pEnt->close() ;
				}
			}
		}

		//在上下两栏之间的实体集合
		nLen = 0 ;
		nArrayHandle.RemoveAll() ;
		pt1[0] = dArrayX.GetAt(iIndexSection) ; 
		pt1[1] = pt_3d_toxhlheader2.y ;
		pt2[0] = dArrayX.GetAt(iIndexSection+1) ; 
		pt2[1] = pt_3d_toxhlheader1.y ;
		pRb = acutBuildList(RTDXF0, _T("INSERT"), -4,  _T(">,>,*"), 10,  pt1, -4,  _T("<,<,*"), 10,  pt2, 0) ;
		nLen = m_mysys.GetSSEntHandle(_T("X"), NULL, NULL, pRb, nArrayHandle) ;
		acutRelRb(pRb) ;
		//按x坐标增大顺序排序
		m_mysys.SortEnt(nArrayHandle, 1) ;
		vector<vector<CString > > vecToXhl ;
		for(int i=0; i<nLen; i++)
		{
			UINT nHandle =nArrayHandle.GetAt(i) ;
			AcDbEntity * pEnt = NULL ;
			if (m_mysys.OpenAcDbEntity(nArrayHandle.GetAt(i), pEnt, AcDb::kForRead))
			{
				if (pEnt->isKindOf(AcDbBlockReference::desc()))
				{
					AcDbBlockReference* pRef = AcDbBlockReference::cast(pEnt) ;
					AcGePoint3d pt_3d_base = pRef->position() ;
					CString strBlkName ;
					m_mysys.GetBlockName(pRef, strBlkName) ;
					CString strDevName ; //信号机名称或道岔编号
					CString strType ;

					if (strBlkName.Left(6)==_T("DT_XHJ"))
					{
						m_mysys.GetAttValue(pRef, _T("XHJNAME"), strDevName) ;
						strType = _T("信号机") ;
					}
					else if (strBlkName.Left(5)==_T("DT_DC"))
					{
						m_mysys.GetAttValue(pRef, _T("BIANHAO"), strDevName) ;
						strType = _T("道岔") ;
					}
					else if(strBlkName.Left(5)==_T("DT_CD"))
					{
						m_mysys.GetAttValue(pRef, _T("NAME"), strDevName) ;
						strType = _T("车档") ;
					}
					else if(strBlkName.Left(6)==_T("DT_JCB"))
					{
						m_mysys.GetAttValue(pRef, _T("NAME"), strDevName) ;
						strType = _T("警冲标") ;
					}
					else 
					{
						m_mysys.GetAttValue(pRef, _T("NAME"), strDevName) ;
						strType = _T("其他") ;
						mapBlkNameToType.Lookup(strBlkName, strType) ;
// 						if(strType==_T("其他"))
// 						{
// 							strDevName=_T("");
// 						}
					}

					if (!strDevName.IsEmpty())
					{
						CString strDist;	
						mapToXhl1.Lookup(strDevName, strDist) ;	

						vector<CString > vec ;
						vec.push_back(strType) ;
						vec.push_back(strDevName) ;
						//strDist.AppendFormat(_T("__%s"), n==0?_T("上"):_T("下")) ;
						vec.push_back(strDist) ;

						vecToXhl.push_back(vec) ;
						acutPrintf(_T("\n第2次搜索:%s：%s"), strDevName, strDist) ;
					}
				}

				pEnt->close() ;
			}
		}

		vecSection.push_back(vecToXhl) ;
	}

	return 0;
}

void CZnSxt::ReadAllEntToDB2_DT(void)
{
	CString strMsg ;

	acutPrintf(_T("\n读取平面图，注意此函数默认搜索公里标范围为%.0f"), m_dMaxLenPmt) ;
	//找到距离信号楼距离框的图块 
	AcGePoint3d pt_3d_toxhlheader1, pt_3d_toxhlheader2 ;//距信号楼距离图块基点
	if(this->GetToXhlHeaderPos(pt_3d_toxhlheader1, pt_3d_toxhlheader2)!=13)
	{
		AfxMessageBox(_T("未发现2个地铁公里标栏图块 ，程序退出，请处理后再继续!")) ;
		return ;
	}

	m_AdoDb.ClearTable(_T("sxt_ent")) ;
	m_AdoDb.ClearTable(_T("sxt_line")) ;

	int nLen = 0 ;
	CLongArray nArrayHandle ;
	AcGePoint3d pt_3d_1(pt_3d_toxhlheader2.x-50, pt_3d_toxhlheader2.y, 0), pt_3d_2(pt_3d_toxhlheader1.x+m_dMaxLenPmt, pt_3d_toxhlheader1.y, 0) ;
	nLen = m_mysys.FindEntInArea(pt_3d_1, pt_3d_2, nArrayHandle, 1) ;

	CString strSql ;
	for(int i=0; i<nLen; i++)
	{
		UINT nHandle =nArrayHandle.GetAt(i) ;
		AcDbEntity * pEnt = NULL ;
		if (m_mysys.OpenAcDbEntity(nHandle, pEnt, AcDb::kForRead))
		{
			if (pEnt->isKindOf(AcDbBlockReference::desc()))
			{
				AcDbBlockReference *pRef = AcDbBlockReference::cast(pEnt) ;
				AcGePoint3d pt_3d_position = pRef->position() ;
				//将块统一为X比例和旋转角变化（X、Y都置-1，等同于旋转180度，X置-1并且旋转180度等同于Y置-1，Y置-1并且旋转180度等同于X置-1）
				AcGeScale3d scale_3d = pRef->scaleFactors()  ;		
				if (scale_3d.sy<0)
				{
					pRef->upgradeOpen() ;
					scale_3d.set(-scale_3d.sx, -scale_3d.sy, scale_3d.sz) ;
					pRef->setScaleFactors(scale_3d) ;
					pRef->setRotation(pRef->rotation()-PI) ;
					pRef->downgradeOpen() ;
				}	

				CString strName ;
				CString strBlkName ;
				m_mysys.GetBlockName(pRef, strBlkName) ;

				CStringArray strArrayBlkName ;
				CBlkUtility::DivideString(strBlkName, _T("_"), strArrayBlkName) ;
				CString strBlkType ;
				if (strArrayBlkName.GetCount()>0)
				{
					strBlkType = strArrayBlkName.GetAt(0) ;
				}

				//if (strBlkType==_T("DC"))
				if(strBlkName.Left(5)==_T("DT_DC"))
				{
					m_mysys.GetAttValue(pRef, _T("BIANHAO"), strName) ;
					//和DC块定/反位连接的线信息
					CLongArray nArrayHandleIntersectLine, nArrayIndexOfVertex ;
					int iDwIntersectMoveToVertex = -1 ;
					AcGePoint3dArray pt3dArrayIntersect ;
					if(this->GetDcConLineDFW(pRef, nArrayHandleIntersectLine, nArrayIndexOfVertex, pt3dArrayIntersect, iDwIntersectMoveToVertex) )
					{
						strSql.Format(_T("insert into sxt_ent(sxt_ent_handle,  sxt_ent_block, sxt_ent_x, sxt_ent_y, sxt_ent_movetovertex, sxt_ent_name, sxt_ent_type,  sxt_ent_online_handle,  sxt_ent_online_vertex,  sxt_ent_online_intersectx,  sxt_ent_online_intersecty,  sxt_ent_fwline_handle,  sxt_ent_fwline_vertex,  sxt_ent_x1, sxt_ent_y1, sxt_ent_online_intersectx1,  sxt_ent_online_intersecty1)  \
										 values(%d, '%s', %.10f, %.10f, %d, '%s', %d, %d, %d, %.10f, %.10f, %d, %d, %.10f, %.10f, %.10f, %.10f) "), nHandle, strBlkName,pt_3d_position.x, pt_3d_position.y, iDwIntersectMoveToVertex, strName, 0, nArrayHandleIntersectLine.GetAt(0), nArrayIndexOfVertex.GetAt(0), pt3dArrayIntersect.at(0).x, pt3dArrayIntersect.at(0).y, nArrayHandleIntersectLine.GetAt(1), nArrayIndexOfVertex.GetAt(1), pt_3d_position.x, pt_3d_position.y, pt3dArrayIntersect.at(0).x, pt3dArrayIntersect.at(0).y) ;
						m_AdoDb.Execute(strSql) ;
					}					
				}				
				else
				{
					int iEntType = -1 ; 

					if (strBlkName==_T("PMT_STATION"))
					{
						iEntType = 310 ;
						m_mysys.GetAttValue(pRef, _T("STATION"), strName) ;
					}
					else if (strBlkType==_T("PMT"))
					{
						iEntType = 350 ;
					}
					else if (strBlkType==_T("XHJ"))
					{
						iEntType = 100 ;
						m_mysys.GetAttValue(pRef, _T("XHJNAME"), strName) ;	
					}
					else if (strBlkType==_T("DT"))
					{			
						iEntType = 500; 
						if(strBlkName.Left(6)==_T("DT_XHJ"))
						{
							iEntType=550;
							m_mysys.GetAttValue(pRef, _T("XHJNAME"), strName) ;	
						}
						else
						{
							m_mysys.GetAttValue(pRef, _T("NAME"), strName);
						}
						if(strBlkName.Left(14)==_T("DT_CABLEIMPORT"))
						{
							iEntType=560;
						}
						if(strBlkName.Left(5)==_T("DT_CD"))
						{
							//车档
							iEntType=570;
						}
			
					}
					strSql.Format(_T("insert into sxt_ent(sxt_ent_handle,  sxt_ent_block, sxt_ent_x, sxt_ent_y, sxt_ent_name, sxt_ent_type,  sxt_ent_x1, sxt_ent_y1, sxt_ent_toxhldist)  \
									 values(%d, '%s', %.10f, %.10f, '%s',  %d, %.10f, %.10f, %d) "), nHandle, strBlkName, pt_3d_position.x, pt_3d_position.y, strName, iEntType,  pt_3d_position.x, pt_3d_position.y, 0) ;
					m_AdoDb.Execute(strSql) ;
				}

			}
			pEnt->close() ;
		}	
	}
	//再遍历一遍，将轨道线读入
	for(int i=0; i<nLen; i++)
	{
		AcDbEntity * pEnt = NULL ;
		if (m_mysys.OpenAcDbEntity(nArrayHandle.GetAt(i), pEnt, AcDb::kForRead))
		{
			if (pEnt->isKindOf(AcDbPolyline::desc()))
			{
				long nHandleLine =nArrayHandle.GetAt(i) ;

				AcDbPolyline *pPolyLine = AcDbPolyline::cast(pEnt) ;	
				bool bIsClosed = pPolyLine->isClosed() ;
				double dLen = m_mysys.GetLineLen(pPolyLine) ;
				if (dLen>20) //粗略筛选轨道线
				{
					int iNumVert = pPolyLine->numVerts() ;
					for (int i=0; i<iNumVert; i++)
					{
						AcGePoint3d pt_3d_vertex(0,0,0) ;
						pPolyLine->getPointAt(i, pt_3d_vertex) ;
						strSql.Format(_T("insert into sxt_line(sxt_line_handle, sxt_line_vertex,  sxt_line_vertex_x, sxt_line_vertex_y, sxt_line_vertex1,  sxt_line_vertex_x1, sxt_line_vertex_y1, sxt_line_type )  \
										 values(%d, %d, %.10f, %.10f, %d, %.10f, %.10f, %d) "), nHandleLine, i,  pt_3d_vertex.x, pt_3d_vertex.y, i,  pt_3d_vertex.x, pt_3d_vertex.y, bIsClosed?11:0) ;
						m_AdoDb.Execute(strSql) ;
					}
				}
			}
			pEnt->close() ;
		}
	}

	//读入公里标栏，i==0：上栏，i==1：下栏
	for (int i=0; i<2; i++)
	{
		for (int j=0; j<2; j++) //j=0:设备栏，j=1：公里标栏
		{
			nLen = 0 ;
			nArrayHandle.RemoveAll() ;
			AcGePoint3d pt_3d_1(i==0?pt_3d_toxhlheader1.x:pt_3d_toxhlheader2.x, i==0?pt_3d_toxhlheader1.y:(pt_3d_toxhlheader2.y-13), 0), pt_3d_2(i==0?(pt_3d_toxhlheader1.x+m_dMaxLenPmt):(pt_3d_toxhlheader2.x+m_dMaxLenPmt), i==0?(pt_3d_toxhlheader1.y+13):pt_3d_toxhlheader2.y, 0) ;
			if (j==1)
			{
				pt_3d_1.y =(i==0?(pt_3d_toxhlheader1.y+13):(pt_3d_toxhlheader2.y-35) ) ;
				pt_3d_2.y = (i==0?(pt_3d_toxhlheader1.y+35):(pt_3d_toxhlheader2.y-13) ) ;
			}
			
			nLen = m_mysys.FindEntInArea(pt_3d_1, pt_3d_2, nArrayHandle, 1) ;

			CString strSql ;
			for(int n=0; n<nLen; n++)
			{
				UINT nHandle =nArrayHandle.GetAt(n) ;
				AcDbEntity * pEnt = NULL ;
				if (m_mysys.OpenAcDbEntity(nHandle, pEnt, AcDb::kForRead))
				{
					if (pEnt->isKindOf(AcDbText::desc()))
					{
						AcDbText* pText = AcDbText::cast(pEnt) ;
						CString strText = pText->textString() ;
						AcGePoint3d pt_3d_position ;
						pt_3d_position = pText->position() ;
						if (!strText.IsEmpty())
						{
							strSql.Format(_T("insert into sxt_ent(sxt_ent_handle,  sxt_ent_x, sxt_ent_y, sxt_ent_name, sxt_ent_type,  sxt_ent_x1, sxt_ent_y1)  \
											 values(%d, %.10f, %.10f, '%s',  %d, %.10f, %.10f) "), nHandle, pt_3d_position.x, pt_3d_position.y, strText,  600+i*10+j,  pt_3d_position.x, pt_3d_position.y) ;
							m_AdoDb.Execute(strSql) ;
						}
					}
					pEnt->close() ;
				}
			}
		}
	}

}



// 绘制双线图（地铁）
int CZnSxt::DrawSxt_DT2(double dScaleX, double dScaleY, const AcGePoint3d & pt_3d_orign, double dLenDcPlus, double dDistBetweenSX)
{
	double dDistBetweenSxHalf = dDistBetweenSX/2 ;
	double dDc25 = dDistBetweenSxHalf, dDc15 = dDistBetweenSX*0.3 ; //道岔双线绘制时
	AcDbObjectId objIdOldLayer = m_pDb->clayer() ;
	m_mysys.Set_Layer(_T(""), _T("sxt")) ;
	AcGeTol tol  ; 
	tol.setEqualVector(0.00001) ;
	try
	{
		CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;

		//begin 根据指定基点、缩放比值，计算实体（信号机、道岔、轨道线）坐标值
		CString strTables[2] ={ _T("sxt_ent"), _T("sxt_line")} ;
		for (int iTbIndex=0; iTbIndex<2; iTbIndex++)
		{
			strSql.Format(_T("select * from %s"), strTables[iTbIndex]) ;
			CString strField1[2], strField2[2], strField3[2], strField4[2] ;

			for (int i=0; i<2; i++)
			{
				if (iTbIndex==0)
				{
					strField1[i].Format(_T("%s_x%d"), strTables[iTbIndex], i+1) ;
					strField2[i].Format(_T("%s_y%d"), strTables[iTbIndex], i+1) ;
					strField3[i].Format(_T("%s_online_intersectx%d"), strTables[iTbIndex], i+1) ;
					strField4[i].Format(_T("%s_online_intersecty%d"), strTables[iTbIndex], i+1) ;
				}
				else if (iTbIndex==1)
				{
					strField1[i].Format(_T("%s_vertex_x%d"), strTables[iTbIndex], i+1) ;
					strField2[i].Format(_T("%s_vertex_y%d"), strTables[iTbIndex], i+1) ;
				}
			}

			if(cSet.Open(strSql))
			{
				while(!cSet.IsEOF())
				{
					double dEntX = 0.000, dEntY = 0.000, dEntX1 = 0.000, dEntY1 = 0.000;		

					cSet.GetFieldValue(strField1[0], dEntX) ;
					cSet.GetFieldValue(strField2[0], dEntY) ;
					dEntX = pt_3d_orign.x +dEntX*dScaleX ;
					dEntY = pt_3d_orign.y +dEntY*dScaleY ;

					if (iTbIndex==0)
					{
						cSet.GetFieldValue(strField3[0], dEntX1) ;
						cSet.GetFieldValue(strField4[0], dEntY1) ;
						dEntX1 = pt_3d_orign.x +dEntX1*dScaleX ;
						dEntY1 = pt_3d_orign.y +dEntY1*dScaleY ;
					}
					cSet.Edit() ;
					cSet.SetFieldValue(strField1[1], dEntX) ;
					cSet.SetFieldValue(strField2[1], dEntY) ;
					if (iTbIndex==0)
					{
						cSet.SetFieldValue(strField3[1], dEntX1) ;
						cSet.SetFieldValue(strField4[1], dEntY1) ;
					}
					cSet.Update() ;

					cSet.MoveNext() ;
				}

				cSet.Close() ;
			}
		}
		//end 根据指定基点、缩放比值，计算实体（信号机、道岔、轨道线）坐标值
		//begin 绘制线
		strSql.Format(_T("select distinct sxt_line_handle from sxt_line where sxt_line_type<>11")) ;
		if (cSet.Open(strSql))
		{
			while(!cSet.IsEOF())
			{
				long  iHandle = 0 ;
				cSet.GetFieldValue(_T("sxt_line_handle"), iHandle) ;
				//begin 读取该线信息for绘制轨道名称
				CString strGdName ;
				int iGdWriteLeftOrRight = -1 ; //轨道名称写在线左侧0还是右侧1
				AcDbEntity* pEntLine = NULL ;
				if (m_mysys.OpenAcDbEntity(iHandle, pEntLine, AcDb::kForRead))
				{
					if(m_mysys.GetXdata(pEntLine, _T("GUIDAONAME"), strGdName))
					{
						if (!strGdName.IsEmpty())
						{
							CString strLeftForS=_T("N"), strRightForS = _T("N") ;
							m_mysys.GetXdata(pEntLine, _T("LEFT_FORS"), strLeftForS) ;
							m_mysys.GetXdata(pEntLine, _T("RIGHT_FORS"), strRightForS) ;
							if (strLeftForS==_T("S"))
							{
								iGdWriteLeftOrRight = 0 ;
							}
							else if (strRightForS==_T("S"))
							{
								iGdWriteLeftOrRight = 1 ;
							}
							else if (strLeftForS==_T("F"))
							{
								iGdWriteLeftOrRight = 0 ;
							}
							else if (strRightForS==_T("F"))
							{
								iGdWriteLeftOrRight = 1 ;
							}
						}
					}
					pEntLine->close() ;
				}
				//end 读取该线信息for绘制轨道名称

				//确定该多段线的起点和终点是否都是DC的反位连接端

				AcGePoint2dArray pt2dArrayUp, pt2dArrayDown ;
				AcArray <double> dDrawOptionUps,dDrawOptionDowns ;
				//VertexInfo=A:0,B:0,C:0;A:1,B:0,C:0;A:2,B:1,C:-1;A:3,B:2,C:1;A:4,B:3,C:2
				//ABC后面数字表示：A，双线中线的顶点序号；B，原平面图线加上极性切割绝缘节后形成的顶点序号；C，原平面图线不考虑极性绝缘节的顶点序号
				CString strXdataUp, strXdataDown ; 
				CString strXdataUpForZZJ, strXdataDownForZZJ ; //记录形式（针对的是道岔FW线）：端头连接的道岔handle-道岔FW朝向（1，朝上；0，朝下）-FW端线的顶点vertex（起点或终点）-道岔所在DW线-道岔所在DW线vertex
				strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d order by sxt_line_vertex1"), iHandle) ;
				CADORecordset cSetVertex(&m_AdoDb) ;
				if (cSetVertex.Open(strSql))
				{
					int iNumVertex = cSetVertex.GetRecordCount() ;
					if (!cSetVertex.IsBOF())
					{
						cSetVertex.MoveFirst() ;
					}
					int iIndexVertex = 0 ;
					int iDrawOptionPre = 2222 ; //前一顶点的DrawOption
					while (!cSetVertex.IsEOF())
					{
						//acutPrintf(_T("\nhandle:%d,vetex:%d"), iHandle, iIndexVertex) ;
						AcGePoint2d pt_2d_up(0,0), pt_2d_down(0,0) ; //双线的上下2个点
						AcGePoint2d pt_2d_cur(0,0), pt_2d_pre(0,0), pt_2d_back(0,0) ; 

						int iIndexVertexOld = iIndexVertex ; //原顶点序号（未加入切割点）
						cSetVertex.GetFieldValue(_T("sxt_line_vertex"), iIndexVertexOld) ;

						cSetVertex.GetFieldValue(_T("sxt_line_vertex_x2"), pt_2d_cur.x) ;
						cSetVertex.GetFieldValue(_T("sxt_line_vertex_y2"), pt_2d_cur.y) ;

						//begin 线宽
						long nHandleVertex = 0 ; //该点对应的绝缘节或者道岔
						int iDrawOptionCur = -1 ; //当前顶点的DrawOption
						cSetVertex.GetFieldValue(_T("sxt_line_vertex_handle"), nHandleVertex) ;

						double dWidthUp = 0, dWidthDown = 0 ;

						int iPolar = 0 ;
						cSetVertex.GetFieldValue(_T("sxt_line_polar"), iPolar) ;
						if (iIndexVertex==(iNumVertex-1)) //最后一段极性取前一个顶点的值
						{
							cSetVertex.MovePrevious() ;
							cSetVertex.GetFieldValue(_T("sxt_line_polar"), iPolar) ;
							cSetVertex.MoveNext() ;
						}
						if (iPolar==-1) //上粗下细
						{
							dWidthUp = 0.5 ;
							dWidthDown = 0 ;							
						}
						else if (iPolar==1)
						{
							dWidthUp = 0 ;
							dWidthDown = 0.5 ;				
						}
						//end 线宽

						if (iIndexVertex!=0)//不是第一个点
						{
							cSetVertex.MovePrevious() ;
							cSetVertex.GetFieldValue(_T("sxt_line_vertex_x2"), pt_2d_pre.x) ;
							cSetVertex.GetFieldValue(_T("sxt_line_vertex_y2"), pt_2d_pre.y) ;
							cSetVertex.MoveNext() ;
						}
						if (iIndexVertex!=(iNumVertex-1)) //不是最后一个点
						{
							cSetVertex.MoveNext() ;
							cSetVertex.GetFieldValue(_T("sxt_line_vertex_x2"), pt_2d_back.x) ;
							cSetVertex.GetFieldValue(_T("sxt_line_vertex_y2"), pt_2d_back.y) ;
							cSetVertex.MovePrevious() ;
						}

						if (iIndexVertex==0)
						{
							AcGeLineSeg2d lineSeg2dSrc(pt_2d_cur, pt_2d_back) ;
							AcGeLineSeg2d lineSeg2dParallelUp, lineSeg2dParallelDown ;
							m_mysys.GetParallelLine(lineSeg2dSrc, lineSeg2dParallelUp, dDistBetweenSxHalf ) ;
							m_mysys.GetParallelLine(lineSeg2dSrc, lineSeg2dParallelDown, dDistBetweenSxHalf, FALSE) ;
							pt_2d_up = lineSeg2dParallelUp.startPoint() ;
							pt_2d_down = lineSeg2dParallelDown.startPoint() ;

							//begin 绘制轨道名称
							if (iGdWriteLeftOrRight==0)
							{
								AcGePoint2d pt_2d_gdname = lineSeg2dSrc.midPoint() ;
								AcGePoint3d pt_3dg_gdname(pt_2d_gdname.x, pt_2d_gdname.y, 0) ;
								AcGeVector2d vecLine = lineSeg2dSrc.direction() ;
								AcDbObjectId objIdGdName ;
								m_mysys.AddText(objIdGdName, strGdName, pt_3dg_gdname, pt_3dg_gdname, AcDb::kTextCenter, AcDb::kTextVertMid, 2.5, 0.8, vecLine.angle(), 4)  ;	
								m_mysys.SetXdata(objIdGdName, _T("TYPE"), (long)101) ; //双线图中轨道名称文本
								m_mysys.SetXdata(objIdGdName, _T("BELONG_ENT"), iHandle) ;
							}
							//end 绘制轨道名称

							//和该FW端相交的DW轨道线的信息
							AcGePoint2d pt_2d_start_dw, pt_2d_end_dw ;	
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d and sxt_ent_fwline_vertex=%d"), iHandle, iIndexVertex) ;
							long iHandleDW = 0 ;
							int iLineVertexDW = 0 ;							
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), iHandleDW) ;
							long nHandleDc = 0 ; //以该线为反位线的道岔记录下来，为双线图绘制尖轨芯轨用
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDc) ;
							CString strDcName ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), strDcName) ;
							int iUpOrDownDc = 0 ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_upordown"), iUpOrDownDc) ;
							if (iHandleDW!=0) //还有不是和DC反位相连的线，比如轨道起始
							{
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_vertex"), iLineVertexDW) ;
								strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d and sxt_line_vertex=%d"), iHandleDW, iLineVertexDW) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_line_vertex_x2"), pt_2d_start_dw.x) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_line_vertex_y2"),  pt_2d_start_dw.y) ;
								strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d and sxt_line_vertex=%d"), iHandleDW, iLineVertexDW+1) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_line_vertex_x2"), pt_2d_end_dw.x) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_line_vertex_y2"),  pt_2d_end_dw.y) ;
								//2条平行线，一条距定位2.5,，另一条是距定位线1.5
								AcGeLineSeg2d lineSeg2dDwSrc(pt_2d_start_dw, pt_2d_end_dw), lineSeg2dDw25, lineSeg2dDw15 ;

								//begin 尖轨、芯轨牵引数量
								//int iJgNum = 3, iXgNum = 2 ; 
								double dAngleDc = 0 ; //道岔块旋转角度
								AcDbEntity* pEntDc = NULL ;
								if (m_mysys.OpenAcDbEntity(nHandleDc, pEntDc, AcDb::kForRead))
								{
									AcDbBlockReference* pRefDc = AcDbBlockReference::cast(pEntDc) ;
									dAngleDc = pRefDc->rotation() ;
									pEntDc->close() ;
								}
								//end 尖轨、芯轨牵引数量

								AcGePoint2d pt_2d_ext ;
								AcGeVector2d vecDw = pt_2d_end_dw-pt_2d_start_dw ;
								//用于判断道岔垂直方向，不能简单根据*** int iDcDirectV = ((pt_2d_back.y-pt_2d_cur.y)>0.5?0:1) ; //道岔垂直方向（FW向上0、向下1）***
								//根据以下算法：道岔旋转角，90~270度之间，向下	
								int iDcDirectV =0 ; //道岔垂直方向（FW向上0、向下1）
								if (dAngleDc>PI/2&&dAngleDc<(PI*3/2))
								{
									iDcDirectV = 1 ;
								}
								if (iDcDirectV==0) //反位端朝上
								{
									//反位端相交的所在多段线及段序号
									m_mysys.GetParallelLine(lineSeg2dDwSrc, lineSeg2dDw25, dDc25, TRUE) ;
									m_mysys.GetParallelLine(lineSeg2dDwSrc, lineSeg2dDw15, dDc15, FALSE) ;

									lineSeg2dParallelUp.intersectWith(lineSeg2dDw25, pt_2d_up) ;
									lineSeg2dParallelDown.intersectWith(lineSeg2dDw15, pt_2d_down) ;

									//Dc反位线和定位线平行的一段
									pt_2d_ext =pt_2d_down ;
									pt_2d_ext.x -=dLenDcPlus ;
									pt_2d_ext.rotateBy(vecDw.angle(), pt_2d_down) ;								
									pt2dArrayDown.append(pt_2d_ext) ;		
									dDrawOptionDowns.append(dWidthDown) ;
									strXdataDown.AppendFormat(_T("A:%d,B:%d,C:%d;"), pt2dArrayDown.length()-1, iIndexVertex, iIndexVertexOld) ;
									strXdataDownForZZJ.AppendFormat(_T("%d-1-%d-%d-%d;"), nHandleDc, iIndexVertex, iHandleDW, iLineVertexDW) ;	
								}
								else //反位端朝下，（反位端在水平线上，视同反位朝下）
								{
									m_mysys.GetParallelLine(lineSeg2dDwSrc, lineSeg2dDw25, dDc25, FALSE) ;
									m_mysys.GetParallelLine(lineSeg2dDwSrc, lineSeg2dDw15, dDc15, TRUE) ;

									lineSeg2dParallelUp.intersectWith(lineSeg2dDw15, pt_2d_up) ;
									lineSeg2dParallelDown.intersectWith(lineSeg2dDw25, pt_2d_down) ;

									//Dc反位线和定位线平行的一段
									pt_2d_ext = pt_2d_up ;
									pt_2d_ext.x -=dLenDcPlus ;
									pt_2d_ext.rotateBy(vecDw.angle(), pt_2d_up) ;
									pt2dArrayUp.append(pt_2d_ext) ;
									dDrawOptionUps.append(dWidthUp) ;
									strXdataUp.AppendFormat(_T("A:%d,B:%d,C:%d;"), pt2dArrayUp.length()-1, iIndexVertex, iIndexVertexOld) ;
									strXdataUpForZZJ.AppendFormat(_T("%d-0-%d-%d-%d;"), nHandleDc, iIndexVertex, iHandleDW, iLineVertexDW) ;
								}

								//begin draw zzj
								int iDcDirectH =0 ; //道岔FW水平朝向，0朝右，1，朝左
								AcGeLineSeg2d line2d(pt_2d_ext,iDcDirectV==0?pt_2d_down:pt_2d_up) ;									
								AcGePoint2d pt_2d_extcen =line2d.midPoint() ;
								AcGePoint2d pt_2d_extcen_mid = pt_2d_extcen ;
								pt_2d_extcen_mid.y+=(iDcDirectV==0?dDc15:-dDc15) ;
								pt_2d_extcen_mid.rotateBy(vecDw.angle(), pt_2d_extcen) ;

								//映射到双线的一边
								AcGePoint2d pt_2d_extcen_jg = pt_2d_extcen_mid ;
								pt_2d_extcen_jg.y +=(iUpOrDownDc==0?-dDistBetweenSxHalf:dDistBetweenSxHalf); 
								pt_2d_extcen_jg.rotateBy(vecDw.angle(), pt_2d_extcen) ;

								//道岔名称
								AcGePoint2d pt_2d_dcname = pt_2d_extcen_jg ;
								pt_2d_dcname.y+=(iUpOrDownDc==0?-2:2); 
								pt_2d_dcname.rotateBy(vecDw.angle(), pt_2d_extcen_jg) ;
								AcGePoint3d pt_3d_dcname(pt_2d_dcname.x, pt_2d_dcname.y,0) ;
								m_mysys.AddText(strDcName, pt_3d_dcname, pt_3d_dcname, AcDb::kTextCenter, AcDb::kTextVertMid, 2.5, 0.8, vecDw.angle(), 3) ;
								//end draw zzj
							}

							pt2dArrayUp.append(pt_2d_up) ;
							dDrawOptionUps.append(dWidthUp) ;
							strXdataUp.AppendFormat(_T("A:%d,B:%d,C:%d;"), pt2dArrayUp.length()-1, iIndexVertex, iIndexVertexOld) ;
							pt2dArrayDown.append(pt_2d_down) ;
							dDrawOptionDowns.append(dWidthDown) ;
							strXdataDown.AppendFormat(_T("A:%d,B:%d,C:%d;"), pt2dArrayDown.length()-1, iIndexVertex, iIndexVertexOld) ;
						}
						else if (iIndexVertex==(iNumVertex-1))
						{
							AcGeLineSeg2d lineSeg2dSrc(pt_2d_pre, pt_2d_cur) ;
							AcGeLineSeg2d lineSeg2dParallelUp, lineSeg2dParallelDown ;
							m_mysys.GetParallelLine(lineSeg2dSrc, lineSeg2dParallelUp, dDistBetweenSxHalf ) ;
							m_mysys.GetParallelLine(lineSeg2dSrc, lineSeg2dParallelDown, dDistBetweenSxHalf, FALSE) ;
							pt_2d_up = lineSeg2dParallelUp.endPoint() ;
							pt_2d_down = lineSeg2dParallelDown.endPoint() ;

							//begin 绘制轨道名称
							if (iGdWriteLeftOrRight==1)
							{
								AcGePoint2d pt_2d_gdname = lineSeg2dSrc.midPoint() ;
								AcGePoint3d pt_3dg_gdname(pt_2d_gdname.x, pt_2d_gdname.y, 0) ;
								AcGeVector2d vecLine = lineSeg2dSrc.direction() ;
								AcDbObjectId objIdGdName ;
								m_mysys.AddText(objIdGdName, strGdName, pt_3dg_gdname, pt_3dg_gdname, AcDb::kTextCenter, AcDb::kTextVertMid, 2.5, 0.8, vecLine.angle(), 4)  ;	
								m_mysys.SetXdata(objIdGdName, _T("TYPE"), (long)101) ; //双线图中轨道名称文本
								m_mysys.SetXdata(objIdGdName, _T("BELONG_ENT"), iHandle) ;
							}
							//end 绘制轨道名称

							int iIndexVertexOld = -1 ;
							cSetVertex.GetFieldValue(_T("sxt_line_vertex"), iIndexVertexOld) ;

							//和该FW端相交的DW轨道线的信息
							AcGePoint2d pt_2d_start_dw, pt_2d_end_dw ;	
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d and sxt_ent_fwline_vertex=%d"), iHandle, iIndexVertexOld) ;
							long iHandleDW = 0 ;
							int iLineVertexDW = 0 ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), iHandleDW) ;
							long nHandleDc = 0 ; //以该线为反位线的道岔记录下来，为双线图绘制尖轨芯轨用
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDc) ;
							CString strDcName ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), strDcName) ;
							int iUpOrDownDc = 0 ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_upordown"), iUpOrDownDc) ;

							if (iHandleDW!=0) //还有不是和DC反位相连的线，比如轨道终点
							{
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_vertex"), iLineVertexDW) ;
								strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d and sxt_line_vertex=%d"), iHandleDW, iLineVertexDW) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_line_vertex_x2"), pt_2d_start_dw.x) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_line_vertex_y2"),  pt_2d_start_dw.y) ;
								strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d and sxt_line_vertex=%d"), iHandleDW, iLineVertexDW+1) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_line_vertex_x2"), pt_2d_end_dw.x) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_line_vertex_y2"),  pt_2d_end_dw.y) ;
								//2条平行线，一条距定位2.5,，另一条是距定位线1.5
								AcGeLineSeg2d lineSeg2dDwSrc(pt_2d_start_dw, pt_2d_end_dw), lineSeg2dDw25, lineSeg2dDw15 ;

								//begin 尖轨、芯轨牵引数量
								double dAngleDc = 0 ; //道岔块旋转角度
								AcDbEntity* pEntDc = NULL ;
								if (m_mysys.OpenAcDbEntity(nHandleDc, pEntDc, AcDb::kForRead))
								{
									AcDbBlockReference* pRefDc = AcDbBlockReference::cast(pEntDc) ;
									dAngleDc = pRefDc->rotation() ;
									pEntDc->close() ;
								}
								//end 尖轨、芯轨牵引数量

								AcGePoint2d pt_2d_ext ;
								AcGeVector2d vecDw = pt_2d_end_dw-pt_2d_start_dw ;
								//用于判断道岔垂直方向，不能简单根据*** int iDcDirectV = ((pt_2d_pre.y-pt_2d_cur.y)>0.5?0:1) ; //道岔垂直方向（FW向上0、向下1）***
								//根据以下算法：道岔旋转角，90~270度之间，向下								
								int iDcDirectV =0 ; //道岔垂直方向（FW向上0、向下1）
								if (dAngleDc>PI/2&&dAngleDc<(PI*3/2))
								{
									iDcDirectV = 1 ;
								}
								if (iDcDirectV==0) //反位端朝上,允许一定误差，这个0.5可再议
								{
									//反位端相交的所在多段线及段序号
									m_mysys.GetParallelLine(lineSeg2dDwSrc, lineSeg2dDw25, dDc25, TRUE) ;
									m_mysys.GetParallelLine(lineSeg2dDwSrc, lineSeg2dDw15, dDc15, FALSE) ;

									lineSeg2dParallelUp.intersectWith(lineSeg2dDw25, pt_2d_up) ;
									lineSeg2dParallelDown.intersectWith(lineSeg2dDw15, pt_2d_down) ;

									pt2dArrayUp.append(pt_2d_up) ;
									dDrawOptionUps.append(dWidthUp) ;
									strXdataUp.AppendFormat(_T("A:%d,B:%d,C:%d;"), pt2dArrayUp.length()-1, iIndexVertex, iIndexVertexOld) ;
									pt2dArrayDown.append(pt_2d_down) ;
									dDrawOptionDowns.append(dWidthDown) ;
									strXdataDown.AppendFormat(_T("A:%d,B:%d,C:%d;"), pt2dArrayDown.length()-1, iIndexVertex, iIndexVertexOld) ;

									//Dc反位线和定位线平行的一段
									pt_2d_ext =pt_2d_down ;
									pt_2d_ext.x +=dLenDcPlus ;
									//AcGeVector2d vecDw = pt_2d_end_dw-pt_2d_start_dw ;
									pt_2d_ext.rotateBy(vecDw.angle(), pt_2d_down) ;								
									pt2dArrayDown.append(pt_2d_ext) ;	
									dDrawOptionDowns.append(dWidthDown) ;
									strXdataDown.AppendFormat(_T("A:%d,B:%d,C:%d;"), pt2dArrayDown.length()-1, iIndexVertex, iIndexVertexOld) ;
									strXdataDownForZZJ.AppendFormat(_T("%d-1-%d-%d-%d;"), nHandleDc, iIndexVertex, iHandleDW, iLineVertexDW) ;
								}
								else //反位端朝下，（反位端在水平线上，视同反位朝下）
								{
									m_mysys.GetParallelLine(lineSeg2dDwSrc, lineSeg2dDw25, dDc25, FALSE) ;
									m_mysys.GetParallelLine(lineSeg2dDwSrc, lineSeg2dDw15, dDc15, TRUE) ;

									lineSeg2dParallelUp.intersectWith(lineSeg2dDw15, pt_2d_up) ;
									lineSeg2dParallelDown.intersectWith(lineSeg2dDw25, pt_2d_down) ;

									pt2dArrayUp.append(pt_2d_up) ;
									dDrawOptionUps.append(dWidthUp) ;
									strXdataUp.AppendFormat(_T("A:%d,B:%d,C:%d;"), pt2dArrayUp.length()-1, iIndexVertex, iIndexVertexOld) ;
									pt2dArrayDown.append(pt_2d_down) ;
									dDrawOptionDowns.append(dWidthDown) ;
									strXdataDown.AppendFormat(_T("A:%d,B:%d,C:%d;"), pt2dArrayDown.length()-1, iIndexVertex, iIndexVertexOld) ;

									//Dc反位线和定位线平行的一段
									pt_2d_ext = pt_2d_up ;
									pt_2d_ext.x +=dLenDcPlus ;
									//AcGeVector2d vecDw = pt_2d_end_dw-pt_2d_start_dw ;
									pt_2d_ext.rotateBy(vecDw.angle(), pt_2d_up) ;
									pt2dArrayUp.append(pt_2d_ext) ;
									dDrawOptionUps.append(dWidthUp) ;
									strXdataUp.AppendFormat(_T("A:%d,B:%d,C:%d;"), pt2dArrayUp.length()-1, iIndexVertex, iIndexVertexOld) ;
									strXdataUpForZZJ.AppendFormat(_T("%d-0-%d-%d-%d;"), nHandleDc, iIndexVertex, iHandleDW, iLineVertexDW) ;
								}

								//begin draw zzj
								int iDcDirectH = 1 ; //道岔FW水平朝向，0朝右，1，朝左
								AcGeLineSeg2d line2d(pt_2d_ext,iDcDirectV==0?pt_2d_down:pt_2d_up) ;									
								AcGePoint2d pt_2d_extcen =line2d.midPoint() ;

								AcGePoint2d pt_2d_extcen_mid = pt_2d_extcen ;
								pt_2d_extcen_mid.y+=(iDcDirectV==0?dDc15:-dDc15) ;
								pt_2d_extcen_mid.rotateBy(vecDw.angle(), pt_2d_extcen) ;

								//映射到双线的一边
								AcGePoint2d pt_2d_extcen_jg = pt_2d_extcen_mid ;
								pt_2d_extcen_jg.y +=(iUpOrDownDc==0?-dDistBetweenSxHalf:dDistBetweenSxHalf); 
								pt_2d_extcen_jg.rotateBy(vecDw.angle(), pt_2d_extcen) ;

								//道岔名称
								AcGePoint2d pt_2d_dcname = pt_2d_extcen_jg ;
								pt_2d_dcname.y+=(iUpOrDownDc==0?-2:2); 
								pt_2d_dcname.rotateBy(vecDw.angle(), pt_2d_extcen_jg) ;
								AcGePoint3d pt_3d_dcname(pt_2d_dcname.x, pt_2d_dcname.y,0) ;
								m_mysys.AddText(strDcName, pt_3d_dcname, pt_3d_dcname, AcDb::kTextCenter, AcDb::kTextVertMid, 2.5, 0.8, vecDw.angle(), 3) ;
								//end draw zzj
							}
							else
							{
								pt2dArrayUp.append(pt_2d_up) ;
								dDrawOptionUps.append(dWidthUp) ;
								strXdataUp.AppendFormat(_T("A:%d,B:%d,C:%d;"), pt2dArrayUp.length()-1, iIndexVertex, iIndexVertexOld) ;
								pt2dArrayDown.append(pt_2d_down) ;
								dDrawOptionDowns.append(dWidthDown) ;
								strXdataDown.AppendFormat(_T("A:%d,B:%d,C:%d;"), pt2dArrayDown.length()-1, iIndexVertex, iIndexVertexOld) ;
							}
						}
						else
						{
							AcGeLineSeg2d lineSeg2dSrc1(pt_2d_pre, pt_2d_cur), lineSeg2dSrc2(pt_2d_cur, pt_2d_back) ;	
							//acutPrintf(_T("\nlinesrc1(%.3f,%.3f)(%.3f,%.3f) linesrc2(%.3f,%.3f)(%.3f,%.3f)"), lineSeg2dSrc1.startPoint().x,lineSeg2dSrc1.startPoint().y,lineSeg2dSrc1.endPoint().x,lineSeg2dSrc1.endPoint().y,lineSeg2dSrc2.startPoint().x,lineSeg2dSrc2.startPoint().y, lineSeg2dSrc2.endPoint().x,lineSeg2dSrc2.endPoint().y );

							AcGeLineSeg2d lineSeg2dParallelUp1, lineSeg2dParallelDown1, lineSeg2dParallelUp2, lineSeg2dParallelDown2 ;
							m_mysys.GetParallelLine(lineSeg2dSrc1, lineSeg2dParallelUp1, dDistBetweenSxHalf ) ;
							m_mysys.GetParallelLine(lineSeg2dSrc1, lineSeg2dParallelDown1, dDistBetweenSxHalf, FALSE) ;
							m_mysys.GetParallelLine(lineSeg2dSrc2, lineSeg2dParallelUp2, dDistBetweenSxHalf ) ;
							m_mysys.GetParallelLine(lineSeg2dSrc2, lineSeg2dParallelDown2, dDistBetweenSxHalf, FALSE) ;				

							m_mysys.IntersectWith(&lineSeg2dParallelUp1, &lineSeg2dParallelUp2, pt_2d_up, AcDb::kExtendBoth) ;
							m_mysys.IntersectWith(&lineSeg2dParallelDown1, &lineSeg2dParallelDown2, pt_2d_down, AcDb::kExtendBoth) ;

							if (lineSeg2dSrc1.isParallelTo(lineSeg2dSrc2, tol))
							{
								pt_2d_up = lineSeg2dParallelUp1.endPoint() ;
								pt_2d_down = lineSeg2dParallelDown1.endPoint() ;
							}
							//acutPrintf(_T("\nintersect-up(%.3f,%.3f)intersect-down(%.3f,%.3f)"), pt_2d_up.x,pt_2d_up.y,pt_2d_down.x,pt_2d_down.y );

							pt2dArrayUp.append(pt_2d_up) ;
							dDrawOptionUps.append(dWidthUp) ;
							strXdataUp.AppendFormat(_T("A:%d,B:%d,C:%d;"), pt2dArrayUp.length()-1, iIndexVertex, iIndexVertexOld) ;
							pt2dArrayDown.append(pt_2d_down) ;
							dDrawOptionDowns.append(dWidthDown) ;
							strXdataDown.AppendFormat(_T("A:%d,B:%d,C:%d;"), pt2dArrayDown.length()-1, iIndexVertex, iIndexVertexOld) ;
						}

						cSetVertex.MoveNext() ;
						iIndexVertex++ ;
						iDrawOptionPre = iDrawOptionCur ;
					}
					cSetVertex.Close() ;
				}

// 				AcDbObjectId objIdUp = m_mysys.AddAcDbPolyLine(pt2dArrayUp, dDrawOptionUps, 7) ;				
// 				AcDbObjectId objIdDown = m_mysys.AddAcDbPolyLine(pt2dArrayDown, dDrawOptionDowns, 7) ;
				AcDbObjectId objIdUp = m_mysys.AddAcDbPolyLine(pt2dArrayUp, 0.5, 7) ;				
				AcDbObjectId objIdDown = m_mysys.AddAcDbPolyLine(pt2dArrayDown, 0.5, 7) ;

				//set xdata
				strXdataUp.TrimRight(_T(";")) ;
				strXdataDown.TrimRight(_T(";")) ;
				strXdataUpForZZJ.TrimRight(_T(";")) ;
				strXdataDownForZZJ.TrimRight(_T(";")) ;
				AcDbEntity * pEnt = NULL ;
				if (m_mysys.OpenAcDbEntity(objIdUp, pEnt))
				{
					m_mysys.SetXdata(pEnt, _T("CloneFrom"), iHandle) ;
					m_mysys.SetXdata(pEnt, _T("UPDOWN"), 1) ;
					m_mysys.SetXdata(pEnt, _T("VertexInfo"), strXdataUp) ;
					if (!strXdataUpForZZJ.IsEmpty())
					{
						m_mysys.SetXdata(pEnt, _T("ForZZJ"), strXdataUpForZZJ) ;
					}
					pEnt->close() ;
				}
				if (m_mysys.OpenAcDbEntity(objIdDown, pEnt))
				{
					m_mysys.SetXdata(pEnt, _T("CloneFrom"), iHandle) ;
					m_mysys.SetXdata(pEnt, _T("UPDOWN"), 0) ;
					m_mysys.SetXdata(pEnt, _T("VertexInfo"), strXdataDown) ;
					if (!strXdataDownForZZJ.IsEmpty())
					{
						m_mysys.SetXdata(pEnt, _T("ForZZJ"), strXdataDownForZZJ) ;
					}
					pEnt->close() ;
				}

				cSet.MoveNext() ;
			}
			cSet.Close() ;
		}
		//end 绘制线

		//begin 绘制封闭的线
		strSql.Format(_T("select distinct sxt_line_handle from sxt_line where sxt_line_type=11")) ;
		if (cSet.Open(strSql))
		{
			while(!cSet.IsEOF())
			{
				long  nHandle = 0 ;
				cSet.GetFieldValue(_T("sxt_line_handle"), nHandle) ;
				strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d order by sxt_line_vertex asc"), nHandle) ;
				CDoubleArray dArrayX, dArrayY ;
				int iRsCount = m_RecordSet.GetRecordValue(strSql, _T("sxt_line_vertex_x2"), dArrayX) ;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_line_vertex_y2"), dArrayY) ;
				if (iRsCount>2)
				{
					AcGePoint3dArray pt3dArray ;
					for (int i=0; i<iRsCount; i++)
					{
						AcGePoint3d pt_3d_tmp(dArrayX.GetAt(i), dArrayY.GetAt(i), 0) ;
						pt3dArray.append(pt_3d_tmp) ;
					}
					m_mysys.AddAcDbPolyLine(pt3dArray, 0.2, 7, TRUE) ;
				}

				cSet.MoveNext() ;
			}
		}
		//end 绘制封闭的线

		//begin 信号机
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type=550")) ;
		if (cSet.Open(strSql))
		{
			while(!cSet.IsEOF())
			{
				long nHandle = 0 ;
				cSet.GetFieldValue(_T("sxt_ent_handle"), nHandle) ;
				double dEntX = 0.000, dEntY = 0.000 ;				
				cSet.GetFieldValue(_T("sxt_ent_x2"), dEntX) ;
				cSet.GetFieldValue(_T("sxt_ent_y2"), dEntY) ;
				CString strBlkName ; 
				cSet.GetFieldValue(_T("sxt_ent_block"), strBlkName) ;
				AcDbEntity * pEnt = NULL ;
				if (m_mysys.OpenAcDbEntity(nHandle, pEnt))
				{
					AcDbBlockReference *pRef = AcDbBlockReference::cast(pEnt) ;
					double dAngle = pRef->rotation() ;
					AcGePoint3d pt_3d_old = pRef->position() ;
					AcDbBlockReference *pRefNew = AcDbBlockReference::cast(pRef->clone()) ;
					m_mysys.SetXdata(pRefNew, _T("CloneFrom"), nHandle) ;
					m_mysys.SetXdata(pRefNew,_T("BELONG_ENT"),nHandle);
					AcGePoint3d pt_3d_new(dEntX, dEntY, 0) ;
					AcGeMatrix3d matrix ;
					AcGeVector3d vec = pt_3d_new - pt_3d_old ;
					matrix.setToTranslation(vec) ;
					pRefNew->transformBy(matrix) ; 
					//begin 在双线图中，信号机基点不能在中线上，要至少上/下移2.5
					AcGePoint3d pt_3d_new2(pt_3d_new) ;
					AcGePoint3d pt_3d_xh;
					int iMarkLoc = strBlkName.Find(_T("_Y_")) ;//正向信号机（Y 立于右侧）, 逆向信号机(Z 立于左侧)
					//角度在90度~270度之间是倒置了
					if (dAngle>PI/2&&dAngle<PI*3/2)
					{
						pt_3d_new2.x=pt_3d_new2.x+(iMarkLoc==-1?dDistBetweenSxHalf:-dDistBetweenSxHalf) ;
						pt_3d_xh.x=pt_3d_new.x+(iMarkLoc==-1?2*dDistBetweenSxHalf:-2*dDistBetweenSxHalf) ;
						pt_3d_xh.y=pt_3d_new.y+(iMarkLoc==-1?6:-6) ;
					}
					else
					{
						pt_3d_new2.x=pt_3d_new2.x+(iMarkLoc==-1?-dDistBetweenSxHalf:dDistBetweenSxHalf) ;
						pt_3d_xh.x=pt_3d_new.x+(iMarkLoc==-1?-2*dDistBetweenSxHalf:2*dDistBetweenSxHalf) ;
						pt_3d_xh.y=pt_3d_new.y+(iMarkLoc==-1?-6:6) ;
					}
					pt_3d_new2.rotateBy(PI/2, AcGeVector3d::kZAxis, pt_3d_new) ;
					vec = pt_3d_new2 - pt_3d_new ;
					matrix.setToTranslation(vec) ;
					pRefNew->transformBy(matrix) ; 
					//m_mysys.InsertBlock()
					//插入信号机FXX
					AcDbBlockReference *pRef2=NULL;
					CString strblkname=_T("DT_BOX_RECT_0");
					//pt_3d_header.y+=31;
					if (m_mysys.InsertBlock(pRef2, strblkname, pt_3d_xh,FALSE,0,TRUE))
					{
						CString strTag=_T("TYPE");
						CString str_name=_T("FXX");
						m_mysys.SetAttValue(pRef2,strTag,str_name,FALSE);
						pRef2->transformBy(matrix);
						pRef2->close() ;
						pRef2 = NULL ;
					}
					//end 在双线图中，信号机基点不能在中线上，要至少上/下移2.5
					pRefNew->setLayer(_T("sxt")) ;
					m_mysys.AddEntityToDb(pRefNew) ;
					pRefNew->close() ;
					pEnt->close() ;
				}

				cSet.MoveNext() ;
			}

			cSet.Close() ;
		}
		//end 信号机

		/*
		//begin 绝缘节
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type\\100=2")) ; //整除为2的
		if (cSet.Open(strSql))
		{
			while(!cSet.IsEOF())
			{
				long nHandle = 0 ;
				cSet.GetFieldValue(_T("sxt_ent_handle"), nHandle) ;
				int iEntType = 0 ;
				cSet.GetFieldValue(_T("sxt_ent_type"), iEntType) ;

				//计算双线中线绝缘节位置时，应以平面图绝缘节和多段线交点作为源数据为宜
				double dEntX = 0.000, dEntY = 0.000 ;				
				cSet.GetFieldValue(_T("sxt_ent_online_intersectx2"), dEntX) ;
				cSet.GetFieldValue(_T("sxt_ent_online_intersecty2"), dEntY) ;

				///begin 绝缘节所在线段信息	
				AcGeVector2d vecOnLine = GetAngleOfJyjOnLine(nHandle) ;
				double dAngleOnLine = 0 ;
				dAngleOnLine = vecOnLine.angle() ;
				//end  绝缘节所在线段信息

				//begin 复制平面图中的绝缘节（2份）并经过移动转换到双线中
				AcDbEntity * pEnt = NULL ;
				if (m_mysys.OpenAcDbEntity(nHandle, pEnt))
				{
					AcDbBlockReference *pRef = AcDbBlockReference::cast(pEnt) ;
					double dAngle = pRef->rotation() ;
					AcGePoint3d pt_3d_old = pRef->position() ;
					AcDbBlockReference *pRefNew1 = AcDbBlockReference::cast(pRef->clone()) ;
					AcDbBlockReference *pRefNew2 = AcDbBlockReference::cast(pRef->clone()) ;
					AcGePoint3d pt_3d_new(dEntX, dEntY, 0) ;//双线中间线和绝缘节交点位置
					AcGeMatrix3d matrix ;
					AcGeVector3d vec = pt_3d_new - pt_3d_old ;
					matrix.setToTranslation(vec) ;					
					pRefNew1->transformBy(matrix) ; 
					pRefNew2->transformBy(matrix) ; 
					//begin 在双线图中，绝缘节基点不能在中线上，上/下移2.5
					AcGePoint3d pt_3d_new1(pt_3d_new), pt_3d_new2(pt_3d_new) ;
					pt_3d_new1.x = pt_3d_new.x+dDistBetweenSxHalf ;
					pt_3d_new2.x = pt_3d_new.x-dDistBetweenSxHalf ;
					pt_3d_new1.rotateBy(PI/2+dAngleOnLine, AcGeVector3d::kZAxis, pt_3d_new) ;
					pt_3d_new2.rotateBy(PI/2+dAngleOnLine, AcGeVector3d::kZAxis, pt_3d_new) ;
					vec = pt_3d_new1 - pt_3d_new ;
					matrix.setToTranslation(vec) ;
					pRefNew1->transformBy(matrix) ; 
					vec = pt_3d_new2 - pt_3d_new ;
					matrix.setToTranslation(vec) ;
					pRefNew2->transformBy(matrix) ; 
					//end 在双线图中，绝缘节基点不能在中线上，上/下移2.5
					pRefNew1->setColorIndex(1) ;
					pRefNew1->setLayer(_T("sxt")) ;
					m_mysys.AddEntityToDb(pRefNew1) ;
					m_mysys.SetXdata(pRefNew1, _T("CloneFrom"), nHandle) ;
					m_mysys.SetXdata(pRefNew1, _T("UPDOWN"), 1) ;
					pRefNew1->close() ;
					pRefNew2->setColorIndex(3) ;
					pRefNew2->setLayer(_T("sxt")) ;
					m_mysys.AddEntityToDb(pRefNew2) ;
					m_mysys.SetXdata(pRefNew2, _T("CloneFrom"), nHandle) ;
					m_mysys.SetXdata(pRefNew2, _T("UPDOWN"), 0) ;
					pRefNew2->close() ;
					pEnt->close() ;
				}
				//end 复制平面图中的绝缘节（2份）并经过移动转换到双线中

				cSet.MoveNext() ;
			}

			cSet.Close() ;
		}
		//end 绝缘节
		*/

		//begin 其他
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type=500")) ;
		if (cSet.Open(strSql))
		{
			while(!cSet.IsEOF())
			{
				long nHandle = 0 ;
				cSet.GetFieldValue(_T("sxt_ent_handle"), nHandle) ;

				double dEntX = 0.000, dEntY = 0.000 ;				
				cSet.GetFieldValue(_T("sxt_ent_x2"), dEntX) ;
				cSet.GetFieldValue(_T("sxt_ent_y2"), dEntY) ;
				CString strBlkName ; 
				cSet.GetFieldValue(_T("sxt_ent_block"), strBlkName) ;
				CString strEntName;
				cSet.GetFieldValue(_T("sxt_ent_name"),strEntName);
				AcDbEntity * pEnt = NULL ;
				if (m_mysys.OpenAcDbEntity(nHandle, pEnt))
				{
					AcDbBlockReference *pRef = AcDbBlockReference::cast(pEnt) ;
					double dAngle = pRef->rotation() ;
					AcGePoint3d pt_3d_old = pRef->position() ;
					AcDbBlockReference *pRefNew = AcDbBlockReference::cast(pRef->clone()) ;
					m_mysys.SetXdata(pRefNew, _T("CloneFrom"), nHandle) ;
					m_mysys.SetXdata(pRefNew,_T("BELONG_ENT"),nHandle);
					AcGePoint3d pt_3d_new(dEntX, dEntY, 0) ;
					AcGePoint3d pt_3d_kad=pt_3d_new;
					//pt_3d_kad.y-=10;
					AcGeMatrix3d matrix ;
					AcGeVector3d vec = pt_3d_new - pt_3d_old ;
					matrix.setToTranslation(vec) ;
					pRefNew->transformBy(matrix) ; 
					//计轴、LDR、VB、IB、道岔需要添加分线箱
					CString strarray[6]={_T("DT_JZ_000"),_T("DT_JZ_100"),_T("DT_JZ_200"),_T("DT_DHX_000"),_T("DT_YDQ_000"),_T("DT_YDQ_200")};
					for(int i=0;i<6;i++)
					{
						if(strBlkName==strarray[i])
						{
							//添加KAD，需要判断是在上行线路还是下行线路上
							//判断方法，查询数据库中设备栏，600是上栏，610是下栏
							//m_mysys.InsertBlock()
							//插入信号机FXX
							AcDbBlockReference *pRef2=NULL;
							CString strblkname=_T("DT_BOX_RECT_0");
							//在设备信息中查询设备名称
							strSql.Format(_T("select * from sxt_ent where sxt_ent_name='%s'  and (sxt_ent_type=600 or sxt_ent_type=610)"),strEntName.GetString());
							//strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=923"));
							int i_type=0;
						    m_RecordSet.GetRecordValue(strSql,_T("sxt_ent_type"),i_type);
							//cSet.GetFieldValue(_T("sxt_ent_type"),i_type);
							if(i_type==600)
							{
								pt_3d_kad.y+=10;
							}
							else if(i_type==610)
							{
								pt_3d_kad.y-=10;
							}
							//pt_3d_header.y+=31;
							if (m_mysys.InsertBlock(pRef2, strblkname, pt_3d_kad,FALSE,0,TRUE))
							{
								CString strTag=_T("TYPE");
								CString str_name=_T("KAD");
								m_mysys.SetAttValue(pRef2,strTag,str_name,FALSE);
								//pRef2->transformBy(matrix);
								pRef2->close() ;
								pRef2 = NULL ;
							}
						}
					}
					pRefNew->setLayer(_T("sxt")) ;
					m_mysys.AddEntityToDb(pRefNew) ;
					pRefNew->close() ;
					pEnt->close() ;
				}

				cSet.MoveNext() ;
			}

			cSet.Close() ;
		}
		//end 其他

		//begin 绘制跳线
		//this->DrawJumpLine() ;		
		//end 绘制跳线

		//begin 绘制 信号楼(20150605：双线不绘制警冲标）
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type\\100=3")) ;
		if (cSet.Open(strSql))
		{
			while(!cSet.IsEOF())
			{
				long nHandle = 0 ;
				cSet.GetFieldValue(_T("sxt_ent_handle"), nHandle) ;

				double dEntX = 0.000, dEntY = 0.000 ;				
				cSet.GetFieldValue(_T("sxt_ent_x2"), dEntX) ;
				cSet.GetFieldValue(_T("sxt_ent_y2"), dEntY) ;
				CString strBlkName ; 
				cSet.GetFieldValue(_T("sxt_ent_block"), strBlkName) ;
				AcDbEntity * pEnt = NULL ;
				if (m_mysys.OpenAcDbEntity(nHandle, pEnt))
				{
					AcDbBlockReference *pRef = AcDbBlockReference::cast(pEnt) ;
					double dAngle = pRef->rotation() ;
					AcGePoint3d pt_3d_old = pRef->position() ;
					AcDbBlockReference *pRefNew = AcDbBlockReference::cast(pRef->clone()) ;
					AcGePoint3d pt_3d_new(dEntX, dEntY, 0) ;
					AcGeMatrix3d matrix ;
					AcGeVector3d vec = pt_3d_new - pt_3d_old ;
					matrix.setToTranslation(vec) ;
					pRefNew->transformBy(matrix) ; 
					pRefNew->setLayer(_T("sxt")) ;
					m_mysys.AddEntityToDb(pRefNew) ;
					pRefNew->close() ;
					pEnt->close() ;
				}

				cSet.MoveNext() ;
			}

			cSet.Close() ;
		}
		//end 绘制警冲标 信号楼

		//begin 绘制距离信号楼距离
		AcGePoint3d pt_3d_min_dwg(0,0,0), pt_3d_max_dwg(0,0,0) ; //先取得当前图纸的范围,以便确定绘制距信号楼距离的坐标
		m_mysys.GetCurExtents(pt_3d_min_dwg, pt_3d_max_dwg) ;
	   //测试范围获取是否正确
		//CLineUtil::Add(pt_3d_min_dwg,pt_3d_max_dwg);
		double dY_Dist[2][2] = {{0,0}, {0,0} } ;
// 		dY_Dist[0][0] = pt_3d_max_dwg.y+80 ;
// 		dY_Dist[0][1] = dY_Dist[0][0]+13 ;
// 		dY_Dist[1][0] = pt_3d_orign.y-(dY_Dist[0][0]-pt_3d_orign.y) ;
// 		dY_Dist[1][1] = dY_Dist[1][0]-13 ; 
		//平面图的基点为（0,143.5）
		dY_Dist[0][0]=143.5*dScaleY+pt_3d_orign.y+200-31;
		dY_Dist[0][1]=dY_Dist[0][0]+13;
		dY_Dist[1][0]=143.5*dScaleY+pt_3d_orign.y-200+31;
		dY_Dist[1][1]=dY_Dist[1][0]-13;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type\\100=6")) ; //整除为6
		if (cSet.Open(strSql))
		{
			while(!cSet.IsEOF())
			{
				int iType = 0 ;
				cSet.GetFieldValue(_T("sxt_ent_type"), iType) ;
				CString strLoc ;
				cSet.GetFieldValue(_T("sxt_ent_name"), strLoc) ;
				double dX2 = 0 ;
				cSet.GetFieldValue(_T("sxt_ent_x2"), dX2) ;

				int i1 = iType%10 ; //个位
				int i10 = iType%100/10 ; //十位
                
				//AcGePoint3d pt_3d_insert_text(dX2-1.5, dY_Dist[i10][i1]+(i10==0?1:-1)*(i1==0?6.5:11), 0) ;
				AcGePoint3d pt_3d_insert_text(dX2-1.5, dY_Dist[i10][i1]+(i10==0?1:-1)*(i1==0?6.5:9.5), 0) ;

				m_mysys.AddText(strLoc, pt_3d_insert_text, pt_3d_insert_text, AcDb::kTextCenter, AcDb::kTextVertMid, 3, 0.55, PI/2) ;

				cSet.MoveNext() ;
			}

			cSet.Close() ;
		}

		//插入表头、绘制行线
		strSql.Format(_T("select min(sxt_ent_x2) as minX, max(sxt_ent_x2) as maxX from sxt_ent where sxt_ent_type\\100=6")) ; //整除为6
		double dMinX = 0, dMaxX = 0 ;
		m_RecordSet.GetRecordValue(strSql, _T("minX"), dMinX) ;
		m_RecordSet.GetRecordValue(strSql, _T("maxX"), dMaxX) ;
		AcDbBlockReference* pRef = NULL ;
		AcGePoint3d pt_3d_header(dMinX-30,0,0) ;
		CString strBlkName ; 
		for (int i=0; i<2; i++)
		{
			//表头
			pt_3d_header.y = dY_Dist[i][0] ;
			strBlkName.Format(_T("DT_LOC_HEADER_%d00"), i) ;
			if (m_mysys.InsertBlock(pRef, strBlkName, pt_3d_header))
			{
				pRef->close() ;
				pRef = NULL ;
			}
			//线
			AcGePoint3d pt_3d_start(pt_3d_header.x+7.5, 0, 0), pt_3d_end(dMaxX+30, 0, 0) ;
			for (int j=0; j<2; j++)
			{
				pt_3d_start.y = dY_Dist[i][j] ;
				pt_3d_end.y = dY_Dist[i][j] ;
				m_mysys.AddAcDbLine(pt_3d_start, pt_3d_end) ;
				if (j==1)
				{
					pt_3d_start.y = dY_Dist[i][j]+(i==0?1:-1)*18 ;
					pt_3d_end.y = dY_Dist[i][j]+(i==0?1:-1)*18 ;
					m_mysys.AddAcDbLine(pt_3d_start, pt_3d_end) ;
				}
			}

		}
		//end 绘制距离信号楼距离
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误!in Drawsxt_DT"));
	}

	m_pDb->setClayer(objIdOldLayer) ;

	return 0;
}
// 绘制双线图（地铁）
int CZnSxt::DrawSxt_DT(double dScaleX, double dScaleY, const AcGePoint3d & pt_3d_orign, double dLenDcPlus, double dDistBetweenSX)
{
	AcDbObjectId  idTextStyleTitle, idTextStyleContent, idTextStyleChs;
	idTextStyleTitle = m_mysys.CreateTextStyleByFileName(_T("zhffontTitle"), _T("txt.shx"), _T("hztxt2.shx"), 4, 0.7) ;	
	idTextStyleContent = m_mysys.CreateTextStyleByFileName(_T("zhffontContent"), _T("txt.shx"), _T("hztxt2.shx"), 3, 0.7) ;
	idTextStyleChs = m_mysys.CreateTextStyle(_T("CHINESE"), _T("Times New Roman"), _T(""), 3, 0.55) ;
	double dDistBetweenSxHalf = dDistBetweenSX/2 ;
	double dDc25 = dDistBetweenSxHalf, dDc15 = dDistBetweenSX*0.3 ; //道岔双线绘制时
	AcDbObjectId objIdOldLayer = m_pDb->clayer() ;
	//m_mysys.Set_Layer(_T(""), _T("sxt")) ;
	AcGeTol tol  ; 
	tol.setEqualVector(0.00001) ;
	try
	{
		CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;

		//begin 根据指定基点、缩放比值，计算实体（信号机、道岔、轨道线）坐标值
		CString strTables[2] ={ _T("sxt_ent"), _T("sxt_line")} ;
		for (int iTbIndex=0; iTbIndex<2; iTbIndex++)
		{
			strSql.Format(_T("select * from %s"), strTables[iTbIndex]) ;
			CString strField1[2], strField2[2], strField3[2], strField4[2] ;

			for (int i=0; i<2; i++)
			{
				if (iTbIndex==0)
				{
					strField1[i].Format(_T("%s_x%d"), strTables[iTbIndex], i+1) ;
					strField2[i].Format(_T("%s_y%d"), strTables[iTbIndex], i+1) ;
					strField3[i].Format(_T("%s_online_intersectx%d"), strTables[iTbIndex], i+1) ;
					strField4[i].Format(_T("%s_online_intersecty%d"), strTables[iTbIndex], i+1) ;
				}
				else if (iTbIndex==1)
				{
					strField1[i].Format(_T("%s_vertex_x%d"), strTables[iTbIndex], i+1) ;
					strField2[i].Format(_T("%s_vertex_y%d"), strTables[iTbIndex], i+1) ;
				}
			}

			if(cSet.Open(strSql))
			{
				while(!cSet.IsEOF())
				{
					double dEntX = 0.000, dEntY = 0.000, dEntX1 = 0.000, dEntY1 = 0.000;		

					cSet.GetFieldValue(strField1[0], dEntX) ;
					cSet.GetFieldValue(strField2[0], dEntY) ;
					dEntX = pt_3d_orign.x +dEntX*dScaleX ;
					dEntY = pt_3d_orign.y +dEntY*dScaleY ;

					if (iTbIndex==0)
					{
						cSet.GetFieldValue(strField3[0], dEntX1) ;
						cSet.GetFieldValue(strField4[0], dEntY1) ;
						dEntX1 = pt_3d_orign.x +dEntX1*dScaleX ;
						dEntY1 = pt_3d_orign.y +dEntY1*dScaleY ;
					}
					cSet.Edit() ;
					cSet.SetFieldValue(strField1[1], dEntX) ;
					cSet.SetFieldValue(strField2[1], dEntY) ;
					if (iTbIndex==0)
					{
						cSet.SetFieldValue(strField3[1], dEntX1) ;
						cSet.SetFieldValue(strField4[1], dEntY1) ;
					}
					cSet.Update() ;

					cSet.MoveNext() ;
				}

				cSet.Close() ;
			}
		}
		//end 根据指定基点、缩放比值，计算实体（信号机、道岔、轨道线）坐标值
		//begin 绘制线
		strSql.Format(_T("select distinct sxt_line_handle from sxt_line where sxt_line_type<>11")) ;
		if (cSet.Open(strSql))
		{
			while(!cSet.IsEOF())
			{
				long  iHandle = 0 ;
				cSet.GetFieldValue(_T("sxt_line_handle"), iHandle) ;
				//begin 读取该线信息for绘制轨道名称
				CString strGdName ;
				int iGdWriteLeftOrRight = -1 ; //轨道名称写在线左侧0还是右侧1
				AcDbEntity* pEntLine = NULL ;
				if (m_mysys.OpenAcDbEntity(iHandle, pEntLine, AcDb::kForRead))
				{
					if(m_mysys.GetXdata(pEntLine, _T("GUIDAONAME"), strGdName))
					{
						if (!strGdName.IsEmpty())
						{
							CString strLeftForS=_T("N"), strRightForS = _T("N") ;
							m_mysys.GetXdata(pEntLine, _T("LEFT_FORS"), strLeftForS) ;
							m_mysys.GetXdata(pEntLine, _T("RIGHT_FORS"), strRightForS) ;
							if (strLeftForS==_T("S"))
							{
								iGdWriteLeftOrRight = 0 ;
							}
							else if (strRightForS==_T("S"))
							{
								iGdWriteLeftOrRight = 1 ;
							}
							else if (strLeftForS==_T("F"))
							{
								iGdWriteLeftOrRight = 0 ;
							}
							else if (strRightForS==_T("F"))
							{
								iGdWriteLeftOrRight = 1 ;
							}
						}
					}
					pEntLine->close() ;
				}
				//end 读取该线信息for绘制轨道名称

				//确定该多段线的起点和终点是否都是DC的反位连接端

				AcGePoint2dArray pt2dArrayUp, pt2dArrayDown ;
				AcArray <double> dDrawOptionUps,dDrawOptionDowns ;
				//VertexInfo=A:0,B:0,C:0;A:1,B:0,C:0;A:2,B:1,C:-1;A:3,B:2,C:1;A:4,B:3,C:2
				//ABC后面数字表示：A，双线中线的顶点序号；B，原平面图线加上极性切割绝缘节后形成的顶点序号；C，原平面图线不考虑极性绝缘节的顶点序号
				CString strXdataUp, strXdataDown ; 
				CString strXdataUpForZZJ, strXdataDownForZZJ ; //记录形式（针对的是道岔FW线）：端头连接的道岔handle-道岔FW朝向（1，朝上；0，朝下）-FW端线的顶点vertex（起点或终点）-道岔所在DW线-道岔所在DW线vertex
				strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d order by sxt_line_vertex1"), iHandle) ;
				CADORecordset cSetVertex(&m_AdoDb) ;
				if (cSetVertex.Open(strSql))
				{
					int iNumVertex = cSetVertex.GetRecordCount() ;
					if (!cSetVertex.IsBOF())
					{
						cSetVertex.MoveFirst() ;
					}
					int iIndexVertex = 0 ;
					int iDrawOptionPre = 2222 ; //前一顶点的DrawOption
					while (!cSetVertex.IsEOF())
					{
						//acutPrintf(_T("\nhandle:%d,vetex:%d"), iHandle, iIndexVertex) ;
						AcGePoint2d pt_2d_up(0,0), pt_2d_down(0,0) ; //双线的上下2个点
						AcGePoint2d pt_2d_cur(0,0), pt_2d_pre(0,0), pt_2d_back(0,0) ; 

						int iIndexVertexOld = iIndexVertex ; //原顶点序号（未加入切割点）
						cSetVertex.GetFieldValue(_T("sxt_line_vertex"), iIndexVertexOld) ;

						cSetVertex.GetFieldValue(_T("sxt_line_vertex_x2"), pt_2d_cur.x) ;
						cSetVertex.GetFieldValue(_T("sxt_line_vertex_y2"), pt_2d_cur.y) ;

						//begin 线宽
						long nHandleVertex = 0 ; //该点对应的绝缘节或者道岔
						int iDrawOptionCur = -1 ; //当前顶点的DrawOption
						cSetVertex.GetFieldValue(_T("sxt_line_vertex_handle"), nHandleVertex) ;

						double dWidthUp = 0, dWidthDown = 0 ;

						int iPolar = 0 ;
						cSetVertex.GetFieldValue(_T("sxt_line_polar"), iPolar) ;
						if (iIndexVertex==(iNumVertex-1)) //最后一段极性取前一个顶点的值
						{
							cSetVertex.MovePrevious() ;
							cSetVertex.GetFieldValue(_T("sxt_line_polar"), iPolar) ;
							cSetVertex.MoveNext() ;
						}
						if (iPolar==-1) //上粗下细
						{
							dWidthUp = 0.5 ;
							dWidthDown = 0 ;							
						}
						else if (iPolar==1)
						{
							dWidthUp = 0 ;
							dWidthDown = 0.5 ;				
						}
						//end 线宽

						if (iIndexVertex!=0)//不是第一个点
						{
							cSetVertex.MovePrevious() ;
							cSetVertex.GetFieldValue(_T("sxt_line_vertex_x2"), pt_2d_pre.x) ;
							cSetVertex.GetFieldValue(_T("sxt_line_vertex_y2"), pt_2d_pre.y) ;
							cSetVertex.MoveNext() ;
						}
						if (iIndexVertex!=(iNumVertex-1)) //不是最后一个点
						{
							cSetVertex.MoveNext() ;
							cSetVertex.GetFieldValue(_T("sxt_line_vertex_x2"), pt_2d_back.x) ;
							cSetVertex.GetFieldValue(_T("sxt_line_vertex_y2"), pt_2d_back.y) ;
							cSetVertex.MovePrevious() ;
						}

						if (iIndexVertex==0)
						{
							AcGeLineSeg2d lineSeg2dSrc(pt_2d_cur, pt_2d_back) ;
							AcGeLineSeg2d lineSeg2dParallelUp, lineSeg2dParallelDown ;
							m_mysys.GetParallelLine(lineSeg2dSrc, lineSeg2dParallelUp, dDistBetweenSxHalf ) ;
							m_mysys.GetParallelLine(lineSeg2dSrc, lineSeg2dParallelDown, dDistBetweenSxHalf, FALSE) ;
							pt_2d_up = lineSeg2dParallelUp.startPoint() ;
							pt_2d_down = lineSeg2dParallelDown.startPoint() ;

							//begin 绘制轨道名称
							if (iGdWriteLeftOrRight==0)
							{
								AcGePoint2d pt_2d_gdname = lineSeg2dSrc.midPoint() ;
								AcGePoint3d pt_3dg_gdname(pt_2d_gdname.x, pt_2d_gdname.y, 0) ;
								AcGeVector2d vecLine = lineSeg2dSrc.direction() ;
								AcDbObjectId objIdGdName ;
								m_mysys.AddText(objIdGdName, strGdName, pt_3dg_gdname, pt_3dg_gdname, AcDb::kTextCenter, AcDb::kTextVertMid, 2.5, 0.8, vecLine.angle(), 4)  ;	
								m_mysys.SetXdata(objIdGdName, _T("TYPE"), (long)101) ; //双线图中轨道名称文本
								m_mysys.SetXdata(objIdGdName, _T("BELONG_ENT"), iHandle) ;
							}
							//end 绘制轨道名称

							//和该FW端相交的DW轨道线的信息
							AcGePoint2d pt_2d_start_dw, pt_2d_end_dw ;	
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d and sxt_ent_fwline_vertex=%d"), iHandle, iIndexVertex) ;
							long iHandleDW = 0 ;
							int iLineVertexDW = 0 ;							
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), iHandleDW) ;
							long nHandleDc = 0 ; //以该线为反位线的道岔记录下来，为双线图绘制尖轨芯轨用
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDc) ;
							CString strDcName ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), strDcName) ;
							int iUpOrDownDc = 0 ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_upordown"), iUpOrDownDc) ;
							if (iHandleDW!=0) //还有不是和DC反位相连的线，比如轨道起始
							{
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_vertex"), iLineVertexDW) ;
								strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d and sxt_line_vertex=%d"), iHandleDW, iLineVertexDW) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_line_vertex_x2"), pt_2d_start_dw.x) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_line_vertex_y2"),  pt_2d_start_dw.y) ;
								strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d and sxt_line_vertex=%d"), iHandleDW, iLineVertexDW+1) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_line_vertex_x2"), pt_2d_end_dw.x) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_line_vertex_y2"),  pt_2d_end_dw.y) ;
								//2条平行线，一条距定位2.5,，另一条是距定位线1.5
								AcGeLineSeg2d lineSeg2dDwSrc(pt_2d_start_dw, pt_2d_end_dw), lineSeg2dDw25, lineSeg2dDw15 ;

								//begin 尖轨、芯轨牵引数量
								//int iJgNum = 3, iXgNum = 2 ; 
								double dAngleDc = 0 ; //道岔块旋转角度
								AcDbEntity* pEntDc = NULL ;
								if (m_mysys.OpenAcDbEntity(nHandleDc, pEntDc, AcDb::kForRead))
								{
									AcDbBlockReference* pRefDc = AcDbBlockReference::cast(pEntDc) ;
									dAngleDc = pRefDc->rotation() ;
									pEntDc->close() ;
								}
								//end 尖轨、芯轨牵引数量

								AcGePoint2d pt_2d_ext ;
								AcGeVector2d vecDw = pt_2d_end_dw-pt_2d_start_dw ;
								//用于判断道岔垂直方向，不能简单根据*** int iDcDirectV = ((pt_2d_back.y-pt_2d_cur.y)>0.5?0:1) ; //道岔垂直方向（FW向上0、向下1）***
								//根据以下算法：道岔旋转角，90~270度之间，向下	
								int iDcDirectV =0 ; //道岔垂直方向（FW向上0、向下1）
								if (dAngleDc>PI/2&&dAngleDc<(PI*3/2))
								{
									iDcDirectV = 1 ;
								}
								if (iDcDirectV==0) //反位端朝上
								{
									//反位端相交的所在多段线及段序号
									m_mysys.GetParallelLine(lineSeg2dDwSrc, lineSeg2dDw25, dDc25, TRUE) ;
									m_mysys.GetParallelLine(lineSeg2dDwSrc, lineSeg2dDw15, dDc15, FALSE) ;

									lineSeg2dParallelUp.intersectWith(lineSeg2dDw25, pt_2d_up) ;
									lineSeg2dParallelDown.intersectWith(lineSeg2dDw15, pt_2d_down) ;

									//Dc反位线和定位线平行的一段
									pt_2d_ext =pt_2d_down ;
									pt_2d_ext.x -=dLenDcPlus ;
									pt_2d_ext.rotateBy(vecDw.angle(), pt_2d_down) ;								
									pt2dArrayDown.append(pt_2d_ext) ;		
									dDrawOptionDowns.append(dWidthDown) ;
									strXdataDown.AppendFormat(_T("A:%d,B:%d,C:%d;"), pt2dArrayDown.length()-1, iIndexVertex, iIndexVertexOld) ;
									strXdataDownForZZJ.AppendFormat(_T("%d-1-%d-%d-%d;"), nHandleDc, iIndexVertex, iHandleDW, iLineVertexDW) ;	
								}
								else //反位端朝下，（反位端在水平线上，视同反位朝下）
								{
									m_mysys.GetParallelLine(lineSeg2dDwSrc, lineSeg2dDw25, dDc25, FALSE) ;
									m_mysys.GetParallelLine(lineSeg2dDwSrc, lineSeg2dDw15, dDc15, TRUE) ;

									lineSeg2dParallelUp.intersectWith(lineSeg2dDw15, pt_2d_up) ;
									lineSeg2dParallelDown.intersectWith(lineSeg2dDw25, pt_2d_down) ;

									//Dc反位线和定位线平行的一段
									pt_2d_ext = pt_2d_up ;
									pt_2d_ext.x -=dLenDcPlus ;
									pt_2d_ext.rotateBy(vecDw.angle(), pt_2d_up) ;
									pt2dArrayUp.append(pt_2d_ext) ;
									dDrawOptionUps.append(dWidthUp) ;
									strXdataUp.AppendFormat(_T("A:%d,B:%d,C:%d;"), pt2dArrayUp.length()-1, iIndexVertex, iIndexVertexOld) ;
									strXdataUpForZZJ.AppendFormat(_T("%d-0-%d-%d-%d;"), nHandleDc, iIndexVertex, iHandleDW, iLineVertexDW) ;
								}

								//begin draw zzj
								int iDcDirectH =0 ; //道岔FW水平朝向，0朝右，1，朝左
								AcGeLineSeg2d line2d(pt_2d_ext,iDcDirectV==0?pt_2d_down:pt_2d_up) ;									
								AcGePoint2d pt_2d_extcen =line2d.midPoint() ;
								AcGePoint2d pt_2d_extcen_mid = pt_2d_extcen ;
								pt_2d_extcen_mid.y+=(iDcDirectV==0?dDc15:-dDc15) ;
								pt_2d_extcen_mid.rotateBy(vecDw.angle(), pt_2d_extcen) ;

								//映射到双线的一边
								AcGePoint2d pt_2d_extcen_jg = pt_2d_extcen_mid ;
								pt_2d_extcen_jg.y +=(iUpOrDownDc==0?-dDistBetweenSxHalf:dDistBetweenSxHalf); 
								pt_2d_extcen_jg.rotateBy(vecDw.angle(), pt_2d_extcen) ;

								//道岔名称
								AcGePoint2d pt_2d_dcname = pt_2d_extcen_jg ;
								pt_2d_dcname.y+=(iUpOrDownDc==0?-2:2); 
								pt_2d_dcname.rotateBy(vecDw.angle(), pt_2d_extcen_jg) ;
								AcGePoint3d pt_3d_dcname(pt_2d_dcname.x, pt_2d_dcname.y,0) ;
								m_mysys.AddText(strDcName, pt_3d_dcname, pt_3d_dcname, AcDb::kTextCenter, AcDb::kTextVertMid, 2.5, 0.8, vecDw.angle(), 3) ;
								//end draw zzj
							}

							pt2dArrayUp.append(pt_2d_up) ;
							dDrawOptionUps.append(dWidthUp) ;
							strXdataUp.AppendFormat(_T("A:%d,B:%d,C:%d;"), pt2dArrayUp.length()-1, iIndexVertex, iIndexVertexOld) ;
							pt2dArrayDown.append(pt_2d_down) ;
							dDrawOptionDowns.append(dWidthDown) ;
							strXdataDown.AppendFormat(_T("A:%d,B:%d,C:%d;"), pt2dArrayDown.length()-1, iIndexVertex, iIndexVertexOld) ;
						}
						else if (iIndexVertex==(iNumVertex-1))
						{
							AcGeLineSeg2d lineSeg2dSrc(pt_2d_pre, pt_2d_cur) ;
							AcGeLineSeg2d lineSeg2dParallelUp, lineSeg2dParallelDown ;
							m_mysys.GetParallelLine(lineSeg2dSrc, lineSeg2dParallelUp, dDistBetweenSxHalf ) ;
							m_mysys.GetParallelLine(lineSeg2dSrc, lineSeg2dParallelDown, dDistBetweenSxHalf, FALSE) ;
							pt_2d_up = lineSeg2dParallelUp.endPoint() ;
							pt_2d_down = lineSeg2dParallelDown.endPoint() ;

							//begin 绘制轨道名称
							if (iGdWriteLeftOrRight==1)
							{
								AcGePoint2d pt_2d_gdname = lineSeg2dSrc.midPoint() ;
								AcGePoint3d pt_3dg_gdname(pt_2d_gdname.x, pt_2d_gdname.y, 0) ;
								AcGeVector2d vecLine = lineSeg2dSrc.direction() ;
								AcDbObjectId objIdGdName ;
								m_mysys.AddText(objIdGdName, strGdName, pt_3dg_gdname, pt_3dg_gdname, AcDb::kTextCenter, AcDb::kTextVertMid, 2.5, 0.8, vecLine.angle(), 4)  ;	
								m_mysys.SetXdata(objIdGdName, _T("TYPE"), (long)101) ; //双线图中轨道名称文本
								m_mysys.SetXdata(objIdGdName, _T("BELONG_ENT"), iHandle) ;
							}
							//end 绘制轨道名称

							int iIndexVertexOld = -1 ;
							cSetVertex.GetFieldValue(_T("sxt_line_vertex"), iIndexVertexOld) ;

							//和该FW端相交的DW轨道线的信息
							AcGePoint2d pt_2d_start_dw, pt_2d_end_dw ;	
							strSql.Format(_T("select * from sxt_ent where sxt_ent_fwline_handle=%d and sxt_ent_fwline_vertex=%d"), iHandle, iIndexVertexOld) ;
							long iHandleDW = 0 ;
							int iLineVertexDW = 0 ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_handle"), iHandleDW) ;
							long nHandleDc = 0 ; //以该线为反位线的道岔记录下来，为双线图绘制尖轨芯轨用
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_handle"), nHandleDc) ;
							CString strDcName ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), strDcName) ;
							int iUpOrDownDc = 0 ;
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_upordown"), iUpOrDownDc) ;

							if (iHandleDW!=0) //还有不是和DC反位相连的线，比如轨道终点
							{
								m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_online_vertex"), iLineVertexDW) ;
								strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d and sxt_line_vertex=%d"), iHandleDW, iLineVertexDW) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_line_vertex_x2"), pt_2d_start_dw.x) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_line_vertex_y2"),  pt_2d_start_dw.y) ;
								strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d and sxt_line_vertex=%d"), iHandleDW, iLineVertexDW+1) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_line_vertex_x2"), pt_2d_end_dw.x) ;
								m_RecordSet.GetRecordValue(strSql, _T("sxt_line_vertex_y2"),  pt_2d_end_dw.y) ;
								//2条平行线，一条距定位2.5,，另一条是距定位线1.5
								AcGeLineSeg2d lineSeg2dDwSrc(pt_2d_start_dw, pt_2d_end_dw), lineSeg2dDw25, lineSeg2dDw15 ;

								//begin 尖轨、芯轨牵引数量
								double dAngleDc = 0 ; //道岔块旋转角度
								AcDbEntity* pEntDc = NULL ;
								if (m_mysys.OpenAcDbEntity(nHandleDc, pEntDc, AcDb::kForRead))
								{
									AcDbBlockReference* pRefDc = AcDbBlockReference::cast(pEntDc) ;
									dAngleDc = pRefDc->rotation() ;
									pEntDc->close() ;
								}
								//end 尖轨、芯轨牵引数量

								AcGePoint2d pt_2d_ext ;
								AcGeVector2d vecDw = pt_2d_end_dw-pt_2d_start_dw ;
								//用于判断道岔垂直方向，不能简单根据*** int iDcDirectV = ((pt_2d_pre.y-pt_2d_cur.y)>0.5?0:1) ; //道岔垂直方向（FW向上0、向下1）***
								//根据以下算法：道岔旋转角，90~270度之间，向下								
								int iDcDirectV =0 ; //道岔垂直方向（FW向上0、向下1）
								if (dAngleDc>PI/2&&dAngleDc<(PI*3/2))
								{
									iDcDirectV = 1 ;
								}
								if (iDcDirectV==0) //反位端朝上,允许一定误差，这个0.5可再议
								{
									//反位端相交的所在多段线及段序号
									m_mysys.GetParallelLine(lineSeg2dDwSrc, lineSeg2dDw25, dDc25, TRUE) ;
									m_mysys.GetParallelLine(lineSeg2dDwSrc, lineSeg2dDw15, dDc15, FALSE) ;

									lineSeg2dParallelUp.intersectWith(lineSeg2dDw25, pt_2d_up) ;
									lineSeg2dParallelDown.intersectWith(lineSeg2dDw15, pt_2d_down) ;

									pt2dArrayUp.append(pt_2d_up) ;
									dDrawOptionUps.append(dWidthUp) ;
									strXdataUp.AppendFormat(_T("A:%d,B:%d,C:%d;"), pt2dArrayUp.length()-1, iIndexVertex, iIndexVertexOld) ;
									pt2dArrayDown.append(pt_2d_down) ;
									dDrawOptionDowns.append(dWidthDown) ;
									strXdataDown.AppendFormat(_T("A:%d,B:%d,C:%d;"), pt2dArrayDown.length()-1, iIndexVertex, iIndexVertexOld) ;

									//Dc反位线和定位线平行的一段
									pt_2d_ext =pt_2d_down ;
									pt_2d_ext.x +=dLenDcPlus ;
									//AcGeVector2d vecDw = pt_2d_end_dw-pt_2d_start_dw ;
									pt_2d_ext.rotateBy(vecDw.angle(), pt_2d_down) ;								
									pt2dArrayDown.append(pt_2d_ext) ;	
									dDrawOptionDowns.append(dWidthDown) ;
									strXdataDown.AppendFormat(_T("A:%d,B:%d,C:%d;"), pt2dArrayDown.length()-1, iIndexVertex, iIndexVertexOld) ;
									strXdataDownForZZJ.AppendFormat(_T("%d-1-%d-%d-%d;"), nHandleDc, iIndexVertex, iHandleDW, iLineVertexDW) ;
								}
								else //反位端朝下，（反位端在水平线上，视同反位朝下）
								{
									m_mysys.GetParallelLine(lineSeg2dDwSrc, lineSeg2dDw25, dDc25, FALSE) ;
									m_mysys.GetParallelLine(lineSeg2dDwSrc, lineSeg2dDw15, dDc15, TRUE) ;

									lineSeg2dParallelUp.intersectWith(lineSeg2dDw15, pt_2d_up) ;
									lineSeg2dParallelDown.intersectWith(lineSeg2dDw25, pt_2d_down) ;

									pt2dArrayUp.append(pt_2d_up) ;
									dDrawOptionUps.append(dWidthUp) ;
									strXdataUp.AppendFormat(_T("A:%d,B:%d,C:%d;"), pt2dArrayUp.length()-1, iIndexVertex, iIndexVertexOld) ;
									pt2dArrayDown.append(pt_2d_down) ;
									dDrawOptionDowns.append(dWidthDown) ;
									strXdataDown.AppendFormat(_T("A:%d,B:%d,C:%d;"), pt2dArrayDown.length()-1, iIndexVertex, iIndexVertexOld) ;

									//Dc反位线和定位线平行的一段
									pt_2d_ext = pt_2d_up ;
									pt_2d_ext.x +=dLenDcPlus ;
									//AcGeVector2d vecDw = pt_2d_end_dw-pt_2d_start_dw ;
									pt_2d_ext.rotateBy(vecDw.angle(), pt_2d_up) ;
									pt2dArrayUp.append(pt_2d_ext) ;
									dDrawOptionUps.append(dWidthUp) ;
									strXdataUp.AppendFormat(_T("A:%d,B:%d,C:%d;"), pt2dArrayUp.length()-1, iIndexVertex, iIndexVertexOld) ;
									strXdataUpForZZJ.AppendFormat(_T("%d-0-%d-%d-%d;"), nHandleDc, iIndexVertex, iHandleDW, iLineVertexDW) ;
								}

								//begin draw zzj
								int iDcDirectH = 1 ; //道岔FW水平朝向，0朝右，1，朝左
								AcGeLineSeg2d line2d(pt_2d_ext,iDcDirectV==0?pt_2d_down:pt_2d_up) ;									
								AcGePoint2d pt_2d_extcen =line2d.midPoint() ;

								AcGePoint2d pt_2d_extcen_mid = pt_2d_extcen ;
								pt_2d_extcen_mid.y+=(iDcDirectV==0?dDc15:-dDc15) ;
								pt_2d_extcen_mid.rotateBy(vecDw.angle(), pt_2d_extcen) ;

								//映射到双线的一边
								AcGePoint2d pt_2d_extcen_jg = pt_2d_extcen_mid ;
								pt_2d_extcen_jg.y +=(iUpOrDownDc==0?-dDistBetweenSxHalf:dDistBetweenSxHalf); 
								pt_2d_extcen_jg.rotateBy(vecDw.angle(), pt_2d_extcen) ;

								//道岔名称
								AcGePoint2d pt_2d_dcname = pt_2d_extcen_jg ;
								pt_2d_dcname.y+=(iUpOrDownDc==0?-2:2); 
								pt_2d_dcname.rotateBy(vecDw.angle(), pt_2d_extcen_jg) ;
								AcGePoint3d pt_3d_dcname(pt_2d_dcname.x, pt_2d_dcname.y,0) ;
								m_mysys.AddText(strDcName, pt_3d_dcname, pt_3d_dcname, AcDb::kTextCenter, AcDb::kTextVertMid, 2.5, 0.8, vecDw.angle(), 3) ;
								//end draw zzj
							}
							else
							{
								pt2dArrayUp.append(pt_2d_up) ;
								dDrawOptionUps.append(dWidthUp) ;
								strXdataUp.AppendFormat(_T("A:%d,B:%d,C:%d;"), pt2dArrayUp.length()-1, iIndexVertex, iIndexVertexOld) ;
								pt2dArrayDown.append(pt_2d_down) ;
								dDrawOptionDowns.append(dWidthDown) ;
								strXdataDown.AppendFormat(_T("A:%d,B:%d,C:%d;"), pt2dArrayDown.length()-1, iIndexVertex, iIndexVertexOld) ;
							}
						}
						else
						{
							AcGeLineSeg2d lineSeg2dSrc1(pt_2d_pre, pt_2d_cur), lineSeg2dSrc2(pt_2d_cur, pt_2d_back) ;	
							//acutPrintf(_T("\nlinesrc1(%.3f,%.3f)(%.3f,%.3f) linesrc2(%.3f,%.3f)(%.3f,%.3f)"), lineSeg2dSrc1.startPoint().x,lineSeg2dSrc1.startPoint().y,lineSeg2dSrc1.endPoint().x,lineSeg2dSrc1.endPoint().y,lineSeg2dSrc2.startPoint().x,lineSeg2dSrc2.startPoint().y, lineSeg2dSrc2.endPoint().x,lineSeg2dSrc2.endPoint().y );

							AcGeLineSeg2d lineSeg2dParallelUp1, lineSeg2dParallelDown1, lineSeg2dParallelUp2, lineSeg2dParallelDown2 ;
							m_mysys.GetParallelLine(lineSeg2dSrc1, lineSeg2dParallelUp1, dDistBetweenSxHalf ) ;
							m_mysys.GetParallelLine(lineSeg2dSrc1, lineSeg2dParallelDown1, dDistBetweenSxHalf, FALSE) ;
							m_mysys.GetParallelLine(lineSeg2dSrc2, lineSeg2dParallelUp2, dDistBetweenSxHalf ) ;
							m_mysys.GetParallelLine(lineSeg2dSrc2, lineSeg2dParallelDown2, dDistBetweenSxHalf, FALSE) ;				

							m_mysys.IntersectWith(&lineSeg2dParallelUp1, &lineSeg2dParallelUp2, pt_2d_up, AcDb::kExtendBoth) ;
							m_mysys.IntersectWith(&lineSeg2dParallelDown1, &lineSeg2dParallelDown2, pt_2d_down, AcDb::kExtendBoth) ;

							if (lineSeg2dSrc1.isParallelTo(lineSeg2dSrc2, tol))
							{
								pt_2d_up = lineSeg2dParallelUp1.endPoint() ;
								pt_2d_down = lineSeg2dParallelDown1.endPoint() ;
							}
							//acutPrintf(_T("\nintersect-up(%.3f,%.3f)intersect-down(%.3f,%.3f)"), pt_2d_up.x,pt_2d_up.y,pt_2d_down.x,pt_2d_down.y );

							pt2dArrayUp.append(pt_2d_up) ;
							dDrawOptionUps.append(dWidthUp) ;
							strXdataUp.AppendFormat(_T("A:%d,B:%d,C:%d;"), pt2dArrayUp.length()-1, iIndexVertex, iIndexVertexOld) ;
							pt2dArrayDown.append(pt_2d_down) ;
							dDrawOptionDowns.append(dWidthDown) ;
							strXdataDown.AppendFormat(_T("A:%d,B:%d,C:%d;"), pt2dArrayDown.length()-1, iIndexVertex, iIndexVertexOld) ;
						}

						cSetVertex.MoveNext() ;
						iIndexVertex++ ;
						iDrawOptionPre = iDrawOptionCur ;
					}
					cSetVertex.Close() ;
				}

// 				AcDbObjectId objIdUp = m_mysys.AddAcDbPolyLine(pt2dArrayUp, dDrawOptionUps,7) ;				
// 				AcDbObjectId objIdDown = m_mysys.AddAcDbPolyLine(pt2dArrayDown, dDrawOptionDowns, 7) ;
				AcDbObjectId objIdUp = m_mysys.AddAcDbPolyLine(pt2dArrayUp, 0.5,7) ;				
				AcDbObjectId objIdDown = m_mysys.AddAcDbPolyLine(pt2dArrayDown, 0.5, 7) ;

				//set xdata
				strXdataUp.TrimRight(_T(";")) ;
				strXdataDown.TrimRight(_T(";")) ;
				strXdataUpForZZJ.TrimRight(_T(";")) ;
				strXdataDownForZZJ.TrimRight(_T(";")) ;
				AcDbEntity * pEnt = NULL ;
				if (m_mysys.OpenAcDbEntity(objIdUp, pEnt))
				{
					m_mysys.SetXdata(pEnt, _T("CloneFrom"), iHandle) ;
					m_mysys.SetXdata(pEnt, _T("UPDOWN"), 1) ;
					m_mysys.SetXdata(pEnt, _T("VertexInfo"), strXdataUp) ;
					if (!strXdataUpForZZJ.IsEmpty())
					{
						m_mysys.SetXdata(pEnt, _T("ForZZJ"), strXdataUpForZZJ) ;
					}
					pEnt->close() ;
				}
				if (m_mysys.OpenAcDbEntity(objIdDown, pEnt))
				{
					m_mysys.SetXdata(pEnt, _T("CloneFrom"), iHandle) ;
					m_mysys.SetXdata(pEnt, _T("UPDOWN"), 0) ;
					m_mysys.SetXdata(pEnt, _T("VertexInfo"), strXdataDown) ;
					if (!strXdataDownForZZJ.IsEmpty())
					{
						m_mysys.SetXdata(pEnt, _T("ForZZJ"), strXdataDownForZZJ) ;
					}
					pEnt->close() ;
				}

				cSet.MoveNext() ;
			}
			cSet.Close() ;
		}
		//end 绘制线

		//begin 绘制封闭的线
		strSql.Format(_T("select distinct sxt_line_handle from sxt_line where sxt_line_type=11")) ;
		if (cSet.Open(strSql))
		{
			while(!cSet.IsEOF())
			{
				long  nHandle = 0 ;
				cSet.GetFieldValue(_T("sxt_line_handle"), nHandle) ;
				strSql.Format(_T("select * from sxt_line where sxt_line_handle=%d order by sxt_line_vertex asc"), nHandle) ;
				CDoubleArray dArrayX, dArrayY ;
				int iRsCount = m_RecordSet.GetRecordValue(strSql, _T("sxt_line_vertex_x2"), dArrayX) ;
				m_RecordSet.GetRecordValue(strSql, _T("sxt_line_vertex_y2"), dArrayY) ;
				if (iRsCount>2)
				{
					AcGePoint3dArray pt3dArray ;
					for (int i=0; i<iRsCount; i++)
					{
						AcGePoint3d pt_3d_tmp(dArrayX.GetAt(i), dArrayY.GetAt(i), 0) ;
						pt3dArray.append(pt_3d_tmp) ;
					}
					m_mysys.AddAcDbPolyLine(pt3dArray, 0.2, 7, TRUE) ;
				}

				cSet.MoveNext() ;
			}
		}
		//end 绘制封闭的线

		//begin 信号机
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type=550")) ;
		if (cSet.Open(strSql))
		{
			while(!cSet.IsEOF())
			{
				long nHandle = 0 ;
				cSet.GetFieldValue(_T("sxt_ent_handle"), nHandle) ;
				double dEntX = 0.000, dEntY = 0.000 ;				
				cSet.GetFieldValue(_T("sxt_ent_x2"), dEntX) ;
				cSet.GetFieldValue(_T("sxt_ent_y2"), dEntY) ;
				CString strBlkName ; 
				cSet.GetFieldValue(_T("sxt_ent_block"), strBlkName) ;
				AcDbEntity * pEnt = NULL ;
				m_mysys.Set_Layer(_T(""), _T("SXT_XHJ")) ;
				if (m_mysys.OpenAcDbEntity(nHandle, pEnt))
				{
					AcDbBlockReference *pRef = AcDbBlockReference::cast(pEnt) ;
					double dAngle = pRef->rotation() ;
					AcGePoint3d pt_3d_old = pRef->position() ;
					AcDbBlockReference *pRefNew = AcDbBlockReference::cast(pRef->clone()) ;
					m_mysys.SetXdata(pRefNew, _T("CloneFrom"), nHandle) ;
					m_mysys.SetXdata(pRefNew, _T("BELONG_ENT"), nHandle) ;
					AcGePoint3d pt_3d_new(dEntX, dEntY, 0) ;
					AcGeMatrix3d matrix ;
					AcGeVector3d vec = pt_3d_new - pt_3d_old ;
					matrix.setToTranslation(vec) ;
					pRefNew->transformBy(matrix) ; 
					//begin 在双线图中，信号机基点不能在中线上，要至少上/下移2.5
					AcGePoint3d pt_3d_new2(pt_3d_new) ;
					int iMarkLoc = strBlkName.Find(_T("_Y_")) ;//正向信号机（Y 立于右侧）, 逆向信号机(Z 立于左侧)
					//角度在90度~270度之间是倒置了
					if (dAngle>PI/2&&dAngle<PI*3/2)
					{
						pt_3d_new2.x=pt_3d_new2.x+(iMarkLoc==-1?dDistBetweenSxHalf:-dDistBetweenSxHalf) ;
			
					}
					else
					{
						pt_3d_new2.x=pt_3d_new2.x+(iMarkLoc==-1?-dDistBetweenSxHalf:dDistBetweenSxHalf) ;
						
					}
				
					pt_3d_new2.rotateBy(PI/2, AcGeVector3d::kZAxis, pt_3d_new) ;
					vec = pt_3d_new2 - pt_3d_new ;
					matrix.setToTranslation(vec) ;
					pRefNew->transformBy(matrix) ; 
					//end 在双线图中，信号机基点不能在中线上，要至少上/下移2.5
					pRefNew->setLayer(_T("SXT_XHJ")) ;
					m_mysys.AddEntityToDb(pRefNew) ;
					pRefNew->close() ;
					pEnt->close() ;
				}

				cSet.MoveNext() ;
			}

			cSet.Close() ;
		}
		//end 信号机
		//begin 其他
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type=500")) ;
		if (cSet.Open(strSql))
		{
			while(!cSet.IsEOF())
			{
				long nHandle = 0 ;
				cSet.GetFieldValue(_T("sxt_ent_handle"), nHandle) ;

				double dEntX = 0.000, dEntY = 0.000 ;				
				cSet.GetFieldValue(_T("sxt_ent_x2"), dEntX) ;
				cSet.GetFieldValue(_T("sxt_ent_y2"), dEntY) ;
				CString strBlkName ; 
				cSet.GetFieldValue(_T("sxt_ent_block"), strBlkName) ;
				CString strLayer=strBlkName.Mid(3,3);
				strLayer=_T("SXT_")+strLayer;
				AcDbEntity * pEnt = NULL ;
				if (m_mysys.OpenAcDbEntity(nHandle, pEnt))
				{
					AcDbBlockReference *pRef = AcDbBlockReference::cast(pEnt) ;
					double dAngle = pRef->rotation() ;
					AcGePoint3d pt_3d_old = pRef->position() ;
					AcDbBlockReference *pRefNew = AcDbBlockReference::cast(pRef->clone()) ;
					m_mysys.SetXdata(pRefNew, _T("CloneFrom"), nHandle) ;
					m_mysys.SetXdata(pRefNew, _T("BELONG_ENT"), nHandle) ;
					AcGePoint3d pt_3d_new(dEntX, dEntY, 0) ;
					AcGeMatrix3d matrix ;
					AcGeVector3d vec = pt_3d_new - pt_3d_old ;
					matrix.setToTranslation(vec) ;
					pRefNew->transformBy(matrix) ; 
					pRefNew->setLayer(strLayer) ;
					//m_mysys.Set_Layer(_T(""), strLayer) ;
					m_mysys.AddEntityToDb(pRefNew) ;
					pRefNew->close() ;
					pEnt->close() ;
				}
				cSet.MoveNext() ;
			}

			cSet.Close() ;
		}
		//end 其他
		//begin 电缆孔
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type=560")) ;
		if (cSet.Open(strSql))
		{
			while(!cSet.IsEOF())
			{
				long nHandle = 0 ;
				cSet.GetFieldValue(_T("sxt_ent_handle"), nHandle) ;

				double dEntX = 0.000, dEntY = 0.000 ;				
				cSet.GetFieldValue(_T("sxt_ent_x2"), dEntX) ;
				cSet.GetFieldValue(_T("sxt_ent_y2"), dEntY) ;
				CString strBlkName ; 
				strBlkName=_T("DT_CABLEIMPORT_100");
				//cSet.GetFieldValue(_T("sxt_ent_block"), strBlkName) ;
				AcDbEntity * pEnt = NULL ;
				if (m_mysys.OpenAcDbEntity(nHandle, pEnt))
				{
					AcDbBlockReference *pRef = AcDbBlockReference::cast(pEnt) ;
					double dAngle = pRef->rotation() ;
					AcGePoint3d pt_3d_old = pRef->position() ;
					AcDbBlockReference *pRefNew = AcDbBlockReference::cast(pRef->clone()) ;
					m_mysys.SetXdata(pRefNew, _T("CloneFrom"), nHandle) ;
					m_mysys.SetXdata(pRefNew, _T("BELONG_ENT"), nHandle) ;
					AcGePoint3d pt_3d_new(dEntX, dEntY, 0) ;
					AcGeMatrix3d matrix ;
					AcGeVector3d vec = pt_3d_new - pt_3d_old ;
					matrix.setToTranslation(vec) ;
					pRefNew->transformBy(matrix) ; 
					pRefNew->setLayer(_T("SXT_CABLEIMPORT")) ;
					m_mysys.Set_Layer(_T(""), _T("SXT_CABLEIMPORT")) ;
					m_mysys.AddEntityToDb(pRefNew) ;
					pRefNew->close() ;
					pEnt->close() ;
				}
				cSet.MoveNext() ;
			}

			cSet.Close() ;
		}
		//end 电缆孔
		//begin 绘制双线车档
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type=570")) ;
		if (cSet.Open(strSql))
		{
			while(!cSet.IsEOF())
			{
				long nHandle = 0 ;
				cSet.GetFieldValue(_T("sxt_ent_handle"), nHandle) ;

				double dEntX = 0.000, dEntY = 0.000 ;				
				cSet.GetFieldValue(_T("sxt_ent_x2"), dEntX) ;
				cSet.GetFieldValue(_T("sxt_ent_y2"), dEntY) ;
				CString strBlkName ; 
				strBlkName=_T("DT_CABLEIMPORT_100");
				//cSet.GetFieldValue(_T("sxt_ent_block"), strBlkName) ;
				AcDbEntity * pEnt = NULL ;
				if (m_mysys.OpenAcDbEntity(nHandle, pEnt))
				{
					AcDbBlockReference *pRef = AcDbBlockReference::cast(pEnt) ;
					double dAngle = pRef->rotation() ;
					AcGePoint3d pt_3d_old = pRef->position() ;
					AcDbBlockReference *pRefNew = AcDbBlockReference::cast(pRef->clone()) ;
					AcDbBlockReference *pRefNew2 = AcDbBlockReference::cast(pRef->clone());
					m_mysys.SetXdata(pRefNew, _T("CloneFrom"), nHandle) ;
					m_mysys.SetXdata(pRefNew, _T("BELONG_ENT"), nHandle) ;
					m_mysys.SetXdata(pRefNew2, _T("CloneFrom"), nHandle) ;
					m_mysys.SetXdata(pRefNew2, _T("BELONG_ENT"), nHandle) ;
					AcGePoint3d pt_3d_new(dEntX, dEntY, 0) ;
					AcGeMatrix3d matrix ;
					AcGeVector3d vec1 = pt_3d_new - pt_3d_old ;
					vec1.y+=5;
					AcGeVector3d vec2 = pt_3d_new - pt_3d_old ;
					vec2.y-=5;
					matrix.setToTranslation(vec1) ;
					pRefNew->transformBy(matrix) ; 
					pRefNew->setLayer(_T("SXT_CD")) ;
					m_mysys.Set_Layer(_T(""), _T("SXT_CD")) ;
					//添加上方轨道线车档
					m_mysys.AddEntityToDb(pRefNew) ;
					matrix.setToTranslation(vec2) ;
					pRefNew2->transformBy(matrix) ; 
					pRefNew2->setLayer(_T("SXT_CD")) ;
					//m_mysys.Set_Layer(_T(""), _T("SXT_CD")) ;
					//添加上方轨道线车档
					m_mysys.AddEntityToDb(pRefNew2) ;
					pRefNew->close() ;
					pRefNew2->close();
					pEnt->close() ;
				}
				cSet.MoveNext() ;
			}

			cSet.Close() ;
		}
		//end 绘制双线车档
		//begin 绘制跳线
		//this->DrawJumpLine() ;		
		//end 绘制跳线

		//begin 绘制 信号楼(20150605：双线不绘制警冲标）
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type\\100=3")) ;
		if (cSet.Open(strSql))
		{
			while(!cSet.IsEOF())
			{
				long nHandle = 0 ;
				cSet.GetFieldValue(_T("sxt_ent_handle"), nHandle) ;

				double dEntX = 0.000, dEntY = 0.000 ;				
				cSet.GetFieldValue(_T("sxt_ent_x2"), dEntX) ;
				cSet.GetFieldValue(_T("sxt_ent_y2"), dEntY) ;
				CString strBlkName ; 
				cSet.GetFieldValue(_T("sxt_ent_block"), strBlkName) ;
				AcDbEntity * pEnt = NULL ;
				if (m_mysys.OpenAcDbEntity(nHandle, pEnt))
				{
					AcDbBlockReference *pRef = AcDbBlockReference::cast(pEnt) ;
					double dAngle = pRef->rotation() ;
					AcGePoint3d pt_3d_old = pRef->position() ;
					AcDbBlockReference *pRefNew = AcDbBlockReference::cast(pRef->clone()) ;
					AcGePoint3d pt_3d_new(dEntX, dEntY, 0) ;
					AcGeMatrix3d matrix ;
					AcGeVector3d vec = pt_3d_new - pt_3d_old ;
					matrix.setToTranslation(vec) ;
					pRefNew->transformBy(matrix) ; 
					pRefNew->setLayer(_T("sxt")) ;
					m_mysys.AddEntityToDb(pRefNew) ;
					pRefNew->close() ;
					pEnt->close() ;
				}

				cSet.MoveNext() ;
			}

			cSet.Close() ;
		}
		//end 绘制警冲标 信号楼

		//begin 绘制距离信号楼距离
		AcGePoint3d pt_3d_min_dwg(0,0,0), pt_3d_max_dwg(0,0,0) ; //先取得当前图纸的范围,以便确定绘制距信号楼距离的坐标
		m_mysys.GetCurExtents(pt_3d_min_dwg, pt_3d_max_dwg) ;
	   //测试范围获取是否正确
		//CLineUtil::Add(pt_3d_min_dwg,pt_3d_max_dwg);
		double dY_Dist[2][2] = {{0,0}, {0,0} } ;
// 		dY_Dist[0][0] = pt_3d_max_dwg.y+80 ;
// 		dY_Dist[0][1] = dY_Dist[0][0]+13 ;
// 		dY_Dist[1][0] = pt_3d_orign.y-(dY_Dist[0][0]-pt_3d_orign.y) ;
// 		dY_Dist[1][1] = dY_Dist[1][0]-13 ; 
		//平面图的基点为（0,143.5）
		dY_Dist[0][0]=143.5*dScaleY+pt_3d_orign.y+200-31;
		dY_Dist[0][1]=dY_Dist[0][0]+13;
		dY_Dist[1][0]=143.5*dScaleY+pt_3d_orign.y-200+31;
		dY_Dist[1][1]=dY_Dist[1][0]-13;
		//图框
		double dY_DistFrame[2][2]={{0,0},{0,0}};
		dY_DistFrame[0][0]=dY_Dist[0][0]+31;
		dY_DistFrame[0][1]=dY_Dist[1][0]-31;
		dY_DistFrame[1][0]=dY_DistFrame[0][0]+10;
		dY_DistFrame[1][1]=dY_DistFrame[1][0]-10;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type\\100=6")) ; //整除为6
		if (cSet.Open(strSql))
		{
			while(!cSet.IsEOF())
			{
				int iType = 0 ;
				cSet.GetFieldValue(_T("sxt_ent_type"), iType) ;
				CString strLoc ;
				cSet.GetFieldValue(_T("sxt_ent_name"), strLoc) ;
				double dX2 = 0 ;
				cSet.GetFieldValue(_T("sxt_ent_x2"), dX2) ;

				int i1 = iType%10 ; //个位
				int i10 = iType%100/10 ; //十位
                
				//AcGePoint3d pt_3d_insert_text(dX2-1.5, dY_Dist[i10][i1]+(i10==0?1:-1)*(i1==0?6.5:11), 0) ;
				AcGePoint3d pt_3d_insert_text(dX2-1.5, dY_Dist[i10][i1]+(i10==0?1:-1)*(i1==0?6.5:9.5), 0) ;
				m_mysys.AddText(strLoc, pt_3d_insert_text, pt_3d_insert_text, AcDb::kTextCenter, AcDb::kTextVertMid, 3, 0.55, PI/2,7,idTextStyleChs) ;

				cSet.MoveNext() ;
			}

			cSet.Close() ;
		}

		//插入表头、绘制行线
		strSql.Format(_T("select min(sxt_ent_x2) as minX, max(sxt_ent_x2) as maxX from sxt_ent where sxt_ent_type\\100=6")) ; //整除为6
		double dMinX = 0, dMaxX = 0 ;
		m_RecordSet.GetRecordValue(strSql, _T("minX"), dMinX) ;
		m_RecordSet.GetRecordValue(strSql, _T("maxX"), dMaxX) ;
		AcDbBlockReference* pRef = NULL ;
		AcGePoint3d pt_3d_header(dMinX-30,0,0) ;
		CString strBlkName ; 
	   	m_mysys.Set_Layer(_T(""), _T("SXT_LOC")) ;
		for (int i=0; i<2; i++)
		{
			//表头
			pt_3d_header.y = dY_Dist[i][0] ;
			strBlkName.Format(_T("DT_LOC_HEADER_%d00"), i) ;
			if (m_mysys.InsertBlock(pRef, strBlkName, pt_3d_header))
			{
				pRef->close() ;
				pRef = NULL ;
			}
			//线
			AcGePoint3d pt_3d_start(pt_3d_header.x+7.5, 0, 0), pt_3d_end(dMaxX+30, 0, 0) ;
			for (int j=0; j<2; j++)
			{
				pt_3d_start.y = dY_Dist[i][j] ;
				pt_3d_end.y = dY_Dist[i][j] ;
				m_mysys.AddAcDbLine(pt_3d_start, pt_3d_end) ;
				if (j==1)
				{
					pt_3d_start.y = dY_Dist[i][j]+(i==0?1:-1)*18 ;
					pt_3d_end.y = dY_Dist[i][j]+(i==0?1:-1)*18 ;
					m_mysys.AddAcDbLine(pt_3d_start, pt_3d_end) ;
				}
			}

		}
		//插入图框
	   AcGePoint3d pt_3d_Frame;
	   pt_3d_Frame.x=dMinX-50;
	   pt_3d_Frame.y=dY_DistFrame[0][0];
	   pt_3d_Frame.z=0;
	   double d_width=dMaxX-dMinX+60;
	   CDtpmt::AddFrameToDtsxt(pt_3d_Frame,d_width);
		
		//end 绘制距离信号楼距离
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误!in Drawsxt_DT"));
	}

	m_pDb->setClayer(objIdOldLayer) ;

	return 0;
}
//AcGePoint3d pt_3dInsert,插入点；int iType,箭头类型，0为直线，1为折线；bool bToRight,true时，箭头朝右;CString strText,为箭头的文字；long nHandleCable,为对应电缆线
void CZnSxt::DT_InsertArrow(AcGePoint3d pt_3dInsert,int iType,bool bToRight,long nHandleCable)
{
	//插入箭头
	AcGePoint3d pt_3d_Arrow=pt_3dInsert;
	//pt_3d_Arrow.x=(pt_3d_LjlineStart.x+pt_3d_LjlineEnd.x)/2;
	AcDbBlockReference *pRefArrow=NULL;
	CString strblknameArrow;
	if(iType==0)
	{
		strblknameArrow=_T("DT_ARROW_000");
	}
	else 
	{
		strblknameArrow=_T("DT_ARROW_100");
	}
	if(m_mysys.InsertBlock(pRefArrow,strblknameArrow,pt_3d_Arrow,FALSE,0,TRUE))
	{
		CString strTag=_T("TEXT");
		CString strTag2=_T("TEXT2");
		CString strText=_T("");
		m_mysys.GetXdata(nHandleCable,_T("CABLESTRING"),strText);
		long iBack=0;
		m_mysys.GetXdata(nHandleCable,_T("ISBACK"),iBack);
		if(iBack!=0)
		{
			int iFindIndex1=strText.FindOneOf(" ");
			int iFindIndex2=strText.FindOneOf("(");
			int iFindIndex3=strText.FindOneOf("-");
			CString strback=strText.Mid(iFindIndex1+1,iFindIndex2-iFindIndex1-1);
			CString strback2=strText.Mid(iFindIndex3+1,iFindIndex2-iFindIndex3-1);
			CString strText2=_T("备用:")+strback+_T("(")+strback2+_T(")");
			m_mysys.SetAttValue(pRefArrow,strTag2,strText2,FALSE);

		}
		//m_mysys.SetEntColorIndex(pRefArrow,iColorIndex);
		m_mysys.SetAttValue(pRefArrow,strTag,strText,FALSE);
		m_mysys.SetXdata(pRefArrow,_T("BELONG_ENT"),nHandleCable);
		if(bToRight==false)
		{
			//m_mysys.RotationEnt(pRefArrow,PI,pt_3d_Arrow);
			AcGePoint3d pt_3d_Arrow2=pt_3d_Arrow;
			pt_3d_Arrow2.y+=10;
			//m_mysys.MirrEnt(pRefArrow,pt_3d_Arrow,pt_3d_Arrow2);
			m_mysys.RotationEnt(pRefArrow,PI,pt_3d_Arrow);
			//m_mysys.RotationEnt(pRefArrow,PI,)
		}
		//pRefArrow->setColorIndex(2);
		//AcDbEntity *pEnt=NULL;
		
	}
	pRefArrow->close();
	pRefArrow=NULL;	
    
}
// 绘制所有信号机盒子
void CZnSxt::DT_DrawBoxXhj(void)
{
	try
	{
		CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type=550")) ; //整除为1的
		if (cSet.Open(strSql))
		{
			while(!cSet.IsEOF())
			{
				long nHandle = 0 ;
				cSet.GetFieldValue(_T("sxt_ent_handle"), nHandle) ;
				if(nHandle==7190)
				{
					nHandle=nHandle;
				}
				this->DT_DrawBoxXhj(nHandle) ;
				cSet.MoveNext() ;
			}
			cSet.Close() ;
		}
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误!in DT_DrawBoxXhj"));
	}
}
// 绘制信号机箱盒,传入平面图中信号机的句柄
void CZnSxt::DT_DrawBoxXhj(long nHandleXhj)
{
	
	
	long nHandleCloneXhj = 0 ; 
	//去掉原来的信号机
	//克隆一个信号机
	this->ClearEnt(nHandleXhj, 1) ;
	CADORecordset cSet(&m_AdoDb) ;
	CString strSql ;
	long dDistBetweenSxHalf=5;
	strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"),nHandleXhj) ; //整除为1的
	double dEntX = 0.000, dEntY = 0.000 ;
	 m_RecordSet.GetRecordValue(strSql,_T("sxt_ent_x2"),dEntX);
	 m_RecordSet.GetRecordValue(strSql,_T("sxt_ent_y2"),dEntY);
	CString strBlkName ; 
	 m_RecordSet.GetRecordValue(strSql,_T("sxt_ent_block"),strBlkName);
	AcDbEntity * pEnt = NULL ;
	if (m_mysys.OpenAcDbEntity(nHandleXhj, pEnt))
	{
		AcDbBlockReference *pRef = AcDbBlockReference::cast(pEnt) ;
		double dAngle = pRef->rotation() ;
		AcGePoint3d pt_3d_old = pRef->position() ;
		//pt_3d_old.y+=10;
		//克隆一个信号机
		AcDbBlockReference *pRefNew = AcDbBlockReference::cast(pRef->clone()) ;
		m_mysys.SetXdata(pRefNew, _T("CloneFrom"), nHandleXhj) ;
		AcGePoint3d pt_3d_new(dEntX, dEntY, 0) ;
		AcGePoint3d pt_3d_new2(pt_3d_new) ;
		AcGePoint3d pt_3d_xh;
		double x_dist=0;
		double y_dist=0;
		int iMarkLoc = strBlkName.Find(_T("_Y_")) ;//正向信号机（Y 立于右侧）, 逆向信号机(Z 立于左侧)
		AcGePoint3d pt_3d_LjlineStart;
		//pt_3d_LjlineStart.y-=1.5;
		AcGePoint3d pt_3d_LjlineEnd;
		//角度在90度~270度之间是倒置了
		double x_move=0;
		if (dAngle>PI/2&&dAngle<PI*3/2)
		{
			pt_3d_new2.x=pt_3d_new2.x+(iMarkLoc==-1?dDistBetweenSxHalf:-dDistBetweenSxHalf) ;
			//pt_3d_new2.y=pt_3d_new2.y+(iMarkLoc==-1?2*dDistBetweenSxHalf:-2*dDistBetweenSxHalf);
			pt_3d_xh.x=pt_3d_new.x+(iMarkLoc==-1?2*dDistBetweenSxHalf:-2*dDistBetweenSxHalf) ;
			pt_3d_xh.y=pt_3d_new.y+(iMarkLoc==-1?6:-6) ;
			//pt_3d_old.y=pt_3d_old.y+(iMarkLoc==-1?-dDistBetweenSxHalf:dDistBetweenSxHalf);
			x_dist=(iMarkLoc==-1?3:-3);
			y_dist=(iMarkLoc==-1?5:-5);
			x_move=(iMarkLoc==-1?-2.5:2.5);
		}
		else
		{
			pt_3d_new2.x=pt_3d_new2.x+(iMarkLoc==-1?-dDistBetweenSxHalf:dDistBetweenSxHalf) ;
			//pt_3d_new2.y=pt_3d_new2.y+(iMarkLoc==-1?-2*dDistBetweenSxHalf:2*dDistBetweenSxHalf);
			pt_3d_xh.x=pt_3d_new.x+(iMarkLoc==-1?-2*dDistBetweenSxHalf:2*dDistBetweenSxHalf) ;
			pt_3d_xh.y=pt_3d_new.y+(iMarkLoc==-1?-6:6) ;
			//pt_3d_old.y=pt_3d_old.y+(iMarkLoc==-1?dDistBetweenSxHalf:-dDistBetweenSxHalf);
			x_dist=(iMarkLoc==-1?3:-3);
			y_dist=(iMarkLoc==-1?5:-5);
			x_move=(iMarkLoc==-1?2.5:-2.5);
		}
		AcGeMatrix3d matrix ;
		AcGeVector3d vec = pt_3d_new - pt_3d_old ;
		matrix.setToTranslation(vec) ;
		pRefNew->transformBy(matrix) ; 
		//begin 在双线图中，信号机基点不能在中线上，要至少上/下移2.5
		pt_3d_new2.rotateBy(PI/2, AcGeVector3d::kZAxis, pt_3d_new) ;
		vec = pt_3d_new2 - pt_3d_new ;
		matrix.setToTranslation(vec) ;
		pRefNew->transformBy(matrix) ; 
		//双线应该再往上挪动一些距离
		pRefNew->transformBy(matrix);
		//AcGePoint3d pt_3d_att=pRefNew->position();
		 pt_3d_LjlineStart=pRefNew->position();
		 m_mysys.AddEntityToDb(pRefNew) ;
		//插入信号机FXX
		AcDbBlockReference *pRef2=NULL;
		CString strblkname=_T("DT_BOX_RECT_0");
		//pt_3d_header.y+=31;
		//查询数据库，获取信号机箱盒等信息
		long nHandleXhjSx=0;
		nHandleXhjSx=m_mysys.GetHandle(pRefNew);
		CXhDevice *pDATA = new CXhDevice(strBlkName) ;
		this->GetEntData(pRefNew,*pDATA);
		//设置属性至块中
		this->SetEntData(pRefNew,pDATA);
		if (m_mysys.InsertBlock(pRef2, strblkname, pt_3d_xh,FALSE,0,TRUE))
		{
			CString strTag=_T("TYPE");
			CString str_name=pDATA->m_strBoxType;
			CString strleng=pDATA->m_strCable1;
			m_mysys.SetAttValue(pRef2,strTag,str_name,FALSE);	
// 			long nHandleXhjSx=0;
// 			nHandleXhjSx=m_mysys.GetHandle(pRefNew);
			m_mysys.SetXdata(pRef2, _T("BELONGTO_PMT"), nHandleXhj) ;
			m_mysys.SetXdata(pRef2, _T("BELONG_ENT"), nHandleXhjSx) ;
			//m_mysys.SetXdata(pRef2, _T(""), nHandleXhjSx) ;
			pRef2->transformBy(matrix);
			pRef2->transformBy(matrix);
			pt_3d_LjlineEnd=pRef2->position();
			pt_3d_LjlineStart.y=pt_3d_LjlineEnd.y;
			pt_3d_LjlineEnd.x+=x_move;
			long nHandleBox=m_mysys.GetHandle(pRef2);
			long nHandleline=m_mysys.GetHandle(CLineUtil::AddPolyLine(CConvertUtil::ToPoint2d(pt_3d_LjlineStart),CConvertUtil::ToPoint2d(pt_3d_LjlineEnd),0));
			//设置连接线属性
			m_mysys.SetXdata(nHandleline,_T("IS_CABLE"),1);
			m_mysys.SetXdata(nHandleline,_T("IS_ENDCABLE"),1);
			m_mysys.SetXdata(nHandleline,_T("BEGIN"),nHandleXhj);
			m_mysys.SetXdata(nHandleline,_T("BELONG_ENT"),nHandleXhjSx);
			m_mysys.SetXdata(nHandleline,_T("END"),nHandleBox);
			m_mysys.SetXdata(nHandleline,_T("CABLESTRING"),strleng);
			pRef2->close() ;
			pRef2 = NULL ;
// 			pRefArrow->close();
// 			pRefArrow=NULL;
			long nHandleCableString=0;
			if(fabs(dAngle-PI)<0.1)
				this->DT_SetCableString(nHandleline,nHandleCableString,strleng,1);
			else
				this->DT_SetCableString(nHandleline,nHandleCableString,strleng,0);

		}
		//end 在双线图中，信号机基点不能在中线上，要至少上/下移2.5
		//this->ClearEnt(nHandleXhj, 1) ;
		pRefNew->setLayer(_T("sxt")) ;
		//m_mysys.AddEntityToDb(pRefNew) ;
// 		CString strname;
// 		m_mysys.GetAttValue(pRefNew,_T("XHJNAME"),strname);
// 		if(strname==_T("F1"))
// 		{
// 			strname=strname;
// 		}
		//如果图块被旋转的180度时，是无法直接移动其属性位置的。所以需要先恢复至初始角度，设置属性位置，再旋转至原来的位置
		if(fabs(dAngle-PI)<=1e-5)
		{
			m_mysys.RotationEnt(pRefNew,PI,pRefNew->position());

		}
		//移动信号机名字属性位置，否则绘制盒子会出现交叉重叠现象
		m_mysys.SetAttPosition(pRefNew,_T("XHJNAME"),x_dist,y_dist);
		if(fabs(dAngle-PI)<=1e-5)
		{
			m_mysys.RotationEnt(pRefNew,PI,pRefNew->position());

		}
		pRefNew->close() ;
		pEnt->close() ;
		pDATA=NULL;
}
}
// 绘制信号机箱盒
void CZnSxt::DT_DrawBoxXhj(long nHandleXhj,CXhDevice *pData)
{
	long nHandleCloneXhj = 0 ; 
	//去掉原来的信号机
	//克隆一个信号机
	this->ClearEnt(nHandleXhj, 1) ;
	CADORecordset cSet(&m_AdoDb) ;
	CString strSql ;
	CXhDevice *pDATA = pData;
	//this->GetEntData(nHandleXhjSx,*pDATA);
	long dDistBetweenSxHalf=5;
	long nHandleXhjPmt=0;
	long nHandleXhjSx=0;
	if(m_mysys.GetXdata(nHandleXhj,_T("CloneFrom"),nHandleXhjPmt))
	{
		nHandleXhjSx=nHandleXhj;
	}
	strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"),nHandleXhjPmt) ; //整除为1的
	double dEntX = 0.000, dEntY = 0.000 ;
	m_RecordSet.GetRecordValue(strSql,_T("sxt_ent_x2"),dEntX);
	m_RecordSet.GetRecordValue(strSql,_T("sxt_ent_y2"),dEntY);
	CString strBlkName ; 
	m_RecordSet.GetRecordValue(strSql,_T("sxt_ent_block"),strBlkName);

	AcDbEntity * pEnt = NULL ;
	if (m_mysys.OpenAcDbEntity(nHandleXhjPmt, pEnt))
	{
		AcDbBlockReference *pRef = AcDbBlockReference::cast(pEnt) ;
		double dAngle = pRef->rotation() ;
		AcGePoint3d pt_3d_old = pRef->position() ;
		//pt_3d_old.y+=10;
		//克隆一个信号机
		AcDbBlockReference *pRefNew = AcDbBlockReference::cast(pRef->clone()) ;
		m_mysys.SetXdata(pRefNew, _T("CloneFrom"), nHandleXhjPmt) ;
		AcGePoint3d pt_3d_new(dEntX, dEntY, 0) ;
		AcGePoint3d pt_3d_new2(pt_3d_new) ;
		AcGePoint3d pt_3d_xh;
		double x_dist=0;
		double y_dist=0;
		int iMarkLoc = strBlkName.Find(_T("_Y_")) ;//正向信号机（Y 立于右侧）, 逆向信号机(Z 立于左侧)
		AcGePoint3d pt_3d_LjlineStart;
		//pt_3d_LjlineStart.y-=1.5;
		AcGePoint3d pt_3d_LjlineEnd;
		//角度在90度~270度之间是倒置了
		double x_move=0;
		if (dAngle>PI/2&&dAngle<PI*3/2)
		{
			pt_3d_new2.x=pt_3d_new2.x+(iMarkLoc==-1?dDistBetweenSxHalf:-dDistBetweenSxHalf) ;
			//pt_3d_new2.y=pt_3d_new2.y+(iMarkLoc==-1?2*dDistBetweenSxHalf:-2*dDistBetweenSxHalf);
			pt_3d_xh.x=pt_3d_new.x+(iMarkLoc==-1?2*dDistBetweenSxHalf:-2*dDistBetweenSxHalf) ;
			pt_3d_xh.y=pt_3d_new.y+(iMarkLoc==-1?6:-6) ;
			//pt_3d_old.y=pt_3d_old.y+(iMarkLoc==-1?-dDistBetweenSxHalf:dDistBetweenSxHalf);
			x_dist=(iMarkLoc==-1?3:-3);
			y_dist=(iMarkLoc==-1?5:-5);
			x_move=(iMarkLoc==-1?-2.5:2.5);
		}
		else
		{
			pt_3d_new2.x=pt_3d_new2.x+(iMarkLoc==-1?-dDistBetweenSxHalf:dDistBetweenSxHalf) ;
			//pt_3d_new2.y=pt_3d_new2.y+(iMarkLoc==-1?-2*dDistBetweenSxHalf:2*dDistBetweenSxHalf);
			pt_3d_xh.x=pt_3d_new.x+(iMarkLoc==-1?-2*dDistBetweenSxHalf:2*dDistBetweenSxHalf) ;
			pt_3d_xh.y=pt_3d_new.y+(iMarkLoc==-1?-6:6) ;
			//pt_3d_old.y=pt_3d_old.y+(iMarkLoc==-1?dDistBetweenSxHalf:-dDistBetweenSxHalf);
			x_dist=(iMarkLoc==-1?3:-3);
			y_dist=(iMarkLoc==-1?5:-5);
			x_move=(iMarkLoc==-1?2.5:-2.5);
		}
		AcGeMatrix3d matrix ;
		AcGeVector3d vec = pt_3d_new - pt_3d_old ;
		matrix.setToTranslation(vec) ;
		pRefNew->transformBy(matrix) ; 
		//begin 在双线图中，信号机基点不能在中线上，要至少上/下移2.5
		pt_3d_new2.rotateBy(PI/2, AcGeVector3d::kZAxis, pt_3d_new) ;
		vec = pt_3d_new2 - pt_3d_new ;
		matrix.setToTranslation(vec) ;
		pRefNew->transformBy(matrix) ; 
		//双线应该再往上挪动一些距离
		pRefNew->transformBy(matrix);
		//AcGePoint3d pt_3d_att=pRefNew->position();
		pt_3d_LjlineStart=pRefNew->position();
		//m_mysys.AddEntityToDb(pRefNew) ;
		//插入信号机FXX
		AcDbBlockReference *pRef2=NULL;
		CString strblkname=_T("DT_BOX_RECT_0");
		//pt_3d_header.y+=31;
		//查询数据库，获取信号机箱盒等信息
// 		long nHandleXhjSx=0;
// 		nHandleXhjSx=m_mysys.GetHandle(pRefNew);
		if ((pDATA->m_bHasBox==TRUE)&&m_mysys.InsertBlock(pRef2, strblkname, pt_3d_xh,FALSE,0,TRUE))
		{
			CString strTag=_T("TYPE");
			CString str_name=pDATA->m_strBoxType;
			CString strleng=pDATA->m_strCable1;
			m_mysys.SetAttValue(pRef2,strTag,str_name,FALSE);	
			// 			long nHandleXhjSx=0;
			// 			nHandleXhjSx=m_mysys.GetHandle(pRefNew);
			m_mysys.SetXdata(pRef2, _T("BELONGTO_PMT"), nHandleXhjPmt) ;
			m_mysys.SetXdata(pRef2, _T("BELONG_ENT"), nHandleXhjSx) ;
			//m_mysys.SetXdata(pRef2, _T(""), nHandleXhjSx) ;
			pRef2->transformBy(matrix);
			pRef2->transformBy(matrix);
			pt_3d_LjlineEnd=pRef2->position();
			pt_3d_LjlineStart.y=pt_3d_LjlineEnd.y;
			pt_3d_LjlineEnd.x+=x_move;
			long nHandleBox=m_mysys.GetHandle(pRef2);
			long nHandleline=m_mysys.GetHandle(CLineUtil::AddPolyLine(CConvertUtil::ToPoint2d(pt_3d_LjlineStart),CConvertUtil::ToPoint2d(pt_3d_LjlineEnd),0));
			//设置连接线属性
			m_mysys.SetXdata(nHandleline,_T("IS_CABLE"),1);
			m_mysys.SetXdata(nHandleline,_T("IS_ENDCABLE"),1);
			m_mysys.SetXdata(nHandleline,_T("BEGIN"),nHandleXhj);
			m_mysys.SetXdata(nHandleline,_T("BELONG_ENT"),nHandleXhjSx);
			m_mysys.SetXdata(nHandleline,_T("END"),nHandleBox);
			m_mysys.SetXdata(nHandleline,_T("CABLESTRING"),strleng);
			pRef2->close() ;
			pRef2 = NULL ;
			long nHandleCableString=0;
			if(fabs(dAngle-PI)<0.1)
				this->DT_SetCableString(nHandleline,nHandleCableString,strleng,1);
			else
				this->DT_SetCableString(nHandleline,nHandleCableString,strleng,0);
			//this->DT_SetCableString(nHandleline,nHandleCableString,strleng,1);
		}
		//end 在双线图中，信号机基点不能在中线上，要至少上/下移2.5
		//this->ClearEnt(nHandleXhj, 1) ;
		pRefNew->setLayer(_T("sxt")) ;
		//m_mysys.AddEntityToDb(pRefNew) ;
// 		CString strname;
// 		m_mysys.GetAttValue(pRefNew,_T("XHJNAME"),strname);
// 		if(strname==_T("F1"))
// 		{
// 			strname=strname;
// 		}
		//如果图块被旋转的180度时，是无法直接移动其属性位置的。所以需要先恢复至初始角度，设置属性位置，再旋转至原来的位置
		if(fabs(dAngle-PI)<=1e-5)
		{
			m_mysys.RotationEnt(pRefNew,PI,pRefNew->position());
		}
		m_mysys.SetAttPosition(pRefNew,_T("XHJNAME"),x_dist,y_dist);
		if(fabs(dAngle-PI)<=1e-5)
		{
			m_mysys.RotationEnt(pRefNew,PI,pRefNew->position());
		}
// 		pRefNew->close() ;
		pEnt->close() ;
		pDATA=NULL;
	}
}
// 绘制所有信号机盒子
void CZnSxt::DT_DrawBoxJZ(void)
{
	try
	{
		CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type=500")) ; //整除为1的
		if (cSet.Open(strSql))
		{
			while(!cSet.IsEOF())
			{
				long nHandle = 0 ;
				cSet.GetFieldValue(_T("sxt_ent_handle"), nHandle) ;
				if(nHandle==7190)
				{
					nHandle=nHandle;
				}
				this->DT_DrawBoxJZ(nHandle) ;
				cSet.MoveNext() ;
			}
			cSet.Close() ;
		}
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误!in DT_DrawBoxJZ"));
	}
}
// 绘制所有信号机盒子
void CZnSxt::DT_DrawSXDlk(void)
{
	try
	{
		CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type=560")) ; //整除为1的
		if (cSet.Open(strSql))
		{
			while(!cSet.IsEOF())
			{
				long nHandle = 0 ;
				cSet.GetFieldValue(_T("sxt_ent_handle"), nHandle) ;
				if(nHandle==7190)
				{
					nHandle=nHandle;
				}
				this->DT_DrawSXDlk(nHandle) ;
				cSet.MoveNext() ;
			}
			cSet.Close() ;
		}
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误!in DT_DrawBoxJZ"));
	}
}
void CZnSxt::DT_DrawSXDlk(long nHandle)
{

	CADORecordset cSet(&m_AdoDb) ;
	CString strSql ;
	/*	this->ClearEnt(nHandle, 1) ;*/
	//去掉克隆设备
	this->ClearEnt(nHandle, 1) ;
	long dDistBetweenSxHalf=5;
	strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"),nHandle) ; //整除为1的
	double dEntX = 0.000, dEntY = 0.000 ;
	m_RecordSet.GetRecordValue(strSql,_T("sxt_ent_x2"),dEntX);
	m_RecordSet.GetRecordValue(strSql,_T("sxt_ent_y2"),dEntY);
	//cSet.GetFieldValue(_T("sxt_ent_x2"), dEntX) ;
	//cSet.GetFieldValue(_T("sxt_ent_y2"), dEntY) ;
// 	CString strBlkName ; 
// 	m_RecordSet.GetRecordValue(strSql,_T("sxt_ent_block"),strBlkName);
// 	CString strEntName;
// 	m_RecordSet.GetRecordValue(strSql,_T("sxt_ent_name"),strEntName);
	//cSet.GetFieldValue(_T("sxt_ent_name"),strEntName);
	AcDbEntity * pEnt = NULL ;
	if (m_mysys.OpenAcDbEntity(nHandle, pEnt))
	{
		AcDbBlockReference *pRef = AcDbBlockReference::cast(pEnt) ;
		double dAngle = pRef->rotation() ;
		AcGePoint3d pt_3d_old = pRef->position() ;
		AcDbBlockReference *pRefNew = NULL;
		CString strBlkName=_T("DT_CABLEIMPORT_100");
		if (m_mysys.InsertBlock(pRefNew, strBlkName, pt_3d_old,FALSE,0,TRUE))
		{

		}
		m_mysys.SetXdata(pRefNew, _T("CloneFrom"), nHandle) ;
		AcGePoint3d pt_3d_new(dEntX, dEntY, 0) ;
		AcGeMatrix3d matrix ;
		AcGeVector3d vec = pt_3d_new - pt_3d_old ;
		matrix.setToTranslation(vec) ;
		pRefNew->transformBy(matrix) ; 
		pRefNew->setLayer(_T("sxt")) ;
		m_mysys.AddEntityToDb(pRefNew) ;
		pRefNew->close() ;
		pEnt->close() ;
	}
}
// 绘制信号机箱盒
void CZnSxt::DT_DrawBoxJZ(long nHandle)
{

	CADORecordset cSet(&m_AdoDb) ;
	CString strSql ;
/*	this->ClearEnt(nHandle, 1) ;*/
	//去掉克隆设备
	this->ClearEnt(nHandle, 1) ;
	long dDistBetweenSxHalf=5;
	//long nHandleJZpmt=0;
	strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"),nHandle) ; //整除为1的
	double dEntX = 0.000, dEntY = 0.000 ;
	m_RecordSet.GetRecordValue(strSql,_T("sxt_ent_x2"),dEntX);
	m_RecordSet.GetRecordValue(strSql,_T("sxt_ent_y2"),dEntY);
	//cSet.GetFieldValue(_T("sxt_ent_x2"), dEntX) ;
	//cSet.GetFieldValue(_T("sxt_ent_y2"), dEntY) ;
	CString strBlkName ; 
	m_RecordSet.GetRecordValue(strSql,_T("sxt_ent_block"),strBlkName);
/*	CXhDevice *pDATA = new CXhDevice(strBlkName) ;*/
	CString strEntName;
	m_RecordSet.GetRecordValue(strSql,_T("sxt_ent_name"),strEntName);
	//cSet.GetFieldValue(_T("sxt_ent_name"),strEntName);
	AcDbEntity * pEnt = NULL ;
	if (m_mysys.OpenAcDbEntity(nHandle, pEnt))
	{
		AcDbBlockReference *pRef = AcDbBlockReference::cast(pEnt) ;
		double dAngle = pRef->rotation() ;
		AcGePoint3d pt_3d_old = pRef->position() ;
		AcDbBlockReference *pRefNew = AcDbBlockReference::cast(pRef->clone()) ;
		m_mysys.SetXdata(pRefNew, _T("CloneFrom"), nHandle) ;
		AcGePoint3d pt_3d_new(dEntX, dEntY, 0) ;
		AcGePoint3d pt_3d_kad=pt_3d_new;
		//pt_3d_kad.y-=10;
		AcGeMatrix3d matrix ;
		AcGeVector3d vec = pt_3d_new - pt_3d_old ;
		matrix.setToTranslation(vec) ;
		pRefNew->transformBy(matrix) ; 
		AcGePoint3d pt_3d_LjlineStart=pRefNew->position();
		m_mysys.AddEntityToDb(pRefNew) ;
		//只有先添加到DWG数据库中，该函数才能执行成功
		long nHandlesx=0;
		nHandlesx=m_mysys.GetHandle(pRefNew);
		//this->GetEntData(nHandlesx,*pDATA);
		CXhDevice *pDATA = new CXhDevice(strBlkName) ;
		this->GetEntData(pRefNew,*pDATA);
		//设置属性至块中
		this->SetEntData(pRefNew,pDATA);
		//pt_3d_LjlineStart.y-=1.5;
		AcGePoint3d pt_3d_LjlineEnd;
		//计轴、LDR、VB、IB、道岔需要添加分线箱
		CString strarray[6]={_T("DT_JZ_000"),_T("DT_JZ_100"),_T("DT_JZ_200"),_T("DT_DHX_000"),_T("DT_YDQ_000"),_T("DT_YDQ_200")};
// 		for(int i=0;i<6;i++)
// 		{
			if(pDATA->m_bHasBox==TRUE)
			{
				//添加KAD，需要判断是在上行线路还是下行线路上
				//判断方法，查询数据库中设备栏，600是上栏，610是下栏
				//m_mysys.InsertBlock()
				//插入信号机FXX
				AcDbBlockReference *pRef2=NULL;
				CString strblkname=_T("DT_BOX_RECT_0");
				//在设备信息中查询设备名称
				strSql.Format(_T("select * from sxt_ent where sxt_ent_name='%s'  and (sxt_ent_type=600 or sxt_ent_type=610)"),strEntName.GetString());
				//strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=923"));
				int i_type=0;
				m_RecordSet.GetRecordValue(strSql,_T("sxt_ent_type"),i_type);
				//cSet.GetFieldValue(_T("sxt_ent_type"),i_type);
				if(i_type==600)
				{
					pt_3d_kad.y+=15;
				}
				else if(i_type==610)
				{
					pt_3d_kad.y-=15;
				}
				pt_3d_LjlineEnd=pt_3d_kad;
				AcGePoint3d pt_3d_Arrow=pt_3d_LjlineEnd;
				if(i_type==600)
				{
					//不同的设备，箱盒电缆线的绘制起点和终点有所不同
					if(strBlkName.Left(6)==_T("DT_DHX"))
					{
						pt_3d_LjlineEnd.y-=1.5;
						pt_3d_LjlineStart.y-=0;
					}
					else if(strBlkName.Left(6)==_T("DT_YDQ"))
					{
						pt_3d_LjlineEnd.y-=1.5;
						pt_3d_LjlineStart.y+=1.5;
					}
					else
					{
						pt_3d_LjlineEnd.y-=1.5;
						pt_3d_LjlineStart.y+=2;

					}
					pt_3d_Arrow.y-=4;
				}
				else if(i_type==610)
				{
					if(strBlkName.Left(6)==_T("DT_DHX"))
					{
						pt_3d_LjlineEnd.y+=1.5;
						pt_3d_LjlineStart.y-=0;
					}
					else if(strBlkName.Left(6)==_T("DT_YDQ"))
					{
						pt_3d_LjlineEnd.y+=1.5;
						pt_3d_LjlineStart.y-=1.5;
					}
					else
					{
						pt_3d_LjlineEnd.y+=1.5;
						pt_3d_LjlineStart.y-=2;

					}
					pt_3d_Arrow.y+=4;
				}
				//pt_3d_header.y+=31;
				long nHandleBox=0;
				//long nHandlesx=0;
				
				if (m_mysys.InsertBlock(pRef2, strblkname, pt_3d_kad,FALSE,0,TRUE))
				{
					CString strTag=_T("TYPE");
					CString str_name=pDATA->m_strBoxType;
					m_mysys.SetAttValue(pRef2,strTag,str_name,FALSE);
					m_mysys.SetXdata(pRef2,_T("BELONGTO_PMT"),nHandle);
					m_mysys.SetXdata(pRef2,_T("BELONG_ENT"),nHandlesx);
					//pRef2->transformBy(matrix);
					nHandleBox=m_mysys.GetHandle(pRef2);
					pRef2->close() ;
					pRef2 = NULL ;
				}
				//创建一条多段线
				//CLineUtil::Add(pt_3d_LjlineStart,pt_3d_LjlineEnd);
				if(pDATA->m_nCable==1)
				{
					long nHandleline=m_mysys.GetHandle(CLineUtil::AddPolyLine(CConvertUtil::ToPoint2d(pt_3d_LjlineStart),CConvertUtil::ToPoint2d(pt_3d_LjlineEnd),0));
					//设置连接线属性
					m_mysys.SetXdata(nHandleline,_T("IS_CABLE"),1);
					m_mysys.SetXdata(nHandleline,_T("IS_ENDCABLE"),1);
					m_mysys.SetXdata(nHandleline,_T("BEGIN"),nHandle);
					m_mysys.SetXdata(nHandleline,_T("BELONG_ENT"),nHandlesx);
					m_mysys.SetXdata(nHandleline,_T("END"),nHandleBox);
					CString strleng=pDATA->m_strCable1;
					m_mysys.SetXdata(nHandleline,_T("CABLESTRING"),strleng);
					long nHandleCableString=0;
					this->DT_SetCableString(nHandleline,nHandleCableString,strleng,0);
				}
				else if(pDATA->m_nCable==2)
				{
					pt_3d_LjlineStart.x-=1;
					pt_3d_LjlineEnd.x-=1;
					long nHandleline=m_mysys.GetHandle(CLineUtil::AddPolyLine(CConvertUtil::ToPoint2d(pt_3d_LjlineStart),CConvertUtil::ToPoint2d(pt_3d_LjlineEnd),0));
					//设置连接线属性
					m_mysys.SetXdata(nHandleline,_T("IS_CABLE"),1);
					m_mysys.SetXdata(nHandleline,_T("IS_ENDCABLE"),1);
					m_mysys.SetXdata(nHandleline,_T("BEGIN"),nHandle);
					m_mysys.SetXdata(nHandleline,_T("BELONG_ENT"),nHandlesx);
					m_mysys.SetXdata(nHandleline,_T("END"),nHandleBox);
					CString strleng=pDATA->m_strCable1;
					m_mysys.SetXdata(nHandleline,_T("CABLESTRING"),strleng);
					long nHandleCableString=0;
					this->DT_SetCableString(nHandleline,nHandleCableString,strleng,0);
					pt_3d_LjlineStart.x+=2;
					pt_3d_LjlineEnd.x+=2;
					long nHandleline2=m_mysys.GetHandle(CLineUtil::AddPolyLine(CConvertUtil::ToPoint2d(pt_3d_LjlineStart),CConvertUtil::ToPoint2d(pt_3d_LjlineEnd),0));
					//设置连接线属性
					m_mysys.SetXdata(nHandleline2,_T("IS_CABLE"),1);
					m_mysys.SetXdata(nHandleline2,_T("IS_ENDCABLE"),1);
					m_mysys.SetXdata(nHandleline2,_T("BEGIN"),nHandle);
					m_mysys.SetXdata(nHandleline2,_T("BELONG_ENT"),nHandlesx);
					m_mysys.SetXdata(nHandleline2,_T("END"),nHandleBox);
					CString strleng2=pDATA->m_strCable2;
					m_mysys.SetXdata(nHandleline2,_T("CABLESTRING"),strleng2);
					long nHandleCableString2=0;
					this->DT_SetCableString(nHandleline2,nHandleCableString2,strleng2,1);
				}
			
				pDATA=NULL;
			}
/*		}*/
		
		pRefNew->setLayer(_T("sxt")) ;
		this->SetEntData(nHandlesx,pDATA);
		pRefNew->close() ;
		pEnt->close() ;
	}
}
// 绘制信号机箱盒
void CZnSxt::DT_DrawBoxJZ(long nHandle,CXhDevice *pData)
{

	CADORecordset cSet(&m_AdoDb) ;
	CString strSql ;
	CXhDevice *pDATA=pData;
	/*	this->ClearEnt(nHandle, 1) ;*/
	//去掉克隆设备
	this->ClearEnt(nHandle, 1) ;
	long dDistBetweenSxHalf=5;
	long nHandleJZpmt=0;
	long nHandleJZSx=nHandle;
	if(m_mysys.GetXdata(nHandle,_T("CloneFrom"),nHandleJZpmt))
	{
		//nHandleJZSx=nHandle;
	}
	strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"),nHandleJZpmt) ; //整除为1的
	double dEntX = 0.000, dEntY = 0.000 ;
	m_RecordSet.GetRecordValue(strSql,_T("sxt_ent_x2"),dEntX);
	m_RecordSet.GetRecordValue(strSql,_T("sxt_ent_y2"),dEntY);
	//cSet.GetFieldValue(_T("sxt_ent_x2"), dEntX) ;
	//cSet.GetFieldValue(_T("sxt_ent_y2"), dEntY) ;
	CString strBlkName ; 
	m_RecordSet.GetRecordValue(strSql,_T("sxt_ent_block"),strBlkName);
	CString strEntName;
	m_RecordSet.GetRecordValue(strSql,_T("sxt_ent_name"),strEntName);
	//cSet.GetFieldValue(_T("sxt_ent_name"),strEntName);
	AcDbEntity * pEnt = NULL ;
	if (m_mysys.OpenAcDbEntity(nHandleJZpmt, pEnt))
	{
		AcDbBlockReference *pRef = AcDbBlockReference::cast(pEnt) ;
		double dAngle = pRef->rotation() ;
		AcGePoint3d pt_3d_old = pRef->position() ;
		AcDbBlockReference *pRefNew = AcDbBlockReference::cast(pRef->clone()) ;
		m_mysys.SetXdata(pRefNew, _T("CloneFrom"), nHandleJZpmt) ;
		AcGePoint3d pt_3d_new(dEntX, dEntY, 0) ;
		AcGePoint3d pt_3d_kad=pt_3d_new;
		//pt_3d_kad.y-=10;
		AcGeMatrix3d matrix ;
		AcGeVector3d vec = pt_3d_new - pt_3d_old ;
		matrix.setToTranslation(vec) ;
		pRefNew->transformBy(matrix) ; 
		AcGePoint3d pt_3d_LjlineStart=pRefNew->position();
		//m_mysys.AddEntityToDb(pRefNew) ;
		//pt_3d_LjlineStart.y-=1.5;
		AcGePoint3d pt_3d_LjlineEnd;
		//计轴、LDR、VB、IB、道岔需要添加分线箱
		CString strarray[6]={_T("DT_JZ_000"),_T("DT_JZ_100"),_T("DT_JZ_200"),_T("DT_DHX_000"),_T("DT_YDQ_000"),_T("DT_YDQ_200")};
// 		for(int i=0;i<6;i++)
// 		{
			if(pDATA->m_bHasBox==TRUE)
			{
				//添加KAD，需要判断是在上行线路还是下行线路上
				//判断方法，查询数据库中设备栏，600是上栏，610是下栏
				//m_mysys.InsertBlock()
				//插入信号机FXX
				AcDbBlockReference *pRef2=NULL;
				CString strblkname=_T("DT_BOX_RECT_0");
				//在设备信息中查询设备名称
				strSql.Format(_T("select * from sxt_ent where sxt_ent_name='%s'  and (sxt_ent_type=600 or sxt_ent_type=610)"),strEntName.GetString());
				//strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=923"));
				int i_type=0;
				m_RecordSet.GetRecordValue(strSql,_T("sxt_ent_type"),i_type);
				//cSet.GetFieldValue(_T("sxt_ent_type"),i_type);
				if(i_type==600)
				{
					pt_3d_kad.y+=15;
				}
				else if(i_type==610)
				{
					pt_3d_kad.y-=15;
				}
				pt_3d_LjlineEnd=pt_3d_kad;
				AcGePoint3d pt_3d_Arrow=pt_3d_LjlineEnd;
				if(i_type==600)
				{
					//不同的设备，箱盒电缆线的绘制起点和终点有所不同
						if(strBlkName.Left(6)==_T("DT_DHX"))
						{
							pt_3d_LjlineEnd.y-=1.5;
							pt_3d_LjlineStart.y-=0;
						}
						else if(strBlkName.Left(6)==_T("DT_YDQ"))
						{
							pt_3d_LjlineEnd.y-=1.5;
							pt_3d_LjlineStart.y+=1.5;
						}
						else
						{
							pt_3d_LjlineEnd.y-=1.5;
							pt_3d_LjlineStart.y+=2;

						}
						pt_3d_Arrow.y-=4;
				}
				else if(i_type==610)
				{
					if(strBlkName.Left(6)==_T("DT_DHX"))
					{
						pt_3d_LjlineEnd.y+=1.5;
						pt_3d_LjlineStart.y-=0;
					}
					else if(strBlkName.Left(6)==_T("DT_YDQ"))
					{
						pt_3d_LjlineEnd.y+=1.5;
						pt_3d_LjlineStart.y-=1.5;
					}
					else
					{
						pt_3d_LjlineEnd.y+=1.5;
						pt_3d_LjlineStart.y-=2;

					}
					pt_3d_Arrow.y+=4;
				}
				//pt_3d_header.y+=31;
				long nHandleBox=0;
				//long nHandlesx=0;
// 				CXhDevice *pDATA = new CXhDevice(strBlkName) ;
// 				nHandlesx=m_mysys.GetHandle(pRefNew);
// 				this->GetEntData(nHandlesx,*pDATA);
				if (m_mysys.InsertBlock(pRef2, strblkname, pt_3d_kad,FALSE,0,TRUE))
				{
					CString strTag=_T("TYPE");
					CString str_name=pDATA->m_strBoxType;
					m_mysys.SetAttValue(pRef2,strTag,str_name,FALSE);
					m_mysys.SetXdata(pRef2,_T("BELONGTO_PMT"),nHandleJZpmt);
					m_mysys.SetXdata(pRef2,_T("BELONG_ENT"),nHandle);
					//pRef2->transformBy(matrix);
					nHandleBox=m_mysys.GetHandle(pRef2);
					pRef2->close() ;
					pRef2 = NULL ;
				}
				//创建一条多段线
				//CLineUtil::Add(pt_3d_LjlineStart,pt_3d_LjlineEnd);
				if(pDATA->m_nCable==1)
				{
					long nHandleline=m_mysys.GetHandle(CLineUtil::AddPolyLine(CConvertUtil::ToPoint2d(pt_3d_LjlineStart),CConvertUtil::ToPoint2d(pt_3d_LjlineEnd),0));
					//设置连接线属性
					m_mysys.SetXdata(nHandleline,_T("IS_CABLE"),1);
					m_mysys.SetXdata(nHandleline,_T("IS_ENDCABLE"),1);
					m_mysys.SetXdata(nHandleline,_T("BEGIN"),nHandle);
					m_mysys.SetXdata(nHandleline,_T("BELONG_ENT"),nHandleJZSx);
					m_mysys.SetXdata(nHandleline,_T("END"),nHandleBox);
					CString strleng=pDATA->m_strCable1;
					m_mysys.SetXdata(nHandleline,_T("CABLESTRING"),strleng);
					long nHandleCableString=0;
					this->DT_SetCableString(nHandleline,nHandleCableString,strleng,0);
				}
				else if(pDATA->m_nCable==2)
				{
					pt_3d_LjlineStart.x-=1;
					pt_3d_LjlineEnd.x-=1;
					long nHandleline=m_mysys.GetHandle(CLineUtil::AddPolyLine(CConvertUtil::ToPoint2d(pt_3d_LjlineStart),CConvertUtil::ToPoint2d(pt_3d_LjlineEnd),0));
					//设置连接线属性
					m_mysys.SetXdata(nHandleline,_T("IS_CABLE"),1);
					m_mysys.SetXdata(nHandleline,_T("IS_ENDCABLE"),1);
					m_mysys.SetXdata(nHandleline,_T("BEGIN"),nHandle);
					m_mysys.SetXdata(nHandleline,_T("BELONG_ENT"),nHandleJZSx);
					m_mysys.SetXdata(nHandleline,_T("END"),nHandleBox);
					CString strleng=pDATA->m_strCable1;
					m_mysys.SetXdata(nHandleline,_T("CABLESTRING"),strleng);
					long nHandleCableString=0;
					this->DT_SetCableString(nHandleline,nHandleCableString,strleng,0);
					pt_3d_LjlineStart.x+=2;
					pt_3d_LjlineEnd.x+=2;
					long nHandleline2=m_mysys.GetHandle(CLineUtil::AddPolyLine(CConvertUtil::ToPoint2d(pt_3d_LjlineStart),CConvertUtil::ToPoint2d(pt_3d_LjlineEnd),0));
					//设置连接线属性
					m_mysys.SetXdata(nHandleline2,_T("IS_CABLE"),1);
					m_mysys.SetXdata(nHandleline2,_T("IS_ENDCABLE"),1);
					m_mysys.SetXdata(nHandleline2,_T("BEGIN"),nHandle);
					m_mysys.SetXdata(nHandleline2,_T("BELONG_ENT"),nHandleJZSx);
					m_mysys.SetXdata(nHandleline2,_T("END"),nHandleBox);
					CString strleng2=pDATA->m_strCable2;
					m_mysys.SetXdata(nHandleline2,_T("CABLESTRING"),strleng2);
					long nHandleCableString2=0;
					this->DT_SetCableString(nHandleline2,nHandleCableString2,strleng2,1);
				}
				pDATA=NULL;
			}
/*		}*/
		pRefNew->setLayer(_T("sxt")) ;

		pRefNew->close() ;
		pEnt->close() ;
	}
}
// 绘制ZZJ箱盒
void CZnSxt::DT_DrawBoxZzj(AcDbBlockReference* pRefJxg, const CDtDcDATA* pDataDc, int iQyIndex, int iDcDirectH)
{
// 	if (pRefJxg!=NULL)
// 	{ 	
		CStringArray strArrayAtt ;
		CString strBlkName = _T("BOX_HZ_0") ;
		//获取基点
		AcGePoint3d pt_3d_jg_insert = pRefJxg->position();
		AcGePoint3d pt_3d_box_insert = pt_3d_jg_insert;
		if(pDataDc->m_strDC_BoxPos==_T("上"))
		{
			pt_3d_box_insert.y+=20;
		}
		else
		{
			pt_3d_box_insert.y-=35;
		}
	   if(pDataDc->m_strDC_BoxType==_T("FXX")||pDataDc->m_strDC_BoxType==_T("KAD"))
	   {
		   strBlkName =_T("DT_BOX_RECT_0");
	   }
	   if(pDataDc->m_bDC_HasBox==FALSE)
	   {
		   //不含盒子
		   return;
	   }
	   else
	   {
		   //含盒子
		   if(pDataDc->m_bDC_ShareBox==TRUE)
		   {
			   //共用一个盒子
		      if(pDataDc->m_nJG_QyNum==1)
			  {
				  //分别各一个盒子
				  strArrayAtt.RemoveAll();
				  strArrayAtt.Add(pDataDc->m_strDC_BoxType) ;
				  AcDbBlockReference* pRef = NULL ;
				  if (m_mysys.InsertBlock(pRef, strBlkName, pt_3d_box_insert, &strArrayAtt, FALSE ))
				  {
					  //m_mysys.SetXdata(pRef, _T("BELONG_DC"), nHandleDc) ;
						long nHandleCurJxg=m_mysys.GetHandle(pRefJxg);
						m_mysys.SetXdata(pRef, _T("BELONG_ENT"), nHandleCurJxg) ;
					  // nHandleBox = m_mysys.GetHandle(pRef) ;
					  pRef->close() ;
					  pRef = NULL ;
				  }

			  }
			  else if(pDataDc->m_nJG_QyNum==2)
			  {
				  pt_3d_box_insert.x+=(iDcDirectH==1?-3:3);
				  //分别各一个盒子
				  strArrayAtt.RemoveAll();
				  strArrayAtt.Add(pDataDc->m_strDC_BoxType) ;
				  AcDbBlockReference* pRef = NULL ;
				  if (m_mysys.InsertBlock(pRef, strBlkName, pt_3d_box_insert, &strArrayAtt, FALSE ))
				  {
					  //m_mysys.SetXdata(pRef, _T("BELONG_DC"), nHandleDc) ;
					long nHandleCurJxg=m_mysys.GetHandle(pRefJxg);
					m_mysys.SetXdata(pRef, _T("BELONG_ENT"), nHandleCurJxg) ;
					  // nHandleBox = m_mysys.GetHandle(pRef) ;
					  pRef->close() ;
					  pRef = NULL ;
				  }

			  }

		   }
		   else
		   {
			   //分别各一个盒子
			   strArrayAtt.RemoveAll();
			   strArrayAtt.Add(pDataDc->m_strDC_BoxType) ;
			   AcDbBlockReference* pRef = NULL ;
			   if (m_mysys.InsertBlock(pRef, strBlkName, pt_3d_box_insert, &strArrayAtt, FALSE ))
			   {
				   //m_mysys.SetXdata(pRef, _T("BELONG_DC"), nHandleDc) ;
				   long nHandleCurJxg=m_mysys.GetHandle(pRefJxg);
				   m_mysys.SetXdata(pRef, _T("BELONG_ENT"), nHandleCurJxg) ;
				  // nHandleBox = m_mysys.GetHandle(pRef) ;
				   pRef->close() ;
				   pRef = NULL ;
			   }
		   }
	   }
// 			if (!strCableString.IsEmpty())
// 			{
// 				AcGePoint3d pt_3d_start, pt_3d_end ;
// 				m_mysys.GetPoint(pt_3d_box, iDcDirectH==1?1.5:-1.5, dAngleFmod, pt_3d_start) ;
// 				m_mysys.GetPoint(pt_3d_box, iDcDirectH==1?2.5:-2.5,  dAngleFmod, pt_3d_end) ;
// 				AcDbObjectId ojbIdLine = m_mysys.AddAcDbPolyLine(pt_3d_start, pt_3d_end) ;
// 				m_mysys.SetXdata(ojbIdLine, _T("BELONG_ENT"),  nHandleBox) ;
// 				long nHandleLine = m_mysys.GetHandle(ojbIdLine) ;
// 				this->DrawCableString(nHandleLine, strCableString) ;
// 
// 				CString strAppNames[8] = {_T("IS_CABLE"), _T("DIST1"), _T("DIST2"), _T("CROSSGD"),  _T("BOXNUM"), _T("PLUS"), _T("BOXFNUM"), _T("INTOXHL")} ;
// 				long iDatas[8] = {1, 0, 0, 0, 2, 0, 0, 0} ;
// 				for (int i=0; i<8; i++)
// 				{
// 					m_mysys.SetXdata(nHandleLine, strAppNames[i], iDatas[i]) ;
// 				}
// 			}            
// 	
	/*   }*/
}
// 绘制ZZJ箱盒
void CZnSxt::DT_DrawBoxZzj(long nHandleDc)
{
	if(nHandleDc==0)
	{
		return;
	}
	else 
	{
		long nHandlePlineBelongDc = 0 ;
		//找到道岔对应的双线中的一条带有尖轨的线
		if (this->GetSxBelongDc(nHandleDc, nHandlePlineBelongDc)==0) //没找到相应的双线，直接退出
		{
			return ;
		}
		//获取属于该道岔的所有牵引点
		struct resbuf* pRb ;
		int nLen = 0 ;
		CLongArray nArrayHandle ;
		CLongArray nArrayHandZZj;
		pRb = acutBuildList(RTDXF0,_T("INSERT"), kDxfRegAppName, _T("BELONG_ENT"), 2, _T("DT_ZZJ_S_000"), 0) ;
		nLen = m_mysys.GetSSEntHandle(_T("X"), NULL, NULL, pRb, nArrayHandle) ;
		acutRelRb(pRb) ;
		for (int i=0; i<nLen; i++)
		{
			long nHandleTmp = 0 ;
			if (m_mysys.GetXdata(nArrayHandle.GetAt(i), _T("BELONG_ENT"), nHandleTmp ))
			{
				if (nHandleTmp == nHandleDc)
				{
					nArrayHandZZj.Add(nArrayHandle.GetAt(i));
				}
			}
		}
		//获取该道岔的属性信息
		CDtDcDATA dcData ;
		int iUpOrDownDc=0;
		this->GetEntData(nHandleDc, dcData) ;
		//没有盒子可以直接返回
		if(dcData.m_bDC_HasBox==FALSE)
			return;
		double dAngleDc = 	m_mysys.GetAngle(nHandleDc) ; //道岔块旋转角度	
		//通过道岔的旋转角度可以判断，道岔位于轨道上方或者下方；旋转角度为180，则在下方；旋转角度为0度，则在上方;
		if(fabs(dAngleDc-PI)<0.2)
		{
			iUpOrDownDc=0;
		}
		else if(fabs(dAngleDc)<0.2)
		{
			iUpOrDownDc=1;
		}
		int iDcDirectV = 0 ; //道岔FW朝向（1，朝上；0，朝下） 
		int iFwIndexVertex = 0 ; //FW端线的顶点vertex（起点或终点）
		AcGePoint2d pt_2d_ext0, pt_2d_ext1, pt_2d_ext2 ; //双线中和online平行的一小段线的端头点和紧邻的点
		AcGeVector2d vecDw ;
		AcDbEntity* pEnt = NULL ;
		if (m_mysys.OpenAcDbEntity(nHandlePlineBelongDc, pEnt, AcDb::kForRead))
		{
			CString strForZZJ ;
			m_mysys.GetXdata(pEnt, _T("ForZZJ"), strForZZJ) ;
			CStringArray strArrayForZZJ ;
			CBlkUtility::DivideString(strForZZJ, _T("-"), strArrayForZZJ) ;
			if (strArrayForZZJ.GetCount()==5)
			{
				CString strTmp = strArrayForZZJ.GetAt(0) ;
				long nHandleDcTmp = 0 ;
				nHandleDcTmp = _ttol(strTmp) ;
				if (nHandleDcTmp==nHandleDc)
				{
					AcDbPolyline* pPolyLine = AcDbPolyline::cast(pEnt) ;
					iDcDirectV = _ttoi(strArrayForZZJ.GetAt(1)) ; //道岔FW朝向（1，朝上；0，朝下） 
					iFwIndexVertex =  _ttoi(strArrayForZZJ.GetAt(2)) ; //FW端线的顶点vertex（起点或终点）
					if (iFwIndexVertex==0)
					{
						pPolyLine->getPointAt(0, pt_2d_ext0) ;
						pPolyLine->getPointAt(1, pt_2d_ext1) ;
						pPolyLine->getPointAt(2, pt_2d_ext2) ;
						vecDw = pt_2d_ext1-pt_2d_ext0 ;
					}
					else
					{
						int iNumOfPline = pPolyLine->numVerts() ;
						pPolyLine->getPointAt(iNumOfPline-1, pt_2d_ext0) ;
						pPolyLine->getPointAt(iNumOfPline-2, pt_2d_ext1) ;
						pPolyLine->getPointAt(iNumOfPline-3, pt_2d_ext2) ;
						vecDw = pt_2d_ext0-pt_2d_ext1 ;
					}
				}
			}
			pEnt->close() ;
		}	
	int iDcDirectH =(iFwIndexVertex==0?0:1) ; //道岔FW水平朝向，0朝右，1，朝左
	CStringArray strArrayAtt ;
	CString strBlkName = _T("BOX_HZ_0") ;
	if(dcData.m_strDC_BoxType==_T("FXX")||dcData.m_strDC_BoxType==_T("KAD"))
	{
		strBlkName =_T("DT_BOX_RECT_0");
	}
	AcGePoint3d pt_3d_LjlineStart;
	AcGePoint3d pt_3d_LjLineMid;
	AcGePoint3d pt_3d_LjlineEnd;
     if(dcData.m_bDC_ShareBox==TRUE)
	 {
		 //放置一个盒子
		 long nHandleTemp=nArrayHandZZj.GetAt(0);
		 //获取牵引点的基点
		 AcGePoint3d pt_3d_jg_insert;
		 m_mysys.GetEntPosition(nHandleTemp,pt_3d_jg_insert);
		 AcGePoint3d pt_3d_box_insert = pt_3d_jg_insert;
		// if(dcData.m_strDC_BoxPos==_T())
		 pt_3d_LjlineStart=pt_3d_jg_insert;
         if(dcData.m_strJG_QyPos==_T("上")) 
		 {
			 pt_3d_LjlineStart.y+=22.5;
		 }
		 else
		 {
			 pt_3d_LjlineStart.y-=22.5;
		 }
		 pt_3d_LjlineEnd=pt_3d_box_insert;
		 if(dcData.m_strDC_BoxPos==_T("上"))
		 {
			 pt_3d_box_insert.y+=20;
			 pt_3d_LjlineEnd.y=pt_3d_box_insert.y-1.5;
		 }
		 else
		 {
			 pt_3d_box_insert.y-=35;
			 pt_3d_LjlineEnd.y=pt_3d_box_insert.y+1.5;
		 }
		 //共用一个盒子
		 if(dcData.m_nJG_QyNum==1)
			{
				//分别各一个盒子
				strArrayAtt.RemoveAll();
				strArrayAtt.Add(dcData.m_strDC_BoxType) ;
				long nHandleBox=0;
				AcDbBlockReference* pRef = NULL ;
				if (m_mysys.InsertBlock(pRef, strBlkName, pt_3d_box_insert, &strArrayAtt, FALSE ))
				{
					m_mysys.SetXdata(pRef, _T("BELONG_ENT"), nHandleDc) ;
					nHandleBox = m_mysys.GetHandle(pRef);
					pRef->close() ;
					pRef = NULL ;
				}
				//添加一条多段线
// 				long nHandleline=m_mysys.GetHandle(CLineUtil::AddPolyLine(CConvertUtil::ToPoint2d(pt_3d_LjlineStart),CConvertUtil::ToPoint2d(pt_3d_LjlineEnd),0));
// 				//设置连接线属性
// 				m_mysys.SetXdata(nHandleline,_T("IS_CABLE"),1);
// 				m_mysys.SetXdata(nHandleline,_T("IS_ENDCABLE"),1);
// 				m_mysys.SetXdata(nHandleline,_T("BEGIN"),nHandleDc);
// 				m_mysys.SetXdata(nHandleline,_T("BELONG_ENT"),nHandleTemp);
// 				m_mysys.SetXdata(nHandleline,_T("END"),nHandleBox);
// 				CString strleng=dcData.m_strCableString;;
// 				m_mysys.SetXdata(nHandleline,_T("CABLESTRING"),strleng);
// 				long nHandleCableString=0;
// 				this->DT_SetCableString(nHandleline,nHandleCableString,strleng,0);
			}
			 else if(dcData.m_nJG_QyNum==2)
			{
				//0道岔反位朝右，1朝左
				pt_3d_box_insert.x+=(iDcDirectH==1?3:-3);
				pt_3d_LjlineEnd=pt_3d_box_insert;
				//分别各一个盒子
				strArrayAtt.RemoveAll();
				strArrayAtt.Add(dcData.m_strDC_BoxType) ;
				AcDbBlockReference* pRef = NULL ;
				if (m_mysys.InsertBlock(pRef, strBlkName, pt_3d_box_insert, &strArrayAtt, FALSE ))
				{
					m_mysys.SetXdata(pRef, _T("BELONG_ENT"), nHandleDc) ;
					pRef->close() ;
					pRef = NULL ;
				}

			}
	 }
	 else
	 {
		 for(int i=0;i<nArrayHandZZj.GetCount();i++)
		 {
			 //放置一个盒子
			 long nHandleTemp=nArrayHandZZj.GetAt(i);
			 //获取牵引点的基点
			 AcGePoint3d pt_3d_jg_insert;
			 m_mysys.GetEntPosition(nHandleTemp,pt_3d_jg_insert);
			 AcGePoint3d pt_3d_box_insert = pt_3d_jg_insert;
			 pt_3d_LjlineStart=pt_3d_jg_insert;
			 if(dcData.m_strJG_QyPos==_T("上")) 
			 {
				 pt_3d_LjlineStart.y+=22.5;
			 }
			 else
			 {
				 pt_3d_LjlineStart.y-=22.5;
			 }
			 pt_3d_LjlineEnd=pt_3d_box_insert;
			 if(dcData.m_strDC_BoxPos==_T("上"))
			 {
				 pt_3d_box_insert.y+=20;
				 pt_3d_LjlineEnd.y=pt_3d_box_insert.y-1.5;
			 }
			 else
			 {
				 pt_3d_box_insert.y-=35;
				 pt_3d_LjlineEnd.y=pt_3d_box_insert.y+1.5;
			 }
			 //分别各一个盒子
			 strArrayAtt.RemoveAll();
			 strArrayAtt.Add(dcData.m_strDC_BoxType) ;
			 AcDbBlockReference* pRef = NULL ;
			 long nHandleBox=0;
			 if (m_mysys.InsertBlock(pRef, strBlkName, pt_3d_box_insert, &strArrayAtt, FALSE ))
			 {
				 m_mysys.SetXdata(pRef, _T("BELONG_ENT"), nHandleDc) ;
				  nHandleBox=m_mysys.GetHandle(pRef);
				 pRef->close() ;
				 pRef = NULL ;
			 }
// 			 //添加一条多段线
// 			 long nHandleline=m_mysys.GetHandle(CLineUtil::AddPolyLine(CConvertUtil::ToPoint2d(pt_3d_LjlineStart),CConvertUtil::ToPoint2d(pt_3d_LjlineEnd),0));
// 			 //设置连接线属性
// 			 m_mysys.SetXdata(nHandleline,_T("IS_CABLE"),1);
// 			 m_mysys.SetXdata(nHandleline,_T("IS_ENDCABLE"),1);
// 			 m_mysys.SetXdata(nHandleline,_T("BEGIN"),nHandleDc);
// 			 m_mysys.SetXdata(nHandleline,_T("BELONG_ENT"),nHandleTemp);
// 			 m_mysys.SetXdata(nHandleline,_T("END"),nHandleBox);
// 			 CString strleng=dcData.m_strCableString;;
// 			 m_mysys.SetXdata(nHandleline,_T("CABLESTRING"),strleng);
// 			 long nHandleCableString=0;
// 			 this->DT_SetCableString(nHandleline,nHandleCableString,strleng,0);
		 }
	 }
}
}
// 绘制指定道岔的转辙机
void CZnSxt::DT_DrawZzj(long nHandleDc)
{
	// 	if (nHandleDc==116158)
	// 	{
	// 		int iii = 0 ;
	// 	}
	//先清除原有图形
	//this->ClearZzj(nHandleDc) ;
	this->ClearEnt(nHandleDc, 1) ;
	long nHandlePlineBelongDc = 0 ;
	//找到道岔对应的双线中的一条带有尖轨的线
	if (this->GetSxBelongDc(nHandleDc, nHandlePlineBelongDc)==0) //没找到相应的双线，直接退出
	{
		return ;
	}
	//保存当前图层，设置要绘制的图层
	AcDbObjectId ojbIdLayerOld = m_pDb->clayer() ;
	m_mysys.SetCLayer(_T("CRSC_SJY_XTS_SXT_DLJL_ZZJ"), 3) ;
	m_iColorIndexCur = 256 ;
	CString strSql ;
	strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleDc) ;
	int iUpOrDownDc = 0 ;
	m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_upordown"), iUpOrDownDc) ;

	//道岔信息
	CDtDcDATA dcData ;
	this->GetEntData(nHandleDc, dcData) ;
	double dAngleDc = 	m_mysys.GetAngle(nHandleDc) ; //道岔块旋转角度	
	//通过道岔的旋转角度可以判断，道岔位于轨道上方或者下方；旋转角度为180，则在下方；旋转角度为0度，则在上方;
	if(fabs(dAngleDc-PI)<0.2)
	{
		iUpOrDownDc=0;
	}
	else if(fabs(dAngleDc)<0.2)
	{
		iUpOrDownDc=1;
	}
	int iDcDirectV = 0 ; //道岔FW朝向（1，朝上；0，朝下） 
	int iFwIndexVertex = 0 ; //FW端线的顶点vertex（起点或终点）
	AcGePoint2d pt_2d_ext0, pt_2d_ext1, pt_2d_ext2 ; //双线中和online平行的一小段线的端头点和紧邻的点
	AcGeVector2d vecDw ;
	AcDbEntity* pEnt = NULL ;
	if (m_mysys.OpenAcDbEntity(nHandlePlineBelongDc, pEnt, AcDb::kForRead))
	{
		CString strForZZJ ;
		m_mysys.GetXdata(pEnt, _T("ForZZJ"), strForZZJ) ;
		CStringArray strArrayForZZJ ;
		CBlkUtility::DivideString(strForZZJ, _T("-"), strArrayForZZJ) ;
		if (strArrayForZZJ.GetCount()==5)
		{
			CString strTmp = strArrayForZZJ.GetAt(0) ;
			long nHandleDcTmp = 0 ;
			nHandleDcTmp = _ttol(strTmp) ;
			if (nHandleDcTmp==nHandleDc)
			{
				AcDbPolyline* pPolyLine = AcDbPolyline::cast(pEnt) ;
				iDcDirectV = _ttoi(strArrayForZZJ.GetAt(1)) ; //道岔FW朝向（1，朝上；0，朝下） 
				iFwIndexVertex =  _ttoi(strArrayForZZJ.GetAt(2)) ; //FW端线的顶点vertex（起点或终点）
				if (iFwIndexVertex==0)
				{
					pPolyLine->getPointAt(0, pt_2d_ext0) ;
					pPolyLine->getPointAt(1, pt_2d_ext1) ;
					pPolyLine->getPointAt(2, pt_2d_ext2) ;
					vecDw = pt_2d_ext1-pt_2d_ext0 ;
				}
				else
				{
					int iNumOfPline = pPolyLine->numVerts() ;
					pPolyLine->getPointAt(iNumOfPline-1, pt_2d_ext0) ;
					pPolyLine->getPointAt(iNumOfPline-2, pt_2d_ext1) ;
					pPolyLine->getPointAt(iNumOfPline-3, pt_2d_ext2) ;
					vecDw = pt_2d_ext0-pt_2d_ext1 ;
				}
			}
		}
		pEnt->close() ;
	}	
	//begin draw zzj
	CStringArray strArrayDrag ;
//	dcData.GetDrag(strArrayDrag, 0) ;
	int iDcDirectH =(iFwIndexVertex==0?0:1) ; //道岔FW水平朝向，0朝右，1，朝左
	//此处对应的线应该为尖轨
	AcGeLineSeg2d line2d(pt_2d_ext0, pt_2d_ext1) ;		
	AcGePoint2d pt_2d_extcen =line2d.midPoint() ;
	AcGePoint2d pt_2d_extcen_mid = pt_2d_extcen ;
	pt_2d_extcen_mid.y+=(iDcDirectV==1?1.5:-1.5) ;
	pt_2d_extcen_mid.rotateBy(vecDw.angle(), pt_2d_extcen) ; //双线中心线
	//映射到双线的一边
	AcGePoint2d pt_2d_extcen_jg = pt_2d_ext0 ;
	//pt_2d_extcen_jg.y +=(iUpOrDownDc==0?-2.5:2.5); 
	double dAngele_zzj=0;
	if(dcData.m_strJG_QyPos==_T("上"))
	{
		pt_2d_extcen_jg.y+=(iUpOrDownDc==1?0:-8);
		dAngele_zzj=0;
	}
	else
	{
		pt_2d_extcen_jg.y+=(iUpOrDownDc==1?+8:0);
		dAngele_zzj=PI;
	}
	pt_2d_extcen_jg.rotateBy(vecDw.angle(), pt_2d_extcen) ;
	AcDbBlockReference* pRefZzj = NULL ;
	CString strBlkNameZzj ;
	strBlkNameZzj.Format(_T("DT_ZZJ_S_000")) ;
	//DT_ZZJ_S_000.dwg
	//strBlkNameZzj.Format(_T("ZZJ_SXT_JX_0")) ;
	//第几动（对于交流直捅到楼的，即使多动也各自按一动处理）
	CString strActionInfo ;
	this->GetDcActionInfo(dcData.m_strJG_QyType, dcData.GetActionNum(), dcData.m_nActIndex==0?1:dcData.m_nActIndex, strActionInfo) ;
	for (int i=0; i<dcData.m_nJG_QyNum; i++)
	{
		/*	double dX_Jg = pt_2d_extcen_jg.x+(iDcDirectH==0?-1:1)*dcData.m_nJG_QyNum*3/2+(iDcDirectH==0?1:-1)*3+i*3*(iDcDirectH==0?1:-1) ;*/
		double dX_Jg = pt_2d_extcen_jg.x+i*(iDcDirectH==0?1:-1)*6;
		AcGePoint2d pt_2d_zzj(dX_Jg, pt_2d_extcen_jg.y) ;
		pt_2d_zzj.rotateBy(vecDw.angle(), pt_2d_extcen_jg) ;
		AcGePoint3d pt_3d_zzj(pt_2d_zzj.x, pt_2d_zzj.y, 0) ;
		CStringArray strArrayVal ;
		CString strVal ;
		//strVal.Format(_T("J%d"), dcData.m_strBoxType) ;
		strVal=dcData.m_strBoxType.Mid(2);
		strArrayVal.Add(strVal) ;	
		//strArrayVal.Add(strArrayDrag.GetAt(i)) ;
		strArrayVal.Add(strActionInfo) ;
		if (m_mysys.InsertBlock(pRefZzj, strBlkNameZzj, pt_3d_zzj, &strArrayVal, FALSE))
		{
			m_mysys.RotationEnt(pRefZzj, vecDw.angle()+dAngele_zzj, pt_3d_zzj) ;
			m_mysys.SetXdata(pRefZzj, _T("IS_JXG"), 1) ;
			m_mysys.SetXdata(pRefZzj, _T("BELONG_ENT"), nHandleDc) ;
			//m_mysys.SetXdata(pRefZzj, _T("ACTION_INFO"), strActionInfo) ;
			//绘制对应的箱盒
// 			if(dcData.m_bDC_ShareBox==FALSE)
// 				this->DT_DrawBoxZzj(pRefZzj, &dcData, i,  iDcDirectH) ;
			pRefZzj->close() ;
			pRefZzj = NULL ;
		}										
	}
	/*if(dcData.m_bDC_ShareBox==TRUE)*/
	this->DT_DrawBoxZzj(nHandleDc);
	//求芯轨绘制中心点
	strArrayDrag.RemoveAll() ;
//	dcData.GetDrag(strArrayDrag, 1) ;
	AcGePoint2d pt_2d_xg(0,0) ; //芯轨绘制中心点
	AcGeLineSeg2d lineseg2d_xg1(pt_2d_ext1 , pt_2d_ext2) ;
	AcGePoint3d pt_3d_xg ;
	this->GetXgInsertPt(nHandleDc, iDcDirectV, pt_3d_xg) ;	
	pt_2d_xg = pt_3d_xg.convert2d(AcGePlane::kXYPlane) ;
	//第几动（对于交流直捅到楼的，即使多动也各自按一动处理）
	strActionInfo = _T("") ;
	//end draw zzj
	m_pDb->setClayer(ojbIdLayerOld) ; //恢复当前图层
}
void CZnSxt::testdtpmt_0()
{
	//COperitionExcel oper_excel;
// 	CString strFileSave;
// 	if(FALSE==this->SelectFilePath(strFilePath))
// 	{
// 		AfxMessageBox(_T("指定的文件路径无效！"));
// 		return ;
// 	}
	//先创建字体样式
	AcDbObjectId  idTextStyleTitle, idTextStyleContent, idTextStyleChs,idTextStyleHZ,idTextStyleStandard;
	idTextStyleTitle = m_mysys.CreateTextStyleByFileName(_T("zhffontTitle"), _T("txt.shx"), _T("hztxt2.shx"), 4, 0.7) ;	
	idTextStyleContent = m_mysys.CreateTextStyleByFileName(_T("zhffontContent"), _T("txt.shx"), _T("hztxt2.shx"), 3, 0.7) ;
	idTextStyleChs = m_mysys.CreateTextStyle(_T("CHINESE"), _T("Times New Roman"), _T(""), 3, 0.55) ;
	idTextStyleHZ = m_mysys.CreateTextStyle(_T("HZ"), _T("Times New Roman"), _T(""), 3, 0.55) ;
	m_mysys.ModifyFontStyle(_T("Standard"), _T("Times New Roman"),FALSE) ;

	CString strFilePath ;
	//strFilePath.Format(_T("%s\\support\\dt\\方庄平面图设计.xls"), g_strBasePath);
	if(FALSE==this->SelectOpenFile(strFilePath))
	{
		//AfxMessageBox(_T("指定的文件路径无效！"));
		return ;
	}
	oper_excel.OpenExcel(strFilePath);
	double d_data1=0.0;
	CString str_data1;
	int i_data1=0;
   oper_excel.ReadDatafromExcel(1,4,2,d_data1);
   oper_excel.ReadDatafromExcel(1,4,2,str_data1);
   oper_excel.ReadDatafromExcel(1,4,2,i_data1);
	acutPrintf(_T("\n d_data1=%.3f \t str_data1=%s\t i_data1=%d"),d_data1,str_data1,i_data1);
	oper_excel.ReadDatafromExcel(1,5,2,d_data1);
	oper_excel.ReadDatafromExcel(1,5,2,str_data1);
	oper_excel.ReadDatafromExcel(1,5,2,i_data1);
	acutPrintf(_T("\n d_data1=%.3f \t str_data1=%s\t i_data1=%d"),d_data1,str_data1,i_data1);
	bool bret=oper_excel.IsCellCString(1,4,2);
	bool bret2=oper_excel.IsCellDouble(1,4,2);
	bool bret3=oper_excel.IsCellInt(1,4,2);
	acutPrintf(_T("\n d_data1=%d \t str_data1=%d\t i_data=%d"),bret,bret2,bret3);
	int iRowCnt=0;
	int iColCnt=0;
	int iColCnt1=0;
	oper_excel.GetRowCount(1,iRowCnt);
	oper_excel.GetColumnCount(1,iColCnt);
	acutPrintf(_T("\n sheet1=%d 行，%d 列"));
	//下行里标1、里标2、设备名称、设备类型、所在线、位置（上下）、备注信息
	vector<double> d_vec_libiao_a;
	vector<double> d_vec_libiao2_a;
	vector<CString> str_vec_name_a;
	vector<CString> str_vec_deviceType_a;
	vector<int> i_vec_line_a; 
	vector<int> i_veczhanType_a;
	vector<CString> str_vec_info_a;
	//上行里标1、里标2、设备名称、设备类型、所在线、位置（上下）、备注信息
	vector<double> d_vec_libiao_b;
	vector<double> d_vec_libiao2_b;
	vector<CString> str_vec_name_b;
	vector<CString> str_vec_deviceType_b;
	vector<int> i_vec_line_b;
	vector<int>i_veczhanType_b;
	vector<CString>str_vec_info_b;
	vector<double> d_vec_libiao_he;
    vector<double> d_vecx;
	vector<double> d_veczhantai_a;
	vector<double> d_veczhantai_b;
	for(int i=3;i<=iRowCnt;i++)
	{
		double d_data=0;
		CString str_data;
		int i_data=-1;
		oper_excel.ReadDatafromExcel(1,i,2,d_data);
 		if(fabs(d_data)>1.0)
 		{
			d_vec_libiao_a.push_back(d_data);
			
			oper_excel.ReadDatafromExcel(1,i,3,str_data);
			str_vec_name_a.push_back(str_data);
			oper_excel.ReadDatafromExcel(1,i,5,str_data);
			str_vec_deviceType_a.push_back(str_data);
			if(str_data==_T("ZTD"))
			{
				d_veczhantai_a.push_back(d_data);
			}
			oper_excel.ReadDatafromExcel(1,i,4,d_data);
			d_vec_libiao2_a.push_back(d_data);
			oper_excel.ReadDatafromExcel(1,i,7,i_data);
			i_veczhanType_a.push_back(i_data);	
			oper_excel.ReadDatafromExcel(1,i,6,i_data);
			if(i_data==0)
			{
				i_data=2;
			}
			i_vec_line_a.push_back(i_data);
			oper_excel.ReadDatafromExcel(1,i,9,str_data);
			str_vec_info_a.push_back(str_data);
		}
		//else
		//	break;
		oper_excel.ReadDatafromExcel(1,i,11,d_data);
		if(fabs(d_data)>1.0)
		{
			d_vec_libiao_b.push_back(d_data);	
			oper_excel.ReadDatafromExcel(1,i,12,str_data);
			str_vec_name_b.push_back(str_data);
			oper_excel.ReadDatafromExcel(1,i,14,str_data);
			str_vec_deviceType_b.push_back(str_data);
			if(str_data==_T("ZTD"))
			{
				d_veczhantai_b.push_back(d_data);
			}
			oper_excel.ReadDatafromExcel(1,i,13,d_data);
			d_vec_libiao2_b.push_back(d_data);
			i_data=-1;
			oper_excel.ReadDatafromExcel(1,i,16,i_data);
			i_veczhanType_b.push_back(i_data);	
			oper_excel.ReadDatafromExcel(1,i,15,i_data);
			if(i_data==0)
			{
				i_data=6;
			}
			i_vec_line_b.push_back(i_data);
			oper_excel.ReadDatafromExcel(1,i,18,str_data);
			str_vec_info_b.push_back(str_data);
		}	
	}
// 	for(int k=0;k<d_vec_libiao_a.size();k++)
// 	{
// 		acutPrintf(_T("\n%d,libiao_a=%.3f,name_a=%s,type_a=%s,i_zhantai_a=%d,size=%d"),k,d_vec_libiao_a[k],str_vec_name_a[k],str_vec_deviceType_a[k],i_veczhanType_a[k],d_vec_libiao_a.size());
// 	}
// 	for(int m=0;m<d_vec_libiao_b.size();m++)
// 	{
// 		acutPrintf(_T("\n%d,libiao_b=%.3f,name_b=%s,type_b=%s,i_zhantaib=%d,size=%d"),m,d_vec_libiao_b[m],str_vec_name_b[m],str_vec_deviceType_b[m],i_veczhanType_b[m],d_vec_libiao_b.size());
// 	}
	CDtpmt my_dt;
	int d_max=10;
	int d_min=6;
	acedGetInt(_T("\n设置相邻设备最大间隔(mm)："),&d_max);
	acedGetInt(_T("\n设置相邻设备最小间隔(mm):"),&d_min);
	acutPrintf(_T("\n d_max=%d,d_min=%d"),d_max,d_min);
	my_dt.ReLay3(d_vec_libiao_a,str_vec_name_a,d_vec_libiao2_a,str_vec_deviceType_a,i_vec_line_a,i_veczhanType_a,str_vec_info_a,d_vec_libiao_b,str_vec_name_b,d_vec_libiao2_b,str_vec_deviceType_b,i_vec_line_b,i_veczhanType_b,str_vec_info_b,d_vec_libiao_he,d_vecx,(double)d_max,(double)d_min);
   //读取上下数据
  vector<double> d_vec_po_up;
  vector<CString> str_vec_podu_up;
  vector<CString> str_vec_polength_up;
  vector<int> i_vec_po_up_sx;
  vector<double> d_vec_po_down;
  vector<CString> str_vec_podu_down;
  vector<CString> str_vec_polength_down;
  vector<int> i_vec_po_down_sx;
  oper_excel.GetRowCount(3,iRowCnt);
  oper_excel.GetColumnCount(3,iColCnt);
  acutPrintf(_T("sheet3=%d行，%d列"),iRowCnt,iColCnt);
  for(int i=3;i<iRowCnt;i++)
  {
	  double d_data=0;
	  oper_excel.ReadDatafromExcel(3,i,3,d_data);
	  d_vec_po_up.push_back(d_data);
	  oper_excel.ReadDatafromExcel(3,i,11,d_data);
	  d_vec_po_down.push_back(d_data);
	  CString str_data;
	  oper_excel.ReadDatafromExcel(3,i,4,str_data);
	  str_vec_polength_up.push_back(str_data);
	  oper_excel.ReadDatafromExcel(3,i,12,str_data);
	  str_vec_polength_down.push_back(str_data);
	  oper_excel.ReadDatafromExcel(3,i,5,str_data);
	  str_vec_podu_up.push_back(str_data);
	  oper_excel.ReadDatafromExcel(3,i,13,str_data);
	  str_vec_podu_down.push_back(str_data);
	  int i_data=-1;
	   oper_excel.ReadDatafromExcel(3,i,6,i_data);
	  i_vec_po_up_sx.push_back(i_data);
	  oper_excel.ReadDatafromExcel(3,i,14,i_data);
	  i_vec_po_down_sx.push_back(i_data);
  }
	for(int k=0;k<iRowCnt-3;k++)
	{
		//acutPrintf(_T("\n%d\t  %.3f\t %s\t %s\t  %d\t"),k,d_vec_po_up[k],str_vec_polength_up[k],str_vec_podu_up[k],i_vec_po_up_sx[k]);
	}
	my_dt.AddPodu_0(d_vec_libiao_he,d_vecx,d_vec_po_up,str_vec_polength_up,str_vec_podu_up,i_vec_po_up_sx,true);
	my_dt.AddPodu_0(d_vec_libiao_he,d_vecx,d_vec_po_down,str_vec_polength_down,str_vec_podu_down,i_vec_po_down_sx,false);
   //读取曲线半径数据
	vector<double> d_qx_start_up;
	vector<double> d_qx_end_up;
    vector<CString> str_qx_ration_up;
	vector<int> i_qx_at_up;
	vector<double> d_qx_start_down;
	vector<double> d_qx_end_down;
	vector<CString> str_qx_ration_down;
	vector<int> i_qx_at_down;
	oper_excel.GetRowCount(2,iRowCnt);
	oper_excel.GetColumnCount(2,iColCnt);
	acutPrintf(_T("sheet2 =%d行，%d列"),iRowCnt,iColCnt);
	for(int i=3;i<iRowCnt;i++)
	{
		double d_data=0;
		oper_excel.ReadDatafromExcel(2,i,2,d_data);
		d_qx_start_up.push_back(d_data);
		//d_vec_po_up.push_back(d_data);
		oper_excel.ReadDatafromExcel(2,i,3,d_data);
		d_qx_end_up.push_back(d_data);
		CString str_data;
		oper_excel.ReadDatafromExcel(2,i,4,str_data);
		str_qx_ration_up.push_back(str_data);
		int i_data=-1;
		oper_excel.ReadDatafromExcel(2,i,5,i_data);
		i_qx_at_up.push_back(i_data);
		oper_excel.ReadDatafromExcel(2,i,9,d_data);
		d_qx_start_down.push_back(d_data);
		//d_vec_po_up.push_back(d_data);
		oper_excel.ReadDatafromExcel(2,i,10,d_data);
		d_qx_end_down.push_back(d_data);
	//	CString str_data;
		oper_excel.ReadDatafromExcel(2,i,11,str_data);
		str_qx_ration_down.push_back(str_data);
//		int i_data=-1;
		oper_excel.ReadDatafromExcel(2,i,12,i_data);
		i_qx_at_down.push_back(i_data);
	
	}
	my_dt.AddQuXian_0(d_vec_libiao_he,d_vecx,d_qx_start_up,d_qx_end_up,str_qx_ration_up,i_qx_at_up,true);
	my_dt.AddQuXian_0(d_vec_libiao_he,d_vecx,d_qx_start_down,d_qx_end_down,str_qx_ration_down,i_qx_at_down,false);
	my_dt.AddZhantai(str_vec_deviceType_a,d_vec_libiao_a,d_vec_libiao_he,d_vecx,d_veczhantai_a,i_veczhanType_a);
	oper_excel.RealseExcel();
}
void CZnSxt::testdtpmt()
{
	//COperitionExcel oper_excel;
	// 	CString strFileSave;
	// 	if(FALSE==this->SelectFilePath(strFilePath))
	// 	{
	// 		AfxMessageBox(_T("指定的文件路径无效！"));
	// 		return ;
	// 	}
	//先创建字体样式
	AcDbObjectId  idTextStyleTitle, idTextStyleContent, idTextStyleChs,idTextStyleHZ,idTextStyleStandard;
	idTextStyleTitle = m_mysys.CreateTextStyleByFileName(_T("zhffontTitle"), _T("txt.shx"), _T("hztxt2.shx"), 4, 0.7) ;	
	idTextStyleContent = m_mysys.CreateTextStyleByFileName(_T("zhffontContent"), _T("txt.shx"), _T("hztxt2.shx"), 3, 0.7) ;
	idTextStyleChs = m_mysys.CreateTextStyle(_T("CHINESE"), _T("Times New Roman"), _T(""), 3, 0.55) ;
	idTextStyleHZ = m_mysys.CreateTextStyle(_T("HZ"), _T("Times New Roman"), _T(""), 3, 0.55) ;
	m_mysys.ModifyFontStyle(_T("Standard"), _T("Times New Roman"),FALSE) ;

	CString strFilePath ;
	//strFilePath.Format(_T("%s\\support\\dt\\方庄平面图设计.xls"), g_strBasePath);
	if(FALSE==this->SelectOpenFile(strFilePath))
	{
		//AfxMessageBox(_T("指定的文件路径无效！"));
		return ;
	}
	oper_excel.OpenExcel(strFilePath);
	double d_data1=0.0;
	CString str_data1;
	int i_data1=0;
	oper_excel.ReadDatafromExcel(1,4,2,d_data1);
	oper_excel.ReadDatafromExcel(1,4,2,str_data1);
	oper_excel.ReadDatafromExcel(1,4,2,i_data1);
	acutPrintf(_T("\n d_data1=%.3f \t str_data1=%s\t i_data1=%d"),d_data1,str_data1,i_data1);
	oper_excel.ReadDatafromExcel(1,5,2,d_data1);
	oper_excel.ReadDatafromExcel(1,5,2,str_data1);
	oper_excel.ReadDatafromExcel(1,5,2,i_data1);
	acutPrintf(_T("\n d_data1=%.3f \t str_data1=%s\t i_data1=%d"),d_data1,str_data1,i_data1);
	bool bret=oper_excel.IsCellCString(1,4,2);
	bool bret2=oper_excel.IsCellDouble(1,4,2);
	bool bret3=oper_excel.IsCellInt(1,4,2);
	acutPrintf(_T("\n d_data1=%d \t str_data1=%d\t i_data=%d"),bret,bret2,bret3);
	int iRowCnt=0;
	int iColCnt=0;
	int iColCnt1=0;
	oper_excel.GetRowCount(1,iRowCnt);
	oper_excel.GetColumnCount(1,iColCnt);
	acutPrintf(_T("\n sheet1=%d 行，%d 列"));
	//下行里标1、里标2、设备名称、设备类型、所在线、位置（上下）、备注信息
	vector<double> d_vec_libiao_a;
	vector<double> d_vec_libiao2_a;
	vector<CString> str_vec_name_a;
	vector<CString> str_vec_deviceType_a;
	vector<int> i_vec_line_a; 
	vector<int> i_veczhanType_a;
	vector<CString> str_vec_info_a;
	//上行里标1、里标2、设备名称、设备类型、所在线、位置（上下）、备注信息
	vector<double> d_vec_libiao_b;
	vector<double> d_vec_libiao2_b;
	vector<CString> str_vec_name_b;
	vector<CString> str_vec_deviceType_b;
	vector<int> i_vec_line_b;
	vector<int>i_veczhanType_b;
	vector<CString>str_vec_info_b;
	vector<double> d_vec_libiao_he;
	vector<double> d_vecx;
	vector<double> d_veczhantai_a;
	vector<double> d_veczhantai_b;
	for(int i=3;i<=iRowCnt;i++)
	{
		double d_data=0;
		CString str_data;
		int i_data=-1;
		oper_excel.ReadDatafromExcel(1,i,2,d_data);
		if(fabs(d_data)>1.0)
		{
			d_vec_libiao_a.push_back(d_data);

			oper_excel.ReadDatafromExcel(1,i,3,str_data);
			str_vec_name_a.push_back(str_data);
			oper_excel.ReadDatafromExcel(1,i,5,str_data);
			str_vec_deviceType_a.push_back(str_data);
			if(str_data==_T("ZTD"))
			{
				d_veczhantai_a.push_back(d_data);
			}
			oper_excel.ReadDatafromExcel(1,i,4,d_data);
			d_vec_libiao2_a.push_back(d_data);
			oper_excel.ReadDatafromExcel(1,i,7,i_data);
			i_veczhanType_a.push_back(i_data);	
			oper_excel.ReadDatafromExcel(1,i,6,i_data);
			if(i_data==0)
			{
				i_data=2;
			}
			i_vec_line_a.push_back(i_data);
			oper_excel.ReadDatafromExcel(1,i,9,str_data);
			str_vec_info_a.push_back(str_data);
		}
		//else
		//	break;
		oper_excel.ReadDatafromExcel(1,i,11,d_data);
		if(fabs(d_data)>1.0)
		{
			d_vec_libiao_b.push_back(d_data);	
			oper_excel.ReadDatafromExcel(1,i,12,str_data);
			str_vec_name_b.push_back(str_data);
			oper_excel.ReadDatafromExcel(1,i,14,str_data);
			str_vec_deviceType_b.push_back(str_data);
			if(str_data==_T("ZTD"))
			{
				d_veczhantai_b.push_back(d_data);
			}
			oper_excel.ReadDatafromExcel(1,i,13,d_data);
			d_vec_libiao2_b.push_back(d_data);
			i_data=-1;
			oper_excel.ReadDatafromExcel(1,i,16,i_data);
			i_veczhanType_b.push_back(i_data);	
			oper_excel.ReadDatafromExcel(1,i,15,i_data);
			if(i_data==0)
			{
				i_data=6;
			}
			i_vec_line_b.push_back(i_data);
			oper_excel.ReadDatafromExcel(1,i,18,str_data);
			str_vec_info_b.push_back(str_data);
		}	
	}
	// 	for(int k=0;k<d_vec_libiao_a.size();k++)
	// 	{
	// 		acutPrintf(_T("\n%d,libiao_a=%.3f,name_a=%s,type_a=%s,i_zhantai_a=%d,size=%d"),k,d_vec_libiao_a[k],str_vec_name_a[k],str_vec_deviceType_a[k],i_veczhanType_a[k],d_vec_libiao_a.size());
	// 	}
	// 	for(int m=0;m<d_vec_libiao_b.size();m++)
	// 	{
	// 		acutPrintf(_T("\n%d,libiao_b=%.3f,name_b=%s,type_b=%s,i_zhantaib=%d,size=%d"),m,d_vec_libiao_b[m],str_vec_name_b[m],str_vec_deviceType_b[m],i_veczhanType_b[m],d_vec_libiao_b.size());
	// 	}
	CDtpmt my_dt;
	int d_max=10;
	int d_min=6;
	acedGetInt(_T("\n设置相邻设备最大间隔(mm)："),&d_max);
	acedGetInt(_T("\n设置相邻设备最小间隔(mm):"),&d_min);
	acutPrintf(_T("\n d_max=%d,d_min=%d"),d_max,d_min);
	my_dt.ReLay2(d_vec_libiao_a,str_vec_name_a,d_vec_libiao2_a,str_vec_deviceType_a,i_vec_line_a,i_veczhanType_a,str_vec_info_a,d_vec_libiao_b,str_vec_name_b,d_vec_libiao2_b,str_vec_deviceType_b,i_vec_line_b,i_veczhanType_b,str_vec_info_b,d_vec_libiao_he,d_vecx,(double)d_max,(double)d_min);
	//读取上下数据
	vector<double> d_vec_po_up;
	vector<CString> str_vec_podu_up;
	vector<CString> str_vec_polength_up;
	vector<int> i_vec_po_up_sx;
	vector<double> d_vec_po_down;
	vector<CString> str_vec_podu_down;
	vector<CString> str_vec_polength_down;
	vector<int> i_vec_po_down_sx;
	oper_excel.GetRowCount(3,iRowCnt);
	oper_excel.GetColumnCount(3,iColCnt);
	acutPrintf(_T("sheet3=%d行，%d列"),iRowCnt,iColCnt);
	for(int i=3;i<iRowCnt;i++)
	{
		double d_data=0;
		oper_excel.ReadDatafromExcel(3,i,3,d_data);
		d_vec_po_up.push_back(d_data);
		oper_excel.ReadDatafromExcel(3,i,11,d_data);
		d_vec_po_down.push_back(d_data);
		CString str_data;
		oper_excel.ReadDatafromExcel(3,i,4,str_data);
		str_vec_polength_up.push_back(str_data);
		oper_excel.ReadDatafromExcel(3,i,12,str_data);
		str_vec_polength_down.push_back(str_data);
		oper_excel.ReadDatafromExcel(3,i,5,str_data);
		str_vec_podu_up.push_back(str_data);
		oper_excel.ReadDatafromExcel(3,i,13,str_data);
		str_vec_podu_down.push_back(str_data);
		int i_data=-1;
		oper_excel.ReadDatafromExcel(3,i,6,i_data);
		i_vec_po_up_sx.push_back(i_data);
		oper_excel.ReadDatafromExcel(3,i,14,i_data);
		i_vec_po_down_sx.push_back(i_data);
	}
	for(int k=0;k<iRowCnt-3;k++)
	{
		//acutPrintf(_T("\n%d\t  %.3f\t %s\t %s\t  %d\t"),k,d_vec_po_up[k],str_vec_polength_up[k],str_vec_podu_up[k],i_vec_po_up_sx[k]);
	}
	my_dt.AddPodu(d_vec_libiao_he,d_vecx,d_vec_po_up,str_vec_polength_up,str_vec_podu_up,i_vec_po_up_sx,true);
	my_dt.AddPodu(d_vec_libiao_he,d_vecx,d_vec_po_down,str_vec_polength_down,str_vec_podu_down,i_vec_po_down_sx,false);
	//读取曲线半径数据
	vector<double> d_qx_start_up;
	vector<double> d_qx_end_up;
	vector<CString> str_qx_ration_up;
	vector<int> i_qx_at_up;
	vector<double> d_qx_start_down;
	vector<double> d_qx_end_down;
	vector<CString> str_qx_ration_down;
	vector<int> i_qx_at_down;
	oper_excel.GetRowCount(2,iRowCnt);
	oper_excel.GetColumnCount(2,iColCnt);
	acutPrintf(_T("sheet2 =%d行，%d列"),iRowCnt,iColCnt);
	for(int i=3;i<iRowCnt;i++)
	{
		double d_data=0;
		oper_excel.ReadDatafromExcel(2,i,2,d_data);
		d_qx_start_up.push_back(d_data);
		//d_vec_po_up.push_back(d_data);
		oper_excel.ReadDatafromExcel(2,i,3,d_data);
		d_qx_end_up.push_back(d_data);
		CString str_data;
		oper_excel.ReadDatafromExcel(2,i,4,str_data);
		str_qx_ration_up.push_back(str_data);
		int i_data=-1;
		oper_excel.ReadDatafromExcel(2,i,5,i_data);
		i_qx_at_up.push_back(i_data);
		oper_excel.ReadDatafromExcel(2,i,9,d_data);
		d_qx_start_down.push_back(d_data);
		//d_vec_po_up.push_back(d_data);
		oper_excel.ReadDatafromExcel(2,i,10,d_data);
		d_qx_end_down.push_back(d_data);
		//	CString str_data;
		oper_excel.ReadDatafromExcel(2,i,11,str_data);
		str_qx_ration_down.push_back(str_data);
		//		int i_data=-1;
		oper_excel.ReadDatafromExcel(2,i,12,i_data);
		i_qx_at_down.push_back(i_data);

	}
	my_dt.AddQuXian(d_vec_libiao_he,d_vecx,d_qx_start_up,d_qx_end_up,str_qx_ration_up,i_qx_at_up,true);
	my_dt.AddQuXian(d_vec_libiao_he,d_vecx,d_qx_start_down,d_qx_end_down,str_qx_ration_down,i_qx_at_down,false);
	my_dt.AddZhantai(str_vec_deviceType_a,d_vec_libiao_a,d_vec_libiao_he,d_vecx,d_veczhantai_a,i_veczhanType_a);
	oper_excel.RealseExcel();
}
//在处理数据前，需要先对各个设备信息表进行汇总
void CZnSxt::testdtpmt2()
{
	//先创建字体样式
	AcDbObjectId  idTextStyleTitle, idTextStyleContent, idTextStyleChs,idTextStyleHZ,idTextStyleStandard;
	idTextStyleTitle = m_mysys.CreateTextStyleByFileName(_T("zhffontTitle"), _T("txt.shx"), _T("hztxt2.shx"), 4, 0.7) ;	
	idTextStyleContent = m_mysys.CreateTextStyleByFileName(_T("zhffontContent"), _T("txt.shx"), _T("hztxt2.shx"), 3, 0.7) ;
	idTextStyleChs = m_mysys.CreateTextStyle(_T("CHINESE"), _T("Times New Roman"), _T(""), 3, 0.55) ;
	idTextStyleHZ = m_mysys.CreateTextStyle(_T("HZ"), _T("Times New Roman"), _T(""), 3, 0.55) ;
	m_mysys.ModifyFontStyle(_T("Standard"), _T("Times New Roman"),FALSE) ;

	CString strFilePath ;
	//strFilePath.Format(_T("%s\\support\\dt\\方庄平面图设计.xls"), g_strBasePath);
	if(FALSE==this->SelectOpenFile(strFilePath))
	{
		//AfxMessageBox(_T("指定的文件路径无效！"));
		return ;
	}
	oper_excel.OpenExcel(strFilePath);
	double d_data1=0.0;
	CString str_data1;
	int i_data1=0;
	oper_excel.ReadDatafromExcel(1,4,2,d_data1);
	oper_excel.ReadDatafromExcel(1,4,2,str_data1);
	oper_excel.ReadDatafromExcel(1,4,2,i_data1);
	acutPrintf(_T("\n d_data1=%.3f \t str_data1=%s\t i_data1=%d"),d_data1,str_data1,i_data1);
	oper_excel.ReadDatafromExcel(1,5,2,d_data1);
	oper_excel.ReadDatafromExcel(1,5,2,str_data1);
	oper_excel.ReadDatafromExcel(1,5,2,i_data1);
	acutPrintf(_T("\n d_data1=%.3f \t str_data1=%s\t i_data1=%d"),d_data1,str_data1,i_data1);
	bool bret=oper_excel.IsCellCString(1,4,2);
	bool bret2=oper_excel.IsCellDouble(1,4,2);
	bool bret3=oper_excel.IsCellInt(1,4,2);
	acutPrintf(_T("\n d_data1=%d \t str_data1=%d\t i_data=%d"),bret,bret2,bret3);
	int iColCnt1=0;
	//下行里标1、里标2、设备名称、设备类型、所在线、位置（上下）、备注信息
	vector<double> d_vec_libiao_a;
	vector<double> d_vec_libiao2_a;
	vector<CString> str_vec_name_a;
	vector<CString> str_vec_deviceType_a;
	vector<int> i_vec_line_a; 
	vector<int> i_veczhanType_a;
	vector<CString> str_vec_info_a;
	//上行里标1、里标2、设备名称、设备类型、所在线、位置（上下）、备注信息
	vector<double> d_vec_libiao_b;
	vector<double> d_vec_libiao2_b;
	vector<CString> str_vec_name_b;
	vector<CString> str_vec_deviceType_b;
	vector<int> i_vec_line_b;
	vector<int>i_veczhanType_b;
	vector<CString>str_vec_info_b;
	vector<double> d_vec_libiao_he;
	vector<double> d_vecx;
	vector<double> d_veczhantai_a;
	vector<double> d_veczhantai_b;
	//装入下行数据
	vector<CDeviceData> Device_vecData_a;
	//装入上行数据
	vector<CDeviceData> Device_vecData_b;
	int i_excel=0;
	for(i_excel=1;i_excel<=7;i_excel++)
	{
		int iRowCnt=0;
		int iColCnt=0;
		oper_excel.GetRowCount(i_excel,iRowCnt);
		oper_excel.GetColumnCount(i_excel,iColCnt);
		acutPrintf(_T("\n sheet1=%d 行，%d 列"));
		for(int i=3;i<=iRowCnt;i++)
		{
			double d_data=0;
			CString str_data;
			CDeviceData Dev_Data;
			int i_data=-1;
			oper_excel.ReadDatafromExcel(i_excel,i,2,d_data);
			if(fabs(d_data)>1.0)
			{
				//d_vec_libiao_a.push_back(d_data);
				Dev_Data.m_dLibiao1 =d_data;
				oper_excel.ReadDatafromExcel(i_excel,i,3,str_data);
				Dev_Data.m_strDevname=str_data;
				//str_vec_name_a.push_back(str_data);
				oper_excel.ReadDatafromExcel(i_excel,i,5,str_data);
				//str_vec_deviceType_a.push_back(str_data);
				Dev_Data.m_strDevType=str_data;
				oper_excel.ReadDatafromExcel(i_excel,i,4,d_data);
				//d_vec_libiao2_a.push_back(d_data);
				Dev_Data.m_dLibiao2=d_data;
				oper_excel.ReadDatafromExcel(i_excel,i,7,i_data);
				//i_veczhanType_a.push_back(i_data);	
				Dev_Data.m_iupordown=i_data;
				oper_excel.ReadDatafromExcel(i_excel,i,6,i_data);
				if(i_data==0)
				{
					i_data=2;
				}
				//i_vec_line_a.push_back(i_data);
				Dev_Data.m_iLine=i_data;
				oper_excel.ReadDatafromExcel(i_excel,i,9,str_data);
				//str_vec_info_a.push_back(str_data);
				Dev_Data.m_strRemarks = str_data;
				Device_vecData_a.push_back(Dev_Data);
				//Device_vecData_a.push_back(Dev_Data);
			}
			
			CDeviceData Dev_Data_b;
			oper_excel.ReadDatafromExcel(i_excel,i,11,d_data);
			if(fabs(d_data)>1.0)
			{
				//d_vec_libiao_b.push_back(d_data);	
				Dev_Data_b.m_dLibiao1 = d_data;
				oper_excel.ReadDatafromExcel(i_excel,i,12,str_data);
				//str_vec_name_b.push_back(str_data);
				Dev_Data_b.m_strDevname =str_data;
				oper_excel.ReadDatafromExcel(i_excel,i,14,str_data);
				//str_vec_deviceType_b.push_back(str_data);
				Dev_Data_b.m_strDevType = str_data;
				oper_excel.ReadDatafromExcel(i_excel,i,13,d_data);
				//d_vec_libiao2_b.push_back(d_data);
				Dev_Data_b.m_dLibiao2=d_data;
				i_data=-1;
				oper_excel.ReadDatafromExcel(i_excel,i,16,i_data);
				//i_veczhanType_b.push_back(i_data);	
				Dev_Data_b.m_iupordown =i_data;
				oper_excel.ReadDatafromExcel(i_excel,i,15,i_data);
				if(i_data==0)
				{
					i_data=6;
				}
				//i_vec_line_b.push_back(i_data);
				Dev_Data_b.m_iLine =i_data;
				oper_excel.ReadDatafromExcel(i_excel,i,18,str_data);
				//str_vec_info_b.push_back(str_data);
				Dev_Data_b.m_strRemarks=str_data;
				Device_vecData_b.push_back(Dev_Data_b);
			}	
		}
	}
	//将vector1，vector2按照公里标1来排序
	sort(Device_vecData_a.begin(),Device_vecData_a.end(),less<CDeviceData>()); //升序排列
	sort(Device_vecData_b.begin(),Device_vecData_b.end(),less<CDeviceData>());//升序排列
	for(int k=0;k<Device_vecData_a.size();k++)
	{
		d_vec_libiao_a.push_back(Device_vecData_a[k].m_dLibiao1);
		str_vec_name_a.push_back(Device_vecData_a[k].m_strDevname);
		d_vec_libiao2_a.push_back(Device_vecData_a[k].m_dLibiao2);
		str_vec_deviceType_a.push_back(Device_vecData_a[k].m_strDevType);
		if(Device_vecData_a[k].m_strDevType==_T("ZTD"))
		{
			d_veczhantai_a.push_back(Device_vecData_a[k].m_dLibiao1);
		}
		i_vec_line_a.push_back(Device_vecData_a[k].m_iLine);
		i_veczhanType_a.push_back(Device_vecData_a[k].m_iupordown);
		str_vec_info_a.push_back(Device_vecData_a[k].m_strRemarks);
		acutPrintf(_T("\n%d,libiao_a=%.3f,name_a=%s,type_a=%s,i_line=%d,iupdown=%d"),k,Device_vecData_a[k].m_dLibiao1,Device_vecData_a[k].m_strDevname,Device_vecData_a[k].m_strDevType,Device_vecData_a[k].m_iLine,Device_vecData_a[k].m_iupordown);
	}
	for(int m=0;m<Device_vecData_b.size();m++)
	{
		d_vec_libiao_b.push_back(Device_vecData_b[m].m_dLibiao1);
		str_vec_name_b.push_back(Device_vecData_b[m].m_strDevname);
		d_vec_libiao2_b.push_back(Device_vecData_b[m].m_dLibiao2);
		str_vec_deviceType_b.push_back(Device_vecData_b[m].m_strDevType);
		if(Device_vecData_b[m].m_strDevType==_T("ZTD"))
		{
			d_veczhantai_b.push_back(Device_vecData_b[m].m_dLibiao1);
		}
		i_vec_line_b.push_back(Device_vecData_b[m].m_iLine);
		i_veczhanType_b.push_back(Device_vecData_b[m].m_iupordown);
		str_vec_info_b.push_back(Device_vecData_b[m].m_strRemarks);
		acutPrintf(_T("\n%d,libiao_b=%.3f,name_b=%s,type_b=%s,i_line=%d,iupdown=%d"),m,Device_vecData_b[m].m_dLibiao1,Device_vecData_b[m].m_strDevname,Device_vecData_b[m].m_strDevType,Device_vecData_b[m].m_iLine,Device_vecData_b[m].m_iupordown);
	}
	CDtpmt my_dt;
	int d_max=10;
	int d_min=6;
	acedGetInt(_T("\n设置相邻设备最大间隔(mm)："),&d_max);
	acedGetInt(_T("\n设置相邻设备最小间隔(mm):"),&d_min);
	acutPrintf(_T("\n d_max=%d,d_min=%d,cnt1=%d,cnt2=%d"),d_max,d_min,d_vec_libiao_a.size(),d_vec_libiao_b.size());
	my_dt.ReLay2(d_vec_libiao_a,str_vec_name_a,d_vec_libiao2_a,str_vec_deviceType_a,i_vec_line_a,i_veczhanType_a,str_vec_info_a,d_vec_libiao_b,str_vec_name_b,d_vec_libiao2_b,str_vec_deviceType_b,i_vec_line_b,i_veczhanType_b,str_vec_info_b,d_vec_libiao_he,d_vecx,(double)d_max,(double)d_min);
	//my_dt.ReLay2()
	//读取上下数据
	vector<double> d_vec_po_up;
	vector<CString> str_vec_podu_up;
	vector<CString> str_vec_polength_up;
	vector<int> i_vec_po_up_sx;
	vector<double> d_vec_po_down;
	vector<CString> str_vec_podu_down;
	vector<CString> str_vec_polength_down;
	vector<int> i_vec_po_down_sx;
	int iRowCnt=0;
	int iColCnt=0;
	//读取坡度数据
	oper_excel.GetRowCount(8,iRowCnt);
	oper_excel.GetColumnCount(8,iColCnt);
	acutPrintf(_T("sheet3=%d行，%d列"),iRowCnt,iColCnt);
	for(int i=3;i<iRowCnt;i++)
	{
		double d_data=0;
		oper_excel.ReadDatafromExcel(8,i,3,d_data);
		d_vec_po_up.push_back(d_data);
		oper_excel.ReadDatafromExcel(8,i,11,d_data);
		d_vec_po_down.push_back(d_data);
		CString str_data;
		oper_excel.ReadDatafromExcel(8,i,4,str_data);
		str_vec_polength_up.push_back(str_data);
		oper_excel.ReadDatafromExcel(8,i,12,str_data);
		str_vec_polength_down.push_back(str_data);
		oper_excel.ReadDatafromExcel(8,i,5,str_data);
		str_vec_podu_up.push_back(str_data);
		oper_excel.ReadDatafromExcel(8,i,13,str_data);
		str_vec_podu_down.push_back(str_data);
		int i_data=-1;
		oper_excel.ReadDatafromExcel(8,i,6,i_data);
		i_vec_po_up_sx.push_back(i_data);
		oper_excel.ReadDatafromExcel(8,i,14,i_data);
		i_vec_po_down_sx.push_back(i_data);
	}
	for(int k=0;k<iRowCnt-3;k++)
	{
		//acutPrintf(_T("\n%d\t  %.3f\t %s\t %s\t  %d\t"),k,d_vec_po_up[k],str_vec_polength_up[k],str_vec_podu_up[k],i_vec_po_up_sx[k]);
	}
	my_dt.AddPodu(d_vec_libiao_he,d_vecx,d_vec_po_up,str_vec_polength_up,str_vec_podu_up,i_vec_po_up_sx,true);
	my_dt.AddPodu(d_vec_libiao_he,d_vecx,d_vec_po_down,str_vec_polength_down,str_vec_podu_down,i_vec_po_down_sx,false);
	//读取曲线半径数据
	vector<double> d_qx_start_up;
	vector<double> d_qx_end_up;
	vector<CString> str_qx_ration_up;
	vector<int> i_qx_at_up;
	vector<double> d_qx_start_down;
	vector<double> d_qx_end_down;
	vector<CString> str_qx_ration_down;
	vector<int> i_qx_at_down;
	//读取曲线数据
	oper_excel.GetRowCount(9,iRowCnt);
	oper_excel.GetColumnCount(9,iColCnt);
	acutPrintf(_T("sheet2 =%d行，%d列"),iRowCnt,iColCnt);
	for(int i=3;i<iRowCnt;i++)
	{
		double d_data=0;
		oper_excel.ReadDatafromExcel(9,i,2,d_data);
		d_qx_start_up.push_back(d_data);
		//d_vec_po_up.push_back(d_data);
		oper_excel.ReadDatafromExcel(9,i,3,d_data);
		d_qx_end_up.push_back(d_data);
		CString str_data;
		oper_excel.ReadDatafromExcel(9,i,4,str_data);
		str_qx_ration_up.push_back(str_data);
		int i_data=-1;
		oper_excel.ReadDatafromExcel(9,i,5,i_data);
		i_qx_at_up.push_back(i_data);
		oper_excel.ReadDatafromExcel(9,i,9,d_data);
		d_qx_start_down.push_back(d_data);
		//d_vec_po_up.push_back(d_data);
		oper_excel.ReadDatafromExcel(9,i,10,d_data);
		d_qx_end_down.push_back(d_data);
		//	CString str_data;
		oper_excel.ReadDatafromExcel(9,i,11,str_data);
		str_qx_ration_down.push_back(str_data);
		//		int i_data=-1;
		oper_excel.ReadDatafromExcel(9,i,12,i_data);
		i_qx_at_down.push_back(i_data);
	}
	my_dt.AddQuXian(d_vec_libiao_he,d_vecx,d_qx_start_up,d_qx_end_up,str_qx_ration_up,i_qx_at_up,true);
	my_dt.AddQuXian(d_vec_libiao_he,d_vecx,d_qx_start_down,d_qx_end_down,str_qx_ration_down,i_qx_at_down,false);
	my_dt.AddZhantai(str_vec_deviceType_a,d_vec_libiao_a,d_vec_libiao_he,d_vecx,d_veczhantai_a,i_veczhanType_a);
	//oper_excel.SaveExcel();
	oper_excel.RealseExcel();
}
// 编辑实体（信号机、道岔、轨道电路）数据
void CZnSxt::ModEntData_DT(void)
{
	CAcModuleResourceOverride myResources ;	

	ads_name en ;
	ads_point pt ;
	BOOL bReturn = false ;
	while (bReturn == false) 
	{
		int iReturn = acedEntSel(_T("\n选择对象（信号机、道岔等）"), en, pt) ;
		if(iReturn == RTNORM)
		{			
			AcDbEntity * pEnt = NULL ;
			if(m_mysys.OpenAcDbEntity(en, pEnt, AcDb::kForRead))
			{
				pEnt->highlight() ;				
				acedUpdateDisplay();

				if(pEnt->isKindOf(AcDbBlockReference::desc()))
				{
					AcDbBlockReference * pRef = AcDbBlockReference::cast(pEnt) ;
					CString strBlkName ;
					m_mysys.GetBlockName(pRef, strBlkName) ;
					//this->GetBoxLocation()
					if (strBlkName.Left(6)==_T("DT_XHJ"))
					{
						CEntPropertyDlg dlg ;
						CXhDevice *pDATA = new CXhDevice(strBlkName) ;
						//CXhDevice *pDATA = new CXhDevice(strBlkName) ;
						//数据写入扩展数据
						this->GetEntData(pEnt,*pDATA) ;
                        //扩展数据写入对话框显示及更新
						dlg.m_grid.m_pEntDATA = pDATA ;
						if (dlg.DoModal()==IDOK)
						{
							pRef->upgradeOpen() ;
							this->SetEntData(pEnt, pDATA) ;
							pRef->downgradeOpen() ;

							long nHandleXhj = m_mysys.GetHandle(pEnt) ;
							pEnt->close() ;

							//更新箱盒
							this->DT_DrawBoxXhj(nHandleXhj,pDATA) ;
						}

						bReturn = true ;
					}
					else if (strBlkName.Left(5)==_T("DT_DC"))
					{
						CEntPropertyDlg dlg ;
						CDtDcDATA *pDATA = new CDtDcDATA ;

						this->GetEntData(pEnt, *pDATA) ;
						dlg.m_grid.m_pEntDATA = pDATA ;
						if (dlg.DoModal()==IDOK)
						{
							pRef->upgradeOpen() ;
							this->SetEntData(pEnt, pDATA) ;
							pRef->downgradeOpen() ;							

							long nHandleDc = m_mysys.GetHandle(pEnt) ;		
							pEnt->close() ;

// 							//更新直弯切割绝缘节
// 							this->AddCutJYJ(nHandleDc) ;
							this->DT_DrawZzj(nHandleDc) ;
						}

						bReturn = true ;
					}
					else if (strBlkName.Left(14)==_T("DT_CABLEIMPORT"))
					{
						CEntPropertyDlg dlg ;
						CCableImportDATA *pDATA = new CCableImportDATA(strBlkName) ;
						//数据写入扩展数据
						this->GetEntData(pEnt,*pDATA) ;
						//扩展数据写入对话框显示及更新
						dlg.m_grid.m_pEntDATA = pDATA ;
						if (dlg.DoModal()==IDOK)
						{
							pRef->upgradeOpen() ;
							this->SetEntData(pEnt, pDATA) ;
							//m_mysys.SetXdata(pEnt,_T("STATIONNAME"),pDATA->m_strStation);
							pRef->downgradeOpen() ;
					/*		long nHandleXhj = m_mysys.GetHandle(pEnt) ;*/
							pEnt->close() ;
						}

						bReturn = true ;
					}
					else if(strBlkName.Left(7)==_T("DT_YDQ_")||strBlkName.Left(6)==_T("DT_DHX"))
					{
						CEntPropertyDlg dlg ;
						CXhDevice *pDATA = new CXhDevice(strBlkName) ;
						//数据写入扩展数据
						this->GetEntData(pEnt,*pDATA) ;
						//扩展数据写入对话框显示及更新
						dlg.m_grid.m_pEntDATA = pDATA ;
						if (dlg.DoModal()==IDOK)
						{
							pRef->upgradeOpen() ;
							this->SetEntData(pEnt, pDATA) ;
							//m_mysys.SetXdata(pEnt,_T("STATIONNAME"),pDATA->m_strStation);
							long nHandleJZ=m_mysys.GetHandle(pEnt);
						
							pRef->downgradeOpen() ;
							/*		long nHandleXhj = m_mysys.GetHandle(pEnt) ;*/
							pEnt->close() ;
							//更新箱盒
							this->DT_DrawBoxJZ(nHandleJZ,pDATA);
						}

						bReturn = true ;

					}
					else if(strBlkName.Left(6)==_T("DT_JZ_"))
					{
						CEntPropertyDlg dlg ;
						CXhDevice *pDATA = new CXhDevice(strBlkName) ;
						//数据写入扩展数据
						this->GetEntData(pEnt,*pDATA) ;
						//扩展数据写入对话框显示及更新
						dlg.m_grid.m_pEntDATA = pDATA ;
						if (dlg.DoModal()==IDOK)
						{
							pRef->upgradeOpen() ;
							this->SetEntData(pEnt, pDATA) ;
							//m_mysys.SetXdata(pEnt,_T("STATIONNAME"),pDATA->m_strStation);
							long nHandleXhj = m_mysys.GetHandle(pEnt) ;
							pRef->downgradeOpen() ;
							
// 							long nHandlePmt=0;
// 							m_mysys.GetXdata(nHandleXhj,_T("CloneFrom"),nHandlePmt);
							pEnt->close() ;
							this->DT_DrawBoxJZ(nHandleXhj,pDATA);
						}

						bReturn = true ;

					}
					else if(strBlkName.Left(6)==_T("DT_AP_"))
					{
						CEntPropertyDlg dlg ;
						CXhDevice *pDATA = new CXhDevice(strBlkName) ;
						//数据写入扩展数据
						this->GetEntData(pEnt,*pDATA) ;
						//扩展数据写入对话框显示及更新
						dlg.m_grid.m_pEntDATA = pDATA ;
						if (dlg.DoModal()==IDOK)
						{
							pRef->upgradeOpen() ;
							this->SetEntData(pEnt, pDATA) ;
							//m_mysys.SetXdata(pEnt,_T("STATIONNAME"),pDATA->m_strStation);
							pRef->downgradeOpen() ;
							/*		long nHandleXhj = m_mysys.GetHandle(pEnt) ;*/
							pEnt->close() ;
						}

						bReturn = true ;

					}
					else if(strBlkName.Left(15)==_T("DT_BUTTON_JJGB_"))
					{
						CEntPropertyDlg dlg ;
						CXhDevice *pDATA = new CXhDevice(strBlkName) ;
						//数据写入扩展数据
						this->GetEntData(pEnt,*pDATA) ;
						//扩展数据写入对话框显示及更新
						dlg.m_grid.m_pEntDATA = pDATA ;
						if (dlg.DoModal()==IDOK)
						{
							pRef->upgradeOpen() ;
							this->SetEntData(pEnt, pDATA) ;
							//m_mysys.SetXdata(pEnt,_T("STATIONNAME"),pDATA->m_strStation);
							pRef->downgradeOpen() ;
							/*		long nHandleXhj = m_mysys.GetHandle(pEnt) ;*/
							pEnt->close() ;
						}

						bReturn = true ;

					}
					else if(strBlkName.Left(9)==_T("DT_FCJSQ_"))
					{
						CEntPropertyDlg dlg ;
						CXhDevice *pDATA = new CXhDevice(strBlkName) ;
						//数据写入扩展数据
						this->GetEntData(pEnt,*pDATA) ;
						//扩展数据写入对话框显示及更新
						dlg.m_grid.m_pEntDATA = pDATA ;
						if (dlg.DoModal()==IDOK)
						{
							pRef->upgradeOpen() ;
							this->SetEntData(pEnt, pDATA) ;
							//m_mysys.SetXdata(pEnt,_T("STATIONNAME"),pDATA->m_strStation);
							pRef->downgradeOpen() ;
							/*		long nHandleXhj = m_mysys.GetHandle(pEnt) ;*/
							pEnt->close() ;
						}

						bReturn = true ;

					}
					else if(strBlkName.Left(8)==_T("DT_STOP_"))
					{
						CEntPropertyDlg dlg ;
						CXhDevice *pDATA = new CXhDevice(strBlkName) ;
						//数据写入扩展数据
						this->GetEntData(pEnt,*pDATA) ;
						//扩展数据写入对话框显示及更新
						dlg.m_grid.m_pEntDATA = pDATA ;
						if (dlg.DoModal()==IDOK)
						{
							pRef->upgradeOpen() ;
							this->SetEntData(pEnt, pDATA) ;
							//m_mysys.SetXdata(pEnt,_T("STATIONNAME"),pDATA->m_strStation);
							pRef->downgradeOpen() ;
							/*		long nHandleXhj = m_mysys.GetHandle(pEnt) ;*/
							pEnt->close() ;
						}

						bReturn = true ;

					}
					else if(strBlkName.Left(8)==_T("DT_DOOR_"))
					{
						CEntPropertyDlg dlg ;
						CXhDevice *pDATA = new CXhDevice(strBlkName) ;
						//数据写入扩展数据
						this->GetEntData(pEnt,*pDATA) ;
						//扩展数据写入对话框显示及更新
						dlg.m_grid.m_pEntDATA = pDATA ;
						if (dlg.DoModal()==IDOK)
						{
							pRef->upgradeOpen() ;
							this->SetEntData(pEnt, pDATA) ;
							//m_mysys.SetXdata(pEnt,_T("STATIONNAME"),pDATA->m_strStation);
							pRef->downgradeOpen() ;
							/*		long nHandleXhj = m_mysys.GetHandle(pEnt) ;*/
							pEnt->close() ;
						}

						bReturn = true ;

					}
					else
					{
						AfxMessageBox(_T("选择的不是信号机、道岔等能编辑的块！")) ;
						bReturn = false ;
					}
			}
				else if (pEnt->isKindOf(AcDbPolyline::desc()))
				{
					AcDbPolyline *pPolyLine = AcDbPolyline::cast(pEnt) ;

					int iXdata = 0 ;					
					if (m_mysys.GetXdata(pEnt, _T("ISGUIDAO"), iXdata)&&iXdata==1)
					{
						CEntPropertyDlg dlg ;
						CGdDATA *pDATA = new CGdDATA ;
						this->GetEntData(pEnt, *pDATA) ;

						dlg.m_grid.m_pEntDATA = pDATA ;
						if (dlg.DoModal()==IDOK)
						{
							pEnt->upgradeOpen() ;
							this->SetEntData(pEnt, pDATA) ;
							pEnt->downgradeOpen() ;

							//同步更新相关信息
							int iShowOrHide = 0 ;
							//if (m_mysys.ReadFromRec(_T("PMT_SHOWINFO"), _T("PMT_SHOWINFO_GUIDAONAME"), iShowOrHide) &&iShowOrHide==0)
							CLongArray nArrayShowGdName ;
							if (FindShowGdName(pEnt, nArrayShowGdName)>0)
							{
								this->ShowGdName(pEnt) ; //更新轨道名称（辅助信息）
							}
							if (m_mysys.ReadFromRec(_T("PMT_SHOWINFO"), _T("PMT_SHOWINFO_FSD"), iShowOrHide) &&iShowOrHide==0)
							{
								this->ShowGdFS(pEnt) ; //更新送受电（辅助信息）
							}
							this->UpdateFsBox(pEnt) ; //更新送受电盒子（在双线中）
							this->UpdateDmhBox(pEnt) ; //更新电码化盒子（在双线中）
							this->UpdateSxtGdName(pEnt, pDATA->m_strName) ; //更新在双线中轨道名称
						}

						bReturn = true ;
					}
					else if (m_mysys.GetXdata(pEnt, _T("IS_XHLCABLE"), iXdata)&&iXdata==1)
					{
						CDtMainCableInfo  dlg;
						//CDtMainCableInfo(pEnt) ;
						dlg.m_pEnt = pEnt ;
						dlg.DoModal() ;
						bReturn = TRUE ;
					}
					else if (m_mysys.GetXdata(pEnt, _T("IS_CABLE"), iXdata)&&iXdata==1)
					{
						AcGePoint3d pt_3d_start, pt_3d_end, pt_3d_mid ;
						pPolyLine->getStartPoint(pt_3d_start) ;
						pPolyLine->getEndPoint(pt_3d_end) ;

						//起始端连接的盒子
						int iFindStartBoxNum = 0 ;
						CLongArray nArrayHandleStart ;
						CZnUtility::DT_FindConBox(pt_3d_start, nArrayHandleStart,0.1) ;
						iFindStartBoxNum =nArrayHandleStart.GetCount() ;
						//末端连接的盒子
						int iFindEndBoxNum = 0 ;
						CLongArray nArrayHandleEnd ;
						CZnUtility::DT_FindConBox(pt_3d_end, nArrayHandleEnd,0.1) ;
						iFindEndBoxNum = nArrayHandleEnd.GetCount() ;

						if (iFindStartBoxNum>0&&iFindEndBoxNum>0)
						{
							if (iFindEndBoxNum!=1||iFindStartBoxNum!=1)
							{
								acutPrintf(_T("\n注意：所选电缆两端连接箱盒不唯一,但仍以其中一个进行计算！")) ;								
							}
							long nHandleCable = 0 ;
							nHandleCable = m_mysys.GetHandle(pEnt) ;							
							long iIsXhlCable = 0 ;
							m_mysys.GetXdata(nArrayHandleEnd.GetAt(0), _T("IS_XHLCABLE"), iIsXhlCable) ;
							pEnt->unhighlight() ;
							pEnt->close() ; //此处先关闭是因为computecableinfo函数传递的参数nHandleCable会稍后打开读写实体，造成多次打开错误
							this->DT_ComputeCableInfo(nArrayHandleStart.GetAt(0), nArrayHandleEnd.GetAt(0), nHandleCable, 2) ;
							// Begin 设置箭头颜色
							long nHandleCableImport=0;
							Adesk::UInt16 iColorIndex=256;
							m_mysys.GetXdata(nArrayHandleEnd.GetAt(0),_T("CABLEIMPORT"),nHandleCableImport);	
							if(nHandleCableImport!=0)
							{
								m_mysys.GetEntColorIndex(nArrayHandleEnd.GetAt(0),iColorIndex);
							}
							//设置ARROW颜色
							long nHandleCableString = 0 ; //电缆（箱盒之间的多段线）对应的文本
							this->DT_GetCableString(nHandleCable, nHandleCableString, 0) ;
							if(nHandleCableString!=0)
							{
								m_mysys.SetEntColorIndex(nHandleCableString,iColorIndex);
							}
							// End 设置箭头颜色
							bReturn = true ;
						}
						else
						{
							acutPrintf(_T("\n所选电缆不是两端都连接箱盒或信号楼或主电缆！")) ;
						}
					}
					else
					{
						AfxMessageBox(_T("选择的多段线不是轨道、电缆等可编辑对象！")) ;
						bReturn = false ;
					}
				}
				else
				{
					AfxMessageBox(_T("选择的不是本工具支持的实体！")) ;
					bReturn = false ;
				}
				if(pEnt!=NULL)
				{
					pEnt->unhighlight() ;
				}
				acedUpdateDisplay();
			}
			pEnt->close() ;	
		}
		else if (iReturn == RTERROR) 
		{
			bReturn = false ;
		}	
		else if (iReturn == RTCAN) {
			bReturn = true ;
		}
	}
}
// 编辑实体（信号机、道岔、轨道电路）数据
void CZnSxt::DT_AddBox(void)
{
	CAcModuleResourceOverride myResources ;	

	ads_name en ;
	ads_point pt ;
	BOOL bReturn = false ;
	while (bReturn == false) 
	{
		int iReturn = acedEntSel(_T("\n选择需要添加盒子的设备（信号机、道岔等）"), en, pt) ;
		if(iReturn == RTNORM)
		{			
			AcDbEntity * pEnt = NULL ;
			if(m_mysys.OpenAcDbEntity(en, pEnt, AcDb::kForRead))
			{
				pEnt->highlight() ;				
				acedUpdateDisplay();
				if(pEnt->isKindOf(AcDbBlockReference::desc()))
				{
					AcDbBlockReference * pRef = AcDbBlockReference::cast(pEnt) ;
					CString strBlkName ;
					m_mysys.GetBlockName(pRef, strBlkName) ;
					//获取句柄;
					long nHandleXhj = m_mysys.GetHandle(pEnt) ;
					CDtAddBoxInfo dlg;
					if(dlg.DoModal()==IDOK)
					{
						CString strblkbox=_T("DT_BOX_RECT_0");
						if(dlg.m_iSelShape==0)
						{
							strblkbox=_T("DT_BOX_RECT_0");
						}
						else
						{
							strblkbox=_T("DT_BOX_CIRCLE_0");
						}
						CString strboxname=dlg.m_strboxName;
						//选择一个点，进行插入箱盒
						AcGePoint3d pt_3d_box_insert;
						ads_point pt_box;
						if(acedGetPoint(NULL,_T("\n 输入箱盒的插入点："),pt_box)!=RTNORM)
						{
							pEnt->close();
							return;
						}
							CStringArray strArrayAtt;
							strArrayAtt.Add(strboxname) ;
							AcDbBlockReference* pRef = NULL ;
							pt_3d_box_insert = asPnt3d(pt_box);
							if (m_mysys.InsertBlock(pRef, strblkbox, pt_3d_box_insert, FALSE,0, TRUE))
							{
								CString strTag=_T("TYPE");
								m_mysys.SetAttValue(pRef,strTag,strboxname,FALSE);
								long nHandle_belong_pmt=0;
								m_mysys.GetXdata(nHandleXhj,_T("CloneFrom"),nHandle_belong_pmt);
								if(nHandle_belong_pmt==0)
								{
									m_mysys.GetXdata(nHandleXhj,_T("BELONGTO_PMT"),nHandle_belong_pmt);
								}
								if(nHandle_belong_pmt!=0)
									m_mysys.SetXdata(pRef,_T("BELONGTO_PMT"),nHandle_belong_pmt);
								m_mysys.SetXdata(pRef, _T("BELONG_ENT"), nHandleXhj) ;
								//nHandleBox = m_mysys.GetHandle(pRef) ;
								pRef->close() ;
								pRef = NULL ;
							}

						
					}
					bReturn =true;
					
				}
				//acedUpdateDisplay();
			}
			if(pEnt!=NULL)
			{
				pEnt->unhighlight() ;
			}
			acedUpdateDisplay();
			pEnt->close() ;	
		}
		else if (iReturn == RTERROR) 
		{
			bReturn = false ;
		}	
		else if (iReturn == RTCAN) {
			bReturn = true ;
		}
	}
}
// 从信号楼出来的主电缆，各分向盒出来的电缆连上次电缆等同于连到信号楼
void CZnSxt::DT_ComputeCableInfo(long nHandleCable)
{
	CAcModuleResourceOverride myResources ; 

// 	CLongArray nArrayHandleGd ;
// 	int iNumCrossGd = this->GetCrossGd(nHandleCable, nArrayHandleGd) ;
// 
// 	CXhlCableInfoDlg dlg ;
// 	dlg.m_iCrossGd = iNumCrossGd ;
// 	AcDbEntity* pEnt = NULL ;
// 	if (m_mysys.OpenAcDbEntity(nHandleCable, pEnt, AcDb::kForRead))
// 	{
// 		dlg.m_pEnt = pEnt ;
// 		dlg.DoModal() ;
// 
// 		pEnt->close() ;
// 	}
	CDtMainCableInfo dlg;
	AcDbEntity* pEnt = NULL ;
	if (m_mysys.OpenAcDbEntity(nHandleCable, pEnt, AcDb::kForWrite))
	{
		//将电缆传入dlg中
		dlg.m_pEnt = pEnt ;
		dlg.DoModal() ;
		pEnt->close() ;
	}
}
void CZnSxt::DT_GetZuoTouCount(long nHandleBoxStart,long nHandleBoxEnd,int &CntZT)
{
	try
	{
		long nHandleBelongStart=0;
		long nHandleBelongEnd=0;
		double d_Dist1=0;
		double d_Dist2=0;
		CString strSql;
		m_mysys.GetXdata(nHandleBoxStart,_T("BELONGTO_PMT"),nHandleBelongStart);
		m_mysys.GetXdata(nHandleBoxEnd,_T("BELONGTO_PMT"),nHandleBelongEnd);
		if(nHandleBelongStart!=0&&nHandleBelongEnd!=0)
		{
			//说明是两个箱盒之间的连接
			CntZT=2;
		}
		else 
		{
			CString strblkname1;
			CString strblkname2;
			m_mysys.GetBlockName(nHandleBoxStart,strblkname1);
			m_mysys.GetBlockName(nHandleBoxEnd,strblkname2);
			if(strblkname1.Left(14)==_T("DT_CABLEIMPORT"))
			{
				CntZT=1;
			}
			else if(strblkname2.Left(14)==_T("DT_CABLEIMPORT"))
			{
				CntZT=1;
			}
			else 
			{
				CntZT=2;
			}

		}
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误! in GetZuoTouCount"));
	}

}
void CZnSxt::DT_GetAnZhuangCount(long nHandleBoxStart,long nHandleBoxEnd,int &CntAZ)
{
	try
	{
		long nHandleBelongStart=0;
		long nHandleBelongEnd=0;
		double d_Dist1=0;
		double d_Dist2=0;
		CString strSql;
		m_mysys.GetXdata(nHandleBoxStart,_T("BELONGTO_PMT"),nHandleBelongStart);
		m_mysys.GetXdata(nHandleBoxEnd,_T("BELONGTO_PMT"),nHandleBelongEnd);
		if(nHandleBelongStart!=0&&nHandleBelongEnd!=0)
		{
			//说明是两个箱盒之间的连接
			CntAZ=1;
		}
		else 
		{
			CString strblkname1;
			CString strblkname2;
			m_mysys.GetBlockName(nHandleBoxStart,strblkname1);
			m_mysys.GetBlockName(nHandleBoxEnd,strblkname2);
			if(strblkname1.Left(14)==_T("DT_CABLEIMPORT"))
			{
				CntAZ=2;
			}
			else if(strblkname2.Left(14)==_T("DT_CABLEIMPORT"))
			{
				CntAZ=2;
			}
			else 
			{
				CntAZ=1;
			}

		}
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误! in GetAnZhuangCount"));
	}

}
void CZnSxt::GetRFMBettwenTwoBox(long nHandleBoxStart,long nHandleBoxEnd,int &CntRfm)
{
	try
	{
			long nHandleBelongStart=0;
			long nHandleBelongEnd=0;
			double d_Dist1=0;
			double d_Dist2=0;
			CString strSql;
			m_mysys.GetXdata(nHandleBoxStart,_T("BELONGTO_PMT"),nHandleBelongStart);
			m_mysys.GetXdata(nHandleBoxEnd,_T("BELONGTO_PMT"),nHandleBelongEnd);
			//strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleBelongStart) ;
			//获取平面图中设备的名称和x坐标
			CString str_ent_name1;
			CString str_ent_name2;
			double d_ent_x1;
			double d_ent_x2;
			int i_updown1=-1;
			int i_updown2=-1;
			//如果没有盒子，则表示两个设备之间的连接
			if(nHandleBelongStart==0)
			{
				m_mysys.GetXdata(nHandleBoxStart,_T("CloneFrom"),nHandleBelongStart);
			}
			if(nHandleBelongEnd==0)
			{
				m_mysys.GetXdata(nHandleBoxEnd,_T("CloneFrom"),nHandleBelongEnd);
			}
			this->GetBlockEnt_xAndUpDown(nHandleBelongStart,d_ent_x1,i_updown1);
			this->GetBlockEnt_xAndUpDown(nHandleBelongEnd,d_ent_x2,i_updown2);
			if(d_ent_x1==0||d_ent_x2==0||i_updown1==-1||i_updown2==-1)
			{
				AfxMessageBox(_T("GetBlockEnt_xAndUpDown 调用失败"));
				return ;
			}
			//在数据库中查找坐标位于x1和x2之间的人防门，先要明确设备位于上行还是下行
			if(i_updown2!=i_updown1)
			{
				AfxMessageBox(_T("指定的两个连接设备不是一个运行方向的"));
				return ;
			}
			CADORecordset cSet(&m_AdoDb) ;
			double d_ent_x_libiao;
			int i_type=0;
			if(i_updown1==0)
			{
				i_type=600;
			}
			else 
			{
				i_type=610;

			}
			double x1=(d_ent_x1>d_ent_x2?d_ent_x2:d_ent_x1);
			double x2=(d_ent_x1>d_ent_x2?d_ent_x1:d_ent_x2);
			CntRfm=0;
			//查找和公里标对齐的文字x坐标
			CString str_blk_name=_T("人防门");
			strSql.Format(_T("select * from sxt_ent where sxt_ent_name='%s' and sxt_ent_type=%d order by sxt_ent_x asc"), str_blk_name,i_type);
			if (cSet.Open(strSql))
			{
				while(!cSet.IsEOF())
				{
					long nHandle = 0 ;
					cSet.GetFieldValue(_T("sxt_ent_x"), d_ent_x_libiao) ;
					if((d_ent_x_libiao>x1)&&(d_ent_x_libiao<x2))
					{
						CntRfm++;
					}
					cSet.MoveNext() ;
				}
				cSet.Close() ;
			}
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误!inGetRFM"));
	}

}
void CZnSxt::GetCDLianBettwenTwoBox(long nHandleBoxStart,long nHandleBoxEnd,double &d_cdLength)
{
	try
	{
		long nHandleBelongStart=0;
		long nHandleBelongEnd=0;
		double d_Dist1=0;
		double d_Dist2=0;
		m_mysys.GetXdata(nHandleBoxStart,_T("BELONGTO_PMT"),nHandleBelongStart);
		m_mysys.GetXdata(nHandleBoxEnd,_T("BELONGTO_PMT"),nHandleBelongEnd);
		//获取平面图中设备的名称和x坐标
		double d_ent_x1;
		double d_ent_x2;
		int i_updown1=-1;
		int i_updown2=-1;
		//如果没有盒子，则表示两个设备之间的连接
		if(nHandleBelongStart==0)
		{
			m_mysys.GetXdata(nHandleBoxStart,_T("CloneFrom"),nHandleBelongStart);
		}
		if(nHandleBelongEnd==0)
		{
			m_mysys.GetXdata(nHandleBoxEnd,_T("CloneFrom"),nHandleBelongEnd);
		}
		this->GetBlockEnt_xAndUpDown(nHandleBelongStart,d_ent_x1,i_updown1);
		this->GetBlockEnt_xAndUpDown(nHandleBelongEnd,d_ent_x2,i_updown2);
		if(d_ent_x1==0||d_ent_x2==0||i_updown1==-1||i_updown2==-1)
		{
			AfxMessageBox(_T("GetBlockEnt_xAndUpDown 调用失败"));
			return ;
		}
		//在数据库中查找坐标位于x1和x2之间的人防门，先要明确设备位于上行还是下行
		if(i_updown2!=i_updown1)
		{
			AfxMessageBox(_T("指定的两个连接设备不是一个运行方向的"));
			return ;
		}
		double d_ent_x_libiao;
		int i_type=0;
		if(i_updown1==1)
		{
			i_type=610;
		}
		else 
		{
			i_type=600;

		}
		double x1=(d_ent_x1>d_ent_x2?d_ent_x2:d_ent_x1);
		double x2=(d_ent_x1>d_ent_x2?d_ent_x1:d_ent_x2);
	    // d_cdLength=0;
		//查找和公里标对齐的文字x坐标
		CString str_ent_name1=_T("长链");
		CString str_ent_name2=_T("短链");
		CADORecordset cSet(&m_AdoDb) ;
		CString strSql;
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type=%d and (sxt_ent_name='长链' or sxt_ent_name='短链')  order by sxt_ent_x asc"), i_type);
		//strSql.Format(_T("select * from sxt_ent where sxt_ent_name='%s' and sxt_ent_type=%d "), str_blk_name,i_type);
		if (cSet.Open(strSql))
		{
			while(!cSet.IsEOF())
			{
			   	cSet.GetFieldValue(_T("sxt_ent_x"), d_ent_x_libiao) ;
				if((d_ent_x_libiao>x1)&&(d_ent_x_libiao<x2))
				{
					//获取长短链对应的两个公里标
					cSet.GetFieldValue(_T("sxt_ent_x"), d_ent_x_libiao) ;
					CString strname;
					cSet.GetFieldValue(_T("sxt_ent_name"), strname) ;
					acutPrintf(_T("\n strname=%s"),strname);
					CString strLocation1;
					CString strLocation2;
					double d_loc1=0;
					double d_loc2=0;
					this->GetXLocation(i_updown1,d_ent_x_libiao,strLocation1,strLocation2,4);
					acutPrintf(_T("\n%s %s "),strLocation1,strLocation2);
		            if(strLocation1==_T("")||strLocation2==_T(""))
					{
						AfxMessageBox(_T("长短链坐标获取失败"));
						return;
					}
					CBlkUtility::CStringToDouble(strLocation1,d_loc1);
					CBlkUtility::CStringToDouble(strLocation2,d_loc2);

					if(strname==_T("长链"))
					{
						d_cdLength+=fabs(d_loc1-d_loc2);
					}
					else if(strname==_T("短链"))
					{
						d_cdLength-=fabs(d_loc1-d_loc2);
					}
					
				}
				cSet.MoveNext() ;
			}
			cSet.Close() ;
		}
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误!inGetRFM"));
	}

}
void CZnSxt::GetBlockEnt_xAndUpDown(long nHandleBlk,double &d_ent_x,int &i_up)
{

	try
	{
		    CString strSql;
			d_ent_x=0;
			i_up=-1;
			strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleBlk) ;
			//获取平面图中设备的名称和x坐标
			CString str_ent_name;
			CString str_ent_block;
			double d_ent_x_temp;
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), str_ent_name) ;
			m_RecordSet.GetRecordValue(strSql,_T("sxt_ent_block"),str_ent_block);
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_x"), d_ent_x_temp) ;
			CADORecordset cSet(&m_AdoDb) ;
			double d_ent_x_libiao;
			if(str_ent_name==_T(""))
			{
				if(str_ent_block.Left(14)==_T("DT_CABLEIMPORT"))
				{
					str_ent_name=_T("电缆孔");
				}
				else if(str_ent_name.Left(6)==_T("DT_RFM"))
				{
					str_ent_name=_T("人防门");
				}
				
			}
			double d_ent_y=0;
			//查找和公里标对齐的文字x坐标			
			strSql.Format(_T("select * from sxt_ent where sxt_ent_name='%s' and (sxt_ent_type=610 or sxt_ent_type=600)"), str_ent_name) ;
			if (cSet.Open(strSql))
			{
				while(!cSet.IsEOF())
				{
					cSet.GetFieldValue(_T("sxt_ent_x"), d_ent_x_libiao) ;
					cSet.GetFieldValue(_T("sxt_ent_y"),d_ent_y);
					if(fabs(d_ent_x_libiao-d_ent_x_temp)<2.0)
					{
						d_ent_x=d_ent_x_libiao;
						int i_type=-1;
						if(d_ent_y<100)
						{
							i_type=611;
						}
						else if(d_ent_y<300)
						{
							i_type=601;
						}
					
						//查询公里标栏
						//strSql.Format(_T("select * from sxt_ent where sxt_ent_x=%.1f and sxt_ent_type=%d"),d_ent_x_libiao,i_type) ;
						//m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_type"), i_type) ;
						if(i_type==601)
						{
							i_up=0;
						}
						else if(i_type==611)
						{
							i_up=1;
						}
						else 
						{
							i_up=-1;
						}
						break;
					}
					cSet.MoveNext() ;
				}
				cSet.Close() ;
			}
	
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("GetBlockEnt_xAndUpDown"));
	}
}
void CZnSxt::GetXLocation(int iUp,double sxt_ent_x,CString &str_location1,CString &str_location2,double d_tt)
{
	try
	{
		CString strSql ;
		//获取平面图中设备的名称和x坐标
		CString str_ent_name;
		double d_ent_x=sxt_ent_x;
		CADORecordset cSet(&m_AdoDb) ;
		double d_ent_x_libiao;
		int iType=0;
		int i=0;
		if(iUp==1)
		{
			iType=611;
		}
		else if(iUp==0)
		{
			iType=601;
		}
		else 
		{
			AfxMessageBox(_T("没有指定上下行方向"));
			return;
		}
		//查找和公里标对齐的文字x坐标
		strSql.Format(_T("select * from sxt_ent where sxt_ent_type=%d  order by sxt_ent_x asc"),iType);
		if (cSet.Open(strSql))
		{
			while(!cSet.IsEOF())
			{
				cSet.GetFieldValue(_T("sxt_ent_x"), d_ent_x_libiao) ;
				if(fabs(d_ent_x_libiao-d_ent_x)<d_tt)
				{
					i++;
					if(i==1)
					{
						cSet.GetFieldValue( _T("sxt_ent_name"), str_location1) ;
					}
					else if(i==2)
					{
						cSet.GetFieldValue(_T("sxt_ent_name"), str_location2) ;
						break;
					}
				}
				cSet.MoveNext() ;
			}
			cSet.Close() ;
		}

	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误! in GetXLocation"));
	}
}
void CZnSxt::GetBlkLocation(long nHandleblk,CString &str_location1,CString &str_location2,double d_tt)
{
	try
	{
			CString strSql ;
			strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleblk) ;
			//获取平面图中设备的名称和x坐标
			CString str_ent_name;
			double d_ent_x;
			CStringArray  strArrayLocation;
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), str_ent_name) ;
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_x"), d_ent_x) ;
			CADORecordset cSet(&m_AdoDb) ;
			double d_ent_x_libiao;
			int i=0;
			//查找和公里标对齐的文字x坐标
			strSql.Format(_T("select * from sxt_ent where (sxt_ent_type=601 or sxt_ent_type=611)  order by sxt_ent_x asc"));
			if (cSet.Open(strSql))
			{
				while(!cSet.IsEOF())
				{
					cSet.GetFieldValue(_T("sxt_ent_x"), d_ent_x_libiao) ;
					if(fabs(d_ent_x_libiao-d_ent_x)<d_tt)
					{
						//strSql.Format(_T("select * from sxt_ent where sxt_ent_x=%.1f and (sxt_ent_type=601 or sxt_ent_type=611)"),d_ent_x_libiao) ;
						i++;
						if(i==1)
						{
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), str_location1) ;
						}
						else if(i==2)
						{
							m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), str_location2) ;
							break;
						}
					}
					cSet.MoveNext() ;
				}
				cSet.Close() ;
			}

	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误! in GetBlkLocation"));
	}
}

void CZnSxt::GetBoxLocation(long nHandleBox,CString &str_location,double d_tt)
{
	int iDist = 0 ;
	CString strSql ;
	long nHandleBelong = 0 ; 
	try
	{
		m_mysys.GetXdata(nHandleBox,_T("BELONGTO_PMT"),nHandleBelong);
		if(nHandleBelong==0)
		{
			m_mysys.GetXdata(nHandleBox,_T("CloneFrom"),nHandleBelong);
		}
		if(nHandleBelong==0)
		{
			//目前针对道岔来用的，有待统一处理
			m_mysys.GetXdata(nHandleBox,_T("BELONG_ENT"),nHandleBelong);
		}
		if(nHandleBelong!=0)
		{
			strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleBelong) ;
			//获取平面图中设备的名称和x坐标
			CString str_ent_name;
			CString str_ent_block;
			double d_ent_x;
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), str_ent_name) ;
		//	m_RecordSet.GetRecordValue(strSql,_T("sxt_block_name"),str_ent_block);
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_x"), d_ent_x) ;
			CADORecordset cSet(&m_AdoDb) ;
			double d_ent_x_libiao;
			//查找和公里标对齐的文字x坐标
			strSql.Format(_T("select * from sxt_ent where  (sxt_ent_type=610 or sxt_ent_type=600) order by sxt_ent_x asc")) ;
			if (cSet.Open(strSql))
			{
				while(!cSet.IsEOF())
				{
					long nHandle = 0 ;
					cSet.GetFieldValue(_T("sxt_ent_x"), d_ent_x_libiao) ;
					if(fabs(d_ent_x_libiao-d_ent_x)<d_tt)
					{
						strSql.Format(_T("select * from sxt_ent where sxt_ent_x=%.1f and (sxt_ent_type=601 or sxt_ent_type=611)"),d_ent_x_libiao) ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), str_location) ;
						break;
					}
					cSet.MoveNext() ;
				}
				cSet.Close() ;
			}
		}
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误!inGetBoxLocation"));
	}
}
//获取平面图中设备位置信息
void CZnSxt::GetBoxLocationForPMT(long nHandleBox,CString &str_location,double d_tt)
{
	int iDist = 0 ;
	CString strSql ;
	long nHandleBelong = 0 ; 
	try
	{
		//m_mysys.GetXdata(nHandleBox,_T("BELONGTO_PMT"),nHandleBelong);
// 		if(nHandleBelong==0)
// 		{
// 			m_mysys.GetXdata(nHandleBox,_T("CloneFrom"),nHandleBelong);
// 		}
		nHandleBelong=nHandleBox;
		if(nHandleBelong!=0)
		{
			strSql.Format(_T("select * from sxt_ent where sxt_ent_handle=%d"), nHandleBelong) ;
			//获取平面图中设备的名称和x坐标
			CString str_ent_name;
			CString str_ent_block;
			double d_ent_x;
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), str_ent_name) ;
			//	m_RecordSet.GetRecordValue(strSql,_T("sxt_block_name"),str_ent_block);
			m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_x"), d_ent_x) ;
			// 			if(str_ent_name==_T(""))
			// 			{
			// 				if(str_ent_block.Left(14)==_T("DT_CABLEIMPORT"))
			// 				{
			// 					str_ent_name=_T("电缆孔");
			// 				}
			// 				else if(str_ent_name.Left(6)==_T("DT_RFM"))
			// 				{
			// 					str_ent_name=_T("人防门");
			// 				}
			// 
			// 			}
			CADORecordset cSet(&m_AdoDb) ;
			double d_ent_x_libiao;
			//查找和公里标对齐的文字x坐标
			strSql.Format(_T("select * from sxt_ent where  (sxt_ent_type=610 or sxt_ent_type=600) order by sxt_ent_x asc")) ;
			if (cSet.Open(strSql))
			{
				while(!cSet.IsEOF())
				{
					long nHandle = 0 ;
					cSet.GetFieldValue(_T("sxt_ent_x"), d_ent_x_libiao) ;
					if(fabs(d_ent_x_libiao-d_ent_x)<d_tt)
					{
						strSql.Format(_T("select * from sxt_ent where sxt_ent_x=%.1f and (sxt_ent_type=601 or sxt_ent_type=611)"),d_ent_x_libiao) ;
						m_RecordSet.GetRecordValue(strSql, _T("sxt_ent_name"), str_location) ;
						break;
					}
					cSet.MoveNext() ;
				}
				cSet.Close() ;
			}
		}
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误!inGetBoxLocation"));
	}
}
// 电缆穿过的轨道
int CZnSxt::DT_GetCrossGd(long nHandleCable, CLongArray& nArrayHandleGd)
{
	int iRet = 0 ;

	struct resbuf* pRb ;
	int nLen = 0 ;
	CLongArray nArrayHandle ;
	pRb = acutBuildList(RTDXF0,_T("LWPOLYLINE"), kDxfRegAppName, _T("CloneFrom"), kDxfRegAppName, _T("UPDOWN"), 0) ;
	nLen = m_mysys.GetSSEntHandle(_T("X"), NULL, NULL, pRb, nArrayHandle) ;
	acutRelRb(pRb) ;

	AcDbEntity* pEnt = NULL ;
	if (m_mysys.OpenAcDbEntity(nHandleCable, pEnt, AcDb::kForRead))
	{
		AcDbPolyline* pPlineCable = AcDbPolyline::cast(pEnt) ;

		for(int i=0; i<nLen; i++)
		{
			AcDbEntity* pEnt2 = NULL ;
			if (m_mysys.OpenAcDbEntity(nArrayHandle.GetAt(i), pEnt2, AcDb::kForRead))
			{
				int iUPDOWN = 0 ;
				m_mysys.GetXdata(pEnt2, _T("UPDOWN"), iUPDOWN) ;
				if (iUPDOWN==1)
				{
					AcGePoint3dArray pt3dArray ;
					pPlineCable->intersectWith(pEnt2, AcDb::kOnBothOperands, pt3dArray) ;
					if (pt3dArray.length()>0)
					{
						nArrayHandleGd.Add(nArrayHandle.GetAt(i)) ;
						iRet++;
					}
				}

				pEnt2->close() ;
			}
		}
		pEnt->close() ;
	}

	return iRet;


}
// 计算并绘制电缆信息
void CZnSxt::DT_ComputeCableInfo(long nHandleBoxStart, long nHandleBoxEnd, long nHandleCable,  int iJigType)
{
	long nHandleCableString = 0 ; //电缆（箱盒之间的多段线）对应的文本
	 this->DT_GetCableString(nHandleCable, nHandleCableString, 0) ;
	CString strCableString ;
	if (nHandleCableString!=0)
	{
		//m_mysys.GetTextString(nHandleCableString, strCableString) ;
		m_mysys.GetAttValue(nHandleCableString,_T("TEXT"),strCableString);
	}
	BOOL bIsEndCable=FALSE;
	//判断是否是为尾缆
	bIsEndCable=this->DT_IsFromSameDevice(nHandleBoxStart,nHandleBoxEnd);
	long iIsback=0;
	m_mysys.GetXdata(nHandleCable,_T("ISBACK"),iIsback);
	CDtCableInfoDlg dlg ;
	//原有电缆信息;
	dlg.m_strCableAllOld=strCableString;
	//原有备用信息
	dlg.m_bIsback=iIsback;
    double dDist1=25346.897;
	double dDist2=26342.885;
	CString str_dist1;
	CString str_dist2;
	//首先需要判断是合缆还是串联的
	long nHandleCableImport=0;
	long nHandleTemp=nHandleBoxEnd;
	//连接线末端是父节点
	if(bIsEndCable==FALSE)
	{
		m_mysys.SetXdata(nHandleCable,_T("FATHER"),nHandleBoxEnd);
	}
	else
	{
		m_mysys.SetXdata(nHandleCable,_T("END"),nHandleBoxEnd);
		m_mysys.SetXdata(nHandleCable,_T("BEGIN"),nHandleBoxStart);
	}
	m_mysys.GetXdata(nHandleBoxEnd,_T("CABLEIMPORT"),nHandleCableImport);	
	if(nHandleCableImport!=0)
	{
		nHandleBoxEnd=nHandleCableImport;
		nHandleTemp=nHandleCableImport;
	}
	this->GetBoxLocation(nHandleBoxStart,str_dist1);
	this->GetBoxLocation(nHandleBoxEnd,str_dist2);
	acutPrintf(_T("\n %s %s"),str_dist1,str_dist2);
	CBlkUtility::CStringToDouble(str_dist1,dDist1);
	CBlkUtility::CStringToDouble(str_dist2,dDist2);
	double dz=0;
	//做头个数
	int iCntZT=0;
	this->DT_GetZuoTouCount(nHandleBoxStart,nHandleBoxEnd,iCntZT);
	dz=iCntZT;
	//安装余量个数
	int iCntAZ=0;
	this->DT_GetAnZhuangCount(nHandleBoxStart,nHandleBoxEnd,iCntAZ);
	//暂时设定电缆孔至设备室距离为0；
	double ds=0;
	m_mysys.GetXdata(nHandleTemp,_T("DISTTOROOM"),ds);
	double  dG=0;
	CLongArray nArrayHandleGd ;
	dG=this->DT_GetCrossGd(nHandleCable,nArrayHandleGd);
	double  dC=0;
	if(bIsEndCable==FALSE)
	this->GetCDLianBettwenTwoBox(nHandleBoxStart,nHandleBoxEnd,dC);
	double  dH=0;
	//上下架个数和做头个数一致
	dH=iCntZT;
	double  dR=2;
	int i_CntRfm=0;
	if(bIsEndCable==FALSE)
	this->GetRFMBettwenTwoBox(nHandleBoxStart,nHandleBoxEnd,i_CntRfm);
	dR=i_CntRfm;
	//设备安装余量，通号电缆间余量
	double  dA=0;
	dA=iCntAZ;
	//修正量。默认为0
	double  dX=0;
	dlg.m_dArrayData.Add(dDist1);
	dlg.m_dArrayData.Add(dDist2);
	dlg.m_dArrayData.Add(dz);
	dlg.m_dArrayData.Add(ds);
	dlg.m_dArrayData.Add(dG);
	dlg.m_dArrayData.Add(dC);
	dlg.m_dArrayData.Add(dH);
    dlg.m_dArrayData.Add(dR);
	dlg.m_dArrayData.Add(dA);
	dlg.m_dArrayData.Add(dX);
	//dlg.m_edit_CableType.SetWindowText(_T("4"));
	//dlg.m_edit_backxinshu.SetWindowText(_T("2"));
// 	sequential_tree<CBoxNodeEnt> cableBoxTree ;
// 	this->DT_BuildCableBoxTree(nHandleBoxStart, nHandleCable, cableBoxTree) ;
// 	sequential_tree<CBoxNodeEnt>::pre_order_iterator pit = cableBoxTree.pre_order_begin();	//先遍历子节点	
// 	sequential_tree<CBoxNodeEnt>::pre_order_iterator pit_end = cableBoxTree.pre_order_end();	
// 	acutPrintf(_T("\n 先序遍历树："));
// 	for (; pit != pit_end; ++pit)
// 	{
// 		CBoxNodeEnt* pBoxNodeTmp = pit.node()->get() ;	
// 		int iNum1=0;
// 		int iNum2=0;
// 		this->DT_FindCableCoreNumFromCableString(pBoxNodeTmp->m_strCableString,iNum1,iNum2);
// 		acutPrintf(_T("\n nHandlelow=%d,boxtype=%s,strcablestring=%s,CoreNum=%d,CoreBack=%d"),pBoxNodeTmp->m_nHandleBox,pBoxNodeTmp->m_strBoxType,pBoxNodeTmp->m_strCableString,iNum1,iNum2);
// 		//acutPrintf(_T("\n nHandlelow=%d,boxtype=%s,strcablestring=%s"),pBoxNodeTmp->m_nHandleBox,pBoxNodeTmp->m_strBoxType,pBoxNodeTmp->m_strCableString);
// 	}
	CLongArray nArrayFind1;
	CLongArray nArrayFind2;
	this->DT_FindCable(nHandleBoxStart,nHandleCable,nArrayFind1,1);
	this->DT_FindCable(nHandleBoxStart,nHandleCable,nArrayFind2,3);
	//计算有效电缆使用芯数
	int iUseCore=0;
	for(int i=0;i<nArrayFind1.GetCount();i++)
	{
		long nHandleCableLine=nArrayFind1.GetAt(i);
		CString strCableString;
		m_mysys.GetXdata(nHandleCableLine,_T("CABLESTRING"),strCableString);
		acutPrintf(_T("\n 1 Cabletring=%s"),strCableString);
		int iNum=0;
		int iBack=0;
		this->DT_FindCableCoreNumFromCableString(strCableString,iNum,iBack);
		iUseCore+=(iNum-iBack);
	}
	
	for(int i=0;i<nArrayFind2.GetCount();i++)
	{
		long nHandleCableLine=nArrayFind2.GetAt(i);
		CString strCableString;
		m_mysys.GetXdata(nHandleCableLine,_T("CABLESTRING"),strCableString);
		acutPrintf(_T("\n 2 Cabletring=%s"),strCableString);
		int iNum=0;
		int iBack=0;
		this->DT_FindCableCoreNumFromCableString(strCableString,iNum,iBack);
		iUseCore+=(iNum-iBack);
	}
	acutPrintf(_T("\n usecore=%d"),iUseCore);
	int iBackupSum=0;
	CString strCableStringnew;
	this->DT_GetUseCableType(iUseCore,iBackupSum,strCableStringnew,0,0);
	dlg.m_strCableType=strCableStringnew;
	CString strBackup;
	//strMsg.Format(_T("读取第%d根主电缆槽相连的电缆..."), i+1) ;
	strBackup.Format(_T("%d"), iBackupSum);
	dlg.m_strCableBackxinshu=strBackup ;
	if(dlg.DoModal()==IDOK)
	{
		CString strText;
		dlg.m_editCableString.GetWindowText(strText);
		dlg.m_edit_distToCable.SetDouble(ds);
		acutPrintf(_T("\nstrText=%s"),strText);
		m_mysys.SetXdata(nHandleCable,_T("CABLESTRING"),dlg.m_strCableAll);
		//添加是否备用信息
		if(bIsEndCable==FALSE)
		m_mysys.SetXdata(nHandleCable,_T("ISBACK"),dlg.m_bIsback);
		this->DT_SetCableString(nHandleCable, nHandleCableString, dlg.m_strCableAll, 0) ;
		if(bIsEndCable==TRUE)
		{
			m_mysys.SetXdata(nHandleCable,_T("IS_ENDCABLE"),1);
			//和设备绑定到一起，删除的时候可以一起删除
			m_mysys.SetXdata(nHandleCable,_T("BELONG_ENT"),nHandleBoxStart);
		}
		else
		{
			CString strAppNames[10] = {_T("DIST1"), _T("DIST2"), _T("ZUOTOU"),  _T("DISTTOCABLE"), _T("GUOGUINUM"), _T("CDLIAN"), _T("SXJNUM"), _T("RFMNUM"),_T("ANZHUNAG"),_T("XIUZHENG")} ;
			for (int i=0; i<10; i++)
			{
				double dData = dlg.m_dArrayData.GetAt(i) ;
				m_mysys.SetXdata(nHandleCable, strAppNames[i], dData) ;
			}
		}
		AcGePoint3d pt_3d_insert(0,0,0);
	}
}
// 通过电缆多线段的handle取得其连接字符串,iType=2时是电缆name,=3时是电缆编号
int CZnSxt::DT_GetCableString(long nHandleCableLine, long& nHandleCableString, int iType)
{
	int iRet = 0 ;

	struct resbuf* pRb ;
	int nLen = 0 ;
	CLongArray nArrayHandle ;
	pRb = acutBuildList(RTDXF0,_T("INSERT"), kDxfRegAppName, _T("BELONG_ENT"), 2, _T("DT_ARROW_000"), 0) ;
	struct resbuf* pRb2 ;
	int nLen2=0;
	CLongArray nArrayHandle2;
	pRb2 =acutBuildList(RTDXF0,_T("INSERT"), kDxfRegAppName, _T("BELONG_ENT"), 2, _T("DT_ARROW_100"), 0) ;
	nLen = m_mysys.GetSSEntHandle(_T("X"), NULL, NULL, pRb, nArrayHandle) ;
	nLen2=m_mysys.GetSSEntHandle(_T("X"), NULL, NULL, pRb2, nArrayHandle2) ;
	acutRelRb(pRb) ;
	acutRelRb(pRb2) ;

	for (int i=0; i<nLen; i++)
	{
		long nHandleTmp = 0 ;
		if (m_mysys.GetXdata(nArrayHandle.GetAt(i), _T("BELONG_ENT"), nHandleTmp ))
		{
			if (nHandleTmp == nHandleCableLine)
			{
				nHandleCableString = nArrayHandle.GetAt(i) ;
				iRet++ ;
			}
			}
	}
	for (int i=0; i<nLen; i++)
	{
		long nHandleTmp = 0 ;
		if (m_mysys.GetXdata(nArrayHandle2.GetAt(i), _T("BELONG_ENT"), nHandleTmp ))
		{
			if (nHandleTmp == nHandleCableLine)
			{
				nHandleCableString = nArrayHandle2.GetAt(i) ;
				iRet++ ;
			}
		}
	}

	return iRet ;
}
// 在相应的线段上绘制电缆信息字符串（从SetCableString分离出来，为复用）
int CZnSxt::DT_DrawCableString(long nHandleCableLine, const CString& strCableString, int iStringType)
{
	AcDbEntity* pEnt = NULL ;
	if(m_mysys.OpenAcDbEntity(nHandleCableLine, pEnt, AcDb::kForRead))
	{
		AcDbPolyline* pPolyLine = AcDbPolyline::cast(pEnt) ;
		AcGeLineSeg3d lineSeg ;
		int iIndexVertMax = -1 ;
		double dLenMax = 0 ;
		for (int i=0; i<pPolyLine->numVerts()-1; i++)
		{
			
			pPolyLine->getLineSegAt(i, lineSeg) ;
			double dLen = lineSeg.length() ;
			AcGeVector3d vec = lineSeg.direction() ;
			//if(fabs(vec.angleOnPlane(AcGePlan)))
			double i_a=fabs(vec.angleOnPlane(AcGePlane::kXYPlane));
			 if(fabs(vec.angleOnPlane(AcGePlane::kXYPlane))<0.2||fabs(i_a/2-PI)<0.2||fabs(i_a-PI)<0.2)
			 {
				 continue;
			 }
			if (dLen>dLenMax+2)
			{
				dLenMax = dLen ;
				iIndexVertMax = i ;
			}
		}

		double dHeight = 2, dWidthFactor = 0.7, dDistToLine = 0 ;
		AcDb::TextHorzMode txthorzmod = AcDb::kTextMid ;
		AcDb::TextVertMode txtvertmod = AcDb::kTextBase ;
   
		pPolyLine->getLineSegAt(iIndexVertMax, lineSeg) ;
		AcGeVector3d vec = lineSeg.direction() ;
		AcGePoint3d pt_3d_mid = lineSeg.midPoint() ;
		AcGePoint3d pt_3d_start=lineSeg.startPoint();
		AcGePoint3d pt_3d_end=lineSeg.endPoint();
		AcGePoint3d pt_3d_midtmp = pt_3d_mid ;
		if(dLenMax!=0)
		{
			//有垂直线时
			if(pt_3d_end.y>pt_3d_start.y)
			{
				pt_3d_midtmp=pt_3d_end;
				pt_3d_midtmp.y-=3;
			}
			else
			{
				pt_3d_midtmp=pt_3d_end;
				pt_3d_midtmp.y+=3;
			}
		}
		else
		{
			//无垂直线
			pt_3d_midtmp=pt_3d_mid;
		}
		
		//AcGePoint3d pt_3d_midtmp = pt_3d_mid ;
		double dAngleTextToLine = vec.angleOnPlane(AcGePlane::kXYPlane)+PI/2 ;
		AcGePoint3d pt_3d_text ;
		m_mysys.GetPoint(pt_3d_midtmp, dDistToLine, dAngleTextToLine,pt_3d_text) ;
		pEnt->close();
		if(iStringType==0)
		{
			if(iIndexVertMax>=0)
			{
				this->DT_InsertArrow(pt_3d_mid,0,true,nHandleCableLine);
			}
			else
			{
				this->DT_InsertArrow(pt_3d_mid,1,true,nHandleCableLine);
			}

		}
		else
		{
			if(iIndexVertMax>=0)
			{
				this->DT_InsertArrow(pt_3d_mid,0,false,nHandleCableLine);
			}
			else
			{
				this->DT_InsertArrow(pt_3d_mid,1,false,nHandleCableLine);
			}

		}
		

	}

	return 0;
}

// 设置电缆字符串，若nHandleCableString=0，则需要创建一个文本对象
int CZnSxt::DT_SetCableString(long nHandleCableLine, long nHandleCableString, CString& strCableString, int iStringType)
{
	if (nHandleCableString<1)
	{
		AcDbEntity* pEnt = NULL ;
		if(m_mysys.OpenAcDbEntity(nHandleCableLine, pEnt, AcDb::kForRead))
		{
			this->DT_DrawCableString(nHandleCableLine, strCableString, iStringType) ;
			pEnt->close() ;
		}
	}
	else
	{
		  AcDbEntity * pEnt = NULL ;
		if(m_mysys.OpenAcDbEntity(nHandleCableString, pEnt, AcDb::kForRead))
		{
			if (pEnt->isKindOf(AcDbBlockReference::desc()))
			{
				AcDbBlockReference* pRef = AcDbBlockReference::cast(pEnt) ;
				//this->SetAttValue(pRef, vec) ;
				m_mysys.SetAttValue(pRef,_T("TEXT"),strCableString);
				long iBack=0;
				m_mysys.GetXdata(nHandleCableLine,_T("ISBACK"),iBack);
				if(iBack!=0)
				{
					CString strText=strCableString;
					int iFindIndex1=strText.FindOneOf(" ");
					int iFindIndex2=strText.FindOneOf("(");
					int iFindIndex3=strText.FindOneOf("-");
					CString strback=strText.Mid(iFindIndex1+1,iFindIndex2-iFindIndex1-1);
					CString strback2=strText.Mid(iFindIndex3+1,iFindIndex2-iFindIndex3-1);
					CString strText2=_T("备用:")+strback+_T("(")+strback2+_T(")");
					m_mysys.SetAttValue(pRef,_T("TEXT2"),strText2,FALSE);
				}
				else
				{
					m_mysys.SetAttValue(pRef,_T("TEXT2"),_T(""),TRUE);
				}
				pRef->close();
			}
			//AcDbText* pText = AcDbText::cast(pEnt) ;
			//pText->setTextString(strCableString) ;	
			//m_mysys.SetAttValue(nHandleCableString,_T("TEXT"),strCableString);
			pEnt->close() ;
		}
	}
	return 0;
}

// 设置电缆名称,只有楼端有
int CZnSxt::DT_SetCableName(long nHandleCableLine, long nHandleCableName, CString &strCableName)
{
	if (nHandleCableName<1)
	{
		AcDbEntity* pEnt = NULL ;
		if(m_mysys.OpenAcDbEntity(nHandleCableLine, pEnt, AcDb::kForRead))
		{
			AcDbPolyline* pPolyLine = AcDbPolyline::cast(pEnt) ;
			int iNumVert = pPolyLine->numVerts() ;
			AcGeLineSeg3d lineSeg ;
			pPolyLine->getLineSegAt(iNumVert-2, lineSeg) ;
			AcGeVector3d vec = lineSeg.direction() ;
			AcGePoint3d pt_3d_end = lineSeg.endPoint() ;
			AcGePoint3d pt_3d_text ;
			m_mysys.GetPoint(pt_3d_end, 1.5, vec.angleOnPlane(AcGePlane::kXYPlane), pt_3d_text) ;
			AcDbObjectId objId ;
			if (strCableName.IsEmpty())
			{
				strCableName = _T("XX-XX#") ;
			}
			m_mysys.AddText(objId, strCableName, pt_3d_text, pt_3d_text, AcDb::kTextMid, AcDb::kTextBase, 2, 0.7, vec.angleOnPlane(AcGePlane::kXYPlane)-PI/2, 256) ;
			this->SetLayer(objId, m_iCurDljlType) ;
			m_mysys.SetXdata(objId, _T("IS_CABLENAME"), 1) ;
			m_mysys.SetXdata(objId, _T("BELONG_ENT"), nHandleCableLine) ;
			pEnt->close() ;
		}
	}
	else
	{
		AcDbEntity* pEnt = NULL ;
		if(m_mysys.OpenAcDbEntity(nHandleCableName, pEnt, AcDb::kForWrite))
		{
			AcDbText* pText = AcDbText::cast(pEnt) ;
			pText->setTextString(strCableName) ;
			pEnt->close() ;
		}
	}
	return 0;
}
// 查找盒子或者设备的其他连接电缆
int CZnSxt::DT_FindCable(long nHandleBox, long nHandlePrePline, CLongArray& nArrayHandleFind)
{
	int iRet = 0 ;

	AcDbEntity* pEntBox = NULL ;
	if (m_mysys.OpenAcDbEntity(nHandleBox, pEntBox, AcDb::kForRead))
	{
		if (pEntBox->isKindOf(AcDbBlockReference::desc()))
		{
			AcDbBlockReference* pRef = AcDbBlockReference::cast(pEntBox) ;
			CString strBlkName ;
			m_mysys.GetBlockName(pRef, strBlkName) ;

			AcGePoint3d pt_3d_blk ;
			pt_3d_blk = pRef->position() ;

			AcGePoint3d pt_3d_minmax[2]  ;
			double dBoxWidth = 5 ; //默认是送受电盒子的长度
			if(strBlkName.Left(6)==_T("DT_BOX"))
			{
				dBoxWidth = 5 ;
				for (int i=0; i<2; i++)
				{
					pt_3d_minmax[i].x = pt_3d_blk.x+(i==0?-1:1)*dBoxWidth/2 ;
					pt_3d_minmax[i].y = pt_3d_blk.y+(i==0?-1:1)*1.5 ;
				}
				iRet = m_mysys.GetConnectLine(pt_3d_minmax[0], pt_3d_minmax[1], nArrayHandleFind, 0.05) ;

			}
			if(strBlkName.Left(14)==_T("DT_CABLEIMPORT"))
			{
				dBoxWidth = 64 ;
				for (int i=0; i<2; i++)
				{
					pt_3d_minmax[i].x = pt_3d_blk.x+(i==0?-1:1)*dBoxWidth/2 ;
					pt_3d_minmax[i].y = pt_3d_blk.y+(i==0?-1:1)*4 ;
				}
				iRet = m_mysys.GetConnectLine(pt_3d_minmax[0], pt_3d_minmax[1], nArrayHandleFind, 0.05) ;

			}
			else if (strBlkName.Left(6)==_T("DT_AP_"))
			{
				dBoxWidth = 3 ;
				for (int i=0; i<2; i++)
				{
					pt_3d_minmax[i].x = pt_3d_blk.x+(i==0?-1:1)*dBoxWidth/2 ;
					pt_3d_minmax[i].y = pt_3d_blk.y+(i==0?-1:1)*1.5 ;
				}
				iRet = m_mysys.GetConnectLine(pt_3d_minmax[0], pt_3d_minmax[1], nArrayHandleFind, 0.05) ;
			}
			else if (strBlkName.Left(6)==_T("DT_YDQ"))
			{						
				dBoxWidth = 3 ;
				iRet = m_mysys.GetConnectLine(pt_3d_blk, 3, nArrayHandleFind, 0.05) ;
			}
			else if (strBlkName.Left(6)==_T("DT_JZ_"))
			{						
				dBoxWidth = 3 ;
				iRet = m_mysys.GetConnectLine(pt_3d_blk, 1.5, nArrayHandleFind, 0.05) ;
			}

			for (int i=0; i<iRet; i++)
			{						
				long nHandleFind = nArrayHandleFind.GetAt(i) ;
				if (nHandleFind==nHandlePrePline)
				{
					nArrayHandleFind.RemoveAt(i) ;
					iRet-- ;
					break;
				}
			}

// 			//20151119:迟汶，排序连接的电缆，以便编号规律
// 			if (strBlkName.Left(6)==_T("BOX_HF"))
// 			{						
// 				//对所有连线排序，主干电缆（两头都是HF的）靠前，逆时针和盒子相交的顺序
// 				//先逆时针排序所有线，取其和HF相交段的向量方向来判断
// 				double  dAnglePreLine = 0.000 ;
// 				dAnglePreLine = this->GetPlineAngle(nHandlePrePline, 0, 0) ;
// 				vector<ConLineInfo> vec ;
// 				for (int i=0; i<nArrayHandleFind.GetCount(); i++)
// 				{
// 					long nHandle = nArrayHandleFind.GetAt(i) ;
// 					ConLineInfo info ;
// 					info.nHandle = nHandle ;
// 					info.dAngle = this->GetPlineAngle(nHandle, dAnglePreLine, 1) ; //相对preline的逆时针角度（不能用angleTo函数）
// 					vec.push_back(info) ;
// 				}
// 				//sort(vec.begin(),vec.end(),greater());//降序排列
// 				sort(vec.begin(),vec.end(),less<ConLineInfo>()); //升序排列
// 				CLongArray nArrayHandleBranchCable ;
// 				nArrayHandleFind.RemoveAll() ;
// 				for (int i=0; i<vec.size(); i++)
// 				{
// 					long nHandle = vec.at(i).nHandle ;
// 					//是否是主干电缆,主干排靠前
// 					AcGePoint3d pt_3d_start ;
// 					this->GetPlinePoint(nHandle, pt_3d_start, 0) ;
// 					CLongArray nArrayHandleFindBox_Start ;
// 					int iRetFind_Start =CZnUtility::FindConBox(pt_3d_start, nArrayHandleFindBox_Start) ;
// 					if (iRetFind_Start==5) //分向盒 HF
// 					{
// 						nArrayHandleFind.Add(nHandle) ;
// 					}
// 					else
// 					{
// 						nArrayHandleBranchCable.Add(nHandle) ;
// 					}
// 				}
// 				nArrayHandleFind.Append(nArrayHandleBranchCable) ;
// 			}
		}
		pEntBox->close() ;
	}

	return iRet ;
}
// 查找盒子或者设备的其他连接电缆,iCable=1:串联电缆；iCable=2:单拉电缆；iCable=3：尾缆;iCable=4：主缆
int CZnSxt::DT_FindCable(long nHandleBox, long nHandlePrePline, CLongArray& nArrayHandleFind,int iCableType)
{
	int iRet = 0 ;
    CLongArray nArrayHandleFindTemp;
	AcDbEntity* pEntBox = NULL ;
	if (m_mysys.OpenAcDbEntity(nHandleBox, pEntBox, AcDb::kForRead))
	{
		if (pEntBox->isKindOf(AcDbBlockReference::desc()))
		{
			AcDbBlockReference* pRef = AcDbBlockReference::cast(pEntBox) ;
			CString strBlkName ;
			m_mysys.GetBlockName(pRef, strBlkName) ;

			AcGePoint3d pt_3d_blk ;
			pt_3d_blk = pRef->position() ;

			AcGePoint3d pt_3d_minmax[2]  ;
			double dBoxWidth = 5 ; //默认是送受电盒子的长度
			if(strBlkName.Left(6)==_T("DT_BOX"))
			{
				dBoxWidth = 5 ;
				for (int i=0; i<2; i++)
				{
					pt_3d_minmax[i].x = pt_3d_blk.x+(i==0?-1:1)*dBoxWidth/2 ;
					pt_3d_minmax[i].y = pt_3d_blk.y+(i==0?-1:1)*1.5 ;
				}
				iRet = m_mysys.GetConnectLine(pt_3d_minmax[0], pt_3d_minmax[1], nArrayHandleFindTemp, 0.05) ;

			}
			if(strBlkName.Left(14)==_T("DT_CABLEIMPORT"))
			{
				dBoxWidth = 64 ;
				for (int i=0; i<2; i++)
				{
					pt_3d_minmax[i].x = pt_3d_blk.x+(i==0?-1:1)*dBoxWidth/2 ;
					pt_3d_minmax[i].y = pt_3d_blk.y+(i==0?-1:1)*4 ;
				}
				iRet = m_mysys.GetConnectLine(pt_3d_minmax[0], pt_3d_minmax[1], nArrayHandleFindTemp, 0.05) ;

			}
			else if (strBlkName.Left(6)==_T("DT_AP_"))
			{
				dBoxWidth = 3 ;
				for (int i=0; i<2; i++)
				{
					pt_3d_minmax[i].x = pt_3d_blk.x+(i==0?-1:1)*dBoxWidth/2 ;
					pt_3d_minmax[i].y = pt_3d_blk.y+(i==0?-1:1)*1.5 ;
				}
				iRet = m_mysys.GetConnectLine(pt_3d_minmax[0], pt_3d_minmax[1], nArrayHandleFindTemp, 0.05) ;
			}
			else if (strBlkName.Left(6)==_T("DT_YDQ"))
			{						
				dBoxWidth = 3 ;
				iRet = m_mysys.GetConnectLine(pt_3d_blk, 3, nArrayHandleFindTemp, 0.05) ;
			}
			else if (strBlkName.Left(6)==_T("DT_JZ_"))
			{						
				dBoxWidth = 3 ;
				iRet = m_mysys.GetConnectLine(pt_3d_blk, 1.5, nArrayHandleFindTemp, 0.05) ;
			}

			for (int i=0; i<iRet; i++)
			{						
				long nHandleFind = nArrayHandleFindTemp.GetAt(i) ;
				if (nHandleFind==nHandlePrePline)
				{
					nArrayHandleFindTemp.RemoveAt(i) ;
					iRet-- ;
					break;
				}
			}
			for(int i=0;i<nArrayHandleFindTemp.GetCount();i++)
			{
				long nHandleLine=nArrayHandleFindTemp.GetAt(i);
				long iType=0;
				m_mysys.GetXdata(nHandleLine,_T("IS_CABLE"),iType);
				if(iType==1)
				{
					if(iCableType==1)
					{
						long iTypeTemp=0;
						m_mysys.GetXdata(nHandleLine,_T("IS_CLCABLE"),iTypeTemp);
						if(iTypeTemp==1)
							nArrayHandleFind.Add(nHandleLine);
					}
					else if(iCableType==2)
					{
						long iTypeTemp=0;
						m_mysys.GetXdata(nHandleLine,_T("IS_DLCABLE"),iTypeTemp);
						if(iTypeTemp==1)
							nArrayHandleFind.Add(nHandleLine);

					}
					else if(iCableType==3)
					{
						long iTypeTemp=0;
						m_mysys.GetXdata(nHandleLine,_T("IS_ENDCABLE"),iTypeTemp);
						if(iTypeTemp==1)
							nArrayHandleFind.Add(nHandleLine);

					}
					else if(iCableType==4)
					{
						long iTypeTemp=0;
						m_mysys.GetXdata(nHandleLine,_T("IS_XHLCABLE"),iTypeTemp);
						if(iTypeTemp==1)
							nArrayHandleFind.Add(nHandleLine);
					}
					else
					{
						nArrayHandleFind.Add(nHandleLine);
					}
				}
			}
		}
		pEntBox->close() ;
	}

	return iRet ;
}
// 读取径路图到数据库
void CZnSxt::DT_ReadJltToDb(void)
{
	CString strSql ;

	m_AdoDb.ClearTable(_T("cablenet_ent")) ;

	CLongArray nArrayHandleCable[3] ;
	this->DT_GetCable(nArrayHandleCable) ;

	int nSumCable = 0 ;
	nSumCable = nArrayHandleCable[2].GetCount() ;
	acutPrintf(_T("\n电缆:%d根"), nSumCable) ;

	int nSumXhlCable = 0 ;
	nSumXhlCable = nArrayHandleCable[1].GetCount() ;
	acutPrintf(_T("\n串联至信号楼电缆:%d根"), nSumXhlCable) ;

	int nSumXhlCable2 = 0 ;
	nSumXhlCable2 = nArrayHandleCable[0].GetCount() ;
	acutPrintf(_T("\n信号楼主电缆:%d根"), nSumXhlCable2) ;
	//读入串联电缆或者合缆电缆
	for (int i=0; i<nSumXhlCable; i++)
	{
		AcDbEntity* pEntXhlCable = NULL ;
		//取串联至信号楼电缆
		long nHandleXhlCable = nArrayHandleCable[1].GetAt(i) ;
		if (m_mysys.OpenAcDbEntity(nHandleXhlCable, pEntXhlCable, AcDb::kForRead))
		{
			AcDbPolyline* pPolyLineXhlCable = AcDbPolyline::cast(pEntXhlCable) ;
			AcGePoint3d pt_3d_start_xhlcable, pt_3d_end_xhlcable ;
			pPolyLineXhlCable->getStartPoint(pt_3d_start_xhlcable) ;
			pPolyLineXhlCable->getEndPoint(pt_3d_end_xhlcable) ;
			//acutPrintf(_T("\n主缆线：%d, end_point(%.3f, %.3f)"),nHandleXhlCable, pt_3d_end_xhlcable.x, pt_3d_end_xhlcable.y) ;
			int iIsXhlLeftOrRight = (pt_3d_end_xhlcable.x>pt_3d_start_xhlcable.x?0:1) ; //0:左侧,1:右侧
			//找到所属电缆孔
			//找到终点所连接的盒子
			long nHandleCableImport=0;
			CLongArray nArrayHandleFindBox_Start;
			CLongArray nArrayHandleFindBox_End;
			int iRetFind_Start =CZnUtility::DT_FindConBox(pt_3d_start_xhlcable, nArrayHandleFindBox_Start) ;
			int iRetFind_End =CZnUtility::DT_FindConBox(pt_3d_end_xhlcable, nArrayHandleFindBox_End) ;
			if(iRetFind_End==12)
			{
				nHandleCableImport=nArrayHandleFindBox_End.GetAt(0);
			}
			//该电缆和主电缆连接
			//acutPrintf(_T("\n连接到主缆的线：%d, start_point(%.3f, %.3f), end_point(%.3f, %.3f)"),nHandleCable, pt_3d_start_cable.x, pt_3d_start_cable.y, pt_3d_end_cable.x, pt_3d_end_cable.y) ;
			CString strCableName ;
			long nHandleCableString = 0 ; //电缆（箱盒之间的多段线）对应的文本
			this->DT_GetCableString(nHandleXhlCable, nHandleCableString, 0) ;
			if (nHandleCableString!=0)
			{
				m_mysys.GetAttValue(nHandleCableString,_T("TEXT"),strCableName);
			}
			m_strCurNameIntoXhl = strCableName ;
			if (strCableName.GetLength()>1)
			{
				int iTypeJlt = CZnUtility::GetJltType(strCableName.Left(2)) ;
				if (iTypeJlt!=-1)
				{
					m_iCurDljlType = iTypeJlt ;
				}
			}
			CString strBranchIndex = _T("1") ;
			strSql.Format(_T("insert into cablenet_ent(handle_low, devname, dev_for, boxtype, box_block, left_or_right, belong_xhl_handle_low, branch_index) \
								values(%d,'%s','%s','%s', '%s', %d, %d, '%s')"),\
								nHandleXhlCable, strCableName, _T("JY1"), _T("XHL"), _T("DZ_XHL_0"), iIsXhlLeftOrRight,  nHandleCableImport, strBranchIndex) ;
			m_AdoDb.Execute(strSql) ;
			strBranchIndex.AppendFormat(_T("-1")) ;
			this->DT_FindBox(nHandleXhlCable, nHandleXhlCable, nHandleXhlCable, strBranchIndex) ;
		}
			pEntXhlCable->close() ;
		}	
	//读入单拉电缆
	for (int i=0; i<nSumXhlCable2; i++)
	{
		AcDbEntity* pEntXhlCable = NULL ;
		//取和信号主电缆
		long nHandleXhlCable2 = nArrayHandleCable[0].GetAt(i) ;
		if (m_mysys.OpenAcDbEntity(nHandleXhlCable2, pEntXhlCable, AcDb::kForRead))
		{
			AcDbPolyline* pPolyLineXhlCable = AcDbPolyline::cast(pEntXhlCable) ;
			AcGePoint3d pt_3d_start_xhlcable, pt_3d_end_xhlcable ;
			pPolyLineXhlCable->getStartPoint(pt_3d_start_xhlcable) ;
			pPolyLineXhlCable->getEndPoint(pt_3d_end_xhlcable) ;
			//acutPrintf(_T("\n主缆线：%d, end_point(%.3f, %.3f)"),nHandleXhlCable, pt_3d_end_xhlcable.x, pt_3d_end_xhlcable.y) ;
			int iIsXhlLeftOrRight = (pt_3d_end_xhlcable.x>pt_3d_start_xhlcable.x?1:0) ; //0:左侧,1:右侧;
			//获取所属电缆孔
			long nHandleCableImport=0;
			CString strDevType;
			m_mysys.GetXdata(nHandleXhlCable2,_T("CABLEIMPORT"),nHandleCableImport);
			//连接的设备类型
			m_mysys.GetXdata(nHandleXhlCable2,_T("DEVTYPE"),strDevType);
			CString strBranchIndex = _T("0") ;
			strSql.Format(_T("insert into cablenet_ent(handle_low, devtype, devname, dev_for, boxtype, box_block, left_or_right, belong_xhl_handle_low, branch_index) \
							 values(%d, '%s','%s','%s','%s', '%s', %d, %d, '%s')"),\
							 nHandleXhlCable2, strDevType,_T("MAIN"), _T("JY1"), _T("XHL_0"), _T("DZ_XHL_1"), iIsXhlLeftOrRight,  nHandleCableImport, strBranchIndex) ;
			m_AdoDb.Execute(strSql) ;
			CString strMsg ;
			strMsg.Format(_T("读取第%d根主电缆槽相连的电缆..."), i+1) ;
			acedSetStatusBarProgressMeter(strMsg, 0, nSumCable) ;	
			for (int j=0; j<nSumCable; j++)
			{
				acedSetStatusBarProgressMeterPos(j) ;
				AcDbEntity* pEntCable = NULL ;
				long nHandleCable = nArrayHandleCable[2].GetAt(j) ;
				if (m_mysys.OpenAcDbEntity(nHandleCable, pEntCable, AcDb::kForRead))
				{
					AcDbPolyline* pPolyLineCable = AcDbPolyline::cast(pEntCable) ;
					AcGePoint3d pt_3d_start_cable, pt_3d_end_cable ;
					pPolyLineCable->getStartPoint(pt_3d_start_cable) ;
					pPolyLineCable->getEndPoint(pt_3d_end_cable) ;
					UINT nVertexIndex_Start = -1, nVertexIndex_End = -1 ;		
					int iIsPtOnPline_Start = -1, iIsPtOnPline_End = -1 ;
					//电缆起点和终点是否在主电缆上
					iIsPtOnPline_Start = m_mysys.IsPointOnPolyLine(pPolyLineXhlCable, pt_3d_start_cable, nVertexIndex_Start, 0.25) ;
					iIsPtOnPline_End = m_mysys.IsPointOnPolyLine(pPolyLineXhlCable, pt_3d_end_cable, nVertexIndex_End, 0.25) ;
					if ((iIsPtOnPline_Start!=-1&&iIsPtOnPline_End==-1) || (iIsPtOnPline_Start==-1&&iIsPtOnPline_End!=-1))
					{					
						//acutPrintf(_T("\n连接到主缆的线：%d, start_point(%.3f, %.3f), end_point(%.3f, %.3f)"),nHandleCable, pt_3d_start_cable.x, pt_3d_start_cable.y, pt_3d_end_cable.x, pt_3d_end_cable.y) ;
						CString strCableName ;
						this->GetCableName(nHandleCable, strCableName) ;
						m_strCurNameIntoXhl = strCableName ;
						if (strCableName.GetLength()>1)
						{
							int iTypeJlt = CZnUtility::GetJltType(strCableName.Left(2)) ;
							if (iTypeJlt!=-1)
							{
								m_iCurDljlType = iTypeJlt ;
							}
						}
						CString strCableString;
						CString strBackCableString;
					//	m_mysys.GetXdata(nHandleCable,_T("CABLESTRING"),strCableString);
						long nHandleCableString = 0 ; //电缆（箱盒之间的多段线）对应的文本
						this->DT_GetCableString(nHandleCable, nHandleCableString, 0) ;
						if (nHandleCableString!=0)
						{
							//m_mysys.GetTextString(nHandleCableString, strCableString) ;
							m_mysys.GetAttValue(nHandleCableString,_T("TEXT"),strCableString);
							m_mysys.GetAttValue(nHandleCableString,_T("TEXT2"),strBackCableString);
						}
						CString strBranchIndex=_T("0-1") ;
						strSql.Format(_T("insert into cablenet_ent(handle_low, devname, dev_for, boxtype, box_block, left_or_right, belong_xhl_handle_low, branch_index,cablestring,backup_string) \
										 values(%d,'%s','%s','%s', '%s', %d, %d, '%s','%s','%s')"),\
										 nHandleCable, strCableName, _T("JY1"), _T("XHL_0"), _T("DZ_XHL_1"), iIsXhlLeftOrRight,  nHandleXhlCable2, strBranchIndex,strCableString,strBackCableString) ;
						m_AdoDb.Execute(strSql) ;
						this->DT_FindBox(nHandleCable, nHandleXhlCable2, nHandleXhlCable2, strBranchIndex) ;
					}
					pEntCable->close() ;
				}
			}
			acedRestoreStatusBar() ;
			pEntXhlCable->close() ;
		}
	}
}
// 读取径路图到数据库
void CZnSxt::DT_ReadJltToDbTest(void)
{
	CString strSql ;

	m_AdoDb.ClearTable(_T("cablenet_ent")) ;

	CLongArray nArrayHandleCable[3] ;
	this->DT_GetCable(nArrayHandleCable) ;

	int nSumCable = 0 ;
	nSumCable = nArrayHandleCable[2].GetCount() ;
	acutPrintf(_T("\n电缆:%d根"), nSumCable) ;

	int nSumXhlCable = 0 ;
	nSumXhlCable = nArrayHandleCable[1].GetCount() ;
	acutPrintf(_T("\n串联至信号楼电缆:%d根"), nSumXhlCable) ;

	int nSumXhlCable2 = 0 ;
	nSumXhlCable2 = nArrayHandleCable[0].GetCount() ;
	acutPrintf(_T("\n信号楼主电缆:%d根"), nSumXhlCable2) ;
	//读入串联电缆或者合缆电缆
	for (int i=0; i<nSumXhlCable; i++)
	{
		AcDbEntity* pEntXhlCable = NULL ;
		//取串联至信号楼电缆
		sequential_tree<CBoxNodeEnt>* pSeqTree=NULL;;
		long nHandleXhlCable = nArrayHandleCable[1].GetAt(i) ;
		if (m_mysys.OpenAcDbEntity(nHandleXhlCable, pEntXhlCable, AcDb::kForRead))
		{
			AcDbPolyline* pPolyLineXhlCable = AcDbPolyline::cast(pEntXhlCable) ;
			AcGePoint3d pt_3d_start_xhlcable, pt_3d_end_xhlcable ;
			pPolyLineXhlCable->getStartPoint(pt_3d_start_xhlcable) ;
			pPolyLineXhlCable->getEndPoint(pt_3d_end_xhlcable) ;
			//acutPrintf(_T("\n主缆线：%d, end_point(%.3f, %.3f)"),nHandleXhlCable, pt_3d_end_xhlcable.x, pt_3d_end_xhlcable.y) ;
			int iIsXhlLeftOrRight = (pt_3d_end_xhlcable.x>pt_3d_start_xhlcable.x?0:1) ; //0:左侧,1:右侧
			//找到所属电缆孔
			//找到终点所连接的盒子
			long nHandleCableImport=0;
			CLongArray nArrayHandleFindBox_Start;
			CLongArray nArrayHandleFindBox_End;
			int iRetFind_Start =CZnUtility::DT_FindConBox(pt_3d_start_xhlcable, nArrayHandleFindBox_Start) ;
			int iRetFind_End =CZnUtility::DT_FindConBox(pt_3d_end_xhlcable, nArrayHandleFindBox_End) ;
			long nHandleBox=nArrayHandleFindBox_Start.GetAt(0);
			sequential_tree<CBoxNodeEnt> cableBoxTree ;
			this->DT_BuildCableBoxTree(0, nHandleXhlCable, cableBoxTree) ;
			sequential_tree<CBoxNodeEnt>::pre_order_iterator pit = cableBoxTree.pre_order_begin();	//先遍历子节点	
			sequential_tree<CBoxNodeEnt>::pre_order_iterator pit_end = cableBoxTree.pre_order_end();	
			acutPrintf(_T("\n 先序遍历树："));
			for (; pit != pit_end; ++pit)
			{
				CBoxNodeEnt* pBoxNodeTmp = pit.node()->get() ;	
				int iNum1=0;
				int iNum2=0;
				this->DT_FindCableCoreNumFromCableString(pBoxNodeTmp->m_strCableString,iNum1,iNum2);
				acutPrintf(_T("\n nHandlelow=%d,boxtype=%s,strcablestring=%s,CoreNum=%d,CoreBack=%d"),pBoxNodeTmp->m_nHandleBox,pBoxNodeTmp->m_strBoxType,pBoxNodeTmp->m_strCableString,iNum1,iNum2);
				//acutPrintf(_T("\n nHandlelow=%d,boxtype=%s,strcablestring=%s"),pBoxNodeTmp->m_nHandleBox,pBoxNodeTmp->m_strBoxType,pBoxNodeTmp->m_strCableString);
			}
			sequential_tree<CBoxNodeEnt>::post_order_iterator pit2 = cableBoxTree.post_order_begin();	//先遍历子节点	
			sequential_tree<CBoxNodeEnt>::post_order_iterator pit_end2 = cableBoxTree.post_order_end();	
			acutPrintf(_T("\n 后序遍历树："));
			for (; pit2 != pit_end2; ++pit2)
			{
				CBoxNodeEnt* pBoxNodeTmp = pit2.node()->get() ;	
				acutPrintf(_T("\n nHandlelow=%d,boxtype=%s,strcablestring=%s"),pBoxNodeTmp->m_nHandleBox,pBoxNodeTmp->m_strBoxType,pBoxNodeTmp->m_strCableString);
			}
			sequential_tree<CBoxNodeEnt>::level_order_iterator pit3 = cableBoxTree.level_order_begin();	//先遍历子节点	
			sequential_tree<CBoxNodeEnt>::level_order_iterator pit_end3 = cableBoxTree.level_order_end();	
			acutPrintf(_T("\n 层次遍历树："));
			for (; pit3 != pit_end3; ++pit3)
			{
				CBoxNodeEnt* pBoxNodeTmp = pit3.node()->get() ;	
				int iNum1=0;
				int iNum2=0;
				this->DT_FindCableCoreNumFromCableString(pBoxNodeTmp->m_strCableString,iNum1,iNum2);
				acutPrintf(_T("\n nHandlelow=%d,boxtype=%s,strcablestring=%s,CoreNum=%d,CoreBack=%d"),pBoxNodeTmp->m_nHandleBox,pBoxNodeTmp->m_strBoxType,pBoxNodeTmp->m_strCableString,iNum1,iNum2);
			}
			pEntXhlCable->close() ;
		}
	}	
	
	//读入单拉电缆
	for (int i=0; i<nSumXhlCable2; i++)
	{
		AcDbEntity* pEntXhlCable = NULL ;
		//取和信号主电缆
		long nHandleXhlCable2 = nArrayHandleCable[0].GetAt(i) ;
		if (m_mysys.OpenAcDbEntity(nHandleXhlCable2, pEntXhlCable, AcDb::kForRead))
		{
			AcDbPolyline* pPolyLineXhlCable = AcDbPolyline::cast(pEntXhlCable) ;
			AcGePoint3d pt_3d_start_xhlcable, pt_3d_end_xhlcable ;
			pPolyLineXhlCable->getStartPoint(pt_3d_start_xhlcable) ;
			pPolyLineXhlCable->getEndPoint(pt_3d_end_xhlcable) ;
			//acutPrintf(_T("\n主缆线：%d, end_point(%.3f, %.3f)"),nHandleXhlCable, pt_3d_end_xhlcable.x, pt_3d_end_xhlcable.y) ;
			int iIsXhlLeftOrRight = (pt_3d_end_xhlcable.x>pt_3d_start_xhlcable.x?1:0) ; //0:左侧,1:右侧;
			//获取所属电缆孔
			long nHandleCableImport=0;
			m_mysys.GetXdata(nHandleXhlCable2,_T("CABLEIMPORT"),nHandleCableImport);
			CString strBranchIndex = _T("0") ;
			strSql.Format(_T("insert into cablenet_ent(handle_low, devname, dev_for, boxtype, box_block, left_or_right, belong_xhl_handle_low, branch_index) \
							 values(%d,'%s','%s','%s', '%s', %d, %d, '%s')"),\
							 nHandleXhlCable2, _T("MAIN"), _T("JY1"), _T("XHL"), _T("DZ_XHL_0"), iIsXhlLeftOrRight,  nHandleCableImport, strBranchIndex) ;
			m_AdoDb.Execute(strSql) ;
			CString strMsg ;
			strMsg.Format(_T("读取第%d根主电缆槽相连的电缆..."), i+1) ;
			acedSetStatusBarProgressMeter(strMsg, 0, nSumCable) ;	
			for (int j=0; j<nSumCable; j++)
			{
				acedSetStatusBarProgressMeterPos(j) ;
				AcDbEntity* pEntCable = NULL ;
				long nHandleCable = nArrayHandleCable[2].GetAt(j) ;
				if (m_mysys.OpenAcDbEntity(nHandleCable, pEntCable, AcDb::kForRead))
				{
					AcDbPolyline* pPolyLineCable = AcDbPolyline::cast(pEntCable) ;
					AcGePoint3d pt_3d_start_cable, pt_3d_end_cable ;
					pPolyLineCable->getStartPoint(pt_3d_start_cable) ;
					pPolyLineCable->getEndPoint(pt_3d_end_cable) ;
					UINT nVertexIndex_Start = -1, nVertexIndex_End = -1 ;		
					int iIsPtOnPline_Start = -1, iIsPtOnPline_End = -1 ;
					//电缆起点和终点是否在主电缆上
					iIsPtOnPline_Start = m_mysys.IsPointOnPolyLine(pPolyLineXhlCable, pt_3d_start_cable, nVertexIndex_Start, 0.25) ;
					iIsPtOnPline_End = m_mysys.IsPointOnPolyLine(pPolyLineXhlCable, pt_3d_end_cable, nVertexIndex_End, 0.25) ;
					if ((iIsPtOnPline_Start!=-1&&iIsPtOnPline_End==-1) || (iIsPtOnPline_Start==-1&&iIsPtOnPline_End!=-1))
					{					
						//acutPrintf(_T("\n连接到主缆的线：%d, start_point(%.3f, %.3f), end_point(%.3f, %.3f)"),nHandleCable, pt_3d_start_cable.x, pt_3d_start_cable.y, pt_3d_end_cable.x, pt_3d_end_cable.y) ;
						CString strCableName ;
						this->GetCableName(nHandleCable, strCableName) ;
						m_strCurNameIntoXhl = strCableName ;
						if (strCableName.GetLength()>1)
						{
							int iTypeJlt = CZnUtility::GetJltType(strCableName.Left(2)) ;
							if (iTypeJlt!=-1)
							{
								m_iCurDljlType = iTypeJlt ;
							}
						}
						CString strCableString;
						//m_mysys.GetXdata(nHandleCable,_T("CABLESTRING"),strCableString);
						CString strBranchIndex=_T("0-1") ;
						strSql.Format(_T("insert into cablenet_ent(handle_low, devname, dev_for, boxtype, box_block, left_or_right, belong_xhl_handle_low, branch_index,cablestring) \
										 values(%d,'%s','%s','%s', '%s', %d, %d, '%s','%s')"),\
										 nHandleCable, strCableName, _T("JY1"), _T("XHL_0"), _T("DZ_XHL_1"), iIsXhlLeftOrRight,  nHandleXhlCable2, strBranchIndex,strCableString) ;
						m_AdoDb.Execute(strSql) ;
						this->DT_FindBox(nHandleCable, nHandleXhlCable2, nHandleXhlCable2, strBranchIndex) ;
					}
					pEntCable->close() ;
				}
			}
			acedRestoreStatusBar() ;
			pEntXhlCable->close() ;
		}
	}
}
// 循环查询一条线所连接的盒子(从信号楼往末端查询,需要保证在绘径路图电缆时始终从末端往信号楼方向), 20150728改：默认（从根节点往子节点搜）iDirect=0，表示本段线搜寻方向为从末端向始端
void CZnSxt::DT_FindBox(long nHandlePline, long nHandleNextBox, long nHandleBelongXhl, CString& strBranchIndex)
{
	AcDbEntity* pEnt = NULL ;
	if (m_mysys.OpenAcDbEntity(nHandlePline, pEnt, AcDb::kForRead))
	{
		if (pEnt->isKindOf(AcDbPolyline::desc()))
		{
			AcDbPolyline* pPolyLine = AcDbPolyline::cast(pEnt) ;
			AcGePoint3d pt_3d_start, pt_3d_end ;
			pPolyLine->getStartPoint(pt_3d_start) ;
			pPolyLine->getEndPoint(pt_3d_end) ;
			CLongArray nArrayHandleFindBox_Start, nArrayHandleFindBox_End ;
			long nHandleBoxFind = 0 ;

			//查找在pt_3d_start,pt_3d_end处的盒子，返回值：0，未找到;1，找到一个多段线（主电缆），>1，盒子
			int iRetFind_Start =CZnUtility::DT_FindConBox(pt_3d_start, nArrayHandleFindBox_Start) ;
			int iRetFind_End =CZnUtility::DT_FindConBox(pt_3d_end, nArrayHandleFindBox_End) ;
			int iLoc_Start = CBlkUtility::FindLongInArray(nArrayHandleFindBox_Start, nHandleNextBox) ;
			int iLoc_End = CBlkUtility::FindLongInArray(nArrayHandleFindBox_End, nHandleNextBox) ;

			if (nHandleNextBox==nHandleBelongXhl) //连接到主电缆的缆，其一侧连接为主栏，没盒子(此时其信号楼端盒子nHandleNextBox==信号楼端nHandleBelongXhl，其实都是该电缆的nHandlePline)
			{
				//连接到主电缆或者连接至电缆孔时
				if (iRetFind_Start>1&&(iRetFind_End==12||iRetFind_End==1))
				{
					nHandleBoxFind = nArrayHandleFindBox_Start.GetAt(0) ;
				}
				else if (iRetFind_End>1&&(iRetFind_Start==12||iRetFind_Start==1))
				{
					nHandleBoxFind = nArrayHandleFindBox_End.GetAt(0) ;
				}
				else
				{
					pEnt->close() ;
					acutPrintf(_T("\n连接到主电缆的线缆在线起点或终点（%.3f,%.3f）-（%.3f,%.3f），在非入楼端发现不是1个连接的盒子！"), pt_3d_start.x, pt_3d_start.y ,pt_3d_end.x ,pt_3d_end.y) ;
					return ;
				}
			}
			else
			{
				//否则每条电缆都应该连接两个设备或者两个电缆孔或者两个箱盒或者分别一个
				if (!(iRetFind_Start>1&&iRetFind_End>1))
				{
					pEnt->close() ;
					acutPrintf(_T("\n在线起点和终点（%.3f,%.3f）-（%.3f,%.3f），未同时各发现1个连接的盒子！"), pt_3d_start.x, pt_3d_start.y ,pt_3d_end.x ,pt_3d_end.y) ;
					return ;
				}
				if (iLoc_Start!=-1&&iLoc_End==-1)
				{
					nHandleBoxFind = nArrayHandleFindBox_End.GetAt(0) ;
				}
				else if (iLoc_Start==-1&&iLoc_End!=-1)
				{
					nHandleBoxFind = nArrayHandleFindBox_Start.GetAt(0) ;
				}
				else
				{
					pEnt->close() ;
					acutPrintf(_T("\n在线起点和终点（%.3f,%.3f）-（%.3f,%.3f），未发现合格的连接盒子！"), pt_3d_start.x, pt_3d_start.y ,pt_3d_end.x ,pt_3d_end.y) ;
					return ;
				}
			}
            //找到电缆线所连接的盒子后，再找这个盒子的其他连接电缆
			CBoxNodeEnt boxNode ;
			if (DT_GetBoxInfo(nHandleBoxFind, boxNode ))
			{
				// 			if (nHandleBoxFind==26348)
				// 			{
				// 				int iii = 0 ;
				// 			}
				boxNode.m_nHandleCable = nHandlePline ;
				long nHandleCableString = 0 ; //电缆（箱盒之间的多段线）对应的文本
				this->DT_GetCableString(nHandlePline, nHandleCableString,2) ;
				CString strCableString ;
				if (nHandleCableString!=0)
				{
					m_mysys.GetTextString(nHandleCableString, strCableString) ;
				}
				CString strBackCableString;
				this->DT_GetCableString(nHandlePline, nHandleCableString, 0) ;
				if (nHandleCableString!=0)
				{
					m_mysys.GetAttValue(nHandleCableString,_T("TEXT2"),strBackCableString);
				}
				m_mysys.GetXdata(nHandlePline,_T("CABLESTRING"),strCableString);
				//begin 补充绘制电缆编号
				//20151121：肯尼亚非要把电缆编号写到电缆字符串前面
				CString strCableSn ;
// 				CBlkUtility::RegReplace(_T("^(.*-)?(\\d+-\\d+[A-Z]*\\(\\d+\\)).*$"), _T("$1"), strCableString, strCableSn) ;
// 				strCableSn.Trim() ;
// 				if (strCableSn.IsEmpty())
// 				{
// 					long nHandleCableSn = 0 ;
// 					this->GetCableString(nHandlePline, nHandleCableSn, 3) ;
// 					strCableSn = strBranchIndex.Right(strBranchIndex.GetLength()-2) ;
// 					strCableSn.Replace(_T('-'), _T('.')) ;
// 					strCableSn.Insert(0, m_strCurNameIntoXhl.Left(3)) ;
// 					strCableSn.Append(_T("#")) ; //20151119：迟汶，肯尼亚要求编号加上#
// 					if (nHandleCableSn==0)
// 					{
// 						this->SetCableString(nHandlePline, nHandleCableSn, strCableSn,  1) ;
// 					}
// 					else //调试过程发现莫名问题，有一nHandleCableSn为空文本，故增加下面判断处理空文本的语句
// 					{
// 						CString strText ;
// 						if (m_mysys.GetTextString(nHandleCableSn, strText))
// 						{
// 							if (strText.IsEmpty())
// 							{
// 								m_mysys.SetTextString(nHandleCableSn, strCableSn) ;
// 							}
// 							else //记录下来后续要导入数据库
// 							{
// 								strCableSn = strText ;
// 							}					
// 						}
// 					}
// 				}
// 				//end 补充绘制电缆编号

				//将电缆信息存储起来，导入excel供审核
				if (m_iCurJob==1)
				{
					m_strArrayCableSn.Add(strCableSn) ;
					m_strArrayCableAll.Add(strCableString) ;
					//CString strAppNames[7] = {_T("DIST1"), _T("DIST2"), _T("CROSSGD"),  _T("BOXNUM"), _T("PLUS"), _T("BOXFNUM"), _T("INTOXHL")} ;
					CString strAppNames[11] = {_T("FATHER"),_T("DIST1"), _T("DIST2"), _T("ZUOTOU"),  _T("DISTTOCABLE"), _T("GUOGUINUM"), _T("CDLIAN"), _T("SXJNUM"), _T("RFMNUM"),_T("ANZHUNAG"),_T("XIUZHENG")} ;
					m_vecCableData.resize(11) ;
					for (int i=0; i<11; i++)
					{
						long nData = 0 ;
						if (m_mysys.GetXdata(nHandlePline, strAppNames[i], nData) )
						{
							int iData = (int)nData ;
							m_vecCableData[i].push_back(iData) ;
							if (i<3)
							{
								acutPrintf(_T("\n%s:DIST%d-%d"), strCableSn, i+1,iData) ;
							}
						}
					}	
				}

				//查询和盒子相连的其他电缆
				CLongArray nArrayHandleFindCableTmp ;
				int iFindCable = this->DT_FindCable(nHandleBoxFind, nHandlePline, nArrayHandleFindCableTmp) ;
				CLongArray nArrayHandleFindCable ;
				//只保留为电缆的连接线
				//先添加尾缆
				for (int i=0; i<iFindCable; i++)
				{
					long iIsCable = 0 ;
					if(m_mysys.GetXdata(nArrayHandleFindCableTmp.GetAt(i), _T("IS_CABLE"), iIsCable)&&iIsCable==1)
					{
						long iIsEndCable=0;
						if(m_mysys.GetXdata(nArrayHandleFindCableTmp.GetAt(i),_T("IS_ENDCABLE"),iIsEndCable)&&iIsEndCable==1)
						{
							nArrayHandleFindCable.Add(nArrayHandleFindCableTmp.GetAt(i)) ;
						}
					}
				}
				for (int i=0; i<iFindCable; i++)
				{
					long iIsCable = 0 ;
					if(m_mysys.GetXdata(nArrayHandleFindCableTmp.GetAt(i), _T("IS_CABLE"), iIsCable)&&iIsCable==1)
					{
						long iIsEndCable=0;
						if(m_mysys.GetXdata(nArrayHandleFindCableTmp.GetAt(i),_T("IS_ENDCABLE"),iIsEndCable)&&iIsEndCable==1)
						{
							//nArrayHandleFindCable.Add(nArrayHandleFindCableTmp.GetAt(i)) ;
						}
						else
							nArrayHandleFindCable.Add(nArrayHandleFindCableTmp.GetAt(i)) ;		
					}
				}
			
				int iIsStart = 0 ;
				iFindCable = nArrayHandleFindCable.GetCount() ;
				if(iFindCable==0)
				{
					iIsStart = 1 ;
				}

				CString strDevFor = _T("JY1") ;
				if (boxNode.m_iEntType==1)
				{
					if (m_iDsNumXhj==1)
					{
						strDevFor = _T("JY3") ;
					}
					if (boxNode.m_strDevBlock.Mid(4,2)==_T("DC")) //调车
					{
						if (m_iDcDsNum==1)
						{
							strDevFor = _T("JY2") ;
						}
						else if (m_iDcDsNum==2)
						{
							strDevFor = _T("JY3") ;
						}
						else
						{
							strDevFor = _T("JY1") ;
						}
					}
				}
				if (boxNode.m_iEntType==0)
				{
					if (m_iQkNum==1)
					{
						strDevFor = _T("JY4") ;
					}
				}

				CString strSql ;
// 				strSql.Format(_T("insert into cablenet_ent(handle_low, devname, devtype, dev_block, dev_for, boxtype, box_block, next_ent_handle_low, cablestring, cable_sn, is_start, belong_xhl_handle_low, distance_to_xhl, branch_index, zzj_type, zzj_index, freq) \
// 								 values(%d, '%s', '%s', '%s', '%s', '%s', '%s', %d, '%s', '%s', %d, %d, %d, '%s', '%s', '%s', '%s')"),\
// 								 nHandleBoxFind, boxNode.m_strDevName, boxNode.m_strDevType, boxNode.m_strDevBlock, strDevFor, boxNode.m_strBoxType, boxNode.m_strBoxBlock, nHandleNextBox, strCableString, strCableSn,  iIsStart, nHandleBelongXhl, boxNode.m_iDistToXhl, strBranchIndex, boxNode.m_strDragType, boxNode.m_strActIndex, boxNode.m_strFreq) ;
				//获取盒子的X坐标
                AcGePoint3d pt_3d_box;
				this->GetBlockPosition(boxNode.m_nHandleBox,pt_3d_box);
				int iStart=0;
				if(boxNode.m_strBoxBlock.Left(3)=_T("DT_"))
				{
					//是否为搜索起点的依据：不是盒子的话，就是起点
					if(boxNode.m_strBoxBlock.Left(7)!=_T("DT_BOX_"))
					{
						iStart=1;
					}
				}
				//long nHandleCableString = 0 ; //电缆（箱盒之间的多段线）对应的文本
			
				strSql.Format(_T("insert into cablenet_ent(handle_low, pt_x,pt_y,devname, dev_for, boxtype, box_block, is_start,left_or_right, next_ent_handle_low,cablestring,backup_string, belong_xhl_handle_low, distance_to_xhl,branch_index) \
								 values(%d,%f,%f,'%s','%s','%s', '%s',%d, %d, %d, '%s','%s',%d,%d,'%s')"),\
								 nHandleBoxFind, pt_3d_box.x,pt_3d_box.y,boxNode.m_strDevName, boxNode.m_strDevBlock, boxNode.m_strBoxType, boxNode.m_strBoxBlock,iStart,0,nHandleNextBox,strCableString,strBackCableString,nHandleBelongXhl, boxNode.m_iDistToXhl,strBranchIndex) ;			
				m_AdoDb.Execute(strSql) ;
				for (int i=0; i<iFindCable; i++)
				{
					CString strBranchIndexTmp ;
					strBranchIndexTmp.Format(_T("%s-%d"), strBranchIndex, i+1 ) ;
					this->DT_FindBox(nArrayHandleFindCable.GetAt(i), nHandleBoxFind, nHandleBelongXhl, strBranchIndexTmp) ;
				}
			}
		}
		pEnt->close() ;
	}
}

BOOL CZnSxt::DT_GetBoxInfo(long nHandleBox, CBoxNodeEnt& boxNode)
{
	BOOL bRet = FALSE ;

	AcDbEntity* pEntFind = NULL ;
	long nHandleDev=0;
 	m_mysys.GetXdata(nHandleBox,_T("BELONGTO_PMT"),nHandleDev);
	if (m_mysys.OpenAcDbEntity(nHandleBox, pEntFind, AcDb::kForRead))
	{
		if (pEntFind->isKindOf(AcDbBlockReference::desc()))
		{
			AcDbBlockReference* pRef = AcDbBlockReference::cast(pEntFind) ;
			//获取箱盒名称
			boxNode.m_nHandleBox=nHandleBox;
			m_mysys.GetBlockName(pRef, boxNode.m_strBoxBlock) ;
			if(boxNode.m_strBoxBlock.Left(6)==_T("DT_XHJ"))
			{
				m_mysys.GetAttValue(pRef,_T("XHJNAME"),boxNode.m_strBoxType);
			}
			else if(boxNode.m_strBoxBlock.Left(6)==_T("DT_BOX"))
			{
				m_mysys.GetAttValue(pRef,_T("TYPE"),boxNode.m_strBoxType);
			}
			else
			{
				m_mysys.GetAttValue(pRef,_T("NAME"),boxNode.m_strBoxType);
			}
            boxNode.m_nHandleDev=nHandleDev;
			m_mysys.GetBlockName(nHandleDev,boxNode.m_strDevBlock);
			//m_mysys.GetAttValue(nHandleDev,_T("NAME"),boxNode.m_strDevName);
			CStringArray strArrayBlkNameBox ;
			CBlkUtility::DivideString(boxNode.m_strBoxBlock, _T("_"), strArrayBlkNameBox) ;
			int iNum = strArrayBlkNameBox.GetCount() ;
			if (iNum<3)
			{
				pEntFind->close() ;
				return FALSE ;
			}
			CString strBlkNameBoxPart0 = strArrayBlkNameBox.GetAt(0) ;
			CString strBlkNameBoxPart1 = strArrayBlkNameBox.GetAt(1) ;
			if (strBlkNameBoxPart0==_T("DT"))
			{
				if (strBlkNameBoxPart1.Left(3)==_T("BOX"))
				{
// 					m_mysys.GetAttValue(pRef, _T("HFNUM"), boxNode.m_strDevName) ;							
// 					int iHfNum = 0 ;
// 					m_mysys.GetAttValue(pRef, _T("HF"), iHfNum) ;
					//boxNode.m_strBoxType.Format(_T("%s"), ) ;
					CString strDistToXhl ;
					this->GetBoxLocation(nHandleBox,strDistToXhl);
					m_mysys.GetAttValue(pRef,_T("TYPE"),boxNode.m_strBoxType);
					//boxNode.m_nHandleDev=nHandleDev;
					//m_mysys.GetBlockName(nHandleDev,boxNode.m_strDevBlock);
					m_mysys.GetAttValue(nHandleDev,_T("NAME"),boxNode.m_strDevName);
					//m_mysys.GetAttValue(pRef, _T("DIST"), strDistToXhl) ;
					//strDistToXhl.TrimLeft(_T('(')) ;
					//strDistToXhl.TrimRight(_T(')')) ;
					double d_temp=0;
					CBlkUtility::CStringToDouble(strDistToXhl,d_temp);	
					//boxNode.m_strBoxBlock=
					boxNode.m_iDistToXhl = (int) d_temp;
					boxNode.m_iEntType = 1 ;
					//m_iCurDljlType = -1 ;
					//pBoxNode = new CBoxNodeBase(strBoxType, strDevName, _T(""), nHandleBox,  0,  strBlkNameBox, _T(""), _T(""), iDistToXhl) ; 
				}
				else if (strBlkNameBoxPart1.Left(2)==_T("AP"))
				{
					CString strDistToXhl ;
					this->GetBoxLocation(nHandleBox,strDistToXhl);
					double d_temp=0;
					CBlkUtility::CStringToDouble(strDistToXhl,d_temp);	
					m_mysys.GetAttValue(pRef,_T("NAME"),boxNode.m_strBoxType);
					//boxNode.m_nHandleDev=nHandleDev;
					//m_mysys.GetBlockName(nHandleDev,boxNode.m_strDevBlock);
					m_mysys.GetAttValue(nHandleDev,_T("NAME"),boxNode.m_strDevName);
					boxNode.m_iDistToXhl = (int) d_temp;
					boxNode.m_iEntType = 2 ;
					//pBoxNode = new CBoxNodeXhj(strBoxType, strDevName, strDevType, iBoxSumCurXhj, iBoxIndexCurXhj, nHandleBox,  nHandleBoxBelong, strBlkNameBox, strBlkNameDev, _T(""), 0) ;
						
				}
				else if(strBlkNameBoxPart1.Left(2)==_T("JZ"))
				{
					CString strDistToXhl ;
					this->GetBoxLocation(nHandleBox,strDistToXhl);
					double d_temp=0;
					CBlkUtility::CStringToDouble(strDistToXhl,d_temp);	
					m_mysys.GetAttValue(pRef,_T("NAME"),boxNode.m_strBoxType);
					//boxNode.m_nHandleDev=nHandleDev;
					//m_mysys.GetBlockName(nHandleDev,boxNode.m_strDevBlock);
					m_mysys.GetAttValue(nHandleDev,_T("NAME"),boxNode.m_strDevName);
					boxNode.m_iDistToXhl = (int) d_temp;
					boxNode.m_iEntType = 4 ;

				}
			else if (strBlkNameBoxPart1.Left(11)==_T("CABLEIMPORT"))
			{
				CString strDistToXhl ;
				this->GetBoxLocation(nHandleBox,strDistToXhl);
				double d_temp=0;
				CBlkUtility::CStringToDouble(strDistToXhl,d_temp);	
				m_mysys.GetAttValue(pRef,_T("TYPE"),boxNode.m_strBoxType);
				boxNode.m_strBoxType=_T("XHL");
				boxNode.m_strDevName=_T("CABLEIMPORT");
				//boxNode.m_nHandleDev=nHandleDev;
				//m_mysys.GetBlockName(nHandleDev,boxNode.m_strDevBlock);
				//m_mysys.GetAttValue(nHandleDev,_T("NAME"),boxNode.m_strDevName);
				boxNode.m_iDistToXhl = (int) d_temp;
				boxNode.m_iEntType = 3 ;
				//pBoxNode = new CBoxNodeXhj(strBoxType, strDevName, strDevType, iBoxSumCurXhj, iBoxIndexCurXhj, nHandleBox,  nHandleBoxBelong, strBlkNameBox, strBlkNameDev, _T(""), 0) ;
			}
		}
				bRet = TRUE ;
		}
	}
		pEntFind->close() ;

	return bRet ;
}
//取得所有信号楼电缆径路和连到信号楼电缆径路一起其他电缆径路
int CZnSxt::DT_GetCable(CLongArray (&nArrayHandle)[3])
{
	AcDbBlockTable * pBlkTbl = NULL ;
	m_pDb->getBlockTable(pBlkTbl,AcDb::kForRead);

	AcDbBlockTableRecord * pBlkTblRec = NULL ;
	pBlkTbl->getAt ( ACDB_MODEL_SPACE, pBlkTblRec,AcDb::kForRead )  ;
	pBlkTbl->close() ;
	AcDbBlockTableRecordIterator *pBlkTblRecIter = NULL ;
	pBlkTblRec->newIterator( pBlkTblRecIter );
	pBlkTblRec->close();

	AcDbEntity * pEnt = NULL ;
	for(pBlkTblRecIter->start();!pBlkTblRecIter->done();pBlkTblRecIter->step())
	{
		//遍历并打开实体 ;	
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForRead))
		{
			//不可见的实体（自身隐藏或者所在层隐藏）放弃
			if (pEnt->visibility()==AcDb::kInvisible||m_mysys.IsOnOffLayer(pEnt))
			{
				pEnt->close() ;
				continue;
			}

			if (pEnt->isKindOf(AcDbPolyline::desc()))
			{
				long nIsCable = 0 ;
				AcDbPolyline* pPolyLine = AcDbPolyline::cast(pEnt) ;
				AcGePoint3d pt_3d_start, pt_3d_end ;
				pPolyLine->getStartPoint(pt_3d_start) ;
				pPolyLine->getEndPoint(pt_3d_end) ;
				CLongArray nArrayHandleFindBox_Start, nArrayHandleFindBox_End ;
				long nHandleBoxFind = 0 ;

				//查找在pt_3d_start,pt_3d_end处的盒子，返回值：0，未找到;1，找到一个多段线（主电缆），>1，盒子
				int iRetFind_Start =CZnUtility::DT_FindConBox(pt_3d_start, nArrayHandleFindBox_Start) ;
				int iRetFind_End =CZnUtility::DT_FindConBox(pt_3d_end, nArrayHandleFindBox_End) ;
				if(m_mysys.GetXdata(pEnt, _T("IS_CABLE"), nIsCable)&&nIsCable==1)
				{
					long nHandleCur = m_mysys.GetHandle(pEnt) ;
					long nIsXhlCable = 0 ;
					if(m_mysys.GetXdata(pEnt, _T("IS_XHLCABLE"), nIsXhlCable)&&nIsXhlCable==1)
					{
						//读取所有信号楼主电缆
						nArrayHandle[0].Add(nHandleCur) ;
					}
					else if(iRetFind_End==12||iRetFind_Start==12)
					{
						//串联至信号楼
						nArrayHandle[1].Add(nHandleCur) ;
					}
					else
					{
						//其他电缆
						nArrayHandle[2].Add(nHandleCur);
					}
				}
			}

			pEnt->close() ;
		}	
	}
	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;
	return 0;
}

// for tree 循环查询一条线所连接的盒子(从信号楼往末端查询,需要保证在绘径路图电缆时始终从末端往信号楼方向) 
void CZnSxt::DT_FindBox(long nHandlePline, long nHandleNextBox, sequential_tree<CBoxNodeEnt>* pSeqTree)
{
	AcDbEntity* pEnt = NULL ;
	if (m_mysys.OpenAcDbEntity(nHandlePline, pEnt, AcDb::kForRead))
	{
		if (pEnt->isKindOf(AcDbPolyline::desc()))
		{
			AcDbPolyline* pPolyLine = AcDbPolyline::cast(pEnt) ;
			AcGePoint3d pt_3d_start ;
			pPolyLine->getStartPoint(pt_3d_start) ;
			CLongArray nArrayHandleFindBox ;
			CZnUtility::DT_FindConBox(pt_3d_start, nArrayHandleFindBox) ;
			int iNumFind = nArrayHandleFindBox.GetCount() ;
			int iBoxSum = 0 ;
			for (int iFindIndex=0; iFindIndex<iNumFind; iFindIndex++)
			{
				long nHandleBox = nArrayHandleFindBox.GetAt(iFindIndex) ;
				CBoxNodeEnt boxNode ;
				m_mysys.GetXdata(nHandlePline,_T("CABLESTRING"),boxNode.m_strCableString);
				if (DT_GetBoxInfo(nHandleBox, boxNode))
				{
					if (iBoxSum==0)
					{
						boxNode.m_nHandleCable = nHandlePline ;
						sequential_tree<CBoxNodeEnt>::iterator it = pSeqTree->insert(boxNode) ;
						//查询和盒子相连的其他电缆
						CLongArray nArrayHandleFindCable ;
						int iFindCable = this->DT_FindCable(nHandleBox, nHandlePline, nArrayHandleFindCable) ;
						for (int i=0; i<iFindCable; i++)
						{
							this->DT_FindBox(nArrayHandleFindCable.GetAt(i), nHandleBox, it.node()) ;
						}
					}
					else
					{
						acutPrintf(_T("\n注意：在 %.3f, %.3f 处有不止1个Box！"), pt_3d_start.x, pt_3d_start.y) ;
					}

					iBoxSum++ ;
				}
			}
		}

		pEnt->close() ;
	}
}
// 建立一个电缆径路数（节点是箱盒）
void CZnSxt::DT_BuildCableBoxTree(long nHandleBox, long nHandleCable, sequential_tree<CBoxNodeEnt>& cableTree)
{
	CBoxNodeEnt boxNode ;
	if (nHandleBox==0) //入楼端（连接到主电缆的径路）
	{
		this->GetCableName(nHandleCable, boxNode.m_strDevName) ;
		m_mysys.GetXdata(nHandleCable,_T("CABLESTRING"),boxNode.m_strCableString);
		boxNode.m_strBoxType = _T("XHL") ;
		boxNode.m_iEntType = -2 ; //信号楼
		cableTree.set(boxNode) ;

		this->DT_FindBox(nHandleCable, nHandleCable, &cableTree) ;
	}
	else
	{
		if(DT_GetBoxInfo(nHandleBox, boxNode) )
		{
			boxNode.m_nHandleCable = nHandleCable ;
			long nHandleCableString = 0 ;
			if (this->DT_GetCableString(nHandleCable, nHandleCableString,0) ==1)
			{
				m_mysys.GetTextString(nHandleCableString, boxNode.m_strCableString) ;
			}
			cableTree.set(boxNode) ;

			CLongArray nArrayCable ;
			int iNumCable = this->DT_FindCable(nHandleBox, nHandleCable, nArrayCable) ;
			CLongArray nArrayHandleFindCableTmp;
			for (int i=0; i<iNumCable; i++)
			{
				long iIsCable = 0 ;
				if(m_mysys.GetXdata(nArrayCable.GetAt(i), _T("IS_CABLE"), iIsCable)&&iIsCable==1)
				{
					long iIsEndCable=0;
					if(m_mysys.GetXdata(nArrayCable.GetAt(i),_T("IS_ENDCABLE"),iIsEndCable)&&iIsEndCable==1)
					{
						//nArrayHandleFindCable.Add(nArrayHandleFindCableTmp.GetAt(i)) ;
					}
					else
						nArrayHandleFindCableTmp.Add(nArrayCable.GetAt(i)) ;		
				}
			}
			for (int i=0; i<iNumCable; i++)
			{
				long iIsCable = 0 ;
				if(m_mysys.GetXdata(nArrayCable.GetAt(i), _T("IS_CABLE"), iIsCable)&&iIsCable==1)
				{
					long iIsEndCable=0;
					if(m_mysys.GetXdata(nArrayCable.GetAt(i),_T("IS_ENDCABLE"),iIsEndCable)&&iIsEndCable==1)
					{
						nArrayHandleFindCableTmp.Add(nArrayCable.GetAt(i)) ;
					}
				}
			}
	      int i_count=nArrayHandleFindCableTmp.GetCount();
			for (int i=0; i<i_count; i++)
			{
				this->DT_FindBox(nArrayHandleFindCableTmp.GetAt(i), nHandleBox, &cableTree) ;
			}
		}
	}

}
BOOL CZnSxt::DT_FindCableCoreNumFromCableString( CString strCableString,int &iNum_Cable,int &iNum_Back )
{
	CString CableString=strCableString;
	int i_dex1=CableString.Find(_T('-'));
	int i_dex2=CableString.Find(_T('('));
	int i_dex3=CableString.Find(_T(')'));
	CString strNum_Cable;
	CString strNum_Back;
	BOOL bRet=FALSE;
	int ilen=strCableString.GetLength();
	if(i_dex1<0||i_dex1>ilen||i_dex2<0||i_dex2>ilen||i_dex3<0||i_dex3>ilen)
	{
		return bRet;
	}
	for(int i=i_dex1;i<i_dex2;i++)
	{
		TCHAR ch=CableString.GetAt(i);
		if(ch>=_T('0')&&ch<=_T('9'))
		{
			strNum_Cable.AppendChar(ch);
		}
	}
	for(int i=i_dex2+1;i<i_dex3;i++)
	{
		TCHAR ch=CableString.GetAt(i);
		if(ch>=_T('0')&&ch<=_T('9'))
		{
			strNum_Back.AppendChar(ch);
		}
	}
	iNum_Cable=StrToIntA(strNum_Cable);
	iNum_Back=StrToIntA(strNum_Back);
	bRet=TRUE;
	return bRet;
}
// 查找满足条件的电缆型号, iDevType=-1：通用（混合），0：道岔，1，信号机，2：轨道电路；3：电码化，iCableType=0,普通综合扭绞（数字）电缆，1，内屏蔽数字电缆
BOOL CZnSxt::DT_GetUseCableType(int iUseCoreNum,  int& iBackupNum, CString& strCableTypeName, int iDevType, int iCableType)
{
	BOOL bRet = FALSE ;
	try
	{
		CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;
		//strSql.Format(_T("select top 1 * from define_cable_type where cable_type=%d and fact_use_num>=%d %s order by id asc"), iCableType, iUseCoreNum, m_iCurDljlType==1?_T("and cable_name not in('14','14A','14B')"):_T("")) ;
		strSql.Format(_T("select top 1 * from define_cable_type where cable_type=%d and fact_use_num>=%d and right(cable_name,1)<>'A' order by id asc"), iCableType, iUseCoreNum) ;
		if (cSet.Open(strSql))
		{
			if (cSet.GetRecordCount()==1)
			{
				int iCoreNum = 0 ;	
				cSet.GetFieldValue(_T("cable_name"), strCableTypeName) ;
				cSet.GetFieldValue(_T("core_num"), iCoreNum) ;
				iBackupNum = iCoreNum-iUseCoreNum ;

				bRet = TRUE ;
			}
			cSet.Close() ;
		}

	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误!in DT_GetUseCableType"));
	}
	return bRet;
}
// 计算指定盒子引入电缆总芯数
int CZnSxt::DT_GetCableCoreNum(sequential_tree<CBoxNodeEnt>& cableBoxTree, CString& strCableTypeName, int& iBackup, int iDist)
{
	int iRet = 0 ;

	sequential_tree<CBoxNodeEnt>::post_order_iterator pit = cableBoxTree.post_order_begin();	//先遍历子节点	
	sequential_tree<CBoxNodeEnt>::post_order_iterator pit_end = cableBoxTree.post_order_end();	
	int iNumOfNodeVarFreq[3] = {0, 0, 0} ; //记录不同频率的节点个数，依次记录偶数频率、奇数频率、无频率（只对电码化有用）
	for (; pit != pit_end; ++pit)
	{
		//acutPrintf(_T("\n:%s, %s, %s"), pit->m_strBoxType, pit->m_strDevName, pit->m_strDevType) ;
		//begin 仅针对电码化
		if (m_iCurDljlType==3)
		{
			CBoxNodeEnt* pBoxNodeTmp = pit.node()->get() ;				

			//const CString strFreqCurNode = pBoxNodeTmp->m_strFreq ;
			if (! pBoxNodeTmp->m_strFreq.IsEmpty())
			{
				int iTmp = _ttoi(pBoxNodeTmp->m_strFreq.Left(2)) ;
				iNumOfNodeVarFreq[iTmp%2]++ ;
			}
			else
			{
				iNumOfNodeVarFreq[2]++ ;
			}
		}
		//end 仅针对电码化
		//本身的芯线（分向盒之类没有）
		if (!pit->m_strDevType.IsEmpty())
		{
			long iLenForPlus = 0 ;
			m_mysys.GetXdata(pit->m_nHandleCable, _T("LENFORPLUS"), iLenForPlus) ;
			this->GetDevCore(*(pit.node()->get()), iLenForPlus>0?iLenForPlus:iDist) ;				
		}
		//从子节点引入的芯线
		sequential_tree<CBoxNodeEnt>::iterator it = pit.node()->begin();
		for (; it != pit.node()->end(); ++it) //遍历各子节点
		{
			vector<CCableCore>::iterator it_core = it->m_vecCableCore.begin() ;
			for (;it_core!=it->m_vecCableCore.end(); ++it_core) //遍历当前子节点的芯线
			{
				if(pit->FindCableCore(*it_core)==false)//查找父节点本身新出芯是否已有，有则舍弃此条
				{
					if (it_core->IsInside()!=1)
					{
						pit->m_vecCableCore.push_back(*it_core) ;
					}
				}
			}
		}
	}

	int iNumNeed4PGroup = 0 ;
	//int iBackupNum = 0 ;
	//CString strCableTypeName ;
	iRet = cableBoxTree.get()->m_vecCableCore.size() ; //实际使用芯数
	int iCableType = 0 ;//普通电缆，1是屏蔽电缆

	if (m_iCurDljlType==3)
	{
		iCableType = 1 ;
		iNumNeed4PGroup = min(iNumOfNodeVarFreq[0], iNumOfNodeVarFreq[1])+abs(iNumOfNodeVarFreq[0]-iNumOfNodeVarFreq[1]) ;//需要用到的内屏蔽4芯组个数	
	}

	this->GetUseCableType(iRet, iNumNeed4PGroup, iBackup, strCableTypeName, m_iCurDljlType, iCableType ) ;
	//strCableTypeAndBackup.Format(_T("%s(%d)"), strCableTypeName, iBackupNum) ;

	return iRet ;
}
#include "stdafx.h"
#include "znutility.h"
#include "SBDestination.h"
//#include "ZnSxt.h"
#include "XhlCableInfoDlg.h"

CZnUtility::CZnUtility(void)
{
}

CZnUtility::~CZnUtility(void)
{
}

//查找在pt_3d_base处的盒子，返回值：0，未找到;1，找到一个多段线（主电缆），>1，盒子
int CZnUtility::FindConBox(const AcGePoint3d& pt_3d_base, CLongArray& nArrayHandleFind, double dOffset)
{
	int iRet = 0 ;

	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase() ;
	CUserSystem mysys ;
	mysys.m_pDb = pDb ;

	CLongArray nArrayHandleXhlCable ; //
	//CLongArray nArrayHandleXhl ;

	AcDbBlockTable * pBlkTbl = NULL ;
	pDb->getBlockTable(pBlkTbl,AcDb::kForRead);

	AcDbBlockTableRecord * pBlkTblRec = NULL ;
	pBlkTbl->getAt ( ACDB_MODEL_SPACE, pBlkTblRec,AcDb::kForRead )  ;
	pBlkTbl->close() ;
	AcDbBlockTableRecordIterator *pBlkTblRecIter = NULL ;
	pBlkTblRec->newIterator( pBlkTblRecIter );
	pBlkTblRec->close();

	AcDbEntity * pEnt = NULL ;
	for(pBlkTblRecIter->start();!pBlkTblRecIter->done();pBlkTblRecIter->step())
	{
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForRead))
		{
			//不可见的实体（自身隐藏或者所在层隐藏）放弃
			if (pEnt->visibility()==AcDb::kInvisible||mysys.IsOnOffLayer(pEnt))
			{
				pEnt->close() ;
				continue;
			}

			long nHandleCur = mysys.GetHandle(pEnt) ;
			
			if (pEnt->isKindOf(AcDbBlockReference::desc()))
			{
				AcDbBlockReference* pRef = AcDbBlockReference::cast(pEnt) ;

				AcGePoint3d pt_3d_blk ;
				pt_3d_blk = pRef->position() ;
				if (pt_3d_blk.distanceTo(pt_3d_base)>10) //距离超过10 的就不考虑了
				{
					pEnt->close() ;
					continue;
				}

				CString strBlkName ;
				mysys.GetBlockName(pRef, strBlkName) ;

				if (strBlkName.Left(6)==_T("BOX_XB"))
				{
					if (IsPointOnRect(pt_3d_base.convert2d(AcGePlane::kXYPlane), pt_3d_blk.convert2d(AcGePlane::kXYPlane), 4, 5))
					{
						nArrayHandleFind.Add(nHandleCur) ;
						iRet = 2 ;
					}
				}
				else if (strBlkName.Left(7)==_T("BOX_FSD"))
				{
					if (IsPointOnRect(pt_3d_base.convert2d(AcGePlane::kXYPlane), pt_3d_blk.convert2d(AcGePlane::kXYPlane), 3, 4))
					{
						nArrayHandleFind.Add(nHandleCur) ;
						iRet = 3 ;
					}
				}
				else if (strBlkName.Left(7)==_T("PMT_XHL"))
				{
					if (IsPointOnRect(pt_3d_base.convert2d(AcGePlane::kXYPlane), pt_3d_blk.convert2d(AcGePlane::kXYPlane), 6, 10))
					{
						nArrayHandleFind.Add(nHandleCur) ;
						//nArrayHandleXhl.Add(nHandleCur) ;
						iRet = 4 ;
					}
				}
				else if (strBlkName.Left(6)==_T("BOX_HF"))
				{					
					if (abs(pt_3d_blk.distanceTo(pt_3d_base)-3)<dOffset)
					{
						nArrayHandleFind.Add(nHandleCur) ;
						iRet = 5 ;
					}
				}
				else if (strBlkName.Left(6)==_T("BOX_HZ"))
				{						
					if (abs(pt_3d_blk.distanceTo(pt_3d_base)-1.5)<dOffset)
					{
						nArrayHandleFind.Add(nHandleCur) ;
						iRet = 6 ;
					}
				}
			}
			else if (pEnt->isKindOf(AcDbPolyline::desc()))
			{
				long iIsXhlCable = 0 ;
				if (mysys.GetXdata(pEnt, _T("IS_XHLCABLE"), iIsXhlCable)&&iIsXhlCable>0)
				{
					AcDbPolyline* pPolyLine = AcDbPolyline::cast(pEnt) ;
					UINT nVertIndex = 0 ;					 
					if (mysys.IsPointOnPolyLine(pPolyLine, pt_3d_base, nVertIndex, dOffset)>-1)
					{
						nArrayHandleFind.Add(nHandleCur) ;
						nArrayHandleXhlCable.Add(nHandleCur) ;
						iRet = 1 ;
					}
				}
			}

			pEnt->close() ;
		}	
	}
	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;

	//int iNumXhl = nArrayHandleXhl.GetCount() ;
	int iNumXhlCable = nArrayHandleXhlCable.GetCount() ;
// 	if (iNumXhl==1&&iNumXhlCable>0)
// 	{
// 		nArrayHandleFind.RemoveAll() ;
// 		nArrayHandleFind.Add(nArrayHandleXhl.GetAt(0)) ;
// 		iRet = 1 ;
// 	}
	if (iNumXhlCable>0)
	{
		nArrayHandleFind.RemoveAll() ;
		nArrayHandleFind.Add(nArrayHandleXhlCable.GetAt(0)) ;
		iRet = 1 ;
	}
	int iNumFind = nArrayHandleFind.GetCount() ;
	if (iNumFind>1)
	{
		acutPrintf(_T("\n在点(%.3f, %.3f)处发现多于1个盒子，请注意"), pt_3d_base.x, pt_3d_base.y) ;
	}

	return iRet ;
}
//查找在pt_3d_base处的盒子，返回值：0，未找到;1，找到一个多段线（主电缆），>1，盒子或者设备
int CZnUtility::DT_FindConBox(const AcGePoint3d& pt_3d_base, CLongArray& nArrayHandleFind, double dOffset)
{
	int iRet = 0 ;

	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase() ;
	CUserSystem mysys ;
	mysys.m_pDb = pDb ;

	CLongArray nArrayHandleXhlCable ; //
	//CLongArray nArrayHandleXhl ;

	AcDbBlockTable * pBlkTbl = NULL ;
	pDb->getBlockTable(pBlkTbl,AcDb::kForRead);

	AcDbBlockTableRecord * pBlkTblRec = NULL ;
	pBlkTbl->getAt ( ACDB_MODEL_SPACE, pBlkTblRec,AcDb::kForRead )  ;
	pBlkTbl->close() ;
	AcDbBlockTableRecordIterator *pBlkTblRecIter = NULL ;
	pBlkTblRec->newIterator( pBlkTblRecIter );
	pBlkTblRec->close();

	AcDbEntity * pEnt = NULL ;
	for(pBlkTblRecIter->start();!pBlkTblRecIter->done();pBlkTblRecIter->step())
	{
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForRead))
		{
			//不可见的实体（自身隐藏或者所在层隐藏）放弃
			if (pEnt->visibility()==AcDb::kInvisible||mysys.IsOnOffLayer(pEnt))
			{
				pEnt->close() ;
				continue;
			}

			long nHandleCur = mysys.GetHandle(pEnt) ;

			if (pEnt->isKindOf(AcDbBlockReference::desc()))
			{
				AcDbBlockReference* pRef = AcDbBlockReference::cast(pEnt) ;

				AcGePoint3d pt_3d_blk ;
				pt_3d_blk = pRef->position() ;
				if (fabs(pt_3d_blk.x-pt_3d_base.x)>5||fabs(pt_3d_blk.y-pt_3d_base.y)>32) //水平距离超过5 的就不考虑了
				{
					pEnt->close() ;
					continue;
				}

				CString strBlkName ;
				mysys.GetBlockName(pRef, strBlkName) ;
				double d_rotation=0;
				d_rotation=pRef->rotation();
				//lzx added for dt 20151105
				if(strBlkName.Left(11)==_T("DT_BOX_RECT"))
				{
					if (fabs(pt_3d_blk.distanceTo(pt_3d_base))<3+dOffset)
					{
						nArrayHandleFind.Add(nHandleCur) ;
						iRet = 10 ;
					}
				}
				else if(strBlkName.Left(13)==_T("DT_BOX_CIRCLE"))
				{
					if (fabs(pt_3d_blk.distanceTo(pt_3d_base))<3+dOffset)
					{
						nArrayHandleFind.Add(nHandleCur) ;
						iRet = 11 ;
					}
				}
				else if (strBlkName.Left(14)==_T("DT_CABLEIMPORT"))
				{
					//水平位置相差4就是连上
					if (fabs(pt_3d_blk.x-pt_3d_base.x)<4+dOffset)
					{
						nArrayHandleFind.Add(nHandleCur) ;
						iRet = 12 ;
					}
				}
				else if(strBlkName.Left(6)==_T("DT_AP_"))
				{
					if(fabs(d_rotation)<dOffset)
					{
						pt_3d_blk.y+=3;
					}
					else if(fabs(d_rotation-PI)<dOffset)
					{
						pt_3d_blk.y-=3;
					}
					if (fabs(pt_3d_blk.distanceTo(pt_3d_base))<3+dOffset)
					{
						nArrayHandleFind.Add(nHandleCur) ;
						iRet = 13 ;
					}
				}
				else if(strBlkName.Left(7)==_T("DT_YDQ_"))
				{
					if (fabs(pt_3d_blk.distanceTo(pt_3d_base))<3+dOffset)
					{
						nArrayHandleFind.Add(nHandleCur) ;
						iRet = 14 ;
					}
				}
				else if(strBlkName.Left(6)==_T("DT_JZ_"))
				{
					if (fabs(pt_3d_blk.distanceTo(pt_3d_base))<2.5+dOffset)
					{
						nArrayHandleFind.Add(nHandleCur) ;
						iRet = 15 ;
					}
				}
				else if(strBlkName.Left(7)==_T("DT_DHX_"))
				{
					if (fabs(pt_3d_blk.distanceTo(pt_3d_base))<2.5+dOffset)
					{
						nArrayHandleFind.Add(nHandleCur) ;
						iRet = 16 ;
					}
				}
				else if(strBlkName.Left(6)==_T("DT_XHJ"))
				{
					//找到HZ的中心
					if(fabs(d_rotation)<dOffset)
					{
						pt_3d_blk.y+=6;
					}
					else if(fabs(d_rotation-PI)<dOffset)
					{
						pt_3d_blk.y-=6;
					}
					if (fabs(pt_3d_blk.distanceTo(pt_3d_base))<3+dOffset)
					{
						nArrayHandleFind.Add(nHandleCur) ;
						iRet = 17 ;
					}
				}
				else if(strBlkName.Left(15)==_T("DT_BUTTON_JJGB_"))
				{
					if (fabs(pt_3d_blk.distanceTo(pt_3d_base))<3+dOffset)
					{
						nArrayHandleFind.Add(nHandleCur) ;
						iRet = 18 ;
					}

				}
				else if(strBlkName.Left(9)==_T("DT_FCJSQ_"))
				{
					if (fabs(pt_3d_blk.distanceTo(pt_3d_base))<3+dOffset)
					{
						nArrayHandleFind.Add(nHandleCur) ;
						iRet = 19 ;
					}

				}
				else if(strBlkName.Left(7)==_T("DT_ZZJ_"))
				{
					//找到HZ的中心
					if(fabs(d_rotation)<dOffset)
					{
						pt_3d_blk.y+=20.5;
					}
					else if(fabs(d_rotation-PI)<dOffset)
					{
						pt_3d_blk.y-=20.5;
					}
					if (fabs(pt_3d_blk.distanceTo(pt_3d_base))<3+dOffset)
					{
						nArrayHandleFind.Add(nHandleCur) ;
						iRet = 20 ;
					}
				}
			}
			else if (pEnt->isKindOf(AcDbPolyline::desc()))
			{
				long iIsXhlCable = 0 ;
				if (mysys.GetXdata(pEnt, _T("IS_XHLCABLE"), iIsXhlCable)&&iIsXhlCable>0)
				{
					AcDbPolyline* pPolyLine = AcDbPolyline::cast(pEnt) ;
					UINT nVertIndex = 0 ;					 
					if (mysys.IsPointOnPolyLine(pPolyLine, pt_3d_base, nVertIndex, dOffset)>-1)
					{
						nArrayHandleFind.Add(nHandleCur) ;
						nArrayHandleXhlCable.Add(nHandleCur) ;
						iRet = 1 ;
					}
				}
			}

			pEnt->close() ;
		}	
	}
	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;

	//int iNumXhl = nArrayHandleXhl.GetCount() ;
	int iNumXhlCable = nArrayHandleXhlCable.GetCount() ;
	// 	if (iNumXhl==1&&iNumXhlCable>0)
	// 	{
	// 		nArrayHandleFind.RemoveAll() ;
	// 		nArrayHandleFind.Add(nArrayHandleXhl.GetAt(0)) ;
	// 		iRet = 1 ;
	// 	}
	if (iNumXhlCable>0)
	{
		nArrayHandleFind.RemoveAll() ;
		nArrayHandleFind.Add(nArrayHandleXhlCable.GetAt(0)) ;
		iRet = 1 ;
	}
	int iNumFind = nArrayHandleFind.GetCount() ;
	if (iNumFind>1)
	{
		acutPrintf(_T("\n在点(%.3f, %.3f)处发现多于1个盒子，请注意"), pt_3d_base.x, pt_3d_base.y) ;
	}

	return iRet ;
}
BOOL CZnUtility::IsPointOnRect(const AcGePoint2d& pt_2d_base, const AcGePoint2d& pt_2d_cen, double dHeight, double dWidth, double dOffset)
{
	for (int i=0; i<4; i++)
	{
		AcGePoint2d pt_2d_tmp1 ;
		pt_2d_tmp1.x = pt_2d_cen.x+(i%3==0?-1:1)*dWidth/2 ;
		pt_2d_tmp1.y = pt_2d_cen.y+(i/2==0?1:-1)*dHeight/2 ;

		int j = (i==0?(i+3):(i-1)) ;
		AcGePoint2d pt_2d_tmp2 ;
		pt_2d_tmp2.x = pt_2d_cen.x+(j%3==0?-1:1)*dWidth/2 ;
		pt_2d_tmp2.y = pt_2d_cen.y+(j/2==0?1:-1)*dHeight/2 ;
		AcGeLineSeg2d lineseg2d(pt_2d_tmp2, pt_2d_tmp1) ;
		AcGeTol tol ;
		tol.setEqualPoint(dOffset) ;
		if (lineseg2d.isOn(pt_2d_base, tol))
		{
			return TRUE ;
		}
	}
	
	return FALSE ;
}

// 取得多段线顶点数组
int CZnUtility::GetVertex(ads_name en, AcGePoint2dArray& pt2dArrayVertex)
{
	int iRet = 0 ;
	CUserSystem mysys ;
	AcDbEntity *pEnt = NULL ;
	if(mysys.OpenAcDbEntity(en, pEnt, AcDb::kForRead))
	{
		if (pEnt->isKindOf(AcDbPolyline::desc()))
		{
			AcDbPolyline* pPolyLine = AcDbPolyline::cast(pEnt) ;
			iRet = GetVertex(pPolyLine, pt2dArrayVertex) ;
		}
		pEnt->close() ;
	}

	return iRet;
}

// 取得多段线顶点数组
int CZnUtility::GetVertex(const AcDbPolyline* pPolyLine, AcGePoint2dArray& pt2dArrayVertex)
{
	int iRet = pPolyLine->numVerts() ;
	for (int i=0; i<iRet; i++)
	{
		AcGePoint2d pt_2d_tmp ;
		pPolyLine->getPointAt(i, pt_2d_tmp) ;
		pt2dArrayVertex.append(pt_2d_tmp) ;
	}

	return iRet ;
}

// 实体所属类型,转辙机:0,信号机:1,轨道电路:2
void CZnUtility::GetEntBelongType(long nHandleBelong, int& iType)
{
	CUserSystem mysys ;
	AcDbEntity* pEnt = NULL ;
	if (mysys.OpenAcDbEntity(nHandleBelong, pEnt, AcDb::kForRead))
	{
		if (pEnt->isKindOf(AcDbBlockReference::desc()))
		{
			AcDbBlockReference* pRef = AcDbBlockReference::cast(pEnt) ;
			CString strBlkName ;
			mysys.GetBlockName(pRef, strBlkName) ;
			if (strBlkName.GetLength()>3)
			{
				CString strBlkPart = strBlkName.Left(3) ;
				if (strBlkPart==_T("XHJ"))
				{
					iType = 1 ;
				}
				else if (strBlkPart==_T("ZZJ"))
				{
					iType = 0 ;
				}
				else if (strBlkPart==_T("JYJ"))
				{
					iType = 2 ;
				}
			}
		}
		pEnt->close() ;
	}
	if (iType==-1)
	{
		long nHandleTmp = 0 ;
		if(mysys.GetXdata(nHandleBelong, _T("BELONG_ENT"), nHandleTmp) &&nHandleTmp>0)
		{
			GetEntBelongType(nHandleTmp, iType) ;
		}
	}
}

// 返回指定的实体（信号机、道岔、轨道电路）
int CZnUtility::GetEnt(CLongArray& nArrayHandle, int iType)
{
// 	int nRet = 0 ;
// 
// 	CUserSystem mysys ;
// 
// 	struct resbuf* pRb ;
// 	int nLen = 0 ;
// 	CLongArray nArrayHandle ;
// 	//pRb = acutBuildList(RTDXF0,_T("LWPOLYLINE"), 1001, _T("CloneFrom"), 1070, nCloneFromHandle, 1001, _T("UPDOWN"), 1070, iUpOrDwon, 0) ;
// 	pRb = acutBuildList(RTDXF0,_T("LWPOLYLINE"), 1001, _T("ISGUIDAO"),  0) ;
// 	nLen = mysys.GetSSEntHandle(_T("X"), NULL, NULL, pRb, nArrayHandle) ;
// 	acutRelRb(pRb) ;
// 	
// 	for (int i=0; i<nLen; i++)
// 	{
// 		AcDbEntity* pEnt = NULL ;
// 		long nTemp = nArrayHandle.GetAt(i) ;
// 		if (mysys.OpenAcDbEntity(nTemp, pEnt, AcDb::kForRead))
// 		{		
// 			int iUPDOWN = 0 ;
// 			mysys.GetXdata(pEnt, _T("CloneFrom"), nCloneFrom) ;
// 			mysys.GetXdata(pEnt, _T("UPDOWN"), iUPDOWN) ;
// 
// 			pEnt->close() ;
// 		}
// 	}

	return 0;
}

int CZnUtility::GetJltType(const CString& strNameIntoXhl)
{
	int iTypeDljl = -1 ;
	if (strNameIntoXhl==_T("DC"))
	{
		iTypeDljl = 0 ;
	}
	else if (strNameIntoXhl==_T("XH"))
	{
		iTypeDljl = 1 ;
	}
	else if (strNameIntoXhl==_T("GD"))
	{
		iTypeDljl = 2 ;
	}
	else if (strNameIntoXhl==_T("DM"))
	{
		iTypeDljl = 3 ;
	}
	else
	{
		//acutPrintf(_T("\n连到主电缆的电缆名称 %s 不是以[DC,XH,GD,DM]开头，请规范名称！"), strNameIntoXhl) ;
	}
	return  iTypeDljl ;
}


// 取得电缆参数
int CZnUtility::GetCableParam(double& dCoeff, int& iIntoXhl, int& iPerGd, int& iPerBetweenGd, int& iPerBox, int& iPerBoxF, int& iPerBetweenBoxF)
{
	double d1 = 1.05 ;
	if (CBlkUtility::QueryValueReg(_T("ZnOption"), _T("CableCoeff"), d1) )
	{
		dCoeff = d1 ;
	}
	CString strAppNames[6] = {_T("CableIntoXhl"), _T("CablePerGd"), _T("CablePerBetweenGd"),  _T("CablePerBox"), _T("CablePerBoxF"), _T("CablePerBetweenBoxF")} ;
	int iData[6] = {50, 10, 10, 4, 4, 500} ;
	for (int i=0; i<6; i++)
	{
		if (CBlkUtility::QueryValueReg(_T("ZnOption"), strAppNames[i], iData[i]))
		{
			switch (i)
			{
			case 0:
				iIntoXhl = iData[i] ;
				break;
			case 1:
				iPerGd = iData[i] ;
				break;
			case 2:
				iPerBetweenGd = iData[i] ;
				break;
			case 3:
				iPerBox = iData[i] ;
				break;
			case 4:
				iPerBoxF = iData[i] ;
				break;
			case 5:
				iPerBetweenBoxF = iData[i] ;
				break;
			default:
				break;
			}
		}
	}
	return 0;
}
// 取得电缆参数
int CZnUtility::DT_GetCableParam(double& dWanquRatio, int& iZuoTou, double& dGuoGuiPer, double& dRFMPer, double& dAZPer, double& dJXPer,double& dSXJiaPer, int& iDistJX)
{
	int  i1 = 1.1 ;
	if (CBlkUtility::QueryValueReg(_T("DtOption"), _T("ZuoTou"), i1))
	{
		iZuoTou = i1 ;
	}
	int i2=500;
	if (CBlkUtility::QueryValueReg(_T("DtOption"), _T("DistJieX"), i2))
	{
		iDistJX = i2 ;
	}
	CString strAppNames[6] = {_T("WanquRatio"), _T("GuoGuiPer"), _T("RFMPer"),  _T("AZPer"), _T("JXPer"), _T("SXJiaPer")} ;
	double  dData[6] = {1.1, 6, 10, 5, 8, 3.5} ;
	for (int i=0; i<6; i++)
	{
		if (CBlkUtility::QueryValueReg(_T("DtOption"), strAppNames[i], dData[i]))
		{
			switch (i)
			{
			case 0:
				dWanquRatio = dData[i] ;
				break;
			case 1:
				dGuoGuiPer = dData[i] ;
				break;
			case 2:
				dRFMPer = dData[i] ;
				break;
			case 3:
				dAZPer = dData[i] ;
				break;
			case 4:
				dJXPer = dData[i] ;
				break;
			case 5:
				dSXJiaPer = dData[i] ;
				break;
			default:
				break;
			}
		}
	}
	return 0;
}


// 返回对象的类类型
// CString CZnUtility::GetClassType(CBoxNodeBase* pObject)
// {
// 	const char *szClassName = typeid(*pObject).name() ;
// 	CString strClassName ;
// // 	CBlkUtility::ConvMultiToWide(szClassName, strClassName) ;
// 	strClassName.TrimLeft(_T("class ")) ;
// 	return strClassName ;
// }

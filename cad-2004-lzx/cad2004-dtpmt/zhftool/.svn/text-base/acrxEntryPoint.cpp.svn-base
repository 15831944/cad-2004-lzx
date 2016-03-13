// (C) Copyright 1990-2002 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- acrxEntryPoint.h
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "ZhfPalette.h"
#include "ZhfPaletteSet.h"
#include "ZhfBlkRefJig.h"
#include "UserTools.h"
#include "PmtSetRbDlg.h"
#include "..\tinyxml\tinyxml.h"
#include "ArxDbgUiDlgSplash.h"

//-----------------------------------------------------------------------------
#define szRDS _RXST("zhf")

//Global
bool g_bIsTukuFirst = true;
//bool g_bAddJyjWhenInsertXhj = true ;
CMDIFrameWnd* g_pAcadFrame = NULL ;
CGsPreviewCtrl* g_pCtrlGsPreview = NULL ;
CArray<CZhfPaletteSet*, CZhfPaletteSet*&> g_pArrayZhfPaletteSet ;
bool g_bIsToolBarLock = false ;
bool bWatchSelectDone = false ;

HINSTANCE dllInstance() ;

void WatchSelect(const MSG * pMsg) ;

void WatchSelect(const MSG * pMsg)
{

	if (pMsg->message==WM_LBUTTONUP)
	{
		//acutPrintf(_T("\nin zhftool: you press left mouse button:%d-%d"), pMsg->pt.x, pMsg->pt.y) ;
	}
	else if (pMsg->message == WM_KEYDOWN &&(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_CANCEL))
	{
		//acutPrintf(_T("\nin zhftool: you press esc")) ;

		if (g_pCtrlGsPreview!=NULL)
		{
			g_pCtrlGsPreview->SetSelect(false) ;
			CZhfPalette* pPalette = (CZhfPalette*)g_pCtrlGsPreview->GetParent() ;
			pPalette->m_iCurSel = -1 ;
			g_pCtrlGsPreview = NULL ;
			g_bIsToolBarLock = false ;
		}
	}
}

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CzhftoolApp : public AcRxArxApp {

public:
	CzhftoolApp () : AcRxArxApp () {}

	static void InitApplication(void);
	static void UnloadApplication(void);

 	CWnd* startSplashScreen(void);
 	void endSplashScreen(CWnd* splashScreen);
// 	HINSTANCE dllInstance() const;


	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		// TODO: Add your initialization code here
		if (!InitSystem())
		{
			return AcRx::kRetError ;
		}
		CWnd* splashScreen = startSplashScreen();

		InitApplication() ;

		//set titile, icon, in AutoCAD2009及以上，因为采用了ribbon菜单，下面语句不起作用，其左上角图标不变
		CWnd* pWnd = AfxGetMainWnd() ;  //也可用下面语句
		//CWnd* pWnd = CWnd::FromHandle( adsw_acadMainWnd() ); 
		pWnd->SetWindowText(_T("铁路工程设计软件"));
 //		pWnd->SetIcon(::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON3)), FALSE) ; // not work
// 		pWnd->SetIcon(::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON2)), TRUE) ; //not work

		HICON hIconSmall = (HICON)::LoadImage(_hdllInstance, MAKEINTRESOURCE(IDI_ICON_X_16), IMAGE_ICON, 16, 16, 0);
		HICON hIconBig = (HICON)::LoadImage(_hdllInstance, MAKEINTRESOURCE(IDI_ICON_TRAIN_48), IMAGE_ICON, 48, 48, 0);
		pWnd->SetIcon(hIconSmall, FALSE); //程序打开左上角和任务栏图标
		pWnd->SetIcon(hIconBig, TRUE);  //按下ALT+TAB键时显示图标

		if (!bWatchSelectDone)
		{
			if (acedRegisterWatchWinMsg(WatchSelect)==FALSE)
			{
				acedPrompt(_T("\nhook can't be registered.")) ;
			}
			else 
			{
				//acedPrompt(_T("hook been registered!\n"));  
				bWatchSelectDone = TRUE;
			}
		}

		endSplashScreen(splashScreen);
		acutPrintf(_T("\n************************************************************")) ;
		acutPrintf(_T("\n\t\t铁路工程设计软件\n\t\t通信信号设计院. 系统所.\n\t\t丰台科技园.通号产业园B626.\n\t\t周海锋\n\t\tEmail:zhfeng@crscd.com.cn\n\t\tTel:18001022281 ")) ;
		acutPrintf(_T("\n************************************************************\n")) ;
		//acutPrintf(_T("\033[31mwrong\033[0m\n"));
		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;
		// TODO: Add your code here
		UnloadApplication() ;

		if(bWatchSelectDone == TRUE)
		{
			acedRemoveWatchWinMsg(WatchSelect);
			acutPrintf(_T("\nHook has been removed!\n"));
		}

		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
		
	}


	// ----- zhfzhftool._drawblk command (do not rename)
	static void zhfzhftool_drawblk(void)
	{
		// Add your code for command zhfzhftool._drawblk here
		if (!g_bIsTukuFirst)
		{
			if (g_pCtrlGsPreview!=NULL)
			{
				g_bIsToolBarLock = true ;
				CZhfPalette* pPalette = (CZhfPalette*)g_pCtrlGsPreview->GetParent() ;

				CZhfBlkRefJig* pJig = new CZhfBlkRefJig(g_pCtrlGsPreview) ;
				pJig->DoIt() ;
				CString strBlkName ;
				AcDbObjectId objId = pJig->m_objId ;
				CBlkUtility::GetFileName(pJig->m_strBlkFile, strBlkName) ;
				delete pJig ;
				//若是符合块，如道岔组合块，需要炸开
				if (strBlkName.GetLength()==6)
				{
					if (strBlkName.Left(2)==_T("DC")&&strBlkName!=_T("DC_000"))
					{
						AcDbEntity* pEnt = NULL ;
						CUserSystem mysys ;
						if (mysys.OpenAcDbEntity(objId, pEnt) )
						{
							mysys.Explode(pEnt) ;

							pEnt->close() ;
						}
					}
				}
				g_bIsToolBarLock = false ;
				if (g_pCtrlGsPreview!=NULL) //有可能按了ESC键，导致gsctrl已经置null
				{
					g_pCtrlGsPreview->SetSelect(false) ;
					pPalette->m_iCurSel = -1 ;
				}
			}
		}
		else
		{
			acutPrintf(_T("\n没有初始化图库，请先运行tuku或dttk")) ;
		}
	}

	// ----- zhfzhftool._tuku command (do not rename)
	static void zhfzhftool_tuku(void)
	{
		// Add your code for command zhfzhftool._tuku here
		if (g_bIsTukuFirst)
		{
			InitApplication() ;
		}
		else
		{
			CMDIFrameWnd* pAcadFrame  = acedGetAcadFrame();

			if (g_pArrayZhfPaletteSet.GetCount()>0)
			{
				CZhfPaletteSet* pSet = g_pArrayZhfPaletteSet.GetAt(0) ;
				if (pSet->IsWindowVisible())
				{
					pAcadFrame->ShowControlBar(pSet, FALSE, FALSE);
				}
				else
				{
					pAcadFrame->ShowControlBar(pSet, TRUE, FALSE);
				}	
			}
			else
			{
				acutPrintf(_T("\n图库面板没有创建或创建失败，无法显示！")) ;
			}
		}
	}

	// ----- zhfzhftool._zhftools command (do not rename)
	static void zhfzhftool_zhftools(void)
	{
		// Add your code for command zhfzhftool._zhftools here
		CUserTools::ZhfTools() ;
	}
	// -----------------------------------------------------------------------------
	virtual AcRx::AppRetCode On_kUnloadDwgMsg(void * pkt);

	// ----- zhfzhftool._zrotate command (do not rename)
	static void zhfzhftool_zrotate(void)
	{
		// Add your code for command zhfzhftool._zrotate here
		CUserTools::RotationEnt() ;
	}

	// ----- zhfzhftool._zmirror command (do not rename)
	static void zhfzhftool_zmirror(void)
	{
		// Add your code for command zhfzhftool._zmirror here
		CUserTools::MirrorEnt() ;
	}

	// ----- zhfzhftool._zadjustpl command (do not rename)
	static void zhfzhftool_zadjustpl(void)
	{
		// Add your code for command zhfzhftool._zadjustpl here
		CUserTools::AdjustPlineVertex() ;
	}

	// ----- zhfzhftool._rbset command (do not rename)
	static void zhfzhftool_rbset(void)
	{
		// Add your code for command zhfzhftool._rbset here
		CAcModuleResourceOverride myResources ;
		CPmtSetRbDlg dlg ;
		dlg.DoModal() ;
	}

	// ----- zhfzhftool._rb command (do not rename)
	static void zhfzhftool_rb(void)
	{
		// Add your code for command zhfzhftool._rb here
		CUserTools::ReplaceEnt() ;	
	}

	// ----- zhfzhftool._moblkatt command (do not rename)
	static void zhfzhftool_moblkatt(void)
	{
		// Add your code for command zhfzhftool._moblkatt here
		//批量修改块属性
		CUserTools::SetBlkAtt() ;
	}

	// ----- zhfzhftool._addvert command (do not rename)
	static void zhfzhftool_addvert(void)
	{
		// Add your code for command zhfzhftool._addvert here
		CUserTools::AddVertex() ;
	}

	// ----- zhfzhftool._reverseline command (do not rename)
	static void zhfzhftool_reverseline(void)
	{
		// Add your code for command zhfzhftool._reverseline here
		CUserTools::ReverseLinePoint() ;
	}

	// ----- zhfzhftool._ad command (do not rename)
	static void zhfzhftool_ad(void)
	{
		// Add your code for command zhfzhftool._ad here
		//编辑文本（变色功能）
		CUserTools::MyEdit() ;
	}

	// ----- zhfzhftool._utext command (do not rename)
	static void zhfzhftool_utext(void)
	{
		// Add your code for command zhfzhftool._utext here
		//输入文本
		CUserTools::MyText() ;
	}

	// ----- zhfzhftool._btext command (do not rename)
	static void zhfzhftool_btext(void)
	{
		// Add your code for command zhfzhftool._btext here
		//批量输入文本
		CUserTools::BText() ;
	}

	// ----- zhfzhftool._tt command (do not rename)
	static void zhfzhftool_tt(void)
	{
		// Add your code for command zhfzhftool._tt here
		//属性文本、多行文本转换为单行文本
		CUserTools::AtMtToTxt() ;
	}

	// ----- zhfzhftool._fr command (do not rename)
	static void zhfzhftool_fr(void)
	{
		// Add your code for command zhfzhftool._fr here
		//查找替换文本
		CUserTools::MyFind() ;
	}

	// ----- zhfzhftool._aligntext command (do not rename)
	static void zhfzhftool_aligntext(void)
	{
		// Add your code for command zhfzhftool._aligntext here
		//对齐文本
		CUserTools::AlignText() ;
	}

	// ----- zhfzhftool._motext command (do not rename)
	static void zhfzhftool_motext(void)
	{
		// Add your code for command zhfzhftool._motext here
		//批量修改文字大小、style、对齐方式
		CUserTools::MoText() ;
	}

	// ----- zhfzhftool._bedit command (do not rename)
	static void zhfzhftool_bedit(void)
	{
		// Add your code for command zhfzhftool._bedit here
		//批量修改文本
		CUserTools::BEdit() ;
	}

	// ----- zhfzhftool._delnulltext command (do not rename)
	static void zhfzhftool_delnulltext(void)
	{
		// Add your code for command zhfzhftool._delnulltext here
		//删除空文本
		CUserTools::DelNullText() ;
	}

	// ----- zhfzhftool._replaceblock command (do not rename)
	static void zhfzhftool_replaceblock(void)
	{
		// Add your code for command zhfzhftool._replaceblock here
		// 替换块
		CUserTools::ReplaceBlock() ;
	}

	// ----- zhfzhftool._dwgtoxls command (do not rename)
	static void zhfzhftool_dwgtoxls(void)
	{
		// Add your code for command zhfzhftool._dwgtoxls here
		//cad中表格转换成excel的表格
		CUserTools::DwgToXls() ;
	}

	// ----- zhfzhftool._xlstodwg command (do not rename)
	static void zhfzhftool_xlstodwg(void)
	{
		// Add your code for command zhfzhftool._xlstodwg here
		// excel表格转换成cad中的表格
		CUserTools::XlsToDwg() ;
	}

	// ----- zhfzhftool._gethw command (do not rename)
	static void zhfzhftool_gethw(void)
	{
		// Add your code for command zhfzhftool._gethw here
		//在将excel转换成cad表格时计算对应height和width
		CUserTools::GetHW() ;
	}

	// ----- zhfzhftool._setxdata command (do not rename)
	static void zhfzhftool_setxdata(void)
	{
		// Add your code for command zhfzhftool._setxdata here
		// 设置扩展数据xdata
		CUserTools::SetXdata() ;
	}

	// ----- zhfzhftool._showxdata command (do not rename)
	static void zhfzhftool_showxdata(void)
	{
		// Add your code for command zhfzhftool._showxdata here
		// 显示扩展数据xdata
		CUserTools::ShowXdata() ;
	}

	// ----- zhfzhftool._delxdata command (do not rename)
	static void zhfzhftool_delxdata(void)
	{
		// Add your code for command zhfzhftool._delxdata here
		//删除扩展数据
		CUserTools::DelXdata() ;
	}

	// ----- zhfzhftool._mofilename command (do not rename)
	static void zhfzhftool_mofilename(void)
	{
		// Add your code for command zhfzhftool._mofilename here
		//批量修改文件名
		CUserTools::MoFileName() ;
	}

	// ----- zhfzhftool._refreshdwg command (do not rename)
	static void zhfzhftool_refreshdwg(void)
	{
		// Add your code for command zhfzhftool._refreshdwg here
		//刷新图纸，主要是某些程序写入的文字没有明确指定对齐点，导致图形显示不正确，移动一下可解决。在单文档模式下有效
		CUserTools::BatchDoDwg(_T("zoom e \r \003\003")) ;
	}

	// ----- zhfzhftool._explodespe command (do not rename)
	static void zhfzhftool_explodespe(void)
	{
		// Add your code for command zhfzhftool._explodespe here
		//炸开以A$开头命名的块
		CUserTools::ExplodeSpeBlk() ;
	}

	// ----- zhfzhftool._sumlinelen command (do not rename)
	static void zhfzhftool_sumlinelen(void)
	{
		// Add your code for command zhfzhftool._sumlinelen here
		//统计选择的线的长度（所有属于AcDbCurve的线）
		CUserTools::SumLineLen() ;
	}

	// ----- zhfzhftool._sumdianlan command (do not rename)
	static void zhfzhftool_sumdianlan(void)
	{
		// Add your code for command zhfzhftool._sumdianlan here
		//统计电缆
		CUserTools::SumDianLan() ;
	}

	// ----- zhfzhftool._showblkatt command (do not rename)
	static void zhfzhftool_showblkatt(void)
	{
		// Add your code for command zhfzhftool._showblkatt here
		//显示、隐藏块属性
		CUserTools::ShowBlkAtt() ;
	}

	// ----- zhfzhftool._checksameid command (do not rename)
	static void zhfzhftool_checksameid(void)
	{
		// Add your code for command zhfzhftool._checksameid here
		CUserTools::CheckSameId() ;
	}

	// ----- zhfzhftool._unchecksameid command (do not rename)
	static void zhfzhftool_unchecksameid(void)
	{
		// Add your code for command zhfzhftool._unchecksameid here
		CUserTools::UnCheckSameId() ;
	}

	// ----- zhfzhftool._hidexdata command (do not rename)
	static void zhfzhftool_hidexdata(void)
	{
		// Add your code for command zhfzhftool._hidexdata here
		CUserTools::HideXdata() ;
	}

	// ----- zhfzhftool._deloneapp command (do not rename)
	static void zhfzhftool_deloneapp(void)
	{
		// Add your code for command zhfzhftool._deloneapp here
		//删除指定app
		CUserTools::DelOneApp() ;
	}

	// ----- zhfzhftool._addnum command (do not rename)
	static void zhfzhftool_addnum(void)
	{
		// Add your code for command zhfzhftool._addnum here
		CUserTools::AddNum() ;
	}

	// ----- zhfzhftool._repappval command (do not rename)
	static void zhfzhftool_repappval(void)
	{
		// Add your code for command zhfzhftool._repappval here
		CUserTools::ReplaceAppValue() ;
	}

	// ----- zhfzhftool._savecircuit command (do not rename)
	static void zhfzhftool_savecircuit(void)
	{
		// Add your code for command zhfzhftool._savecircuit here
		CUserSystem mysys ;
		mysys.WriteToRec("second_draw", "draw_type", "CIRCUIT") ;
	}

	// ----- zhfzhftool._zlist command (do not rename)
	static void zhfzhftool_zlist(void)
	{
		// Add your code for command zhfzhftool._zlist here
		// 显示实体Handle
		CUserTools::ZList() ;
	}

	// ----- zhfzhftool._topline command (do not rename)
	static void zhfzhftool_topline(void)
	{
		// Add your code for command zhfzhftool._topline here
		CUserTools::ConvertToPolyline() ;
	}

	// ----- zhfzhftool._joinline command (do not rename)
	static void zhfzhftool_joinline(void)
	{
		// Add your code for command zhfzhftool._joinline here
		CUserTools::JoinLine() ;
	}

	// ----- zhfzhftool._addatt command (do not rename)
	static void zhfzhftool_addatt(void)
	{
		// Add your code for command zhfzhftool._addatt here
		CUserTools::AddAtt() ;
	}

	// ----- zhfzhftool._dttk command (do not rename)
	static void zhfzhftool_dttk(void)
	{
		// Add your code for command zhfzhftool._dttk here
		if (g_bIsTukuFirst)
		{
			InitApplication() ;
		}
		else
		{
			CMDIFrameWnd* pAcadFrame  = acedGetAcadFrame();

			if (g_pArrayZhfPaletteSet.GetCount()>1)
			{
				CZhfPaletteSet* pSet = g_pArrayZhfPaletteSet.GetAt(1) ;
				if (pSet->IsWindowVisible())
				{
					pAcadFrame->ShowControlBar(pSet, FALSE, FALSE);
				}
				else
				{
					pAcadFrame->ShowControlBar(pSet, TRUE, FALSE);
				}	
			}
			else
			{
				acutPrintf(_T("\n图库面板没有创建或创建失败，无法显示！")) ;
			}
		}
	}

	// ----- zhfzhftool._setplwidth command (do not rename)
	static void zhfzhftool_setplwidth(void)
	{
		// Add your code for command zhfzhftool._setplwidth here
		CUserTools::SetPlineWidth() ;
	}

	// ----- zhfzhftool._rptk command (do not rename)
	static void zhfzhftool_rptk(void)
	{
		// Add your code for command zhfzhftool._rptk here
		CUserTools::ReplaceTUKU() ;
	}
	// ----- zhfzhftool._xlstolsb command (do not rename)
	static void zhfzhftool_xlstolsb(void)
	{
		// Add your code for command zhfzhftool._xlstolsb here
		//将xls转换成联锁表格
		// excel表格转换成cad中的表格
		CUserTools::XlsToLsb();
	}

	// ----- zhfzhftool._batchdodwg command (do not rename)
	static void zhfzhftool_batchdodwg(void)
	{
		// Add your code for command zhfzhftool._batchdodwg here
		TCHAR chCommand[100] ;
		if (acedGetString(1, _T("\n*****对指定的文件夹下的dwg文件执行命令！*****\n输入要执行的命令后回车确定："), chCommand)==RTNORM)
		{
			CString strCommand ;
			strCommand.Format(_T("%s"), chCommand) ;
			CUserTools::BatchDoDwg(strCommand) ;
		}
	}

	// ----- zhfzhftool._test2 command (do not rename)
	static void zhfzhftool_test2(void)
	{
		// Add your code for command zhfzhftool._test2 here
		CUserSystem mysys ;
		struct resbuf *pRb ;
		pRb = acutBuildList(RTDXF0, _T("INSERT"), 0) ;
		ads_name ss_temp ;
		int iRet = acedSSGet(_T("_I"), NULL, NULL, pRb, ss_temp) ;

		if (iRet== RTNORM)
		{
			long nLen = 0 ;
			acedSSLength(ss_temp, &nLen) ;
			for (int i=0; i<nLen; i++)
			{
				ads_name temp_name ;
				acedSSName(ss_temp, i, temp_name) ;
				CString strBlkName ;
				mysys.GetBlockName(temp_name, strBlkName) ;
				acutPrintf(_T("\n%d:%s"), i, strBlkName) ;
			}
			acedSSFree(ss_temp) ;
		}
		acutRelRb(pRb) ;

	}

} ;


void CzhftoolApp::InitApplication(void)
{
	CMDIFrameWnd* pAcadFrame  = acedGetAcadFrame();

	CString strFileName  ;
	strFileName.Format(_T("%s\\zhfcad.xml"), g_strBasePath) ;
	CStringA strFileNameA ;
#ifdef UNICODE
	CBlkUtility::ConvWideToMulti(strFileName, strFileNameA) ;
#else
    strFileNameA = strFileName ;
#endif

	TiXmlDocument doc(strFileNameA.GetBuffer(0)) ;
	bool bLoadOk = doc.LoadFile() ;
	if (bLoadOk)
	{
		TiXmlHandle docHandle(&doc) ;
		TiXmlElement* elPaletteSet = docHandle.FirstChild("ZhfCAD").FirstChild("PaletteSet").ToElement() ;
		for (; elPaletteSet!=NULL; elPaletteSet=elPaletteSet->NextSiblingElement())
		{
			CString strPaletteSetName ;
			const char* strPaletteSetNameA = elPaletteSet->Attribute("Name") ;
			CBlkUtility::ConvMultiToWide(strPaletteSetNameA, strPaletteSetName) ;
			CZhfPaletteSet* pSet = new CZhfPaletteSet ;
			CRect rect(0, 0, 320, 300);
			pSet->Create(strPaletteSetName, WS_OVERLAPPED|WS_DLGFRAME, rect, pAcadFrame,	PSS_EDIT_NAME | PSS_PROPERTIES_MENU | PSS_AUTO_ROLLUP |	PSS_CLOSE_BUTTON) ;

			TiXmlElement* elPalette = elPaletteSet->FirstChildElement() ;
			for (;elPalette!=NULL;elPalette=elPalette->NextSiblingElement())
			{
				int iPaletteShowAtt = 0 ;
				elPalette->Attribute("DefaultShowAttribute", &iPaletteShowAtt) ;
				CZhfPalette* pZhfPalette = new CZhfPalette() ;
				pZhfPalette->m_iFilterMode = iPaletteShowAtt ;

				TiXmlElement* elFile = elPalette->FirstChildElement() ;
				for (;elFile!=NULL;elFile=elFile->NextSiblingElement())
				{
					CString strFilePath, strShowName ;
					const char* strFilePathA = elFile->Attribute("FullPath") ;
					const char* strShowNameA = elFile->Attribute("ShowName") ;
					CBlkUtility::ConvMultiToWide(strFilePathA, strFilePath) ;
					CBlkUtility::ConvMultiToWide(strShowNameA, strShowName) ;
					CString strFilePathTmp ;
					strFilePathTmp.Format(_T("%s\\support\\%s"), g_strBasePath, strFilePath) ;					
					pZhfPalette->m_strArrayFile.Add(strFilePathTmp) ;
					pZhfPalette->m_strArrayFileName.Add(strShowName) ;
					int iFilterMode = 0 ;
					elFile->Attribute("ShowAttribute", &iFilterMode) ;
					pZhfPalette->m_nArrayFilterMode.Add(abs(iFilterMode)) ;
				}

				pZhfPalette->m_iCtrlWidth=80 ;
				pZhfPalette->m_iCtrlHeight = 80 ;
				CString strPaletteName ;
				const char* strPaletteNameA = elPalette->Attribute("Name") ;
				CBlkUtility::ConvMultiToWide(strPaletteNameA, strPaletteName) ;
				pZhfPalette->Create(WS_CHILD | WS_VISIBLE|WS_VSCROLL,strPaletteName, pSet, PS_EDIT_NAME);
				pSet->AddPalette(pZhfPalette) ;
			}

			pSet->EnableDocking(CBRS_ALIGN_ANY);
			pSet->RestoreControlBar();

			pAcadFrame->ShowControlBar(pSet, FALSE, FALSE);	
			g_pArrayZhfPaletteSet.Add(pSet) ;

		}
	}

	g_bIsTukuFirst = false ;
	//插入信号机时是否加绝缘节
// 	DWORD iAddJyj = 1 ;
// 	CBlkUtility::QueryValueReg(_T("ZHFTOOLS"), _T("AddJyjWhenInsertXhj"), iAddJyj) ;
// 	g_bAddJyjWhenInsertXhj = (iAddJyj==1?true:false) ;
}




void CzhftoolApp::UnloadApplication(void)
{
	for (int i=0; i<g_pArrayZhfPaletteSet.GetCount(); i++)
	{
		CZhfPaletteSet* pSet = g_pArrayZhfPaletteSet.GetAt(i) ;
		pSet->DestroyWindow() ;
		delete pSet ;
	}
}
AcRx::AppRetCode CzhftoolApp::On_kUnloadDwgMsg(void * pkt)
{
	CMDIFrameWnd* pAcadFrame  = acedGetAcadFrame();
	AcApDocManager *pDocMgr = acDocManager ;
	int iDocCount = pDocMgr->documentCount() ;
	if (iDocCount==1)
	{
		for (int i=0; i<g_pArrayZhfPaletteSet.GetCount(); i++)
		{
			CZhfPaletteSet* pSet = g_pArrayZhfPaletteSet.GetAt(i) ;
			if (pSet->IsWindowVisible())
			{
				pAcadFrame->ShowControlBar(pSet, FALSE, FALSE);
			}
		}
	}	

	AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadDwgMsg (pkt) ;
	return (retCode) ;
}

CWnd* CzhftoolApp::startSplashScreen(void)
{
	CString bmpFileName = _T("splash.bmp"); 	   // TBD: make this better later!

	// make sure file is there (either in a main directory, which is
	// where someone would install it, or for developers, go out of 
	// "Debug" directory and up to the main directory.
	CFileStatus status;
	BOOL success = CFile::GetStatus(bmpFileName, status);
	if (success == FALSE) 
	{
		bmpFileName.Format(_T("%s\\support\\pic\\splash.bmp"), g_strBasePath); 
		success = CFile::GetStatus(bmpFileName, status);
	}

	if (success)
	{
		ArxDbgUiDlgSplash* dbox = new ArxDbgUiDlgSplash(bmpFileName);
		dbox->setTextStrings(_T("铁路工程设计软件"), _T("1.0"),  _T(""), _T(""));

		// TBD: have to do this manually right now, although it should
		// have worked to pass up resource handle through the constructor!
		//acDocManager->pushResourceHandle(dllInstance());
		dbox->Create(acedGetAcadDwgView());
		acDocManager->popResourceHandle();

		dbox->ShowWindow(SW_SHOW);
		dbox->UpdateWindow();

		return dbox;
	}
	else 
	{        // intended a splash screen but could not find it!
		acutPrintf(_T("\nCould not find splash box image: \"%s\""), bmpFileName);
		return NULL;
	}
}
void CzhftoolApp::endSplashScreen(CWnd* splashScreen)
{
	if (splashScreen) {
		Sleep(1000);    // waste a little bit of time to ensure that they see it
		splashScreen->ShowWindow(SW_HIDE);
		splashScreen->UpdateWindow();
		splashScreen->DestroyWindow();
		delete splashScreen;
		acedGetAcadDwgView()->Invalidate();
		acedGetAcadDwgView()->UpdateWindow();
	}
}



//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CzhftoolApp)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _drawblk, drawblk, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _tuku, tuku, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _zhftools, zhftools, ACRX_CMD_TRANSPARENT, NULL)
// -----------------------------------------------------------------------------

ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _zrotate, zr, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _zmirror, zm, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _zadjustpl, zadpl, ACRX_CMD_TRANSPARENT, NULL)

ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _rbset, rbset, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _rb, rb, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _moblkatt, moblkatt, ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET | ACRX_CMD_REDRAW, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _addvert, addvert, ACRX_CMD_TRANSPARENT, NULL)

ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _reverseline, reverseline, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _ad, ad, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _utext, utext, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _btext, btext, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _tt, tt, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _fr, fr, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _aligntext, aligntext, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _motext, motext, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _bedit, bedit, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _delnulltext, delnulltext, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _replaceblock, replaceblock, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _dwgtoxls, dwgtoxls, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _xlstodwg, xlstodwg, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _gethw, gethw, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _setxdata, setxdata, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _showxdata, showxdata, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _delxdata, delxdata, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _mofilename, mofilename, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _refreshdwg, refreshdwg, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _explodespe, explodespe, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _sumlinelen, sumlinelen, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _sumdianlan, sumdianlan, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _showblkatt, showblkatt, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _checksameid, checksameid, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _unchecksameid, unchecksameid, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _hidexdata, hidexdata, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _deloneapp, deloneapp, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _addnum, addnum, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _repappval, repappval, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _savecircuit, savecircuit, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _zlist, zlist, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _topline, topline, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _joinline, joinline, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _xlstolsb, xlstolsb, ACRX_CMD_TRANSPARENT, NULL)
//ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _addatt, addatt, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _addatt, addatt, ACRX_CMD_TRANSPARENT|ACRX_CMD_USEPICKSET|ACRX_CMD_REDRAW, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _dttk, dttk, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _setplwidth, setplwidth, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _rptk, rptk, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _batchdodwg, batchdodwg, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzhftoolApp, zhfzhftool, _test2, test2, ACRX_CMD_TRANSPARENT, NULL)


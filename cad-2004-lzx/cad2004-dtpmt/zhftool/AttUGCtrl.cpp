// AttUGCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "AttUGCtrl.h"

// CAttUGCtrl

IMPLEMENT_DYNAMIC(CAttUGCtrl, CUGCtrl)
CAttUGCtrl::CAttUGCtrl()
: m_strCellPreText(_T(""))
{
	for (int i=0; i<30; i++)
	{
		m_iItemIsNull[i] = 0 ;
	}
}

CAttUGCtrl::~CAttUGCtrl()
{
	UGXPThemes::CleanUp();
        
}


BEGIN_MESSAGE_MAP(CAttUGCtrl, CUGCtrl)
END_MESSAGE_MAP()



// CAttUGCtrl 消息处理程序

/////////////////////////////////////////////////////////////////////////////
//	OnSetup
//		This function is called just after the grid window 
//		is created or attached to a dialog item.
//		It can be used to initially setup the grid
void CAttUGCtrl::OnSetup()
{
	//setup the fonts
	m_defFont.CreateFont( 14, 0, 0, 0, 500, 0, 0, 0, 0, 0, 0, 0, 0, _T( "Arial" ) );
	m_boldFont.CreateFont( 14, 0, 0, 0, 900, 0, 0, 0, 0, 0, 0, 0, 0, _T( "MS Sans Serif" ) );
	m_smFont.CreateFont( 10, 0, 0, 0, 500, 0, 0, 0, 0, 0, 0, 0, 0, _T( "Arial" ) );	
	m_medFont.CreateFont( 16, 0, 0, 0, 500, 0, 0, 0, 0, 0, 0, 0, 0, _T( "MS Sans Serif" ) );
	m_medBoldFont.CreateFont( 16, 0, 0, 0, 900, 0, 0, 0, 0, 0, 0, 0, 0, _T( "MS Sans Serif" ) );
	m_lrgFont.CreateFont( 20, 0, 0, 0, 600, 0, 0, 0, 0, 0, 0, 0, 0, _T( "Times New Roman" ) );	
	SetDefFont( &m_defFont );

	int nCols = m_vecStringHead.size() ;
	int nRows = m_vecStringCell.size() ;

	int nRowsTmp = nRows ;
	if (nRows<11)
	{
		nRowsTmp = 10 ;
	}

	SetDefColWidth( 50 );
	SetSH_Width(0) ;
	SetTH_Height(18) ;
	//EnableMenu( TRUE );
	SetMultiSelectMode( TRUE );
	SetDoubleBufferMode( TRUE );

	// 	SetUserSizingMode( 2 );
	// 	SetVScrollMode( UG_SCROLLTRACKING );
	// 	SetHScrollMode( UG_SCROLLTRACKING );

	CUGCell cell;
	// setup the default grid look
	GetGridDefault( &cell );
	cell.SetFont( GetFont( AddFont( _T("Arial"), -12, 400 )));
	//cell.SetBackColor(RGB(238,237,232)) ;
	cell.SetAlignment(UG_ALIGNLEFT|UG_ALIGNVCENTER) ;
	SetGridDefault( &cell );	

	SetNumberCols( nCols );
	SetNumberRows( nRowsTmp );
	//tophead
	for (int i=0; i<nCols; i++) 
	{
		GetCellIndirect( i, -1, &cell );
		cell.SetAlignment( UG_ALIGNCENTER|UG_ALIGNVCENTER );		
		cell.SetText(m_vecStringHead[i]);
		SetCell(i, -1, &cell) ;
		SetColWidth(i, 80) ;
	}


	for (int i=0; i<nRowsTmp; i++)
	{	
		for (int j=0; j<nCols; j++)
		{			
// 			GetCellIndirect( -1, i, &cell );
// 			cell.SetAlignment( UG_ALIGNLEFT|UG_ALIGNVCENTER );
// 			cell.SetText(m_vecStringCell[i][j+1]) ;
// 			SetCell(j, i, &cell) ;	

			GetCellIndirect( j, i, &cell );
			cell.ClearAll() ;
			cell.SetBackColor(RGB(240,240,240));		
			cell.SetAlignment( UG_ALIGNLEFT|UG_ALIGNVCENTER );
			CString strCell = (i>(nRows-1)?_T(""):m_vecStringCell[i][j]) ;
			cell.SetText(strCell) ;
			if (j==1&&i<nRows)
			{
				cell.SetParam(CELLTYPE_IS_EDITABLE);
			}
			SetCell(j, i, &cell) ;	
		}		
	}



}
/////////////////////////////////////////////////////////////////////////////
//	OnGetCell
//		This message is sent everytime the grid needs to
//		draw a cell in the grid. At this point the cell
//		object has been populated with all of the information
//		that will be used to draw the cell. This information
//		can now be changed before it is used for drawing.
//	Warning:
//		This notification is called for each cell that needs to be painted
//		Placing complicated calculations here will slowdown the refresh speed.
//	Params:
//		col, row	- coordinates of cell currently drawing
//		cell		- pointer to the cell object that is being drawn
//	Return:
//		<none>
void CAttUGCtrl::OnGetCell(int col,long row,CUGCell *cell)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*cell);
}

/////////////////////////////////////////////////////////////////////////////
//	OnDClicked
//		Sent whenever the user double clicks the left mouse button within the grid
//	Params:
//		col, row	- coordinates of a cell that received mouse click event
//		processed	- indicates if current event was processed by other control in the grid.
//		rect		- represents the CDC rectangle of cell in question
//		point		- represents the screen point where the mouse event was detected
//	Return:
//		<none>
void CAttUGCtrl::OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(processed);

	CUGCell cell;
	GetCell(col,row,&cell);
	int nCellTypeIndex = cell.GetCellType();
	int nParam = cell.GetParam();
	//acutPrintf(_T("\n%s-%d"), cell.GetText(),nParam) ;
	CString strlabel;
	if(cell.GetLabelText() != NULL)
		strlabel = cell.GetLabelText();

	// 	if(processed)
	// 	{
	// 		if(cell.GetCellType() == m_nSpinIndex)
	// 			return ;
	// 	}

	if(nParam == CELLTYPE_IS_EDITABLE || strlabel == "CELLTYPE_IS_EDITABLE")
	{
		StartEdit();
	}
}

int CAttUGCtrl::OnCellTypeNotify(long ID,int col,long row,long msg,LONG_PTR param){

	if(ID == UGCT_DROPLIST)
	{
		return OnDropList(ID, col, row, msg, param);
	}
	// 	if(ID == UGCT_CHECKBOX){
	// 		return OnCheckbox(ID, col, row, msg, param);
	// 	}
	// 	if(ID == m_nRadioIndex){
	// 	}
	// 	if(ID == m_nDateTimeIndex){
	// 		return OnDateTime(ID, col, row, msg, param);		
	// 	}
	// 	if(ID == m_nButtonIndex){
	// 		return OnPushButton( ID, col, row, msg, param);
	// 	}
	// 	if(ID == m_nEllipsisIndex){
	// 		return OnEllipsisButton( ID, col, row, msg, param);
	// 	}
	// 	if(ID == m_nSpinIndex){
	// 		return OnSpinButton( ID, col, row, msg, param);
	// 	}
	// 	if(ID == m_nSliderIndex){
	// 		return OnSlider( ID, col, row, msg, param);
	// 	}

	return TRUE;
}

int CAttUGCtrl::OnDropList(long ID,int col,long row,long msg,long param){

	CUGCell cell;
	GetCell(col,row,&cell);
	int nCellTypeIndex = cell.GetCellType();
	int nParam = cell.GetParam();

	if(nParam==DROPLIST_JGDRAG_LOAD||nParam==DROPLIST_XGDRAG_LOAD)
	{
		if(msg == UGCT_DROPLISTPOSTSELECT)
		{
// 			int nCol_QyNum = 0, nRow_QyNum = (nParam==DROPLIST_JGDRAG_LOAD?2:6) ;
// 			CString strQyType = cell.GetText();
// 			int iDragNum = CDcDATA::GetDrag(strQyType) ;
// 			if (iDragNum>=0)
// 			{
// 				CString strDragNum ;
// 				strDragNum.Format(_T("%d"), iDragNum) ;
// 				QuickSetText(nCol_QyNum, nRow_QyNum, strDragNum) ;
// 				SetCellParam(nCol_QyNum, nRow_QyNum, CELLTYPE_ISNOT_EDITABLE) ;
// 			}
// 			else if (iDragNum==-1)
// 			{
// 				SetCellParam(nCol_QyNum, nRow_QyNum,CELLTYPE_IS_EDITABLE) ;
// 			}
// 			this->m_CUGGrid->Update() ;
		}
	}

	return TRUE;
}

int CAttUGCtrl::SetCellParam(int nCol, int nRow, int nParam)
{
	int iRet = UG_SUCCESS ;
	// 	CUGCell cell;
	// 	GetCell(nCol, nRow, &cell);
	// 	return cell.SetParam(nParam) ;

	m_cell.ClearAll();
	GetCell(nCol, nRow, &m_cell);
	iRet = m_cell.SetParam(nParam) ;
	if (iRet==UG_SUCCESS)
	{
		iRet = SetCell(nCol, nRow, &m_cell);
	}

	return iRet ;
}
void CAttUGCtrl::SetupSection(long nRow, LPCTSTR lpcstrTitle)
{
	CUGCell cell;

	cell.SetBackColor(RGB(240,240,240));
	cell.SetTextColor(RGB(0,0,128));
	cell.SetFont(&m_boldFont);
	cell.SetAlignment(UG_ALIGNVCENTER);
	QuickSetRange(0,nRow,GetNumberCols() - 1,nRow,&cell);

	QuickSetText(-1,nRow,lpcstrTitle);
}

double CAttUGCtrl::QuickGetNumber(int nCol, int nRow)
{
	m_cell.ClearAll();
	GetCellIndirect(nCol,nRow,&m_cell);
	return m_cell.GetNumber();
}

int CAttUGCtrl::QuickGetNumber(int nCol, int nRow, int& number)
{
	m_cell.ClearAll();
	GetCellIndirect(nCol,nRow,&m_cell);
	return m_cell.GetNumber(&number);
}
int CAttUGCtrl::QuickGetNumber(int nCol, int nRow, double& number)
{
	m_cell.ClearAll();
	GetCellIndirect(nCol,nRow,&m_cell);
	return m_cell.GetNumber(&number);
}
BOOL CAttUGCtrl::QuickGetBool(int nCol, int nRow)
{
	m_cell.ClearAll();
	GetCellIndirect(nCol,nRow,&m_cell);
	return m_cell.GetBool();
}

// -----------------------------------------------------------------------------
int CAttUGCtrl::OnEditFinish(int  col, long  row, CWnd * edit, LPCTSTR  string, BOOL  cancelFlag)
{
	int retCode =CUGCtrl::OnEditFinish (col, row, edit, string, cancelFlag) ;
	m_vecStringCell[row][1] = string ;
	acutPrintf(_T("\nfinished col:%d, row:%d, %s"), col, row, m_vecStringCell[row][1]) ;
// 	if (m_pEntDATA->GetClassType()==_T("CDcDATA"))
// 	{
// 		if (col==0&&row==13)
// 		{
// 			CString strDc = QuickGetText(0, 0) ;
// 			CZnSxt sxt ;
// 			sxt.SetAcDbDB(m_pDb) ;
// 			if (strDc!=_T("")&&_tcscmp(string, _T(""))!=0 )
// 			{
// 				QuickSetText(0, 11, _T("否")) ;
// 				if (!sxt.UpdataConDc(strDc, string))
// 				{
// 					return 0 ;
// 				}
// 				acutPrintf(_T("\n编辑了道岔%s的多动关联道岔，更新其关联道岔%s的属性"), strDc, string) ;
// 			}
// 			else if (strDc!=_T("")&&_tcscmp(string, _T(""))==0 )
// 			{
// 				QuickSetText(0, 11, _T("是")) ;
// 				if (!sxt.UpdataConDc(strDc, m_strCellPreText, 1))
// 				{
// 					return 0 ;
// 				}
// 				acutPrintf(_T("\n删除了道岔%s的多动关联道岔，更新其关联道岔%s的属性"), strDc, m_strCellPreText) ;
// 
// 			}
// 			this->m_CUGGrid->Update() ;
// 
// 		}
	return (retCode) ;
}

// -----------------------------------------------------------------------------
int CAttUGCtrl::OnEditStart(int  col, long  row, CWnd ** edit)
{
	m_strCellPreText = QuickGetText(col, row) ;

	int retCode =CUGCtrl::OnEditStart (col, row, edit) ;
	return (retCode) ;
}

// 从外界传入数据更新Grid
int CAttUGCtrl::Refresh()
{
	OnSetup() ;
	Invalidate() ;
	return 0;
}

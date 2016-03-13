#include "dt_pmt.h"

Cdt_pmt::Cdt_pmt(void)
{
}

Cdt_pmt::~Cdt_pmt(void)
{
}

// void dt_pmt::Add_AP( double d_x,double dhight_blk,double dhight_libiao,CString str_name,double str_libiao ,int ncolorindex)
// {
// 	//²åÈëÍ¼¿é²âÊÔ
// 	AcDbBlockReference* pRef = NULL ;
// 	AcGePoint3d pt_3d_header ;
// 	CString strBlkName ; 
// 	//±íÍ·
// 	pt_3d_header.y = dhight_blk;
// 	pt_3d_header.x=d_x;
// 	pt_3d_header.z=0;
// 	strBlkName.Format(_T("DT_AP_100")) ;
// 	if (mysis.InsertBlock(pRef, strBlkName, pt_3d_header))
// 	{
// 		CString strTag=_T("NAME");
// 		//CString strValue=_T("AP22");
// 		mysis.SetAttValue(pRef,strTag,str_name,FALSE);
// 		mysis.SetAttColor(pRef,strTag,ncolorindex);
// 		pRef->close() ;
// 		pRef = NULL ;
// 	}
// 	AcGePoint3d pt_3d_insert_text(d_x,dhight_libiao,0);
// 	AcGePoint3d pt_3d_insert_text2(d_x,dhight_libiao-10,0);
// 	mysis.AddText(str_libiao, pt_3d_insert_text, pt_3d_insert_text, AcDb::kTextCenter, AcDb::kTextVertMid, 3, 0.55, PI/2,2) ;
// 	mysis.AddText(str_name, pt_3d_insert_text2, pt_3d_insert_text2, AcDb::kTextCenter, AcDb::kTextVertMid, 3, 0.55, PI/2,2) ;
// }

void Cdt_pmt::Add_AP( double d_x,double dhight_blk,double dhight_libiao,CString str_name,CString str_libiao ,CString strblkname/*=_T("DT_AP_100")*/,int ncolorindex/*=7*/ )
{
	//²åÈëÍ¼¿é²âÊÔ
	AcDbBlockReference* pRef = NULL ;
	AcGePoint3d pt_3d_header ;
	//CString strBlkName ; 
	//±íÍ·
	pt_3d_header.y = dhight_blk;
	pt_3d_header.x=d_x;
	pt_3d_header.z=0;
	//strBlkName.Format(_T("DT_AP_100")) ;
	// (m_mysys.InsertBlock(pRef2, strBlkName2, pt_3d_header2,FALSE,0,TRUE))
	if (mysis.InsertBlock(pRef, strblkname, pt_3d_header,FALSE,0,TRUE))
	{
		CString strTag=_T("NAME");
		//CString strValue=_T("AP22");
		mysis.SetAttValue(pRef,strTag,str_name,FALSE);
		mysis.SetAttColor(pRef,strTag,ncolorindex);
		pRef->close() ;
		pRef = NULL ;
	}
	AcGePoint3d pt_3d_insert_text(d_x,dhight_libiao,0);
	AcGePoint3d pt_3d_insert_text2(d_x,dhight_libiao-15,0);
	mysis.AddText(str_libiao, pt_3d_insert_text, pt_3d_insert_text, AcDb::kTextCenter, AcDb::kTextVertMid, 3, 0.55, PI/2,ncolorindex) ;
	mysis.AddText(str_name, pt_3d_insert_text2, pt_3d_insert_text2, AcDb::kTextCenter, AcDb::kTextVertMid, 3, 0.55, PI/2,ncolorindex) ;

}


void Cdt_pmt::ReLayDevice( vector<double> &d_veca,vector<CString> &str_veca,vector<double>&d_vecb,vector<CString>£¦str_vecb )
{

}


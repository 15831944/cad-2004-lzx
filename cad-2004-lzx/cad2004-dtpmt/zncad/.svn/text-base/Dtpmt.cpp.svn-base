#include "dtpmt.h"

CDtpmt::CDtpmt(void)
{
}

CDtpmt::~CDtpmt(void)
{
}

void CDtpmt::Add_AP( double d_x,double dhight_blk,double dhight_libiao,CString str_name,CString str_libiao ,CString strblkname/*=_T("DT_AP_100")*/,int ncolorindex/*=7*/ )
{
	//插入图块测试
	AcDbBlockReference* pRef = NULL ;
	AcGePoint3d pt_3d_header ;
	//CString strBlkName ; 
	//表头
	pt_3d_header.y = dhight_blk;
	if(strblkname==_T("DT_AP_100"))
	{
		pt_3d_header.y+=10;
		//绿色
		ncolorindex=3;
	}
	else if(strblkname.Left(5)==_T("DT_JZ"))
	{
		//黄色
		ncolorindex=2;
	}
	else if(strblkname.Left(6)==_T("DT_YDQ"))
	{
		//红色
		ncolorindex=1;
	}
	else if(strblkname.Left(6)==_T("DT_RFM"))
	{
		//绿色
		pt_3d_header.y+=8;
		ncolorindex=7;
	}
	else if(strblkname.Left(6)==_T("DT_DHX"))
	{
		//绿蓝色
		ncolorindex=4;
	}
	else if(strblkname==_T("XHJ_DT_ULH_A_Y_H_0"))
	{
		//pt_3d_header.y+=5;
		//绿色
		//ncolorindex=3;

	}
	else if(strblkname==_T("XHJ_DT_LH_A_Y_H_0"))
	{
		//pt_3d_header.y+=5;

	}
	else if(strblkname.Left(10)==_T("DT_STATION"))
	{
		pt_3d_header.y+=40;
	}
	else
	{
		//绿色
		ncolorindex=6;
	}
	pt_3d_header.x=d_x;
	pt_3d_header.z=0;
	if (mysis.InsertBlock(pRef, strblkname, pt_3d_header,FALSE,0,TRUE))
	{
		CString strTag=_T("NAME");
		if(strblkname.Left(6)==_T("DT_XHJ"))
		{
			strTag=_T("XHJNAME");
		}
		//CString strValue=_T("AP22");
		if(strblkname.Left(6)!=_T("DT_RFM"))
		{
			mysis.SetAttValue(pRef,strTag,str_name,FALSE);
			mysis.SetAttColor(pRef,strTag,ncolorindex);
		}
		if(strblkname.Left(10)==_T("DT_STATION"))
		{
			CString strQLB ;
			CString strBLB;
			int iLoc = 0 ;
			iLoc = str_libiao.Find(_T("+"), 0) ;
			if (iLoc!=-1)
			{
				strQLB = str_libiao.Mid(0, iLoc) ;
				strBLB=  str_libiao.Mid(iLoc,str_libiao.GetLength()+1);
				mysis.SetAttValue(pRef,_T("QLB"),strQLB,FALSE);
				mysis.SetAttValue(pRef,_T("BLB"),strBLB,FALSE);
			}	
		}
		pRef->close() ;
		pRef = NULL ;
	}
	if(strblkname.Left(10)!=_T("DT_STATION"))
	{
		AcGePoint3d pt_3d_insert_text(d_x,dhight_libiao+13,0);
		AcGePoint3d pt_3d_insert_text2(d_x,dhight_libiao,0);
		mysis.AddText(str_libiao, pt_3d_insert_text, pt_3d_insert_text, AcDb::kTextLeft, AcDb::kTextVertMid, 3, 0.55, PI/2,ncolorindex) ;
		mysis.AddText(str_name, pt_3d_insert_text2, pt_3d_insert_text2, AcDb::kTextLeft, AcDb::kTextVertMid, 3, 0.55, PI/2,ncolorindex) ;
	}	
}
void CDtpmt::Add_Device( double d_x,double dhight_blk,double dhight_libiao,CString str_name,CString str_libiao ,CString str_libiao2,int i_upordown,CString strblkname/*=_T("DT_AP_100")*/,int ncolorindex/*=7*/ )
{
	AcDbObjectId  idTextStyleTitle, idTextStyleContent, idTextStyleChs;
	idTextStyleTitle = mysis.CreateTextStyleByFileName(_T("zhffontTitle"), _T("txt.shx"), _T("hztxt2.shx"), 4, 0.7) ;	
	idTextStyleContent = mysis.CreateTextStyleByFileName(_T("zhffontContent"), _T("txt.shx"), _T("hztxt2.shx"), 3, 0.7) ;
	idTextStyleChs = mysis.CreateTextStyle(_T("CHINESE"), _T("Times New Roman"), _T(""), 2.5, 0.8) ;
	//插入图块测试
	AcDbBlockReference* pRef = NULL ;
	AcGePoint3d pt_3d_header ;
	//CString strBlkName ; 
	//表头
	pt_3d_header.y = dhight_blk;
	if(strblkname.Left(6)==_T("DT_AP_"))
	{
		pt_3d_header.y+=10;
		//绿色
		ncolorindex=3;
		mysis.Set_Layer(_T(""), _T("PMT_AP")) ;
	}
	else if(strblkname.Left(5)==_T("DT_JZ"))
	{
		//黄色
		ncolorindex=2;
		mysis.Set_Layer(_T(""), _T("PMT_JZ")) ;
	}
	else if(strblkname.Left(6)==_T("DT_YDQ"))
	{
		//红色
		ncolorindex=1;
		mysis.Set_Layer(_T(""), _T("PMT_YDQ")) ;
	}
	else if(strblkname.Left(5)==_T("DT_CD"))
	{
		ncolorindex=1;
		mysis.Set_Layer(_T(""),_T("PMT_CD"));
	}
	else if(strblkname.Left(6)==_T("DT_JCB"))
	{
		ncolorindex=1;
		if(i_upordown==0)
		{
			pt_3d_header.y-=5;
		}
		else if(i_upordown==1)
		{
			pt_3d_header.y+=5;
		}
		else
		{
			acutPrintf(_T("\n 警冲标方向输入值不对，只能取0,1"));
		}
		mysis.Set_Layer(_T(""),_T("PMT_JCB"));
	}
	else if(strblkname.Left(6)==_T("DT_RFM"))
	{
		//绿色
		pt_3d_header.y+=8;
		ncolorindex=7;
		mysis.Set_Layer(_T(""), _T("PMT_RFM")) ;
	}
	else if(strblkname.Left(6)==_T("DT_DHX"))
	{
		//绿蓝色
		ncolorindex=4;
		mysis.Set_Layer(_T(""), _T("PMT_DHX")) ;
	}
	else if(strblkname==_T("XHJ_DT_ULH_A_Y_H_0"))
	{
		//pt_3d_header.y+=5;
		//绿色
		//ncolorindex=3;
		mysis.Set_Layer(_T(""), _T("PMT_XHJ")) ;
	}
	else if(strblkname.Left(6)==_T("DT_XHJ"))
	{
		//pt_3d_header.y+=5;
		mysis.Set_Layer(_T(""), _T("PMT_XHJ")) ;
	}
	else if(strblkname.Left(10)==_T("DT_STATION"))
	{
		pt_3d_header.y+=40;
		mysis.Set_Layer(_T(""), _T("PMT_STATION")) ;
	}
	else if(strblkname.Left(14)==_T("DT_CABLEIMPORT"))
	{
		pt_3d_header.y+=42;
		mysis.Set_Layer(_T(""), _T("PMT_CABLEIMPORT")) ;
	}
	else
	{
		//绿色
		ncolorindex=6;
		mysis.Set_Layer(_T(""), _T("PMT_OTHERS")) ;
	}
	pt_3d_header.x=d_x;
	pt_3d_header.z=0;
	if (mysis.InsertBlock(pRef, strblkname, pt_3d_header,FALSE,0,TRUE))
	{
		CString strTag=_T("NAME");
		if(strblkname.Left(6)==_T("DT_XHJ"))
		{
			strTag=_T("XHJNAME");
			if(i_upordown==0)
			{
				mysis.RotationEnt(pRef,PI,pt_3d_header);
			}
		}
		if(strblkname.Left(5)==_T("DT_CD"))
		{
			if(i_upordown==0)
			{
				//默认朝右
			}
			else if(i_upordown==1)
			{
				//旋转180度
				mysis.RotationEnt(pRef,PI,pt_3d_header);
			}
			else
			{
				acutPrintf(_T("\n 车档方向输入值不对，只能取0,1"));
			}
		}
		if(strblkname.Left(6)==_T("DT_JCB"))
		{
			//需要设置警冲标长度数据
			mysis.SetAttValue(pRef,_T("LEN"),str_libiao,FALSE);
		}
		if(strblkname.Left(6)==_T("DT_DC_"))
		{
			strTag=_T("BIANHAO");
			if(i_upordown==5)
			{

			}
			else if(i_upordown==7)
			{
				mysis.RotationEnt(pRef,PI,pt_3d_header);

			}
			else if(i_upordown==6)
			{
				AcGePoint3d pt_3d_mirrY=pt_3d_header;
				pt_3d_mirrY.y+=4;
				mysis.MirrEnt(pRef,pt_3d_header,pt_3d_mirrY);

			}
			else if(i_upordown==8)
			{
				AcGePoint3d pt_3d_mirrY=pt_3d_header;
				pt_3d_mirrY.y+=4;
				mysis.MirrEnt(pRef,pt_3d_header,pt_3d_mirrY);
				mysis.RotationEnt(pRef,PI,pt_3d_header);
			}
		}
		if(strblkname.Left(6)!=_T("DT_RFM")&&(strblkname.Left(14)!=_T("DT_CABLEIMPORT")))
		{
			mysis.SetAttValue(pRef,strTag,str_name,FALSE);
			mysis.SetAttColor(pRef,strTag,ncolorindex);
			pRef->setLayer(strblkname.Left(6));
			//if(strblkname.Left(6)==_T())
		}
		if(strblkname.Left(10)==_T("DT_STATION"))
		{
			CString strQLB ;
			CString strBLB;
			int iLoc = 0 ;
			iLoc = str_libiao.Find(_T("+"), 0) ;
			if (iLoc!=-1)
			{
				strQLB = str_libiao.Mid(0, iLoc) ;
				strBLB=  str_libiao.Mid(iLoc,str_libiao.GetLength()+1);
				mysis.SetAttValue(pRef,_T("QLB"),strQLB,FALSE);
				mysis.SetAttValue(pRef,_T("BLB"),strBLB,FALSE);
			}	
		}
		pRef->setLayer(_T("pmt"));
		pRef->close() ;
		pRef = NULL ;
	}
	if(strblkname.Left(10)!=_T("DT_STATION")&&strblkname.Left(6)!=_T("DT_JCB"))
	{
		AcGePoint3d pt_3d_insert_text(d_x,dhight_libiao+13,0);
		AcGePoint3d pt_3d_insert_text2(d_x,dhight_libiao,0);
		if(str_libiao2!=_T(""))
		{
			pt_3d_insert_text.x-=2;
			mysis.AddText(str_libiao, pt_3d_insert_text, pt_3d_insert_text, AcDb::kTextLeft, AcDb::kTextVertMid, 3, 0.55, PI/2,ncolorindex) ;
			pt_3d_insert_text.x+=4;
			mysis.AddText(str_libiao2, pt_3d_insert_text, pt_3d_insert_text, AcDb::kTextLeft, AcDb::kTextVertMid, 3, 0.55, PI/2,ncolorindex) ;
			mysis.AddText(str_name, pt_3d_insert_text2, pt_3d_insert_text2, AcDb::kTextLeft, AcDb::kTextVertMid, 3, 0.55, PI/2,ncolorindex) ;
		}
		else 
		{
			mysis.AddText(str_libiao, pt_3d_insert_text, pt_3d_insert_text, AcDb::kTextLeft, AcDb::kTextVertMid, 3, 0.55, PI/2,ncolorindex) ;
			mysis.AddText(str_name, pt_3d_insert_text2, pt_3d_insert_text2, AcDb::kTextLeft, AcDb::kTextVertMid, 3, 0.55, PI/2,ncolorindex) ;
		}
	}	
}


void CDtpmt::Add_AP_1( double d_x,double dhight_blk,double dhight_libiao,CString str_name,CString str_libiao ,CString strblkname/*=_T("DT_AP_100")*/,int ncolorindex/*=7*/ )
{
	//插入图块测试
	AcDbBlockReference* pRef = NULL ;
	AcGePoint3d pt_3d_header ;
	//CString strBlkName ; 
	//表头
	pt_3d_header.y = dhight_blk;
	pt_3d_header.x=d_x;
	pt_3d_header.z=0;
	if(strblkname.Left(5)==_T("DT_AP"))
	{
		pt_3d_header.y-=10;
		ncolorindex=3;
	}
	else if(strblkname.Left(5)==_T("DT_JZ"))
	{
		ncolorindex=2;
	}
	else if(strblkname.Left(6)==_T("DT_YDQ"))
	{
		ncolorindex=1;
	}
	else if(strblkname.Left(6)==_T("DT_RFM"))
	{
		pt_3d_header.y-=8;
		ncolorindex=7;
	}
	else if(strblkname.Left(6)==_T("DT_DHX"))
	{
		ncolorindex=4;
	}
	else if(strblkname==_T("XHJ_DT_ULH_A_Y_H_0"))
	{
		//pt_3d_header.y-=5;
		//绿色
		ncolorindex=3;
	}
	else if(strblkname==_T("XHJ_DT_LH_A_Y_H_0"))
	{
		//pt_3d_header.y-=5;
	}
	else
	{
		ncolorindex=6;
	}
	if (mysis.InsertBlock(pRef, strblkname, pt_3d_header,FALSE,0,TRUE))
	{
		CString strTag=_T("NAME");
		if(strblkname.Left(6)==_T("DT_XHJ"))
		{
			strTag=_T("XHJNAME");
		}
		if(strblkname.Left(6)!=_T("DT_RFM"))
		{
			mysis.SetAttValue(pRef,strTag,str_name,FALSE);
			mysis.SetAttColor(pRef,strTag,ncolorindex);
			pRef->setLayer(strblkname.Left(6));
		}
		if(strblkname.Left(5)==_T("DT_AP")||strblkname.Left(6)==_T("DT_XHJ"))
		{
			mysis.RotationEnt(pRef,PI,pt_3d_header);
		}	
		pRef->close() ;
		pRef = NULL ;
	}
	AcGePoint3d pt_3d_insert_text(d_x,dhight_libiao-31,0);
	AcGePoint3d pt_3d_insert_text2(d_x,dhight_libiao-13,0);
	mysis.AddText(str_libiao, pt_3d_insert_text, pt_3d_insert_text, AcDb::kTextLeft, AcDb::kTextVertMid, 3, 0.55, PI/2,ncolorindex) ;
	mysis.AddText(str_name, pt_3d_insert_text2, pt_3d_insert_text2, AcDb::kTextLeft, AcDb::kTextVertMid, 3, 0.55, PI/2,ncolorindex);	
}
void CDtpmt::Add_Device_1( double d_x,double dhight_blk,double dhight_libiao,CString str_name,CString str_libiao ,CString str_libiao2,int i_upordown,CString strblkname/*=_T("DT_AP_100")*/,int ncolorindex/*=7*/ )
{
	AcDbObjectId  idTextStyleTitle, idTextStyleContent, idTextStyleChs;
	idTextStyleTitle = mysis.CreateTextStyleByFileName(_T("zhffontTitle"), _T("txt.shx"), _T("hztxt2.shx"), 4, 0.7) ;	
	idTextStyleContent = mysis.CreateTextStyleByFileName(_T("zhffontContent"), _T("txt.shx"), _T("hztxt2.shx"), 3, 0.7) ;
	idTextStyleChs = mysis.CreateTextStyle(_T("CHINESE"), _T("Times New Roman"), _T(""), 2.5, 0.8) ;
	//插入图块测试
	AcDbBlockReference* pRef = NULL ;
	AcGePoint3d pt_3d_header ;
	//CString strBlkName ; 
	//表头
	pt_3d_header.y = dhight_blk;
	pt_3d_header.x=d_x;
	pt_3d_header.z=0;
	if(strblkname.Left(5)==_T("DT_AP"))
	{
		pt_3d_header.y-=10;
		ncolorindex=3;
		mysis.Set_Layer(_T(""), _T("PMT_AP")) ;
	}
	else if(strblkname.Left(5)==_T("DT_JZ"))
	{
		ncolorindex=2;
		mysis.Set_Layer(_T(""), _T("PMT_JZ")) ;
	}
	else if(strblkname.Left(6)==_T("DT_YDQ"))
	{
		ncolorindex=1;
		mysis.Set_Layer(_T(""), _T("PMT_YDQ")) ;
	}
	else if(strblkname.Left(6)==_T("DT_RFM"))
	{
		pt_3d_header.y-=8;
		ncolorindex=7;
		mysis.Set_Layer(_T(""), _T("PMT_RFM")) ;
	}
	else if(strblkname.Left(6)==_T("DT_DHX"))
	{
		ncolorindex=4;
		mysis.Set_Layer(_T(""), _T("PMT_DHX")) ;
	}
	else if(strblkname.Left(6)==_T("DT_XHJ"))
	{
		//pt_3d_header.y-=5;
		//绿色
		ncolorindex=3;
		mysis.Set_Layer(_T(""), _T("PMT_XHJ")) ;
	}
	else if(strblkname.Left(6)==_T("DT_JCB"))
	{
		ncolorindex=1;
		if(i_upordown==0)
		{
			pt_3d_header.y-=5;
		}
		else if(i_upordown==1)
		{
			pt_3d_header.y+=5;
		}
		else
		{
			acutPrintf(_T("\n 警冲标方向输入值不对，只能取0,1"));
		}
		mysis.Set_Layer(_T(""),_T("PMT_JCB"));
	}
	else if(strblkname.Left(5)==_T("DT_CD"))
	{
		ncolorindex=1;
		mysis.Set_Layer(_T(""),_T("PMT_CD"));
	}
	else if(strblkname==_T("XHJ_DT_LH_A_Y_H_0"))
	{
		//pt_3d_header.y-=5;
		mysis.Set_Layer(_T(""), _T("PMT_XHJ")) ;
	}
	else if(strblkname.Left(14)==_T("DT_CABLEIMPORT"))
	{
		pt_3d_header.y-=42;
		mysis.Set_Layer(_T(""), _T("PMT_CABLEIMPORT")) ;

	}
	else
	{
		ncolorindex=6;
		mysis.Set_Layer(_T(""), _T("PMT_OTHERS")) ;
	}
	if (mysis.InsertBlock(pRef, strblkname, pt_3d_header,FALSE,0,TRUE))
	{
		CString strTag=_T("NAME");
		if(strblkname.Left(6)==_T("DT_XHJ"))
		{
			strTag=_T("XHJNAME");
			if(i_upordown==0)
			{
				mysis.RotationEnt(pRef,PI,pt_3d_header);
				//mysis.SetAttValue(pRef,strTag,str_name,FALSE);
				//mysis.SetAttTextAlignMode(pRef,strTag,AcDb::kTextRight,AcDb::kTextVertMid);
			}
		}
		if(strblkname.Left(10)==_T("DT_STATION"))
		{
			//旋转180度
			mysis.RotationEnt(pRef,PI,pt_3d_header);

		}
		if(strblkname.Left(5)==_T("DT_CD"))
		{
			if(i_upordown==0)
			{
				//默认朝右
			}
			else if(i_upordown==1)
			{
				//旋转180度
				mysis.RotationEnt(pRef,PI,pt_3d_header);
			}
			else
			{
				acutPrintf(_T("\n 车档方向输入值不对，只能取0,1"));
			}
		}
		if(strblkname.Left(6)==_T("DT_JCB"))
		{
			//需要设置警冲标长度数据
			mysis.SetAttValue(pRef,_T("LEN"),str_libiao,FALSE);
		}
		if(strblkname.Left(5)==_T("DT_JZ"))
		{
			//上行线的计轴位置需要旋转180度
			mysis.RotationEnt(pRef,PI,pt_3d_header);
			pRef->setLayer(_T("PMT_JZ"));
			//mysis.Set_Layer(_T(""), _T("PMT_CABLEIMPORT")) ;
		}
		if(strblkname.Left(6)==_T("DT_DC_"))
		{
			strTag=_T("BIANHAO");
			if(i_upordown==5)
			{

			}
			else if(i_upordown==7)
			{
				mysis.RotationEnt(pRef,PI,pt_3d_header);

			}
			else if(i_upordown==6)
			{
				AcGePoint3d pt_3d_mirrY=pt_3d_header;
				pt_3d_mirrY.y+=4;
				mysis.MirrEnt(pRef,pt_3d_header,pt_3d_mirrY);

			}
			else if(i_upordown==8)
			{
				AcGePoint3d pt_3d_mirrY=pt_3d_header;
				pt_3d_mirrY.y+=4;
				mysis.MirrEnt(pRef,pt_3d_header,pt_3d_mirrY);
				mysis.RotationEnt(pRef,PI,pt_3d_header);
			}
			pRef->setLayer(_T("PMT_DC"));
		}
		if(strblkname.Left(6)!=_T("DT_RFM")&&(strblkname.Left(14)!=_T("DT_CABLEIMPORT")))
		{
			mysis.SetAttValue(pRef,strTag,str_name,FALSE);
			mysis.SetAttColor(pRef,strTag,ncolorindex);
			//pRef->setLayer(_T("PMT_OTHERS"));
		}
		if(strblkname.Left(5)==_T("DT_AP"))
		{
			mysis.RotationEnt(pRef,PI,pt_3d_header);
		}	
		//pRef->setLayer(_T("pmt"));
		pRef->close() ;
		pRef = NULL ;
	}
	AcGePoint3d pt_3d_insert_text(d_x,dhight_libiao-31,0);
	AcGePoint3d pt_3d_insert_text2(d_x,dhight_libiao-13,0);
	if(str_libiao2!=_T(""))
	{
		pt_3d_insert_text.x-=2;
		mysis.AddText(str_libiao, pt_3d_insert_text, pt_3d_insert_text, AcDb::kTextLeft, AcDb::kTextVertMid, 3, 0.55, PI/2,ncolorindex) ;
		pt_3d_insert_text.x+=4;
		mysis.AddText(str_libiao2, pt_3d_insert_text, pt_3d_insert_text, AcDb::kTextLeft, AcDb::kTextVertMid, 3, 0.55, PI/2,ncolorindex) ;
		mysis.AddText(str_name, pt_3d_insert_text2, pt_3d_insert_text2, AcDb::kTextLeft, AcDb::kTextVertMid, 3, 0.55, PI/2,ncolorindex);	
	}
	else if(strblkname.Left(7)!=_T("DT_JCB_")) 
	{
		mysis.AddText(str_libiao, pt_3d_insert_text, pt_3d_insert_text, AcDb::kTextLeft, AcDb::kTextVertMid, 3, 0.55, PI/2,ncolorindex) ;
		mysis.AddText(str_name, pt_3d_insert_text2, pt_3d_insert_text2, AcDb::kTextLeft, AcDb::kTextVertMid, 3, 0.55, PI/2,ncolorindex);	
	}

}
void CDtpmt::ReLayDevice( vector<double> &d_veca,vector<CString> &str_veca,vector<CString>&str_vec_type_a, vector<double> &d_vecb,vector<CString> &str_vecb,vector<CString> &str_vec_type_b,vector<double> &d_veche,vector<double> &d_vecx,double d_width_max,double d_width_min )
{
	int i=0,j=0;
	double d_x=50;
	int cnt1=d_veca.size();
	int cnt2=d_vecb.size();
	int cnt3=str_veca.size();
	int cnt4=str_vecb.size();
	double d_data_last=0;
	while(i<cnt1&&j<cnt3)
	{
		double d_data_a=d_veca[i];
		double d_data_b=d_vecb[j];
		if(i==cnt1-1)
		{
			if((d_data_b-d_data_last)<3.5)
			{
				d_x+=d_width_min;
			}
			else
				d_x+=d_width_max;
			CString strlibiao;
			DoubleToCString(d_data_b,strlibiao);
			d_veche.push_back(d_data_b);
			d_vecx.push_back(d_x);
			CString strType;
			GetBlkType2(str_vec_type_b[j],strType);
			Add_AP_1(d_x,121,64,str_vecb[j],strlibiao,strType);
			d_data_last=d_data_b;
			j++;
		}
		else if(j==cnt3-1)
		{
			if((d_data_a-d_data_last)<3.5)
			{
				d_x+=d_width_min;
			}
			else
				d_x+=d_width_max;
			CString strlibiao;
			DoubleToCString(d_data_a,strlibiao);
			CString strType;
			GetBlkType2(str_vec_type_a[i],strType);
			d_veche.push_back(d_data_a);
			d_vecx.push_back(d_x);
			Add_AP(d_x,166,223,str_veca[i],strlibiao,strType);
			d_data_last=d_data_a;
			i++;
		}
		else if((d_data_b-d_data_a)>0.001)
		{
			if((d_data_a-d_data_last)<3.5)
			{
				d_x+=d_width_min;
			}
			else
				d_x+=d_width_max;
			//d_x+=d_width;
			CString strlibiao;
			DoubleToCString(d_data_a,strlibiao);
			d_veche.push_back(d_data_a);
			d_vecx.push_back(d_x);
			CString strType;
			GetBlkType2(str_vec_type_a[i],strType);
			Add_AP(d_x,166,223,str_veca[i],strlibiao,strType);
			d_data_last=d_data_a;
			i++;
		}
		else if(fabs(d_data_b-d_data_a)<0.001)
		{
			if(fabs(d_data_a-d_data_last)<3.5)
			{
				d_x+=d_width_min;
			}
			else
				d_x+=d_width_max;
			CString strlibiao;
			DoubleToCString(d_data_a,strlibiao);
			d_veche.push_back(d_data_a);
			d_vecx.push_back(d_x);
			d_veche.push_back(d_data_b);
			d_vecx.push_back(d_x);
			CString strType;
			GetBlkType2(str_vec_type_a[i],strType);
			Add_AP(d_x,166,223,str_veca[i],strlibiao,strType);
			GetBlkType2(str_vec_type_b[j],strType);
			Add_AP_1(d_x,121,64,str_vecb[j],strlibiao,strType);			
			d_data_last=d_data_a;
			i++;
			j++;
		}
		else 
		{
			if((d_data_b-d_data_last)<3.5)
			{
				d_x+=d_width_min;
			}
			else
				d_x+=d_width_max;
			CString strlibiao;
			DoubleToCString(d_data_b,strlibiao);
			d_veche.push_back(d_data_b);
			d_vecx.push_back(d_x);
			CString strType;
			GetBlkType2(str_vec_type_b[j],strType);
			Add_AP_1(d_x,121,64,str_vecb[j],strlibiao,strType);
			d_data_last=d_data_b;
			j++;
		}
	}
	AcGePoint2d pt2d_start1;
	AcGePoint2d pt2d_end1;
	AcGePoint2d pt2d_start2;
	AcGePoint2d pt2d_end2;
	pt2d_end1.x=d_x;
	pt2d_end1.y=121;
	//pt3d_end1.z=0;
	pt2d_start1=pt2d_end1;
	pt2d_start1.x=50;
	pt2d_start2=pt2d_start1;
	pt2d_start2.y=pt2d_start1.y+45;
	pt2d_end2=pt2d_end1;
	pt2d_end2.y=pt2d_end1.y+45;
	CLineUtil::AddPolyLine(pt2d_start1,pt2d_end1,0.3);
	CLineUtil::AddPolyLine(pt2d_start2,pt2d_end2,0.3);
	AcGePoint2d pt2dRect1;
	pt2dRect1.x=50;
	pt2dRect1.y=63;
	AcGePoint2d pt2dRect2=pt2dRect1;
	pt2dRect2.x=d_x;
	pt2dRect2.y=pt2dRect1.y-13;
	CPolylineUtil::AddRectangle(pt2dRect1,pt2dRect2,0);
	pt2dRect1.y-=13;
	pt2dRect2.y-=18;
	CPolylineUtil::AddRectangle(pt2dRect1,pt2dRect2,0);
	pt2dRect1.y-=18;
	pt2dRect2.y-=15;
	CPolylineUtil::AddRectangle(pt2dRect1,pt2dRect2,0);
	pt2dRect1.y-=15;
	pt2dRect2.y-=15;
	CPolylineUtil::AddRectangle(pt2dRect1,pt2dRect2,0);
	AcGePoint2d pt2dRect3;
	pt2dRect3.x=50;
	pt2dRect3.y=222;
	AcGePoint2d pt2dRect4=pt2dRect3;
	pt2dRect4.x=d_x;
	pt2dRect4.y=pt2dRect3.y+13;
	CPolylineUtil::AddRectangle(pt2dRect3,pt2dRect4,0);
	pt2dRect3.y+=13;
	pt2dRect4.y+=18;
	CPolylineUtil::AddRectangle(pt2dRect3,pt2dRect4,0);
	pt2dRect3.y+=18;
	pt2dRect4.y+=15;
	CPolylineUtil::AddRectangle(pt2dRect3,pt2dRect4,0);
	pt2dRect3.y+=15;
	pt2dRect4.y+=15;
	CPolylineUtil::AddRectangle(pt2dRect3,pt2dRect4,0);
	AcGePoint3d pt3d_frame;
	pt3d_frame.z=0;
	pt3d_frame.x=0;
	pt3d_frame.y=0;
	AddFrameToPmt(pt3d_frame,d_x);
	//插入图块测试
	AcDbBlockReference* pRef = NULL ;
	AcGePoint3d pt_3d_header ;
	pt_3d_header.x=50-7.5;
	pt_3d_header.z=0;
	pt_3d_header.y=63;
	CString strblkname=_T("DT_LOC_HEADER_100");
	if (mysis.InsertBlock(pRef, strblkname, pt_3d_header,FALSE,0,TRUE))
	{

		pRef->close() ;
		pRef = NULL ;
	}
	//AcDbBlockReference* pRef3 = NULL ;
	strblkname=_T("DT_PD_HEADER_000");
	pt_3d_header.y-=31+15;
	if (mysis.InsertBlock(pRef, strblkname, pt_3d_header,FALSE,0,TRUE))
	{
		CString strTag=_T("NAME");
		CString str_name=_T("坡度");
		mysis.SetAttValue(pRef,strTag,str_name,FALSE);
		pRef->close() ;
		pRef= NULL ;
	}
	pt_3d_header.y-=15;
	if (mysis.InsertBlock(pRef, strblkname, pt_3d_header,FALSE,0,TRUE))
	{
		CString strTag=_T("NAME");
		CString str_name=_T("曲线半径");
		mysis.SetAttValue(pRef,strTag,str_name,FALSE);
		pRef->close() ;
		pRef = NULL ;
	}
	AcDbBlockReference* pRef2 = NULL ;
	pt_3d_header.x=50-7.5;
	pt_3d_header.z=0;
	pt_3d_header.y=222;
	strblkname=_T("DT_LOC_HEADER_000");
	if (mysis.InsertBlock(pRef2, strblkname, pt_3d_header,FALSE,0,TRUE))
	{

		pRef2->close() ;
		pRef2 = NULL ;
	}
	strblkname=_T("DT_PD_HEADER_000");
	pt_3d_header.y+=31;
	if (mysis.InsertBlock(pRef2, strblkname, pt_3d_header,FALSE,0,TRUE))
	{
		CString strTag=_T("NAME");
		CString str_name=_T("坡度");
		mysis.SetAttValue(pRef2,strTag,str_name,FALSE);
		pRef2->close() ;
		pRef2 = NULL ;
	}
	pt_3d_header.y+=15;
	if (mysis.InsertBlock(pRef2, strblkname, pt_3d_header,FALSE,0,TRUE))
	{
		CString strTag=_T("NAME");
		CString str_name=_T("曲线半径");
		mysis.SetAttValue(pRef2,strTag,str_name,FALSE);
		pRef2->close() ;
		pRef2 = NULL ;
	}

}
// void CDtpmt::ReLay( vector<double> &d_veca,vector<CString> &str_veca,vector<CString>&str_vec_type_a, vector<int> &i_vec_upordown_a,vector<double> &d_vecb,vector<CString> &str_vecb,vector<CString> &str_vec_type_b, vector<int> &i_vec_upordown_b,vector<double> &d_veche,vector<double> &d_vecx,double d_width_max,double d_width_min)
// {
// 	int i=0,j=0;
// 	double d_x=50;
// 	int cnt1=d_veca.size();
// 	int cnt2=d_vecb.size();
// 	double d_data_last=0;
// 	while(i<cnt1&&j<cnt2)
// 	{
// 		double d_data_a=d_veca[i];
// 		double d_data_b=d_vecb[j];
// 		if(i==cnt1-1)
// 		{
// 			if((d_data_b-d_data_last)<3.5)
// 			{
// 				d_x+=d_width_min;
// 			}
// 			else
// 				d_x+=d_width_max;
// 			CString strlibiao;
// 			DoubleToCString(d_data_b,strlibiao);
// 			d_veche.push_back(d_data_b);
// 			d_vecx.push_back(d_x);
// 			CString strType;
// 			GetBlkType2(str_vec_type_b[j],strType);
// 			Add_Device_1(d_x,121,64,str_vecb[j],strlibiao,i_vec_upordown_b[j],strType);
// 			d_data_last=d_data_b;
// 			j++;
// 		}
// 		else if(j==cnt2-1)
// 		{
// 			if((d_data_a-d_data_last)<3.5)
// 			{
// 				d_x+=d_width_min;
// 			}
// 			else
// 				d_x+=d_width_max;
// 			CString strlibiao;
// 			DoubleToCString(d_data_a,strlibiao);
// 			CString strType;
// 			GetBlkType2(str_vec_type_a[i],strType);
// 			d_veche.push_back(d_data_a);
// 			d_vecx.push_back(d_x);
// 			Add_Device(d_x,166,223,str_veca[i],strlibiao,i_vec_upordown_a[i],strType);
// 			d_data_last=d_data_a;
// 			i++;
// 		}
// 		else if((d_data_b-d_data_a)>0.001)
// 		{
// 			if((d_data_a-d_data_last)<3.5)
// 			{
// 				d_x+=d_width_min;
// 			}
// 			else
// 				d_x+=d_width_max;
// 			//d_x+=d_width;
// 			CString strlibiao;
// 			DoubleToCString(d_data_a,strlibiao);
// 			d_veche.push_back(d_data_a);
// 			d_vecx.push_back(d_x);
// 			CString strType;
// 			GetBlkType2(str_vec_type_a[i],strType);
// 			Add_Device(d_x,166,223,str_veca[i],strlibiao,i_vec_upordown_a[i],strType);
// 			d_data_last=d_data_a;
// 			i++;
// 		}
// 		else if(fabs(d_data_b-d_data_a)<0.001)
// 		{
// 			if(fabs(d_data_a-d_data_last)<3.5)
// 			{
// 				d_x+=d_width_min;
// 			}
// 			else
// 				d_x+=d_width_max;
// 			CString strlibiao;
// 			DoubleToCString(d_data_a,strlibiao);
// 			d_veche.push_back(d_data_a);
// 			d_vecx.push_back(d_x);
// 			d_veche.push_back(d_data_b);
// 			d_vecx.push_back(d_x);
// 			CString strType;
// 			GetBlkType2(str_vec_type_a[i],strType);
// 			Add_Device(d_x,166,223,str_veca[i],strlibiao,i_vec_upordown_a[i],strType);
// 			GetBlkType2(str_vec_type_b[j],strType);
// 			Add_Device_1(d_x,121,64,str_vecb[j],strlibiao,i_vec_upordown_b[j],strType);			
// 			d_data_last=d_data_a;
// 			i++;
// 			j++;
// 		}
// 		else 
// 		{
// 			if((d_data_b-d_data_last)<3.5)
// 			{
// 				d_x+=d_width_min;
// 			}
// 			else
// 				d_x+=d_width_max;
// 			CString strlibiao;
// 			DoubleToCString(d_data_b,strlibiao);
// 			d_veche.push_back(d_data_b);
// 			d_vecx.push_back(d_x);
// 			CString strType;
// 			GetBlkType2(str_vec_type_b[j],strType);
// 			Add_Device_1(d_x,121,64,str_vecb[j],strlibiao,i_vec_upordown_b[j],strType);
// 			d_data_last=d_data_b;
// 			j++;
// 		}
// 	}
// 	AcGePoint2d pt2d_start1;
// 	AcGePoint2d pt2d_end1;
// 	AcGePoint2d pt2d_start2;
// 	AcGePoint2d pt2d_end2;
// 	pt2d_end1.x=d_x;
// 	pt2d_end1.y=121;
// 	//pt3d_end1.z=0;
// 	pt2d_start1=pt2d_end1;
// 	pt2d_start1.x=50;
// 	pt2d_start2=pt2d_start1;
// 	pt2d_start2.y=pt2d_start1.y+45;
// 	pt2d_end2=pt2d_end1;
// 	pt2d_end2.y=pt2d_end1.y+45;
// 	CLineUtil::AddPolyLine(pt2d_start1,pt2d_end1,0.3);
// 	CLineUtil::AddPolyLine(pt2d_start2,pt2d_end2,0.3);
// 	AcGePoint2d pt2dRect1;
// 	pt2dRect1.x=50;
// 	pt2dRect1.y=63;
// 	AcGePoint2d pt2dRect2=pt2dRect1;
// 	pt2dRect2.x=d_x;
// 	pt2dRect2.y=pt2dRect1.y-13;
// 	CPolylineUtil::AddRectangle(pt2dRect1,pt2dRect2,0);
// 	pt2dRect1.y-=13;
// 	pt2dRect2.y-=18;
// 	CPolylineUtil::AddRectangle(pt2dRect1,pt2dRect2,0);
// 	pt2dRect1.y-=18;
// 	pt2dRect2.y-=15;
// 	CPolylineUtil::AddRectangle(pt2dRect1,pt2dRect2,0);
// 	pt2dRect1.y-=15;
// 	pt2dRect2.y-=15;
// 	CPolylineUtil::AddRectangle(pt2dRect1,pt2dRect2,0);
// 	AcGePoint2d pt2dRect3;
// 	pt2dRect3.x=50;
// 	pt2dRect3.y=222;
// 	AcGePoint2d pt2dRect4=pt2dRect3;
// 	pt2dRect4.x=d_x;
// 	pt2dRect4.y=pt2dRect3.y+13;
// 	CPolylineUtil::AddRectangle(pt2dRect3,pt2dRect4,0);
// 	pt2dRect3.y+=13;
// 	pt2dRect4.y+=18;
// 	CPolylineUtil::AddRectangle(pt2dRect3,pt2dRect4,0);
// 	pt2dRect3.y+=18;
// 	pt2dRect4.y+=15;
// 	CPolylineUtil::AddRectangle(pt2dRect3,pt2dRect4,0);
// 	pt2dRect3.y+=15;
// 	pt2dRect4.y+=15;
// 	CPolylineUtil::AddRectangle(pt2dRect3,pt2dRect4,0);
// 	AcGePoint3d pt3d_frame;
// 	pt3d_frame.z=0;
// 	pt3d_frame.x=0;
// 	pt3d_frame.y=0;
// 	AddFrameToPmt(pt3d_frame,d_x);
// 	//插入图块测试
// 	AcDbBlockReference* pRef = NULL ;
// 	AcGePoint3d pt_3d_header ;
// 	pt_3d_header.x=50-7.5;
// 	pt_3d_header.z=0;
// 	pt_3d_header.y=63;
// 	CString strblkname=_T("DT_LOC_HEADER_100");
// 	if (mysis.InsertBlock(pRef, strblkname, pt_3d_header,FALSE,0,TRUE))
// 	{
// 
// 		pRef->close() ;
// 		pRef = NULL ;
// 	}
// 	//AcDbBlockReference* pRef3 = NULL ;
// 	strblkname=_T("DT_PD_HEADER_000");
// 	pt_3d_header.y-=31+15;
// 	if (mysis.InsertBlock(pRef, strblkname, pt_3d_header,FALSE,0,TRUE))
// 	{
// 		CString strTag=_T("NAME");
// 		CString str_name=_T("坡度");
// 		mysis.SetAttValue(pRef,strTag,str_name,FALSE);
// 		pRef->close() ;
// 		pRef= NULL ;
// 	}
// 	pt_3d_header.y-=15;
// 	if (mysis.InsertBlock(pRef, strblkname, pt_3d_header,FALSE,0,TRUE))
// 	{
// 		CString strTag=_T("NAME");
// 		CString str_name=_T("曲线半径");
// 		mysis.SetAttValue(pRef,strTag,str_name,FALSE);
// 		pRef->close() ;
// 		pRef = NULL ;
// 	}
// 	AcDbBlockReference* pRef2 = NULL ;
// 	pt_3d_header.x=50-7.5;
// 	pt_3d_header.z=0;
// 	pt_3d_header.y=222;
// 	strblkname=_T("DT_LOC_HEADER_000");
// 	if (mysis.InsertBlock(pRef2, strblkname, pt_3d_header,FALSE,0,TRUE))
// 	{
// 
// 		pRef2->close() ;
// 		pRef2 = NULL ;
// 	}
// 	strblkname=_T("DT_PD_HEADER_000");
// 	pt_3d_header.y+=31;
// 	if (mysis.InsertBlock(pRef2, strblkname, pt_3d_header,FALSE,0,TRUE))
// 	{
// 		CString strTag=_T("NAME");
// 		CString str_name=_T("坡度");
// 		mysis.SetAttValue(pRef2,strTag,str_name,FALSE);
// 		pRef2->close() ;
// 		pRef2 = NULL ;
// 	}
// 	pt_3d_header.y+=15;
// 	if (mysis.InsertBlock(pRef2, strblkname, pt_3d_header,FALSE,0,TRUE))
// 	{
// 		CString strTag=_T("NAME");
// 		CString str_name=_T("曲线半径");
// 		mysis.SetAttValue(pRef2,strTag,str_name,FALSE);
// 		pRef2->close() ;
// 		pRef2 = NULL ;
// 	}
// 
// }
void CDtpmt::ReLay2( vector<double> &d_veca,vector<CString> &str_veca,vector<double> &d_vec_libiao2_a, vector<CString>&str_vec_type_a, vector<int> &i_vec_line_a,vector<int> &i_vec_upordown_a,vector<CString> &str_vec_info_a,vector<double> &d_vecb,vector<CString> &str_vecb,vector<double> &d_vec_libiao2_b,vector<CString> &str_vec_type_b, vector<int> &i_vec_line_b,vector<int> &i_vec_upordown_b,vector<CString> &str_vec_info_b,vector<double> &d_veche,vector<double> &d_vecx,double d_width_max,double d_width_min)
{
	int i=0,j=0;
	double d_x=50;
	int cnt1=d_veca.size();
	int cnt2=d_vecb.size();
	double d_data_last=0;
	double d_start[7];
	double d_end[7];
// 	mysis.SetLayer(_T(""),_T("pmt"));
 	//AcDbObjectId objIdOldLayer = ->clayer() ;
 	//mysis.Set_Layer(_T(""), _T("pmt")) ;
	for(int k=0;k<7;k++)
	{
		d_start[k]=d_x;
		d_end[k]=d_x;
	}
	while(i<cnt1&&j<cnt2)
	{
		double d_data_a=d_veca[i];
		double d_data_b=d_vecb[j];
		if(i==cnt1-1)
		{
			if((d_data_b-d_data_last)<3.5)
			{
				d_x+=d_width_min;
			}
			else
				d_x+=d_width_max;
			CString strlibiao;
			DoubleToCString(d_data_b,strlibiao);
			CString strlibiao2;
			DoubleToCString(d_vec_libiao2_b[j],strlibiao2);
			d_veche.push_back(d_data_b);
			d_vecx.push_back(d_x);
			CString strType;
			GetBlkType2(str_vec_type_b[j],strType);
			double d_hight_blk_b=0;
			GetHightBlkAndLibiao(i_vec_line_b[j],d_hight_blk_b);
			if(i_vec_line_b[j]<=7&&i_vec_line_b[j]>=1)
			{
				if(d_start[i_vec_line_b[j]-1]==50.0)
				{
					d_start[i_vec_line_b[j]-1]=d_x;
				}
				d_end[i_vec_line_b[j]-1]=d_x;
			}
			Add_Device_1(d_x,d_hight_blk_b,64,str_vecb[j],strlibiao,strlibiao2,i_vec_upordown_b[j],strType);
			d_data_last=d_data_b;
			j++;
		}
		else if(j==cnt2-1)
		{
			if((d_data_a-d_data_last)<3.5)
			{
				d_x+=d_width_min;
			}
			else
				d_x+=d_width_max;
			CString strlibiao;
			DoubleToCString(d_data_a,strlibiao);
			CString strlibiao2;
			DoubleToCString(d_vec_libiao2_a[i],strlibiao2);
			CString strType;
			GetBlkType2(str_vec_type_a[i],strType);
			d_veche.push_back(d_data_a);
			d_vecx.push_back(d_x);
			double d_hight_blk_a=0;
			GetHightBlkAndLibiao(i_vec_line_a[i],d_hight_blk_a);
			if(i_vec_line_a[i]<=7&&i_vec_line_a[i]>=1)
			{
				if(d_start[i_vec_line_a[i]-1]==50.0)
				{
					d_start[i_vec_line_a[i]-1]=d_x;
				}
				d_end[i_vec_line_a[i]-1]=d_x;
			}
			Add_Device(d_x,d_hight_blk_a,223,str_veca[i],strlibiao,strlibiao2,i_vec_upordown_a[i],strType);
			d_data_last=d_data_a;
			i++;
		}
		else if((d_data_b-d_data_a)>0.001)
		{
			if((d_data_a-d_data_last)<3.5)
			{
				d_x+=d_width_min;
			}
			else
				d_x+=d_width_max;
			//d_x+=d_width;
			CString strlibiao;
			DoubleToCString(d_data_a,strlibiao);
			CString strlibiao2;
			DoubleToCString(d_vec_libiao2_a[i],strlibiao2);
			d_veche.push_back(d_data_a);
			d_vecx.push_back(d_x);
			CString strType;
			GetBlkType2(str_vec_type_a[i],strType);
			double d_hight_blk_a=0;
			GetHightBlkAndLibiao(i_vec_line_a[i],d_hight_blk_a);
			if(i_vec_line_a[i]<=7&&i_vec_line_a[i]>=1)
			{
				if(d_start[i_vec_line_a[i]-1]==50.0)
				{
					d_start[i_vec_line_a[i]-1]=d_x;
				}
				d_end[i_vec_line_a[i]-1]=d_x;
			}
			Add_Device(d_x,d_hight_blk_a,223,str_veca[i],strlibiao,strlibiao2,i_vec_upordown_a[i],strType);
			d_data_last=d_data_a;
			i++;
		}
		else if(fabs(d_data_b-d_data_a)<0.001)
		{
			if(fabs(d_data_a-d_data_last)<3.5)
			{
				d_x+=d_width_min;
			}
			else
				d_x+=d_width_max;
			CString strlibiao;
			DoubleToCString(d_data_a,strlibiao);
			CString strlibiao2;
			DoubleToCString(d_vec_libiao2_a[i],strlibiao2);
			d_veche.push_back(d_data_a);
			d_vecx.push_back(d_x);
			d_veche.push_back(d_data_b);
			d_vecx.push_back(d_x);
			CString strType;
			GetBlkType2(str_vec_type_a[i],strType);
			double d_hight_blk_a=0;
			GetHightBlkAndLibiao(i_vec_line_a[i],d_hight_blk_a);
			if(i_vec_line_a[i]<=7&&i_vec_line_a[i]>=1)
			{
				if(d_start[i_vec_line_a[i]-1]==50.0)
				{
					d_start[i_vec_line_a[i]-1]=d_x;
				}
				d_end[i_vec_line_a[i]-1]=d_x;
			}
			Add_Device(d_x,d_hight_blk_a,223,str_veca[i],strlibiao,strlibiao2,i_vec_upordown_a[i],strType);
			GetBlkType2(str_vec_type_b[j],strType);
			DoubleToCString(d_vec_libiao2_b[j],strlibiao2);
			double d_hight_blk_b=0;
			GetHightBlkAndLibiao(i_vec_line_b[j],d_hight_blk_b);
			if(i_vec_line_b[j]<=7&&i_vec_line_b[j]>=1)
			{
				if(d_start[i_vec_line_b[j]-1]==50.0)
				{
					d_start[i_vec_line_b[j]-1]=d_x;
				}
				d_end[i_vec_line_b[j]-1]=d_x;
			}
			Add_Device_1(d_x,d_hight_blk_b,64,str_vecb[j],strlibiao,strlibiao2,i_vec_upordown_b[j],strType);			
			d_data_last=d_data_a;
			i++;
			j++;
		}
		else 
		{
			if((d_data_b-d_data_last)<3.5)
			{
				d_x+=d_width_min;
			}
			else
				d_x+=d_width_max;
			CString strlibiao;
			DoubleToCString(d_data_b,strlibiao);
			CString strlibiao2;
			DoubleToCString(d_vec_libiao2_b[j],strlibiao2);
			d_veche.push_back(d_data_b);
			d_vecx.push_back(d_x);
			CString strType;
			GetBlkType2(str_vec_type_b[j],strType);
			double d_hight_blk_b=0;
			GetHightBlkAndLibiao(i_vec_line_b[j],d_hight_blk_b);
			if(i_vec_line_b[j]<=7&&i_vec_line_b[j]>=1)
			{
				if(d_start[i_vec_line_b[j]-1]==50.0)
				{
					d_start[i_vec_line_b[j]-1]=d_x;
				}
				d_end[i_vec_line_b[j]-1]=d_x;
			}
			Add_Device_1(d_x,d_hight_blk_b,64,str_vecb[j],strlibiao,strlibiao2,i_vec_upordown_b[j],strType);
			d_data_last=d_data_b;
			j++;
		}
	}
	AcGePoint2d pt2d_start1;
	AcGePoint2d pt2d_end1;
	AcGePoint2d pt2d_start2;
	AcGePoint2d pt2d_end2;
	pt2d_end1.x=d_x;
	pt2d_end1.y=121;
	//pt3d_end1.z=0;
	pt2d_start1=pt2d_end1;
	pt2d_start1.x=50;
	pt2d_start2=pt2d_start1;
	pt2d_start2.y=pt2d_start1.y+45;
	pt2d_end2=pt2d_end1;
	pt2d_end2.y=pt2d_end1.y+45;
	for(int k=0;k<7;k++)
	{
		if(k==1||k==5)
		{
			pt2d_start1.x=50;
		}
		else
			pt2d_start1.x=d_start[k];
		GetHightBlkAndLibiao(k+1,pt2d_start1.y);
		if(k==1||k==5)
			pt2d_end1.x=(d_end[1]>d_end[5]?d_end[1]:d_end[5])+5;
		else
			pt2d_end1.x=d_end[k];
		GetHightBlkAndLibiao(k+1,pt2d_end1.y);
		if(d_start[k]!=d_end[k])
		{
			CLineUtil::AddPolyLine(pt2d_start1,pt2d_end1,0.3);
		}
		
	}
	AcGePoint2d pt2dRect1;
	pt2dRect1.x=50;
	pt2dRect1.y=63;
	AcGePoint2d pt2dRect2=pt2dRect1;
	pt2dRect2.x=d_x;
	pt2dRect2.y=pt2dRect1.y-13;
	CPolylineUtil::AddRectangle(pt2dRect1,pt2dRect2,0.1);
	pt2dRect1.y-=13;
	pt2dRect2.y-=18;
	CPolylineUtil::AddRectangle(pt2dRect1,pt2dRect2,0.1);
	pt2dRect1.y-=18;
	pt2dRect2.y-=15;
	CPolylineUtil::AddRectangle(pt2dRect1,pt2dRect2,0.1);
	pt2dRect1.y-=15;
	pt2dRect2.y-=15;
	CPolylineUtil::AddRectangle(pt2dRect1,pt2dRect2,0.1);
	AcGePoint2d pt2dRect3;
	pt2dRect3.x=50;
	pt2dRect3.y=222;
	AcGePoint2d pt2dRect4=pt2dRect3;
	pt2dRect4.x=d_x;
	pt2dRect4.y=pt2dRect3.y+13;
	CPolylineUtil::AddRectangle(pt2dRect3,pt2dRect4,0.1);
	pt2dRect3.y+=13;
	pt2dRect4.y+=18;
	CPolylineUtil::AddRectangle(pt2dRect3,pt2dRect4,0.1);
	pt2dRect3.y+=18;
	pt2dRect4.y+=15;
	CPolylineUtil::AddRectangle(pt2dRect3,pt2dRect4,0.1);
	pt2dRect3.y+=15;
	pt2dRect4.y+=15;
	CPolylineUtil::AddRectangle(pt2dRect3,pt2dRect4,0.1);
	AcGePoint3d pt3d_frame;
	pt3d_frame.z=0;
	pt3d_frame.x=0;
	pt3d_frame.y=0;
	AddFrameToPmt(pt3d_frame,d_x);
	//插入图块测试
	AcDbBlockReference* pRef = NULL ;
	AcGePoint3d pt_3d_header ;
	pt_3d_header.x=50-7.5;
	pt_3d_header.z=0;
	pt_3d_header.y=63;
	CString strblkname=_T("DT_LOC_HEADER_100");
	if (mysis.InsertBlock(pRef, strblkname, pt_3d_header,FALSE,0,TRUE))
	{

		pRef->close() ;
		pRef = NULL ;
	}
	//AcDbBlockReference* pRef3 = NULL ;
	strblkname=_T("DT_PD_HEADER_000");
	pt_3d_header.y-=31+15;
	if (mysis.InsertBlock(pRef, strblkname, pt_3d_header,FALSE,0,TRUE))
	{
		CString strTag=_T("NAME");
		CString str_name=_T("坡度");
		mysis.SetAttValue(pRef,strTag,str_name,FALSE);
		pRef->close() ;
		pRef= NULL ;
	}
	pt_3d_header.y-=15;
	if (mysis.InsertBlock(pRef, strblkname, pt_3d_header,FALSE,0,TRUE))
	{
		CString strTag=_T("NAME");
		CString str_name=_T("曲线半径");
		mysis.SetAttValue(pRef,strTag,str_name,FALSE);
		pRef->close() ;
		pRef = NULL ;
	}
	AcDbBlockReference* pRef2 = NULL ;
	pt_3d_header.x=50-7.5;
	pt_3d_header.z=0;
	pt_3d_header.y=222;
	strblkname=_T("DT_LOC_HEADER_000");
	if (mysis.InsertBlock(pRef2, strblkname, pt_3d_header,FALSE,0,TRUE))
	{

		pRef2->close() ;
		pRef2 = NULL ;
	}
	strblkname=_T("DT_PD_HEADER_000");
	pt_3d_header.y+=31;
	if (mysis.InsertBlock(pRef2, strblkname, pt_3d_header,FALSE,0,TRUE))
	{
		CString strTag=_T("NAME");
		CString str_name=_T("坡度");
		mysis.SetAttValue(pRef2,strTag,str_name,FALSE);
		pRef2->close() ;
		pRef2 = NULL ;
	}
	pt_3d_header.y+=15;
	if (mysis.InsertBlock(pRef2, strblkname, pt_3d_header,FALSE,0,TRUE))
	{
		CString strTag=_T("NAME");
		CString str_name=_T("曲线半径");
		mysis.SetAttValue(pRef2,strTag,str_name,FALSE);
		pRef2->close() ;
		pRef2 = NULL ;
	}

}
//根据张家铭要求，将坡度、曲线半径和公里标改成熊总规定的标准显示
void CDtpmt::ReLay3( vector<double> &d_veca,vector<CString> &str_veca,vector<double> &d_vec_libiao2_a, vector<CString>&str_vec_type_a, vector<int> &i_vec_line_a,vector<int> &i_vec_upordown_a,vector<CString> &str_vec_info_a,vector<double> &d_vecb,vector<CString> &str_vecb,vector<double> &d_vec_libiao2_b,vector<CString> &str_vec_type_b, vector<int> &i_vec_line_b,vector<int> &i_vec_upordown_b,vector<CString> &str_vec_info_b,vector<double> &d_veche,vector<double> &d_vecx,double d_width_max,double d_width_min)
{
	int i=0,j=0;
	double d_x=50;
	int cnt1=d_veca.size();
	int cnt2=d_vecb.size();
	double d_data_last=0;
	double d_start[7];
	double d_end[7];
	// 	mysis.SetLayer(_T(""),_T("pmt"));
	//AcDbObjectId objIdOldLayer = ->clayer() ;
	//mysis.Set_Layer(_T(""), _T("pmt")) ;
	for(int k=0;k<7;k++)
	{
		d_start[k]=d_x;
		d_end[k]=d_x;
	}
	while(i<cnt1&&j<cnt2)
	{
		double d_data_a=d_veca[i];
		double d_data_b=d_vecb[j];
		if(i==cnt1-1)
		{
			if((d_data_b-d_data_last)<3.5)
			{
				d_x+=d_width_min;
			}
			else
				d_x+=d_width_max;
			CString strlibiao;
			DoubleToCString(d_data_b,strlibiao);
			CString strlibiao2;
			DoubleToCString(d_vec_libiao2_b[j],strlibiao2);
			d_veche.push_back(d_data_b);
			d_vecx.push_back(d_x);
			CString strType;
			GetBlkType2(str_vec_type_b[j],strType);
			double d_hight_blk_b=0;
			GetHightBlkAndLibiao(i_vec_line_b[j],d_hight_blk_b);
			if(i_vec_line_b[j]<=7&&i_vec_line_b[j]>=1)
			{
				if(d_start[i_vec_line_b[j]-1]==50.0)
				{
					d_start[i_vec_line_b[j]-1]=d_x;
				}
				d_end[i_vec_line_b[j]-1]=d_x;
			}
			//Add_Device_1(d_x,d_hight_blk_b,64,str_vecb[j],strlibiao,strlibiao2,i_vec_upordown_b[j],strType);
			Add_Device_01(d_x,d_hight_blk_b,86,str_vecb[j],strlibiao,strlibiao2,i_vec_upordown_b[j],strType);
			d_data_last=d_data_b;
			j++;
		}
		else if(j==cnt2-1)
		{
			if((d_data_a-d_data_last)<3.5)
			{
				d_x+=d_width_min;
			}
			else
				d_x+=d_width_max;
			CString strlibiao;
			DoubleToCString(d_data_a,strlibiao);
			CString strlibiao2;
			DoubleToCString(d_vec_libiao2_a[i],strlibiao2);
			CString strType;
			GetBlkType2(str_vec_type_a[i],strType);
			d_veche.push_back(d_data_a);
			d_vecx.push_back(d_x);
			double d_hight_blk_a=0;
			GetHightBlkAndLibiao(i_vec_line_a[i],d_hight_blk_a);
			if(i_vec_line_a[i]<=7&&i_vec_line_a[i]>=1)
			{
				if(d_start[i_vec_line_a[i]-1]==50.0)
				{
					d_start[i_vec_line_a[i]-1]=d_x;
				}
				d_end[i_vec_line_a[i]-1]=d_x;
			}
			//Add_Device(d_x,d_hight_blk_a,223,str_veca[i],strlibiao,strlibiao2,i_vec_upordown_a[i],strType);
			Add_Device_00(d_x,d_hight_blk_a,201,str_veca[i],strlibiao,strlibiao2,i_vec_upordown_a[i],strType);
			d_data_last=d_data_a;
			i++;
		}
		else if((d_data_b-d_data_a)>0.001)
		{
			if((d_data_a-d_data_last)<3.5)
			{
				d_x+=d_width_min;
			}
			else
				d_x+=d_width_max;
			//d_x+=d_width;
			CString strlibiao;
			DoubleToCString(d_data_a,strlibiao);
			CString strlibiao2;
			DoubleToCString(d_vec_libiao2_a[i],strlibiao2);
			d_veche.push_back(d_data_a);
			d_vecx.push_back(d_x);
			CString strType;
			GetBlkType2(str_vec_type_a[i],strType);
			double d_hight_blk_a=0;
			GetHightBlkAndLibiao(i_vec_line_a[i],d_hight_blk_a);
			if(i_vec_line_a[i]<=7&&i_vec_line_a[i]>=1)
			{
				if(d_start[i_vec_line_a[i]-1]==50.0)
				{
					d_start[i_vec_line_a[i]-1]=d_x;
				}
				d_end[i_vec_line_a[i]-1]=d_x;
			}
			//Add_Device(d_x,d_hight_blk_a,223,str_veca[i],strlibiao,strlibiao2,i_vec_upordown_a[i],strType);
			Add_Device_00(d_x,d_hight_blk_a,201,str_veca[i],strlibiao,strlibiao2,i_vec_upordown_a[i],strType);
			d_data_last=d_data_a;
			i++;
		}
		else if(fabs(d_data_b-d_data_a)<0.001)
		{
			if(fabs(d_data_a-d_data_last)<3.5)
			{
				d_x+=d_width_min;
			}
			else
				d_x+=d_width_max;
			CString strlibiao;
			DoubleToCString(d_data_a,strlibiao);
			CString strlibiao2;
			DoubleToCString(d_vec_libiao2_a[i],strlibiao2);
			d_veche.push_back(d_data_a);
			d_vecx.push_back(d_x);
			d_veche.push_back(d_data_b);
			d_vecx.push_back(d_x);
			CString strType;
			GetBlkType2(str_vec_type_a[i],strType);
			double d_hight_blk_a=0;
			GetHightBlkAndLibiao(i_vec_line_a[i],d_hight_blk_a);
			if(i_vec_line_a[i]<=7&&i_vec_line_a[i]>=1)
			{
				if(d_start[i_vec_line_a[i]-1]==50.0)
				{
					d_start[i_vec_line_a[i]-1]=d_x;
				}
				d_end[i_vec_line_a[i]-1]=d_x;
			}
			//Add_Device(d_x,d_hight_blk_a,223,str_veca[i],strlibiao,strlibiao2,i_vec_upordown_a[i],strType);
			Add_Device_00(d_x,d_hight_blk_a,201,str_veca[i],strlibiao,strlibiao2,i_vec_upordown_a[i],strType);
			GetBlkType2(str_vec_type_b[j],strType);
			DoubleToCString(d_vec_libiao2_b[j],strlibiao2);
			double d_hight_blk_b=0;
			GetHightBlkAndLibiao(i_vec_line_b[j],d_hight_blk_b);
			if(i_vec_line_b[j]<=7&&i_vec_line_b[j]>=1)
			{
				if(d_start[i_vec_line_b[j]-1]==50.0)
				{
					d_start[i_vec_line_b[j]-1]=d_x;
				}
				d_end[i_vec_line_b[j]-1]=d_x;
			}
			//Add_Device_1(d_x,d_hight_blk_b,64,str_vecb[j],strlibiao,strlibiao2,i_vec_upordown_b[j],strType);		
			Add_Device_01(d_x,d_hight_blk_b,86,str_vecb[j],strlibiao,strlibiao2,i_vec_upordown_b[j],strType);
			d_data_last=d_data_a;
			i++;
			j++;
		}
		else 
		{
			if((d_data_b-d_data_last)<3.5)
			{
				d_x+=d_width_min;
			}
			else
				d_x+=d_width_max;
			CString strlibiao;
			DoubleToCString(d_data_b,strlibiao);
			CString strlibiao2;
			DoubleToCString(d_vec_libiao2_b[j],strlibiao2);
			d_veche.push_back(d_data_b);
			d_vecx.push_back(d_x);
			CString strType;
			GetBlkType2(str_vec_type_b[j],strType);
			double d_hight_blk_b=0;
			GetHightBlkAndLibiao(i_vec_line_b[j],d_hight_blk_b);
			if(i_vec_line_b[j]<=7&&i_vec_line_b[j]>=1)
			{
				if(d_start[i_vec_line_b[j]-1]==50.0)
				{
					d_start[i_vec_line_b[j]-1]=d_x;
				}
				d_end[i_vec_line_b[j]-1]=d_x;
			}
			//Add_Device_1(d_x,d_hight_blk_b,64,str_vecb[j],strlibiao,strlibiao2,i_vec_upordown_b[j],strType);
			Add_Device_01(d_x,d_hight_blk_b,86,str_vecb[j],strlibiao,strlibiao2,i_vec_upordown_b[j],strType);
			d_data_last=d_data_b;
			j++;
		}
	}
	AcGePoint2d pt2d_start1;
	AcGePoint2d pt2d_end1;
	AcGePoint2d pt2d_start2;
	AcGePoint2d pt2d_end2;
	pt2d_end1.x=d_x;
	pt2d_end1.y=121;
	//pt3d_end1.z=0;
	pt2d_start1=pt2d_end1;
	pt2d_start1.x=50;
	pt2d_start2=pt2d_start1;
	pt2d_start2.y=pt2d_start1.y+45;
	pt2d_end2=pt2d_end1;
	pt2d_end2.y=pt2d_end1.y+45;
	for(int k=0;k<7;k++)
	{
		if(k==1||k==5)
		{
			pt2d_start1.x=50;
		}
		else
			pt2d_start1.x=d_start[k];
		GetHightBlkAndLibiao(k+1,pt2d_start1.y);
		if(k==1||k==5)
			pt2d_end1.x=(d_end[1]>d_end[5]?d_end[1]:d_end[5])+5;
		else
			pt2d_end1.x=d_end[k];
		GetHightBlkAndLibiao(k+1,pt2d_end1.y);
		if(d_start[k]!=d_end[k])
		{
			CLineUtil::AddPolyLine(pt2d_start1,pt2d_end1,0.3);
		}

	}
	AcGePoint2d pt2dRect1;
	pt2dRect1.x=50;
	pt2dRect1.y=34;
	AcGePoint2d pt2dRect2=pt2dRect1;
	pt2dRect2.x=d_x;
	pt2dRect2.y=pt2dRect1.y-15;
	CPolylineUtil::AddRectangle(pt2dRect1,pt2dRect2,0.1);
	pt2dRect1.y-=17;
	pt2dRect2.y-=17;
	CPolylineUtil::AddRectangle(pt2dRect1,pt2dRect2,0.1);
	// 	pt2dRect1.y-=18;
	// 	pt2dRect2.y-=15;
	// 	CPolylineUtil::AddRectangle(pt2dRect1,pt2dRect2,0.1);
	// 	pt2dRect1.y-=15;
	// 	pt2dRect2.y-=15;
	// 	CPolylineUtil::AddRectangle(pt2dRect1,pt2dRect2,0.1);
	AcGePoint2d pt2dRect3;
	pt2dRect3.x=50;
	pt2dRect3.y=253;
	AcGePoint2d pt2dRect4=pt2dRect3;
	pt2dRect4.x=d_x;
	pt2dRect4.y=pt2dRect3.y+15;
	CPolylineUtil::AddRectangle(pt2dRect3,pt2dRect4,0.1);
	pt2dRect3.y+=17;
	pt2dRect4.y+=17;
	CPolylineUtil::AddRectangle(pt2dRect3,pt2dRect4,0.1);
	// 	pt2dRect3.y+=18;
	// 	pt2dRect4.y+=15;
	// 	CPolylineUtil::AddRectangle(pt2dRect3,pt2dRect4,0.1);
	// 	pt2dRect3.y+=15;
	// 	pt2dRect4.y+=15;
	// 	CPolylineUtil::AddRectangle(pt2dRect3,pt2dRect4,0.1);
	AcGePoint3d pt3d_frame;
	pt3d_frame.z=0;
	pt3d_frame.x=0;
	pt3d_frame.y=0;
	AddFrameToPmt(pt3d_frame,d_x);
	//插入图块测试
	AcDbBlockReference* pRef = NULL ;
	AcGePoint3d pt_3d_header ;
	pt_3d_header.x=50-6;
	pt_3d_header.z=0;
	pt_3d_header.y=63;
	CString strblkname=_T("DT_LOC_HEADER_100");
	// 	if (mysis.InsertBlock(pRef, strblkname, pt_3d_header,FALSE,0,TRUE))
	// 	{
	// 
	// 		pRef->close() ;
	// 		pRef = NULL ;
	// 	}
	//AcDbBlockReference* pRef3 = NULL ;
	strblkname=_T("DT_PD_HEADER_000");
	pt_3d_header.y=2;
	if (mysis.InsertBlock(pRef, strblkname, pt_3d_header,FALSE,0,TRUE))
	{
		// 		CString strTag=_T("NAME");
		// 		CString str_name=_T("坡度");
		// 		mysis.SetAttValue(pRef,strTag,str_name,FALSE);
		pRef->close() ;
		pRef= NULL ;
	}
	pt_3d_header.y+=17;
	strblkname=_T("DT_QX_HEADER_000");
	if (mysis.InsertBlock(pRef, strblkname, pt_3d_header,FALSE,0,TRUE))
	{
		// 		CString strTag=_T("NAME");
		// 		CString str_name=_T("曲线半径");
		// 		mysis.SetAttValue(pRef,strTag,str_name,FALSE);
		pRef->close() ;
		pRef = NULL ;
	}
	AcDbBlockReference* pRef2 = NULL ;
	pt_3d_header.x=50-6;
	pt_3d_header.z=0;
	pt_3d_header.y=222;
	strblkname=_T("DT_LOC_HEADER_000");
	// 	if (mysis.InsertBlock(pRef2, strblkname, pt_3d_header,FALSE,0,TRUE))
	// 	{
	// 
	// 		pRef2->close() ;
	// 		pRef2 = NULL ;
	// 	}
	strblkname=_T("DT_PD_HEADER_100");
	pt_3d_header.y=287-17;
	if (mysis.InsertBlock(pRef2, strblkname, pt_3d_header,FALSE,0,TRUE))
	{
		// 		CString strTag=_T("NAME");
		// 		CString str_name=_T("坡度");
		// 		mysis.SetAttValue(pRef2,strTag,str_name,FALSE);
		pRef2->close() ;
		pRef2 = NULL ;
	}
	pt_3d_header.y-=17;
	strblkname=_T("DT_QX_HEADER_100");
	if (mysis.InsertBlock(pRef2, strblkname, pt_3d_header,FALSE,0,TRUE))
	{
		// 		CString strTag=_T("NAME");
		// 		CString str_name=_T("曲线半径");
		// 		mysis.SetAttValue(pRef2,strTag,str_name,FALSE);
		pRef2->close() ;
		pRef2 = NULL ;
	}

}

BOOL CDtpmt::DoubleToCString( double d_a,CString &str_a )
{
		if(fabs(d_a)<0.001)
		{
			str_a=_T("");
			return FALSE;
		}
		int i_k=0;
		int i_b=0;
		double d_c=0;
		d_c=d_a-(int)(d_a);
		int i_c=(int)(d_c*1000) ;
		i_k=(int)(d_a)/1000 ;
		i_b=(int)(d_a)%1000;
		str_a.Format(_T("K%02d+%03d.%03d"),i_k,i_b,i_c);
		//acutPrintf(_T("\n%s"),str_a);
		return TRUE;
}
BOOL CDtpmt::CStringToDouble( CString str_a,double &d_a)
{
	try
	{
		if(str_a==_T(""))
		{
			d_a=0;
			return FALSE;
		}
		//查找K或k，+,_T('.')的位置
		int i_indexK=-1;
		int i_indexPlus=-1;
		int i_indexP=-1;
		if(i_indexK=str_a.Find(_T('K'))==-1)
		{
			if((i_indexK=str_a.Find(_T('k')))==-1)
			{	
				d_a=0;
				return FALSE;
			}
		}
		if(i_indexPlus=str_a.Find(_T('+'))==-1)
		{
			d_a=0;
			return FALSE;
		}
		if(i_indexP=str_a.Find(_T('.'))==-1)
		{
			d_a=0;
			return FALSE;
		}	
		CString strKfw=str_a.Mid(i_indexK+1,i_indexPlus-1);
		CString strBfw=str_a.Mid(i_indexPlus+1,i_indexP-1-i_indexPlus);
		CString strDfw=str_a.Mid(i_indexP+1,str_a.GetLength()-i_indexP-1);
		double d_a=StrToIntA(strKfw)*1000+StrToIntA(strBfw)+StrToIntA(strDfw)/1000.000 ;
		return TRUE;
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);   
		return FALSE;
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误!in CStringToDouble"));
		return FALSE;
	}		
	
}

void CDtpmt::GetBlkType( CString str_device,CString &strblkname )
{
	strblkname=_T("");
	if(str_device.Left(2)==_T("AP"))
	{
		strblkname=_T("DT_AP_100");
	}
	else if(str_device.Left(2)==_T("JZ"))
	{
		strblkname=_T("DT_JZ_000");
	}
	else if(str_device.Left(2)==_T("FB"))
	{
		strblkname=_T("DT_YDQ_100");
	}
	else if(str_device.Left(2)==_T("VB"))
	{
		strblkname=_T("DT_YDQ_000");
	}
	else if(str_device.Left(2)==_T("IB"))
	{
		strblkname=_T("DT_YDQ_200");
	}
	else if(str_device==_T("人防门"))
	{
		strblkname=_T("DT_RFM_000");
	}
	else if(str_device.Left(3)==_T("LDR"))
	{
		strblkname=_T("DT_DHX_000");
	}
	else if(str_device.Left(1)==_T("Z")||str_device.Left(1)==_T("S")||str_device.Left(1)==_T("X"))
	{
		strblkname=_T("XHJ_DT_LH_A_Y_H_0");

	}
	else if(str_device.Left(1)==_T("F"))
	{
		strblkname=_T("XHJ_DT_ULH_A_Y_H_0");
	}
	else if(str_device==_T("方庄站"))
	{
		strblkname=_T("DT_STATION_JZ_00");
	}
	else
	{
		strblkname=_T("DT_AP_100");
	}

}
void CDtpmt::GetBlkType2( CString str_device,CString &strblkname )
{
	strblkname=_T("");
	if(str_device.Left(3)==_T("AP_"))
	{
		strblkname=_T("DT_")+str_device;
	}
	else if(str_device.Left(3)==_T("JZ_"))
	{
		strblkname=_T("DT_")+str_device;
	}
	else if(str_device.Left(4)==_T("YDQ_"))
	{
		strblkname=_T("DT_")+str_device;
	}
	else if(str_device.Left(3)==_T("DHX"))
	{
		strblkname=_T("DT_")+str_device;
	}
	else if(str_device==_T("RFM"))
	{
		strblkname=_T("DT_RFM");
	}
	else if(str_device.Left(3)==_T("DC_"))
	{
		strblkname=_T("DT_")+str_device;
	}
	else if(str_device.Left(4)==_T("JCB_"))
	{
		strblkname=_T("DT_")+str_device;
	}
	else if(str_device.Left(4)==_T("XHJ_"))
	{
		strblkname=_T("DT_")+str_device;
	}
	else if(str_device.Left(8)==_T("STATION_"))
	{
		strblkname=_T("DT_")+str_device ;
	}
	else if(str_device.Left(11)==_T("CABLEIMPORT"))
	{
		strblkname=_T("DT_")+str_device;
	}
	else if(str_device.Left(2)==_T("CD"))
	{
		strblkname=_T("DT_")+str_device;
	}
	else
	{
		//strblkname=_T("DT_AP_100");
	}

}

void CDtpmt::AddFrameToPmt( AcGePoint3d start_pt_3d,double d_width )
{

	AcGePoint3d base_pt3d=start_pt_3d;
	AcGePoint2d base_pt2d=CConvertUtil::ToPoint2d(base_pt3d);
	AcGePoint2d start_rectangle1_pt2d1=base_pt2d;
	double xls_len=d_width;
	AcGePoint2d start_rectangle1_pt2d2=base_pt2d;
// 	start_rectangle1_pt2d2.x+=(287+xls_len);
// 	start_rectangle1_pt2d2.y-=287;
	start_rectangle1_pt2d2.x+=(287+xls_len);
	start_rectangle1_pt2d2.y+=287;
	CPolylineUtil::AddRectangle(start_rectangle1_pt2d1,start_rectangle1_pt2d2,0.5);
	AcGePoint2d start_rectangle2_pt2d1=base_pt2d;
// 	start_rectangle2_pt2d1.x-=25;
// 	start_rectangle2_pt2d1.y+=5;
	start_rectangle2_pt2d1.x-=25;
	start_rectangle2_pt2d1.y-=5;
	AcGePoint2d start_rectangle2_pt2d2=base_pt2d;
	start_rectangle2_pt2d2.x+=(302+xls_len);
	start_rectangle2_pt2d2.y+=292;
	CPolylineUtil::AddRectangle(start_rectangle2_pt2d1,start_rectangle2_pt2d2,0);
	//创建圆形
	AcGePoint2d ptCenter_2d=start_rectangle1_pt2d1;
	ptCenter_2d.x-=12.5;
	ptCenter_2d.y+=36;
	CPolylineUtil::AddPolyCircle(ptCenter_2d,7.4,0);
	AcGePoint3d sign_point1=CConvertUtil::ToPoint3d(start_rectangle1_pt2d2);
// 	sign_point1.x+=-180+63;
 	sign_point1.y-=287;
	CString tuname;
	CString tuhao;
	CString riqi;
	CString program_name1;
	CString program_name2;
	CString strFilePath2 ;
	strFilePath2.Format(_T("%s\\support\\sign\\SIGN_6.dwg"), g_strBasePath) ;
	TCHAR *file_sign=(TCHAR *)strFilePath2.GetBuffer(strFilePath2.GetLength());
	TCHAR *bsign=_T("my_sign");
	AcDbObjectId blockid = CBlockUtil::InsertDwgBlockDef(file_sign,bsign,1);
	//CBlockUtil::InsertBlockRef(blockid,ptStart);
	AcDbObjectId blkRefId = CBlockUtil::InsertBlockRefWithAttribute(blockid,sign_point1);
	AcDbBlockReference *pBlkRef =NULL;
	if(acdbOpenObject(pBlkRef,blkRefId,AcDb::kForWrite)==Acad::eOk)
	{
		CBlockUtil::SetBlockRefAttribute(pBlkRef,_T("TUNAME_001"),tuname.GetString());
		CBlockUtil::SetBlockRefAttribute(pBlkRef,_T("TUHAO_001"),tuhao.GetString());
		CBlockUtil::SetBlockRefAttribute(pBlkRef,_T("RIQI_001"),riqi.GetString());
		CBlockUtil::SetBlockRefAttribute(pBlkRef,_T("PROGRAM_NAME_001"),program_name1.GetString());
		CBlockUtil::SetBlockRefAttribute(pBlkRef,_T("PROGRAM_NAME2_001"),program_name2.GetString());
		pBlkRef->close();
	}
}
void CDtpmt::AddFrameToDtsxt( AcGePoint3d start_pt_3d,double d_width )
{

	AcGePoint3d base_pt3d=start_pt_3d;
	AcGePoint2d base_pt2d=CConvertUtil::ToPoint2d(base_pt3d);
	AcGePoint2d start_rectangle1_pt2d1=base_pt2d;
	double xls_len=d_width;
	AcGePoint2d start_rectangle1_pt2d2=base_pt2d;
	start_rectangle1_pt2d2.x+=(287+xls_len);
	start_rectangle1_pt2d2.y-=400;
	CPolylineUtil::AddRectangle(start_rectangle1_pt2d1,start_rectangle1_pt2d2,0.5);
	AcGePoint2d start_rectangle2_pt2d1=base_pt2d;
	start_rectangle2_pt2d1.x-=25;
	start_rectangle2_pt2d1.y+=10;
	AcGePoint2d start_rectangle2_pt2d2=start_rectangle1_pt2d2;
	start_rectangle2_pt2d2.x+=10;
	start_rectangle2_pt2d2.y-=10;
	CPolylineUtil::AddRectangle(start_rectangle2_pt2d1,start_rectangle2_pt2d2,0);
	//创建圆形
	AcGePoint2d ptCenter_2d=start_rectangle1_pt2d2;
	ptCenter_2d.x=start_rectangle1_pt2d1.x;
	ptCenter_2d.x-=12.5;
	ptCenter_2d.y+=36;
	CPolylineUtil::AddPolyCircle(ptCenter_2d,7.4,0);
	AcGePoint3d sign_point1=CConvertUtil::ToPoint3d(start_rectangle1_pt2d2);
	//sign_point1.y-=400;
	CString tuname;
	CString tuhao;
	CString riqi;
	CString program_name1;
	CString program_name2;
	CString strFilePath2 ;
	strFilePath2.Format(_T("%s\\support\\sign\\SIGN_4.dwg"), g_strBasePath) ;
	TCHAR *file_sign=(TCHAR *)strFilePath2.GetBuffer(strFilePath2.GetLength());
	TCHAR *bsign=_T("my_sign");
	AcDbObjectId blockid = CBlockUtil::InsertDwgBlockDef(file_sign,bsign,1);
	//CBlockUtil::InsertBlockRef(blockid,ptStart);
	AcDbObjectId blkRefId = CBlockUtil::InsertBlockRefWithAttribute(blockid,sign_point1);
	AcDbBlockReference *pBlkRef =NULL;
	if(acdbOpenObject(pBlkRef,blkRefId,AcDb::kForWrite)==Acad::eOk)
	{
		CBlockUtil::SetBlockRefAttribute(pBlkRef,_T("DWGNAME"),tuname.GetString());
		CBlockUtil::SetBlockRefAttribute(pBlkRef,_T("DWGNUM"),tuhao.GetString());
		CBlockUtil::SetBlockRefAttribute(pBlkRef,_T("DATE"),riqi.GetString());
		CBlockUtil::SetBlockRefAttribute(pBlkRef,_T("PROJECTNAME"),program_name1.GetString());
		//CBlockUtil::SetBlockRefAttribute(pBlkRef,_T("PROGRAM_NAME2_001"),program_name2.GetString());
		pBlkRef->close();
	}
}
BOOL CDtpmt::FindSortNum( const vector<double> &d_vecSource,double d_dest, int &i_num )
{
	int j=0;
	for(j=0;j<d_vecSource.size();j++)
	{
		double d_temp=d_vecSource[j];
		if((d_dest-d_temp)<1.0)
		{
			i_num=j;
			return TRUE;
		}
	}
	i_num=j;
	return FALSE;
}

void CDtpmt::AddPodu( vector<double> &d_vechedata,vector<double> &d_vec_xposition,vector<double>&d_vecdestdata,vector<CString> &str_vecpolength,vector<CString>&str_vec_podu,vector<int>&i_vec_sx,bool up )
{
	CLineUtil myline;
	AcGePoint2d pt2d;
	pt2d.x=50;
	if(up==true)
	   pt2d.y=253;
	else
		pt2d.y=17;
	int iNum=0;
	int iNum0=0;
	for(int j=0;j<d_vecdestdata.size();j++)
	{
		double d_datatemp=d_vecdestdata[j];
		FindSortNum(d_vechedata,d_datatemp,iNum);
		acutPrintf(_T("\n%d"),iNum);
		if(iNum==iNum0)
		{
			iNum++;
		}
		if(iNum>=d_vechedata.size()-1)
		{
			iNum=d_vechedata.size()-1;
		}
		if(j==0)
		{
			pt2d.x=50;
		}
		else
		{
			pt2d.x=d_vec_xposition[iNum0];
		}
		double d_x=d_vec_xposition[iNum];
		CString strlibiao;
		DoubleToCString(d_vecdestdata[j],strlibiao);
		myline.drawpodu(pt2d,d_x,strlibiao,str_vecpolength[j],str_vec_podu[j],i_vec_sx[j]);
		iNum0=iNum;
	}

}
void CDtpmt::AddPodu_0( vector<double> &d_vechedata,vector<double> &d_vec_xposition,vector<double>&d_vecdestdata,vector<CString> &str_vecpolength,vector<CString>&str_vec_podu,vector<int>&i_vec_sx,bool up )
{
	CLineUtil myline;
	AcGePoint2d pt2d;
	pt2d.x=50;
	if(up==true)
		pt2d.y=253;
	else
		pt2d.y=17;
	int iNum=0;
	int iNum0=0;
	for(int j=0;j<d_vecdestdata.size();j++)
	{
		double d_datatemp=d_vecdestdata[j];
		FindSortNum(d_vechedata,d_datatemp,iNum);
		acutPrintf(_T("\n%d"),iNum);
		if(iNum==iNum0)
		{
			iNum++;
		}
		if(iNum>=d_vechedata.size()-1)
		{
			iNum=d_vechedata.size()-1;
		}
		if(j==0)
		{
			pt2d.x=50;
		}
		else
		{
			pt2d.x=d_vec_xposition[iNum0];
		}
		double d_x=d_vec_xposition[iNum];
		CString strlibiao;
		DoubleToCString(d_vecdestdata[j],strlibiao);
		myline.drawpodu(pt2d,d_x,strlibiao,str_vecpolength[j],str_vec_podu[j],i_vec_sx[j]);
		iNum0=iNum;
	}

}

void CDtpmt::AddQuXian( vector<double> &d_vechedata,vector<double> &d_vec_xposition,vector<double> &d_vecstart,vector<double> &d_vecend,vector<CString> &str_vecration,vector<int>&i_vec_at,bool up )
{
	CLineUtil myline;
	AcGePoint2d pt2d;
	pt2d.x=50;
	if(up==true)
		pt2d.y=268;
	else
		pt2d.y=2;
	int iNum=0;
	int iNum0=0;
	AcGePoint2d pt2dline1=pt2d;
	pt2dline1.y+=7.5;
	AcGePoint2d pt2dline2=pt2d;
	pt2dline2.y+=7.5;
	for(int j=0;j<d_vecstart.size();j++)
	{
		double d_datastart=d_vecstart[j];
		double d_dataend=d_vecend[j];
		acutPrintf(_T("\n start=%.3f \t,end=%.3f"),d_datastart,d_dataend);
		FindSortNum(d_vechedata,d_datastart,iNum0);
		FindSortNum(d_vechedata,d_dataend,iNum);
		//acutPrintf(_T("\n%d"),iNum);
		if(iNum<=iNum0)
		{
			iNum++;
		}
		if(iNum>=d_vechedata.size()-1)
		{
			iNum=d_vechedata.size()-1;
		}
		pt2d.x=d_vec_xposition[iNum0];
		pt2dline2.x=pt2d.x;
		double d_x=d_vec_xposition[iNum];
		while(d_x==pt2d.x)
		{
			iNum++;
			d_x=d_vec_xposition[iNum];
		}
		myline.AddPolyLine(pt2dline1,pt2dline2,0.1);
		CString strlibiaostart;
		DoubleToCString(d_datastart,strlibiaostart);
		CString strlibiaoend;
		DoubleToCString(d_dataend,strlibiaoend);
		myline.drawquxian(pt2d,d_x,strlibiaostart,strlibiaoend,str_vecration[j],i_vec_at[j]);
		//myline.drawpodu(pt2d,d_x,strlibiao,str_vecpolength[j],str_vec_podu[j],i_vec_sx[j]);
		pt2dline1.x=d_x;
		iNum0=iNum;
	}

}
void CDtpmt::AddQuXian_0( vector<double> &d_vechedata,vector<double> &d_vec_xposition,vector<double> &d_vecstart,vector<double> &d_vecend,vector<CString> &str_vecration,vector<int>&i_vec_at,bool up )
{
	CLineUtil myline;
	AcGePoint2d pt2d;
	pt2d.x=50;
	if(up==true)
		pt2d.y=268;
	else
		pt2d.y=2;
	int iNum=0;
	int iNum0=0;
	AcGePoint2d pt2dline1=pt2d;
	pt2dline1.y+=7.5;
	AcGePoint2d pt2dline2=pt2d;
	pt2dline2.y+=7.5;
	for(int j=0;j<d_vecstart.size();j++)
	{
		double d_datastart=d_vecstart[j];
		double d_dataend=d_vecend[j];
		acutPrintf(_T("\n start=%.3f \t,end=%.3f"),d_datastart,d_dataend);
		FindSortNum(d_vechedata,d_datastart,iNum0);
		FindSortNum(d_vechedata,d_dataend,iNum);
		//acutPrintf(_T("\n%d"),iNum);
		if(iNum<=iNum0)
		{
			iNum++;
		}
		if(iNum>=d_vechedata.size()-1)
		{
			iNum=d_vechedata.size()-1;
		}
		pt2d.x=d_vec_xposition[iNum0];
		pt2dline2.x=pt2d.x;
		double d_x=d_vec_xposition[iNum];
		while(d_x==pt2d.x)
		{
			iNum++;
			d_x=d_vec_xposition[iNum];
		}
		myline.AddPolyLine(pt2dline1,pt2dline2,0.1);
		CString strlibiaostart;
		DoubleToCString(d_datastart,strlibiaostart);
		CString strlibiaoend;
		DoubleToCString(d_dataend,strlibiaoend);
		myline.drawquxian(pt2d,d_x,strlibiaostart,strlibiaoend,str_vecration[j],i_vec_at[j]);
		//myline.drawpodu(pt2d,d_x,strlibiao,str_vecpolength[j],str_vec_podu[j],i_vec_sx[j]);
		pt2dline1.x=d_x;
		iNum0=iNum;
	}

}
void CDtpmt::drawzhantai( const AcGePoint2d pt2d,const double d_xend,CString strStation,int iType,bool up )
{
    CString strLayer=_T("PMT_ZT");
	AcGePoint2d pt2dStart=pt2d;
	AcGePoint2d pt2dEnd=pt2dStart;
	double d_hight=0;
	if(iType==0)
	{
		d_hight=16;
	}
	else 
	{
		d_hight=8;
	}
	pt2dEnd.x=d_xend;
	CLineUtil::AddRect2(pt2dStart,pt2dEnd,d_hight,0.1);
	//加入发车计时器，4个算
	CString strblkname=_T("DT_BUTTON_JJGB_000");
	AcDbBlockReference* pRef0=NULL;
	AcDbBlockReference* pRef2=NULL;
	AcGePoint2d pt2dInsert0=pt2dStart;
	pt2dInsert0.x+=8.6;
	pt2dInsert0.y-=5.5;
	if (mysis.InsertBlock(pRef0, strblkname,CConvertUtil::ToPoint3d(pt2dInsert0),FALSE,0,TRUE))
	{
		CString strTag=_T("NAME");
		CString str_name=_T("EMP1");
		mysis.SetAttValue(pRef0,strTag,str_name,FALSE);
		mysis.RotationEnt(pRef0,PI,CConvertUtil::ToPoint3d(pt2dInsert0));
		pRef0->setLayer(strLayer);
		pRef0->close() ;
		pRef0 = NULL ;
	}
	pt2dInsert0.y=pt2dStart.y-(d_hight-5.5);
	if (mysis.InsertBlock(pRef0, strblkname,CConvertUtil::ToPoint3d(pt2dInsert0),FALSE,0,TRUE))
	{
		CString strTag=_T("NAME");
		CString str_name=_T("EMP2");
		mysis.SetAttValue(pRef0,strTag,str_name,FALSE);
		AcGePoint2d pt2dMi1=pt2dInsert0;
		AcGePoint2d pt2dMi2=pt2dInsert0;
		pt2dMi2.y+=2;
		mysis.MirrEnt(pRef0,CConvertUtil::ToPoint3d(pt2dMi1),CConvertUtil::ToPoint3d(pt2dMi2));
		pRef0->close() ;
		pRef0->setLayer(strLayer);
		pRef0 = NULL ;
	}
	AcGePoint2d pt2dInsert1=pt2dEnd;
	pt2dInsert1.x-=8.6;
	pt2dInsert1.y-=5.5;
	if (mysis.InsertBlock(pRef2, strblkname,CConvertUtil::ToPoint3d(pt2dInsert1),FALSE,0,TRUE))
	{
		CString strTag=_T("NAME");
		CString str_name=_T("EMP3");
		mysis.SetAttValue(pRef2,strTag,str_name,FALSE);
		mysis.RotationEnt(pRef2,PI,CConvertUtil::ToPoint3d(pt2dInsert1));
		AcGePoint2d pt2dMi1=pt2dInsert1;
		AcGePoint2d pt2dMi2=pt2dInsert1;
		pt2dMi2.y+=2;
		mysis.MirrEnt(pRef2,CConvertUtil::ToPoint3d(pt2dMi1),CConvertUtil::ToPoint3d(pt2dMi2));
		pRef2->setLayer(strLayer);
		pRef2->close() ;
		pRef2 = NULL ;
	}
	pt2dInsert1.y=pt2dEnd.y-(d_hight-5.5);
	if (mysis.InsertBlock(pRef2, strblkname,CConvertUtil::ToPoint3d(pt2dInsert1),FALSE,0,TRUE))
	{
		CString strTag=_T("NAME");
		CString str_name=_T("EMP4");
		mysis.SetAttValue(pRef2,strTag,str_name,FALSE);
		pRef2->setLayer(strLayer);
		pRef2->close();
		pRef2 = NULL ;
	}
	//插入发车计时器
	AcGePoint2d pt2dJsq1=pt2dStart;
	strblkname=_T("DT_FCJSQ_000");
	pt2dJsq1.x+=4;
	pt2dJsq1.y-=3.4;
	if (mysis.InsertBlock(pRef2, strblkname,CConvertUtil::ToPoint3d(pt2dJsq1),FALSE,0,TRUE))
	{
		CString strTag=_T("NAME");
		CString str_name=_T("TDT1");
		mysis.SetAttValue(pRef2,strTag,str_name,FALSE);
 		AcGePoint2d pt2dRo=pt2dJsq1;
		pt2dRo.x+=1.5;
		pt2dRo.y+=1.5;
		mysis.RotationEnt(pRef2,PI,CConvertUtil::ToPoint3d(pt2dRo));
		pRef2->setLayer(strLayer);
		pRef2->close() ;
		pRef2 = NULL ;
	}
	AcGePoint2d pt2dJsq2=pt2dEnd;
	pt2dJsq2.x-=4;
	pt2dJsq2.y-=(d_hight-0.4);
	if (mysis.InsertBlock(pRef2, strblkname,CConvertUtil::ToPoint3d(pt2dJsq2),FALSE,0,TRUE))
	{
		CString strTag=_T("NAME");
		CString str_name=_T("TDT2");
		mysis.SetAttValue(pRef2,strTag,str_name,FALSE);
		pRef2->setLayer(strLayer);
		pRef2->close() ;
		pRef2 = NULL ;
	}
	//插入安全门
	strblkname=_T("DT_DOOR_000");
	AcGePoint2d pt2dDoor;
	pt2dDoor.x=(pt2dStart.x+d_xend)/2;
	pt2dDoor.y=pt2dStart.y-1;
	if (mysis.InsertBlock(pRef2, strblkname,CConvertUtil::ToPoint3d(pt2dDoor),FALSE,0,TRUE))
	{
		CString strTag=_T("NAME");
		CString str_name=_T("PSD1");
		mysis.SetAttValue(pRef2,strTag,str_name,FALSE);
		pRef2->setLayer(strLayer);
		pRef2->close() ;
		pRef2 = NULL ;
	}
	//插入安全门
	//strblkname=_T("DT_DOOR");
	AcGePoint2d pt2dDoor2;
	pt2dDoor2.x=(pt2dStart.x+d_xend)/2;
	pt2dDoor2.y=(pt2dStart.y-d_hight+1);
	if (mysis.InsertBlock(pRef2, strblkname,CConvertUtil::ToPoint3d(pt2dDoor2),FALSE,0,TRUE))
	{
		CString strTag=_T("NAME");
		CString str_name=_T("PSD2");
		mysis.SetAttValue(pRef2,strTag,str_name,FALSE);
		mysis.RotationEnt(pRef2,PI,CConvertUtil::ToPoint3d(pt2dDoor2));
		pRef2->setLayer(strLayer);
		pRef2->close() ;
		pRef2 = NULL ;
	}
	//插入停车表
	AcGePoint2d pt2dTcb=pt2dStart;
	pt2dTcb.x+=2;
	strblkname=_T("DT_STOP_000");
	if (mysis.InsertBlock(pRef2, strblkname,CConvertUtil::ToPoint3d(pt2dTcb),FALSE,0,TRUE))
	{
		CString strTag=_T("NUM");
		CString str_name=_T("6");
		mysis.SetAttValue(pRef2,strTag,str_name,FALSE);
		mysis.RotationEnt(pRef2,PI,CConvertUtil::ToPoint3d(pt2dTcb));
		pRef2->setLayer(strLayer);
		pRef2->close() ;
		pRef2 = NULL ;
	}
	AcGePoint2d pt2dTcb2=pt2dEnd;
	pt2dTcb2.x-=2;
	pt2dTcb2.y-=d_hight;
	strblkname=_T("DT_STOP_000");
	if (mysis.InsertBlock(pRef2, strblkname,CConvertUtil::ToPoint3d(pt2dTcb2),FALSE,0,TRUE))
	{
		CString strTag=_T("NUM");
		CString str_name=_T("6");
		mysis.SetAttValue(pRef2,strTag,str_name,FALSE);
		//mysis.RotationEnt(pRef2,PI,CConvertUtil::ToPoint3d(pt2dTcb));
		pRef2->setLayer(strLayer);
		pRef2->close() ;
		pRef2 = NULL ;
	}
}
void CDtpmt::drawzhantai2( const AcGePoint2d pt2d,const double d_xend,CString strStation,int iType,bool up )
{
	CString strLayer=_T("PMT_ZT");
	AcGePoint2d pt2dStart=pt2d;
	AcGePoint2d pt2dEnd=pt2dStart;
	double d_hight=0;
	if(iType==0)
	{
		d_hight=16;
	}
	else 
	{
		d_hight=8;
	}
	pt2dEnd.x=d_xend;
	CLineUtil::AddRect2(pt2dStart,pt2dEnd,d_hight,0.1);
	//加入紧急关闭按钮，2个算
	CString strblkname=_T("DT_BUTTON_JJGB_000");
	AcDbBlockReference* pRef0=NULL;
	AcDbBlockReference* pRef2=NULL;
	AcGePoint2d pt2dInsert0=pt2dStart;
	if(up==true)
	{
		pt2dInsert0.x+=8.6;
		pt2dInsert0.y-=2.5;
		if (mysis.InsertBlock(pRef0, strblkname,CConvertUtil::ToPoint3d(pt2dInsert0),FALSE,0,TRUE))
		{
			CString strTag=_T("NAME");
			CString str_name=_T("EMP1");
			mysis.SetAttValue(pRef0,strTag,str_name,FALSE);
			AcGePoint2d pt2dMi1=pt2dInsert0;
			AcGePoint2d pt2dMi2=pt2dInsert0;
			pt2dMi2.y+=2;
			mysis.MirrEnt(pRef0,CConvertUtil::ToPoint3d(pt2dMi1),CConvertUtil::ToPoint3d(pt2dMi2));
			pRef0->setLayer(strLayer);
			pRef0->close() ;
			pRef0 = NULL ;
		}
	}
	else
	{
		AcGePoint2d pt2dInsert0=pt2dStart;
		pt2dInsert0.x+=8.6;
		pt2dInsert0.y-=(d_hight-2.5);
		if (mysis.InsertBlock(pRef0, strblkname,CConvertUtil::ToPoint3d(pt2dInsert0),FALSE,0,TRUE))
		{
			CString strTag=_T("NAME");
			CString str_name=_T("EMP2");
			mysis.SetAttValue(pRef0,strTag,str_name,FALSE);
			mysis.RotationEnt(pRef0,PI,CConvertUtil::ToPoint3d(pt2dInsert0));
			pRef0->setLayer(strLayer);
			pRef0->close() ;
			pRef0 = NULL ;
		}
	}
	AcGePoint2d pt2dInsert1=pt2dEnd;
	if(up==true)
	{
		pt2dInsert1.x-=8.6;
		pt2dInsert1.y-=2.5;
		if (mysis.InsertBlock(pRef2, strblkname,CConvertUtil::ToPoint3d(pt2dInsert1),FALSE,0,TRUE))
		{
			CString strTag=_T("NAME");
			CString str_name=_T("EMP3");
			mysis.SetAttValue(pRef2,strTag,str_name,FALSE);
			pRef2->setLayer(strLayer);
			pRef2->close();
			pRef2 = NULL ;
		}

	}
	else
	{
		AcGePoint2d pt2dInsert1=pt2dEnd;
		pt2dInsert1.x-=8.6;
		pt2dInsert1.y-=(d_hight-2.5);
		if (mysis.InsertBlock(pRef2, strblkname,CConvertUtil::ToPoint3d(pt2dInsert1),FALSE,0,TRUE))
		{
			CString strTag=_T("NAME");
			CString str_name=_T("EMP4");
			mysis.SetAttValue(pRef2,strTag,str_name,FALSE);
			mysis.RotationEnt(pRef2,PI,CConvertUtil::ToPoint3d(pt2dInsert1));
			AcGePoint2d pt2dMi1=pt2dInsert1;
			AcGePoint2d pt2dMi2=pt2dInsert1;
			pt2dMi2.y+=2;
			mysis.MirrEnt(pRef2,CConvertUtil::ToPoint3d(pt2dMi1),CConvertUtil::ToPoint3d(pt2dMi2));
			pRef2->setLayer(strLayer);
			pRef2->close() ;
			pRef2 = NULL ;
		}

	}
	AcGePoint2d pt2dDoor2;
	strblkname=_T("DT_DOOR_000");
	if(up==true)
	{
		pt2dDoor2.x=(pt2dStart.x+d_xend)/2;
		pt2dDoor2.y=(pt2dStart.y-d_hight+1);
		if (mysis.InsertBlock(pRef2, strblkname,CConvertUtil::ToPoint3d(pt2dDoor2),FALSE,0,TRUE))
		{
			CString strTag=_T("NAME");
			CString str_name=_T("PSD1");
			mysis.SetAttValue(pRef2,strTag,str_name,FALSE);
			mysis.RotationEnt(pRef2,PI,CConvertUtil::ToPoint3d(pt2dDoor2));
			pRef2->setLayer(strLayer);
			pRef2->close() ;
			pRef2 = NULL ;
		}

	}
	else
	{
		pt2dDoor2.x=(pt2dStart.x+d_xend)/2;
		pt2dDoor2.y=pt2dStart.y-1;
		if (mysis.InsertBlock(pRef2, strblkname,CConvertUtil::ToPoint3d(pt2dDoor2),FALSE,0,TRUE))
		{
			CString strTag=_T("NAME");
			CString str_name=_T("PSD2");
			mysis.SetAttValue(pRef2,strTag,str_name,FALSE);
			//mysis.RotationEnt(pRef2,PI,CConvertUtil::ToPoint3d(pt2dDoor2));
			pRef2->setLayer(strLayer);
			pRef2->close() ;
			pRef2 = NULL ;
		}

	}
	//插入发车计时器
	AcGePoint2d pt2dJsq1;
	strblkname=_T("DT_FCJSQ_000");
	if(up==true)
	{
		pt2dJsq1=pt2dStart;
		pt2dJsq1.x+=2;
		pt2dJsq1.y-=(d_hight-0.45);
		if (mysis.InsertBlock(pRef2, strblkname,CConvertUtil::ToPoint3d(pt2dJsq1),FALSE,0,TRUE))
		{
			CString strTag=_T("NAME");
			CString str_name=_T("TDT1");
			mysis.SetAttValue(pRef2,strTag,str_name,FALSE);
			AcGePoint2d pt2dRo=pt2dJsq1;
			pt2dRo.x+=1.5;
			pt2dRo.y+=1.5;
			mysis.RotationEnt(pRef2,PI,CConvertUtil::ToPoint3d(pt2dRo));
			pRef2->setLayer(strLayer);
			pRef2->close() ;
			pRef2 = NULL ;
		}
	}
	else
	{
		pt2dJsq1=pt2dEnd;
		pt2dJsq1.x-=4;
		pt2dJsq1.y-=3.55;
		if (mysis.InsertBlock(pRef2, strblkname,CConvertUtil::ToPoint3d(pt2dJsq1),FALSE,0,TRUE))
		{
			CString strTag=_T("NAME");
			CString str_name=_T("TDT2");
			mysis.SetAttValue(pRef2,strTag,str_name,FALSE);
			pRef2->setLayer(strLayer);
			pRef2->close() ;
			pRef2 = NULL ;
		}

	}
	//插入停车标
	AcGePoint2d pt2dTcb2;
	strblkname=_T("DT_STOP_000");
	if(up==true)
	{
		pt2dTcb2=pt2dStart;
		pt2dTcb2.x+=2;
		pt2dTcb2.y-=d_hight;	
		if (mysis.InsertBlock(pRef2, strblkname,CConvertUtil::ToPoint3d(pt2dTcb2),FALSE,0,TRUE))
		{
			CString strTag=_T("NUM");
			CString str_name=_T("6");
			mysis.SetAttValue(pRef2,strTag,str_name,FALSE);
			//mysis.RotationEnt(pRef2,PI,CConvertUtil::ToPoint3d(pt2dTcb));
			pRef2->setLayer(strLayer);
			pRef2->close() ;
			pRef2 = NULL ;
		}
	}
	else
	{
		pt2dTcb2=pt2dEnd;
		pt2dTcb2.x-=2;
		if (mysis.InsertBlock(pRef2, strblkname,CConvertUtil::ToPoint3d(pt2dTcb2),FALSE,0,TRUE))
		{
			CString strTag=_T("NUM");
			CString str_name=_T("6");
			mysis.SetAttValue(pRef2,strTag,str_name,FALSE);
			mysis.RotationEnt(pRef2,PI,CConvertUtil::ToPoint3d(pt2dTcb2));
			pRef2->setLayer(strLayer);
			pRef2->close() ;
			pRef2 = NULL ;
		}

	}
}

void CDtpmt::AddZhantai( vector<CString> &str_vec_deviceType_a,vector<double> &d_veclibiao_a,vector<double> &d_vec_libiao_he,vector<double> &d_vecx,vector<double> &d_veczhantai_a,vector<int> &i_vecType)
{
	for(int j=0;j<str_vec_deviceType_a.size();j++)
	{
		if(str_vec_deviceType_a[j].Left(8)==_T("STATION_"))
		{
			double d_temp=d_veclibiao_a[j];
			int iNum=-1;
			FindSortNum(d_veczhantai_a,d_temp,iNum);
			if(iNum>=0)
			{
				double d_zt1=d_veczhantai_a[iNum-1];
				double d_zt2=d_veczhantai_a[iNum];
				int index1=0;
				int index2=0;
				FindSortNum(d_vec_libiao_he,d_zt1,index1);
				FindSortNum(d_vec_libiao_he,d_zt2,index2);
				AcGePoint2d pt2dZT;
				//站台在中间
				pt2dZT.x=d_vecx[index1];
				if(i_vecType[j]==0)
				{
					pt2dZT.y=151.5;
					 drawzhantai(pt2dZT,d_vecx[index2],_T(""),0,0);
				}
				else
				{
					pt2dZT.y=166+20;
					drawzhantai2(pt2dZT,d_vecx[index2],_T(""),1,true);
					pt2dZT.y=121-12;
					drawzhantai2(pt2dZT,d_vecx[index2],_T(""),1,false);				
				}
				//pt2dZT.y=151.5;
			}
		}

	}

}

BOOL CDtpmt::GetHightBlkAndLibiao( const int iOnline,double &d_hight_blk)
{
	if(iOnline>7||iOnline<1)
	{
		d_hight_blk=0;
		return FALSE;
	}
	if(iOnline==2)
	{
		d_hight_blk=166;
	}
	else if(iOnline==6)
	{
		d_hight_blk=121;
	}
	else if(iOnline==3)
	{
		d_hight_blk=151;
	}
	else if(iOnline==5)
	{
		d_hight_blk=136;
	}
	else if(iOnline==4)
	{
		d_hight_blk=143.5;
	}
	else if(iOnline==1)
	{
		d_hight_blk=181;
	}
	else if(iOnline==7)
	{
		d_hight_blk=106;
	}
	else 
	{
		d_hight_blk=0;
	}
	return TRUE;
}
void CDtpmt::Add_Device_00( double d_x,double dhight_blk,double dhight_libiao,CString str_name,CString str_libiao ,CString str_libiao2,int i_upordown,CString strblkname/*=_T("DT_AP_100")*/,int ncolorindex/*=7*/ )
{
	AcDbObjectId  idTextStyleTitle, idTextStyleContent, idTextStyleChs;
	idTextStyleTitle = mysis.CreateTextStyleByFileName(_T("zhffontTitle"), _T("txt.shx"), _T("hztxt2.shx"), 4, 0.7) ;	
	idTextStyleContent = mysis.CreateTextStyleByFileName(_T("zhffontContent"), _T("txt.shx"), _T("hztxt2.shx"), 3, 0.7) ;
	idTextStyleChs = mysis.CreateTextStyle(_T("CHINESE"), _T("Times New Roman"), _T(""), 2.5, 0.8) ;
	//2?è?í??é2aê?
	AcDbBlockReference* pRef = NULL ;
	AcGePoint3d pt_3d_header ;
	//CString strBlkName ; 
	//±íí·
	pt_3d_header.y = dhight_blk;
	if(strblkname==_T("DT_AP_100"))
	{
		pt_3d_header.y+=10;
		//?ìé?
		ncolorindex=3;
		mysis.Set_Layer(_T(""), _T("PMT_AP")) ;
	}
	else if(strblkname.Left(5)==_T("DT_JZ"))
	{
		//??é?
		ncolorindex=2;
		mysis.Set_Layer(_T(""), _T("PMT_JZ")) ;
	}
	else if(strblkname.Left(6)==_T("DT_YDQ"))
	{
		//oìé?
		ncolorindex=1;
		mysis.Set_Layer(_T(""), _T("PMT_YDQ")) ;
	}
	else if(strblkname.Left(5)==_T("DT_CD"))
	{
		ncolorindex=1;
		mysis.Set_Layer(_T(""),_T("PMT_CD"));
	}
	else if(strblkname.Left(6)==_T("DT_JCB"))
	{
		ncolorindex=1;
		if(i_upordown==0)
		{
			pt_3d_header.y-=5;
		}
		else if(i_upordown==1)
		{
			pt_3d_header.y+=5;
		}
		else
		{
			acutPrintf(_T("\n ?ˉ3?±ê·??òê?è??μ2???￡????üè?0,1"));
		}
		mysis.Set_Layer(_T(""),_T("PMT_JCB"));
	}
	else if(strblkname.Left(6)==_T("DT_RFM"))
	{
		//?ìé?
		pt_3d_header.y+=8;
		ncolorindex=7;
		mysis.Set_Layer(_T(""), _T("PMT_RFM")) ;
	}
	else if(strblkname.Left(6)==_T("DT_DHX"))
	{
		//?ìà?é?
		ncolorindex=4;
		mysis.Set_Layer(_T(""), _T("PMT_DHX")) ;
	}
	else if(strblkname==_T("XHJ_DT_ULH_A_Y_H_0"))
	{
		//pt_3d_header.y+=5;
		//?ìé?
		//ncolorindex=3;
		mysis.Set_Layer(_T(""), _T("PMT_XHJ")) ;
	}
	else if(strblkname==_T("XHJ_DT_LH_A_Y_H_0"))
	{
		//pt_3d_header.y+=5;
		mysis.Set_Layer(_T(""), _T("PMT_XHJ")) ;
	}
	else if(strblkname.Left(10)==_T("DT_STATION"))
	{
		if(i_upordown==0)
		{
			//??·?
			pt_3d_header.y=54;

		}
		else 
		{
			//é?·?
			pt_3d_header.y=233;
		}
		pt_3d_header.y=233;
		mysis.Set_Layer(_T(""), _T("PMT_STATION")) ;
	}
	else if(strblkname.Left(14)==_T("DT_CABLEIMPORT"))
	{
		pt_3d_header.y+=42;
		mysis.Set_Layer(_T(""), _T("PMT_CABLEIMPORT")) ;
	}
	else
	{
		//?ìé?
		ncolorindex=6;
		mysis.Set_Layer(_T(""), _T("PMT_OTHERS")) ;
	}
	pt_3d_header.x=d_x;
	pt_3d_header.z=0;
	if (mysis.InsertBlock(pRef, strblkname, pt_3d_header,FALSE,0,TRUE))
	{
		CString strTag=_T("NAME");
		if(strblkname.Left(6)==_T("DT_XHJ"))
		{
			strTag=_T("XHJNAME");
			if(i_upordown==0)
			{
				mysis.RotationEnt(pRef,PI,pt_3d_header);
			}
		}
		if(strblkname.Left(5)==_T("DT_CD"))
		{
			if(i_upordown==0)
			{
				//??è?3ˉóò
			}
			else if(i_upordown==1)
			{
				//Dy×a180?è
				mysis.RotationEnt(pRef,PI,pt_3d_header);
			}
			else
			{
				acutPrintf(_T("\n 3μμμ·??òê?è??μ2???￡????üè?0,1"));
			}
		}
		if(strblkname.Left(6)==_T("DT_JCB"))
		{
			// 			if(i_upordown==0)
			// 			{
			// 				pt_3d_header.y-=5;
			// 			}
			// 			else if(i_upordown==1)
			// 			{
			// 				pt_3d_header.y+=5;
			// 			}
			// 			else
			// 			{
			// 				acutPrintf(_T("\n ?ˉ3?±ê·??òê?è??μ2???￡????üè?0,1"));
			// 			}
		}
		if(strblkname.Left(6)==_T("DT_DC_"))
		{
			strTag=_T("BIANHAO");
			if(i_upordown==5)
			{

			}
			else if(i_upordown==7)
			{
				mysis.RotationEnt(pRef,PI,pt_3d_header);

			}
			else if(i_upordown==6)
			{
				AcGePoint3d pt_3d_mirrY=pt_3d_header;
				pt_3d_mirrY.y+=4;
				mysis.MirrEnt(pRef,pt_3d_header,pt_3d_mirrY);

			}
			else if(i_upordown==8)
			{
				AcGePoint3d pt_3d_mirrY=pt_3d_header;
				pt_3d_mirrY.y+=4;
				mysis.MirrEnt(pRef,pt_3d_header,pt_3d_mirrY);
				mysis.RotationEnt(pRef,PI,pt_3d_header);
			}
		}
		if(strblkname.Left(6)!=_T("DT_RFM")&&(strblkname.Left(14)!=_T("DT_CABLEIMPORT")))
		{
			mysis.SetAttValue(pRef,strTag,str_name,FALSE);
			mysis.SetAttColor(pRef,strTag,ncolorindex);
			pRef->setLayer(strblkname.Left(6));
			//if(strblkname.Left(6)==_T())
		}
		if(strblkname.Left(10)==_T("DT_STATION"))
		{
			CString strQLB ;
			CString strBLB;
			int iLoc = 0 ;
			iLoc = str_libiao.Find(_T("+"), 0) ;
			if (iLoc!=-1)
			{
				strQLB = str_libiao.Mid(0, iLoc) ;
				strBLB=  str_libiao.Mid(iLoc,str_libiao.GetLength()+1);
				mysis.SetAttValue(pRef,_T("QLB"),strQLB,FALSE);
				mysis.SetAttValue(pRef,_T("BLB"),strBLB,FALSE);
			}	
		}
		pRef->setLayer(_T("pmt"));
		pRef->close() ;
		pRef = NULL ;
	}
	// 	if(strblkname.Left(10)!=_T("DT_STATION"))
	// 	{
	// 		//??·?ê?oí′óìúéè±?1?à?±ê·?ê?ò???￡?
	// 		AcGePoint3d pt_3d_insert_text(d_x,dhight_libiao+13,0);
	// 		AcGePoint3d pt_3d_insert_text2(d_x,dhight_libiao,0);
	// 		if(str_libiao2!=_T(""))
	// 		{
	// 			pt_3d_insert_text.x-=2;
	// 			mysis.AddText(str_libiao, pt_3d_insert_text, pt_3d_insert_text, AcDb::kTextLeft, AcDb::kTextVertMid, 3, 0.55, PI/2,ncolorindex,idTextStyleChs);
	// 			pt_3d_insert_text.x+=4;
	// 			mysis.AddText(str_libiao2, pt_3d_insert_text, pt_3d_insert_text, AcDb::kTextLeft, AcDb::kTextVertMid, 3, 0.55, PI/2,ncolorindex,idTextStyleChs);
	// 			mysis.AddText(str_name, pt_3d_insert_text2, pt_3d_insert_text2, AcDb::kTextLeft, AcDb::kTextVertMid, 3, 0.55, PI/2,ncolorindex,idTextStyleChs);
	// 		}
	// 		else
	// 		{
	// 			mysis.AddText(str_libiao, pt_3d_insert_text, pt_3d_insert_text, AcDb::kTextLeft, AcDb::kTextVertMid, 3, 0.55, PI/2,ncolorindex,idTextStyleChs);
	// 			mysis.AddText(str_name, pt_3d_insert_text2, pt_3d_insert_text2, AcDb::kTextLeft, AcDb::kTextVertMid, 3, 0.55, PI/2,ncolorindex,idTextStyleChs);
	// 		}
	// 	}	
	if(strblkname.Left(10)!=_T("DT_STATION"))
	{
		//??·?ê?oí′óìúéè±?1?à?±ê·?ê?ò???￡?
		AcGePoint3d pt_3d_insert_text(d_x,dhight_libiao+20,0);
		AcGePoint3d pt_3d_insert_text2(d_x,dhight_libiao,0);
		AcGePoint3d pt_3d_insert_start(d_x+2,dhight_libiao+18,0);
		AcGePoint3d pt_3d_insert_End(d_x+2,dhight_libiao,0);
		str_name =_T("(")+str_name+_T(")");
		if(str_libiao2!=_T(""))
		{
			//pt_3d_insert_text.x-=2;
			mysis.AddAcDb2dPolyline(pt_3d_insert_start,pt_3d_insert_End,0.1);
			mysis.AddText(str_name, pt_3d_insert_text, pt_3d_insert_text, AcDb::kTextLeft, AcDb::kTextVertMid, 2.5, 0.8, PI/2,ncolorindex,idTextStyleChs);
			mysis.AddText(str_libiao, pt_3d_insert_text2, pt_3d_insert_text2, AcDb::kTextLeft, AcDb::kTextVertMid, 2.5, 0.8, PI/2,ncolorindex,idTextStyleChs);
			pt_3d_insert_text2.x+=4;
			mysis.AddText(str_libiao2, pt_3d_insert_text2, pt_3d_insert_text2, AcDb::kTextLeft, AcDb::kTextVertMid, 2.5, 0.8, PI/2,ncolorindex,idTextStyleChs);

		}
		else
		{
			mysis.AddAcDb2dPolyline(pt_3d_insert_start,pt_3d_insert_End,0.1);
			mysis.AddText(str_libiao, pt_3d_insert_text2, pt_3d_insert_text2, AcDb::kTextLeft, AcDb::kTextVertMid, 2.5, 0.8, PI/2,ncolorindex,idTextStyleChs);
			mysis.AddText(str_name, pt_3d_insert_text, pt_3d_insert_text, AcDb::kTextLeft, AcDb::kTextVertMid, 2.5, 0.8, PI/2,ncolorindex,idTextStyleChs);
		}
	}	
}
void CDtpmt::Add_Device_01( double d_x,double dhight_blk,double dhight_libiao,CString str_name,CString str_libiao ,CString str_libiao2,int i_upordown,CString strblkname/*=_T("DT_AP_100")*/,int ncolorindex/*=7*/ )
{
	AcDbObjectId  idTextStyleTitle, idTextStyleContent, idTextStyleChs;
	idTextStyleTitle = mysis.CreateTextStyleByFileName(_T("zhffontTitle"), _T("txt.shx"), _T("hztxt2.shx"), 4, 0.7) ;	
	idTextStyleContent = mysis.CreateTextStyleByFileName(_T("zhffontContent"), _T("txt.shx"), _T("hztxt2.shx"), 3, 0.7) ;
	idTextStyleChs = mysis.CreateTextStyle(_T("CHINESE"), _T("Times New Roman"), _T(""), 2.5, 0.8) ;
	//2?è?í??é2aê?
	AcDbBlockReference* pRef = NULL ;
	AcGePoint3d pt_3d_header ;
	//CString strBlkName ; 
	//±íí·
	pt_3d_header.y = dhight_blk;
	pt_3d_header.x=d_x;
	pt_3d_header.z=0;
	if(strblkname.Left(5)==_T("DT_AP"))
	{
		pt_3d_header.y-=10;
		ncolorindex=3;
		mysis.Set_Layer(_T(""), _T("PMT_AP")) ;
	}
	else if(strblkname.Left(5)==_T("DT_JZ"))
	{
		ncolorindex=2;
		mysis.Set_Layer(_T(""), _T("PMT_JZ")) ;
	}
	else if(strblkname.Left(6)==_T("DT_YDQ"))
	{
		ncolorindex=1;
		mysis.Set_Layer(_T(""), _T("PMT_YDQ")) ;
	}
	else if(strblkname.Left(10)==_T("DT_STATION"))
	{
		if(i_upordown==0)
		{
			//??·?
			pt_3d_header.y=54;

		}
		else 
		{
			//é?·?
			pt_3d_header.y=233;
		}
		pt_3d_header.y=54;
		mysis.Set_Layer(_T(""), _T("PMT_STATION")) ;
	}
	else if(strblkname.Left(5)==_T("DT_CD"))
	{
		ncolorindex=1;
		mysis.Set_Layer(_T(""),_T("PMT_CD"));
	}
	else if(strblkname.Left(6)==_T("DT_RFM"))
	{
		pt_3d_header.y-=8;
		ncolorindex=7;
		mysis.Set_Layer(_T(""), _T("PMT_RFM")) ;
	}
	else if(strblkname.Left(6)==_T("DT_DHX"))
	{
		ncolorindex=4;
		mysis.Set_Layer(_T(""), _T("PMT_DHX")) ;
	}
	else if(strblkname==_T("XHJ_DT_ULH_A_Y_H_0"))
	{
		//pt_3d_header.y-=5;
		//?ìé?
		ncolorindex=3;
		mysis.Set_Layer(_T(""), _T("PMT_XHJ")) ;
	}
	else if(strblkname.Left(6)==_T("DT_JCB"))
	{
		ncolorindex=1;
		if(i_upordown==0)
		{
			pt_3d_header.y-=5;
		}
		else if(i_upordown==1)
		{
			pt_3d_header.y+=5;
		}
		else
		{
			acutPrintf(_T("\n ?ˉ3?±ê·??òê?è??μ2???￡????üè?0,1"));
		}
		mysis.Set_Layer(_T(""),_T("PMT_JCB"));
	}
	else if(strblkname==_T("XHJ_DT_LH_A_Y_H_0"))
	{
		//pt_3d_header.y-=5;
		mysis.Set_Layer(_T(""), _T("PMT_XHJ")) ;
	}
	else if(strblkname.Left(14)==_T("DT_CABLEIMPORT"))
	{
		pt_3d_header.y-=42;
		mysis.Set_Layer(_T(""), _T("PMT_CABLEIMPORT")) ;

	}
	else
	{
		ncolorindex=6;
		mysis.Set_Layer(_T(""), _T("PMT_OTHERS")) ;
	}
	if (mysis.InsertBlock(pRef, strblkname, pt_3d_header,FALSE,0,TRUE))
	{
		CString strTag=_T("NAME");
		if(strblkname.Left(6)==_T("DT_XHJ"))
		{
			strTag=_T("XHJNAME");
			if(i_upordown==0)
			{
				mysis.RotationEnt(pRef,PI,pt_3d_header);
				//mysis.SetAttValue(pRef,strTag,str_name,FALSE);
				//mysis.SetAttTextAlignMode(pRef,strTag,AcDb::kTextRight,AcDb::kTextVertMid);
			}
		}
		if(strblkname.Left(10)==_T("DT_STATION"))
		{
			//Dy×a180?è
			mysis.RotationEnt(pRef,PI,pt_3d_header);

		}
		if(strblkname.Left(5)==_T("DT_CD"))
		{
			if(i_upordown==0)
			{
				//??è?3ˉóò
			}
			else if(i_upordown==1)
			{
				//Dy×a180?è
				mysis.RotationEnt(pRef,PI,pt_3d_header);
			}
			else
			{
				acutPrintf(_T("\n 3μμμ·??òê?è??μ2???￡????üè?0,1"));
			}
		}
		if(strblkname.Left(6)==_T("DT_JCB"))
		{
			// 			if(i_upordown==0)
			// 			{
			// 				pt_3d_header.y-=5;
			// 			}
			// 			else if(i_upordown==1)
			// 			{
			// 				pt_3d_header.y+=5;
			// 			}
			// 			else
			// 			{
			// 				acutPrintf(_T("\n ?ˉ3?±ê·??òê?è??μ2???￡????üè?0,1"));
			// 			}
		}
		if(strblkname.Left(5)==_T("DT_JZ"))
		{
			//é?DD??μ????á????DèòaDy×a180?è
			mysis.RotationEnt(pRef,PI,pt_3d_header);
			pRef->setLayer(_T("PMT_JZ"));
			//mysis.Set_Layer(_T(""), _T("PMT_CABLEIMPORT")) ;
		}
		if(strblkname.Left(6)==_T("DT_DC_"))
		{
			strTag=_T("BIANHAO");
			if(i_upordown==5)
			{

			}
			else if(i_upordown==7)
			{
				mysis.RotationEnt(pRef,PI,pt_3d_header);

			}
			else if(i_upordown==6)
			{
				AcGePoint3d pt_3d_mirrY=pt_3d_header;
				pt_3d_mirrY.y+=4;
				mysis.MirrEnt(pRef,pt_3d_header,pt_3d_mirrY);

			}
			else if(i_upordown==8)
			{
				AcGePoint3d pt_3d_mirrY=pt_3d_header;
				pt_3d_mirrY.y+=4;
				mysis.MirrEnt(pRef,pt_3d_header,pt_3d_mirrY);
				mysis.RotationEnt(pRef,PI,pt_3d_header);
			}
			pRef->setLayer(_T("PMT_DC"));
		}
		if(strblkname.Left(6)!=_T("DT_RFM")&&(strblkname.Left(14)!=_T("DT_CABLEIMPORT")))
		{
			mysis.SetAttValue(pRef,strTag,str_name,FALSE);
			mysis.SetAttColor(pRef,strTag,ncolorindex);
			//pRef->setLayer(_T("PMT_OTHERS"));
		}
		if(strblkname.Left(5)==_T("DT_AP"))
		{
			mysis.RotationEnt(pRef,PI,pt_3d_header);
		}	
		//pRef->setLayer(_T("pmt"));
		pRef->close() ;
		pRef = NULL ;
	}
	// 	if(strblkname.Left(10)!=_T("DT_STATION"))
	// 	{
	// 		//??·?ê?oí′óìúéè±?1?à?±ê·?ê?ò???
	// 		AcGePoint3d pt_3d_insert_text(d_x,dhight_libiao-31,0);
	// 		AcGePoint3d pt_3d_insert_text2(d_x,dhight_libiao-13,0);
	// 		if(str_libiao2!=_T(""))
	// 		{
	// 			pt_3d_insert_text.x-=2;
	// 			mysis.AddText(str_libiao, pt_3d_insert_text, pt_3d_insert_text, AcDb::kTextLeft, AcDb::kTextVertMid, 3, 0.55, PI/2,ncolorindex,idTextStyleChs) ;
	// 			pt_3d_insert_text.x+=4;
	// 			mysis.AddText(str_libiao2, pt_3d_insert_text, pt_3d_insert_text, AcDb::kTextLeft, AcDb::kTextVertMid, 3, 0.55, PI/2,ncolorindex,idTextStyleChs) ;
	// 			mysis.AddText(str_name, pt_3d_insert_text2, pt_3d_insert_text2, AcDb::kTextLeft, AcDb::kTextVertMid, 3, 0.55, PI/2,ncolorindex,idTextStyleChs);	
	// 		}
	// 		else
	// 		{
	// 			mysis.AddText(str_libiao, pt_3d_insert_text, pt_3d_insert_text, AcDb::kTextLeft, AcDb::kTextVertMid, 3, 0.55, PI/2,ncolorindex,idTextStyleChs) ;
	// 			mysis.AddText(str_name, pt_3d_insert_text2, pt_3d_insert_text2, AcDb::kTextLeft, AcDb::kTextVertMid, 3, 0.55, PI/2,ncolorindex,idTextStyleChs);	
	// 		}
	// 	}
	if(strblkname.Left(10)!=_T("DT_STATION"))
	{
		//??·?ê?oí′óìúéè±?1?à?±ê·?ê?ò???
		AcGePoint3d pt_3d_insert_text(d_x,dhight_libiao-20,0);
		AcGePoint3d pt_3d_insert_text2(d_x,dhight_libiao,0);
		AcGePoint3d pt_3d_insert_Start(d_x+2,dhight_libiao-18,0);
		AcGePoint3d pt_3d_insert_End(d_x+2,dhight_libiao,0);
		str_name =_T("(")+str_name+_T(")");
		if(str_libiao2!=_T(""))
		{
			//pt_3d_insert_text.x-=2;

			//CLineUtil::AddPolyLine(pt2d_start1,pt2d_end1,0.3);
			mysis.AddAcDb2dPolyline(pt_3d_insert_Start,pt_3d_insert_End,0.1);
			mysis.AddText(str_libiao, pt_3d_insert_text2, pt_3d_insert_text2, AcDb::kTextRight, AcDb::kTextVertMid, 2.5, 0.8, PI/2,ncolorindex,idTextStyleChs) ;
			pt_3d_insert_text2.x+=4;
			mysis.AddText(str_libiao2, pt_3d_insert_text2, pt_3d_insert_text2, AcDb::kTextRight, AcDb::kTextVertMid, 2.5, 0.8, PI/2,ncolorindex,idTextStyleChs) ;
			mysis.AddText(str_name, pt_3d_insert_text, pt_3d_insert_text, AcDb::kTextRight, AcDb::kTextVertMid, 2.5, 0.8, PI/2,ncolorindex,idTextStyleChs);	
		}
		else
		{
			mysis.AddAcDb2dPolyline(pt_3d_insert_Start,pt_3d_insert_End,0.1);
			mysis.AddText(str_libiao, pt_3d_insert_text2, pt_3d_insert_text2, AcDb::kTextRight, AcDb::kTextVertMid, 2.5, 0.8, PI/2,ncolorindex,idTextStyleChs) ;
			mysis.AddText(str_name, pt_3d_insert_text, pt_3d_insert_text, AcDb::kTextRight, AcDb::kTextVertMid, 2.5, 0.8, PI/2,ncolorindex,idTextStyleChs);	
		}
	}

}




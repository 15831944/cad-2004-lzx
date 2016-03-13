#pragma once
#include "..\blkuserdll\UserSystem.h"
#include "StdAfx.h"
#include "LineUtil.h"
#include "PolylineUtil.h"
#include "BlockUtil.h"
#include "ConvertUtil.h"
class CDtpmt
{
public:
	CDtpmt(void);
	CUserSystem mysis;
	CLineUtil   myline;
	void Add_AP( double d_x,double dhight_blk,double dhight_libiao,CString str_name,CString str_libiao ,CString strblkname=_T("DT_AP_100"),int ncolorindex=7);
	void ReLayDevice( vector<double> &d_veca,vector<CString> &str_veca,vector<CString>&str_vec_type_a, vector<double> &d_vecb,vector<CString> &str_vecb,vector<CString> &str_vec_type_b,vector<double> &d_veche,vector<double> &d_vecx,double d_width_max,double);
	BOOL DoubleToCString(double d_a,CString &str_a);
	void GetBlkType(CString str_device,CString &strblkname);
	//void AddFrameToPmt(AcGePoint3d pt3dStart,double d_width);
	void AddFrameToPmt( AcGePoint3d start_pt_3d,double d_width );
	BOOL FindSortNum(const vector<double> &d_vecSource,double d_dest, int &i_num);
	//void   AddPodu(vector<double> &d_vechedata,vector<double>&d_vecdestdata,vector<CString> &str_vecpolength,vector<CString>&str_vec_po,vector<int>&i_vec_sx);
	void AddPodu( vector<double> &d_vechedata,vector<double> &d_vec_xposition,vector<double>&d_vecdestdata,vector<CString> &str_vecpolength,vector<CString>&str_vec_podu,vector<int>&i_vec_sx,bool up );
	void AddQuXian( vector<double> &d_vechedata,vector<double> &d_vec_xposition,vector<double> &d_vecstart,vector<double> &d_vecend,vector<CString> &str_vecration,vector<int>&i_vec_at,bool up);
	void drawzhantai(const AcGePoint2d pt2d,const double d_xend,CString strStation,int iType,bool up);
	~CDtpmt(void);
	void Add_AP_1( double d_x,double dhight_blk,double dhight_libiao,CString str_name,CString str_libiao ,CString strblkname=_T("DT_AP_100"),int ncolorindex=7);
	void drawzhantai2( const AcGePoint2d pt2d,const double d_xend,CString strStation,int iType,bool up );
	//void AddZhantai(vector<CString> &str_deviceType_a,vector<double> d_veclibiao_a,vector<double> d_veczhantai_a,vector<int> i_vecType,vector<int>i_upordown);
	void AddZhantai( vector<CString> &str_vec_deviceType_a,vector<double> &d_veclibiao_a,vector<double> &d_vec_libiao_he,vector<double> &d_vecx,vector<double> &d_veczhantai_a,vector<int> &i_vecType);
	void GetBlkType2( CString str_device,CString &strblkname );
	//BOOL GetHightBlkAndLibiao(const int iOnline,double &d_hight_blk,double &d_hight_libiao);
	BOOL GetHightBlkAndLibiao( const int iOnline,double &d_hight_blk);
	//void Add_Device( double d_x,double dhight_blk,double dhight_libiao,CString str_name,CString str_libiao ,int i_upordown,CString strblkname=_T("DT_AP_100"),int ncolorindex=7);
	void Add_Device( double d_x,double dhight_blk,double dhight_libiao,CString str_name,CString str_libiao ,CString str_libiao2,int i_upordown,CString strblkname=_T("DT_AP_100"),int ncolorindex=7);
	//void Add_Device_1( double d_x,double dhight_blk,double dhight_libiao,CString str_name,CString str_libiao ,int i_upordown,CString strblkname=_T("DT_AP_100"),int ncolorindex=7);
	void Add_Device_1( double d_x,double dhight_blk,double dhight_libiao,CString str_name,CString str_libiao ,CString str_libiao2,int i_upordown,CString strblkname=_T("DT_AP_100"),int ncolorindex=7 );
	//void ReLay( vector<double> &d_veca,vector<CString> &str_veca,vector<CString>&str_vec_type_a, vector<int> &i_vec_upordown_a,vector<double> &d_vecb,vector<CString> &str_vecb,vector<CString> &str_vec_type_b, vector<int> &i_vec_upordown_b,vector<double> &d_veche,vector<double> &d_vecx,double d_width_max,double d_width_min);
	void ReLay2( vector<double> &d_veca,vector<CString> &str_veca,vector<double> &d_vec_libiao2_a, vector<CString>&str_vec_type_a, vector<int> &i_vec_line_a,vector<int> &i_vec_upordown_a,vector<CString> &str_vec_info_a,vector<double> &d_vecb,vector<CString> &str_vecb,vector<double> &d_vec_libiao2_b,vector<CString> &str_vec_type_b, vector<int> &i_vec_line_b,vector<int> &i_vec_upordown_b,vector<CString> &str_vec_info_b,vector<double> &d_veche,vector<double> &d_vecx,double d_width_max,double d_width_min);
	static void AddFrameToDtsxt( AcGePoint3d start_pt_3d,double d_width );
	BOOL CStringToDouble( CString str_a,double &d_a);
	void Add_Device_00( double d_x,double dhight_blk,double dhight_libiao,CString str_name,CString str_libiao ,CString str_libiao2,int i_upordown,CString strblkname=_T("DT_AP_100"),int ncolorindex=7);
	void Add_Device_01( double d_x,double dhight_blk,double dhight_libiao,CString str_name,CString str_libiao ,CString str_libiao2,int i_upordown,CString strblkname=_T("DT_AP_100"),int ncolorindex=7);
	void AddQuXian_0( vector<double> &d_vechedata,vector<double> &d_vec_xposition,vector<double> &d_vecstart,vector<double> &d_vecend,vector<CString> &str_vecration,vector<int>&i_vec_at,bool up );
	void AddPodu_0( vector<double> &d_vechedata,vector<double> &d_vec_xposition,vector<double>&d_vecdestdata,vector<CString> &str_vecpolength,vector<CString>&str_vec_podu,vector<int>&i_vec_sx,bool up );
	void ReLay3( vector<double> &d_veca,vector<CString> &str_veca,vector<double> &d_vec_libiao2_a, vector<CString>&str_vec_type_a, vector<int> &i_vec_line_a,vector<int> &i_vec_upordown_a,vector<CString> &str_vec_info_a,vector<double> &d_vecb,vector<CString> &str_vecb,vector<double> &d_vec_libiao2_b,vector<CString> &str_vec_type_b, vector<int> &i_vec_line_b,vector<int> &i_vec_upordown_b,vector<CString> &str_vec_info_b,vector<double> &d_veche,vector<double> &d_vecx,double d_width_max,double d_width_min);
};

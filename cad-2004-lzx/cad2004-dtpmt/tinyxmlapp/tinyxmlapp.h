// tinyxmlapp.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ������


// CtinyxmlappApp:
// �йش����ʵ�֣������ tinyxmlapp.cpp
//

class CtinyxmlappApp : public CWinApp
{
public:
	CtinyxmlappApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CtinyxmlappApp theApp;


// HyperLPR.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CHyperLPRApp: 
// �йش����ʵ�֣������ HyperLPR.cpp
//

class CHyperLPRApp : public CWinApp
{
public:
	CHyperLPRApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CHyperLPRApp theApp;

// Ucom.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "DeConsole.h"

// CUcomApp: 
// �йش����ʵ�֣������ Ucom.cpp
//

class CUcomApp : public CWinApp
{
public:
	CUcomApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CUcomApp theApp;
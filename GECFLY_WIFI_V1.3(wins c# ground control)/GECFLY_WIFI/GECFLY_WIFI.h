
// GECFLY_WIFI.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CGECFLY_WIFIApp: 
// �йش����ʵ�֣������ GECFLY_WIFI.cpp
//

class CGECFLY_WIFIApp : public CWinApp
{
public:
	CGECFLY_WIFIApp();
	char *CStringtoChar(CString str, char ch[]);
	void SetSkin(CString strSkinFile);
	static DWORD ThreadKill(LPVOID lp);
// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CGECFLY_WIFIApp theApp;
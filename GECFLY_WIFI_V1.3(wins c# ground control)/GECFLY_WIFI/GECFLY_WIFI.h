
// GECFLY_WIFI.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CGECFLY_WIFIApp: 
// 有关此类的实现，请参阅 GECFLY_WIFI.cpp
//

class CGECFLY_WIFIApp : public CWinApp
{
public:
	CGECFLY_WIFIApp();
	char *CStringtoChar(CString str, char ch[]);
	void SetSkin(CString strSkinFile);
	static DWORD ThreadKill(LPVOID lp);
// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CGECFLY_WIFIApp theApp;
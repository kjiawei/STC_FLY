
// GECFLY_WIFI.cpp : 定义应用程序的类行为。
#include "stdafx.h"
#include "GECFLY_WIFI.h"
#include "GECFLY_WIFIDlg.h"
/******************************以下为默认生成，一般不需要改动*************************/
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGECFLY_WIFIApp
BEGIN_MESSAGE_MAP(CGECFLY_WIFIApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

// CGECFLY_WIFIApp 构造
CGECFLY_WIFIApp::CGECFLY_WIFIApp()
{
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}

// 唯一的一个 CGECFLY_WIFIApp 对象
CGECFLY_WIFIApp theApp;

// CGECFLY_WIFIApp 初始化
BOOL CGECFLY_WIFIApp::InitInstance()
{
	//默认处理函数
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	CWinApp::InitInstance();
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;
	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO:  应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	/******************************以下为Skin++皮肤设置函数*********************************/
	//Skin++皮肤设置函数
	theApp.SetSkin(_T("AquaOS.ssk"));
	/******************************以上为Skin++皮肤设置函数*********************************/
	CGECFLY_WIFIDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO:  在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO:  在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
		TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

/*******************************以下为支持Skin++皮肤的函数****************************/

void CGECFLY_WIFIApp::SetSkin(CString strSkinFile)
{
	DWORD dwThread;
	HANDLE hThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadKill, 0, 0, &dwThread);
	CloseHandle(hThread);
	char p[30] = { 0 };
	CStringtoChar(strSkinFile.GetBuffer(strSkinFile.GetLength()), p);
	BOOL bReg = skinppLoadSkin(p);
	strSkinFile.ReleaseBuffer();
}

DWORD CGECFLY_WIFIApp::ThreadKill(LPVOID lp)
{
	int iCount = 0;
	while (iCount++<10000)
	{
		Sleep(1);
		HWND hWnd = ::FindWindow(NULL, _T("UIPOWER"));
		if (hWnd == NULL) continue;
		WPARAM wp = BN_CLICKED << 16 | IDNO;
		::SendMessage(hWnd, WM_COMMAND, wp, 0);
		break;
	}
	return EXIT_SUCCESS;
}


char *CGECFLY_WIFIApp::CStringtoChar(CString str, char ch[])
{
	int i;
	char *tmpch;
	int wLen = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);//得到Char的长度    
	tmpch = new char[wLen + 1];                                             //分配变量的地址大小    
	WideCharToMultiByte(CP_ACP, 0, str, -1, tmpch, wLen, NULL, NULL);       //将CString转换成char*    
	for (i = 0; tmpch[i] != '\0'; i++) ch[i] = tmpch[i];
	ch[i] = '\0';
	return ch;
}
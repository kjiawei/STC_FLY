
// GECFLY_WIFI.cpp : ����Ӧ�ó��������Ϊ��
#include "stdafx.h"
#include "GECFLY_WIFI.h"
#include "GECFLY_WIFIDlg.h"
/******************************����ΪĬ�����ɣ�һ�㲻��Ҫ�Ķ�*************************/
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGECFLY_WIFIApp
BEGIN_MESSAGE_MAP(CGECFLY_WIFIApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

// CGECFLY_WIFIApp ����
CGECFLY_WIFIApp::CGECFLY_WIFIApp()
{
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}

// Ψһ��һ�� CGECFLY_WIFIApp ����
CGECFLY_WIFIApp theApp;

// CGECFLY_WIFIApp ��ʼ��
BOOL CGECFLY_WIFIApp::InitInstance()
{
	//Ĭ�ϴ�����
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
	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;
	// ���Windows Native���Ӿ����������Ա��� MFC �ؼ�����������
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO:  Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	/******************************����ΪSkin++Ƥ�����ú���*********************************/
	//Skin++Ƥ�����ú���
	theApp.SetSkin(_T("AquaOS.ssk"));
	/******************************����ΪSkin++Ƥ�����ú���*********************************/
	CGECFLY_WIFIDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO:  �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO:  �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
		TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

/*******************************����Ϊ֧��Skin++Ƥ���ĺ���****************************/

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
	int wLen = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);//�õ�Char�ĳ���    
	tmpch = new char[wLen + 1];                                             //��������ĵ�ַ��С    
	WideCharToMultiByte(CP_ACP, 0, str, -1, tmpch, wLen, NULL, NULL);       //��CStringת����char*    
	for (i = 0; tmpch[i] != '\0'; i++) ch[i] = tmpch[i];
	ch[i] = '\0';
	return ch;
}
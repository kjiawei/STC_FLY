
// GECFLY_WIFIDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "Param.h "
#include "GECFLY_WIFI.h"
#include "afxdialogex.h"


#include <Winsock2.h>
#include <stdio.h>
#include <string.h>
#include <process.h>
#include <stdlib.h>
#include <iostream>
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"SkinPPWTL.lib")
#define PI 3.14159265358979323846

// CGECFLY_WIFIDlg �Ի���
class CGECFLY_WIFIDlg : public CDialogEx
{
// ����
public:
	CGECFLY_WIFIDlg(CWnd* pParent = NULL);	// ��׼���캯��
// �Ի�������
	enum { IDD = IDD_GECFLY_WIFI_DIALOG };
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	static char send_data[34];
	static char recv_data[34];
	static UINT Thread_RecvData(void *param);
	static UINT Thread_SendData(void *param);
	static UINT Thread_DrawLine(void *param);
	static SOCKET sockClient;
	CWinThread *thread_senddata;
	CWinThread *thread_recvdata;
	CWinThread *thread_drawline;
	Param param_dlg;
	//�ؼ���������
	CButton mc_lock;
	CEdit mc_ip;
	CEdit mc_port;
	CEdit mc_recv;
	CEdit mc_power;
	CSliderCtrl mc_fb;
	CSliderCtrl mc_lr;
	CSliderCtrl mc_power_slider;
	CProgressCtrl mc_power_progress;

	//�ؼ���ֵ����
	BOOL lock;
	CString mv_ip;
	UINT mv_power;
	int mv_port;
	int mv_fb;
	int mv_lr;
	//������⺯��
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//����������
	afx_msg void OnNMCustomdrawSliderpower(NMHDR *pNMHDR, LRESULT *pResult);
	//CStringתchar *����
	char *CStringtoChar(CString str, char ch[]);
	//�����ʼ������
	void Initbuf(char buf[]);
	//���������⺯��
	UCHAR CheckRecv();
	int SetPower(UINT i);
	int SetPitch(UINT i);
	int SetYaw(UINT i);
	int SetRoll(UINT i);
	void SetOuterPid();
	void GetOuterPid();
	void SetInsidePid();
	void GetInsidePid();
	void SetAcc();
	void GetAcc();
	void GetState();

	//��ť������
	afx_msg void OnBnClickedButtonconnet();
	afx_msg void OnBnClickedButtonpower();
	afx_msg void OnBnClickedButtonunlock();
	afx_msg void OnBnClickedButtonparam();
	afx_msg void OnBnClickedButtonW();
	afx_msg void OnBnClickedButtonS();
	afx_msg void OnBnClickedButtonfall();
	afx_msg void OnBnClickedButtonstop();
	afx_msg void OnBnClickedButtonfront();
	afx_msg void OnBnClickedButtonback();
	afx_msg void OnBnClickedButtonleft();
	afx_msg void OnBnClickedButtonright();





};

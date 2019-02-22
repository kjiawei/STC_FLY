
#include "stdafx.h"
#include "GECFLY_WIFIDlg.h"
using namespace std;
/******************************����ΪĬ�����ɣ�һ�㲻��Ҫ�Ķ�*************************/
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};
CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}
BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


void CGECFLY_WIFIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}
// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�
void CGECFLY_WIFIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}
//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CGECFLY_WIFIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*******************************����Ϊ��Ҫ��ע���Ķ��ĵط�****************************/

char CGECFLY_WIFIDlg::send_data[34] = { 0 };
char CGECFLY_WIFIDlg::recv_data[34] = { 0 };
SOCKET CGECFLY_WIFIDlg::sockClient = 0;

// ���캯������ʼ�������ĵط�
CGECFLY_WIFIDlg::CGECFLY_WIFIDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGECFLY_WIFIDlg::IDD, pParent)
	, mv_ip(_T(""))
	, mv_port(0)
	, mv_power(0)
	, lock(true)
	//, param_dlg((CGECFLY_WIFIDlg *)this,NULL)
	, mv_fb(1500)
	, mv_lr(1500)
{
	//���ײ�SOCKET�汾�Ƿ�֧��
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(1, 1);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		return;
	}
	if (LOBYTE(wsaData.wVersion) != 1 ||
		HIBYTE(wsaData.wVersion) != 1) {
		WSACleanup();
		return;
	}
	//����socket TCP�׽���
	sockClient = socket(AF_INET, SOCK_STREAM, 0);
	//���������̣߳�������
	thread_senddata = AfxBeginThread(Thread_SendData, this, 0, 0, CREATE_SUSPENDED, NULL);
	//���������̣߳�������
	thread_recvdata = AfxBeginThread(Thread_RecvData, this, 0, 0, CREATE_SUSPENDED, NULL);
	//������ͼ�̣߳�������
	thread_drawline = AfxBeginThread(Thread_DrawLine, this, 0, 0, CREATE_SUSPENDED, NULL);
	//��ʼ����������
	Initbuf(send_data);
	

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

//�󶨱�����ID����
void CGECFLY_WIFIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Edit_ip, mc_ip);
	DDX_Control(pDX, IDC_Edit_port, mc_port);
	DDX_Text(pDX, IDC_Edit_ip, mv_ip);
	//  DDX_Text(pDX, IDC_Edit_port, mv_port);
	DDX_Text(pDX, IDC_Edit_port, mv_port);
	DDX_Control(pDX, IDC_Edit_recv, mc_recv);
	DDX_Control(pDX, IDC_Edit_power, mc_power);
	DDX_Control(pDX, IDC_Slider_power, mc_power_slider);
	DDX_Control(pDX, IDC_Progress_power, mc_power_progress);
	DDX_Text(pDX, IDC_Edit_power, mv_power);
	DDV_MinMaxUInt(pDX, mv_power, 0, 2000);
	DDX_Control(pDX, IDC_Button_lock, mc_lock);
	DDX_Slider(pDX, IDC_slider_fb, mv_fb);
	DDV_MinMaxInt(pDX, mv_fb, 0, 3000);
	DDX_Slider(pDX, IDC_slider_lr, mv_lr);
	DDV_MinMaxInt(pDX, mv_lr, 0, 3000);
	DDX_Control(pDX, IDC_slider_fb, mc_fb);
	DDX_Control(pDX, IDC_slider_lr, mc_lr);
}

//��Ϣӳ�䣬���ô������ĵط�
BEGIN_MESSAGE_MAP(CGECFLY_WIFIDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_Button_connet, &CGECFLY_WIFIDlg::OnBnClickedButtonconnet)
ON_NOTIFY(NM_CUSTOMDRAW, IDC_Slider_power, &CGECFLY_WIFIDlg::OnNMCustomdrawSliderpower)
ON_BN_CLICKED(IDC_Button_power, &CGECFLY_WIFIDlg::OnBnClickedButtonpower)
ON_BN_CLICKED(IDC_Button_lock, &CGECFLY_WIFIDlg::OnBnClickedButtonunlock)
ON_BN_CLICKED(IDC_Button_param, &CGECFLY_WIFIDlg::OnBnClickedButtonparam)
ON_BN_CLICKED(IDC_Button_W, &CGECFLY_WIFIDlg::OnBnClickedButtonW)
ON_BN_CLICKED(IDC_Button_S, &CGECFLY_WIFIDlg::OnBnClickedButtonS)
ON_BN_CLICKED(IDC_Button_fall, &CGECFLY_WIFIDlg::OnBnClickedButtonfall)
ON_BN_CLICKED(IDC_Button_stop, &CGECFLY_WIFIDlg::OnBnClickedButtonstop)
ON_BN_CLICKED(IDC_Button_front, &CGECFLY_WIFIDlg::OnBnClickedButtonfront)
ON_BN_CLICKED(IDC_Button_back, &CGECFLY_WIFIDlg::OnBnClickedButtonback)
ON_BN_CLICKED(IDC_Button_left, &CGECFLY_WIFIDlg::OnBnClickedButtonleft)
ON_BN_CLICKED(IDC_Button_right, &CGECFLY_WIFIDlg::OnBnClickedButtonright)
END_MESSAGE_MAP()

// �������ʼ������
BOOL CGECFLY_WIFIDlg::OnInitDialog()
{
	//Ĭ�ϴ���
	CDialogEx::OnInitDialog();
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// �ڴ�����Լ��Ĵ���
	mc_ip.SetWindowTextW(_T("192.168.4.1"));
	mc_port.SetWindowTextW(_T("333"));
	//mc_ip.SetWindowTextW(_T("192.168.1.165"));
	//mc_port.SetWindowTextW(_T("8080"));
	mc_power_slider.SetRange(0,1000,false);
	mc_power_slider.SetPos(1000);
	mc_power_progress.SetRange(0,1000);
	mc_power_progress.SetPos(0);
	mc_fb.SetRange(0, 3000);
	mc_lr.SetRange(0, 3000);
	mc_fb.SetPos(1500);
	mc_lr.SetPos(1500);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

//CStringתchar *����
char *CGECFLY_WIFIDlg::CStringtoChar(CString str, char ch[])
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

/********************************�̴߳�����*******************************/
//�����߳�
UINT CGECFLY_WIFIDlg::Thread_RecvData(void *param)
{
	CGECFLY_WIFIDlg *dlg = (CGECFLY_WIFIDlg*)param;
	CString str;
	while (1)
	{
		//memset((void *)recv_data, 0, 34);
		recv(sockClient, (char *)recv_data, 34, 0);
		if (!dlg->CheckRecv())
			continue;
		else
		{
			switch (dlg->CheckRecv())
			{
				//�����⻷PID
			case 0x20:		
				dlg->GetOuterPid(); 
				break;
				//�����ڻ�PID
			case 0x24:		
				dlg->GetInsidePid(); 
				break;
				//������̬����
			case 0x21:		
				dlg->GetAcc(); 
				break;
				//���ӳɹ�
			case 0x50:		
				break;
				//��ʱ����ֵ
			case 0x30:
				break;
			}
		}
		Sleep(3);
	}
	return 0;
}

//�����߳�
UINT CGECFLY_WIFIDlg::Thread_SendData(void *param)
{
	CGECFLY_WIFIDlg *dlg = (CGECFLY_WIFIDlg*)param;
	while (1)
	{
		send(sockClient, (const char *)send_data, 34, 0);
		Sleep(3);
	}
	return 0;
}

//��ͼ�߳�
UINT CGECFLY_WIFIDlg::Thread_DrawLine(void *param)
{
	CGECFLY_WIFIDlg *dlg = (CGECFLY_WIFIDlg*)param;
	CString str;
	while (1)
	{

		recv(sockClient, (char *)recv_data, 34, 0);
		if (!dlg->CheckRecv())
			continue;
		else
		{
			if (dlg->CheckRecv() == 0x30)
			{
				dlg->GetState();
			}
		}
		memset((void *)recv_data, 0, 34);
	}
	return 0;
}
/*******************************�����������ú���****************************/
//��ʼ��Э������
void CGECFLY_WIFIDlg::Initbuf(char buf[])
{
	memset((void *)buf, 0, 34);
	buf[0] = (char)0xAA;
	buf[1] = 0x00;
	buf[2] = 0x1C;
	buf[31] = 0x1C;
	buf[32] = 0x0D;
	buf[33] = 0x0A;

}

//��������
int CGECFLY_WIFIDlg::SetPower(UINT i)
{
	if (i<0 || i>1000)
		return 1;
	//Initbuf(send_data);
	send_data[1] = (UCHAR)0xC0;
	send_data[3] = (UCHAR)(i >> 8) & 0xff;			//��λ
	send_data[4] = (UCHAR)i & 0xff;					//��λ
	return 0;
}

//���ø���
int CGECFLY_WIFIDlg::SetPitch(UINT i)	
{
	if (i<0 || i>3000)
		return 1;
	send_data[1] = (UCHAR)0xC0;
	send_data[9] = (UCHAR)(i >> 8) & 0xff;			//��λ
	send_data[10] = (UCHAR)i & 0xff;					//��λ
	return 0;
}

//���ú���
int CGECFLY_WIFIDlg::SetYaw(UINT i)	
{
	if (i<0 || i>3000)
		return 1;
	send_data[1] = (UCHAR)0xC0;
	send_data[5] = (UCHAR)(i >> 8) & 0xff;			//��λ
	send_data[6] = (UCHAR)i & 0xff;					//��λ
	return 0;
}

//���ú��
int CGECFLY_WIFIDlg::SetRoll(UINT i)
{
	if (i<0 || i>3000)
		return 1;
	send_data[1] = (UCHAR)0xC0;
	send_data[7] = (UCHAR)(i >> 8) & 0xff;			//��λ
	send_data[8] = (UCHAR)i & 0xff;					//��λ
	return 0;
}

//�����⻷PID
void CGECFLY_WIFIDlg::SetOuterPid()	
{
	param_dlg.UpdateData(true);
	Initbuf(send_data);
	send_data[1] = 0x10;
	send_data[3] = (param_dlg.mv_orp * 100) >> 8 & 0xFF;		//���P��λ
	send_data[4] = (param_dlg.mv_orp * 100) & 0xFF;				//���P��λ
	send_data[5] = (param_dlg.mv_ori * 100) >> 8 & 0xFF;		//���I��λ
	send_data[6] = (param_dlg.mv_ori * 100) & 0xFF;				//���I��λ
	send_data[7] = (param_dlg.mv_ord * 100) >> 8 & 0xFF;		//���D��λ
	send_data[8] = (param_dlg.mv_ord * 100) & 0xFF;				//���D��λ

	send_data[9]  = (param_dlg.mv_opp * 100) >> 8 & 0xFF;		//����P��λ
	send_data[10] = (param_dlg.mv_opp * 100) & 0xFF;			//����P��λ
	send_data[11] = (param_dlg.mv_opi * 100) >> 8 & 0xFF;		//����I��λ
	send_data[12] = (param_dlg.mv_opi * 100) & 0xFF;			//����I��λ
	send_data[13] = (param_dlg.mv_opd * 100) >> 8 & 0xFF;		//����D��λ
	send_data[14] = (param_dlg.mv_opd * 100) & 0xFF;			//����D��λ

	send_data[15] = (param_dlg.mv_oyp * 100) >> 8 & 0xFF;		//����P��λ
	send_data[16] = (param_dlg.mv_oyp * 100)  & 0xFF;			//����P��λ
	send_data[17] = (param_dlg.mv_oyi * 100) >> 8 & 0xFF;		//����I��λ
	send_data[18] = (param_dlg.mv_oyi * 100)  & 0xFF;			//����I��λ
	send_data[19] = (param_dlg.mv_oyd * 100) >> 8 & 0xFF;		//����D��λ
	send_data[20] = (param_dlg.mv_oyd * 100)  & 0xFF;			//����D��λ
	thread_senddata->ResumeThread();
	Sleep(300);
	thread_senddata->SuspendThread();
}

//�����ڻ�PID
void CGECFLY_WIFIDlg::SetInsidePid()	
{
	param_dlg.UpdateData(true);
	Initbuf(send_data);
	send_data[1] = 0x14;
	send_data[3] = (param_dlg.mv_irp * 10) >> 8 & 0xFF;		//���P��λ
	send_data[4] = (param_dlg.mv_irp * 10) & 0xFF;				//���P��λ
	send_data[5] = (param_dlg.mv_iri * 10) >> 8 & 0xFF;		//���I��λ
	send_data[6] = (param_dlg.mv_iri * 10) & 0xFF;				//���I��λ
	send_data[7] = (param_dlg.mv_ird * 10) >> 8 & 0xFF;		//���D��λ
	send_data[8] = (param_dlg.mv_ird * 10) & 0xFF;				//���D��λ

	send_data[9] = (param_dlg.mv_ipp * 10) >> 8 & 0xFF;		//����P��λ
	send_data[10] = (param_dlg.mv_ipp * 10) & 0xFF;			//����P��λ
	send_data[11] = (param_dlg.mv_ipi * 10) >> 8 & 0xFF;		//����I��λ
	send_data[12] = (param_dlg.mv_ipi * 10) & 0xFF;			//����I��λ
	send_data[13] = (param_dlg.mv_ipd * 10) >> 8 & 0xFF;		//����D��λ
	send_data[14] = (param_dlg.mv_ipd * 10) & 0xFF;			//����D��λ

	send_data[15] = (param_dlg.mv_iyp * 10) >> 8 & 0xFF;		//����P��λ
	send_data[16] = (param_dlg.mv_iyp * 10) & 0xFF;			//����P��λ
	send_data[17] = (param_dlg.mv_iyi * 10) >> 8 & 0xFF;		//����I��λ
	send_data[18] = (param_dlg.mv_iyi * 10) & 0xFF;			//����I��λ
	send_data[19] = (param_dlg.mv_iyd * 10) >> 8 & 0xFF;		//����D��λ
	send_data[20] = (param_dlg.mv_iyd * 10) & 0xFF;			//����D��λ
	thread_senddata->ResumeThread();
	Sleep(300);
	thread_senddata->SuspendThread();
}

//������̬
void CGECFLY_WIFIDlg::SetAcc()	
{
	param_dlg.UpdateData(true);
	Initbuf(send_data);
	send_data[1] = 0x11;
	send_data[3] = param_dlg.mv_ax >> 8 & 0xFF;			//���ٶ�X��λ
	send_data[4] = param_dlg.mv_ax & 0xFF;				//���ٶ�X��λ
	send_data[5] = param_dlg.mv_ay >> 8 & 0xFF;			//���ٶ�Y��λ
	send_data[6] = param_dlg.mv_ay  & 0xFF;				//���ٶ�Y��λ
	send_data[7] = param_dlg.mv_az >> 8 & 0xFF;			//���ٶ�Z��λ
	send_data[8] = param_dlg.mv_az  & 0xFF;				//���ٶ�Z��λ

	send_data[9] = param_dlg.mv_rx >> 8 & 0xFF;			//������X��λ
	send_data[10] = param_dlg.mv_rx  & 0xFF;			//������X��λ
	send_data[11] = param_dlg.mv_ry >> 8 & 0xFF;		//������Y��λ
	send_data[12] = param_dlg.mv_ry  & 0xFF;			//������Y��λ
	send_data[13] = param_dlg.mv_rz >> 8 & 0xFF;		//������Z��λ
	send_data[14] = param_dlg.mv_rz  & 0xFF;			//������Z��λ

	thread_senddata->ResumeThread();
	Sleep(300);
	thread_senddata->SuspendThread();
}

/*******************************�������ݴ�����****************************/
//�������ͼ�麯��
UCHAR CGECFLY_WIFIDlg::CheckRecv()
{
	UCHAR checkbit = 0;
	for (int i = 0; i < 31; i++)
		checkbit += recv_data[i];
	checkbit &= 0xFF;
	if (checkbit != (recv_data[31] & 0xFF))
		return 0;
	else
		return recv_data[1];
}

//��ȡ�⻷PID
void CGECFLY_WIFIDlg::GetOuterPid()
{
	param_dlg.mv_orp = (MAKEWORD(recv_data[4], recv_data[3])) / 100;		//���P
	param_dlg.mv_ori = (MAKEWORD(recv_data[6], recv_data[5])) / 100;		//���I
	param_dlg.mv_ord = (MAKEWORD(recv_data[8], recv_data[7])) / 100;		//���D
	param_dlg.mv_opp = (MAKEWORD(recv_data[10], recv_data[9])) / 100;		//����P
	param_dlg.mv_opi = (MAKEWORD(recv_data[12], recv_data[11])) / 100;		//����I
	param_dlg.mv_opd = (MAKEWORD(recv_data[14], recv_data[13])) / 100;		//����D
	param_dlg.mv_oyp = (MAKEWORD(recv_data[16], recv_data[15])) / 100;		//����P
	param_dlg.mv_oyi = (MAKEWORD(recv_data[18], recv_data[17])) / 100;		//����I
	param_dlg.mv_oyd = (MAKEWORD(recv_data[20], recv_data[19])) / 100;		//����D
	memset((void *)recv_data, 0, 34);
	param_dlg.GetDlgItem(IDC_Text_state1)->SetWindowTextW(_T("��ȡ�ɹ�"));
}

//��ȡ�ڻ�PID
void CGECFLY_WIFIDlg::GetInsidePid()
{
	param_dlg.mv_irp = (MAKEWORD(recv_data[4], recv_data[3])) / 10;		//���P
	param_dlg.mv_iri = (MAKEWORD(recv_data[6], recv_data[5])) / 10;		//���I
	param_dlg.mv_ird = (MAKEWORD(recv_data[8], recv_data[7])) / 10;		//���D
	param_dlg.mv_ipp = (MAKEWORD(recv_data[10], recv_data[9])) / 10;		//����P
	param_dlg.mv_ipi = (MAKEWORD(recv_data[12], recv_data[11])) / 10;		//����I
	param_dlg.mv_ipd = (MAKEWORD(recv_data[14], recv_data[13])) / 10;		//����D
	param_dlg.mv_iyp = (MAKEWORD(recv_data[16], recv_data[15])) / 10;		//����P
	param_dlg.mv_iyi = (MAKEWORD(recv_data[18], recv_data[17])) / 10;		//����I
	param_dlg.mv_iyd = (MAKEWORD(recv_data[20], recv_data[19])) / 10;		//����D
	memset((void *)recv_data, 0, 34);
	param_dlg.GetDlgItem(IDC_Text_state2)->SetWindowTextW(_T("��ȡ�ɹ�"));

}

//��ȡ��̬
void CGECFLY_WIFIDlg::GetAcc()
{
	param_dlg.mv_ax = (MAKEWORD(recv_data[4], recv_data[3]) + 500) % 65536;			//���ٶ�X
	param_dlg.mv_ay = (MAKEWORD(recv_data[6], recv_data[5]) + 500) % 65536;			//���ٶ�Y
	param_dlg.mv_az = (MAKEWORD(recv_data[8], recv_data[7]) + 500) % 65536;			//���ٶ�Z
	param_dlg.mv_rx = (MAKEWORD(recv_data[10], recv_data[9])) % 65536;				//������X
	param_dlg.mv_ry = (MAKEWORD(recv_data[12], recv_data[11])) % 65536;				//������Y
	param_dlg.mv_rz = (MAKEWORD(recv_data[14], recv_data[13])) % 65536;				//������Z
	memset((void *)recv_data, 0, 34);
	param_dlg.GetDlgItem(IDC_Text_state3)->SetWindowTextW(_T("��ȡ�ɹ�"));
	param_dlg.GetDlgItem(IDC_Text_state4)->SetWindowTextW(_T("��ȡ�ɹ�"));
}
//��ȡ����״̬
void CGECFLY_WIFIDlg::GetState()
{
	//ʮ�����߷ֱ�Ϊ[���ٶ�x/y/z ������x/y/z ���/����/���� ʵ����������/����/����]
	//MAKEWORD(��λ����λ)
	param_dlg.angle_y = (MAKEWORD(recv_data[4], recv_data[3]));		//���ٶ�y
	param_dlg.angle_x =		(MAKEWORD(recv_data[6], recv_data[5]) );		//���ٶ�x
	param_dlg.angle_z =		(MAKEWORD(recv_data[8], recv_data[7]) );		//���ٶ�z
	param_dlg.angle_x /= 100;
	param_dlg.angle_y /= 100;
	param_dlg.angle_z /= 100;
	param_dlg.gyroscope_x =	MAKEWORD(recv_data[10], recv_data[9]);			//������x
	param_dlg.gyroscope_y =	MAKEWORD(recv_data[12], recv_data[11]);			//������y
	param_dlg.gyroscope_z =	MAKEWORD(recv_data[14], recv_data[13]);			//������z
	param_dlg.roll =		MAKEWORD(recv_data[16], recv_data[15])/10;		//���
	param_dlg.pitch =		MAKEWORD(recv_data[18], recv_data[17])/10;		//����
	param_dlg.yaw =			MAKEWORD(recv_data[20], recv_data[19])/10;		//����
	param_dlg.real_roll =	MAKEWORD(recv_data[22], recv_data[21]);			//�����ʵ���ֵ
	param_dlg.real_pitch =	MAKEWORD(recv_data[24], recv_data[23]);			//������ʵ���ֵ
	param_dlg.real_yaw =	MAKEWORD(recv_data[26], recv_data[25]);			//������ʵ���ֵ
	param_dlg.power =		MAKEWORD(recv_data[28], recv_data[27]);			//����
	param_dlg.adc =			MAKEWORD(recv_data[30],0)*10;					//��ѹ

	
	//CString str;
	//str.Format(_T("%x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x"),
	//	recv_data[0], recv_data[1], recv_data[2], recv_data[3], recv_data[4], recv_data[5], recv_data[6], recv_data[7], recv_data[8], recv_data[9], 
	//	recv_data[10], recv_data[11], recv_data[12], recv_data[13], recv_data[14], recv_data[15], recv_data[16], recv_data[17], recv_data[18], recv_data[19], 
	//	recv_data[20], recv_data[21], recv_data[22], recv_data[23], recv_data[24], recv_data[25], recv_data[26], recv_data[27], recv_data[28], recv_data[29], 
	//	recv_data[30], recv_data[31], recv_data[32], send_data[33]);
	//mc_recv.SetWindowTextW(str);

}


/*******************************�ؼ�����������****************************/
//���̼�⺯��
BOOL CGECFLY_WIFIDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			mc_recv.SetWindowTextW(_T("Enter")); return true; break;
		case VK_ESCAPE:
			mc_recv.SetWindowTextW(_T("Esc")); return true; break;
		case VK_UP:
			mc_recv.SetWindowTextW(_T("Up"));
			if (mv_fb >= 3000)break;
			mv_fb += 50;
			SetPitch(mv_fb);
			mc_fb.SetPos(3000 - mv_fb);
			break;
		case VK_DOWN:
			mc_recv.SetWindowTextW(_T("Down"));
			if (mv_fb <= 0)break;
			mv_fb -= 50;
			SetPitch(mv_fb);
			mc_fb.SetPos(3000 - mv_fb);
			break;
		case VK_LEFT:
			mc_recv.SetWindowTextW(_T("Left"));
			if (mv_lr >= 3000)break;
			mv_lr += 50;
			SetRoll(mv_lr);
			mc_lr.SetPos(3000 - mv_lr);
			break;
		case VK_RIGHT:
			mc_recv.SetWindowTextW(_T("Right"));
			if (mv_lr <= 0)break;
			mv_lr -= 50;
			SetRoll(mv_lr);
			mc_lr.SetPos(3000 - mv_lr);
			break;
		}
	}
	if (pMsg->message == WM_CHAR)
	{
		CString str;
		switch (pMsg->wParam)
		{
		case 'W':case 'w':
			if (mv_power >= 1000)
				break;
			str.Format(_T("%u"), mv_power += 1);
			mc_power.SetWindowTextW(str);
			SetPower(mv_power);
			mc_power_slider.SetPos(1000 - mv_power);
			mc_power_progress.SetPos(mv_power);
			break;
		case 'S':case 's':
			if (mv_power <= 0)break;
			str.Format(_T("%u"), mv_power -= 1);
			mc_power.SetWindowTextW(str);
			SetPower(mv_power);
			mc_power_slider.SetPos(1000 - mv_power);
			mc_power_progress.SetPos(mv_power);
			break;
		case 'A':case 'a':
			mc_recv.SetWindowTextW(_T("A")); break;
		case 'D':case 'd':
			mc_recv.SetWindowTextW(_T("D")); break;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

//���Ӱ�ť������
void CGECFLY_WIFIDlg::OnBnClickedButtonconnet()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	SOCKADDR_IN addrSrv;
	UpdateData(true);
	char ip_char[20] = { 0 };
	CStringtoChar(mv_ip, ip_char);		//CStringתchar *
	addrSrv.sin_addr.S_un.S_addr = inet_addr(ip_char);//�������˵ĵ�ַ  
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(mv_port);

	if (connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR)) == 0)
	{
		GetDlgItem(IDC_Button_connet)->EnableWindow(false);
		mc_ip.EnableWindow(false);
		mc_port.EnableWindow(false);
		send(sockClient, "GEC\r\n", strlen("GEC\r\n"), 0);
	}
	else
	{
		return;
	}
}

//���Ź���������
void CGECFLY_WIFIDlg::OnNMCustomdrawSliderpower(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	CString str;
	mv_power = 1000 - mc_power_slider.GetPos();	//��Ϊ��ֱ���������ݵߵ�
	str.Format(_T("%u"), mv_power);
	mc_power.SetWindowTextW(str);
	mc_power_progress.SetPos(mv_power);
	SetPower(mv_power);
	*pResult = 0;
}

//�������ð�ť����
void CGECFLY_WIFIDlg::OnBnClickedButtonpower()
{
	UpdateData(true);
	mc_power_slider.SetPos(1000 - mv_power);		//��Ϊ��ֱ���������ݵߵ�
	mc_power_progress.SetPos(mv_power);
	SetPower(mv_power);

}

//��������ť������
void CGECFLY_WIFIDlg::OnBnClickedButtonunlock()
{
	if (lock)	//���Ϊ�棬��ʾ�Ѿ�����,����Ҫ����
	{
		Initbuf(send_data);
		lock = false;
		mc_lock.SetWindowTextW(_T("�ѽ���"));
		thread_senddata->ResumeThread();
		GetDlgItem(IDC_Button_W)->EnableWindow(true);
		GetDlgItem(IDC_Button_fly)->EnableWindow(true);
		GetDlgItem(IDC_Button_stop)->EnableWindow(true);
		GetDlgItem(IDC_Button_fall)->EnableWindow(true);
		GetDlgItem(IDC_Button_S)->EnableWindow(true);
		GetDlgItem(IDC_Button_param)->EnableWindow(true); 
		GetDlgItem(IDC_Slider_power)->EnableWindow(true); 
		GetDlgItem(IDC_Edit_power)->EnableWindow(true);
		GetDlgItem(IDC_Button_power)->EnableWindow(true);
		GetDlgItem(IDC_Button_front)->EnableWindow(true);
		GetDlgItem(IDC_Button_back)->EnableWindow(true);
		GetDlgItem(IDC_Button_left)->EnableWindow(true);
		GetDlgItem(IDC_Button_right)->EnableWindow(true);
		GetDlgItem(IDC_Button_param)->EnableWindow(false);
		
	}
	else
	{
		Initbuf(send_data);
		lock = true;
		mc_lock.SetWindowTextW(_T("������"));
		thread_senddata->SuspendThread();
		GetDlgItem(IDC_Button_W)->EnableWindow(false);
		GetDlgItem(IDC_Button_fly)->EnableWindow(false);
		GetDlgItem(IDC_Button_stop)->EnableWindow(false);
		GetDlgItem(IDC_Button_fall)->EnableWindow(false);
		GetDlgItem(IDC_Button_S)->EnableWindow(false);
		GetDlgItem(IDC_Button_param)->EnableWindow(false);
		GetDlgItem(IDC_Slider_power)->EnableWindow(false);
		GetDlgItem(IDC_Edit_power)->EnableWindow(false);
		GetDlgItem(IDC_Button_power)->EnableWindow(false);
		GetDlgItem(IDC_Button_front)->EnableWindow(false);
		GetDlgItem(IDC_Button_back)->EnableWindow(false);
		GetDlgItem(IDC_Button_left)->EnableWindow(false);
		GetDlgItem(IDC_Button_right)->EnableWindow(false);
		GetDlgItem(IDC_Button_param)->EnableWindow(true);
	}
}

//�������ð�ť������
void CGECFLY_WIFIDlg::OnBnClickedButtonparam()
{
	thread_recvdata->ResumeThread();
	param_dlg.DoModal();
}

//W��ť������
void CGECFLY_WIFIDlg::OnBnClickedButtonW()
{
	CString str;
	if (mv_power > 990)
		return;
	str.Format(_T("%u"), mv_power += 10);
	mc_power.SetWindowTextW(str);
	SetPower(mv_power);
	mc_power_slider.SetPos(1000 - mv_power);
	mc_power_progress.SetPos(mv_power);
}

//S��ť������
void CGECFLY_WIFIDlg::OnBnClickedButtonS()
{
	CString str;
	if (mv_power < 10)return;
	str.Format(_T("%u"), mv_power -= 10);
	mc_power.SetWindowTextW(str);
	SetPower(mv_power);
	mc_power_slider.SetPos(1000 - mv_power);
	mc_power_progress.SetPos(mv_power);
}

//һ�����䰴ť����
void CGECFLY_WIFIDlg::OnBnClickedButtonfall()
{
	CString str;
	str.Format(_T("%u"), mv_power = 0);
	mc_power.SetWindowTextW(str);
	SetPower(mv_power);
	mc_power_slider.SetPos(1000 - mv_power);
	mc_power_progress.SetPos(mv_power);
}

//һ����ͣ��ť����
void CGECFLY_WIFIDlg::OnBnClickedButtonstop()
{
	CString str;
	str.Format(_T("%u"), mv_power = 300);
	mc_power.SetWindowTextW(str);
	SetPower(mv_power);
	mc_power_slider.SetPos(1000 - mv_power);
	mc_power_progress.SetPos(mv_power);
}

//��ǰ��ť����
void CGECFLY_WIFIDlg::OnBnClickedButtonfront()
{
	if (mv_fb > 2900)return;
	mv_fb += 100;
	SetPitch(mv_fb);
	mc_fb.SetPos(3000 - mv_fb);
}

//���ť����
void CGECFLY_WIFIDlg::OnBnClickedButtonback()
{
	if (mv_fb < 100)return;
	mv_fb -= 100;
	SetPitch(mv_fb);
	mc_fb.SetPos(3000 - mv_fb);
}

//����ť����
void CGECFLY_WIFIDlg::OnBnClickedButtonleft()
{
	if (mv_lr > 2900)return;
	mv_lr += 100;
	SetRoll(mv_lr);
	mc_lr.SetPos(3000 - mv_lr);
}

//���Ұ�ť����
void CGECFLY_WIFIDlg::OnBnClickedButtonright()
{
	if (mv_lr < 100)return;
	mv_lr -= 100;
	SetRoll(mv_lr);
	mc_lr.SetPos(3000 - mv_lr);
}

			
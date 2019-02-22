
#include "stdafx.h"
#include "GECFLY_WIFIDlg.h"
using namespace std;
/******************************以下为默认生成，一般不需要改动*************************/
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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
// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。
void CGECFLY_WIFIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}
//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CGECFLY_WIFIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*******************************以下为需要关注、改动的地方****************************/

char CGECFLY_WIFIDlg::send_data[34] = { 0 };
char CGECFLY_WIFIDlg::recv_data[34] = { 0 };
SOCKET CGECFLY_WIFIDlg::sockClient = 0;

// 构造函数，初始化变量的地方
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
	//检测底层SOCKET版本是否支持
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
	//创建socket TCP套接字
	sockClient = socket(AF_INET, SOCK_STREAM, 0);
	//创建发送线程，并挂起
	thread_senddata = AfxBeginThread(Thread_SendData, this, 0, 0, CREATE_SUSPENDED, NULL);
	//创建接收线程，并挂起
	thread_recvdata = AfxBeginThread(Thread_RecvData, this, 0, 0, CREATE_SUSPENDED, NULL);
	//创建绘图线程，并挂起
	thread_drawline = AfxBeginThread(Thread_DrawLine, this, 0, 0, CREATE_SUSPENDED, NULL);
	//初始化发送数组
	Initbuf(send_data);
	

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

//绑定变量与ID函数
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

//消息映射，设置处理函数的地方
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

// 主界面初始化函数
BOOL CGECFLY_WIFIDlg::OnInitDialog()
{
	//默认处理
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
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// 在此添加自己的代码
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
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

//CString转char *函数
char *CGECFLY_WIFIDlg::CStringtoChar(CString str, char ch[])
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

/********************************线程处理函数*******************************/
//接收线程
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
				//返回外环PID
			case 0x20:		
				dlg->GetOuterPid(); 
				break;
				//返回内环PID
			case 0x24:		
				dlg->GetInsidePid(); 
				break;
				//返回姿态数据
			case 0x21:		
				dlg->GetAcc(); 
				break;
				//连接成功
			case 0x50:		
				break;
				//定时返回值
			case 0x30:
				break;
			}
		}
		Sleep(3);
	}
	return 0;
}

//发送线程
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

//绘图线程
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
/*******************************发送数据设置函数****************************/
//初始化协议数组
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

//设置油门
int CGECFLY_WIFIDlg::SetPower(UINT i)
{
	if (i<0 || i>1000)
		return 1;
	//Initbuf(send_data);
	send_data[1] = (UCHAR)0xC0;
	send_data[3] = (UCHAR)(i >> 8) & 0xff;			//高位
	send_data[4] = (UCHAR)i & 0xff;					//低位
	return 0;
}

//设置俯仰
int CGECFLY_WIFIDlg::SetPitch(UINT i)	
{
	if (i<0 || i>3000)
		return 1;
	send_data[1] = (UCHAR)0xC0;
	send_data[9] = (UCHAR)(i >> 8) & 0xff;			//高位
	send_data[10] = (UCHAR)i & 0xff;					//低位
	return 0;
}

//设置航向
int CGECFLY_WIFIDlg::SetYaw(UINT i)	
{
	if (i<0 || i>3000)
		return 1;
	send_data[1] = (UCHAR)0xC0;
	send_data[5] = (UCHAR)(i >> 8) & 0xff;			//高位
	send_data[6] = (UCHAR)i & 0xff;					//低位
	return 0;
}

//设置横滚
int CGECFLY_WIFIDlg::SetRoll(UINT i)
{
	if (i<0 || i>3000)
		return 1;
	send_data[1] = (UCHAR)0xC0;
	send_data[7] = (UCHAR)(i >> 8) & 0xff;			//高位
	send_data[8] = (UCHAR)i & 0xff;					//低位
	return 0;
}

//设置外环PID
void CGECFLY_WIFIDlg::SetOuterPid()	
{
	param_dlg.UpdateData(true);
	Initbuf(send_data);
	send_data[1] = 0x10;
	send_data[3] = (param_dlg.mv_orp * 100) >> 8 & 0xFF;		//横滚P高位
	send_data[4] = (param_dlg.mv_orp * 100) & 0xFF;				//横滚P低位
	send_data[5] = (param_dlg.mv_ori * 100) >> 8 & 0xFF;		//横滚I高位
	send_data[6] = (param_dlg.mv_ori * 100) & 0xFF;				//横滚I低位
	send_data[7] = (param_dlg.mv_ord * 100) >> 8 & 0xFF;		//横滚D高位
	send_data[8] = (param_dlg.mv_ord * 100) & 0xFF;				//横滚D低位

	send_data[9]  = (param_dlg.mv_opp * 100) >> 8 & 0xFF;		//俯仰P高位
	send_data[10] = (param_dlg.mv_opp * 100) & 0xFF;			//俯仰P低位
	send_data[11] = (param_dlg.mv_opi * 100) >> 8 & 0xFF;		//俯仰I高位
	send_data[12] = (param_dlg.mv_opi * 100) & 0xFF;			//俯仰I低位
	send_data[13] = (param_dlg.mv_opd * 100) >> 8 & 0xFF;		//俯仰D高位
	send_data[14] = (param_dlg.mv_opd * 100) & 0xFF;			//俯仰D低位

	send_data[15] = (param_dlg.mv_oyp * 100) >> 8 & 0xFF;		//航向P高位
	send_data[16] = (param_dlg.mv_oyp * 100)  & 0xFF;			//航向P低位
	send_data[17] = (param_dlg.mv_oyi * 100) >> 8 & 0xFF;		//航向I高位
	send_data[18] = (param_dlg.mv_oyi * 100)  & 0xFF;			//航向I低位
	send_data[19] = (param_dlg.mv_oyd * 100) >> 8 & 0xFF;		//航向D高位
	send_data[20] = (param_dlg.mv_oyd * 100)  & 0xFF;			//航向D低位
	thread_senddata->ResumeThread();
	Sleep(300);
	thread_senddata->SuspendThread();
}

//设置内环PID
void CGECFLY_WIFIDlg::SetInsidePid()	
{
	param_dlg.UpdateData(true);
	Initbuf(send_data);
	send_data[1] = 0x14;
	send_data[3] = (param_dlg.mv_irp * 10) >> 8 & 0xFF;		//横滚P高位
	send_data[4] = (param_dlg.mv_irp * 10) & 0xFF;				//横滚P低位
	send_data[5] = (param_dlg.mv_iri * 10) >> 8 & 0xFF;		//横滚I高位
	send_data[6] = (param_dlg.mv_iri * 10) & 0xFF;				//横滚I低位
	send_data[7] = (param_dlg.mv_ird * 10) >> 8 & 0xFF;		//横滚D高位
	send_data[8] = (param_dlg.mv_ird * 10) & 0xFF;				//横滚D低位

	send_data[9] = (param_dlg.mv_ipp * 10) >> 8 & 0xFF;		//俯仰P高位
	send_data[10] = (param_dlg.mv_ipp * 10) & 0xFF;			//俯仰P低位
	send_data[11] = (param_dlg.mv_ipi * 10) >> 8 & 0xFF;		//俯仰I高位
	send_data[12] = (param_dlg.mv_ipi * 10) & 0xFF;			//俯仰I低位
	send_data[13] = (param_dlg.mv_ipd * 10) >> 8 & 0xFF;		//俯仰D高位
	send_data[14] = (param_dlg.mv_ipd * 10) & 0xFF;			//俯仰D低位

	send_data[15] = (param_dlg.mv_iyp * 10) >> 8 & 0xFF;		//航向P高位
	send_data[16] = (param_dlg.mv_iyp * 10) & 0xFF;			//航向P低位
	send_data[17] = (param_dlg.mv_iyi * 10) >> 8 & 0xFF;		//航向I高位
	send_data[18] = (param_dlg.mv_iyi * 10) & 0xFF;			//航向I低位
	send_data[19] = (param_dlg.mv_iyd * 10) >> 8 & 0xFF;		//航向D高位
	send_data[20] = (param_dlg.mv_iyd * 10) & 0xFF;			//航向D低位
	thread_senddata->ResumeThread();
	Sleep(300);
	thread_senddata->SuspendThread();
}

//设置姿态
void CGECFLY_WIFIDlg::SetAcc()	
{
	param_dlg.UpdateData(true);
	Initbuf(send_data);
	send_data[1] = 0x11;
	send_data[3] = param_dlg.mv_ax >> 8 & 0xFF;			//加速度X高位
	send_data[4] = param_dlg.mv_ax & 0xFF;				//加速度X低位
	send_data[5] = param_dlg.mv_ay >> 8 & 0xFF;			//加速度Y高位
	send_data[6] = param_dlg.mv_ay  & 0xFF;				//加速度Y低位
	send_data[7] = param_dlg.mv_az >> 8 & 0xFF;			//加速度Z高位
	send_data[8] = param_dlg.mv_az  & 0xFF;				//加速度Z低位

	send_data[9] = param_dlg.mv_rx >> 8 & 0xFF;			//陀螺仪X高位
	send_data[10] = param_dlg.mv_rx  & 0xFF;			//陀螺仪X低位
	send_data[11] = param_dlg.mv_ry >> 8 & 0xFF;		//陀螺仪Y高位
	send_data[12] = param_dlg.mv_ry  & 0xFF;			//陀螺仪Y低位
	send_data[13] = param_dlg.mv_rz >> 8 & 0xFF;		//陀螺仪Z高位
	send_data[14] = param_dlg.mv_rz  & 0xFF;			//陀螺仪Z低位

	thread_senddata->ResumeThread();
	Sleep(300);
	thread_senddata->SuspendThread();
}

/*******************************接收数据处理函数****************************/
//接收类型检查函数
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

//获取外环PID
void CGECFLY_WIFIDlg::GetOuterPid()
{
	param_dlg.mv_orp = (MAKEWORD(recv_data[4], recv_data[3])) / 100;		//横滚P
	param_dlg.mv_ori = (MAKEWORD(recv_data[6], recv_data[5])) / 100;		//横滚I
	param_dlg.mv_ord = (MAKEWORD(recv_data[8], recv_data[7])) / 100;		//横滚D
	param_dlg.mv_opp = (MAKEWORD(recv_data[10], recv_data[9])) / 100;		//俯仰P
	param_dlg.mv_opi = (MAKEWORD(recv_data[12], recv_data[11])) / 100;		//俯仰I
	param_dlg.mv_opd = (MAKEWORD(recv_data[14], recv_data[13])) / 100;		//俯仰D
	param_dlg.mv_oyp = (MAKEWORD(recv_data[16], recv_data[15])) / 100;		//航向P
	param_dlg.mv_oyi = (MAKEWORD(recv_data[18], recv_data[17])) / 100;		//航向I
	param_dlg.mv_oyd = (MAKEWORD(recv_data[20], recv_data[19])) / 100;		//航向D
	memset((void *)recv_data, 0, 34);
	param_dlg.GetDlgItem(IDC_Text_state1)->SetWindowTextW(_T("读取成功"));
}

//获取内环PID
void CGECFLY_WIFIDlg::GetInsidePid()
{
	param_dlg.mv_irp = (MAKEWORD(recv_data[4], recv_data[3])) / 10;		//横滚P
	param_dlg.mv_iri = (MAKEWORD(recv_data[6], recv_data[5])) / 10;		//横滚I
	param_dlg.mv_ird = (MAKEWORD(recv_data[8], recv_data[7])) / 10;		//横滚D
	param_dlg.mv_ipp = (MAKEWORD(recv_data[10], recv_data[9])) / 10;		//俯仰P
	param_dlg.mv_ipi = (MAKEWORD(recv_data[12], recv_data[11])) / 10;		//俯仰I
	param_dlg.mv_ipd = (MAKEWORD(recv_data[14], recv_data[13])) / 10;		//俯仰D
	param_dlg.mv_iyp = (MAKEWORD(recv_data[16], recv_data[15])) / 10;		//航向P
	param_dlg.mv_iyi = (MAKEWORD(recv_data[18], recv_data[17])) / 10;		//航向I
	param_dlg.mv_iyd = (MAKEWORD(recv_data[20], recv_data[19])) / 10;		//航向D
	memset((void *)recv_data, 0, 34);
	param_dlg.GetDlgItem(IDC_Text_state2)->SetWindowTextW(_T("读取成功"));

}

//获取姿态
void CGECFLY_WIFIDlg::GetAcc()
{
	param_dlg.mv_ax = (MAKEWORD(recv_data[4], recv_data[3]) + 500) % 65536;			//加速度X
	param_dlg.mv_ay = (MAKEWORD(recv_data[6], recv_data[5]) + 500) % 65536;			//加速度Y
	param_dlg.mv_az = (MAKEWORD(recv_data[8], recv_data[7]) + 500) % 65536;			//加速度Z
	param_dlg.mv_rx = (MAKEWORD(recv_data[10], recv_data[9])) % 65536;				//陀螺仪X
	param_dlg.mv_ry = (MAKEWORD(recv_data[12], recv_data[11])) % 65536;				//陀螺仪Y
	param_dlg.mv_rz = (MAKEWORD(recv_data[14], recv_data[13])) % 65536;				//陀螺仪Z
	memset((void *)recv_data, 0, 34);
	param_dlg.GetDlgItem(IDC_Text_state3)->SetWindowTextW(_T("读取成功"));
	param_dlg.GetDlgItem(IDC_Text_state4)->SetWindowTextW(_T("读取成功"));
}
//获取飞行状态
void CGECFLY_WIFIDlg::GetState()
{
	//十二条线分别为[加速度x/y/z 陀螺仪x/y/z 横滚/俯仰/航向 实际输出（横滚/俯仰/航向）]
	//MAKEWORD(低位，高位)
	param_dlg.angle_y = (MAKEWORD(recv_data[4], recv_data[3]));		//加速度y
	param_dlg.angle_x =		(MAKEWORD(recv_data[6], recv_data[5]) );		//加速度x
	param_dlg.angle_z =		(MAKEWORD(recv_data[8], recv_data[7]) );		//加速度z
	param_dlg.angle_x /= 100;
	param_dlg.angle_y /= 100;
	param_dlg.angle_z /= 100;
	param_dlg.gyroscope_x =	MAKEWORD(recv_data[10], recv_data[9]);			//陀螺仪x
	param_dlg.gyroscope_y =	MAKEWORD(recv_data[12], recv_data[11]);			//陀螺仪y
	param_dlg.gyroscope_z =	MAKEWORD(recv_data[14], recv_data[13]);			//陀螺仪z
	param_dlg.roll =		MAKEWORD(recv_data[16], recv_data[15])/10;		//横滚
	param_dlg.pitch =		MAKEWORD(recv_data[18], recv_data[17])/10;		//俯仰
	param_dlg.yaw =			MAKEWORD(recv_data[20], recv_data[19])/10;		//航向
	param_dlg.real_roll =	MAKEWORD(recv_data[22], recv_data[21]);			//横滚真实输出值
	param_dlg.real_pitch =	MAKEWORD(recv_data[24], recv_data[23]);			//俯仰真实输出值
	param_dlg.real_yaw =	MAKEWORD(recv_data[26], recv_data[25]);			//航向真实输出值
	param_dlg.power =		MAKEWORD(recv_data[28], recv_data[27]);			//油门
	param_dlg.adc =			MAKEWORD(recv_data[30],0)*10;					//电压

	
	//CString str;
	//str.Format(_T("%x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x"),
	//	recv_data[0], recv_data[1], recv_data[2], recv_data[3], recv_data[4], recv_data[5], recv_data[6], recv_data[7], recv_data[8], recv_data[9], 
	//	recv_data[10], recv_data[11], recv_data[12], recv_data[13], recv_data[14], recv_data[15], recv_data[16], recv_data[17], recv_data[18], recv_data[19], 
	//	recv_data[20], recv_data[21], recv_data[22], recv_data[23], recv_data[24], recv_data[25], recv_data[26], recv_data[27], recv_data[28], recv_data[29], 
	//	recv_data[30], recv_data[31], recv_data[32], send_data[33]);
	//mc_recv.SetWindowTextW(str);

}


/*******************************控件触发处理函数****************************/
//键盘检测函数
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

//连接按钮处理函数
void CGECFLY_WIFIDlg::OnBnClickedButtonconnet()
{
	// TODO:  在此添加控件通知处理程序代码
	SOCKADDR_IN addrSrv;
	UpdateData(true);
	char ip_char[20] = { 0 };
	CStringtoChar(mv_ip, ip_char);		//CString转char *
	addrSrv.sin_addr.S_un.S_addr = inet_addr(ip_char);//服务器端的地址  
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

//油门滚动条函数
void CGECFLY_WIFIDlg::OnNMCustomdrawSliderpower(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	CString str;
	mv_power = 1000 - mc_power_slider.GetPos();	//因为垂直拉动条数据颠倒
	str.Format(_T("%u"), mv_power);
	mc_power.SetWindowTextW(str);
	mc_power_progress.SetPos(mv_power);
	SetPower(mv_power);
	*pResult = 0;
}

//油门设置按钮函数
void CGECFLY_WIFIDlg::OnBnClickedButtonpower()
{
	UpdateData(true);
	mc_power_slider.SetPos(1000 - mv_power);		//因为垂直拉动条数据颠倒
	mc_power_progress.SetPos(mv_power);
	SetPower(mv_power);

}

//控制锁按钮处理函数
void CGECFLY_WIFIDlg::OnBnClickedButtonunlock()
{
	if (lock)	//如果为真，表示已经锁上,现在要解锁
	{
		Initbuf(send_data);
		lock = false;
		mc_lock.SetWindowTextW(_T("已解锁"));
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
		mc_lock.SetWindowTextW(_T("已上锁"));
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

//参数设置按钮处理函数
void CGECFLY_WIFIDlg::OnBnClickedButtonparam()
{
	thread_recvdata->ResumeThread();
	param_dlg.DoModal();
}

//W按钮处理函数
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

//S按钮处理函数
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

//一键降落按钮函数
void CGECFLY_WIFIDlg::OnBnClickedButtonfall()
{
	CString str;
	str.Format(_T("%u"), mv_power = 0);
	mc_power.SetWindowTextW(str);
	SetPower(mv_power);
	mc_power_slider.SetPos(1000 - mv_power);
	mc_power_progress.SetPos(mv_power);
}

//一键悬停按钮函数
void CGECFLY_WIFIDlg::OnBnClickedButtonstop()
{
	CString str;
	str.Format(_T("%u"), mv_power = 300);
	mc_power.SetWindowTextW(str);
	SetPower(mv_power);
	mc_power_slider.SetPos(1000 - mv_power);
	mc_power_progress.SetPos(mv_power);
}

//向前按钮函数
void CGECFLY_WIFIDlg::OnBnClickedButtonfront()
{
	if (mv_fb > 2900)return;
	mv_fb += 100;
	SetPitch(mv_fb);
	mc_fb.SetPos(3000 - mv_fb);
}

//向后按钮函数
void CGECFLY_WIFIDlg::OnBnClickedButtonback()
{
	if (mv_fb < 100)return;
	mv_fb -= 100;
	SetPitch(mv_fb);
	mc_fb.SetPos(3000 - mv_fb);
}

//向左按钮函数
void CGECFLY_WIFIDlg::OnBnClickedButtonleft()
{
	if (mv_lr > 2900)return;
	mv_lr += 100;
	SetRoll(mv_lr);
	mc_lr.SetPos(3000 - mv_lr);
}

//向右按钮函数
void CGECFLY_WIFIDlg::OnBnClickedButtonright()
{
	if (mv_lr < 100)return;
	mv_lr -= 100;
	SetRoll(mv_lr);
	mc_lr.SetPos(3000 - mv_lr);
}

			
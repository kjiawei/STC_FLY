// Param.cpp : 实现文件
#include "stdafx.h"
#include "Param.h"
#include "GECFLY_WIFIDlg.h"
#include "string.h"

// Param 对话框
IMPLEMENT_DYNAMIC(Param, CDialogEx)

//构造函数
Param::Param(CWnd* pParent /*=NULL*/)
	: CDialogEx(Param::IDD, pParent)
	, mv_opp(0), mv_opi(0), mv_opd(0)
	, mv_oyp(0), mv_oyi(0), mv_oyd(0)
	, mv_orp(0), mv_ori(0), mv_ord(0)
	, mv_ipp(0), mv_ipi(0), mv_ipd(0)
	, mv_iyp(0), mv_iyi(0), mv_iyd(0)
	, mv_irp(0), mv_iri(0), mv_ird(0)
	, mv_ax(0)
	, mv_check_ax(FALSE)
	, mv_check_ay(FALSE)
	, mv_check_az(FALSE)
	, mv_check_gx(FALSE)
	, mv_check_gy(FALSE)
	, mv_check_gz(FALSE)
	, mv_check_roll(FALSE)
	, mv_check_pitch(FALSE)
	, mv_check_yaw(FALSE)
	, mv_check_rroll(FALSE)
	, mv_check_rpitch(FALSE)
	, mv_check_ryaw(FALSE)
	, draw(FALSE)
	, pAxis(NULL)
{
	////创建绘图线程，并挂起
	//thread_drawline = AfxBeginThread(Thread_DrawLine,
	//	(CGECFLY_WIFIDlg *)this->GetParent(), 0, 0, CREATE_SUSPENDED, NULL);
}
//析构函数
Param::~Param()
{
}
//绑定变量与ID函数
void Param::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_Edit_OPP, mv_opp);
	DDV_MinMaxInt(pDX, mv_opp, 0, 1000);
	DDX_Text(pDX, IDC_Edit_OPI, mv_opi);
	DDV_MinMaxInt(pDX, mv_opi, 0, 1000);
	DDX_Text(pDX, IDC_Edit_OPD, mv_opd);
	DDV_MinMaxInt(pDX, mv_opd, 0, 1000);
	DDX_Text(pDX, IDC_Edit_OYP, mv_oyp);
	DDV_MinMaxInt(pDX, mv_oyp, 0, 1000);
	DDX_Text(pDX, IDC_Edit_OYI, mv_oyi);
	DDV_MinMaxInt(pDX, mv_oyi, 0, 1000);
	DDX_Text(pDX, IDC_Edit_OYD, mv_oyd);
	DDV_MinMaxInt(pDX, mv_oyd, 0, 1000);
	DDX_Text(pDX, IDC_Edit_ORP, mv_orp);
	DDV_MinMaxInt(pDX, mv_orp, 0, 1000);
	DDX_Text(pDX, IDC_Edit_ORI, mv_ori);
	DDV_MinMaxInt(pDX, mv_ori, 0, 1000);
	DDX_Text(pDX, IDC_Edit_ORD, mv_ord);
	DDV_MinMaxInt(pDX, mv_ord, 0, 1000);

	DDX_Text(pDX, IDC_Edit_IPP, mv_ipp);
	DDV_MinMaxInt(pDX, mv_ipp, 0, 1000);
	DDX_Text(pDX, IDC_Edit_IPI, mv_ipi);
	DDV_MinMaxInt(pDX, mv_ipi, 0, 1000);
	DDX_Text(pDX, IDC_Edit_IPD, mv_ipd);
	DDV_MinMaxInt(pDX, mv_ipd, 0, 1000);
	DDX_Text(pDX, IDC_Edit_IYP, mv_iyp);
	DDV_MinMaxInt(pDX, mv_iyp, 0, 1000);
	DDX_Text(pDX, IDC_Edit_IYI, mv_iyi);
	DDV_MinMaxInt(pDX, mv_iyi, 0, 1000);
	DDX_Text(pDX, IDC_Edit_IYD, mv_iyd);
	DDV_MinMaxInt(pDX, mv_iyd, 0, 1000);
	DDX_Text(pDX, IDC_Edit_IRP, mv_irp);
	DDV_MinMaxInt(pDX, mv_irp, 0, 1000);
	DDX_Text(pDX, IDC_Edit_IRI, mv_iri);
	DDV_MinMaxInt(pDX, mv_iri, 0, 1000);
	DDX_Text(pDX, IDC_Edit_IRD, mv_ird);
	DDV_MinMaxInt(pDX, mv_ird, 0, 1000);

	DDX_Control(pDX, IDC_Spin_OPP, mc_opp);
	DDX_Control(pDX, IDC_Spin_OPI, mc_opi);
	DDX_Control(pDX, IDC_Spin_OPD, mc_opd);
	DDX_Control(pDX, IDC_Spin_OYP, mc_oyp);
	DDX_Control(pDX, IDC_Spin_OYI, mc_oyi);
	DDX_Control(pDX, IDC_Spin_OYD, mc_oyd);
	DDX_Control(pDX, IDC_Spin_ORP, mc_orp);
	DDX_Control(pDX, IDC_Spin_ORI, mc_ori);
	DDX_Control(pDX, IDC_Spin_ORD, mc_ord);

	DDX_Control(pDX, IDC_Spin_IPP, mc_ipp);
	DDX_Control(pDX, IDC_Spin_IPI, mc_ipi);
	DDX_Control(pDX, IDC_Spin_IPD, mc_ipd);
	DDX_Control(pDX, IDC_Spin_IYP, mc_iyp);
	DDX_Control(pDX, IDC_Spin_IYI, mc_iyi);
	DDX_Control(pDX, IDC_Spin_IYD, mc_iyd);
	DDX_Control(pDX, IDC_Spin_IRP, mc_irp);
	DDX_Control(pDX, IDC_Spin_IRI, mc_iri);
	DDX_Control(pDX, IDC_Spin_IRD, mc_ird);

	DDX_Text(pDX, IDC_Edit_AX, mv_ax);
	DDV_MinMaxInt(pDX, mv_ax, 0, 1000);
	DDX_Text(pDX, IDC_Edit_AY, mv_ay);
	DDV_MinMaxInt(pDX, mv_ay, 0, 1000);
	DDX_Text(pDX, IDC_Edit_AZ, mv_az);
	DDV_MinMaxInt(pDX, mv_az, 0, 1000);
	DDX_Text(pDX, IDC_Edit_RX, mv_rx);
	DDV_MinMaxInt(pDX, mv_rx, 0, 1000);
	DDX_Text(pDX, IDC_Edit_RY, mv_ry);
	DDV_MinMaxInt(pDX, mv_ry, 0, 1000);
	DDX_Text(pDX, IDC_Edit_RZ, mv_rz);
	DDV_MinMaxInt(pDX, mv_rz, 0, 1000);
	DDX_Control(pDX, IDC_Slider_AX, mc_ax_slider);
	DDX_Control(pDX, IDC_Slider_AY, mc_ay_slider);
	DDX_Control(pDX, IDC_Slider_AZ, mc_az_slider);
	DDX_Control(pDX, IDC_Slider_RX, mc_rx_slider);
	DDX_Control(pDX, IDC_Slider_RY, mc_ry_slider);
	DDX_Control(pDX, IDC_Slider_RZ, mc_rz_slider);

	DDX_Control(pDX, IDC_ChartCtrl, mc_chartctrl);
	DDX_Check(pDX, IDC_check_ax, mv_check_ax);
	DDX_Check(pDX, IDC_check_ay, mv_check_ay);
	DDX_Check(pDX, IDC_check_az, mv_check_az);
	DDX_Check(pDX, IDC_check_gx, mv_check_gx);
	DDX_Check(pDX, IDC_check_gy, mv_check_gy);
	DDX_Check(pDX, IDC_check_gz, mv_check_gz);
	DDX_Check(pDX, IDC_check_roll, mv_check_roll);
	DDX_Check(pDX, IDC_check_pitch, mv_check_pitch);
	DDX_Check(pDX, IDC_check_yaw, mv_check_yaw);
	DDX_Check(pDX, IDC_check_rroll, mv_check_rroll);
	DDX_Check(pDX, IDC_check_rpitch, mv_check_rpitch);
	DDX_Check(pDX, IDC_check_ryaw, mv_check_ryaw);

}

//消息绑定
BEGIN_MESSAGE_MAP(Param, CDialogEx)
	ON_NOTIFY(UDN_DELTAPOS, IDC_Spin_OPP, &Param::OnDeltaposSpinOpp)
	ON_NOTIFY(UDN_DELTAPOS, IDC_Spin_OPI, &Param::OnDeltaposSpinOpi)
	ON_NOTIFY(UDN_DELTAPOS, IDC_Spin_OPD, &Param::OnDeltaposSpinOpd)
	ON_NOTIFY(UDN_DELTAPOS, IDC_Spin_OYP, &Param::OnDeltaposSpinOyp)
	ON_NOTIFY(UDN_DELTAPOS, IDC_Spin_OYI, &Param::OnDeltaposSpinOyi)
	ON_NOTIFY(UDN_DELTAPOS, IDC_Spin_OYD, &Param::OnDeltaposSpinOyd)
	ON_NOTIFY(UDN_DELTAPOS, IDC_Spin_ORP, &Param::OnDeltaposSpinOrp)
	ON_NOTIFY(UDN_DELTAPOS, IDC_Spin_ORI, &Param::OnDeltaposSpinOri)
	ON_NOTIFY(UDN_DELTAPOS, IDC_Spin_ORD, &Param::OnDeltaposSpinOrd)
	ON_NOTIFY(UDN_DELTAPOS, IDC_Spin_IPP, &Param::OnDeltaposSpinIpp)
	ON_NOTIFY(UDN_DELTAPOS, IDC_Spin_IPI, &Param::OnDeltaposSpinIpi)
	ON_NOTIFY(UDN_DELTAPOS, IDC_Spin_IPD, &Param::OnDeltaposSpinIpd)
	ON_NOTIFY(UDN_DELTAPOS, IDC_Spin_IYP, &Param::OnDeltaposSpinIyp)
	ON_NOTIFY(UDN_DELTAPOS, IDC_Spin_IYI, &Param::OnDeltaposSpinIyi)
	ON_NOTIFY(UDN_DELTAPOS, IDC_Spin_IYD, &Param::OnDeltaposSpinIyd)
	ON_NOTIFY(UDN_DELTAPOS, IDC_Spin_IRP, &Param::OnDeltaposSpinIrp)
	ON_NOTIFY(UDN_DELTAPOS, IDC_Spin_IRI, &Param::OnDeltaposSpinIri)
	ON_NOTIFY(UDN_DELTAPOS, IDC_Spin_IRD, &Param::OnDeltaposSpinIrd)
	ON_NOTIFY(UDN_DELTAPOS, IDC_Spin_AX, &Param::OnDeltaposSpinAx)
	ON_NOTIFY(UDN_DELTAPOS, IDC_Spin_AY, &Param::OnDeltaposSpinAy)
	ON_NOTIFY(UDN_DELTAPOS, IDC_Spin_AZ, &Param::OnDeltaposSpinAz)
	ON_NOTIFY(UDN_DELTAPOS, IDC_Spin_RX, &Param::OnDeltaposSpinRx)
	ON_NOTIFY(UDN_DELTAPOS, IDC_Spin_RY, &Param::OnDeltaposSpinRy)
	ON_NOTIFY(UDN_DELTAPOS, IDC_Spin_RZ, &Param::OnDeltaposSpinRz)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_Slider_AX, &Param::OnNMCustomdrawSliderAx)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_Slider_AY, &Param::OnNMCustomdrawSliderAy)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_Slider_AZ, &Param::OnNMCustomdrawSliderAz)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_Slider_RX, &Param::OnNMCustomdrawSliderRx)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_Slider_RY, &Param::OnNMCustomdrawSliderRy)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_Slider_RZ, &Param::OnNMCustomdrawSliderRz)
	ON_EN_CHANGE(IDC_Edit_AX, &Param::OnEnChangeEditAx)
	ON_EN_CHANGE(IDC_Edit_AY, &Param::OnEnChangeEditAy)
	ON_EN_CHANGE(IDC_Edit_AZ, &Param::OnEnChangeEditAz)
	ON_EN_CHANGE(IDC_Edit_RX, &Param::OnEnChangeEditRx)
	ON_EN_CHANGE(IDC_Edit_RY, &Param::OnEnChangeEditRy)
	ON_EN_CHANGE(IDC_Edit_RZ, &Param::OnEnChangeEditRz)
	ON_BN_CLICKED(IDC_Button_default, &Param::OnBnClickedButtondefault)
	ON_BN_CLICKED(IDC_Button_readouter, &Param::OnBnClickedButtonreadouter)
	ON_BN_CLICKED(IDC_Button_writeouter, &Param::OnBnClickedButtonwriteouter)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_Button_readinside, &Param::OnBnClickedButtonreadinside)
	ON_BN_CLICKED(IDC_Button_writeinside, &Param::OnBnClickedButtonwriteinside)
	ON_BN_CLICKED(IDC_Button_readacc, &Param::OnBnClickedButtonreadacc)
	ON_BN_CLICKED(IDC_Button_writeacc, &Param::OnBnClickedButtonwriteacc)
	ON_BN_CLICKED(IDC_Button_readrotate, &Param::OnBnClickedButtonreadrotate)
	ON_BN_CLICKED(IDC_Button_writerotate, &Param::OnBnClickedButtonwriterotate)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_check_ax, &Param::OnBnClickedcheckax)
	ON_BN_CLICKED(IDC_check_ay, &Param::OnBnClickedcheckay)
	ON_BN_CLICKED(IDC_check_az, &Param::OnBnClickedcheckaz)
	ON_BN_CLICKED(IDC_check_gx, &Param::OnBnClickedcheckgx)
	ON_BN_CLICKED(IDC_check_gy, &Param::OnBnClickedcheckgy)
	ON_BN_CLICKED(IDC_check_gz, &Param::OnBnClickedcheckgz)
	ON_BN_CLICKED(IDC_check_roll, &Param::OnBnClickedcheckroll)
	ON_BN_CLICKED(IDC_check_pitch, &Param::OnBnClickedcheckpitch)
	ON_BN_CLICKED(IDC_check_yaw, &Param::OnBnClickedcheckyaw)
	ON_BN_CLICKED(IDC_check_rroll, &Param::OnBnClickedcheckrroll)
	ON_BN_CLICKED(IDC_check_rpitch, &Param::OnBnClickedcheckrpitch)
	ON_BN_CLICKED(IDC_check_ryaw, &Param::OnBnClickedcheckryaw)
	ON_BN_CLICKED(IDC_drawstart, &Param::OnBnClickeddrawstart)
	ON_BN_CLICKED(IDC_drawstop, &Param::OnBnClickeddrawstop)
END_MESSAGE_MAP()




//界面初始化函数
BOOL Param::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	UpdateData(false);

	// TODO:  在此添加额外的初始化
	mc_opp.SetBuddy(GetDlgItem(IDC_Edit_OPP));
	mc_opp.SetRange(0, 1000);
	mc_opi.SetBuddy(GetDlgItem(IDC_Edit_OPI));
	mc_opi.SetRange(0, 1000);
	mc_opd.SetBuddy(GetDlgItem(IDC_Edit_OPD));
	mc_opd.SetRange(0, 1000);
	mc_opp.SetBuddy(GetDlgItem(IDC_Edit_OYP));
	mc_opp.SetRange(0, 1000);
	mc_opi.SetBuddy(GetDlgItem(IDC_Edit_OYI));
	mc_opi.SetRange(0, 1000);
	mc_opd.SetBuddy(GetDlgItem(IDC_Edit_OYD));
	mc_opd.SetRange(0, 1000);
	mc_opp.SetBuddy(GetDlgItem(IDC_Edit_ORP));
	mc_opp.SetRange(0, 1000);
	mc_opi.SetBuddy(GetDlgItem(IDC_Edit_ORI));
	mc_opi.SetRange(0, 1000);
	mc_opd.SetBuddy(GetDlgItem(IDC_Edit_ORD));
	mc_opd.SetRange(0, 1000);

	mc_ipp.SetBuddy(GetDlgItem(IDC_Edit_IPP));
	mc_ipp.SetRange(0, 1000);
	mc_ipi.SetBuddy(GetDlgItem(IDC_Edit_IPI));
	mc_ipi.SetRange(0, 1000);
	mc_ipd.SetBuddy(GetDlgItem(IDC_Edit_IPD));
	mc_ipd.SetRange(0, 1000);
	mc_ipp.SetBuddy(GetDlgItem(IDC_Edit_IYP));
	mc_ipp.SetRange(0, 1000);
	mc_ipi.SetBuddy(GetDlgItem(IDC_Edit_IYI));
	mc_ipi.SetRange(0, 1000);
	mc_ipd.SetBuddy(GetDlgItem(IDC_Edit_IYD));
	mc_ipd.SetRange(0, 1000);
	mc_ipp.SetBuddy(GetDlgItem(IDC_Edit_IRP));
	mc_ipp.SetRange(0, 1000);
	mc_ipi.SetBuddy(GetDlgItem(IDC_Edit_IRI));
	mc_ipi.SetRange(0, 1000);
	mc_ipd.SetBuddy(GetDlgItem(IDC_Edit_IRD));
	mc_ipd.SetRange(0, 1000);

	mc_ax_slider.SetRange(0, 20, false);
	mc_ax_slider.SetPos(10);
	mc_ay_slider.SetRange(0, 20, false);
	mc_ay_slider.SetPos(10);
	mc_az_slider.SetRange(0, 20, false);
	mc_az_slider.SetPos(10);
	mc_rx_slider.SetRange(0, 20, false);
	mc_rx_slider.SetPos(0);
	mc_ry_slider.SetRange(0, 20, false);
	mc_ry_slider.SetPos(0);
	mc_rz_slider.SetRange(0, 20, false);
	mc_rz_slider.SetPos(0);

	
	//GetDlgItem(IDC_ChartCtrl)->ModifyStyle(WS_VSCROLL);
	GetDlgItem(IDC_drawstop)->EnableWindow(false);      
	pAxis = mc_chartctrl.CreateStandardAxis(CChartCtrl::BottomAxis);     //初始化底部数轴
	pAxis->SetAutomatic(true);          //设置为自动
	pAxis = mc_chartctrl.CreateStandardAxis(CChartCtrl::LeftAxis);    //初始化左边数轴
	pAxis->SetAutomatic(true);          //设置为自动
	for (int i = 0; i<360; i++)
	{
		x[i] = i;
		y_ax[i] = 0;
		y_ay[i] = 0;
		y_az[i] = 0;
		y_gx[i] = 0;
		y_gy[i] = 0;
		y_gz[i] = 0;
		y_roll[i] = 0;
		y_pitch[i] = 0;
		y_yaw[i] = 0;
		y_rroll[i] = 0;
		y_rpitch[i] = 0;
		y_ryaw[i] = 0;
	}
	//mc_chartctrl.EnableRefresh(false);	 //关闭更新
	//TChartString str1;         //使用提供的TCHartString
	//str1 = _T("加速度数据波形图");
	//mc_chartctrl.GetTitle()->AddString(str1);  //设置标题1
	//p_Line1->SetSeriesOrdering(poNoOrdering);	 //设置为无序  
	//p_Line1->AddPoints(x_rd1, y_rd1, 360);	 //用数组加入绘制点
	//p_Line1->SetName(_T("这是IDC_ChartCtrl1的第一条线"));
	//mc_chartctrl.EnableRefresh(true);




	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
//界面关闭时触发的函数
void Param::OnClose()
{
	CGECFLY_WIFIDlg *parent = (CGECFLY_WIFIDlg *)GetParent();
	parent->thread_recvdata->SuspendThread();
	memset((void *)parent->send_data, 0, 34);
	memset((void *)parent->recv_data, 0, 34);
	if (draw)OnBnClickeddrawstop();
	mc_chartctrl.RemoveAllSeries();
	CDialogEx::OnClose();
}

/***********************************以下为控件触发处理函数********************************/
/****************************微调按钮处理函数****************************/
////外环PID调节按钮触发函数
void Param::OnDeltaposSpinOpp(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if (pNMUpDown->iDelta == -1) // 如果此值为-1 , 说明点击了Spin的往下的箭头
	{
		if (mv_opp>=10)
			mv_opp -= 10;
	}
	else if (pNMUpDown->iDelta == 1) // 如果此值为1, 说明点击了Spin的往上的箭头
	{
		if (mv_opp < 1000)
				mv_opp += 10;
	}
	UpdateData(false);
	*pResult = 0;
}
void Param::OnDeltaposSpinOpi(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if (pNMUpDown->iDelta == -1) // 如果此值为-1 , 说明点击了Spin的往下的箭头
	{
		if (mv_opi >= 10)
			mv_opi -= 10;
	}
	else if (pNMUpDown->iDelta == 1) // 如果此值为1, 说明点击了Spin的往上的箭头
	{
		if (mv_opi < 1000)
			mv_opi += 10;
	}
	UpdateData(false);
	*pResult = 0;
}
void Param::OnDeltaposSpinOpd(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if (pNMUpDown->iDelta == -1) // 如果此值为-1 , 说明点击了Spin的往下的箭头
	{
		if (mv_opd >= 10)
			mv_opd -= 10;
	}
	else if (pNMUpDown->iDelta == 1) // 如果此值为1, 说明点击了Spin的往上的箭头
	{
		if (mv_opd < 1000)
			mv_opd += 10;
	}
	UpdateData(false);
	*pResult = 0;
}
//上下倒转
void Param::OnDeltaposSpinOyp(NMHDR *pNMHDR, LRESULT *pResult)		
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if (pNMUpDown->iDelta == 1) // 如果此值为-1 , 说明点击了Spin的往下的箭头
	{
		if (mv_oyp >= 10)
			mv_oyp -= 10;
	}
	else if (pNMUpDown->iDelta == -1) // 如果此值为1, 说明点击了Spin的往上的箭头
	{
		if (mv_oyp < 1000)
			mv_oyp += 10;
	}
	UpdateData(false);
	*pResult = 0;
}
void Param::OnDeltaposSpinOyi(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if (pNMUpDown->iDelta == 1) // 如果此值为-1 , 说明点击了Spin的往下的箭头
	{
		if (mv_oyi >= 10)
			mv_oyi -= 10;
	}
	else if (pNMUpDown->iDelta == -1) // 如果此值为1, 说明点击了Spin的往上的箭头
	{
		if (mv_oyi < 1000)
			mv_oyi += 10;
	}
	UpdateData(false);
	*pResult = 0;
}
void Param::OnDeltaposSpinOyd(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if (pNMUpDown->iDelta == 1) // 如果此值为-1 , 说明点击了Spin的往下的箭头
	{
		if (mv_oyd >= 10)
			mv_oyd -= 10;
	}
	else if (pNMUpDown->iDelta == -1) // 如果此值为1, 说明点击了Spin的往上的箭头
	{
		if (mv_oyd < 1000)
			mv_oyd += 10;
	}
	UpdateData(false);
	*pResult = 0;
}
void Param::OnDeltaposSpinOrp(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if (pNMUpDown->iDelta == 1) // 如果此值为-1 , 说明点击了Spin的往下的箭头
	{
		if (mv_orp >= 10)
			mv_orp -= 10;
	}
	else if (pNMUpDown->iDelta == -1) // 如果此值为1, 说明点击了Spin的往上的箭头
	{
		if (mv_orp < 1000)
			mv_orp += 10;
	}
	UpdateData(false);
	*pResult = 0;
}
void Param::OnDeltaposSpinOri(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if (pNMUpDown->iDelta == 1) // 如果此值为-1 , 说明点击了Spin的往下的箭头
	{
		if (mv_ori >= 10)
			mv_ori -= 10;
	}
	else if (pNMUpDown->iDelta == -1) // 如果此值为1, 说明点击了Spin的往上的箭头
	{
		if (mv_ori < 1000)
			mv_ori += 10;
	}
	UpdateData(false);
	*pResult = 0;
}
void Param::OnDeltaposSpinOrd(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if (pNMUpDown->iDelta == 1) // 如果此值为-1 , 说明点击了Spin的往下的箭头
	{
		if (mv_ord >= 10)
			mv_ord -= 10;
	}
	else if (pNMUpDown->iDelta == -1) // 如果此值为1, 说明点击了Spin的往上的箭头
	{
		if (mv_ord < 1000)
			mv_ord += 10;
	}
	UpdateData(false);
	*pResult = 0;
}

////内环PID调节按钮触发函数
void Param::OnDeltaposSpinIpp(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if (pNMUpDown->iDelta == -1) // 如果此值为-1 , 说明点击了Spin的往下的箭头
	{
		if (mv_ipp >= 10)
			mv_ipp -= 10;
	}
	else if (pNMUpDown->iDelta == 1) // 如果此值为1, 说明点击了Spin的往上的箭头
	{
		if (mv_ipp < 1000)
			mv_ipp += 10;
	}
	UpdateData(false);
	*pResult = 0;
}
void Param::OnDeltaposSpinIpi(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if (pNMUpDown->iDelta == -1) // 如果此值为-1 , 说明点击了Spin的往下的箭头
	{
		if (mv_ipi >= 10)
			mv_ipi -= 10;
	}
	else if (pNMUpDown->iDelta == 1) // 如果此值为1, 说明点击了Spin的往上的箭头
	{
		if (mv_ipi < 1000)
			mv_ipi += 10;
	}
	UpdateData(false);
	*pResult = 0;
}
void Param::OnDeltaposSpinIpd(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if (pNMUpDown->iDelta == -1) // 如果此值为-1 , 说明点击了Spin的往下的箭头
	{
		if (mv_ipd >= 10)
			mv_ipd -= 10;
	}
	else if (pNMUpDown->iDelta == 1) // 如果此值为1, 说明点击了Spin的往上的箭头
	{
		if (mv_ipd < 1000)
			mv_ipd += 10;
	}
	UpdateData(false);
	*pResult = 0;
}
//上下倒转
void Param::OnDeltaposSpinIyp(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if (pNMUpDown->iDelta == 1) // 如果此值为-1 , 说明点击了Spin的往下的箭头
	{
		if (mv_iyp >= 10)
			mv_iyp -= 10;
	}
	else if (pNMUpDown->iDelta == -1) // 如果此值为1, 说明点击了Spin的往上的箭头
	{
		if (mv_iyp < 1000)
			mv_iyp += 10;
	}
	UpdateData(false);
	*pResult = 0;
}
void Param::OnDeltaposSpinIyi(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if (pNMUpDown->iDelta == 1) // 如果此值为-1 , 说明点击了Spin的往下的箭头
	{
		if (mv_iyi >= 10)
			mv_iyi -= 10;
	}
	else if (pNMUpDown->iDelta == -1) // 如果此值为1, 说明点击了Spin的往上的箭头
	{
		if (mv_iyi < 1000)
			mv_iyi += 10;
	}
	UpdateData(false);
	*pResult = 0;
}
void Param::OnDeltaposSpinIyd(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if (pNMUpDown->iDelta == 1) // 如果此值为-1 , 说明点击了Spin的往下的箭头
	{
		if (mv_iyd >= 10)
			mv_iyd -= 10;
	}
	else if (pNMUpDown->iDelta == -1) // 如果此值为1, 说明点击了Spin的往上的箭头
	{
		if (mv_iyd < 1000)
			mv_iyd += 10;
	}
	UpdateData(false);
	*pResult = 0;
}
void Param::OnDeltaposSpinIrp(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if (pNMUpDown->iDelta == 1) // 如果此值为-1 , 说明点击了Spin的往下的箭头
	{
		if (mv_irp >= 10)
			mv_irp -= 10;
	}
	else if (pNMUpDown->iDelta == -1) // 如果此值为1, 说明点击了Spin的往上的箭头
	{
		if (mv_irp < 1000)
			mv_irp += 10;
	}
	UpdateData(false);
	*pResult = 0;
}
void Param::OnDeltaposSpinIri(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if (pNMUpDown->iDelta == 1) // 如果此值为-1 , 说明点击了Spin的往下的箭头
	{
		if (mv_iri >= 10)
			mv_iri -= 10;
	}
	else if (pNMUpDown->iDelta == -1) // 如果此值为1, 说明点击了Spin的往上的箭头
	{
		if (mv_iri < 1000)
			mv_iri += 10;
	}
	UpdateData(false);
	*pResult = 0;
}
void Param::OnDeltaposSpinIrd(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if (pNMUpDown->iDelta == 1) // 如果此值为-1 , 说明点击了Spin的往下的箭头
	{
		if (mv_ird >= 10)
			mv_ird -= 10;
	}
	else if (pNMUpDown->iDelta == -1) // 如果此值为1, 说明点击了Spin的往上的箭头
	{
		if (mv_ird < 1000)
			mv_ird += 10;
	}
	UpdateData(false);
	*pResult = 0;
}

//加速度、陀螺仪调节按钮触发函数，上下按钮倒转
void Param::OnDeltaposSpinAx(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if (pNMUpDown->iDelta == 1) // 如果此值为-1 , 说明点击了Spin的往下的箭头
	{
		if (mv_ax >= 50)
			mv_ax -= 50;
	}
	else if (pNMUpDown->iDelta == -1) // 如果此值为1, 说明点击了Spin的往上的箭头
	{
		if (mv_ax < 1000)
			mv_ax += 50;
	}
	UpdateData(false);
	*pResult = 0;
}
void Param::OnDeltaposSpinAy(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if (pNMUpDown->iDelta == 1) // 如果此值为-1 , 说明点击了Spin的往下的箭头
	{
		if (mv_ay >= 50)
			mv_ay -= 50;
	}
	else if (pNMUpDown->iDelta == -1) // 如果此值为1, 说明点击了Spin的往上的箭头
	{
		if (mv_ay < 1000)
			mv_ay += 50;
	}
	UpdateData(false);
	*pResult = 0;
}
void Param::OnDeltaposSpinAz(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if (pNMUpDown->iDelta == 1) // 如果此值为-1 , 说明点击了Spin的往下的箭头
	{
		if (mv_az >= 50)
			mv_az -= 50;
	}
	else if (pNMUpDown->iDelta == -1) // 如果此值为1, 说明点击了Spin的往上的箭头
	{
		if (mv_az < 1000)
			mv_az += 50;
	}
	UpdateData(false);
	*pResult = 0;
}
void Param::OnDeltaposSpinRx(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if (pNMUpDown->iDelta == 1) // 如果此值为-1 , 说明点击了Spin的往下的箭头
	{
		if (mv_rx >= 50)
			mv_rx -= 50;
	}
	else if (pNMUpDown->iDelta == -1) // 如果此值为1, 说明点击了Spin的往上的箭头
	{
		if (mv_rx < 1000)
			mv_rx += 50;
	}
	UpdateData(false);
	*pResult = 0;
}
void Param::OnDeltaposSpinRy(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if (pNMUpDown->iDelta == 1) // 如果此值为-1 , 说明点击了Spin的往下的箭头
	{
		if (mv_ry >= 50)
			mv_ry -= 50;
	}
	else if (pNMUpDown->iDelta == -1) // 如果此值为1, 说明点击了Spin的往上的箭头
	{
		if (mv_ry < 1000)
			mv_ry += 50;
	}
	UpdateData(false);
	*pResult = 0;
}
void Param::OnDeltaposSpinRz(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if (pNMUpDown->iDelta == 1) // 如果此值为-1 , 说明点击了Spin的往下的箭头
	{
		if (mv_rz >= 50)
			mv_rz -= 50;
	}
	else if (pNMUpDown->iDelta == -1) // 如果此值为1, 说明点击了Spin的往上的箭头
	{
		if (mv_rz < 1000)
			mv_rz += 50;
	}
	UpdateData(false);
	*pResult = 0;
}

/*********************************拖动条处理函数*********************************/
//拖动条触发函数
void Param::OnNMCustomdrawSliderAx(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	mv_ax = mc_ax_slider.GetPos()*50;	
	UpdateData(false);
	UpdateData(true);
	*pResult = 0;
}
void Param::OnNMCustomdrawSliderAy(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	mv_ay = mc_ay_slider.GetPos() * 50;
	UpdateData(false);
	UpdateData(true);
	*pResult = 0;
}
void Param::OnNMCustomdrawSliderAz(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	mv_az = mc_az_slider.GetPos() * 50;
	UpdateData(false);
	UpdateData(true);
	*pResult = 0;
}
void Param::OnNMCustomdrawSliderRx(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	mv_rx = mc_rx_slider.GetPos() * 50;
	UpdateData(false);
	UpdateData(true);
	*pResult = 0;
}
void Param::OnNMCustomdrawSliderRy(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	mv_ry = mc_ry_slider.GetPos() * 50;
	UpdateData(false);
	UpdateData(true);
	*pResult = 0;
}
void Param::OnNMCustomdrawSliderRz(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	mv_rz = mc_rz_slider.GetPos() * 50;
	UpdateData(false);
	UpdateData(true);
	*pResult = 0;
}

/*********************************编辑框处理函数*********************************/
//编辑框改变触发函数
void Param::OnEnChangeEditAx()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData(true);
	mc_ax_slider.SetPos((int)mv_ax/50);
	UpdateData(false);
	// TODO:  在此添加控件通知处理程序代码
}
void Param::OnEnChangeEditAy()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData(true);
	mc_ay_slider.SetPos((int)mv_ay / 50);
	UpdateData(false);
	// TODO:  在此添加控件通知处理程序代码
}
void Param::OnEnChangeEditAz()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData(true);
	mc_az_slider.SetPos((int)mv_az / 50);
	UpdateData(false);
	// TODO:  在此添加控件通知处理程序代码
}
void Param::OnEnChangeEditRx()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData(true);
	mc_rx_slider.SetPos((int)mv_rx / 50);
	UpdateData(false);
	// TODO:  在此添加控件通知处理程序代码
}
void Param::OnEnChangeEditRy()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData(true);
	mc_ry_slider.SetPos((int)mv_ry / 50);
	UpdateData(false);
	// TODO:  在此添加控件通知处理程序代码
}
void Param::OnEnChangeEditRz()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData(true);
	mc_rz_slider.SetPos((int)mv_rz / 50);
	UpdateData(false);
	// TODO:  在此添加控件通知处理程序代码
}

/*********************************按钮处理函数*********************************/
//默认值按钮处理函数
void Param::OnBnClickedButtondefault()
{
	// TODO:  在此添加控件通知处理程序代码
	mv_opp = 80;
	mv_oyp = 10;
	mv_orp = 80;
	mv_ipp = 7;
	mv_ipi = 2;
	mv_ipd = 12;
	mv_iyp = 100;
	mv_iyi = 30;
	mv_iyd = 0;
	mv_irp = 7;
	mv_iri = 2;
	mv_ird = 12;
	mv_ax = 500;
	mv_ay = 500;
	mv_az = 500;
	mv_rx = 0;
	mv_ry = 0;
	mv_rz = 0;
	UpdateData(false);
}

//写外环PID按钮处理函数
void Param::OnBnClickedButtonwriteouter()
{
	CGECFLY_WIFIDlg *parent = (CGECFLY_WIFIDlg *)GetParent();
	parent->SetOuterPid();

}

//写外环PID按钮处理函数
void Param::OnBnClickedButtonwriteinside()
{
	CGECFLY_WIFIDlg *parent = (CGECFLY_WIFIDlg *)GetParent();
	parent->SetInsidePid();
}

//写加速度按钮处理函数
void Param::OnBnClickedButtonwriteacc()
{
	CGECFLY_WIFIDlg *parent = (CGECFLY_WIFIDlg *)GetParent();
	parent->SetAcc();
}

//写陀螺仪按钮处理函数
void Param::OnBnClickedButtonwriterotate()
{
	OnBnClickedButtonwriteacc();
}

//读外环PID按钮处理函数
void Param::OnBnClickedButtonreadouter()
{
	GetDlgItem(IDC_Text_state1)->SetWindowTextW(_T(""));
	CGECFLY_WIFIDlg *parent = (CGECFLY_WIFIDlg *)GetParent();
	parent->Initbuf(parent->send_data);
	parent->send_data[1] = 0x20;
	parent->thread_senddata->ResumeThread();
	Sleep(300);
	parent->thread_senddata->SuspendThread();
	UpdateData(false);
}

//读内环PID按钮处理函数
void Param::OnBnClickedButtonreadinside()
{
	GetDlgItem(IDC_Text_state2)->SetWindowTextW(_T(""));
	CGECFLY_WIFIDlg *parent = (CGECFLY_WIFIDlg *)GetParent();
	parent->Initbuf(parent->send_data);
	parent->send_data[1] = 0x24;
	parent->thread_senddata->ResumeThread();
	Sleep(300);
	parent->thread_senddata->SuspendThread();
	UpdateData(false);
}

//读加速度按钮处理函数
void Param::OnBnClickedButtonreadacc()
{
	GetDlgItem(IDC_Text_state3)->SetWindowTextW(_T(""));
	CGECFLY_WIFIDlg *parent = (CGECFLY_WIFIDlg *)GetParent();
	parent->Initbuf(parent->send_data);
	parent->send_data[1] = 0x21;
	parent->thread_senddata->ResumeThread();
	Sleep(300);
	parent->thread_senddata->SuspendThread();
	UpdateData(false);
}

//读陀螺仪按钮处理函数
void Param::OnBnClickedButtonreadrotate()
{
	GetDlgItem(IDC_Text_state4)->SetWindowTextW(_T(""));
	OnBnClickedButtonreadacc();
}


/*********************************波形图处理函数*********************************/
//数组名，长度，新加入的数据
void Param::LeftMoveArray(double* ptr, size_t length, double data)
{
	for (UINT i = 1; i<length; ++i)
	{
		ptr[i - 1] = ptr[i];
	}
	ptr[length - 1] = data;
}


//定时绘线函数
void Param::OnTimer(UINT_PTR nIDEvent)
{
	if (0 == nIDEvent)
	{
		UpdateData(true);
		CString str;
		str.Format(_T("%u"), adc);
		GetDlgItem(IDC_Textadc)->SetWindowTextW(str);
		str.Format(_T("%u"), power);
		GetDlgItem(IDC_Text_power)->SetWindowTextW(str);
		if (mv_check_ax)        //roll
		{
			LeftMoveArray(y_ax, 360, (double)angle_x);
			p_Line[0]->ClearSerie();
			p_Line[0]->AddPoints(x, y_ax, 360);
		}
		if (mv_check_ay)        //pitch
		{
			LeftMoveArray(y_ay, 360, (double)angle_y);
			p_Line[1]->ClearSerie();
			p_Line[1]->AddPoints(x, y_ay, 360);
		}
		if (mv_check_az)        //yaw
		{
			LeftMoveArray(y_az, 360, (double)angle_z);
			p_Line[2]->ClearSerie();
			p_Line[2]->AddPoints(x, y_az, 360);
		}
		if (mv_check_gx)        //roll
		{
			LeftMoveArray(y_gx, 360, (double)gyroscope_x);
			p_Line[3]->ClearSerie();
			p_Line[3]->AddPoints(x, y_gx, 360);
		}
		if (mv_check_gy)        //pitch
		{
			LeftMoveArray(y_gy, 360, (double)gyroscope_y);
			p_Line[4]->ClearSerie();
			p_Line[4]->AddPoints(x, y_gy, 360);
		}
		if (mv_check_gz)        //yaw
		{
			LeftMoveArray(y_gz, 360, (double)gyroscope_z);
			p_Line[5]->ClearSerie();
			p_Line[5]->AddPoints(x, y_gz, 360);
		}
		if (mv_check_roll)        //roll
		{
			LeftMoveArray(y_roll, 360, (double)roll);
			p_Line[6]->ClearSerie();
			p_Line[6]->AddPoints(x, y_roll, 360);
		}
		if (mv_check_pitch)        //pitch
		{
			LeftMoveArray(y_pitch, 360, (double)pitch);
			p_Line[7]->ClearSerie();
			p_Line[7]->AddPoints(x, y_pitch, 360);
		}
		if (mv_check_yaw)        //yaw
		{
			LeftMoveArray(y_yaw, 360, (double)yaw);
			p_Line[8]->ClearSerie();
			p_Line[8]->AddPoints(x, y_yaw, 360);
		}
		if (mv_check_rroll)        //roll
		{
			LeftMoveArray(y_rroll, 360, (double)real_roll);
			p_Line[9]->ClearSerie();
			p_Line[9]->AddPoints(x, y_rroll, 360);
		}
		if (mv_check_rpitch)        //pitch
		{
			LeftMoveArray(y_rpitch, 360, (double)real_pitch);
			p_Line[10]->ClearSerie();
			p_Line[10]->AddPoints(x, y_rpitch, 360);
		}
		if (mv_check_ryaw)        //yaw
		{
			LeftMoveArray(y_ryaw, 360, (double)real_yaw);
			p_Line[11]->ClearSerie();
			p_Line[11]->AddPoints(x, y_ryaw, 360);
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}

/********************检测复选框函数*******************/
void Param::OnBnClickedcheckax()
{
	UpdateData(true);
	if (mv_check_ax);
	else
	{
		p_Line[0]->ClearSerie();
	}
}

void Param::OnBnClickedcheckay()
{
	UpdateData(true);
	if (mv_check_ay);
	else
	{
		p_Line[1]->ClearSerie();
	}
}

void Param::OnBnClickedcheckaz()
{
	UpdateData(true);
	if (mv_check_az);
	else
	{
		p_Line[2]->ClearSerie();
	}
}

void Param::OnBnClickedcheckgx()
{
	UpdateData(true);
	if (mv_check_gx);
	else
	{
		p_Line[3]->ClearSerie();
	}
}

void Param::OnBnClickedcheckgy()
{
	UpdateData(true);
	if (mv_check_gy);
	else
	{
		p_Line[4]->ClearSerie();
	}
}

void Param::OnBnClickedcheckgz()
{
	UpdateData(true);
	if (mv_check_gz);
	else
	{
		p_Line[5]->ClearSerie();
	}
}

void Param::OnBnClickedcheckroll()
{
	UpdateData(true);
	if (mv_check_roll);
	else
	{
		p_Line[6]->ClearSerie();
	}
}

void Param::OnBnClickedcheckpitch()
{
	UpdateData(true);
	if (mv_check_pitch);
	else
	{
		p_Line[7]->ClearSerie();
	}
}

void Param::OnBnClickedcheckyaw()
{
	UpdateData(true);
	if (mv_check_yaw);
	else
	{
		p_Line[8]->ClearSerie();
	}
}

void Param::OnBnClickedcheckrroll()
{
	UpdateData(true);
	if (mv_check_rroll);
	else
	{
		p_Line[9]->ClearSerie();
	}
}

void Param::OnBnClickedcheckrpitch()
{
	UpdateData(true);
	if (mv_check_rpitch);
	else
	{
		p_Line[10]->ClearSerie();
	}
}

void Param::OnBnClickedcheckryaw()
{
	UpdateData(true);
	if (mv_check_ryaw);
	else
	{
		p_Line[11]->ClearSerie();
	}
}

//开启绘图线程
void Param::OnBnClickeddrawstart()
{
	CGECFLY_WIFIDlg *parent = (CGECFLY_WIFIDlg *)GetParent();
	parent->thread_drawline->ResumeThread();
	parent->thread_recvdata->SuspendThread();
	GetDlgItem(IDC_drawstart)->EnableWindow(false);
	GetDlgItem(IDC_drawstop)->EnableWindow(true);
	mc_chartctrl.RemoveAllSeries();	 //先清空  
	for (int i = 0; i < 12; i++)
	{
		p_Line[i] = mc_chartctrl.CreateLineSerie();	 //创建一条线
	}
	SetTimer(0, 5, NULL);
	draw = true;
}

//关闭绘图线程
void Param::OnBnClickeddrawstop()
{
	CGECFLY_WIFIDlg *parent = (CGECFLY_WIFIDlg *)GetParent();
	parent->thread_drawline->SuspendThread();
	parent->thread_recvdata->ResumeThread();
	GetDlgItem(IDC_drawstop)->EnableWindow(false);
	GetDlgItem(IDC_drawstart)->EnableWindow(true);
	for (int i = 0; i < 12; i++)
	{
		p_Line[i]->ClearSerie();	
	}
	GetDlgItem(IDC_Textadc)->SetWindowTextW(_T(""));
	GetDlgItem(IDC_Text_power)->SetWindowTextW(_T(""));
	KillTimer(0);
	draw = false;
}

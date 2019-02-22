#pragma once
#include "afxcmn.h"
#include "afxdialogex.h"
#include "GECFLY_WIFI.h"


// Param 对话框

class Param : public CDialogEx
{
	DECLARE_DYNAMIC(Param)

public:
	Param(
	//class CGECFLY_WIFIDlg* p = NULL,
		CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Param();

// 对话框数据
	enum { IDD = IDD_Dialog_param };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	//pid数据变量
	UINT mv_opp;
	UINT mv_opi;
	UINT mv_opd;
	UINT mv_oyp;
	UINT mv_oyi;
	UINT mv_oyd;
	UINT mv_orp;
	UINT mv_ori;
	UINT mv_ord;
	UINT mv_ipp;
	UINT mv_ipi;
	UINT mv_ipd;
	UINT mv_iyp;
	UINT mv_iyi;
	UINT mv_iyd;
	UINT mv_irp;
	UINT mv_iri;
	UINT mv_ird;
	//姿态数据变量		
	UINT mv_ax;
	UINT mv_ay;
	UINT mv_az;
	UINT mv_rx;
	UINT mv_ry;
	UINT mv_rz;
	//pid按钮控制变量
	CSpinButtonCtrl mc_opp;
	CSpinButtonCtrl mc_opi;
	CSpinButtonCtrl mc_opd;
	CSpinButtonCtrl mc_oyp;
	CSpinButtonCtrl mc_oyi;
	CSpinButtonCtrl mc_oyd;
	CSpinButtonCtrl mc_orp;
	CSpinButtonCtrl mc_ori;
	CSpinButtonCtrl mc_ord;
	CSpinButtonCtrl mc_ipp;
	CSpinButtonCtrl mc_ipi;
	CSpinButtonCtrl mc_ipd;
	CSpinButtonCtrl mc_iyp;
	CSpinButtonCtrl mc_iyi;
	CSpinButtonCtrl mc_iyd;
	CSpinButtonCtrl mc_irp;
	CSpinButtonCtrl mc_iri;
	CSpinButtonCtrl mc_ird;
	//姿态拖动栏控制变量
	CSliderCtrl mc_ax_slider;
	CSliderCtrl mc_ay_slider;
	CSliderCtrl mc_az_slider;
	CSliderCtrl mc_rx_slider;
	CSliderCtrl mc_ry_slider;
	CSliderCtrl mc_rz_slider;
	
	//pid数据按钮函数
	afx_msg void OnDeltaposSpinOpp(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinOpi(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinOpd(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinOyp(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinOyi(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinOyd(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinOrp(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinOri(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinOrd(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinIpp(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinIpi(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinIpd(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinIyp(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinIyi(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinIyd(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinIrp(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinIri(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinIrd(NMHDR *pNMHDR, LRESULT *pResult);
	//姿态数据按钮函数
	afx_msg void OnDeltaposSpinAx(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinAy(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinAz(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinRx(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinRy(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinRz(NMHDR *pNMHDR, LRESULT *pResult);
	//姿态拖动栏函数
	afx_msg void OnNMCustomdrawSliderAx(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderAy(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderAz(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderRx(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderRy(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderRz(NMHDR *pNMHDR, LRESULT *pResult);
	//姿态数据编辑框改动函数
	afx_msg void OnEnChangeEditAx();
	afx_msg void OnEnChangeEditAy();
	afx_msg void OnEnChangeEditAz();
	afx_msg void OnEnChangeEditRx();
	afx_msg void OnEnChangeEditRy();
	afx_msg void OnEnChangeEditRz();
	afx_msg void OnBnClickedButtondefault();
	afx_msg void OnBnClickedButtonreadouter();
	//读写参数按钮处理函数
	afx_msg void OnBnClickedButtonwriteouter();
	afx_msg void OnBnClickedButtonreadinside();
	afx_msg void OnBnClickedButtonwriteinside();
	afx_msg void OnBnClickedButtonreadacc();
	afx_msg void OnBnClickedButtonwriteacc();
	afx_msg void OnBnClickedButtonreadrotate();
	afx_msg void OnBnClickedButtonwriterotate();


	//static UINT Thread_DrawLine(void *param);
	//CWinThread *thread_drawline;
	//void GetState();
	//绘图窗口控制变量
	CChartCtrl mc_chartctrl;
	//创建时间族类指针
	CChartAxis *pAxis;
	//十二条线分别为[加速度x/y/z 陀螺仪x/y/z 横滚/俯仰/航向 实际输出（横滚/俯仰/航向）]
	CChartLineSerie *p_Line[12];    //线1
	unsigned int RunCount[12];
	void LeftMoveArray(double* ptr, size_t length, double data);
	double x[360];
	double y_ax[360];
	double y_ay[360];
	double y_az[360];
	double y_gx[360];
	double y_gy[360];
	double y_gz[360];
	double y_roll[360];
	double y_pitch[360];
	double y_yaw[360];
	double y_rroll[360];
	double y_rpitch[360];
	double y_ryaw[360];

	short angle_x ;		//加速度x
	short angle_y;		//加速度y
	short angle_z;		//加速度z
	short gyroscope_x;	//陀螺仪x
	short gyroscope_y;	//陀螺仪y
	short gyroscope_z;	//陀螺仪z
	short roll;			//横滚
	short pitch;		//俯仰
	short yaw;			//航向
	short real_roll;	//横滚真实输出值
	short real_pitch;	//俯仰真实输出值
	short real_yaw;		//航向真实输出值
	short power;	//油门
	short adc;			//电压

	
	BOOL draw;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	
	afx_msg void OnBnClickedcheckax();
	afx_msg void OnBnClickedcheckay();
	afx_msg void OnBnClickedcheckaz();
	afx_msg void OnBnClickedcheckgx();
	afx_msg void OnBnClickedcheckgy();
	afx_msg void OnBnClickedcheckgz();
	afx_msg void OnBnClickedcheckroll();
	afx_msg void OnBnClickedcheckpitch();
	afx_msg void OnBnClickedcheckyaw();
	afx_msg void OnBnClickedcheckrroll();
	afx_msg void OnBnClickedcheckrpitch();
	afx_msg void OnBnClickedcheckryaw();
	BOOL mv_check_ax;
	BOOL mv_check_ay;
	BOOL mv_check_az;
	BOOL mv_check_gx;
	BOOL mv_check_gy;
	BOOL mv_check_gz;
	BOOL mv_check_roll;
	BOOL mv_check_pitch;
	BOOL mv_check_yaw;
	BOOL mv_check_rroll;
	BOOL mv_check_rpitch;
	BOOL mv_check_ryaw;
	afx_msg void OnBnClickeddrawstart();
	afx_msg void OnBnClickeddrawstop();
};

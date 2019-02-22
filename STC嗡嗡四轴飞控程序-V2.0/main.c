/*------------------------------------------------------------
此产品是基于STC宏晶公司，在2015年最新开发出一款STC15W4K（IAP15W4K）系列的
8位单片机。此系列在STC12系列的基础上 增加了宽电压和多路硬件PWM功能（15位）。
此条件满足了四轴飞行的基本设计需求，经过本人多方面学习与研究的过程中，
掌握了迷你四轴的飞行原理和制作具体要求设计参数等。

以下是“嗡嗡四轴”的程序更能说明：
单片机采用STC15W4K48S4-增强型单片机（IPA需修改EEPROM代码）
工作电压为：5V	晶振频率为：24M	无线通讯模块为：LT8910(无线收发)
本程序中 采用了MCU6050做姿态数据采集加PID算法给硬件4路PWM控制MOS管驱动空心杯电机
注意：此程序只兼容：空心杯716电机与45MM专用正反桨（自己的电机或需修改PID参数）
在后期的程序更新中，会增加硬件代码达到预期飞行功能。敬请关注。

开源程序-广大四轴爱好者都可以进行修改与技术交流。
您如果在此基础上能调出更好的飞行效果，可与设计者联系，有丰厚实物奖励哦。
原创设计者：联系方式QQ：250278069     STC迷你四轴技术交流群：421109612
淘宝网站：http://shop117773209.taobao.com/shop/view_shop.htm?spm=a1z0e.1.0.0.eqIhKz&mytmenu=mdianpu&utkn=g,meztsobuhe3to1427461542456&user_number_id=669230514&scm=1028.1.1.20001
制作完成时间：2016年8月8日-V2.0版
------------------------------------------------------------*/

#include <STC15W4K48S4.h> //STC15W4K48S4 专用头文件
#include <intrins.h>			//STC特殊命令符号声明
#include <MPU6050.H>			//MPU6050数字陀螺仪
#include <STC15W4K-PWM.H> //单片机所有IO口初始化-PWM口初始化
#include <LT8910.h>				//无线2.4G模块
//#include <STC15W4K-ADC.h>	//STC15W4K-ADC	硬件ADC模数转换
#include <IMU.H>					//IMU(惯性测量单元)飞行核心算法
#include <KalmanFilter.h> //卡尔曼滤波算法

//==================================================//
//  LED灯 引脚定义
//==================================================//
sbit LEDR = P0 ^ 1; //LED 红色 R 0亮1灭
sbit LEDG = P1 ^ 7; //LED 绿色 G
sbit LEDB = P1 ^ 6; //LED 蓝色 B

//sbit	KARX=P3^0; //排针接口 RXD
//sbit	KATX=P3^1; //排针接口 TXD
//==================================================//
//  飞行控制变量
//==================================================//
unsigned char JieSuo;		//断开/连接 解锁变量
unsigned char SSLL;			//通讯状态 变量
unsigned char ShiLian;	//失联变量
unsigned char SLJS;			//失联计数变量
unsigned int YouMen;		//油门变量
unsigned int HangXiang; //航向变量
unsigned int HengGun;		//横滚变量
unsigned int FuYang;		//俯仰变量
unsigned char FYHG;			//俯仰横滚变量
unsigned char Xiao1;		//CRC 算出效验包
unsigned char Xiao2;		//CRC 接收效验包

/*------------------------------------------------
                  全局函数定义
------------------------------------------------*/
unsigned char data RxBuf[20]; //设置接收长度，最高为32字节
//*****************飞控控制参数**************************
unsigned int YM = 0;																//油门变化速度控制，不这样做的话快速变化油门时四轴会失速翻转并GG
int speed0 = 0, speed1 = 0, speed2 = 0, speed3 = 0; //电机速度参数
int PWM0 = 0, PWM1 = 0, PWM2 = 0, PWM3 = 0;					//加载至PWM模块的参数
//double g_x=0,g_y=0,g_z=0;       //陀螺仪矫正参数
char a_x = 0, a_y = 0, a_z = 0; //角度矫正参数
float IMU_gz;
//*****************MPU-6050 寄存器数据参数**************************
double Gyro_y = 0, Gyro_x = 0, Gyro_z = 0;					//Y轴陀螺仪数据暂存
int xdata Angle_ax = 0, Angle_ay = 0, Angle_az = 0; //由加速度计算的加速度(弧度制)
int idata Angle_gy = 0, Angle_gx = 0, Angle_gz = 0; //由角速度计算的角速率(角度制)
int data AngleX = 0, AngleY = 0;										//四元数解算出的欧拉角(飞机倾斜角,10倍)  ,AngleZ=0
unsigned char data tp[16];
//==================================================//
//   PID算法变量
//==================================================//
float data PID_Output;				 //PID最终输出量
float xdata Last_Angle_gx = 0; //外环PI输出量  上一次陀螺仪数据
float xdata Last_Angle_gy = 0;
float xdata ERRORX_In = 0;	//内环P 内环I 内环D 内环误差积分
float xdata ERRORX_Out = 0; //外环P 外环I       外环误差积分
float xdata ERRORY_In = 0;
float xdata ERRORY_Out = 0;
float xdata ERRORZ_Out = 0;
float xdata Anglezlate = 0; //Z轴相关
float xdata Last_Ax = 0;
float xdata Last_Ay = 0;
float xdata Last_gx = 0;
float xdata Last_gy = 0;

/////////////////////////////////////////////////////////////
float xdata Ax = 0, Ay = 0;
Az = 0;										//加入遥控器控制量后的角度
int xdata gx = 0, gy = 0; //加入遥控器控制量后的角?
long xdata g_x_aver = 0;
long xdata g_y_aver = 0;
long xdata g_z_aver = 0;
int delta_rc_x = 0;
int delta_rc_y = 0;
int delta_rc_z = 0;
long idata PID_P;
long idata PID_I;
long idata PID_D;

#define Q15(X) \
	((X < 0.0) ? (int)(32768 * (X)-0.5) : (int)(32767 * (X) + 0.5))
#define KALMAN_Q Q15(0.20)
#define KALMAN_R Q15(0.80)
//==================================================//
//   PID 手动微调参数值
//   修改PID以下参数 可以吧飞控调的更加平稳
//   具体调试方法颗参考学习资料
//==================================================//
#define Out_XP 12.0f		 //外环P
#define Out_XI Q15(0.12) //外环I
#define Out_XD 5.2f			 //外环D
#define In_XP Q15(0.31)	//内环P
#define In_XI Q15(0.012) //内环I
#define In_XD 4.3f			 //内环D
#define Out_YP 12.0f		 //外环P
#define Out_YI Q15(0.12) //外环I
#define Out_YD 5.2f			 //外环D
#define In_YP Q15(0.31)	//内环P
#define In_YI Q15(0.012) //内环I
#define In_YD 4.3f			 //内环D
#define ZP 5.0f			 //Z轴 P
#define ZI Q15(0.24) //Z轴 I
#define ZD 4.0f			 //Z轴 D
#define ERR_MAX 800 //积分限幅

//--------------------------------------------------//
//  定时器0 初始化函数
//--------------------------------------------------//
void Timer0Init(void) //12毫秒@24.000MHz
{
	AUXR &= 0x7F; //定时器时钟12T模式
	TMOD &= 0xF0; //设置定时器模式
	TL0 = 0x40;		//设置定时初值
	TH0 = 0xA2;		//设置定时初值
	TF0 = 0;			//清除TF0标志
	TR0 = 1;			//定时器0开始计时
	ET0 = 1;			//Timer0 Interrupt Enable
}

//--------------------------------------------------//
//  时间延时 函数
//--------------------------------------------------//
void Delay(unsigned int x)
{
	unsigned int i, j;
	for (i = 0; i < x; i++)
		for (j = 0; j < 250; j++)
			;
}

//-------------------------------------------------//
//  PID算法飞控自平衡 函数	定时器0中断 12毫秒执行一次
//--------------------------------------------------//
void Flight(void) interrupt 1
{
	RX_model();					 //接收模式
	js_shuju(RxBuf, 15); //读取数据包
	Read_MPU6050(tp); //直接读取MPU6050陀螺仪和加速度的数据包

	Angle_ax = KalmanFilter_ax(((int *)&tp)[0], KALMAN_Q, KALMAN_R); //优化算法
	Angle_ay = KalmanFilter_ay(((int *)&tp)[1], KALMAN_Q, KALMAN_R);
	Angle_az = KalmanFilter_az(((int *)&tp)[2], KALMAN_Q, KALMAN_R);
	Angle_gx = ((float)(((int *)&tp)[4])) / 65.5; //陀螺仪处理	结果单位是 +-度
	Angle_gy = ((float)(((int *)&tp)[5])) / 65.5; //陀螺仪量程 +-500度/S, 1度/秒 对应读数 65.536
	Angle_gz = KalmanFilter_gyroz(((int *)&tp)[6], Q15(0.2), Q15(0.8));
	IMU_gz = Angle_gz / 65.5;
	Last_Angle_gx = Angle_gx; //储存上一次角速度数据
	Last_Angle_gy = Angle_gy;

	//*********************************** 四元数解算 ***********************************
	IMUupdate(Angle_gx * 0.0174533f, Angle_gy * 0.0174533f, IMU_gz * 0.0174533f, Angle_ax, Angle_ay, Angle_az); //姿态解算，精度0.1度
	///发送到遥控器
	//	TxBuf[0]=(AngleX+900)/0xff; // 数值是 48~1752 = 0-360度
	//	TxBuf[1]=(AngleX+900)%0xff;	// 数值是 48~1752 = 0-360度
	//	TxBuf[2]=(AngleY+900)/0xff;	// 数值是 48~1752 = 0-360度
	//	TxBuf[3]=(AngleY+900)%0xff;	// 数值是 48~1752 = 0-360度
	
	//飞控失联判断 自动降落算法,接收遥控器发来的不断更新数据 判断联机通讯是否正常
	if (SSLL == ShiLian) //如果SSLL的数据没有更新即失联
	{
		if (++SLJS >= 20)
		{
			SLJS = 19;			 //状态标识
			HangXiang = 128; //航向变量
			HengGun = 128;	 //横滚变量
			FuYang = 128;		 //俯仰变量
			if(YM > 20) YM--; //油门在原值逐渐减小
		}
	}else{
		SLJS = 0;
		if(YouMen > 1001) YouMen = 1000; //油门量0-1000最大值,油门优化算法 【将油门摇杆的控制幅度从60%增加到90%控制算法】如下
		else
		{ //摇杆量上50执行
			if(YouMen > 50) YM = (YouMen + 300) / 1.3;//摇杆增幅算法
			else YM = YouMen; //摇杆低于直接赋值
		}
	}
	ShiLian = SSLL; //失联变量更新
	YM = YouMen;
	//倾斜角度极限控制,极限值:30度
	if((AngleX + 900) > 1200)
	{
		LEDR = 0;
		YM = 0;
	} //飞控向右倾斜
	else if ((AngleX + 900) < 500)
	{
		LEDR = 0;
		YM = 0;
	} //飞控向左倾斜
	else if ((AngleY + 900) > 1200)
	{
		LEDR = 0;
		YM = 0;
	} //飞控向前倾斜
	else if ((AngleY + 900) < 500)
	{
		LEDR = 0;
		YM = 0;
	} //飞控向后倾斜
	else LEDR = 1; //灭红灯

	//飞行控制算法,MPU6050 X轴指向
	delta_rc_x = ((float)HengGun - 128) * 2; //得到 横滚数据变量
	Ax = -AngleX - delta_rc_x + a_x * 5;		 //
	//	Ax =-AngleX+a_x*5;
	if (YM > 20) ERRORX_Out += Ax; //外环积分(油门小于某个值时不积分)
	else ERRORX_Out = 0; //油门小于定值时清除积分值
	if (ERRORX_Out > ERR_MAX) ERRORX_Out = ERR_MAX; //积分限幅
	else if (ERRORX_Out < -ERR_MAX) ERRORX_Out = -ERR_MAX; //积分限幅

	PID_P = (long)Ax * Out_YP;
	PID_I = ((long)ERRORX_Out * Out_XI) >> 15;
	PID_D = ((Angle_gy + Last_Angle_gy) / 2) * Out_XD;
	PID_Output = (PID_P + PID_I + PID_D + 5) / 10; //外环PID

	Last_Ax = Ax;
	gx = PID_Output - Angle_gy; //外环 -   陀螺仪Y轴

	if (YM > 20) ERRORX_In += gx; //内环积分(油门小于某个值时不积分)
	else ERRORX_In = 0; //油门小于定值时清除积分值
	if (ERRORX_In > ERR_MAX) ERRORX_In = ERR_MAX;
	else if (ERRORX_In < -ERR_MAX) ERRORX_In = -ERR_MAX; //积分限幅

	PID_P = ((long)gx * In_XP) >> 15;
	PID_I = ((long)ERRORX_In * In_XI) >> 15;
	PID_D = ((long)gx - Last_gx) * In_XD;
	PID_Output = PID_P + PID_I + PID_D; //内环PID
	Last_gx = gx;

	if(PID_Output > 1000) PID_Output = 1000; //输出量限幅
	if(PID_Output < -1000) PID_Output = -1000;
	speed0 = 0 + PID_Output;
	speed1 = 0 - PID_Output;
	speed3 = 0 + PID_Output;
	speed2 = 0 - PID_Output;
	//**************MPU6050 Y轴指向**************************************************
	delta_rc_y = ((float)FuYang - 128) * 2; //得到 俯仰数据变量
	Ay = -AngleY - delta_rc_y - a_y * 5;
	//Ay  =-AngleY-a_y*5;
	if (YM > 20) ERRORY_Out += Ay; //外环积分(油门小于某个值时不积分)
	else ERRORY_Out = 0; //油门小于定值时清除积分值

	if(ERRORY_Out > ERR_MAX) ERRORY_Out = ERR_MAX;
	else if(ERRORY_Out < -ERR_MAX) ERRORY_Out = -ERR_MAX; //积分限幅

	PID_P = (long)Ay * Out_YP;
	PID_I = ((long)ERRORY_Out * Out_YI) >> 15;
	PID_D = ((Angle_gx + Last_Angle_gx) / 2) * Out_YD;
	PID_Output = (PID_P + PID_I + PID_D + 5) / 10; //外环PID，“+5”为了四舍五入?

	Last_Ay = Ay;
	gy = PID_Output - Angle_gx;

	if (YM > 20) ERRORY_In += gy; //内环积分(油门小于某个值时不积分)
	else ERRORY_In = 0; //油门小于定值时清除积分值
	if(ERRORY_In > ERR_MAX) ERRORY_In = ERR_MAX;
	else if(ERRORY_In < -ERR_MAX) ERRORY_In = -ERR_MAX; //积分限幅

	PID_P = ((long)gy * In_YP) >> 15;
	PID_I = ((long)ERRORY_In * In_YI) >> 15;
	PID_D = ((long)gy - Last_gy) * In_YD;
	PID_Output = PID_P + PID_I + PID_D;
	Last_gy = gy;

	if(PID_Output > 1000) PID_Output = 1000; //输出量限幅
	if(PID_Output < -1000) PID_Output = -1000;
	speed0 = speed0 + PID_Output;
	speed1 = speed1 + PID_Output; //加载到速度参数
	speed3 = speed3 - PID_Output;
	speed2 = speed2 - PID_Output;

	//************** MPU6050 Z轴指向 *****************************
	delta_rc_z = -Angle_gz + ((float)HangXiang - 128) * 65 + a_z * 20; //得到 航向数据变量 操作量
	if(YM > 20) ERRORZ_Out += delta_rc_z;
	else ERRORZ_Out = 0;
	if(ERRORZ_Out > 50000) ERRORZ_Out = 50000;
	else if (ERRORZ_Out < -50000) ERRORZ_Out = -50000; //积分限幅
	PID_P = ((long)delta_rc_z) * ZP;
	PID_I = ((long)ERRORZ_Out * ZI) >> 15;
	PID_D = ((long)delta_rc_z - Anglezlate) * ZD;
	PID_Output = (PID_P + PID_I + PID_D) >> 6;

	Anglezlate = delta_rc_z;
	speed0 = speed0 + PID_Output;
	speed1 = speed1 - PID_Output;
	speed3 = speed3 - PID_Output;
	speed2 = speed2 + PID_Output;
	//速度参数控制(速度参数加载至PWM)防止超过PWM参数范围0-1000（X型有效）
	PWM0 = (YM + speed0);
	if(PWM0 > 1000) PWM0 = 1000;
	else if (PWM0 < 0) PWM0 = 0;
	PWM1 = (YM + speed1);
	if(PWM1 > 1000) PWM1 = 1000;
	else if (PWM1 < 0) PWM1 = 0;
	PWM2 = (YM + speed2);
	if(PWM2 > 1000) PWM2 = 1000;
	else if (PWM2 < 0) PWM2 = 0;
	PWM3 = (YM + speed3);
	if(PWM3 > 1000) PWM3 = 1000;
	else if (PWM3 < 0) PWM3 = 0;
	//满足条件:(解锁：2.4G=5；油门大于30）才能控制电机)
	if(JieSuo == 5 && YM >= 30) PWM(1000 - PWM1, 1000 - PWM2, 1000 - PWM3, 1000 - PWM0);//启动PWM
	else PWM(1000, 1000, 1000, 1000);//关闭PWM
}

void main(void)
{
	PWMGO();										 //初始化PWM
	PWM(1000, 1000, 1000, 1000); //关闭电机
	LEDR = 0;
	LEDG = 1;
	LEDB = 1;		
	Delay(100); 
	LEDR = 1;
	LEDG = 0;
	LEDB = 1;		
	Delay(100); 
	LEDR = 1;
	LEDG = 1;
	LEDB = 0;		
	Delay(100);
	LEDR = 1;
	LEDG = 1;
	LEDB = 1;				 
	Delay(10);				 
	//	InitADC();		//ADC模数转换 初始化（后期开发）
	//	Delay(10);    // 延时 100
	Init_MPU6050();	//初始化MPU-6050
	Delay(10);			 // 延时 100
	LT8910_Init();	 //无线2.4G模块初始化
	Delay(100);			 //延时一会 1S
	Timer0Init();		 //初始化定时器
	Delay(100);			 //延时一会 1S
	YouMen = 0;			 //初始化油门变量
	HangXiang = 128; //初始化航向变量
	HengGun = 128;	 //初始化横滚变量
	FuYang = 128;		 //初始化俯仰变量
	LEDB = 0;				 //开启蓝灯
	a_x = 0;				 // 横滚手动值
	a_y = 0;				 // 俯仰手动值
	a_z = 0;				 // 航向手动值
	//Flight();//编译后2个警告是说 飞控函数中断量 不在主函数里【不需要纠结】
	EA = 1; //开总中断
	while (1)
	{
		//要放到定时器中断里运行
		//	RX_model();				//接收模式
		//	js_shuju(RxBuf,15);		//读取数据包
		Xiao1 = RxBuf[0] + RxBuf[1] + RxBuf[2] + RxBuf[3] + RxBuf[4] + RxBuf[5] + RxBuf[6] + RxBuf[7] + RxBuf[8] + RxBuf[9];
		Xiao2 = RxBuf[10];
		if (Xiao1 == Xiao2)
		{
			SSLL = RxBuf[0];										 //接收 失联变量
			JieSuo = RxBuf[1];									 //接收 命令值
			YouMen = RxBuf[2] * 0xff + RxBuf[3]; //接收 油门变量
			HangXiang = RxBuf[4];								 //接收 航向变量
			HengGun = RxBuf[5];									 //接收 横滚变量
			FuYang = RxBuf[6];									 //接收 俯仰变量
			a_x = RxBuf[7] - 128;								 //读出 X轴保存值
			a_y = RxBuf[8] - 128;								 //读出 Y轴保存值
			a_z = RxBuf[9] - 128;								 //读出 Z轴保存值
			LEDG = 1;														 //LED 绿
		}else LEDG = 0; //亮绿
		if(JieSuo == 1) LEDB = 1;//灭蓝
		if(JieSuo == 5) LEDB = 0;//亮蓝
		Delay(3); //延时一会
		//ADC电压低压检测自停保护功能:未实现
	}
}

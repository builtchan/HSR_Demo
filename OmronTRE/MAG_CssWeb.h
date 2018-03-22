#ifndef MAGCSSWEB_H_
#define MAGCSSWEB_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

#define MAGCSSWEB_OK 0
#define MAGCSSWEB_FAIL 1
#define MAG_COM_NO 2

typedef struct
{
	bool bFlag;
	int iDataLen;	//一个磁道信息长度
	unsigned char ucData[256];//一个磁道全信息
}T_EW_IPC_MAGCARD_TRACK_INFO;

typedef T_EW_IPC_MAGCARD_TRACK_INFO tMagCardTrackInfo[9];

typedef struct
{
	tMagCardTrackInfo tWriteMagInfo;//全部磁信息
	int iPrintFlagInfo;//打印标志
} T_EW_WRITE_PRINT_MAGCARD_INFO;

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MYDLL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MYDLL_API_MAG functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef MYDLL_EXPORTS
#define MYDLL_API_MAG  __declspec(dllexport)
#else
#define MYDLL_API_MAG  __declspec(dllimport)
#endif

#define MAG_CSSWEB_STDCALL WINAPI

#ifdef __cplusplus
extern "C" {
#endif

//#define  MYDLL_API_MAG extern "C"
// This class is exported from the mydll.dll
// class MYDLL_API_MAG CMydll {
// public:
// 	CMydll(void);
// 	// TODO: add your methods here.
// };

//extern MYDLL_API_MAG int nMydll;

//动态库函数
/* MAG_Init
函数功能：初始化磁模块，若通道内有票，弹票到出口
入参：保留
出参：pOut:8位字符串,表示物理错误码（3位厂家代码+5位错误码）
返回值：1:失败  0:成功
*/
MYDLL_API_MAG unsigned short MAG_CSSWEB_STDCALL MAG_Init(char *pIn, char *pOut);


/* MAG_Uninit
函数功能：释放已初始化的磁模块设备
入参：保留
出参：pOut:8位字符串,表示物理错误码（3位厂家代码+5位错误码）
返回值：1:失败  0:成功
*/
MYDLL_API_MAG unsigned short MAG_CSSWEB_STDCALL MAG_Uninit(char *pIn, char *pOut);


/* MAG_CheckTicketPos
函数功能：检查入口或者出口是否有票
入参：
	pIn:“1”：入口
		“2”：出口
出参：pOut:返回成功时：“0”：无票 “1”：有票
	  返回错误时：错误描述
返回值：0:成功 其他：参见状态表
*/
MYDLL_API_MAG unsigned short MAG_CSSWEB_STDCALL MAG_CheckTicketPos(char *pIn, char *pOut);


/* MAG_Read
函数功能：吸票，读取磁数据
入参：
pIn:保留
出参：pOut:
	失败：8位字符串，表示物理错误码（3位厂家代码+5位错误码）
	成功：读取到的磁数据（读磁结构体T_EW_IPC_MAGCARD_TRACK_INFO）
返回错误时：错误描述
返回值：0:成功 其他：参见状态表
*/
MYDLL_API_MAG unsigned short MAG_CSSWEB_STDCALL MAG_Read(char *pIn, char *pOut);


/* MAG_Eject
函数功能：弹票到入口或出口
入参：
pIn:"1":入口
	"2":出口
出参：pOut:
	失败：8位字符串，表示物理错误码（3位厂家代码+5位错误码）
返回错误时：错误描述
返回值：0:成功 其他：参见状态表
*/
MYDLL_API_MAG unsigned short MAG_CSSWEB_STDCALL MAG_Eject(char *pIn, char *pOut);


/* MAG_WritePrintEject
函数功能：写磁数据打印标记弹票到出口
入参：pIn:磁数据以及打印标记数据（写磁结构体T_EW_WRITE_PRINT_MAGCARD_INFO）
出参：
	pOut:
失败：8位字符串，表示物理错误码（3位厂家代码+5位错误码）
返回错误时：错误描述
返回值：0:成功 其他：参见状态表
*/
MYDLL_API_MAG unsigned short MAG_CSSWEB_STDCALL MAG_WritePrintEject(char *pIn, char *pOut);


/* MAG_CheckStart
函数功能：检票功能开启,打开入票口阀门
入参：pIn:保留
出参：pOut:8位字符串，表示物理错误码（3位厂家代码+5位错误码）
返回错误时：错误描述
返回值：0:成功 其他：参见状态表
*/
MYDLL_API_MAG unsigned short MAG_CSSWEB_STDCALL MAG_CheckStart(char *pIn, char *pOut);



/* MAG_CheckStop
函数功能：关闭入票口阀门
入参：pIn:保留
出参：pOut:无
返回错误时：错误描述
返回值：0:成功 其他：参见状态表
*/
MYDLL_API_MAG unsigned short MAG_CSSWEB_STDCALL MAG_CheckStop(char *pIn, char *pOut);


/* MAG_LightControl
函数功能：磁模块显示灯颜色
入参：pIn:具体的控制指令字符串,每个字节代表一种灯，
	第1字节：磁入口灯
	第2字节：磁出口灯
	第3 4字节：预留
	灯的颜色定义：
	'0' 灯默认状态（不用控制）
	'1' 绿灯
	'3' 红灯
	'4' 黄灯
	'5' 闪绿灯
	'6' 闪红灯
	'7' 闪黄灯
	'9' 灭灯
出参：pOut::8位字符串，表示物理错误码（3位厂家代码+5位错误码）
返回错误时：错误描述
返回值：0:成功 1:失败
*/
MYDLL_API_MAG unsigned short MAG_CSSWEB_STDCALL MAG_LightControl(char *pIn, char *pOut);



/* MAG_GetStatus
函数功能：获取磁模块的状态
入参：pIn:保留
出参：pOut:8位字符串，表示物理错误码（3位厂家代码+5位错误码）
返回错误时：错误描述
返回值：0:成功 其他：参见状态表
*/
MYDLL_API_MAG unsigned short MAG_CSSWEB_STDCALL MAG_GetStatus(char *pIn, char *pOut);

void untoCstring(unsigned char * pDataText, char *szDest, int length);

#ifdef __cplusplus
}
#endif
#endif
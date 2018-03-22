#ifndef GATCSSWEB_H_
#define GATCSSWEB_H_

#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <direct.h>
#include <time.h>

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MYDLL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MYDLL_API_GAT functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef MYDLL_EXPORTS
#define MYDLL_API_GAT __declspec(dllexport)
#else
#define MYDLL_API_GAT __declspec(dllimport)
#endif


#define GAT_CSSWEB_STDCALL WINAPI
#ifdef __cplusplus
extern "C" {
#endif
// This class is exported from the mydll.dll
// class MYDLL_API_GAT CMydll {
// public:
// 	CMydll(void);
// 	// TODO: add your methods here.
// };

//extern MYDLL_API_GAT int nMydll;

//动态库函数
/* GAT_Init
函数功能：对闸机门模块进行初始化设置
入参：保留
出参：8位字符串，表示物理错误码（3位厂家代码+5位错误码）
返回值：1:失败  0:成功   其他：参见状态表
*/
MYDLL_API_GAT unsigned short GAT_CSSWEB_STDCALL GAT_Init(char *pIn, char *pOut);

/* GAT_Uninit
函数功能：用于卸载已初始化的设备
入参：保留
出参：8位字符串，表示物理错误码（3位厂家代码+5位错误码）
返回值：1:失败  0:成功   其他：参见状态表
*/
MYDLL_API_GAT unsigned short GAT_CSSWEB_STDCALL GAT_Uninit(char *pIn, char *pOut);


/* GAT_SetMode
函数功能：设置门模式函数
入参：“1”：常开状态  “2”：关闭状态（正常）
出参：8位字符串，表示物理错误码（3位厂家代码+5位错误码）
返回值：1:失败  0:成功   其他：参见状态表
*/
MYDLL_API_GAT unsigned short GAT_CSSWEB_STDCALL GAT_SetMode(char *pIn, char *pOut);


/* GAT_SetLampAndBeepStatus
函数功能：向闸机指示灯发控制命令
入参：为具体的控制指令字符串，每个字节代表一种灯，第一个字节：蜂鸣器，第二个字节：门顶灯，
第三个字节：门前端显示灯，第四个字节：门后端显示灯，后面字节预留
	灯的颜色定义：
	'0' 灯默认状态（不用控制）
	'1' 绿灯
	'3' 红灯
	'4' 黄灯
	'5' 闪绿灯
	'6' 闪红灯
	'7' 闪黄灯
	'9' 灭灯
蜂鸣器控制：
	‘0’蜂鸣器默认状态（不用控制）
	‘1’蜂鸣器停止响声
	‘2’蜂鸣器开启响声
出参：8位字符串，表示物理错误码（3位厂家代码+5位错误码）
返回值：1:失败  0:成功   其他：参见状态表
*/
MYDLL_API_GAT unsigned short GAT_CSSWEB_STDCALL GAT_SetLampAndBeepStatus(char *pIn, char *pOut);


/* GAT_Control
函数功能：控制闸门打开，关闭
入参：具体控制指令的字符串
"1":关门指令（预留暂不实现）
"2":开门指令
"3":通过闸机设置禁止闯闸报警
出参：8位字符串，表示物理错误码（3位厂家代码+5位错误码）
返回值：1:失败  0:成功   其他：参见状态表
*/
MYDLL_API_GAT unsigned short GAT_CSSWEB_STDCALL GAT_Control(char *pIn, char *pOut);


/* GAT_GetStatus
函数功能：获取通道状态函数,需要不停的轮询
入参：保留
出参：8位字符串，表示物理错误码（3位厂家代码+5位错误码）
返回值：1:失败  0:成功   其他：参见状态表
*/
MYDLL_API_GAT unsigned short GAT_CSSWEB_STDCALL GAT_GetStatus(char *pIn, char *pOut);

/* GAT_GetDeviceInfo
函数功能：读取硬件的信息。包括：厂家全称，厂家简称(4位)，设备型号，出厂日期，序列号(唯一)，闸门唯一编号，闸门型号，闸门品牌。
																				共8项，以逗号分隔。前5项是整机信息，后3项是闸门信息
入参：保留
出参：返回值是成功时，硬件信息的字符串；
				返回值是失败时，8位字符串，表示物理错误码(3位厂家代码+5位错误码)
返回值：1:失败  0:成功   其他：参见状态表
*/
MYDLL_API_GAT unsigned short GAT_CSSWEB_STDCALL GAT_GetDeviceInfo(char *pIn, char *pOut);

#ifdef __cplusplus
}
#endif

#endif

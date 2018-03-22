#ifndef VOSCSSWEB_H_
#define VOSCSSWEB_H_

#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <direct.h>
#include <time.h>
#include <Mmsystem.h>

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


#define VOS_CSSWEB_STDCALL WINAPI
#ifdef __cplusplus
extern "C" {
#endif

//动态库函数
/* VOS_Init
函数功能：对音频模块进行初始化设置
入参：保留
出参：8位字符串，表示物理错误码（3位厂家代码+5位错误码）
返回值：1:失败  0:成功   其他：参见状态表
*/
MYDLL_API_GAT unsigned short VOS_CSSWEB_STDCALL VOS_Init(char *pIn, char *pOut);

/* VOS_Uninit
函数功能：用于卸载已初始化的设备
入参：保留
出参：8位字符串，表示物理错误码（3位厂家代码+5位错误码）
返回值：1:失败  0:成功   其他：参见状态表
*/
MYDLL_API_GAT unsigned short VOS_CSSWEB_STDCALL VOS_Uninit(char *pIn, char *pOut);


/* VOS_DisplayVoice
函数功能：播放语音。根据状态码或者语音字符串，播放提示语音
入参：pStr：	6位状态码+语音字符串。状态码见《铁路人脸识别设备软件接口规范V1.3.doc》的状态表
				bwait:		是否等待，不等待就抢占播放。默认是false为抢占播放；ture为等待播放。
				nret :		播放语速，默认0
输出参数：无
返回值：1:失败  0:成功   其他：参见状态表
*/
MYDLL_API_GAT unsigned short VOS_CSSWEB_STDCALL VOS_DisplayVoice (char *pStr, bool bwait = false, int nret = 0);


#ifdef __cplusplus
}
#endif

#endif

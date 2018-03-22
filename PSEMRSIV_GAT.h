#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <direct.h>
#include <time.h>
#include <Mmsystem.h>

#define MYDLL_API_GA __declspec(dllexport)

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _ST_MODULEINFO_
{
	char ID[32]; // 模块编号（唯一）;
    char Model[32]; // 模块型号;
    char Brand[32]; // 模块厂商（厂商全称）;
	char Version[32]; //版本号;格式:x.x.x.x;
	char VerBuildTime[80];//版本生成时间，格式:yyyy-mm-dd HH:MM:SS;
} ModuleInfo[2];

//动态库函数
/*
函数功能：
读取闸门模块设备静态详细信息。
输入参数：无
输出参数：

*/
MYDLL_API_GA int  WINAPI GAT_GetModuleInfo(ModuleInfo * p_ModuleInfo);

#ifdef __cplusplus
}
#endif


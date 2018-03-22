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
	char ID[32]; // ģ���ţ�Ψһ��;
    char Model[32]; // ģ���ͺ�;
    char Brand[32]; // ģ�鳧�̣�����ȫ�ƣ�;
	char Version[32]; //�汾��;��ʽ:x.x.x.x;
	char VerBuildTime[80];//�汾����ʱ�䣬��ʽ:yyyy-mm-dd HH:MM:SS;
} ModuleInfo[2];

//��̬�⺯��
/*
�������ܣ�
��ȡբ��ģ���豸��̬��ϸ��Ϣ��
�����������
���������

*/
MYDLL_API_GA int  WINAPI GAT_GetModuleInfo(ModuleInfo * p_ModuleInfo);

#ifdef __cplusplus
}
#endif


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

//��̬�⺯��
/* VOS_Init
�������ܣ�����Ƶģ����г�ʼ������
��Σ�����
���Σ�8λ�ַ�������ʾ��������루3λ���Ҵ���+5λ�����룩
����ֵ��1:ʧ��  0:�ɹ�   �������μ�״̬��
*/
MYDLL_API_GAT unsigned short VOS_CSSWEB_STDCALL VOS_Init(char *pIn, char *pOut);

/* VOS_Uninit
�������ܣ�����ж���ѳ�ʼ�����豸
��Σ�����
���Σ�8λ�ַ�������ʾ��������루3λ���Ҵ���+5λ�����룩
����ֵ��1:ʧ��  0:�ɹ�   �������μ�״̬��
*/
MYDLL_API_GAT unsigned short VOS_CSSWEB_STDCALL VOS_Uninit(char *pIn, char *pOut);


/* VOS_DisplayVoice
�������ܣ���������������״̬����������ַ�����������ʾ����
��Σ�pStr��	6λ״̬��+�����ַ�����״̬�������·����ʶ���豸����ӿڹ淶V1.3.doc����״̬��
				bwait:		�Ƿ�ȴ������ȴ�����ռ���š�Ĭ����falseΪ��ռ���ţ�tureΪ�ȴ����š�
				nret :		�������٣�Ĭ��0
�����������
����ֵ��1:ʧ��  0:�ɹ�   �������μ�״̬��
*/
MYDLL_API_GAT unsigned short VOS_CSSWEB_STDCALL VOS_DisplayVoice (char *pStr, bool bwait = false, int nret = 0);


#ifdef __cplusplus
}
#endif

#endif

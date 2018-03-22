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

//��̬�⺯��
/* GAT_Init
�������ܣ���բ����ģ����г�ʼ������
��Σ�����
���Σ�8λ�ַ�������ʾ��������루3λ���Ҵ���+5λ�����룩
����ֵ��1:ʧ��  0:�ɹ�   �������μ�״̬��
*/
MYDLL_API_GAT unsigned short GAT_CSSWEB_STDCALL GAT_Init(char *pIn, char *pOut);

/* GAT_Uninit
�������ܣ�����ж���ѳ�ʼ�����豸
��Σ�����
���Σ�8λ�ַ�������ʾ��������루3λ���Ҵ���+5λ�����룩
����ֵ��1:ʧ��  0:�ɹ�   �������μ�״̬��
*/
MYDLL_API_GAT unsigned short GAT_CSSWEB_STDCALL GAT_Uninit(char *pIn, char *pOut);


/* GAT_SetMode
�������ܣ�������ģʽ����
��Σ���1��������״̬  ��2�����ر�״̬��������
���Σ�8λ�ַ�������ʾ��������루3λ���Ҵ���+5λ�����룩
����ֵ��1:ʧ��  0:�ɹ�   �������μ�״̬��
*/
MYDLL_API_GAT unsigned short GAT_CSSWEB_STDCALL GAT_SetMode(char *pIn, char *pOut);


/* GAT_SetLampAndBeepStatus
�������ܣ���բ��ָʾ�Ʒ���������
��Σ�Ϊ����Ŀ���ָ���ַ�����ÿ���ֽڴ���һ�ֵƣ���һ���ֽڣ����������ڶ����ֽڣ��Ŷ��ƣ�
�������ֽڣ���ǰ����ʾ�ƣ����ĸ��ֽڣ��ź����ʾ�ƣ������ֽ�Ԥ��
	�Ƶ���ɫ���壺
	'0' ��Ĭ��״̬�����ÿ��ƣ�
	'1' �̵�
	'3' ���
	'4' �Ƶ�
	'5' ���̵�
	'6' �����
	'7' ���Ƶ�
	'9' ���
���������ƣ�
	��0��������Ĭ��״̬�����ÿ��ƣ�
	��1��������ֹͣ����
	��2����������������
���Σ�8λ�ַ�������ʾ��������루3λ���Ҵ���+5λ�����룩
����ֵ��1:ʧ��  0:�ɹ�   �������μ�״̬��
*/
MYDLL_API_GAT unsigned short GAT_CSSWEB_STDCALL GAT_SetLampAndBeepStatus(char *pIn, char *pOut);


/* GAT_Control
�������ܣ�����բ�Ŵ򿪣��ر�
��Σ��������ָ����ַ���
"1":����ָ�Ԥ���ݲ�ʵ�֣�
"2":����ָ��
"3":ͨ��բ�����ý�ֹ��բ����
���Σ�8λ�ַ�������ʾ��������루3λ���Ҵ���+5λ�����룩
����ֵ��1:ʧ��  0:�ɹ�   �������μ�״̬��
*/
MYDLL_API_GAT unsigned short GAT_CSSWEB_STDCALL GAT_Control(char *pIn, char *pOut);


/* GAT_GetStatus
�������ܣ���ȡͨ��״̬����,��Ҫ��ͣ����ѯ
��Σ�����
���Σ�8λ�ַ�������ʾ��������루3λ���Ҵ���+5λ�����룩
����ֵ��1:ʧ��  0:�ɹ�   �������μ�״̬��
*/
MYDLL_API_GAT unsigned short GAT_CSSWEB_STDCALL GAT_GetStatus(char *pIn, char *pOut);

/* GAT_GetDeviceInfo
�������ܣ���ȡӲ������Ϣ������������ȫ�ƣ����Ҽ��(4λ)���豸�ͺţ��������ڣ����к�(Ψһ)��բ��Ψһ��ţ�բ���ͺţ�բ��Ʒ�ơ�
																				��8��Զ��ŷָ���ǰ5����������Ϣ����3����բ����Ϣ
��Σ�����
���Σ�����ֵ�ǳɹ�ʱ��Ӳ����Ϣ���ַ�����
				����ֵ��ʧ��ʱ��8λ�ַ�������ʾ���������(3λ���Ҵ���+5λ������)
����ֵ��1:ʧ��  0:�ɹ�   �������μ�״̬��
*/
MYDLL_API_GAT unsigned short GAT_CSSWEB_STDCALL GAT_GetDeviceInfo(char *pIn, char *pOut);

#ifdef __cplusplus
}
#endif

#endif

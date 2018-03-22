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
	int iDataLen;	//һ���ŵ���Ϣ����
	unsigned char ucData[256];//һ���ŵ�ȫ��Ϣ
}T_EW_IPC_MAGCARD_TRACK_INFO;

typedef T_EW_IPC_MAGCARD_TRACK_INFO tMagCardTrackInfo[9];

typedef struct
{
	tMagCardTrackInfo tWriteMagInfo;//ȫ������Ϣ
	int iPrintFlagInfo;//��ӡ��־
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

//��̬�⺯��
/* MAG_Init
�������ܣ���ʼ����ģ�飬��ͨ������Ʊ����Ʊ������
��Σ�����
���Σ�pOut:8λ�ַ���,��ʾ��������루3λ���Ҵ���+5λ�����룩
����ֵ��1:ʧ��  0:�ɹ�
*/
MYDLL_API_MAG unsigned short MAG_CSSWEB_STDCALL MAG_Init(char *pIn, char *pOut);


/* MAG_Uninit
�������ܣ��ͷ��ѳ�ʼ���Ĵ�ģ���豸
��Σ�����
���Σ�pOut:8λ�ַ���,��ʾ��������루3λ���Ҵ���+5λ�����룩
����ֵ��1:ʧ��  0:�ɹ�
*/
MYDLL_API_MAG unsigned short MAG_CSSWEB_STDCALL MAG_Uninit(char *pIn, char *pOut);


/* MAG_CheckTicketPos
�������ܣ������ڻ��߳����Ƿ���Ʊ
��Σ�
	pIn:��1�������
		��2��������
���Σ�pOut:���سɹ�ʱ����0������Ʊ ��1������Ʊ
	  ���ش���ʱ����������
����ֵ��0:�ɹ� �������μ�״̬��
*/
MYDLL_API_MAG unsigned short MAG_CSSWEB_STDCALL MAG_CheckTicketPos(char *pIn, char *pOut);


/* MAG_Read
�������ܣ���Ʊ����ȡ������
��Σ�
pIn:����
���Σ�pOut:
	ʧ�ܣ�8λ�ַ�������ʾ��������루3λ���Ҵ���+5λ�����룩
	�ɹ�����ȡ���Ĵ����ݣ����Žṹ��T_EW_IPC_MAGCARD_TRACK_INFO��
���ش���ʱ����������
����ֵ��0:�ɹ� �������μ�״̬��
*/
MYDLL_API_MAG unsigned short MAG_CSSWEB_STDCALL MAG_Read(char *pIn, char *pOut);


/* MAG_Eject
�������ܣ���Ʊ����ڻ����
��Σ�
pIn:"1":���
	"2":����
���Σ�pOut:
	ʧ�ܣ�8λ�ַ�������ʾ��������루3λ���Ҵ���+5λ�����룩
���ش���ʱ����������
����ֵ��0:�ɹ� �������μ�״̬��
*/
MYDLL_API_MAG unsigned short MAG_CSSWEB_STDCALL MAG_Eject(char *pIn, char *pOut);


/* MAG_WritePrintEject
�������ܣ�д�����ݴ�ӡ��ǵ�Ʊ������
��Σ�pIn:�������Լ���ӡ������ݣ�д�Žṹ��T_EW_WRITE_PRINT_MAGCARD_INFO��
���Σ�
	pOut:
ʧ�ܣ�8λ�ַ�������ʾ��������루3λ���Ҵ���+5λ�����룩
���ش���ʱ����������
����ֵ��0:�ɹ� �������μ�״̬��
*/
MYDLL_API_MAG unsigned short MAG_CSSWEB_STDCALL MAG_WritePrintEject(char *pIn, char *pOut);


/* MAG_CheckStart
�������ܣ���Ʊ���ܿ���,����Ʊ�ڷ���
��Σ�pIn:����
���Σ�pOut:8λ�ַ�������ʾ��������루3λ���Ҵ���+5λ�����룩
���ش���ʱ����������
����ֵ��0:�ɹ� �������μ�״̬��
*/
MYDLL_API_MAG unsigned short MAG_CSSWEB_STDCALL MAG_CheckStart(char *pIn, char *pOut);



/* MAG_CheckStop
�������ܣ��ر���Ʊ�ڷ���
��Σ�pIn:����
���Σ�pOut:��
���ش���ʱ����������
����ֵ��0:�ɹ� �������μ�״̬��
*/
MYDLL_API_MAG unsigned short MAG_CSSWEB_STDCALL MAG_CheckStop(char *pIn, char *pOut);


/* MAG_LightControl
�������ܣ���ģ����ʾ����ɫ
��Σ�pIn:����Ŀ���ָ���ַ���,ÿ���ֽڴ���һ�ֵƣ�
	��1�ֽڣ�����ڵ�
	��2�ֽڣ��ų��ڵ�
	��3 4�ֽڣ�Ԥ��
	�Ƶ���ɫ���壺
	'0' ��Ĭ��״̬�����ÿ��ƣ�
	'1' �̵�
	'3' ���
	'4' �Ƶ�
	'5' ���̵�
	'6' �����
	'7' ���Ƶ�
	'9' ���
���Σ�pOut::8λ�ַ�������ʾ��������루3λ���Ҵ���+5λ�����룩
���ش���ʱ����������
����ֵ��0:�ɹ� 1:ʧ��
*/
MYDLL_API_MAG unsigned short MAG_CSSWEB_STDCALL MAG_LightControl(char *pIn, char *pOut);



/* MAG_GetStatus
�������ܣ���ȡ��ģ���״̬
��Σ�pIn:����
���Σ�pOut:8λ�ַ�������ʾ��������루3λ���Ҵ���+5λ�����룩
���ش���ʱ����������
����ֵ��0:�ɹ� �������μ�״̬��
*/
MYDLL_API_MAG unsigned short MAG_CSSWEB_STDCALL MAG_GetStatus(char *pIn, char *pOut);

void untoCstring(unsigned char * pDataText, char *szDest, int length);

#ifdef __cplusplus
}
#endif
#endif
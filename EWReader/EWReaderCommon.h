
#ifndef READERCSSWEB_H_
#define READERCSSWEB_H_

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
#define MYDLL_API_READER __declspec(dllexport)
#else
#define MYDLL_API_READER __declspec(dllimport)
#endif


#define EWREADER_STDCALL WINAPI
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

MYDLL_API_READER unsigned short  EWREADER_STDCALL READER_Init(char *pIn, char *pOut);

MYDLL_API_READER unsigned short  EWREADER_STDCALL READER_Uninit(char *pIn, char *pOut);

MYDLL_API_READER unsigned short  EWREADER_STDCALL READER_Reset(char *pIn, char *pOut);

MYDLL_API_READER unsigned short   EWREADER_STDCALL READER_SetBeep(char *pIn, char *pOut);

MYDLL_API_READER unsigned short   EWREADER_STDCALL READER_GetVersion(char *pIn, char *pOut);

MYDLL_API_READER unsigned short   EWREADER_STDCALL READER_ReadId(char *pIn, char *pOut);

MYDLL_API_READER unsigned short   EWREADER_STDCALL READER_ReadAllInfo(char *pIn, char *pOut);

#ifdef __cplusplus
}
#endif
#endif

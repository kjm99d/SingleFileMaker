#pragma once
#include <Windows.h>
#include "SFMError.h"

#define STR_SFM_FMT_SIGNATURE	"SFM_DATA"

typedef enum _ExtractType {
	NONE = 0,
	EXIST_NOT_COPY,
	RANDOM_FILENAME
} ExtractType;

typedef struct _SFM_FILE_INFO_OUTPUT
{
	CHAR szFilePath[MAX_PATH];
	DWORD dwFileSize;
	ExtractType extractType;
} SFM_FILE_INFO_OUTPUT, * PSFM_FILE_INFO_OUTPUT;

typedef struct _SFM_FILE_INFO
{
	CHAR szFilePath[MAX_PATH];
	SFM_FILE_INFO_OUTPUT output;
} SFM_FILE_INFO, * PSFM_FILE_INFO;


void SFM_AddInfo(PSFM_FILE_INFO pSfmFileInfo);

BOOL SFM_MakeFile(LPCSTR lpFilePath);
SFM_ERROR SFM_ExtractFile(LPCSTR lpFilePath);
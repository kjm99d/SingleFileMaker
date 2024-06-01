#pragma once
#include <Windows.h>

#define STR_SFM_FMT_SIGNATURE	"SFM_DATA"

typedef struct _SFM_FILE_INFO
{
	CHAR szFilePath[MAX_PATH];
	DWORD dwFileSize;
} SFM_FILE_INFO, *PSFM_FILE_INFO;


void SFM_AddInfo(PSFM_FILE_INFO pSfmFileInfo);

BOOL SFM_MakeFile(LPCSTR lpFilePath);
BOOL SFM_ExtractFile(LPCSTR lpFilePath);
// libSFM.cpp : 정적 라이브러리를 위한 함수를 정의합니다.
//
#include "framework.h"

#include "libSFM.h"
#include "FileCtrl.h"

#include <strsafe.h>


std::vector<PSFM_FILE_INFO> g_vecSfm;

// (strstr mod) memmem 함수 구현
PVOID memmem(const void* haystack, size_t haystack_len, const void* needle, size_t needle_len) 
{
    if (needle_len == 0)
        return (void*)haystack;
    

    const char* haystack_ptr = (const char*)haystack;
    const char* needle_ptr = (const char*)needle;

    for (size_t i = 0; i <= haystack_len - needle_len; i++) 
    {
        if (memcmp(haystack_ptr + i, needle_ptr, needle_len) == 0) 
        {
            return (void*)(haystack_ptr + i);
        }
    }

    return NULL;
}

void SFM_AddInfo(PSFM_FILE_INFO pSfmFileInfo)
{
	SecureZeroMemory(pSfmFileInfo, sizeof(SFM_FILE_INFO));
	
	g_vecSfm.push_back(pSfmFileInfo);
}

BOOL SFM_MakeFile(LPCSTR lpFilePath)
{
    size_t nCount = g_vecSfm.size();
    if (nCount <= 0)
        return FALSE;
    
    /*
    *   |---------------------------------------------------------|
    *   |       8Byte           |     4Byte      |     4Byte      |
    *   |----------------------------------------|----------------|
    *   |       Signature       |    Version     |     Count      |
    *   |---------------------------------------------------------|
    */


    // 시그니처 작성하기
    const int nSignature = 8;
    CHAR szSignature[nSignature] = { 0x00, };
    StringCchPrintfA(szSignature, _countof(szSignature), "%s", STR_SFM_FMT_SIGNATURE);
    AppendToFile(lpFilePath, szSignature, _countof(szSignature));
    
    // 버전정보 작성하기
    int nVersion = 1;
    AppendToFile(lpFilePath, &nVersion, sizeof(nVersion));


    // 복사할 파일의 수 작성하기
    int nElement = static_cast<int>(nCount);
    AppendToFile(lpFilePath, &nElement, sizeof(nElement));

    for (int i = 0; i < nCount; i++)
    {
        PVOID pBuffer = NULL;
        DWORD dwSize = 0;

        if (0 == ReadFileToBuffer(g_vecSfm[i]->szFilePath, &pBuffer, &dwSize))
        {
            g_vecSfm[i]->dwFileSize = dwSize;

            AppendToFile(lpFilePath, g_vecSfm[i], sizeof(SFM_FILE_INFO));
            AppendToFile(lpFilePath, pBuffer, dwSize);
        }

        free(pBuffer);
        pBuffer = NULL;
    }

    return TRUE;
}

BOOL SFM_ExtractFile(LPCSTR lpFilePath)
{
    PVOID pBuffer = NULL;
    DWORD dwBufferSize = 0;
    if (0 != ReadFileToBuffer(lpFilePath, &pBuffer, &dwBufferSize))
        return FALSE;
    

    const int nSignature = 8;
    CHAR szSignature[nSignature] = { 0x00, };
    StringCchPrintfA(szSignature, _countof(szSignature), "%s", STR_SFM_FMT_SIGNATURE);
    AppendToFile(lpFilePath, szSignature, _countof(szSignature));

    PVOID pPos = memmem(pBuffer, dwBufferSize, szSignature, nSignature);
    if (pPos)
    {
        PBYTE dwPos = (PBYTE)pPos;
        dwPos += 8;

        int nVersion = *(int*)dwPos;
        dwPos += 4;

        int nCount = *(int*)dwPos;
        dwPos += 4;

        for (int i = 0; i < nCount; i++)
        {
            PSFM_FILE_INFO pInfo = (PSFM_FILE_INFO)dwPos;
            dwPos += sizeof(SFM_FILE_INFO);

            
            WriteDataToFile(pInfo->szFilePath, (PBYTE)dwPos, (size_t)pInfo->dwFileSize);

            dwPos += pInfo->dwFileSize;
        }
    }

    return TRUE;
}



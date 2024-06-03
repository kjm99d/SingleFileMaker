#include "StrCtrl.h"
#include <strsafe.h>

BOOL GetRandomFileName(CHAR* pszBuffer, DWORD cch)
{
    if (pszBuffer == nullptr || cch == 0) 
        return FALSE;
    

    // 임시 파일 경로를 가져옵니다.
    CHAR tempPath[MAX_PATH];
    DWORD pathLen = GetTempPathA(MAX_PATH, tempPath);
    if (pathLen == 0 || pathLen > MAX_PATH) 
        return FALSE;

    // 임시 파일 이름을 생성합니다.
    CHAR tempFileName[MAX_PATH];
    if (GetTempFileNameA(tempPath, "TMP", 0, tempFileName) == 0) 
        return FALSE;

    // 결과를 사용자 제공 버퍼에 복사합니다.
    HRESULT hr = StringCchCopyA(pszBuffer, cch, tempPath);
    if (FAILED(hr)) 
        return FALSE;

    return TRUE;
}

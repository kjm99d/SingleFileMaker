#include "StrCtrl.h"
#include <strsafe.h>

BOOL GetRandomFileName(CHAR* pszBuffer, DWORD cch)
{
    if (pszBuffer == nullptr || cch == 0) 
        return FALSE;
    

    // �ӽ� ���� ��θ� �����ɴϴ�.
    CHAR tempPath[MAX_PATH];
    DWORD pathLen = GetTempPathA(MAX_PATH, tempPath);
    if (pathLen == 0 || pathLen > MAX_PATH) 
        return FALSE;

    // �ӽ� ���� �̸��� �����մϴ�.
    CHAR tempFileName[MAX_PATH];
    if (GetTempFileNameA(tempPath, "TMP", 0, tempFileName) == 0) 
        return FALSE;

    // ����� ����� ���� ���ۿ� �����մϴ�.
    HRESULT hr = StringCchCopyA(pszBuffer, cch, tempPath);
    if (FAILED(hr)) 
        return FALSE;

    return TRUE;
}

#include "FileCtrl.h"
#include <Windows.h>

int AppendToFile(LPCSTR lpFilePath, PVOID pData, DWORD nData)
{
    HANDLE hFile = NULL;
    hFile = CreateFileA(lpFilePath, FILE_APPEND_DATA, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    // 파일 열기 실패 확인
    if (hFile == INVALID_HANDLE_VALUE)
        return GetLastError();

    SetFilePointer(hFile, 0, NULL, FILE_END);

    DWORD bytesWritten = 0;
    BOOL result = WriteFile(hFile, pData, nData, &bytesWritten, NULL);

    CloseHandle(hFile);

    return GetLastError();
}

int ReadFileToBuffer(LPCSTR lpFilePath, PVOID* pBuffer, PDWORD pFileSize)
{
    HANDLE hFile = NULL;

    hFile = CreateFileA(lpFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        *pBuffer = NULL;
        *pFileSize = 0;
        return GetLastError();
    }

    *pFileSize = GetFileSize(hFile, NULL);
    if (*pFileSize == INVALID_FILE_SIZE)
    {
        CloseHandle(hFile);
        *pBuffer = NULL;
        return GetLastError();
    }

    *pBuffer = malloc(*pFileSize);
    if (*pBuffer == NULL)
    {
        CloseHandle(hFile);
        return GetLastError();
    }

    DWORD dwReadByte = 0;
    BOOL result = ReadFile(hFile, *pBuffer, *pFileSize, &dwReadByte, NULL);

    if (!result || dwReadByte != *pFileSize)
    {
        free(*pBuffer);
        *pBuffer = NULL;
    }

    CloseHandle(hFile);

    return GetLastError();
}




DWORD GetPEFileSize(LPCSTR lpFilePath, PDWORD pFileSize)
{
    HANDLE hFile = CreateFileA(lpFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        return GetLastError();
    }

    // 파일 매핑 생성
    HANDLE hFileMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
    if (hFileMapping == NULL) {
        DWORD error = GetLastError();
        CloseHandle(hFile);
        return error;
    }

    // 파일 매핑 뷰 생성
    LPVOID lpFileBase = MapViewOfFile(hFileMapping, FILE_MAP_READ, 0, 0, 0);
    if (lpFileBase == NULL) {
        DWORD error = GetLastError();
        CloseHandle(hFileMapping);
        CloseHandle(hFile);
        return error;
    }

    // DOS 헤더 체크
    PIMAGE_DOS_HEADER pDOSHeader = (PIMAGE_DOS_HEADER)lpFileBase;
    if (pDOSHeader->e_magic != IMAGE_DOS_SIGNATURE) {
        UnmapViewOfFile(lpFileBase);
        CloseHandle(hFileMapping);
        CloseHandle(hFile);
        return ERROR_INVALID_DATA; // 유효하지 않은 데이터 에러 코드 사용
    }

    // NT 헤더 체크
    PIMAGE_NT_HEADERS pNTHeaders = (PIMAGE_NT_HEADERS)((DWORD_PTR)lpFileBase + pDOSHeader->e_lfanew);
    if (pNTHeaders->Signature != IMAGE_NT_SIGNATURE) {
        UnmapViewOfFile(lpFileBase);
        CloseHandle(hFileMapping);
        CloseHandle(hFile);
        return ERROR_INVALID_DATA; // 유효하지 않은 데이터 에러 코드 사용
    }

    // 파일 크기 정보 획득
    *pFileSize = pNTHeaders->OptionalHeader.SizeOfImage;

    // 리소스 해제
    UnmapViewOfFile(lpFileBase);
    CloseHandle(hFileMapping);
    CloseHandle(hFile);

    return ERROR_SUCCESS; // 성공 코드 반환
}


int WriteDataToFile(LPCSTR lpFilePath, PVOID data, size_t dataSize)
{
    // 파일을 생성하거나 열기
    HANDLE hFile = CreateFileA(lpFilePath,          // 파일명
        GENERIC_WRITE,       // 쓰기 접근 권한
        0,                   // 공유 모드 없음
        NULL,                // 기본 보안 속성
        CREATE_NEW,          // 파일이 존재하지 않으면 새 파일 생성
        FILE_ATTRIBUTE_NORMAL, // 기본 파일 속성
        NULL);               // 템플릿 파일 없음

    // 파일 핸들이 유효한지 확인
    if (hFile == INVALID_HANDLE_VALUE) {
        return GetLastError();
    }

    DWORD written;
    BOOL success = WriteFile(hFile, data, (DWORD)dataSize, &written, NULL);

    // 파일 닫기
    CloseHandle(hFile);

    // 쓰기 성공 여부 확인
    if (!success || written != dataSize) {
        return GetLastError();
    }

    return ERROR_SUCCESS;
}
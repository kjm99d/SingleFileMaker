#include "FileCtrl.h"
#include <Windows.h>

int AppendToFile(LPCSTR lpFilePath, PVOID pData, DWORD nData)
{
    HANDLE hFile = NULL;
    hFile = CreateFileA(lpFilePath, FILE_APPEND_DATA, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    // ���� ���� ���� Ȯ��
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

    // ���� ���� ����
    HANDLE hFileMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
    if (hFileMapping == NULL) {
        DWORD error = GetLastError();
        CloseHandle(hFile);
        return error;
    }

    // ���� ���� �� ����
    LPVOID lpFileBase = MapViewOfFile(hFileMapping, FILE_MAP_READ, 0, 0, 0);
    if (lpFileBase == NULL) {
        DWORD error = GetLastError();
        CloseHandle(hFileMapping);
        CloseHandle(hFile);
        return error;
    }

    // DOS ��� üũ
    PIMAGE_DOS_HEADER pDOSHeader = (PIMAGE_DOS_HEADER)lpFileBase;
    if (pDOSHeader->e_magic != IMAGE_DOS_SIGNATURE) {
        UnmapViewOfFile(lpFileBase);
        CloseHandle(hFileMapping);
        CloseHandle(hFile);
        return ERROR_INVALID_DATA; // ��ȿ���� ���� ������ ���� �ڵ� ���
    }

    // NT ��� üũ
    PIMAGE_NT_HEADERS pNTHeaders = (PIMAGE_NT_HEADERS)((DWORD_PTR)lpFileBase + pDOSHeader->e_lfanew);
    if (pNTHeaders->Signature != IMAGE_NT_SIGNATURE) {
        UnmapViewOfFile(lpFileBase);
        CloseHandle(hFileMapping);
        CloseHandle(hFile);
        return ERROR_INVALID_DATA; // ��ȿ���� ���� ������ ���� �ڵ� ���
    }

    // ���� ũ�� ���� ȹ��
    *pFileSize = pNTHeaders->OptionalHeader.SizeOfImage;

    // ���ҽ� ����
    UnmapViewOfFile(lpFileBase);
    CloseHandle(hFileMapping);
    CloseHandle(hFile);

    return ERROR_SUCCESS; // ���� �ڵ� ��ȯ
}


int WriteDataToFile(LPCSTR lpFilePath, PVOID data, size_t dataSize)
{
    // ������ �����ϰų� ����
    HANDLE hFile = CreateFileA(lpFilePath,          // ���ϸ�
        GENERIC_WRITE,       // ���� ���� ����
        0,                   // ���� ��� ����
        NULL,                // �⺻ ���� �Ӽ�
        CREATE_NEW,          // ������ �������� ������ �� ���� ����
        FILE_ATTRIBUTE_NORMAL, // �⺻ ���� �Ӽ�
        NULL);               // ���ø� ���� ����

    // ���� �ڵ��� ��ȿ���� Ȯ��
    if (hFile == INVALID_HANDLE_VALUE) {
        return GetLastError();
    }

    DWORD written;
    BOOL success = WriteFile(hFile, data, (DWORD)dataSize, &written, NULL);

    // ���� �ݱ�
    CloseHandle(hFile);

    // ���� ���� ���� Ȯ��
    if (!success || written != dataSize) {
        return GetLastError();
    }

    return ERROR_SUCCESS;
}
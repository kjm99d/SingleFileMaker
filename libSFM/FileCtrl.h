#pragma once
#include <Windows.h>

int AppendToFile(LPCSTR lpFilePath, PVOID pData, DWORD nData);
int ReadFileToBuffer(LPCSTR lpFilePath, PVOID* pBuffer, PDWORD pFileSize);

// �� �ڵ�� PE ���� ������� ���ϱ� ������ ������ ������ �˾Ҵµ� .. 
// C# ���� ����� ������ �ȵǴ� �� ����.
DWORD GetPEFileSize(LPCSTR lpFilePath, PDWORD pFileSize);

int WriteDataToFile(LPCSTR lpFilePath, PVOID data, size_t dataSize);
#pragma once
#include <Windows.h>

int AppendToFile(LPCSTR lpFilePath, PVOID pData, DWORD nData);
int ReadFileToBuffer(LPCSTR lpFilePath, PVOID* pBuffer, PDWORD pFileSize);

// 이 코드는 PE 파일 헤더에서 구하기 때문에 문제가 없을지 알았는데 .. 
// C# 으로 빌드된 파일은 안되는 것 같다.
DWORD GetPEFileSize(LPCSTR lpFilePath, PDWORD pFileSize);

int WriteDataToFile(LPCSTR lpFilePath, PVOID data, size_t dataSize);
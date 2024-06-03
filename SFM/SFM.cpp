#include <iostream>
#include "libSFM.h"
#include <strsafe.h>
#pragma comment(lib, "libSFM")

int main()
{
    std::cout << "Hello World!\n";

    SFM_FILE_INFO item;
    SFM_AddInfo(&item);

    
    StringCchCopyA(item.szFilePath, _countof(item.szFilePath), "D:\\TEST\\aespa.txt");
    StringCchCopyA(item.output.szFilePath, _countof(item.output.szFilePath), "D:\\TEST\\aespa2.txt");

    SFM_MakeFile("D:\\TEST\\test.exe");
    SFM_ExtractFile("D:\\TEST\\test.exe");
}
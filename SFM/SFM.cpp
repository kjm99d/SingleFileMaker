#include <iostream>
#include "libSFM.h"
#include <strsafe.h>
#pragma comment(lib, "libSFM")

int main()
{
    std::cout << "Hello World!\n";

    SFM_FILE_INFO item;
    // 압축 할 클라이언트 디스크에 존재하는 파일의 경로를 입력 파일 정보로 저장
    StringCchCopyA(item.szFilePath, _countof(item.szFilePath), "D:\\TEST\\aespa.txt");
    // 압축 해제 할 클라이언트에 저장할 파일의 경로
    StringCchCopyA(item.output.szFilePath, _countof(item.output.szFilePath), "D:\\TEST\\aespa2.txt");
    // 압축 해제 할 클라이언트에 파일을 저장 시 저장할 옵션 정보
    item.output.extractType = ExtractType::RANDOM_FILENAME;


    // 파일의 정보를 저장한 데이터를 SFM 관리자에게 전달한다.
    SFM_AddInfo(&item);

    // 캡슐화된 파일을 만들어낸다.
    SFM_MakeFile("D:\\TEST\\test.exe");
    // 캡슐화된 파일에 대해 역캡슐화를 진행한다.
    SFM_ExtractFile("D:\\TEST\\test.exe");
}
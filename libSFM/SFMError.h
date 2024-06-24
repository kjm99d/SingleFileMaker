
enum class SFM_ERROR
{
	SFM_SUCCESS = 0,	// << 성공 

	SFM_HAS_SIGNATURE = 100,	// << 이미 시그니처를 가지고 있음

	SFM_FILE_READ_FAILED = 1000,	// 파일 읽기에 실패

	SFM_FILE_WRITE_FAILED = 1100,	// 파일 쓰기에 실패
};
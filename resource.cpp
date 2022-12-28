#include "functions.h";

void resourceManagement::createResource() {
	// ULID , 자산 이미지 , 자산명 , 분류 , 최초 등록 일시 , 상태 , 위치 , 사용직원 , 최종 수정 일시 , 구매처 , 최초 구매일 , 가격
	srand((unsigned int)time(NULL));
	int ulid = rand() * 1000000000;

	cout << ulid;
}
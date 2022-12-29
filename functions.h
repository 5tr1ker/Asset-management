#ifndef functions_H
#define functions_H
#include <map>
#include<iostream>
#include<string>
#include<Windows.h>
#pragma comment(lib , "libmySQL.lib")
#include <mysql.h>
#include<vector>
#include <list>

using namespace std;

class resourceManagement {
public:
	// 소프트웨어 정보
	void updateSystemInfo();
	void updateSoftwareInfo();
	void getSoftwareInfo(const char* ulid);
	void getSystemInfo(const char* ulid);
	// 자산 분류
	void createResource();
	void getTotalResourceCount();
	void getDetailResource(const char* ulid);
	void getAllResource(int limit, int page);
	void searchResource(const char* item , const char* search , int limit, int page);
	void modifiyResource(const char* ulid , const char* item , const char* data);
	// 히스토리
	void getHistory();
	void createHistory();
};

#endif
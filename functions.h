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
	void updateSystemInfo();
	void updateSoftwareInfo();
	void getSoftwareInfo(const char* ulid);
	void getSystemInfo(const char* ulid);
	void createResource();
};

#endif
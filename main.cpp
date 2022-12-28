#include "functions.h"
using namespace std;

int main() {
    resourceManagement rm;
    /* 소프트웨어 정보 가져오기
    const char* chr = "1";
    rm.getSoftwareInfo(chr);
    */
    
    //rm.updateSystemInfo();
    /*
    const char* chr = "1";
    rm.getSystemInfo(chr);
    */
    
    rm.createResource();
    return 0;
}
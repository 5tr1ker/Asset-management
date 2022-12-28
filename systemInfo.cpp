#include "functions.h"
#define BUFF_SIZE 1024

bool checkAlpha(string str) {
    for (int i = 0; i < str.length(); i++) {
        if (isdigit(str[i]) == 0) {
            return false;
        }
    }

    return true;
}

std::string GetProcessorName()
{
    wchar_t Cpu_info[100];
    HKEY hKey;
    int i = 0;
    long result = 0;
    DWORD c_size = sizeof(Cpu_info);

    //레지스트리를 조사하여 프로세서의 모델명을 얻어냅니다.
    RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"Hardware\\Description\\System\\CentralProcessor\\0", 0, KEY_QUERY_VALUE, &hKey);
    RegQueryValueEx(hKey, L"ProcessorNameString", NULL, NULL, (LPBYTE)Cpu_info, &c_size);
    RegCloseKey(hKey);

    //GetSystemInfo 함수를 이용해 논리적 코어 개수를 얻어냅니다.
    /*
    wchar_t num[8];
    SYSTEM_INFO systemInfo;
    GetSystemInfo(&systemInfo);
    swprintf(num, 8, L" * %d", systemInfo.dwNumberOfProcessors);
    wcscat_s(Cpu_info, 100, num);
    */

    wstring ws(Cpu_info);
    string str(ws.begin(), ws.end());
    return str;
}

std::string getBaseBoard() {
    wchar_t BaseBoard_info[100];
    wchar_t BaseBoard_product[100];
    wchar_t BaseBoard_Version[100];
    HKEY hKey;
    int i = 0;
    long result = 0;
    DWORD c_size = sizeof(BaseBoard_info);

    //레지스트리를 조사하여 프로세서의 모델명을 얻어냅니다.
    RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"Hardware\\Description\\System\\BIOS", 0, KEY_QUERY_VALUE, &hKey);
    if (RegQueryValueEx(hKey, L"BaseBoardManufacturer", NULL, NULL, (LPBYTE)BaseBoard_info, &c_size) != ERROR_SUCCESS)
    {
        return "Fail to Load the BaseBoardManufacturer";
    }
    if (RegQueryValueEx(hKey, L"BaseBoardProduct", NULL, NULL, (LPBYTE)BaseBoard_product, &c_size) != ERROR_SUCCESS)
    {
        return "Fail to Load the BaseBoardProduct";
    }
    if (RegQueryValueEx(hKey, L"BaseBoardVersion", NULL, NULL, (LPBYTE)BaseBoard_Version, &c_size) != ERROR_SUCCESS)
    {
        return "Fail to Load the BaseBoardVersion";
    }

    RegCloseKey(hKey);

    wstring ws(BaseBoard_info);
    string str(ws.begin(), ws.end());
    wstring ws2(BaseBoard_product);
    string str2(ws2.begin(), ws2.end());
    wstring ws3(BaseBoard_Version);
    string str3(ws3.begin(), ws3.end());
    return str + " " + str2 + " " + str3;
}

std::string GetOSName()
{
    wchar_t ProductName[100];
    wchar_t CSDVersion[100];
    wchar_t Version[100];
    std::wstring Os_info;

    HKEY hKey;
    int i = 0;

    DWORD c_size = 100;

    //레지스트리를 조사하여 운영체제 이름을 조사합니다.
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", 0, KEY_READ, &hKey) != ERROR_SUCCESS)
    {
        return "Fail to Open OS_info";
    }
    if (RegQueryValueEx(hKey, L"ProductName", NULL, NULL, (LPBYTE)ProductName, &c_size) != ERROR_SUCCESS)
    {
        return "Fail to Load the ProductName";
    }
    if (RegQueryValueEx(hKey, L"CurrentBuild", NULL, NULL, (LPBYTE)Version, &c_size) != ERROR_SUCCESS)
    {
        return "Fail to Load the BuildLab";
    }
    if (RegQueryValueEx(hKey, L"CSDVersion", NULL, NULL, (LPBYTE)CSDVersion, &c_size) != ERROR_SUCCESS)
    {
        RegCloseKey(hKey);
        // return ProductName;
    }
    wstring ws(ProductName);
    string str(ws.begin(), ws.end());

    wstring ws2(Version);
    string str2(ws2.begin(), ws2.end());

    RegCloseKey(hKey);
    
    return str + " " + str2;
}

std::string ExecCommand(std::string szCmdArg)
{
    std::string szResult;

    HANDLE hChildStdoutRd;
    HANDLE hChildStdoutWr;

    BOOL fSuccess;

    // Create security attributes to create pipe.
    SECURITY_ATTRIBUTES saAttr = { sizeof(SECURITY_ATTRIBUTES) };
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    // Create a pipe to get results from child's stdout.
    if (!CreatePipe(&hChildStdoutRd, &hChildStdoutWr, &saAttr, 0))
    {
        return szResult;
    }

    STARTUPINFOA si = { sizeof(STARTUPINFO) };

    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    si.hStdOutput = hChildStdoutWr;
    si.hStdError = hChildStdoutWr;
    si.wShowWindow = SW_HIDE;

    PROCESS_INFORMATION pi = { 0 };

    // Create the child process.
    fSuccess = CreateProcessA(
        NULL,
        (LPSTR)szCmdArg.c_str(),    // command line
        NULL,                       // process security attributes
        NULL,                       // primary thread security attributes
        TRUE,                       // TRUE=handles are inherited. Required.
        CREATE_NEW_CONSOLE,         // creation flags
        NULL,                       // use parent's environment
        NULL,                       // use parent's current directory
        &si,                        // __in, STARTUPINFO pointer
        &pi);                       // __out, receives PROCESS_INFORMATION

    if (!fSuccess)
    {
        return szResult;
    }

    // Wait until child processes exit. Don't wait forever.
    WaitForSingleObject(pi.hProcess, 2000);
    TerminateProcess(pi.hProcess, 0);

    if (!CloseHandle(hChildStdoutWr))
    {
        return szResult;
    }

    for (;;)
    {
        DWORD dwRead;
        CHAR chBuf[4096];
        
        bool done = !ReadFile(hChildStdoutRd, chBuf, 4096, &dwRead, NULL) || dwRead == 0;
        if (done)
        {
            break;
        }

        szResult += chBuf;
    }

    CloseHandle(hChildStdoutRd);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return szResult;
}

string RunCommand(char* command) {
    char  buff[BUFF_SIZE];
    FILE* fp;
    string result;

    fp = _popen(command, "r");
    if (NULL == fp)
    {
        perror("popen() 실패");
        return NULL;
    }

    while (fgets(buff, BUFF_SIZE, fp)) {
        result += buff;
    }
        //printf("%s", buff);

    _pclose(fp);
    return result;
}

list<string> getHDD() {
    char cmd[] = "wmic diskdrive get Model , size";
    string result = RunCommand(cmd);
    
    vector<string> vector;
    string str;
    for (int i = 0; result.length() > i; i++) {
        if(result[i] != ' ') str += result[i];
        if (result[i] == ' ') {
            if (result.length() > i && result[i + 1] == ' ') {
                if (str != " " && str.size() != 0) vector.push_back(str);
                str = "";
                while (result[i + 1] != ' ' && result.length() > i) {
                    i++;
                }
            }
        }
    }

    list<string> returnData;
    for (int i = 2; i < vector.size(); i += 2) {
        string np = vector.at(i + 1);
        vector[i + 1] = np.substr(0 , np.size() - 9) + "GB";
        returnData.push_back(vector.at(i) + " " + vector.at(i + 1));
    }
    return returnData;
}

vector<string> getGPU() {
    char cmd[] = "wmic path win32_VideoController get name";
    string gpu = RunCommand(cmd);
    string result = gpu.substr(6); // Model 문구 제거
    int npos = result.find_first_not_of(' ');
    result.erase(0, npos + 2);
    
    vector<string> vector;
    string str;
    for(int i = 0; result.length() > i ; i++) {
        str += result[i];
        if (result[i] == ' ') {
            if (result.length() > i && result[i + 1] == ' ') {
                if(str != " ") vector.push_back(str);
                str = "";
                while (result[i + 1] != ' ' && result.length() > i) {
                    i++;
                }
            }
        }
    }

    return vector;
}

string getOSUUID() {
    char cmd[] = "wmic csproduct get uuid";
    string gpu = RunCommand(cmd);
    string result = gpu.substr(5); // UUID 문구 제거
    int npos = result.find_first_not_of(' ');

    return result.erase(0, npos + 2);;
}

vector<string> getInstalledSoftware() {
    char cmd[] = "wmic product get name, version, vendor";
    string result = RunCommand(cmd);

    vector<string> vector;
    string str;
    for (int i = 0; result.length() > i; i++) {
        str += result[i];
        if (result[i] == ' ') {
            if (result.length() > i && result[i + 1] == ' ' && result[i + 2] != 'x') {
                if (str != " ") vector.push_back(str);
                str = "";
                while (result[i + 1] != ' ' && result.length() > i) {
                    i++;
                }
            }
        }
    }

    return vector;
}

/*
* 20 DDR
* 21 DDR2
* 22 DDR2 FB-DIMM
* 24 DDR3
* 26 DDR4
* 1073741824Byte = 1GB
*/
list<string> getMemory() {
    char cmd[] = "wmic memorychip get manufacturer , Capacity , SMBIOSMemoryType , partNumber"; // 메모리 용량과 DDR 타입 가져오기  , partNumber
    //string result = "Capacity   manufacturer   PartNumber  SMBIOSMemoryType\n  8589934592  samsung  ABCABC  26\n  8589934592  samsung   ABCABC  26";
    string result = RunCommand(cmd);

    string str;
    vector<string> vector;
    list<string> returnIndex;

    // cout << result << endl;
    for (int i = 0; result.length() > i; i++) {
        if (result[i] != ' ') str += result[i];
        if (result[i] == ' ') {
            if (result.length() > i && result[i + 1] == ' ') {
                if (str != " " && str.length() > 0) vector.push_back(str);
                str = "";
                while (result[i + 1] != ' ' && result.length() > i) {
                    i++;
                }
            }
        }
    }

    // 마지막 남은 문자 추가
    str.erase(str.find_last_not_of(" \n\r\t") + 1);
    if (str.size() != 0) {
        vector.push_back(str);
    }
    
    int idx = 0;
    for (int i = 4; vector.size() > i; i += 2) {
        string memorySize = vector.at(i);
        string manufacture = vector.at(i + 1);
        i++;
        string partNumber;
        if (!checkAlpha(vector[i + 1])) { // part 번호가 존재할 때
            partNumber = vector.at(i + 1);
            i++;
        }
        string code = vector.at(i + 1);
        code.erase(code.find_last_not_of(" \n\r\t") + 1);
        
        string afterCode;
        memorySize = memorySize.substr(0 , memorySize.length() - 9) + "GB"; // 메모리 전체크기
        for (int i = 0; i < memorySize.size(); i++) { // 개행문자 제거
            if (memorySize[i] > 14) break;
            else memorySize[i] = ' ';
        }

        if (code == "20") afterCode = "DDR";
        else if (code == "21") afterCode = "DDR2";
        else if (code == "22") afterCode = "DDR2 FB-DIMM";
        else if (code == "24") afterCode = "DDR3";
        else if (code == "26") afterCode = "DDR4";

        returnIndex.push_back(manufacture + " " + afterCode + " " + memorySize + " " + partNumber);
    }

    return returnIndex;
}

/* mysql DDL Query
CREATE TABLE `comon`.`system_infos` (
  `ulid` VARCHAR(100) NOT NULL,
  `os` VARCHAR(100) NOT NULL,
  `cpu` VARCHAR(100) NOT NULL,
  `baseboard` VARCHAR(100) NOT NULL,
  `memory1` VARCHAR(100) NOT NULL,
  `memory2` VARCHAR(100) NULL,
  `memory3` VARCHAR(100) NULL,
  `memory4` VARCHAR(100) NULL,
  `disk1` VARCHAR(100) NOT NULL,
  `disk2` VARCHAR(100) NULL,
  `graphic1` VARCHAR(100) NOT NULL,
  `graphic2` VARCHAR(100) NULL,
  `uuid` VARCHAR(100) NOT NULL,
  PRIMARY KEY (`ulid`));
*/
void resourceManagement::updateSystemInfo() {
    MYSQL* conn, connection;
    MYSQL_ROW row;

    char DB_HOST[] = "localhost";
    char DB_USER[] = "root";
    char DB_PASS[] = "password";
    char DB_NAME[] = "comon";

    // DB 커넥션 연결
    mysql_init(&connection);
    conn = mysql_real_connect(&connection, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, (char*)NULL, 0);
    char sql[1024];

    // System 정보 가져와 쿼리 만들기
    string query = "INSERT INTO `comon`.`system_infos` (`ulid`, `os`, `cpu`, `baseboard`, `uuid` ";
    string last_query;
    list<string> hddList = getHDD();
    list<string> memoryList = getMemory();
    vector<string> gpuList = getGPU();
    int i;
    for (i = 0; i < gpuList.size(); i++) {
        query = query + ", `graphic" + to_string(i + 1) + "` ";
        last_query = last_query + ", '" + gpuList[i] + "' ";
    }
    i = 0;
    for (string str : memoryList) {
        query = query + ", `memory" + to_string(i + 1) + "` ";
        last_query = last_query + ", '" + str + "' ";
        i++;
    }
    i = 0;
    for (string str : hddList) {
        query = query + ", `disk" + to_string(i + 1) + "` ";
        last_query = last_query + ", '" + str + "' ";
        i++;
    }
    query += ") VALUES ('1' , '" + GetOSName() + "' , '" + GetProcessorName() + "' , '" + getBaseBoard() + "' , '" + getOSUUID() + "' " + last_query + ");";

    // 작성된 쿼리를 DB에 저장하기
    strcpy_s(sql, query.c_str());
    if (mysql_query(conn, sql) != 0) {
        cout << "systemInfo.cpp 파일에서 오류 발생!";
    }

    mysql_close(conn);
}

/*
CREATE TABLE `comon`.`software_infos` (
  `ulid` VARCHAR(100) NOT NULL,
  `name` VARCHAR(100) NOT NULL,
  `vendor` VARCHAR(100) NOT NULL,
  `version` VARCHAR(100) NOT NULL,
  `software_infos_key` INT NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`software_infos_key`));
*/
void resourceManagement::updateSoftwareInfo() {
    MYSQL* conn, connection;
    MYSQL_ROW row;

    char DB_HOST[] = "localhost";
    char DB_USER[] = "root";
    char DB_PASS[] = "password";
    char DB_NAME[] = "comon";

    // DB 커넥션 연결
    mysql_init(&connection);
    conn = mysql_real_connect(&connection, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, (char*)NULL, 0);
    char sql[1024];
    
    // query 작성
    // 한글이 포함되어 있을 때를 위해 사용
    mysql_query(conn, "set session character_set_connection=euckr;");
    mysql_query(conn, "set session character_set_results=euckr;");
    mysql_query(conn, "set session character_set_client=euckr;");
    string query;
    vector<string> softwareList = getInstalledSoftware();
    for (int i = 3; i < softwareList.size(); i++) {
        string name = softwareList[i];
        if (name.size() < 5) {
            continue;
        }
        string vendor = softwareList[i + 1];
        string version = softwareList[i + 2];
        query = "INSERT INTO `comon`.`software_infos` (`ulid` , `name` , `vendor` , `version`) VALUES ('1' , '" + name + "' , '" + vendor + "' , '" + version + "');";
        i += 2;

        strcpy_s(sql, query.c_str());

        if (mysql_query(conn, sql) != 0) {
            cout << "오류 쿼리 : " << sql << endl;
        }
    }

    mysql_close(conn);
}

void resourceManagement::getSoftwareInfo(const char * ulid) {
    MYSQL* conn, connection;
    MYSQL_RES* result;
    MYSQL_ROW row;

    char DB_HOST[] = "localhost";
    char DB_USER[] = "root";
    char DB_PASS[] = "password";
    char DB_NAME[] = "comon";

    // DB 커넥션 연결
    mysql_init(&connection);
    conn = mysql_real_connect(&connection, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, (char*)NULL, 0);
    char sql[1024];

    // query 작성
    // 한글이 포함되어 있을 때를 위해 사용
    mysql_query(conn, "set session character_set_connection=euckr;");
    mysql_query(conn, "set session character_set_results=euckr;");
    mysql_query(conn, "set session character_set_client=euckr;");

    string ulid_id(ulid);
    string str = "select name , vendor , version from software_infos where ulid = '" + ulid_id + "';";
    strcpy_s(sql, str.c_str());

    if (mysql_query(conn, sql) == 0) {
        result = mysql_store_result(conn);
        while ((row = mysql_fetch_row(result)) != NULL) {
            cout << row[0] << row[1] << row[2] << endl;
        }
        mysql_free_result(result);
    }
    else { // sql 실패
        cerr << "SQL 문 실행에 실패했습니다.";
    }
}

void resourceManagement::getSystemInfo(const char* ulid) {
    MYSQL* conn, connection;
    MYSQL_RES* result;
    MYSQL_ROW row;

    char DB_HOST[] = "localhost";
    char DB_USER[] = "root";
    char DB_PASS[] = "password";
    char DB_NAME[] = "comon";

    // DB 커넥션 연결
    mysql_init(&connection);
    conn = mysql_real_connect(&connection, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, (char*)NULL, 0);
    char sql[1024];

    string ulid_id(ulid);
    string str = "select * from system_infos where ulid = '" + ulid_id + "';";
    strcpy_s(sql, str.c_str());

    if (mysql_query(conn, sql) == 0) {
        result = mysql_store_result(conn);
        while ((row = mysql_fetch_row(result)) != NULL) {
            cout << row[0] << row[1];
            if (row[6] == NULL) {
                cout << "NuLL 값 이라니 ㅜㅜ" << endl;
            }
        }
        mysql_free_result(result);
    }
}

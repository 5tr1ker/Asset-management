#include "functions.h";

void resourceManagement::createResource() {
	// ULID , 자산 이미지 , 자산명 , 분류 , 최초 등록 일시 , 상태 , 위치 , 사용직원 , 최종 수정 일시 , 구매처 , 최초 구매일 , 가격
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


	string ulid;
	while (true) {
		srand((unsigned int)time(NULL));
		ulid = "";
		for (int i = 0; i < 10; i++) { // 일련번호 생성
			std::string alphabet = "570968123405981647230908243765175896420312983645071";
			char ch = alphabet[rand() % alphabet.size()];
			ulid += ch;
		}
		string query = "select ulid from system_infos where ulid = '" + ulid + "';";
		strcpy_s(sql, query.c_str());

		if (mysql_query(conn, sql) == 0) { // 일련변호가 겹치는지 확인
			result = mysql_store_result(conn);
			if ((row = mysql_fetch_row(result)) == NULL) { // 겹치지 않을 경우 종료
				break;
			};
			mysql_free_result(result);
		}
	}
	
	cout << ulid;
}
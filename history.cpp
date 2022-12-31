#include "functions.h"

/*
CREATE TABLE `comon`.`historys` (
  `idhistorys` INT NOT NULL AUTO_INCREMENT,
  `manager` VARCHAR(45) NOT NULL,
  `process_date` DATETIME NOT NULL,
  `division` VARCHAR(45) NOT NULL,
  `classification` VARCHAR(100) ,
  `content` VARCHAR(100) ,
  `etc` VARCHAR(45) ,
  PRIMARY KEY (`idhistorys`));
*/
void resourceManagement::getHistory() {
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
	char sql[1024] = "select * from historys;";

	if (mysql_query(conn, sql) == 0) {
		result = mysql_store_result(conn);
		while ((row = mysql_fetch_row(result)) != NULL) {
			cout << row[0] << " : " << row[1] << " : " << row[2] << endl;
		}
		mysql_free_result(result);
	}
	else { // sql 실패
		cerr << "SQL 문 실행에 실패했습니다.";
	}
}

void resourceManagement::createHistory(const char* division , const char* classification , const char* content , const char* etc) {
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

	string queryInsert = "'manager' , '2000-03-02 00:00:00' , '" + string(division) + "' , '" + string(classification) + "' , '" + string(content) + "' , '" + string(etc) + "');";
	string query = "INSERT INTO historys (manager , process_date , division , classification , content , etc) VALUES (" + queryInsert;
	strcpy_s(sql, query.c_str());

	cout << sql;
	if (mysql_query(conn, sql) != 0) {
		cout << "sql 실행에 실패했습니다.";
	}
}

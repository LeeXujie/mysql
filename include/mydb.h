#ifndef __MYBD_H__
#include<iostream>
#include<string.h>
#include<mysql/mysql.h>

class MyDB
{
public:
    MyDB();
    ~MyDB();
    bool InitDB ( std::string host, std::string user, std::string pwd, std::string db_name );
    bool QueryInfo ( std::string sql );
    bool CreateDB ( char* database );
    bool CreateTable ( char* table, char* fields );
    bool InsertData ( char* table, char* values );
    bool UpdateData ( char* table, char* update_name, char* update_value, char* field_name, char* field_value );
    bool DeleteData ( char* table, char* field_name, char* field_value );
    bool QueryData1 ( char* table, char* columns );
    bool QueryData2 ( char* table, char* field_name, char* field_value );
    bool QueryData3 ( char* table, char* query_name, char* field_name, char* field_value );
    bool TruncateTable ( char* table );
    bool TruncateDB ( char* database );
    bool DropTable ( char* table );
    bool DropDB ( char* database );
private:
    MYSQL *connection;
    MYSQL_RES *result;
    MYSQL_ROW row;

    bool setUTF8();
    bool getResult();
};

#endif

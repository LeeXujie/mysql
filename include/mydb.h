#ifndef _MYBD_H_
#define _MYBD_H_
#include <iostream>
#include <string.h>
#include <mysql/mysql.h>

class MyDB
{
public:
    MyDB();
    ~MyDB();
    bool InitDB ( std::string host, std::string user, std::string pwd, std::string db_name );
    bool QueryInfo ( std::string sql );
    bool CreateDB ( char* database );
    bool CreateTable ( char* table, char* fields );
    bool CreateTmpTable ( char* table, char* fields );
    bool InsertData ( char* table, char* values );
    bool UpdateData ( char* table, char* update_name, char* update_value, char* field_name, char* field_value );
    bool DeleteData ( char* table, char* field_name, char* field_value );
    bool QueryData1 ( char* table, char* columns );
    bool QueryData2 ( char* table, char* field_name, char* field_value );
    bool QueryData3 ( char* table, char* query_name, char* field_name, char* field_value );
    bool GetValue ( char* table, char* query_name, std::string &query_value, int n );
    bool GetMaxPrimarykey ( char* table, char* primary_key, int &max_value );
    bool GetRowsNum ( char* table, char* field_value, int &num );
    bool CopyTable ( char* copy_table, char* prim_table, char* range );
    bool ExpTable ( char* table, char* fields, char* save_name );
    bool GetDate ( std::string &current_date );
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

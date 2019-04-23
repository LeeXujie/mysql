#ifndef _MY_TABLE_H_
#define _MY_TABLE_H_
#include "mydb.h"

class MyTable: public MyDB
{
public:
    MyTable ( char* _table, bool _delOldTab = false );
    ~MyTable();
    bool InsertData();
    bool QueryData ( char* columns );
    bool QueryData ( char* field_name, char* field_value );
    bool QueryData ( char* query_name, char* field_name, char* field_value );
    bool CheckIssues();
    bool SetFalseDetect( bool falseDetect );
    bool GetErrorRate();

public:
    char* placeNum;
    char* boardNum;
    char* stdImg;
    char* defectImg;
    char* status;

private:
    char* table;
    char* tmpTable;
    char id[100];
    int checknum;
    std::string checkid;
};

#endif
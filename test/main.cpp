#include "mydb.h"
using namespace std;

int main()
{
    MyDB db;
    char* table="students";
    char* fields="id INT, name VARCHAR(20), email VARCHAR(20), 学历 VARCHAR(20)";
    if ( !db.InitDB ( "localhost", "root", "123", "test" ) )
    {
        return false;
    }
    db.QueryInfo ( "show databases" );
    db.DropTable ( table );
    db.CreateTable ( table,fields );
    db.InsertData ( table, "1,'张三','zhangsan@163.com','硕士研究生'),(2,'李四','lisi@163.com','硕士研究生'" );
    db.QueryData2 ( table, "name", "李四" );
    db.UpdateData ( table,"email","lisi@qq.com","name","李四" );
    db.QueryData1 ( table, "*" );
    db.DeleteData ( table,"name","李四" );
    db.QueryData1 ( table, "*" );
    db.TruncateTable ( table );
    db.InsertData ( table, "1,'张三','zhangsan@163.com','硕士研究生'),(2,'李四','lisi@163.com','硕士研究生'" );
    db.QueryData3 ( table, "email","name","李四" );
    return 0;
}

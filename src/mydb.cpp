#include <cstdlib>
#include "mydb.h"
using namespace std;

MyDB::MyDB()
{
    connection = mysql_init ( NULL ); // 初始化数据库连接变量
    if ( connection == NULL )
    {
        cout << "Error:" << mysql_error ( connection ) << endl;
        exit ( 1 );
    }
}

MyDB::~MyDB()
{
    if ( connection != NULL ) // 关闭数据库连接
    {
        mysql_close ( connection );
    }
}

//连接数据库
bool MyDB::InitDB ( string host, string user, string pwd, string db_name )
{
    if ( !mysql_real_connect ( connection, host.c_str(), user.c_str(), pwd.c_str(), db_name.c_str(), 0, NULL, 0 ) )
    {
        cout << "Connect error:" << mysql_error ( connection ) << endl;
        return false;
    }
    if ( setUTF8() )
    {
        return true;
    }
    return false;
}

//查询数据库信息
bool MyDB::QueryInfo ( string sql )
{
    if ( mysql_query ( connection, sql.c_str() ) )
    {
        printf ( "Query database info failed (%s).\n", mysql_error ( connection ) );
        return false;
    }
    else
    {
        if ( getResult() )
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

//创建数据库
bool MyDB::CreateDB ( char* database )
{
    char sql[1024];
    sprintf ( sql,"create database if not exists %s",database );
    if ( mysql_query ( connection, sql ) )  //执行SQL语句
    {
        printf ( "Create database failed (%s).\n",mysql_error ( connection ) );
        return false;
    }
    else
    {
//         printf ( "Create database '%s' success.\n",database );
        return true;
    }
}

//创建表
bool MyDB::CreateTable ( char* table, char* fields )
{
    char sql[1024];
    sprintf ( sql,"create table if not exists %s(%s)", table, fields );
    if ( mysql_query ( connection, sql ) )  //执行SQL语句
    {
        printf ( "Create table failed (%s).\n",mysql_error ( connection ) );
        return false;
    }
    else
    {
//         printf ( "Create table '%s' success.\n",table );
        return true;
    }
}

//创建临时表
bool MyDB::CreateTmpTable ( char* table, char* fields )
{
    char sql[1024];
    sprintf ( sql,"create temporary table if not exists %s(%s)", table, fields );
    if ( mysql_query ( connection, sql ) )  //执行SQL语句
    {
        printf ( "Create temporary table failed (%s).\n",mysql_error ( connection ) );
        return false;
    }
    else
    {
//         printf ( "Create temporary table '%s' success.\n",table );
        return true;
    }
}

//插入数据
bool MyDB::InsertData ( char* table, char* values )
{
    char sql[1024];
    sprintf ( sql,"insert into %s values(%s)", table, values );
    if ( mysql_query ( connection, sql ) )  //执行SQL语句
    {
        printf ( "Insert data failed (%s).\n",mysql_error ( connection ) );
        return false;
    }
    else
    {
//         printf ( "Insert data success.\n" );
        return true;
    }
}

//修改数据
bool MyDB::UpdateData ( char* table, char* update_name, char* update_value, char* field_name, char* field_value )
{
    char sql[1024];
    sprintf ( sql,"update %s set %s='%s' where %s='%s'", table, update_name, update_value, field_name, field_value );
    if ( mysql_query ( connection, sql ) )  //执行SQL语句
    {
        printf ( "Update data failed (%s).\n",mysql_error ( connection ) );
        return false;
    }
    else
    {
//         printf ( "Update data success.\n" );
        return true;
    }
}

//删除数据
bool MyDB::DeleteData ( char* table, char* field_name, char* field_value )
{
    char sql[1024];
    sprintf ( sql,"delete from %s where %s = '%s'", table, field_name, field_value );
    if ( mysql_query ( connection, sql ) )  //执行SQL语句
    {
        printf ( "Delete data failed (%s).\n",mysql_error ( connection ) );
        return false;
    }
    else
    {
//         printf ( "Delete data success.\n" );
        return true;
    }
}

//查询数据（按列）
bool MyDB::QueryData1 ( char* table, char* columns )
{
    char sql[1024];
    sprintf ( sql,"select %s from %s", columns, table );
    if ( mysql_query ( connection, sql ) )
    {
        printf ( "Query data failed (%s).\n", mysql_error ( connection ) );
        return false;
    }
    else
    {
        if ( getResult() )
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

//查询数据（按行）
bool MyDB::QueryData2 ( char* table, char* field_name, char* field_value )
{
    char sql[1024];
    sprintf ( sql,"select * from %s where %s='%s'", table, field_name, field_value );
    if ( mysql_query ( connection, sql ) )
    {
        printf ( "Query data failed (%s).\n", mysql_error ( connection ) );
        return false;
    }
    else
    {
        if ( getResult() )
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

//查询数据（某行中的某个值）
bool MyDB::QueryData3 ( char* table, char* query_name, char* field_name, char* field_value )
{
    char sql[1024];
    sprintf ( sql,"select %s from %s where %s='%s'", query_name, table, field_name, field_value );
    if ( mysql_query ( connection, sql ) )
    {
        printf ( "Query data failed (%s).\n", mysql_error ( connection ) );
        return false;
    }
    else
    {
        if ( getResult() )
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

//查询第n行的某个元素，并获得它的值
bool MyDB::GetValue ( char* table, char* query_name, std::string &query_value, int n )
{
    char sql[1024];
    sprintf ( sql,"select %s from %s limit %d,1", query_name, table, n );
    if ( mysql_query ( connection, sql ) )
    {
        printf ( "Query data failed (%s).\n", mysql_error ( connection ) );
        return false;
    }
    else
    {
        result = mysql_store_result ( connection );
        if ( !result )
        {
            printf ( "Couldn't get result from %s.\n", mysql_error ( connection ) );
            return false;
        }
        // 获取列数
        int j = mysql_num_fields ( result );

        //存储字段信息
        char *str_field[1024];

        //获取字段名
        for ( int i = 0; i < mysql_num_fields ( result ); i++ )
        {
            str_field[i] = mysql_fetch_field ( result )->name;
            if ( 0==strcmp ( query_name, str_field[i] ) )
            {
                j=i;
                break;
            }
        }

        if ( row = mysql_fetch_row ( result ) )
        {
            query_value=row[j];
            return true;
        }
        return false;
    }
}

//获得表中主键的最大值
bool MyDB::GetMaxPrimarykey ( char* table, char* primary_key, int &max_value )
{
    char sql[1024];
    sprintf ( sql,"select max(%s) from %s", primary_key, table );
    if ( mysql_query ( connection, sql ) )
    {
        printf ( "Query data failed (%s).\n", mysql_error ( connection ) );
        return false;
    }
    else
    {
        result = mysql_store_result ( connection );
        if ( !result )
        {
            printf ( "Couldn't get result from %s.\n", mysql_error ( connection ) );
            return false;
        }

        if ( row = mysql_fetch_row ( result ) )
        {
            max_value=atoi ( row[0] );
            return true;
        }
        return false;
    }
}

//获得某字段为某值的所有行数
bool MyDB::GetRowsNum ( char* table, char* field_value, int& num )
{
    char sql[1024];
    sprintf ( sql,"select * from %s where %s", table, field_value );
    if ( mysql_query ( connection, sql ) )
    {
        printf ( "Query data failed (%s).\n", mysql_error ( connection ) );
        return false;
    }
    else
    {
        result = mysql_store_result ( connection );
        if ( !result )
        {
            printf ( "Couldn't get result from %s.\n", mysql_error ( connection ) );
            return false;
        }
        num=mysql_affected_rows ( connection );
        return true;
    }
}

//将从原表中查询的某些数据复制到另一个表中
bool MyDB::CopyTable ( char* copy_table, char* prim_table, char* range )
{
    char sql[1024];
    sprintf ( sql,"insert into %s select * from %s where %s",copy_table,prim_table,range );
    if ( mysql_query ( connection, sql ) )  //执行SQL语句
    {
        printf ( "Copy table from '%s' to '%s' failed (%s).\n", prim_table, copy_table, mysql_error ( connection ) );
        return false;
    }
    else
    {
//         printf ( "Copy table from '%s' to '%s' success (%s).\n", prim_table, copy_table );
        return true;
    }
}

//导出表为txt文件
bool MyDB::ExpTable ( char* table, char* fields, char* save_name )
{
    char sql[1024];
    sprintf ( sql,"select %s from %s into outfile '/var/lib/mysql-files/%s.txt'",fields,table,save_name );
    if ( mysql_query ( connection, sql ) )  //执行SQL语句
    {
        printf ( "Saving '%s' failed (%s).\n", table, mysql_error ( connection ) );
        return false;
    }
    else
    {
        printf ( "Table %s has been saved as '/var/lib/mysql-files/%s.txt'.\n", table, save_name );
        return true;
    }
}

//获取当前日期
bool MyDB::GetDate ( std::string &current_date )
{
    if ( mysql_query ( connection, "SELECT CURRENT_DATE()" ) )
    {
        printf ( "Query data failed (%s).\n", mysql_error ( connection ) );
        return false;
    }
    else
    {
        result = mysql_store_result ( connection );
        if ( !result )
        {
            printf ( "Couldn't get result from %s.\n", mysql_error ( connection ) );
            return false;
        }

        if ( row = mysql_fetch_row ( result ) )
        {
            current_date = row[0];
            return true;
        }
        return false;
    }
}

//清空表
bool MyDB::TruncateTable ( char* table )
{
    char sql[1024];
    sprintf ( sql,"truncate table if exists %s",table );
    if ( mysql_query ( connection, sql ) )  //执行SQL语句
    {
        printf ( "Truncate table '%s' failed (%s).\n", table, mysql_error ( connection ) );
        return false;
    }
    else
    {
//         printf ( "Table '%s' has been truncated.\n", table );
        return true;
    }
}

//清空数据库
bool MyDB::TruncateDB ( char* database )
{
    char sql[1024];
    sprintf ( sql,"truncate table if exists %s",database );
    if ( mysql_query ( connection, sql ) )  //执行SQL语句
    {
        printf ( "Truncate database '%s' failed (%s).\n", database, mysql_error ( connection ) );
        return false;
    }
    else
    {
//         printf ( "Database '%s' has been truncated.\n", database );
        return true;
    }
}

//删除表
bool MyDB::DropTable ( char* table )
{
    char sql[1024];
    sprintf ( sql,"drop table if exists %s",table );
    if ( mysql_query ( connection, sql ) )  //执行SQL语句
    {
        printf ( "Drop table '%s' failed (%s).\n", table, mysql_error ( connection ) );
        return false;
    }
    else
    {
//         printf ( "Table '%s' has been droped.\n", table );
        return true;
    }
}

//删除数据库
bool MyDB::DropDB ( char* database )
{
    char sql[1024];
    sprintf ( sql,"drop table if exists %s",database );
    if ( mysql_query ( connection, sql ) )  //执行SQL语句
    {
        printf ( "Drop database '%s' failed (%s).\n", database, mysql_error ( connection ) );
        return false;
    }
    else
    {
//         printf ( "Database '%s' has been droped.\n", database );
        return true;
    }
}

//设置utf8编码
bool MyDB::setUTF8()
{
    if ( !mysql_query ( connection, "set character_set_client = utf8" ) )
        if ( !mysql_query ( connection, "set character_set_server = utf8" ) )
            if ( !mysql_query ( connection, "set character_set_connection = utf8" ) )
                if ( !mysql_query ( connection, "set character_set_database = utf8" ) )
                    if ( !mysql_query ( connection, "set character_set_results = utf8" ) )
                        if ( !mysql_query ( connection, "set collation_connection = utf8_general_ci" ) )
                            if ( !mysql_query ( connection, "set collation_database = utf8_general_ci" ) )
                                if ( !mysql_query ( connection, "set collation_server = utf8_general_ci" ) )
                                {
                                    return true;
                                }

    printf ( "Set utf8 failed (%s).\n",mysql_error ( connection ) );
    return false;
}

//获得查询结果
bool MyDB::getResult()
{
    result = mysql_store_result ( connection );
    if ( !result )
    {
        printf ( "Couldn't get result from %s.\n", mysql_error ( connection ) );
        return false;
    }

//     printf ( "number of dataline returned: %llu.\n", mysql_affected_rows ( connection ) );

    // 获取列数
    int j = mysql_num_fields ( result );

    //存储字段信息
    char *str_field[1024];

    //获取字段名
    for ( int i = 0; i < mysql_num_fields ( result ); i++ )
    {
        str_field[i] = mysql_fetch_field ( result )->name;
    }

    //打印字段
    for ( int i = 0; i < mysql_num_fields ( result ); i++ )
    {
        printf ( "%-10s\t", str_field[i] );
    }
    printf ( "\n" );

    while ( row = mysql_fetch_row ( result ) )
    {
        // 获取列数
        for ( int i=0; i<j; i++ )
        {
            printf ( "%-10s\t", row[i] );
        }
        printf ( "\n" );
    }
    cout << endl;
    return true;
}

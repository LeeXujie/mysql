#include "mytable.h"

//连接数据库并建表
MyTable::MyTable ( char* _table, bool _delOldTab ) : table ( _table ), tmpTable ( "tmpTable" ), checknum ( -1 ) //有一些关键字不能作为table的值，比如"table"
{
    if ( !InitDB ( "localhost", "root", "123", "test" ) ) //这里需要根据实际情况进行修改
    {
        exit ( 1 );
    }
    if ( _delOldTab )
    {
        DropTable ( table );
    }
    char* fields="序号 BIGINT UNSIGNED PRIMARY KEY AUTO_INCREMENT, 位置号 VARCHAR(20), 板卡号 VARCHAR(20), 标准图 VARCHAR(20), 缺陷图 VARCHAR(20), 状态 VARCHAR(20), 是否误检 VARCHAR(20), INDEX indexName (状态(10))";
    CreateTable ( table,fields );
}

//断开连接
MyTable::~MyTable() {}

//插入数据
bool MyTable::InsertData()
{
    char sql[256];
    sprintf ( sql,"NULL,'%s','%s','%s','%s','%s',NULL",placeNum,boardNum,stdImg,defectImg,status );
    if ( MyDB::InsertData ( table, sql ) )
    {
        return true;
    }
    else
    {
        return false;
    }
}

//查询数据（按列）
bool MyTable::QueryData ( char* columns )
{
    if ( QueryData1 ( table,columns ) )
    {
        return true;
    }
    else
    {
        return false;
    }
}

//查询数据（按行）
bool MyTable::QueryData ( char* field_name, char* field_value )
{
    if ( QueryData2 ( table,field_name,field_value ) )
    {
        return true;
    }
    else
    {
        return false;
    }
}

//查询数据（某行中的某个值）
bool MyTable::QueryData ( char* query_name, char* field_name, char* field_value )
{
    if ( QueryData3 ( table,query_name,field_name,field_value ) )
    {
        return true;
    }
    else
    {
        return false;
    }
}

//查看有缺陷的元器件
bool MyTable::CheckIssues()
{
    if ( checknum<0 )
    {
        char* fields="序号 INT UNSIGNED, 位置号 VARCHAR(20), 板卡号 VARCHAR(20), 标准图 VARCHAR(20), 缺陷图 VARCHAR(20), 状态 VARCHAR(20), 是否误检 VARCHAR(20)";
        if ( !CreateTmpTable ( tmpTable, fields ) )
        {
            return false;
        }
        if ( !CopyTable ( tmpTable,table,"状态!='正常' and 是否误检 is null" ) )
        {
            return false;
        }
    }
    else//从临时表中挨个查看
    {
        char sql[256];
        sprintf ( sql,"select * from %s limit %d,1", tmpTable, checknum );
        if ( !QueryInfo ( sql ) )
        {
            return false;
        }
        if ( GetValue ( tmpTable,"序号",checkid,checknum ) )
        {
            sprintf ( id, "%s", checkid.c_str() );
        }
        else
        {
            return false;
        }
    }
    checknum++;
    return true;
}

//设置误检位（误检设为1，否则设为0）
bool MyTable::SetFalseDetect ( bool falseDetect )
{
    char* yesOrNot;
    if ( falseDetect )
    {
        yesOrNot="是";
    }
    else
    {
        yesOrNot="否";
    }
    if ( UpdateData ( table, "是否误检",yesOrNot,"序号",id ) )
    {
        return true;
    }
    else
    {
        return false;
    }
}

//统计误检率
bool MyTable::GetErrorRate()
{
    int totalNum,errorNum;
    if ( GetMaxPrimarykey ( table,"序号",totalNum ) )
    {
        if ( GetRowsNum ( table,"是否误检='是'",errorNum ) )
        {
	    float errorRate=(float)errorNum/totalNum;
	    std::cout<<"总数："<<totalNum<<std::endl;
	    std::cout<<"误检数："<<errorNum<<std::endl;
            std::cout<<"误检率："<<errorRate<<std::endl;
            return true;
        }
    }
    return false;
}

/* -- START -- */
/*
 *代码借用：http://www.cnblogs.com/sardine/archive/2011/07/30/2121853.html
 */
#include <sqlite3.h>
#include <stdio.h>

static int _sql_callback(void * notused, int argc, char ** argv, char ** szColName)
{
		int i;
		for ( i=0; i < argc; i++ )
		{
				printf( "%s = %s\n", *szColName, argv == 0 ? "NUL" : *argv );
		}
		return 0;
}


int main()
{
    const char * sSQL1 = "create table users(userid varchar(20) PRIMARY KEY,age int,regtime datetime);";
    const char * sSQL2 = "insert into users values('guilh',29,'2009-7-16');";
    const char * sSQL3 = "select * from users;";

		sqlite3 * db = 0;
		char * pErrMsg = 0;

		int ret = 0;
		const char* const dbname = "gtest.db";
    
		//打开数据库，如果数据库不存在，会建立一个数据库
    ret = sqlite3_open(dbname, &db);

    if ( ret != SQLITE_OK )
    {
        printf("open error! : %s", sqlite3_errmsg(db));
        return(1);
    }

    printf("open db OK!\n");

    // 执行建表SQL
    sqlite3_exec( db, sSQL1, 0, 0, &pErrMsg );
    if ( ret != SQLITE_OK )
    {
        //fprintf(stderr, "SQL error: %s\n", pErrMsg);
        printf("SQL error: %s\n", pErrMsg);
        sqlite3_free(pErrMsg);
    }

    // 执行插入记录SQL
    sqlite3_exec( db, sSQL2, 0, 0, &pErrMsg);

    // 查询数据表
    sqlite3_exec( db, sSQL3, _sql_callback, 0, &pErrMsg);

    // 关闭数据库
    sqlite3_close(db);
    db = 0;

    return 0;
}
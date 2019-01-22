#include <stdio.h>
#include <mysql.h>
int main(int argc, const char * argv[])
{
	MYSQL *connection, mysql;
	mysql_init(&mysql);
	connection = mysql_real_connect(&mysql,"localhost","root","1!Gzg19841221","test",0,0,0);
	if (connection == NULL)
	{
		printf("连接失败:%s\n", mysql_error(&mysql));
	}
	else
	{
		printf("连接成功, 服务器版本: %s, 客户端版本: %s.\n", MYSQL_SERVER_VERSION, mysql_get_client_info());
		if(mysql_query(connection,"select * from test"))
		{
			printf("查询失败:%s",mysql_error(connection));
			return -1;
		}
		MYSQL_RES *result = mysql_store_result(connection);
		MYSQL_ROW row;
		while(1)
		{
			row = (MYSQL_ROW)mysql_fetch_row(result);
			if(!row)
				break;
			printf("%s \n",row[0]);
		}
		mysql_free_result(result);
		mysql_close(connection);
	}
	mysql_close(&mysql);
	return 0;
}


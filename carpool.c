/*****************************************************
copyright (C), 2014-2015, Lighting Studio. Co.,     Ltd. 
File name：
Author：Jerey_Jobs    Version:0.1    Date: 
Description：
Funcion List: 
*****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <time.h>
#include <string.h>
int flag = 0;
void open_db(sqlite3 ** db)
{
	char * errmsg;
	if(SQLITE_OK != sqlite3_open("park.db",db))
	{
		printf("open error!:errmsg");
		exit(-1);
	}
	else
	{
		printf("open sucess!\n");
	}
}
void create_table(sqlite3 * db)
{
	char * sql;
	char * errmsg;
	sql = "create table if not exists park (num txt primary key,intime txt,times integer);";
	if(SQLITE_OK != sqlite3_exec(db,sql,NULL,NULL,&errmsg))
	{
		printf("create table error!:%s",errmsg);
		exit(-1);
	}
}
void car_in(sqlite3 *db)
{
	char  sql[100];
	char * errmsg;
	char num[10];
	printf("请输入车牌号\n");
	scanf("%s",num);
	getchar();

	char intime[100] ="0";
	char tmp[100]= "0";
	long times;


	time_t timer;
	struct tm *tblock;
	timer = time(NULL);
	times = timer;
	tblock = localtime(&timer);
	strcpy(tmp,asctime(tblock));
	strncpy(intime,tmp,strlen(tmp)-1);
//    strcpy(intime,asctime(tblock));
	

	sprintf(sql,"insert into park (num,intime,times) values ('%s','%s',%d);",num,intime,times);
	if(SQLITE_OK != (sqlite3_exec(db,sql,NULL,NULL,&errmsg)))
	{
		printf("car in error!:%s",errmsg);
		exit(-1);
	}
	printf("入场成功\n");
}
int change(char * p)
{
	char *tmp;
	tmp = p;
	int temp = 0;
	while((*tmp) != '\0')
	{
		temp = temp*10 +((*tmp) - '0');
		tmp++;
	}
	return temp;
}
void find_car(sqlite3 * db)
{
	char num[10];
	printf("需要查询的车号:\n");
	scanf("%s",num);
	getchar();
	char * errmsg;
	char *sql;
	char **result;
	int n_row;
	int n_col;
	sprintf(sql,"select * from park where num = '%s';",num);

	if(SQLITE_OK != sqlite3_get_table(db,sql,&result,&n_row,&n_col,&errmsg))
	{
		printf("times error!:%s",errmsg);
		exit(-1);
	}
	printf("当前车：%s\n",result[3]);
	printf("入场时间：%s\n",result[4]);
    long intime = (change(result[5]));
	time_t outtime;
	outtime = time(NULL);
	float cost =( (outtime - intime) / 1200+ 1 )* 1.5;
	printf("需要收费%3.1f圆\n",cost);
	sqlite3_free_table(result);
}
void car_out(sqlite3 * db)
{
	char num[10];
	printf("需要出场的车号:\n");
	scanf("%s",num);
	getchar();
	char * errmsg;
	char sql[100];
	char **result;
	int n_row;
	int n_col;
	sprintf(sql,"select * from park where num = '%s';",num);

	if(SQLITE_OK != sqlite3_get_table(db,sql,&result,&n_row,&n_col,&errmsg))
	{
		printf("times error!:%s",errmsg);
		exit(-1);
	}
	printf("当前车：%s\n",result[3]);
	printf("入场时间：%s\n",result[4]);
    long intime = (change(result[5]));
	time_t outtime;
	outtime = time(NULL);
	float cost = ((outtime - intime) /1200 + 1) * 1.5;
	printf("收费%3.1f圆\n",cost);
	sqlite3_free_table(result);
	sprintf(sql,"delete from park where num = '%s';",num);
	if(SQLITE_OK != sqlite3_exec(db,sql,NULL,NULL,&errmsg))
	{
		printf("delete error!:%s",errmsg);
		exit(-1);
	}

}
int display(void * para,int n_col,char**col_value,char **col_name)
{
	int i;
	if(flag ==0)
	{
		for(i = 0;i < n_col;i++)
		{
			printf("%20s",col_name[i]);
		}
		printf("\n");
		flag++;
	}
	for(i = 0 ;i <  n_col;i++)
	{
		printf("%20s\t",col_name[i]);
	}
	printf("\n");
	return 0;
}
void display_all(sqlite3 * db)
{
	int i;
	char * sql;
	char * errmsg;
	char **result;
	int n_row;
	int n_col;
	sql = "select * from park;";
	if(SQLITE_OK != (sqlite3_get_table(db,sql,&result,&n_row,&n_col,&errmsg)))
	{
		printf("display all error!:%s",errmsg);
		exit(-1);
	}
	printf("             共有%d辆车\n",n_row);
	for(i = 0;i<(n_row + 1) * n_col;i++ )
	{
		if( i % n_col == 0)
		{
			printf("%10s",result[i]);
		}
		if(i % n_col == 1)
		{
			printf("%25s",result[i]);
		}
	/*	if(i % n_col == 2)
		{
			printf("%10s",result[i]);
		}*/
		
		if((i + 1) % n_col == 0)
		{
			printf("\n");
		}
	}
	sqlite3_free_table(result);
}

void main_interface(sqlite3 * db)
{
	int main_i = 0;
	while(main_i != 5)
	{
		printf("********************\n");
		printf("1.车入场\n");
		printf("2.车出场\n");
		printf("3.所有车辆查询\n");
		printf("4.查询车辆信息\n");
		printf("5.退出系统\n");
		printf("请输入操作代码：\n");
		scanf("%d",&main_i);
		getchar();
		switch(main_i)
		{
			case 1 :car_in(db);
					break;
			case 2 :car_out(db);
					break;
			case 3 :display_all(db);
			        break;
			case 4 :find_car(db);
					break;
			case 5 :break;
		}
	}
}
int main()
{
	sqlite3 * db =NULL;
	open_db(&db);
	create_table(db);
	main_interface(db);
    return 0;
}

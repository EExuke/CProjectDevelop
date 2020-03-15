#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <mysql.h>
#include "resource.h"
#pragma  comment(lib,"libmysql.lib")
#pragma comment(lib,"comctl32.lib")
/*全局变量宏,方便全局变量的定义和使用*/
#ifdef _Utility_GLOBAL_ 
#define _LIB_EXT_  
#else
#define _LIB_EXT_ extern
#endif
	
#define TITLE_LENTH_MAX 20	   //ListView控件标题字符数最大值
#define	FIELD_LENTH_MAX 20     //字段名称字符数最大值
#define VALUE_LENTH_MAX 25	   //字段值字符数最大值
#define SQL_LENTH_MAX 300	  //SQL语句字符数最大值



#define DB_CHAR 1
#define DB_INT 2

_LIB_EXT_ MYSQL mysql;
_LIB_EXT_ MYSQL_RES *result;
_LIB_EXT_ MYSQL_ROW row;
_LIB_EXT_ HINSTANCE g_hInstance;
_LIB_EXT_ int g_level;
_LIB_EXT_ char g_operator[VALUE_LENTH_MAX];
#ifndef _LIB_CM_
#define _LIB_CM_
typedef struct CM
{
	char value[VALUE_LENTH_MAX];
	char fieldName[FIELD_LENTH_MAX];
	int type;
}ColumnMessage;
#endif
void InitListViewColumns(HWND hView,char (*titles)[TITLE_LENTH_MAX],int nums);
void QueryRecordToView(HWND hView,char* pTbName,int nums,char* condition,int clear,int offset);
void DeleteFromListView(HWND hView,char* tb_name,char* primary,int db_type);
BOOL InsertData(char *tb_name,char *field_names,char *values);
BOOL FomatCMInsert(ColumnMessage* cms,int nums,char* fields,char* values);
BOOL FomatCMUpdate(ColumnMessage* cms,int nums,char *condition);
BOOL HoldInsertIDCondition(char* condition,char * prim);
void UpDateDataFromListView(HWND hView,char *tb_name,char *sets,int nums,char *primary,int db_type);
void CreateSubViewProc(HWND hDlg,HWND* pView,char *caption,char (*titles)[FIELD_LENTH_MAX],int count,int viewIndex);
void SetSubViewFromEdit(HWND hEdit,HWND subView,char* subTBNAME,char* fk,int count);

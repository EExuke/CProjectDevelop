#define _Utility_GLOBAL_ 
/*进入全局变量宏判断之前定义全局变量宏*/
#include "Utility.h"
void InitListViewColumns(HWND hView,char (*titles)[FIELD_LENTH_MAX],int nums)
{
	LVCOLUMN lvColumn; 
	int index;
	 lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT ;
	 if(nums == 1)
	 {
		lvColumn.fmt = LVCFMT_CENTER;
		lvColumn.cx = 320;
	 }
	 else
	 {
		lvColumn.fmt = LVCFMT_LEFT;
		 lvColumn.cx = 180;
	 }
	for (index = 0; index < nums; index++)
  {
    lvColumn.pszText = titles[index];
    ListView_InsertColumn (hView, index, &lvColumn);
  }
}
void QueryRecordToView(HWND hView,char* pTbName,int nums,char* condition,int clear,int offset)
{	
	char sql[SQL_LENTH_MAX];
    int r_nums; 
	LVITEM lvItem;
	int index;
	int array;
	if(clear)
	{
		SendMessage(hView,LVM_DELETEALLITEMS,0,0);
	}
	if((condition==NULL)||(strlen(condition)==0))
	{
		
		sprintf(sql,"select * from %s",pTbName);
	}
	else
	{
		sprintf(sql,"select * from %s where %s ",pTbName,condition);
	}
	mysql_query(&mysql,sql);
	if(mysql_errno(&mysql))
	{
		MessageBox(GetParent(hView),"操作错误","提示",MB_ICONERROR);
		return;
	}
	result = mysql_store_result(&mysql);
	r_nums = (int)mysql_num_rows(result);
	lvItem.mask = LVIF_TEXT;	
	lvItem.cchTextMax = VALUE_LENTH_MAX;	
	for(index = 0;index<r_nums;index++)
	{
		lvItem.iItem = index;
		row = mysql_fetch_row(result);	
		lvItem.iSubItem = 0;
		lvItem.pszText = "";
		ListView_InsertItem (hView,&lvItem);
		for(array = 0;array<nums;array++)
		{
			//lvItem.cColumns = array;   //在WINXP的win api 中，结构体LVITEM包含cColumns表示列,但在vc98中不支持。读者可以查阅MSDN当中的LVITEM相关项来获得详细情况
			ListView_SetItemText(hView,index,array,row[array+offset]);
		}
	}
	mysql_free_result(result);
}
void DeleteFromListView(HWND hView,char* tb_name,char* primary,int db_type)
{

	int index = SendMessage(hView,LVM_GETSELECTIONMARK,0,0);
	char tb_prim[VALUE_LENTH_MAX];
	char sql[SQL_LENTH_MAX];
	if(index == -1)
	{
		MessageBoxEx(GetParent(hView),"请在列表中选中需要删除的数据。","确认信息",MB_OK|MB_TOPMOST,0);
		return;
	}
	if(IDOK!=MessageBoxEx(GetParent(hView),"确定删除该项?","确认信息",MB_OKCANCEL|MB_TOPMOST,0))
	{
		return;
	}
	ListView_GetItemText(hView,index,0,tb_prim,VALUE_LENTH_MAX);
	if(db_type == DB_CHAR)
	{
		sprintf(sql,"delete from %s where %s='%s'",tb_name,primary,tb_prim);
	}
	if(db_type == DB_INT)
	{
		sprintf(sql,"delete from %s where %s=%s",tb_name,primary,tb_prim);
	}
	mysql_query(&mysql,sql);

	ListView_DeleteItem(hView,index);
	MessageBoxEx(hView,"删除成功","提示",MB_OK|MB_TOPMOST,0);
}
BOOL InsertData(char *tb_name,char *field_names,char *values)
{
	char sql[SQL_LENTH_MAX];
	sprintf(sql,"insert into %s(%s) values(%s)",tb_name,field_names,values);
	mysql_query(&mysql,sql);
	if(mysql_errno(&mysql))
	{
		return 0;
	}
	return 1;
}
BOOL FomatCMInsert(ColumnMessage* cms,int nums,char* fields,char* values)
{
	int b = 0;
	int i;

			for(i=0;i<nums;i++)
			{
				if(strlen(cms[i].value)>0)
				{
					if(b==0)
					{
						if(cms[i].type == DB_INT)
						{
							sprintf(values,"%s",cms[i].value);
						}
						else if(cms[i].type==DB_CHAR)
						{
							sprintf(values,"'%s'",cms[i].value);
						}
						sprintf(fields,"%s",cms[i].fieldName);
						/*产生了第一个受到影响的列*/
						b++;
					}
					else
					{
						if(cms[i].type == DB_INT)
						{
							sprintf(values,"%s,%s",values,cms[i].value);
						}
						else if(cms[i].type==DB_CHAR)
						{
							sprintf(values,"%s,'%s'",values,cms[i].value);
						}	
						sprintf(fields,"%s,%s",fields,cms[i].fieldName);
					}
				
				}
			}
			return b;
}
BOOL FomatCMUpdate(ColumnMessage* cms,int nums,char *condition)
{
	int updateFlag = 0;
	int i;
		for(i=0;i<nums;i++)
			{
				if(strlen(cms[i].value)>0)
				{
					
					if(updateFlag == 0)
					{
						if(cms[i].type == DB_INT)
						{
							sprintf(condition,"%s=%s",cms[i].fieldName,cms[i].value);
						}
						else if(cms[i].type==DB_CHAR)
						{
							sprintf(condition,"%s='%s'",cms[i].fieldName,cms[i].value);
						}
					}
					else
					{
						if(cms[i].type == DB_INT)
						{
							sprintf(condition,"%s,%s=%s",condition,cms[i].fieldName,cms[i].value);
						}
						else if(cms[i].type==DB_CHAR)
						{
							sprintf(condition,"%s,%s='%s'",condition,cms[i].fieldName,cms[i].value);
						}
					}
					updateFlag++;
				}
			}
		return updateFlag;
}
BOOL HoldInsertIDCondition(char* condition,char * prim)
{
		mysql_query(&mysql,"SELECT LAST_INSERT_ID()");
		if(mysql_errno(&mysql))
		{
			return 0;
		}
		result = mysql_store_result(&mysql);
		row = mysql_fetch_row(result);
		sprintf(condition,"%s=%s",prim,row[0]);
		return 1;
}

void UpDateDataFromListView(HWND hView,char *tb_name,char *sets,int nums,char *primary,int db_type)
{
	char sql[SQL_LENTH_MAX];
	char tb_prim[VALUE_LENTH_MAX];
	int index;
	index = SendMessage(hView,LVM_GETSELECTIONMARK,0,0);
	if(index==-1)
	{
		MessageBoxEx(GetParent(hView),"请选中需要修改的数据","提示",MB_ICONERROR|MB_TOPMOST,0);
		return;
	}
	ListView_GetItemText(hView,index,0,tb_prim,VALUE_LENTH_MAX);
	if(db_type == DB_CHAR)
	{
		sprintf(sql,"update %s set %s where %s='%s'",tb_name,sets,primary,tb_prim);
	}
	if(db_type == DB_INT)
	{
		sprintf(sql,"update %s set %s where %s=%s",tb_name,sets,primary,tb_prim);
	}
	mysql_query(&mysql,sql);
	if(mysql_errno(&mysql))
	{
		MessageBoxEx(GetParent(hView),"操作错误","提示",MB_ICONERROR|MB_TOPMOST,0);
		return;
	}
	QueryRecordToView(hView,tb_name,nums,"",1,0);
	MessageBox(hView,"修改成功","提示",MB_OK);
}
void CreateSubViewProc(HWND hDlg,HWND* pView,char *caption,char (*titles)[FIELD_LENTH_MAX],int count,int viewIndex)
{
	RECT rect;
	POINT point;
	int length;
	if(IsWindow(*pView)==0)
	{
		GetWindowRect(hDlg,&rect);
		point.x = rect.right+400*(viewIndex/2);
		point.y = rect.top+(viewIndex%2)*(rect.bottom-rect.top)/2;
		length = (rect.bottom-rect.top)/2;
		*pView = CreateWindowEx(WS_EX_TRANSPARENT,WC_LISTVIEW,caption,WS_TILEDWINDOW|LVS_ALIGNTOP|LVS_REPORT|LVS_SINGLESEL,point.x,point.y,400,length,hDlg,0,g_hInstance,0);
		SendMessage(*pView,LVM_SETEXTENDEDLISTVIEWSTYLE,0,LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP|LVS_EX_GRIDLINES|LVS_EX_ONECLICKACTIVATE|LVS_EX_FLATSB);
		InitListViewColumns(*pView,titles,count);
		ShowWindow(*pView,SW_SHOW);
		SendMessage(hDlg,WM_SETFOCUS,(WPARAM)*pView,0);
	}
	
}
void SetSubViewFromEdit(HWND hEdit,HWND subView,char* subTBName,char* fk,int count)
{
	char condition[SQL_LENTH_MAX];
	char text[VALUE_LENTH_MAX];
	GetWindowText(hEdit,text,VALUE_LENTH_MAX);
	if(strlen(text)>2)
	{
		sprintf(condition,"%s Like\"%s%s\"",fk,text,"___%");
		QueryRecordToView(subView,subTBName,count,condition,1,0);
	}
}
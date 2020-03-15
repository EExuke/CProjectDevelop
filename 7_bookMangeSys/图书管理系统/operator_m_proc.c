#include "operator_m_proc.h"
#define FIELD_NUM 3  //表的字段数
#define PRIMARY "name"   //表的key
#define TB_NAME "tb_operators"
BOOL CALLBACK Operator_M_Proc(HWND hDlg, UINT message,WPARAM wParam, LPARAM lParam)
{
	char titles[FIELD_NUM][TITLE_LENTH_MAX] = {"操作员登陆名","操作员密码","操作员等级"};
	HWND hView = GetDlgItem(hDlg,IDC_LIST_VIEW);
	ColumnMessage cms[FIELD_NUM];
	char values[VALUE_LENTH_MAX*FIELD_NUM+FIELD_NUM-1];	 //inser语句中的值,数组大小为：值总长度和逗号数量加和的最大值
	char fields[FIELD_LENTH_MAX*FIELD_NUM+FIELD_NUM-1];  //inser语句中的字段,数组大小为：字段总长度和逗号数量加和的最大值
	int selIndex;													 //控件当前被选中的项
	int updateFlag;													//更新标识，当没有参数需要更新时，flag为0
	char sets[(VALUE_LENTH_MAX+FIELD_LENTH_MAX)*(FIELD_NUM)+(FIELD_NUM-1)*2+1] = "\0"; //Update语句中SET的内容，数组大小为：值和字段的总长度加上逗号、加号的数量 
	switch (message)    
	{
	case   WM_INITDIALOG : 
		InitCommonControls();	
		SendMessage(hView,LVM_SETEXTENDEDLISTVIEWSTYLE,0,LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP|LVS_EX_GRIDLINES|LVS_EX_ONECLICKACTIVATE|LVS_EX_FLATSB);
		InitListViewColumns(hView,titles,FIELD_NUM );
		QueryRecordToView(hView,TB_NAME,FIELD_NUM,"",1,0);
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_LEVEL),CB_ADDSTRING,0,(LPARAM)"1");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_LEVEL),CB_ADDSTRING,0,(LPARAM)"2");
		return TRUE ;
	case   WM_COMMAND :
		switch(LOWORD(wParam))
		{
		case IDC_BUTTON_ADD:
			sprintf(cms[0].fieldName,"name");
			sprintf(cms[1].fieldName,"password");
			sprintf(cms[2].fieldName,"level");
			cms[0].type = DB_CHAR;
			cms[1].type = DB_CHAR;
			cms[2].type = DB_INT;
			GetDlgItemText(hDlg,IDC_EDIT_NAME,cms[0].value,VALUE_LENTH_MAX);
			if(strlen(cms[0].value)<1)
			{
				MessageBox(hDlg,"请输入操作员名称","提示",MB_ICONHAND);
				return 0;
			}
			GetDlgItemText(hDlg,IDC_EDIT_PWD,cms[1].value,VALUE_LENTH_MAX);
			selIndex = SendMessage(GetDlgItem(hDlg,IDC_COMBO_LEVEL),CB_GETCURSEL,0,0);
			if(selIndex == -1)
			{
				strcpy(cms[2].value,"");
			}
			else
			{
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_LEVEL),CB_GETLBTEXT,selIndex,(LPARAM)cms[2].value);
			}
			/*格式化字符串*/
			FomatCMInsert(cms,FIELD_NUM,fields,values);
			/*对信息表进行操作*/
			if(!InsertData(TB_NAME,fields,values))
			{
				MessageBoxEx(GetParent(hView),"操作错误","提示",MB_ICONERROR|MB_TOPMOST,0);
				mysql_query(&mysql,"ROLLBACK");
				return 0;
			}
		    QueryRecordToView(hView,TB_NAME,FIELD_NUM,"",1,0);
			MessageBox(hDlg,"添加成功","提示",MB_OK);
			break;
		case IDC_BUTTON_DELETE:
			DeleteFromListView(hView,TB_NAME,PRIMARY,DB_CHAR);
			break;
		case IDC_BUTTON_MODIFY:
			updateFlag = 0;
			sprintf(cms[0].fieldName,"name");
			sprintf(cms[1].fieldName,"password");
			sprintf(cms[2].fieldName,"level");
			cms[0].type = DB_CHAR;
			cms[1].type = DB_CHAR;
			cms[2].type = DB_INT;
			GetDlgItemText(hDlg,IDC_EDIT_NAME,cms[0].value,VALUE_LENTH_MAX);
			GetDlgItemText(hDlg,IDC_EDIT_PWD,cms[1].value,VALUE_LENTH_MAX);
			selIndex = SendMessage(GetDlgItem(hDlg,IDC_COMBO_LEVEL),CB_GETCURSEL,0,0);
			if(selIndex == -1)
			{
				strcpy(cms[2].value,"");
			}
			else
			{
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_LEVEL),CB_GETLBTEXT,selIndex,(LPARAM)cms[2].value);
			}
			/*格式化字符串*/
			if(FomatCMUpdate(cms,FIELD_NUM,sets)>0)
			{
				UpDateDataFromListView(hView,TB_NAME,sets,FIELD_NUM,PRIMARY,DB_CHAR);		//更新数据
			}
			else
			{
				MessageBox(hDlg,"修改信息不可全部为空!","提示",MB_ICONERROR);
			}		
			break;
		case ID_QUIT:
			EndDialog(hDlg,0);
			break;
		}
	   return TRUE ;
	case WM_NOTIFY:
		return 1;
	case WM_CLOSE: 
		EndDialog(hDlg,0);
	case  WM_DESTROY :
		break;
	   return 0 ;
    }
    return FALSE ;	
}

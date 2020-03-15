#include "desk_info_proc.h"
#define FIELD_NUM 1								//柜台基本表的字段数
#define PRIMARY "deskname"						//柜台基本表的key
#define TB_NAME "tb_base_desk_info"					//柜台基本表名
#define	EDIT_COUNT 1								//EDIT控件数量
#define	NO1_EDIT			IDC_EDIT_DESK
BOOL CALLBACK Desk_Info_Proc(HWND hDlg, UINT message,WPARAM wParam, LPARAM lParam)
{
	char titles[FIELD_NUM][TITLE_LENTH_MAX] = {"柜台名称"};
	HWND hView = GetDlgItem(hDlg,IDC_LIST_VIEW);
	ColumnMessage cms[FIELD_NUM];
	char values[VALUE_LENTH_MAX*FIELD_NUM+FIELD_NUM-1];										 //inser语句中的值,数组大小为：值总长度和逗号数量加和的最大值
	char fields[FIELD_LENTH_MAX*FIELD_NUM+FIELD_NUM-1];									   	//inser语句中的字段,数组大小为：字段总长度和逗号数量加和的最大值
	int selIndex;																			 //控件当前被选中的项
	int i;																					 //迭代器
	int updateFlag;																			//更新标识，当没有参数需要更新时，flag为0
	char sets[(VALUE_LENTH_MAX+FIELD_LENTH_MAX)*(FIELD_NUM)+(FIELD_NUM-1)*2+1] = "\0";		 //Update语句中SET的内容，数组大小为：值和字段的总长度加上逗号、加号的数量
	LPNMHDR pNmhdr;																			//NMHDR表示WM_NOTIFY消息内容,LPNMHDR则表示指向它的指针类型
	char temp[VALUE_LENTH_MAX];																//设置EDIT文字时所用的字符缓冲
	switch (message)						
	{
	case   WM_INITDIALOG : 
		InitCommonControls();	
		SendMessage(hView,LVM_SETEXTENDEDLISTVIEWSTYLE,0,LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP|LVS_EX_GRIDLINES|LVS_EX_ONECLICKACTIVATE|LVS_EX_FLATSB);
		InitListViewColumns(hView,titles,FIELD_NUM );
		QueryRecordToView(hView,TB_NAME,FIELD_NUM,"",1,0);
		return TRUE ;
	case   WM_COMMAND :
		switch(LOWORD(wParam))
		{
		case IDC_BUTTON_ADD:
			sprintf(cms[0].fieldName,"deskname");
			for(i=0;i<FIELD_NUM;i++)
			{
				cms[i].type = DB_CHAR;
			}
			GetDlgItemText(hDlg,IDC_EDIT_DESK,cms[0].value,VALUE_LENTH_MAX);
			FomatCMInsert(cms,FIELD_NUM,fields,values);
			/*对信息表进行操作*/
			if(!InsertData(TB_NAME,fields,values))
			{
				MessageBoxEx(GetParent(hView),"操作错误","提示",MB_ICONERROR|MB_TOPMOST,0);
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
			sprintf(cms[0].fieldName,"deskname");
			
			for(i =0;i<EDIT_COUNT;i++)
			{
	     		GetDlgItemText(hDlg,NO1_EDIT+i,cms[i].value,VALUE_LENTH_MAX);
			}
			for(i=0;i<FIELD_NUM;i++)
			{
				cms[i].type = DB_CHAR;
			}
			/*格式化字符串*/
			if(FomatCMUpdate(cms,FIELD_NUM,sets)>0)
			{
				UpDateDataFromListView(hView,TB_NAME,sets,FIELD_NUM,PRIMARY,DB_CHAR);
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
       break ;
	case WM_NOTIFY:
		if(wParam == IDC_LIST_VIEW)
		{
			pNmhdr = (LPNMHDR)lParam;
			if(NM_DBLCLK == pNmhdr->code)
			{
				selIndex = SendMessage(GetDlgItem(hDlg,IDC_LIST_VIEW),LVM_GETSELECTIONMARK,0,0);
				if(selIndex == -1)
				{
					return 0;
				}
				for(i = 0;i<EDIT_COUNT;i++)
				{
					ListView_GetItemText(hView,selIndex,i,temp,VALUE_LENTH_MAX);
					SetDlgItemText(hDlg,NO1_EDIT+i,temp);				//相连的EDIT ID
				}
			}
		}
		return 1;
	case  WM_CLOSE: 
		EndDialog(hDlg,0);
		//PostQuitMessage (0) ;
	case  WM_DESTROY :
		//PostQuitMessage (0) ;
		break;
	   return 0 ;
    }
    return FALSE ;	
}


